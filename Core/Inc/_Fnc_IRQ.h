

void TIM7_IRQHandler(void)
{
	if (LL_TIM_IsActiveFlag_UPDATE(TIM7)){
		LL_TIM_ClearFlag_UPDATE(TIM7);
		ms_tick_cnt++;
		delay_1ms_cnt++;
		ButtScanDelay_cnt++;


	}
}


void SysTick_Handler(void) {

	asm("NOP");
}


void DMA1_Stream0_IRQHandler(void)
{
    if (LL_DMA_IsActiveFlag_TC0(DMA1))  // Transfer Complete interrupt
    {
        LL_DMA_ClearFlag_TC0(DMA1);  // Clear the interrupt flag

        // Move to the next string in the buffer
        buffer_tail = (buffer_tail + 1) % BUFFER_SIZE;

        if (buffer_tail != buffer_head)  // More strings in the buffer
        {
            size_t string_length = strlen(dma_uart_buffer[buffer_tail]);
            LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_0, string_length);
            LL_DMA_ConfigAddresses(DMA1, LL_DMA_STREAM_0,
                                   (uint32_t)dma_uart_buffer[buffer_tail],
                                   LL_USART_DMA_GetRegAddr(UART5, LL_USART_DMA_REG_DATA_TRANSMIT),
                                   LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
            LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_0);  // Start next transmission
        }
        else
        {
            dma_busy = 0;  // Buffer is empty, DMA is idle
        }
    }
}

