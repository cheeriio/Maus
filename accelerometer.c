#include <stm32.h>
#include <gpio.h>
#include <string.h>
#include "i2c.h"
#include "messages.h"

#define LIS35DE_ADDR 0x1D

void startAccelerometer() {
    // char buffer_zero_ctrl_reg_1[] = {0x20, 0x00};
    // i2c_write_read(2, 0, LIS35DE_ADDR, buffer_zero_ctrl_reg_1, 0);
    // char buffer_zero_ctrl_reg_3[] = {0x22, 0x00};
    // i2c_write_read(2, 0, LIS35DE_ADDR, buffer_zero_ctrl_reg_3, 0);

    // GPIOinConfigure(GPIOA,
    //                 1,
    //                 GPIO_PuPd_UP,
    //                 EXTI_Mode_Interrupt,
    //                 EXTI_Trigger_Rising);
                
    // NVIC_EnableIRQ(EXTI1_IRQn);

    send_message("Setting up accelerometer\r\n");
    char buffer_setup_ctrl_reg_1[] = {0x20, 0x47};
    i2c_write_read(2, 0, LIS35DE_ADDR, buffer_setup_ctrl_reg_1, 0);
    // Data Ready interrupt on line INT1
    // char buffer_setup_ctrl_reg_3[] = {0x22, 0x00};
    // i2c_write_read(2, 0, LIS35DE_ADDR, buffer_setup_ctrl_reg_3, 0);
}

static signed char i2c_output = 100;

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


// void EXTI1_IRQHandler(void) {
//     send_message("Got interrupt from accelerometer\r\n");
//     EXTI->PR = EXTI_PR_PR1;
//     char x_val = abs(readX());
//     char y_val = abs(readY());
//     char z_val = abs(readZ());
//     TIM3->CCR1 = 1000 - x_val*3;
//     TIM3->CCR2 = 1000 - y_val*3;
//     TIM3->CCR3 = 1000 - z_val*3;

//     char message[16];
//     strcpy(message, "XYZ: ");
//     char_to_hex(x_val, message + 5);
//     message[7] = '-';
//     char_to_hex(y_val, message + 8);
//     message[10] = '-';
//     char_to_hex(z_val, message + 11);
//     message[13] = '\r';
//     message[14] = '\n';
//     message[15] = '\0';
//     send_message(message);
// }