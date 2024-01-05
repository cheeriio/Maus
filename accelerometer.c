#include <stm32.h>
#include <gpio.h>
#include <string.h>
#include "i2c.h"
#include "messages.h"
#include <stdlib.h>

#define LIS35DE_ADDR 0x1D

void startAccelerometer() {
    send_message("Setting up accelerometer\r\n");
    char buffer_setup_ctrl_reg_1[] = {0x20, 0x47};
    i2c_write_read(2, 0, LIS35DE_ADDR, buffer_setup_ctrl_reg_1, NULL);
}

static char hex(char x) {
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
        default: return '?';
    }
}

static void char_to_hex(char x, char* buf) {
    char y = x % 16;
    buf[0] = hex(x / 16);
    buf[1] = hex(y);
}

static void print_val(char x) {
    char message[6];
    char_to_hex(x, message);
    message[2] = '\r';
    message[3] = '\n';
    message[4] = '\0';
    send_message(message);
}

static void read_x_callback(char* buffer) {
    print_val(buffer[0]);
    TIM3->CCR1 = 1000 - abs(buffer[0])*3;
}

static void read_y_callback(char* buffer) {
    print_val(buffer[0]);
    TIM3->CCR2 = 1000 - abs(buffer[0])*3;
}

static void read_z_callback(char* buffer) {
    print_val(buffer[0]);
    TIM3->CCR3 = 1000 - abs(buffer[0])*3;
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
