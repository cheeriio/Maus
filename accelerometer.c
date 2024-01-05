#include <stm32.h>
#include <gpio.h>
#include <string.h>
#include "i2c.h"
#include "messages.h"
#include <stdlib.h>

#define LIS35DE_ADDR 0x1D

void startAccelerometer() {
    char buffer_setup_ctrl_reg_1[] = {0x20, 0x47};
    i2c_write_read(2, 0, LIS35DE_ADDR, buffer_setup_ctrl_reg_1, NULL);
}

static void read_x_callback(char* buffer) {
    TIM3->CCR1 = 1000 - abs((signed char)buffer[0])*3;
}

static void read_y_callback(char* buffer) {
    TIM3->CCR2 = 1000 - abs((signed char)buffer[0])*3;
}

static void read_z_callback(char* buffer) {
    TIM3->CCR3 = 1000 - abs((signed char)buffer[0])*3;
}

static void readX() {
    char buffer_write[] = {0x29};
    i2c_write_read(1, 1, LIS35DE_ADDR, buffer_write, read_x_callback);
}

static void readY() {
    char buffer_write[] = {0x2B};
    i2c_write_read(1, 1, LIS35DE_ADDR, buffer_write, read_y_callback);
}

static void readZ() {
    char buffer_write[] = {0x2D};
    i2c_write_read(1, 1, LIS35DE_ADDR, buffer_write, read_z_callback);
}

void updateLED() {
    readX();
    readY();
    readZ();
}
