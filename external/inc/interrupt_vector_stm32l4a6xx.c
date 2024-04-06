/* This file is generated automatically. */

#define WEAK __attribute__ ((weak, alias("Default_Handler")))

/* Interrupt handler declarations */
WEAK void NMI_Handler(void);
WEAK void HardFault_Handler(void);
WEAK void MemManage_Handler(void);
WEAK void BusFault_Handler(void);
WEAK void UsageFault_Handler(void);
WEAK void SVC_Handler(void);
WEAK void DebugMon_Handler(void);
WEAK void PendSV_Handler(void);
WEAK void SysTick_Handler(void);
WEAK void WWDG_IRQHandler(void);
WEAK void PVD_PVM_IRQHandler(void);
WEAK void TAMP_STAMP_IRQHandler(void);
WEAK void RTC_WKUP_IRQHandler(void);
WEAK void FLASH_IRQHandler(void);
WEAK void RCC_IRQHandler(void);
WEAK void EXTI0_IRQHandler(void);
WEAK void EXTI1_IRQHandler(void);
WEAK void EXTI2_IRQHandler(void);
WEAK void EXTI3_IRQHandler(void);
WEAK void EXTI4_IRQHandler(void);
WEAK void DMA1_Channel1_IRQHandler(void);
WEAK void DMA1_Channel2_IRQHandler(void);
WEAK void DMA1_Channel3_IRQHandler(void);
WEAK void DMA1_Channel4_IRQHandler(void);
WEAK void DMA1_Channel5_IRQHandler(void);
WEAK void DMA1_Channel6_IRQHandler(void);
WEAK void DMA1_Channel7_IRQHandler(void);
WEAK void ADC1_2_IRQHandler(void);
WEAK void CAN1_TX_IRQHandler(void);
WEAK void CAN1_RX0_IRQHandler(void);
WEAK void CAN1_RX1_IRQHandler(void);
WEAK void CAN1_SCE_IRQHandler(void);
WEAK void EXTI9_5_IRQHandler(void);
WEAK void TIM1_BRK_TIM15_IRQHandler(void);
WEAK void TIM1_UP_TIM16_IRQHandler(void);
WEAK void TIM1_TRG_COM_TIM17_IRQHandler(void);
WEAK void TIM1_CC_IRQHandler(void);
WEAK void TIM2_IRQHandler(void);
WEAK void TIM3_IRQHandler(void);
WEAK void TIM4_IRQHandler(void);
WEAK void I2C1_EV_IRQHandler(void);
WEAK void I2C1_ER_IRQHandler(void);
WEAK void I2C2_EV_IRQHandler(void);
WEAK void I2C2_ER_IRQHandler(void);
WEAK void SPI1_IRQHandler(void);
WEAK void SPI2_IRQHandler(void);
WEAK void USART1_IRQHandler(void);
WEAK void USART2_IRQHandler(void);
WEAK void USART3_IRQHandler(void);
WEAK void EXTI15_10_IRQHandler(void);
WEAK void RTC_Alarm_IRQHandler(void);
WEAK void DFSDM1_FLT3_IRQHandler(void);
WEAK void TIM8_BRK_IRQHandler(void);
WEAK void TIM8_UP_IRQHandler(void);
WEAK void TIM8_TRG_COM_IRQHandler(void);
WEAK void TIM8_CC_IRQHandler(void);
WEAK void ADC3_IRQHandler(void);
WEAK void FMC_IRQHandler(void);
WEAK void SDMMC1_IRQHandler(void);
WEAK void TIM5_IRQHandler(void);
WEAK void SPI3_IRQHandler(void);
WEAK void UART4_IRQHandler(void);
WEAK void UART5_IRQHandler(void);
WEAK void TIM6_DAC_IRQHandler(void);
WEAK void TIM7_IRQHandler(void);
WEAK void DMA2_Channel1_IRQHandler(void);
WEAK void DMA2_Channel2_IRQHandler(void);
WEAK void DMA2_Channel3_IRQHandler(void);
WEAK void DMA2_Channel4_IRQHandler(void);
WEAK void DMA2_Channel5_IRQHandler(void);
WEAK void DFSDM1_FLT0_IRQHandler(void);
WEAK void DFSDM1_FLT1_IRQHandler(void);
WEAK void DFSDM1_FLT2_IRQHandler(void);
WEAK void COMP_IRQHandler(void);
WEAK void LPTIM1_IRQHandler(void);
WEAK void LPTIM2_IRQHandler(void);
WEAK void OTG_FS_IRQHandler(void);
WEAK void DMA2_Channel6_IRQHandler(void);
WEAK void DMA2_Channel7_IRQHandler(void);
WEAK void LPUART1_IRQHandler(void);
WEAK void QUADSPI_IRQHandler(void);
WEAK void I2C3_EV_IRQHandler(void);
WEAK void I2C3_ER_IRQHandler(void);
WEAK void SAI1_IRQHandler(void);
WEAK void SAI2_IRQHandler(void);
WEAK void SWPMI1_IRQHandler(void);
WEAK void TSC_IRQHandler(void);
WEAK void LCD_IRQHandler(void);
WEAK void AES_IRQHandler(void);
WEAK void HASH_RNG_IRQHandler(void);
WEAK void FPU_IRQHandler(void);
WEAK void CRS_IRQHandler(void);
WEAK void I2C4_EV_IRQHandler(void);
WEAK void I2C4_ER_IRQHandler(void);
WEAK void DCMI_IRQHandler(void);
WEAK void CAN2_TX_IRQHandler(void);
WEAK void CAN2_RX0_IRQHandler(void);
WEAK void CAN2_RX1_IRQHandler(void);
WEAK void CAN2_SCE_IRQHandler(void);
WEAK void DMA2D_IRQHandler(void);

/* Interrupt table */
__attribute__ ((section(".isr_vector")))
void (* const g_pfnVectors[])(void) = {
  (void*)&_estack,
  Reset_Handler,
  NMI_Handler,
  HardFault_Handler,
  MemManage_Handler,
  BusFault_Handler,
  UsageFault_Handler,
  0,
  0,
  0,
  0,
  SVC_Handler,
  DebugMon_Handler,
  0,
  PendSV_Handler,
  SysTick_Handler,
  WWDG_IRQHandler,
  PVD_PVM_IRQHandler,
  TAMP_STAMP_IRQHandler,
  RTC_WKUP_IRQHandler,
  FLASH_IRQHandler,
  RCC_IRQHandler,
  EXTI0_IRQHandler,
  EXTI1_IRQHandler,
  EXTI2_IRQHandler,
  EXTI3_IRQHandler,
  EXTI4_IRQHandler,
  DMA1_Channel1_IRQHandler,
  DMA1_Channel2_IRQHandler,
  DMA1_Channel3_IRQHandler,
  DMA1_Channel4_IRQHandler,
  DMA1_Channel5_IRQHandler,
  DMA1_Channel6_IRQHandler,
  DMA1_Channel7_IRQHandler,
  ADC1_2_IRQHandler,
  CAN1_TX_IRQHandler,
  CAN1_RX0_IRQHandler,
  CAN1_RX1_IRQHandler,
  CAN1_SCE_IRQHandler,
  EXTI9_5_IRQHandler,
  TIM1_BRK_TIM15_IRQHandler,
  TIM1_UP_TIM16_IRQHandler,
  TIM1_TRG_COM_TIM17_IRQHandler,
  TIM1_CC_IRQHandler,
  TIM2_IRQHandler,
  TIM3_IRQHandler,
  TIM4_IRQHandler,
  I2C1_EV_IRQHandler,
  I2C1_ER_IRQHandler,
  I2C2_EV_IRQHandler,
  I2C2_ER_IRQHandler,
  SPI1_IRQHandler,
  SPI2_IRQHandler,
  USART1_IRQHandler,
  USART2_IRQHandler,
  USART3_IRQHandler,
  EXTI15_10_IRQHandler,
  RTC_Alarm_IRQHandler,
  DFSDM1_FLT3_IRQHandler,
  TIM8_BRK_IRQHandler,
  TIM8_UP_IRQHandler,
  TIM8_TRG_COM_IRQHandler,
  TIM8_CC_IRQHandler,
  ADC3_IRQHandler,
  FMC_IRQHandler,
  SDMMC1_IRQHandler,
  TIM5_IRQHandler,
  SPI3_IRQHandler,
  UART4_IRQHandler,
  UART5_IRQHandler,
  TIM6_DAC_IRQHandler,
  TIM7_IRQHandler,
  DMA2_Channel1_IRQHandler,
  DMA2_Channel2_IRQHandler,
  DMA2_Channel3_IRQHandler,
  DMA2_Channel4_IRQHandler,
  DMA2_Channel5_IRQHandler,
  DFSDM1_FLT0_IRQHandler,
  DFSDM1_FLT1_IRQHandler,
  DFSDM1_FLT2_IRQHandler,
  COMP_IRQHandler,
  LPTIM1_IRQHandler,
  LPTIM2_IRQHandler,
  OTG_FS_IRQHandler,
  DMA2_Channel6_IRQHandler,
  DMA2_Channel7_IRQHandler,
  LPUART1_IRQHandler,
  QUADSPI_IRQHandler,
  I2C3_EV_IRQHandler,
  I2C3_ER_IRQHandler,
  SAI1_IRQHandler,
  SAI2_IRQHandler,
  SWPMI1_IRQHandler,
  TSC_IRQHandler,
  LCD_IRQHandler,
  AES_IRQHandler,
  HASH_RNG_IRQHandler,
  FPU_IRQHandler,
  CRS_IRQHandler,
  I2C4_EV_IRQHandler,
  I2C4_ER_IRQHandler,
  DCMI_IRQHandler,
  CAN2_TX_IRQHandler,
  CAN2_RX0_IRQHandler,
  CAN2_RX1_IRQHandler,
  CAN2_SCE_IRQHandler,
  DMA2D_IRQHandler
};

