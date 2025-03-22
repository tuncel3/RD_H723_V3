

inline void delayA_1ms(uint32_t ms)
{
    // Adjusted iteration count for 1 ms at 550 MHz
    uint32_t iterations_per_ms = 540505;

    // Calculate total iterations needed for the given ms
    uint32_t total_iterations = iterations_per_ms * ms;

    __asm volatile (
        "1: subs %0, %0, #1 \n"  // Decrement the counter
        "   bne 1b         \n"   // If not zero, branch back to label 1
        : "+r" (total_iterations) // Input/output: total_iterations register
        :                        // No input-only operands
        : "cc"                   // Clobbers condition flags
    );
}


void extern prfm(char *string)
{
    // Move to a new line before sending a new message
    snprintf(dma_uart_buffer[buffer_head], MAX_STRING_LENGTH, "\n\r%s", string);

    // Move buffer head
    buffer_head = (buffer_head + 1) % BUFFER_SIZE;

    // Check for buffer overflow
    if (buffer_head == buffer_tail)
    {
        buffer_tail = (buffer_tail + 1) % BUFFER_SIZE; // Discard oldest data
    }

    // Reset uart_debug_cnt counter when a new string is sent
    prfm_r_counter = 0;

    // Start DMA transmission if not busy
    if (!dma_busy)
    {
        dma_busy = 1;
        size_t string_length = strlen(dma_uart_buffer[buffer_tail]);
        LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_0, string_length);
        LL_DMA_ConfigAddresses(DMA1, LL_DMA_STREAM_0,
                               (uint32_t)dma_uart_buffer[buffer_tail],
                               LL_USART_DMA_GetRegAddr(UART5, LL_USART_DMA_REG_DATA_TRANSMIT),
                               LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
        LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_0);
    }
}

