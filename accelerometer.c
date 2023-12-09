#include "i2c.h"

#define LIS35DE_ADDR 0x1D

void startAccelerometer() {
    char buffer_write[] = {0x20, 0x47};
    i2c_write_read(2, 0, LIS35DE_ADDR, buffer_write, 0);
}

static signed char i2c_output;

static void read_callback(char* buffer) {
    i2c_output = buffer[0];
}

int checkDataAvailable() {
    char buffer_write[] = {0x27};
    i2c_write_read(1, 1, LIS35DE_ADDR, buffer_write, read_callback);
    return i2c_output & (1 << 3);
}

signed char readX() {
    char buffer_write[] = {0x29};
    i2c_write_read(1, 1, LIS35DE_ADDR, buffer_write, read_callback);
    return i2c_output;
}

signed char readY() {
    char buffer_write[] = {0x2B};
    i2c_write_read(1, 1, LIS35DE_ADDR, buffer_write, read_callback);
    return i2c_output;
}

signed char readZ() {
    char buffer_write[] = {0x2D};
    i2c_write_read(1, 1, LIS35DE_ADDR, buffer_write, read_callback);
    return i2c_output;
}