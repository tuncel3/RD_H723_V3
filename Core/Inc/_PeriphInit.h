
// UART1
//LL_APB1_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_UART5);
LL_USART_SetBaudRate(UART5, 137500000, 0, 16, 1500000);
LL_USART_SetDataWidth(UART5, LL_USART_DATAWIDTH_8B);
LL_USART_SetStopBitsLength(UART5, LL_USART_STOPBITS_1);
LL_USART_SetParity(UART5, LL_USART_PARITY_NONE);
//LL_USART_SetTransferDirection(UART5, LL_USART_DIRECTION_TX_RX);
LL_USART_SetHWFlowCtrl(UART5, LL_USART_HWCONTROL_NONE);
LL_USART_EnableFIFO(UART5);
LL_USART_SetTXFIFOThreshold(UART5, LL_USART_FIFOTHRESHOLD_1_4);
LL_USART_SetRXFIFOThreshold(UART5, LL_USART_FIFOTHRESHOLD_1_4);
LL_USART_EnableDMAReq_TX(UART5);
LL_USART_Enable(UART5);
// UART5
///////////////////////////////////////////////////////////
// DMA FOR USART
LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
LL_DMA_SetPeriphRequest(DMA1, LL_DMA_STREAM_0, LL_DMAMUX1_REQ_UART5_TX); // UART5 TX request
LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_STREAM_0, LL_DMA_DIRECTION_MEMORY_TO_PERIPH); // Memory-to-Peripheral
LL_DMA_SetStreamPriorityLevel(DMA1, LL_DMA_STREAM_0, LL_DMA_PRIORITY_HIGH); // High priority
LL_DMA_SetMode(DMA1, LL_DMA_STREAM_0, LL_DMA_MODE_NORMAL); // Normal mode
LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_STREAM_0, LL_DMA_PERIPH_NOINCREMENT); // Fixed peripheral address
LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_STREAM_0, LL_DMA_MEMORY_INCREMENT); // Increment memory address
LL_DMA_SetPeriphSize(DMA1, LL_DMA_STREAM_0, LL_DMA_PDATAALIGN_BYTE); // Peripheral data size: Byte
LL_DMA_SetMemorySize(DMA1, LL_DMA_STREAM_0, LL_DMA_MDATAALIGN_BYTE); // Memory data size: Byte
LL_DMA_SetMemoryAddress(DMA1, LL_DMA_STREAM_0, (uint32_t)DUB);
LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_0, sizeof(DUB) - 1);
LL_DMA_ConfigAddresses(DMA1, LL_DMA_STREAM_0,
	   (uint32_t)DUB,  // Memory address (source)
	   LL_USART_DMA_GetRegAddr(UART5, LL_USART_DMA_REG_DATA_TRANSMIT), // UART5 TX register (destination)
	   LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
NVIC_SetPriority(DMA1_Stream0_IRQn, 0);
NVIC_EnableIRQ(DMA1_Stream0_IRQn);
LL_DMA_EnableIT_TC(DMA1, LL_DMA_STREAM_0);
// DMA FOR USART
/////////////////////////////////////////////////////////////
// DMA FOR ADC
LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
LL_DMA_SetPeriphRequest(DMA1, LL_DMA_STREAM_1, LL_DMAMUX1_REQ_ADC1);
LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_STREAM_1, LL_DMA_DIRECTION_PERIPH_TO_MEMORY); // From peripheral to memory
LL_DMA_SetStreamPriorityLevel(DMA1, LL_DMA_STREAM_1, LL_DMA_PRIORITY_HIGH);                // High priority
LL_DMA_SetMode(DMA1, LL_DMA_STREAM_1, LL_DMA_MODE_CIRCULAR);                               // Circular mode
LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_STREAM_1, LL_DMA_PERIPH_NOINCREMENT);                 // Fixed peripheral address
LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_STREAM_1, LL_DMA_MEMORY_INCREMENT);                  // Increment memory address
LL_DMA_SetPeriphSize(DMA1, LL_DMA_STREAM_1, LL_DMA_PDATAALIGN_WORD);                      // Peripheral data size: Word (32 bits)
LL_DMA_SetMemorySize(DMA1, LL_DMA_STREAM_1, LL_DMA_MDATAALIGN_WORD);                      // Memory data size: Word (32 bits)
LL_DMA_ConfigAddresses(  DMA1,  LL_DMA_STREAM_1,
  LL_ADC_DMA_GetRegAddr(ADC1, LL_ADC_DMA_REG_REGULAR_DATA), // ADC1 data register
  (uint32_t)adc_buffer,                                     // ADC buffer in memory
  LL_DMA_DIRECTION_PERIPH_TO_MEMORY);
LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_1, NUM_CHANNELS);
LL_ADC_REG_SetDataTransferMode(ADC1, LL_ADC_REG_DMA_TRANSFER_UNLIMITED);
NVIC_SetPriority(DMA1_Stream1_IRQn, 0);
NVIC_EnableIRQ(DMA1_Stream1_IRQn);
LL_DMA_EnableIT_TC(DMA1, LL_DMA_STREAM_1);
LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_1);
// DMA FOR ADC
///////////////////////////////////////////////////////////
// ADC
//LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_PATH_INTERNAL_VREFINT | LL_ADC_PATH_INTERNAL_TEMPSENSOR);
//LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_7);
//LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_2, LL_ADC_CHANNEL_VREFINT);
//LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_3, LL_ADC_CHANNEL_TEMPSENSOR);
//LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_4, LL_ADC_CHANNEL_VBAT);
//
//LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_7, LL_ADC_SAMPLINGTIME_16CYCLES_5);
//LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_VREFINT, LL_ADC_SAMPLINGTIME_16CYCLES_5);
//LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_TEMPSENSOR, LL_ADC_SAMPLINGTIME_16CYCLES_5);
//LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_VBAT, LL_ADC_SAMPLINGTIME_16CYCLES_5);
//
//LL_ADC_SetChannelSingleDiff(ADC1, LL_ADC_CHANNEL_7, LL_ADC_SINGLE_ENDED);
//LL_ADC_SetChannelSingleDiff(ADC1, LL_ADC_CHANNEL_VREFINT, LL_ADC_SINGLE_ENDED);
//LL_ADC_SetChannelSingleDiff(ADC1, LL_ADC_CHANNEL_TEMPSENSOR, LL_ADC_SINGLE_ENDED);
//LL_ADC_SetChannelSingleDiff(ADC1, LL_ADC_CHANNEL_VBAT, LL_ADC_SINGLE_ENDED);
//
//LL_ADC_SetChannelPreselection(ADC1, LL_ADC_CHANNEL_7);
//
//LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_PATH_INTERNAL_NONE);
//LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_PATH_INTERNAL_VREFINT | LL_ADC_PATH_INTERNAL_TEMPSENSOR);
LL_ADC_StartCalibration(ADC1, LL_ADC_CALIB_OFFSET_LINEARITY, LL_ADC_SINGLE_ENDED);
while (LL_ADC_IsCalibrationOnGoing(ADC1)) {asm("NOP");}
LL_ADC_Enable(ADC1);
while (!LL_ADC_IsActiveFlag_ADRDY(ADC1)) {asm("NOP");}
LL_ADC_REG_StartConversion(ADC1);
// ADC INIT
///////////////////////////////////////////////////////////
