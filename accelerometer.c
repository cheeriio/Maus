#include <stm32.h>
#include <gpio.h>
#include <string.h>
#include "i2c.h"
#include "messages.h"
#include <stdlib.h>

#define LIS35DE_ADDR 0x1D

static signed char read_values[2];

void startAccelerometer() {
    char buffer_setup_ctrl_reg_1[] = {0x20, 0x47};
    i2c_write_read(2, 0, LIS35DE_ADDR, buffer_setup_ctrl_reg_1, NULL);
}

static void read_x_callback(char* buffer) {
    TIM3->CCR1 = 980 - abs((signed char)buffer[0])*3;
    read_values[0] = (signed char)buffer[0];
}

static void read_y_callback(char* buffer) {
    TIM3->CCR2 = 980 - abs((signed char)buffer[0])*3;
    read_values[1] = (signed char)buffer[0];
    send_message((char*)read_values, 2);
}

static void readX() {
    char buffer_write[] = {0x29};
    i2c_write_read(1, 1, LIS35DE_ADDR, buffer_write, read_x_callback);
}

static void readY() {
    char buffer_write[] = {0x2B};
    i2c_write_read(1, 1, LIS35DE_ADDR, buffer_write, read_y_callback);
}

void readFromAccelerometer() {
    readX();
    readY();
}
