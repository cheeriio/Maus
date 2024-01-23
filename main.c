#include <gpio.h>
#include <stm32.h>
#include "i2c.h"
#include "accelerometer.h"

#define HSI_HZ 16000000U
#define PCLK1_HZ HSI_HZ
#define BAUD 9600U

void configureDMA();

void configureUSART();

int main() {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN
                 | RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_DMA1EN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN | RCC_APB1ENR_TIM3EN
                  | RCC_APB1ENR_I2C1EN | RCC_APB1ENR_TIM4EN;
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    __NOP();

    configureUSART();
    configureDMA();

    NVIC_EnableIRQ(DMA1_Stream6_IRQn);
    USART2->CR1 |= USART_CR1_UE;

    i2c_enable();

    startAccelerometer();

    // Link the TIM3 output to diodes.
    GPIOafConfigure(GPIOA, 6, GPIO_OType_PP,
                    GPIO_Low_Speed,
                    GPIO_PuPd_NOPULL, GPIO_AF_TIM3);
    GPIOafConfigure(GPIOA, 7, GPIO_OType_PP,
                    GPIO_Low_Speed,
                    GPIO_PuPd_NOPULL, GPIO_AF_TIM3);

    TIM3->PSC = 99;
    TIM3->ARR = 999;
    TIM3->EGR = TIM_EGR_UG;
    TIM3->CCR1 = 980;
    TIM3->CCR2 = 980;

    // Use PWM 2, output inverted.
    TIM3->CCMR1 =
        TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 |
        TIM_CCMR1_OC1M_0 | TIM_CCMR1_OC1PE |
        TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1 |
        TIM_CCMR1_OC2M_0 | TIM_CCMR1_OC2PE;
    
    TIM3->CCER = TIM_CCER_CC1E | TIM_CCER_CC1P |
                 TIM_CCER_CC2E | TIM_CCER_CC2P;

    TIM3->CR1 = TIM_CR1_ARPE |  TIM_CR1_CMS_0 |
                TIM_CR1_CMS_1 | TIM_CR1_CEN;

    // Fetch data from accelerometer every 20 ms.
    TIM4->CR1 = TIM_CR1_URS;
    TIM4->PSC = 15999;
    TIM4->ARR = 19;
    TIM4->EGR = TIM_EGR_UG;
    TIM4->SR = ~TIM_SR_UIF;
    TIM4->DIER =  TIM_DIER_UIE;
    TIM4->CR1 |= TIM_CR1_CEN;

    NVIC_EnableIRQ(TIM4_IRQn);
    
    while(1) {}
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

void TIM4_IRQHandler(void) {
    TIM4->SR = ~TIM_SR_UIF;
    readFromAccelerometer();
}
