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
    // I2C1->CR2 = PCLK1_MHZ | (1 << I2C_CR2_ITEVTEN);
    I2C1->TRISE = PCLK1_MHZ + 1;

    // NVIC_EnableIRQ(I2C1_EV_IRQn);

    I2C1->CR1 |= I2C_CR1_PE;
}

#define MAX_READ_LEN 100

static int _write;
static int _read;
static char* _buffer_write;
static char _buffer_read[MAX_READ_LEN];

static int _i_write;
static int _i_read;

static int _i2c_busy = 0;
static void (*_callback)(char*);

void i2c_write_read(int write,
                    int read,
                    char slave_address,
                    char* buffer_write,
                    void (*callback)(char*)) {
    if (_i2c_busy)
        return;
    _i2c_busy = 1;

    _write = write;
    _read = read;
    _buffer_write = buffer_write;
    _callback = callback;
    
    _i_write = 0;
    _i_read = 0;


    if(_write == 0 && _read == 0)
        return;

    I2C1->CR1 |= I2C_CR1_START;
    while(!(I2C1->SR1 & I2C_SR1_SB)) {}
    I2C1->DR = slave_address << 1;
    while(!(I2C1->SR1 & I2C_SR1_ADDR)) {}
    I2C1->SR2;

    for(; _i_write < _write; _i_write++) {
        I2C1->DR = buffer_write[_i_write];
        if (_i_write == _write - 1)
            {while(!(I2C1->SR2 & I2C_SR1_BTF)) {}}
        else
            {while(!(I2C1->SR1 & I2C_SR1_BTF)) {}} // Originally there was I2C_SR1_TXE but before the change the code didn't work properly...
    }

    if(_read == 0) {
        I2C1->CR1 |= I2C_CR1_STOP;
        _i2c_busy = 0;
        return;
    }

    I2C1->CR1 |= I2C_CR1_START;
    while(!(I2C1->SR1 & I2C_SR1_SB)) {}
    I2C1->DR = slave_address << 1 | 1;

    if(_read == 1) {
        I2C1->CR1 &= ~I2C_CR1_ACK;
        while(!(I2C1->SR1 & I2C_SR1_ADDR)) {}
        I2C1->SR2;
        I2C1->CR1 |= I2C_CR1_STOP;
        while(!(I2C1->SR1 & I2C_SR1_RXNE)) {}
        _buffer_read[0] = I2C1->DR;
        if(_callback)
            _callback(_buffer_read);
        _i2c_busy = 0;
        return;
    }

    I2C1->CR1 |= I2C_CR1_ACK;
    while(!(I2C1->SR1 & I2C_SR1_ADDR)) {}
    I2C1->SR2;

    for(; _i_read < _read; _i_read++) {
        if(_i_read == _read - 1) {
            I2C1->CR1 &= ~I2C_CR1_ACK;
            I2C1->CR1 |= I2C_CR1_STOP;
        }
        while(!(I2C1->SR1 & I2C_SR1_RXNE)) {}
        _buffer_read[_i_read] = I2C1->DR;
    }
    if(_callback)
        _callback(_buffer_read);
    _i2c_busy = 0;
    return;
}

// void I2C1_EV_IRQHandler(void) {
//     __NOP();
// }
