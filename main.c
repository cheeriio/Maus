#include <string.h>
#include <delay.h>
#include <gpio.h>
#include <stm32.h>
#include "utils.h"
#include "prio.h"
#include "messages.h"
#include "i2c.h"
#include "accelerometer.h"
#include <stdlib.h>

#define HSI_HZ 16000000U
#define PCLK1_HZ HSI_HZ
#define BAUD 9600U

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

void configureDMA();

void configureUSART();

int main() {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN
                 | RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_DMA1EN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN | RCC_APB1ENR_TIM3EN | RCC_APB1ENR_I2C1EN;
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    __NOP();

    configureUSART();
    configureDMA();

    NVIC_EnableIRQ(DMA1_Stream6_IRQn);
    USART2->CR1 |= USART_CR1_UE;

    i2c_enable();

    GPIOafConfigure(GPIOA, 6, GPIO_OType_PP,
                    GPIO_Low_Speed,
                    GPIO_PuPd_NOPULL, GPIO_AF_TIM3);
    GPIOafConfigure(GPIOA, 7, GPIO_OType_PP,
                    GPIO_Low_Speed,
                    GPIO_PuPd_NOPULL, GPIO_AF_TIM3);
    GPIOafConfigure(GPIOB, 0, GPIO_OType_PP,
                    GPIO_Low_Speed,
                    GPIO_PuPd_NOPULL, GPIO_AF_TIM3);

    TIM3->PSC = 99;
    TIM3->ARR = 999;
    TIM3->EGR = TIM_EGR_UG;
    TIM3->CCR1 = 990;
    TIM3->CCR2 = 990;
    TIM3->CCR3 = 990;

    TIM3->CCMR1 =
        TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 |
        TIM_CCMR1_OC1M_0 | TIM_CCMR1_OC1PE |
        TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1 |
        TIM_CCMR1_OC2M_0 | TIM_CCMR1_OC2PE;

    TIM3->CCMR2 =
        TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1 |
        TIM_CCMR2_OC3M_0 | TIM_CCMR2_OC3PE;
    
    TIM3->CCER = TIM_CCER_CC1E | TIM_CCER_CC1P |
                 TIM_CCER_CC2E | TIM_CCER_CC2P |
                 TIM_CCER_CC3E | TIM_CCER_CC3P;

    TIM3->CR1 = TIM_CR1_ARPE |  TIM_CR1_CMS_0 |
                TIM_CR1_CMS_1 | TIM_CR1_CEN;

    startAccelerometer();

    int x = 0;
    while(1) {
        x = (x + 1) % 1000001;
        if (x == 1000000) {
            send_message("Reading X\r\n");
            char x_val = abs(readX());
            send_message("Reading Y\r\n");
            char y_val = abs(readY());
            send_message("Reading Z\r\n");
            char z_val = abs(readZ());
            TIM3->CCR1 = 1000 - x_val*3;
            TIM3->CCR2 = 1000 - y_val*3;
            TIM3->CCR3 = 1000 - z_val*3;

            char message[16];
            strcpy(message, "XYZ: ");
            char_to_hex(x_val, message + 5);
            message[7] = '-';
            char_to_hex(y_val, message + 8);
            message[10] = '-';
            char_to_hex(z_val, message + 11);
            message[13] = '\r';
            message[14] = '\n';
            message[15] = '\0';
            send_message(message);
        }
    }
}

void configureDMA() {
    DMA1_Stream6->CR = 4U << 25 |
                       DMA_SxCR_PL_1 |
                       DMA_SxCR_MINC |
                       DMA_SxCR_DIR_0 |
                       DMA_SxCR_TCIE;

    DMA1_Stream6->PAR = (uint32_t)&USART2->DR;

    DMA1->HIFCR = DMA_HIFCR_CTCIF6;
}

void configureUSART() {
    GPIOafConfigure(GPIOA,
                    2,
                    GPIO_OType_PP,
                    GPIO_Fast_Speed,
                    GPIO_PuPd_NOPULL,
                    GPIO_AF_USART2);
    GPIOafConfigure(GPIOA,
                    3,
                    GPIO_OType_PP,
                    GPIO_Fast_Speed,
                    GPIO_PuPd_UP,
                    GPIO_AF_USART2);

    USART2->CR1 = USART_CR1_TE;
    USART2->CR2 = 0;
    USART2->BRR = (PCLK1_HZ + (BAUD / 2U)) / BAUD;
    USART2->CR3 = USART_CR3_DMAT;
}
