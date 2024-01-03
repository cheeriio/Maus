#include <stm32.h>
#include <gpio.h>
#include "i2c.h"
#include "messages.h"

#define I2C_SPEED_HZ 100000
#define PCLK1_MHZ 16

// Initial version without using interrupts.
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
    // I2C1->CR2 = (PCLK1_MHZ | I2C_CR2_ITERREN | I2C_CR2_ITEVTEN);
    I2C1->TRISE = PCLK1_MHZ + 1;

    NVIC_EnableIRQ(I2C1_EV_IRQn);

    I2C1->CR1 |= I2C_CR1_PE;
}

#define MAX_READ_LEN 100

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
    if (_i2c_busy) {
        send_message("I2C busy, returned\r\n");
        return;
    }
    _i2c_busy = 1;

    _write = write;
    _read = read;
    _buffer_write = buffer_write;
    _slave_address = slave_address;
    _callback = callback;
    
    _i_write = 0;
    _i_read = 0;


    if(_write == 0 && _read == 0)
        return;

    I2C1->CR2 |= I2C_CR2_ITERREN | I2C_CR2_ITEVTEN;
    I2C1->CR1 |= I2C_CR1_START;

    int x = 0;
    while(_i2c_busy) {
        x = (x + 1) % 20000000;
        if(x % 250000 == 0)
            send_message("kuku\r\n");
    } // Makes whole point of using interrupts invalid. Later will add buffer for the i2c_write_read calls.
    send_message("Exited the loop\r\n");
    return;
}

static char hex(unsigned char x) {
    switch (x)
    {
        case 0: return '0';
        case 1: return '1';
        case 2: return '2';
        case 3: return '3';
        case 4: return '4';
        case 5: return '5';
        case 6: return '6';
        case 7: return '7';
        case 8: return '8';
        case 9: return '9';
        case 10: return 'A';
        case 11: return 'B';
        case 12: return 'C';
        case 13: return 'D';
        case 14: return 'E';
        case 15: return 'F';
        default: return '.';
    }
}

static void char_to_hex(unsigned char x, char* buf) {
    unsigned char y = x % 16;
    buf[0] = hex(x / 16);
    buf[1] = hex(y);

}

void print_interrupt(uint32_t SR1) {
    if(SR1 & I2C_SR1_SB)
        send_message("\tI2C_SR1_SB\r\n");
    if(SR1 & I2C_SR1_ADDR)
        send_message("\tI2C_SR1_ADDR\r\n");
    if(SR1 & I2C_SR1_RXNE)
        send_message("\tI2C_SR1_RXNE\r\n");
    if(SR1 & I2C_SR1_BTF)
        send_message("\tI2C_SR1_BTF\r\n");
    if(SR1 & I2C_SR1_TXE)
        send_message("\tI2C_SR1_TXE\r\n");
    if(SR1 & I2C_SR1_STOPF)
        send_message("\tI2C_SR1_STOPF\r\n");
    if(SR1 & I2C_SR1_ADD10)
        send_message("\tI2C_SR1_ADD10\r\n");
    if(SR1 & I2C_SR1_BERR)
        send_message("\tI2C_SR1_BERR\r\n");
    if(SR1 & I2C_SR1_ARLO)
        send_message("\tI2C_SR1_ARLO\r\n");
    if(SR1 & I2C_SR1_AF)
        send_message("\tI2C_SR1_AF\r\n");
    if(SR1 & I2C_SR1_OVR)
        send_message("\tI2C_SR1_OVR\r\n");
    if(SR1 & I2C_SR1_PECERR)
        send_message("\tI2C_SR1_PECERR\r\n");
    if(SR1 & I2C_SR1_TIMEOUT)
        send_message("\tI2C_SR1_ADD10\r\n");
    if(SR1 & I2C_SR1_SMBALERT)
        send_message("\tI2C_SR1_SMBALERT\r\n");
    
    for(int i = 0; i < 32; i++) {
        if(SR1 & (1 << (31 - i)))
            send_message("1");
        else
            send_message("0");
    }

    send_message("\r\n");
}

static void handle_writing() {
    uint32_t SR1 = I2C1->SR1;
    send_message("'handle_writing': got\r\n");
    print_interrupt(SR1);
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
            send_message("Stopping the transmission\r\n");
            I2C1->CR1 |= I2C_CR1_STOP;
            I2C1->CR2 &= ~(I2C_CR2_ITERREN | I2C_CR2_ITEVTEN);
            _i2c_busy = 0;
        } else {
            send_message("Writing done. Time to read.\r\n");
            I2C1->CR1 |= I2C_CR1_START;
        }
    } else if(SR1 & I2C_SR1_TXE && _i_write < _write - 1) {
        _i_write++;
        if(_i_write == _write - 1)
            I2C1->CR2 &= ~I2C_CR2_ITBUFEN; // Turn them off
        I2C1->DR = _buffer_write[_i_write];
    } else
        send_message("^ --- That was unexpected\r\n");
}

static void handle_reading() {
    uint32_t SR1 = I2C1->SR1;
    send_message("'handle_reading': got\r\n");
    print_interrupt(SR1);
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
        // else if(_i_read == _read - 1) {
        //     I2C1->CR1 &= ~I2C_CR1_ACK;
        //     I2C1->CR1 |= I2C_CR1_STOP;
        // }
    } else if(SR1 & I2C_SR1_RXNE) {
        _buffer_read[_i_read] = I2C1->DR;
        _i_read++;
        if(_i_read == _read) {
            I2C1->CR2 &= ~I2C_CR2_ITBUFEN;
            I2C1->CR2 &= ~(I2C_CR2_ITERREN | I2C_CR2_ITEVTEN);
            if(_callback)
                _callback(_buffer_read);
            _i2c_busy = 0;
        } else if(_i_read == _read - 1) {
            I2C1->CR1 &= ~I2C_CR1_ACK;
            I2C1->CR1 |= I2C_CR1_STOP;
        }
    } else {
        send_message("^ --- That was unexpected\r\n");
    }
}

void I2C1_EV_IRQHandler(void) {
    if(_i_write < _write) {
        handle_writing();
    }
    else if(_i_read < _read){
        handle_reading();
    }
    else {
        uint32_t sr = I2C1->SR1;
        send_message("Unexpected interrupt: ");
        print_interrupt(sr);
    }
}

void I2C1_ER_IRQHandler(void) {
    uint32_t sr = I2C1->SR1;
    send_message("Error: ");
    print_interrupt(sr);
}