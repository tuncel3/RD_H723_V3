
uint8_t Dec2BCD(uint8_t decimal_value)
{
    uint8_t tens = decimal_value / 10;  // Extract tens place
    uint8_t ones = decimal_value % 10;  // Extract ones place
    return (tens << 4) | ones;          // Combine tens and ones in BCD format
}

uint8_t Write_To_Register(uint8_t register_address, uint8_t data_byte) {
    uint8_t data[2] = {register_address, data_byte};  // Register address and data byte
    uint32_t timeout = 5500000;  // Timeout duration (adjust as needed)

    // Wait until I2C bus is not busy
    while (LL_I2C_IsActiveFlag_BUSY(I2C2));

    // Generate START condition and send the slave address in WRITE mode
    LL_I2C_HandleTransfer(I2C2, MCP7940N_I2C_ADDRESS, LL_I2C_ADDRSLAVE_7BIT, 2, LL_I2C_MODE_SOFTEND, LL_I2C_GENERATE_START_WRITE);

    for (int i = 0; i < 2; i++)  // Send the register address and data byte
    {
        // Wait until TXE (Transmit Data Register Empty) flag is set
        while (!LL_I2C_IsActiveFlag_TXE(I2C2))
        {
            // Check for NACK
            if (LL_I2C_IsActiveFlag_NACK(I2C2))
            {
                LL_I2C_ClearFlag_NACK(I2C2);  // Clear NACK flag
                LL_I2C_GenerateStopCondition(I2C2);  // Send STOP condition
                return 0;  // Exit function on failure
            }
        }
        LL_I2C_TransmitData8(I2C2, data[i]);  // Transmit data (register address or data byte)
    }

    // Wait until Transfer Complete (TC) flag is set
    while (!LL_I2C_IsActiveFlag_TC(I2C2))
    {
        // Check for NACK
        if (LL_I2C_IsActiveFlag_NACK(I2C2))
        {
            LL_I2C_ClearFlag_NACK(I2C2);  // Clear NACK flag
            LL_I2C_GenerateStopCondition(I2C2);  // Send STOP condition
            return 0;  // Exit function on failure
        }
    }

    // Send STOP condition
    LL_I2C_GenerateStopCondition(I2C2);

    // Wait for STOP condition to be cleared
    while (LL_I2C_IsActiveFlag_STOP(I2C2) && timeout--);
    if (timeout == 0)
    {
        return 0;  // Timeout error
    }

    // Clear the STOP flag
    LL_I2C_ClearFlag_STOP(I2C2);

    return 1;  // Success
}

uint8_t Read_Register(uint8_t register_address) {
//    uint8_t register_address = 0x00;  // Register address (RTCSEC)
    uint8_t data_byte = 0;            // Data to be read from register

    // Wait until I2C bus is not busy
    while (LL_I2C_IsActiveFlag_BUSY(I2C2));

    // Generate START condition and send the slave address in WRITE mode
    LL_I2C_HandleTransfer(I2C2, MCP7940N_I2C_ADDRESS, LL_I2C_ADDRSLAVE_7BIT, 1, LL_I2C_MODE_SOFTEND, LL_I2C_GENERATE_START_WRITE);

    // Wait until TXE flag is set (ready to transmit the register address)
    while (!LL_I2C_IsActiveFlag_TXE(I2C2));

    // Send the register address
    LL_I2C_TransmitData8(I2C2, register_address);

    // Wait until TC (Transfer Complete) flag is set
    while (!LL_I2C_IsActiveFlag_TC(I2C2));

    // Now send a repeated START condition and switch to READ mode
    LL_I2C_HandleTransfer(I2C2, MCP7940N_I2C_ADDRESS, LL_I2C_ADDRSLAVE_7BIT, 1, LL_I2C_MODE_SOFTEND, LL_I2C_GENERATE_START_READ);
    while (!LL_I2C_IsActiveFlag_RXNE(I2C2));

    // Receive the data byte
    data_byte = LL_I2C_ReceiveData8(I2C2);

    // Generate a STOP condition
    LL_I2C_GenerateStopCondition(I2C2);

    uint32_t timeout = 5500000;  // Adjust timeout duration as needed
    while (LL_I2C_IsActiveFlag_STOP(I2C2) && timeout--);
    if (timeout == 0)
    {
        return 0;  // Timeout error
    }
    LL_I2C_ClearFlag_STOP(I2C2);

    return data_byte;
}

uint8_t Write_RTC_Time_Data_With_Oscillator(void)
{
    uint8_t register_address = 0x00;  // Register 0x00 (RTCSEC)
    uint32_t timeout = 5500000;      // Timeout duration (adjust as needed)

    PRF_GEN("s1");
    // Step 1: Stop oscillator by sending 0 to register 0x00
    while (LL_I2C_IsActiveFlag_BUSY(I2C2));
    LL_I2C_HandleTransfer(I2C2, MCP7940N_I2C_ADDRESS, LL_I2C_ADDRSLAVE_7BIT, 2, LL_I2C_MODE_SOFTEND, LL_I2C_GENERATE_START_WRITE);

    PRF_GEN("s2");
    // Send the register address and data (0)
    while (!LL_I2C_IsActiveFlag_TXE(I2C2));
    LL_I2C_TransmitData8(I2C2, register_address);

    PRF_GEN("s3");
    while (!LL_I2C_IsActiveFlag_TXE(I2C2));
    LL_I2C_TransmitData8(I2C2, 0x00);  // Write 0 to stop the oscillator

    PRF_GEN("s4");
    while (!LL_I2C_IsActiveFlag_TC(I2C2));
    LL_I2C_GenerateStopCondition(I2C2);

    PRF_GEN("s5");
    timeout = 5500000;  // Reset timeout
    while (!LL_I2C_IsActiveFlag_STOP(I2C2) && timeout--);
    if (timeout == 0)
    {
        return 0;  // Timeout error
    }
    LL_I2C_ClearFlag_STOP(I2C2);


    PRF_GEN("s6");
    // Step 2: Write 7 elements (minute to year) with special handling for weekday/config
    while (LL_I2C_IsActiveFlag_BUSY(I2C2));
    LL_I2C_HandleTransfer(I2C2, MCP7940N_I2C_ADDRESS, LL_I2C_ADDRSLAVE_7BIT, 7, LL_I2C_MODE_SOFTEND, LL_I2C_GENERATE_START_WRITE);

    PRF_GEN("s7");
    // Send the starting register address (0x01)
    while (!LL_I2C_IsActiveFlag_TXE(I2C2));
    LL_I2C_TransmitData8(I2C2, register_address + 1);  // Start from register 0x01

    PRF_GEN("s8");
    for (uint8_t i = 1; i <= 6; i++)
    {
        while (!LL_I2C_IsActiveFlag_TXE(I2C2));
        if (i == 1) {
            LL_I2C_TransmitData8(I2C2, Dec2BCD(rtc_min_edit));
        }
        else if (i == 2) {
            LL_I2C_TransmitData8(I2C2, Dec2BCD(rtc_hour_edit));
        }
        else if (i == 3)  // weekday/config byte
        {
            LL_I2C_TransmitData8(I2C2, 0b1000);  // Send fixed value for weekday/config
        }
        else if (i == 4) {
            LL_I2C_TransmitData8(I2C2, Dec2BCD(rtc_day_edit));
        }
        else if (i == 5) {
            LL_I2C_TransmitData8(I2C2, Dec2BCD(rtc_month_edit));
        }
        else if (i == 6) {
            LL_I2C_TransmitData8(I2C2, Dec2BCD(rtc_year_edit));
        }
    }

    PRF_GEN("s9");
    while (!LL_I2C_IsActiveFlag_TC(I2C2));
    LL_I2C_GenerateStopCondition(I2C2);

    PRF_GEN("s10");
    timeout = 5500000;  // Reset timeout
    while (!LL_I2C_IsActiveFlag_STOP(I2C2) && timeout--);
    if (timeout == 0)
    {
        return 0;  // Timeout error
    }
    LL_I2C_ClearFlag_STOP(I2C2);


    PRF_GEN("s11");
    // Step 3: Re-enable oscillator and set the seconds value
    while (LL_I2C_IsActiveFlag_BUSY(I2C2));
    LL_I2C_HandleTransfer(I2C2, MCP7940N_I2C_ADDRESS, LL_I2C_ADDRSLAVE_7BIT, 2, LL_I2C_MODE_SOFTEND, LL_I2C_GENERATE_START_WRITE);

    PRF_GEN("s12");
    // Send the register address (0x00)
    while (!LL_I2C_IsActiveFlag_TXE(I2C2));
    LL_I2C_TransmitData8(I2C2, register_address);

    PRF_GEN("s13");
    // Send the seconds value with the oscillator bit set (bit 7)
    while (!LL_I2C_IsActiveFlag_TXE(I2C2));
    LL_I2C_TransmitData8(I2C2, Dec2BCD(rtc_sec_edit) | 0b10000000);

    PRF_GEN("s14");
    while (!LL_I2C_IsActiveFlag_TC(I2C2));
    LL_I2C_GenerateStopCondition(I2C2);

    PRF_GEN("s15");
    timeout = 5500000;  // Reset timeout
    while (LL_I2C_IsActiveFlag_STOP(I2C2) && timeout--);
    if (timeout == 0)
    {
        return 0;  // Timeout error
    }
    LL_I2C_ClearFlag_STOP(I2C2);

    PRF_GEN("s16");
    return 1;  // Success
}


uint8_t Read_RTC_Osc_Status(void) {
    uint8_t register_address = 0x00;  // Register address (RTCSEC)
    uint8_t data_byte = 0;            // Data to be read from register

    // Wait until I2C bus is not busy
    while (LL_I2C_IsActiveFlag_BUSY(I2C2));

    // Generate START condition and send the slave address in WRITE mode
    LL_I2C_HandleTransfer(I2C2, MCP7940N_I2C_ADDRESS, LL_I2C_ADDRSLAVE_7BIT, 1, LL_I2C_MODE_SOFTEND, LL_I2C_GENERATE_START_WRITE);

    // Wait until TXE flag is set (ready to transmit the register address)
    while (!LL_I2C_IsActiveFlag_TXE(I2C2));

    // Send the register address
    LL_I2C_TransmitData8(I2C2, register_address);

    // Wait until TC (Transfer Complete) flag is set
    while (!LL_I2C_IsActiveFlag_TC(I2C2));

    // Now send a repeated START condition and switch to READ mode
    LL_I2C_HandleTransfer(I2C2, MCP7940N_I2C_ADDRESS, LL_I2C_ADDRSLAVE_7BIT, 1, LL_I2C_MODE_SOFTEND, LL_I2C_GENERATE_START_READ);
    while (!LL_I2C_IsActiveFlag_RXNE(I2C2));

    // Receive the data byte
    data_byte = LL_I2C_ReceiveData8(I2C2);

    // Generate a STOP condition
    LL_I2C_GenerateStopCondition(I2C2);

    uint32_t timeout = 5500000;  // Adjust timeout duration as needed
    while (LL_I2C_IsActiveFlag_STOP(I2C2) && timeout--);
    if (timeout == 0)
    {
        return 0;  // Timeout error
    }
    LL_I2C_ClearFlag_STOP(I2C2);

    // Check if bit 7 is set in the received data
    if (data_byte & 0b10000000)
    {
        return 1;  // Oscillator is enabled
    }
    else
    {
        return 0;  // Oscillator is not enabled
    }
}


uint8_t Read_Register_0x00_to_0x06(void) {
    uint8_t register_address = 0x00;  // Starting register address (RTCSEC)
    uint8_t raw_byte = 0;            // Temporary variable to hold raw seconds data
    uint8_t i = 0;
    while (LL_I2C_IsActiveFlag_BUSY(I2C2));
    LL_I2C_HandleTransfer(I2C2, MCP7940N_I2C_ADDRESS, LL_I2C_ADDRSLAVE_7BIT, 1, LL_I2C_MODE_SOFTEND, LL_I2C_GENERATE_START_WRITE);
    while (!LL_I2C_IsActiveFlag_TXE(I2C2));	// Wait until TXE flag is set (ready to transmit the register address)
    LL_I2C_TransmitData8(I2C2, register_address);	// Send the register address

    while (!LL_I2C_IsActiveFlag_TC(I2C2));	// Wait until TC (Transfer Complete) flag is set
    // Now send a repeated START condition and switch to READ mode
    LL_I2C_HandleTransfer(I2C2, MCP7940N_I2C_ADDRESS, LL_I2C_ADDRSLAVE_7BIT, 7, LL_I2C_MODE_SOFTEND, LL_I2C_GENERATE_START_READ);
    for (i = 0; i < 7; i++) { // Read 6 bytes (registers 0x00 to 0x05)
        while (!LL_I2C_IsActiveFlag_RXNE(I2C2)) { asm("nop");}	// Wait for data to be received
		if (i < 6) {
	        raw_byte = LL_I2C_ReceiveData8(I2C2); // Store received data	receivedbyte &= 0x7F;
			if (i == 0) {
				rtc_sec_recv = (((raw_byte >> 4) & 0x07) * 10) + (raw_byte & 0x0F);
			}
			if (i == 1) {
				rtc_min_recv = (((raw_byte >> 4) & 0x07) * 10) + (raw_byte & 0x0F);
			}
			if (i == 2) {
				rtc_hour_recv = (((raw_byte >> 4) & 0x03) * 10) + (raw_byte & 0x0F);
			}
			if (i == 4) {	// skipped i==3. it is weekday. array is 6 elements so substracting 1 from next array indexes.
				rtc_day_recv = (((raw_byte >> 4) & 0x03) * 10) + (raw_byte & 0x0F);
			}
			if (i == 5) {
				rtc_month_recv = (((raw_byte >> 4) & 0x01) * 10) + (raw_byte & 0x0F);
			}
	        LL_I2C_AcknowledgeNextData(I2C2, LL_I2C_ACK); // Send ACK to continue receiving the next byte
		} else {
			raw_byte = LL_I2C_ReceiveData8(I2C2); // Store the last byte
			rtc_year_recv = (((raw_byte >> 4) & 0x0F) * 10) + (raw_byte & 0x0F);
			LL_I2C_GenerateStopCondition(I2C2); // Send STOP condition (no ACK after the last byte)
		}
    }
    LL_I2C_ClearFlag_STOP(I2C2);
    return 1;  // Success
}
