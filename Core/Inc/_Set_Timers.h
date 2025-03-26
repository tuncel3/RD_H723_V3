
// TIM1		E							16
// TIM2		E							32
// TIM3		DELAY ZC					16
// TIM4		E							16
// TIM5		MEAS PER					32
// TIM6		TRIGGER ADC					16
// TIM7		1MS COUNTER					16
// TIM8		E							16
// TIM12	E							16
// TIM13	E							16
// TIM14	E							16
// TIM15	FORWARD CONV. MOS GATE		16
// TIM16	THY DRV PWM					16
// TIM23	E							32
// TIM24	E							32

NVIC_SetPriority(SysTick_IRQn, 14);			// SYS TICK
NVIC_SetPriority(DMA1_Stream1_IRQn, 0);		// ADC DMA
NVIC_SetPriority(EXTI9_5_IRQn, 2);			// ZCR INPUTS
NVIC_SetPriority(TIM16_IRQn, 3);			// TIM16	THY DRV PWM
NVIC_SetPriority(TIM2_IRQn, 4);				// TIM2		THY TRIG TIME R
NVIC_SetPriority(TIM23_IRQn, 4);			// TIM23	THY TRIG TIME S
NVIC_SetPriority(TIM24_IRQn, 4);			// TIM24	THY TRIG TIME T
NVIC_SetPriority(TIM3_IRQn, 4);				// TIM3		DELAY ZC
NVIC_SetPriority(TIM7_IRQn, 10);			// TIM7		1MS COUNTER
NVIC_SetPriority(DMA1_Stream0_IRQn, 15);	// UART DMA

NVIC_EnableIRQ(TIM2_IRQn);
NVIC_EnableIRQ(TIM3_IRQn);
NVIC_EnableIRQ(TIM7_IRQn);
NVIC_EnableIRQ(TIM16_IRQn);
NVIC_EnableIRQ(TIM17_IRQn);
NVIC_EnableIRQ(TIM23_IRQn);
NVIC_EnableIRQ(TIM24_IRQn);

//////////////SysTick Settings///////////////////
SysTick->VAL   = 0xffffff;			// SysTick Current Value Register (Start Value)
SysTick->LOAD  = 10999;				// Reload value
SysTick->CTRL  = 0b111;				// Control Reg. CLK SRC | INT EN_g | CNT EN_g;
//////////////SysTick Settings///////////////////
// TIM5		MEAS PER
LL_TIM_SetPrescaler(TIM5, 0);
LL_TIM_SetCounterMode(TIM5, LL_TIM_COUNTERMODE_UP);
LL_TIM_EnableARRPreload(TIM5);
LL_TIM_SetAutoReload(TIM5, 4294967295);
LL_TIM_EnableCounter(TIM5);
// TIM5 Settings
////////////////////////////////////////////
// TIM6		TRIGGER ADC
LL_TIM_SetPrescaler(TIM6, 4);
LL_TIM_SetCounterMode(TIM6, LL_TIM_COUNTERMODE_UP);
LL_TIM_SetTriggerOutput(TIM6, LL_TIM_TRGO_UPDATE);
LL_TIM_EnableARRPreload(TIM6);
LL_TIM_SetAutoReload(TIM6, 2749);
LL_TIM_EnableIT_UPDATE(TIM6);
LL_TIM_EnableCounter(TIM6);
// TIM6
////////////////////////////////////////////
// TIM7		1MS COUNTER
LL_TIM_EnableIT_UPDATE(TIM7);
LL_TIM_SetPrescaler(TIM7, 4);
LL_TIM_SetCounterMode(TIM7, LL_TIM_COUNTERMODE_UP);
LL_TIM_SetAutoReload(TIM7, 54999);
LL_TIM_EnableARRPreload(TIM7);
LL_TIM_SetTriggerOutput(TIM7, LL_TIM_TRGO_RESET);
LL_TIM_DisableMasterSlaveMode(TIM7);
LL_TIM_EnableCounter(TIM7);
// TIM7
////////////////////////////////////////////
// TIM15	FORWARD CONV. MOS GATE
LL_TIM_SetPrescaler(TIM15, 0);
LL_TIM_SetAutoReload(TIM15, 3849);
LL_TIM_OC_SetCompareCH1(TIM15, 1680);
LL_TIM_CC_EnableChannel(TIM15, LL_TIM_CHANNEL_CH1);
LL_TIM_EnableAllOutputs(TIM15);
LL_TIM_EnableCounter(TIM15);
LL_TIM_EnableARRPreload(TIM15);
// TIM15
////////////////////////////////////////////
// TIM16	THY DRV PWM
LL_TIM_SetPrescaler(TIM16, 0);
LL_TIM_SetCounterMode(TIM16, LL_TIM_COUNTERMODE_UP);
LL_TIM_EnableARRPreload(TIM16);
LL_TIM_SetAutoReload(TIM16, 17049);
LL_TIM_OC_SetCompareCH1(TIM16, 5114);
LL_TIM_EnableIT_UPDATE(TIM16);
LL_TIM_EnableIT_CC1(TIM16);
LL_TIM_CC_EnableChannel(TIM16, LL_TIM_CHANNEL_CH1);
LL_TIM_EnableCounter(TIM16);
// TIM16
////////////////////////////////////////////

// TIM2			THY TRIG TIME R
LL_TIM_SetPrescaler(TIM2, 0);
LL_TIM_SetAutoReload(TIM2, 3299999);
LL_TIM_OC_SetCompareCH1(TIM2, 100);
LL_TIM_EnableIT_UPDATE(TIM2);
LL_TIM_EnableCounter(TIM2);
LL_TIM_CC_EnablePreload(TIM2);
LL_TIM_OC_EnablePreload(TIM2, LL_TIM_CHANNEL_CH1);
LL_TIM_GenerateEvent_UPDATE(TIM2);
// TIM2 4294967296
////////////////////////////////////////////

// TIM23		THY TRIG TIME S
LL_TIM_SetPrescaler(TIM23, 0);
LL_TIM_SetAutoReload(TIM23, 3299999);
LL_TIM_OC_SetCompareCH1(TIM23, 100);
LL_TIM_EnableIT_UPDATE(TIM23);
LL_TIM_EnableCounter(TIM23);
LL_TIM_CC_EnablePreload(TIM23);
LL_TIM_OC_EnablePreload(TIM23, LL_TIM_CHANNEL_CH1);
LL_TIM_GenerateEvent_UPDATE(TIM23);
// TIM23
////////////////////////////////////////////

// TIM24		THY TRIG TIME T
LL_TIM_SetPrescaler(TIM24, 0);
LL_TIM_SetAutoReload(TIM24, 3299999);
LL_TIM_OC_SetCompareCH1(TIM24, 100);
LL_TIM_EnableIT_UPDATE(TIM24);
LL_TIM_EnableCounter(TIM24);
LL_TIM_CC_EnablePreload(TIM24);
LL_TIM_OC_EnablePreload(TIM24, LL_TIM_CHANNEL_CH1);
LL_TIM_GenerateEvent_UPDATE(TIM24);
// TIM24
////////////////////////////////////////////

// TIM3	DELAY ZC
LL_TIM_SetPrescaler(TIM3, zc_start_delay_300u_psc_16);
LL_TIM_SetAutoReload(TIM3, zc_start_delay_300u_arr_16);	// 300 us after zero cross
LL_TIM_EnableIT_UPDATE(TIM3);
LL_TIM_GenerateEvent_UPDATE(TIM3);
LL_TIM_EnableCounter(TIM3);
// TIM3
////////////////////////////////////////////



