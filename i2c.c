#include <stm32.h>
#include <gpio.h>
#include "i2c.h"
#include "messages.h"
#include <string.h>

#define I2C_SPEED_HZ 100000
#define PCLK1_MHZ 16

#define _Q_LEN 1000
#define _MAX_WRITE_LEN 10

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

#define MAX_READ_LEN 100

typedef struct {
    int write;
    int read;
    char slave_address;
    char buffer_write[_MAX_WRITE_LEN];
    void (*callback)(char*);
} Transmission;

static Transmission trans_queue[_Q_LEN];
static int first = 0;
static int q_size = 0;

static int _write;
static int _read;
static char* _buffer_write;
static char _buffer_read[MAX_READ_LEN];
static char _slave_address;

static int _i_write;
static int _i_read;

static int _i2c_busy = 0;
static void (*_callback)(char*);

void i2c_write_read(int write,
                    int read,
                    char slave_address,
                    char* buffer_write,
                    void (*callback)(char*)) {
    // Turn off interrupts - add later

    int index = (first + q_size) % _Q_LEN;
    q_size++;
    trans_queue[index].write = write;
    trans_queue[index].read = read;
    trans_queue[index].slave_address = slave_address;
    trans_queue[index].callback = callback;
    memcpy(trans_queue[index].buffer_write, buffer_write, write);

    if (_i2c_busy) {
        return;
    }

    _i2c_busy = 1;

    // Turn on interrupts - add later

    _write = write;
    _read = read;
    _buffer_write = trans_queue[index].buffer_write;
    _slave_address = slave_address;
    _callback = trans_queue[index].callback;
    
    _i_write = 0;
    _i_read = 0;

    I2C1->CR2 |= I2C_CR2_ITEVTEN;
    I2C1->CR1 |= I2C_CR1_START;
}

static void start_next_transmission() {
    Transmission t = trans_queue[first];
    first = (first + 1) % _Q_LEN;
    q_size--;

    _write = t.write;
    _read = t.read;
    _buffer_write = t.buffer_write;
    _slave_address = t.slave_address;
    _callback = t.callback;
    
    _i_write = 0;
    _i_read = 0;

    I2C1->CR1 |= I2C_CR1_START;
}

static void handle_writing() {
    uint32_t SR1 = I2C1->SR1;
    if (SR1 & I2C_SR1_SB) {
        I2C1->DR = _slave_address << 1;
    } else if (SR1 & I2C_SR1_ADDR) {
        I2C1->SR2;
        if(_write > 1)
            I2C1->CR2 |= I2C_CR2_ITBUFEN; // Turn on buffer related interrupts
        I2C1->DR = _buffer_write[0];
    } else if(SR1 & I2C_SR1_BTF && _i_write == _write - 1) {
        _i_write++;
        if(_read == 0){
            I2C1->CR1 |= I2C_CR1_STOP;
            if(q_size > 0) {
                start_next_transmission();
            } else {
                I2C1->CR2 &= ~I2C_CR2_ITEVTEN;
                _i2c_busy = 0;
            }
        } else {
            I2C1->CR1 |= I2C_CR1_START;
        }
    } else if(SR1 & I2C_SR1_TXE && _i_write < _write - 1) {
        _i_write++;
        if(_i_write == _write - 1)
            I2C1->CR2 &= ~I2C_CR2_ITBUFEN; // Turn them off
        I2C1->DR = _buffer_write[_i_write];
    }
}

static void handle_reading() {
    uint32_t SR1 = I2C1->SR1;
    if (SR1 & I2C_SR1_SB) {
        I2C1->DR = _slave_address << 1 | 1;
        if(_i_read == _read - 1) 
            I2C1->CR1 &= ~I2C_CR1_ACK;
        else
            I2C1->CR1 |= I2C_CR1_ACK;
    } else if(SR1 & I2C_SR1_ADDR) {
        I2C1->CR2 |= I2C_CR2_ITBUFEN;
        I2C1->SR2;
        if(_read == 1)
            I2C1->CR1 |= I2C_CR1_STOP;
    } else if(SR1 & I2C_SR1_RXNE) {
        _buffer_read[_i_read] = I2C1->DR;
        _i_read++;
        if(_i_read == _read) {
            I2C1->CR2 &= ~I2C_CR2_ITBUFEN;
            if(_callback)
                _callback(_buffer_read);
            if(q_size > 0) {
                start_next_transmission();
            } else {
                I2C1->CR2 &= ~I2C_CR2_ITEVTEN;
                _i2c_busy = 0;
            }
        } else if(_i_read == _read - 1) {
            I2C1->CR1 &= ~I2C_CR1_ACK;
            I2C1->CR1 |= I2C_CR1_STOP;
        }
    }
}

void I2C1_EV_IRQHandler(void) {
    if(_i_write < _write) {
        handle_writing();
    }
    else if(_i_read < _read){
        handle_reading();
    }
}
