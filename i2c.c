#include <stm32.h>
#include <gpio.h>
#include "i2c.h"

#include <string.h>

#define I2C_SPEED_HZ 100000
#define PCLK1_MHZ 16

#define _Q_LEN 1000
#define _MAX_WRITE_LEN 10
#define MAX_READ_LEN 100

void i2c_enable() {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
    __NOP();

    GPIOafConfigure(GPIOB, 8, GPIO_OType_OD,
                    GPIO_Low_Speed, GPIO_PuPd_NOPULL,
                    GPIO_AF_I2C1);
    GPIOafConfigure(GPIOB, 9, GPIO_OType_OD,
                    GPIO_Low_Speed, GPIO_PuPd_NOPULL,
                    GPIO_AF_I2C1);

    I2C1->CR1 = 0;
    I2C1->CCR = (PCLK1_MHZ * 1000000) / (I2C_SPEED_HZ << 1);
    I2C1->CR2 = PCLK1_MHZ;
    I2C1->TRISE = PCLK1_MHZ + 1;

    NVIC_EnableIRQ(I2C1_EV_IRQn);

    I2C1->CR1 |= I2C_CR1_PE;
}

typedef struct {
    int write;
    int read;
    char slave_address;
    char buffer_write[_MAX_WRITE_LEN];
    void (*callback)(char*);
} Transmission;

// Queue for all transmissions' data.
static Transmission trans_queue[_Q_LEN];
static int first = 0;
static int q_size = 0;

// Values for current I2C transmission.
static int cur_write;
static int cur_read;
static char* cur_buffer_write;
static void (*cur_callback)(char*);
static char cur_slave_address;

// Iterators for write/read buffers.
static int i_write;
static int i_read;

// Values read from I2C slave.
static char cur_buffer_read[MAX_READ_LEN];

static int i2c_busy = 0;


void i2c_write_read(int write,
                    int read,
                    char slave_address,
                    char* buffer_write,
                    void (*callback)(char*)) {

    int index = (first + q_size) % _Q_LEN;
    q_size++;
    trans_queue[index].write = write;
    trans_queue[index].read = read;
    trans_queue[index].slave_address = slave_address;
    trans_queue[index].callback = callback;
    memcpy(trans_queue[index].buffer_write, buffer_write, write);

    if (i2c_busy)
        return;

    i2c_busy = 1;

    first = (first + 1) % _Q_LEN;
    q_size--;

    cur_write = write;
    cur_read = read;
    cur_buffer_write = trans_queue[index].buffer_write;
    cur_slave_address = slave_address;
    cur_callback = trans_queue[index].callback;
    
    i_write = 0;
    i_read = 0;

    I2C1->CR2 |= I2C_CR2_ITEVTEN;
    I2C1->CR1 |= I2C_CR1_START;
}

static void start_next_transmission() {
    Transmission t = trans_queue[first];
    first = (first + 1) % _Q_LEN;
    q_size--;

    cur_write = t.write;
    cur_read = t.read;
    cur_buffer_write = t.buffer_write;
    cur_slave_address = t.slave_address;
    cur_callback = t.callback;
    
    i_write = 0;
    i_read = 0;

    I2C1->CR1 |= I2C_CR1_START;
}

static void handle_writing() {
    uint32_t SR1 = I2C1->SR1;
    if (SR1 & I2C_SR1_SB) {
        I2C1->DR = cur_slave_address << 1;
    } else if (SR1 & I2C_SR1_ADDR) {
        I2C1->SR2;
        if(cur_write > 1)
            I2C1->CR2 |= I2C_CR2_ITBUFEN;
        I2C1->DR = cur_buffer_write[0];
    } else if(SR1 & I2C_SR1_BTF && i_write == cur_write - 1) {
        i_write++;
        if(cur_read == 0){
            I2C1->CR1 |= I2C_CR1_STOP;
            if(q_size > 0) {
                start_next_transmission();
            } else {
                I2C1->CR2 &= ~I2C_CR2_ITEVTEN;
                i2c_busy = 0;
            }
        } else {
            I2C1->CR1 |= I2C_CR1_START;
        }
    } else if(SR1 & I2C_SR1_TXE && i_write < cur_write - 1) {
        i_write++;
        if(i_write == cur_write - 1)
            I2C1->CR2 &= ~I2C_CR2_ITBUFEN;
        I2C1->DR = cur_buffer_write[i_write];
    }
}

static void handle_reading() {
    uint32_t SR1 = I2C1->SR1;
    if (SR1 & I2C_SR1_SB) {
        I2C1->DR = cur_slave_address << 1 | 1;
        if(i_read == cur_read - 1) 
            I2C1->CR1 &= ~I2C_CR1_ACK;
        else
            I2C1->CR1 |= I2C_CR1_ACK;
    } else if(SR1 & I2C_SR1_ADDR) {
        I2C1->CR2 |= I2C_CR2_ITBUFEN;
        I2C1->SR2;
        if(cur_read == 1)
            I2C1->CR1 |= I2C_CR1_STOP;
    } else if(SR1 & I2C_SR1_RXNE) {
        cur_buffer_read[i_read] = I2C1->DR;
        i_read++;
        if(i_read == cur_read) {
            I2C1->CR2 &= ~I2C_CR2_ITBUFEN;
            if(cur_callback)
                cur_callback(cur_buffer_read);
            if(q_size > 0) {
                start_next_transmission();
            } else {
                I2C1->CR2 &= ~I2C_CR2_ITEVTEN;
                i2c_busy = 0;
            }
        } else if(i_read == cur_read - 1) {
            I2C1->CR1 &= ~I2C_CR1_ACK;
            I2C1->CR1 |= I2C_CR1_STOP;
        }
    }
}

void I2C1_EV_IRQHandler(void) {
    if(i_write < cur_write) {
        handle_writing();
    }
    else if(i_read < cur_read){
        handle_reading();
    }
}
