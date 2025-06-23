inline void SPI4_SendByte(uint8_t data);
inline uint8_t SPI4_ReceiveByte(void);
uint8_t SPI4_ReadStatusRegister(void);
//void SPI4_ReadConfigSafety(void);
void SPI4_WriteEnable(void);
void SPI4_SetStatusConfig(void);
void SPI4_ReadVolatileRegister(void);
void SPI4_WriteVolatRegEnableBuff(void);
void SPI4_WriteVolatRegDisableBuff(void);
void SPI4_PageProgram(uint32_t address, uint8_t *data, uint8_t length);
void SPI4_PageWrite(uint32_t address, uint8_t *data, uint8_t length);
void SPI4_ReadData(uint32_t address, uint8_t *buffer, uint32_t length);
void SPI4_PageErase(uint32_t address);
void SPI4_SectorErase(uint32_t address);
void SPI4_BlockErase(uint32_t address);
void SPI4_ChipErase(void);
void SPI4_PageWriteFault(uint32_t ind_, uint32_t array_[][2]);
void SPI4_ReadDataFaultRegion(uint32_t address, uint32_t nm_fault);
inline extern void delayA_1us(uint32_t us);
float SPI4_ReadDataSetting(uint32_t address);
uint8_t Rec_Dat_to_EEp_f(uint32_t setting_code);
void convert_timestamp_to_date_string(time_t timestamp, char *date_string, size_t size);

extern inline void SPI4_SendByte(uint8_t data) {
    for (int i = 7; i >= 0; i--) {
        if (data & (1 << i)) {
            set_(SPI4_MOSI_P);
        } else {
            res_(SPI4_MOSI_P);
        }
        set_(SPI4_CLK_P);
        delayA_1us(1);
        res_(SPI4_CLK_P);
        delayA_1us(1);
    }
}

extern inline uint8_t SPI4_ReceiveByte(void) {
    uint8_t data = 0;
    for (int i = 7; i >= 0; i--) {
        set_(SPI4_CLK_P);  // Raise clock
        delayA_1us(1);
        if (isInSet_(SPI4_MISO_P)) {
            data |= (1 << i);  // Read bit
        }
        res_(SPI4_CLK_P);  // Lower clock
        delayA_1us(1);
    }
    return data;
}

uint8_t SPI4_ReadStatusRegister(void) {
	uint8_t status_=0;
    res_(CS_M95P32);
    delayA_1us(10);

    SPI4_SendByte(CMD_RDSR);
    status_ = SPI4_ReceiveByte();

    set_(CS_M95P32);
    return status_;
}

//void SPI4_ReadConfigSafety(void) {
//    uint8_t command = CMD_RDCR;
//
//    res_(CS_M95P32);  // Activate chip select
//    delayA_1us(10);
//
//    // Send Read Config & Safety Register Command (0x15)
//    SPI4_SendByte(command);
//
//    // Read Configuration Register Byte
//    EEP_reg_conf = SPI4_ReceiveByte();
//
//    // Read Safety Register Byte
//    EEP_reg_safety = SPI4_ReceiveByte();
//
//    set_(CS_M95P32);  // Deactivate chip select
//}

void SPI4_WriteEnable(void) {
    res_(CS_M95P32);
    delayA_1us(10);

    SPI4_SendByte(CMD_WREN);

    set_(CS_M95P32);
}

void SPI4_SetStatusConfig(void) {
    // Step 1: Enable Write
    SPI4_WriteEnable();

    res_(CS_M95P32);
    delayA_1us(10);

    SPI4_SendByte(CMD_WRSR);  // Send Write Status Config Register command
    // Status Register
    SPI4_SendByte(0x00);  // disable all block protection
    // Configuration Register
    SPI4_SendByte(0b0100001);  // b5=0, b6=1, buffer strength. b0=1, LID, locks the identification page.

    set_(CS_M95P32);

    while (SPI4_ReadStatusRegister() & 0x01) {
        delayA_1us(10);
    }
}

void SPI4_ReadVolatileRegister(void) {
    res_(CS_M95P32);
    delayA_1us(10);

    SPI4_SendByte(CMD_RDVR);  // Send Read Volatile Register Command
    EEP_reg_volatile = SPI4_ReceiveByte();

    set_(CS_M95P32);
}

void SPI4_WriteVolatRegEnableBuff(void) {
    SPI4_WriteEnable();

    res_(CS_M95P32);
    delayA_1us(10);

    SPI4_SendByte(CMD_WRVR);  // Send Write Volatile Register Command
    SPI4_SendByte(0b10);      // Send New Register Value

    set_(CS_M95P32);  // CS must go HIGH to execute the command

    while (SPI4_ReadStatusRegister() & 0x01) {
        delayA_1us(10);
    }
}

void SPI4_WriteVolatRegDisableBuff(void) {
    SPI4_WriteEnable();

    res_(CS_M95P32);
    delayA_1us(10);

    SPI4_SendByte(CMD_WRVR);  // Send Write Volatile Register Command
    SPI4_SendByte(0b00);      // Send New Register Value

    set_(CS_M95P32);  // CS must go HIGH to execute the command

    while (SPI4_ReadStatusRegister() & 0x01) {
        delayA_1us(10);
    }
}

void SPI4_PageProgram(uint32_t address, uint8_t *data, uint8_t length) {
    // Step 1: Enable Write
    SPI4_WriteEnable();

    res_(CS_M95P32);  // Activate chip select
    delayA_1us(10);

    // Step 2: Send Page Program Instruction (0x0A)
    SPI4_SendByte(CMD_PGPR);

    // Step 3: Send 24-bit Address (MSB First)
    SPI4_SendByte((address >> 16) & 0xFF);  // A23-A16
    SPI4_SendByte((address >> 8) & 0xFF);   // A15-A8
    SPI4_SendByte(address & 0xFF);          // A7-A0

    // Step 4: Send Data Bytes (MSB First)
    for (uint8_t i = 0; i < length; i++)
    {
        SPI4_SendByte(data[i]);
    }

    set_(CS_M95P32);  // End Transmission

    while (SPI4_ReadStatusRegister() & 0x01) {
        delayA_1us(10);
    }
}

void SPI4_PageWrite(uint32_t address, uint8_t *data, uint8_t length) {
    // Step 1: Enable Write
    SPI4_WriteEnable();

    res_(CS_M95P32);  // Activate chip select
    delayA_1us(10);

    // Step 2: Send Page Write Instruction (0x0A)
    SPI4_SendByte(CMD_PGWR);

    // Step 3: Send 24-bit Address (MSB First)
    SPI4_SendByte((address >> 16) & 0xFF);  // A23-A16
    SPI4_SendByte((address >> 8) & 0xFF);   // A15-A8
    SPI4_SendByte(address & 0xFF);          // A7-A0

    // Step 4: Send Data Bytes (MSB First)
    for (uint8_t i = 0; i < length; i++)
    {
        SPI4_SendByte(data[i]);
    }

    set_(CS_M95P32);  // End Transmission

    while (SPI4_ReadStatusRegister() & 0x01) {
        delayA_1us(10);
    }
}

void SPI4_ReadData(uint32_t address, uint8_t *buffer, uint32_t length) {
    res_(CS_M95P32);  // Activate chip select
    delayA_1us(10);

    // Step 1: Send Read Data Single Output Instruction (0x03)
    SPI4_SendByte(CMD_READ);

    // Step 2: Send 24-bit Address (MSB First)
    SPI4_SendByte((address >> 16) & 0xFF);  // A23-A16
    SPI4_SendByte((address >> 8) & 0xFF);   // A15-A8
    SPI4_SendByte(address & 0xFF);          // A7-A0

    // Step 3: Read Data Bytes Sequentially
    for (uint32_t i = 0; i < length; i++)
    {
        buffer[i] = SPI4_ReceiveByte();
    }

    set_(CS_M95P32);  // Deactivate chip select
}


void SPI4_PageErase(uint32_t address) { // 512byte 1.1 ms
    SPI4_WriteEnable();

    res_(CS_M95P32);
    delayA_1us(10);

    SPI4_SendByte(CMD_PGER);  // Send Page Erase Command
    // Step 2: Send 24-bit Address (MSB First)
    SPI4_SendByte((address >> 16) & 0xFF);  // A23-A16
    SPI4_SendByte((address >> 8) & 0xFF);   // A15-A8
    SPI4_SendByte(address & 0xFF);          // A7-A0

    set_(CS_M95P32);  // CS must go HIGH to execute the command
    while (SPI4_ReadStatusRegister() & 0x01) {
        delayA_1us(10);
    }
}
void SPI4_SectorErase(uint32_t address) { // 4kbyte 1.3 ms
    SPI4_WriteEnable();

    res_(CS_M95P32);
    delayA_1us(10);

    SPI4_SendByte(CMD_SCER);  // Send Sector Erase Command
    // Step 2: Send 24-bit Address (MSB First)
    SPI4_SendByte((address >> 16) & 0xFF);  // A23-A16
    SPI4_SendByte((address >> 8) & 0xFF);   // A15-A8
    SPI4_SendByte(address & 0xFF);          // A7-A0

    set_(CS_M95P32);  // CS must go HIGH to execute the command
    while (SPI4_ReadStatusRegister() & 0x01) {
        delayA_1us(10);
    }
}

void SPI4_BlockErase(uint32_t address) { // 64kbyte 4 ms
    SPI4_WriteEnable();

    res_(CS_M95P32);
    delayA_1us(10);

    SPI4_SendByte(CMD_BKER);  // Send Block Erase Command
    // Step 2: Send 24-bit Address (MSB First)
    SPI4_SendByte((address >> 16) & 0xFF);  // A23-A16
    SPI4_SendByte((address >> 8) & 0xFF);   // A15-A8
    SPI4_SendByte(address & 0xFF);          // A7-A0

    set_(CS_M95P32);  // CS must go HIGH to execute the command
    while (SPI4_ReadStatusRegister() & 0x01) {
        delayA_1us(10);
    }
}

void SPI4_ChipErase(void) { // 4Mbyte 15 ms
    SPI4_WriteEnable();

    res_(CS_M95P32);
    delayA_1us(10);

    SPI4_SendByte(CMD_CHER);  // Send Chip Erase Command

    set_(CS_M95P32);  // CS must go HIGH to execute the command
    while (SPI4_ReadStatusRegister() & 0x01) {
        delayA_1us(10);
    }
}

uint8_t Rec_Dat_to_EEp_f(uint32_t setting_code) {
    float setting_val=EpD[setting_code][0].V1;
    uint8_t float_bytes[4]; // Buffer to hold float bytes
    memcpy(float_bytes, &setting_val, sizeof(float)); // Convert float to 4 bytes

    uint32_t address = (uint32_t) EpD[0][0].V1 + setting_code * 8;

    // Step 1: Enable Write
    SPI4_WriteEnable();

    res_(CS_M95P32);  // Activate chip select
    delayA_1us(10);

    // Step 2: Send Page Write Instruction (0x0A)
    SPI4_SendByte(CMD_PGWR);

    // Step 3: Send 24-bit Address (MSB First)
    SPI4_SendByte((address >> 16) & 0xFF);  // A23-A16
    SPI4_SendByte((address >> 8) & 0xFF);   // A15-A8
    SPI4_SendByte(address & 0xFF);          // A7-A0

    // Step 4: Send First uint32_t (MSB First) - Setting Code
    for (int i = 3; i >= 0; i--) {
        SPI4_SendByte((setting_code >> (i * 8)) & 0xFF);
    }

    // Step 5: Send Second uint32_t (MSB First) - Setting Value
    for (int i = 3; i >= 0; i--) {
        SPI4_SendByte((float_bytes[i]) & 0xFF);
    }

    set_(CS_M95P32);  // End Transmission

    // Wait for Write Operation to Complete
    while (SPI4_ReadStatusRegister() & 0x01) {
        delayA_1us(10);
    }

    float verify_val;
    verify_val=SPI4_ReadDataSetting(address);

    // Step 7: Check if the write was successful
if (verify_val == setting_val) {
	PRF_GEN("Saved %s write %f read %f", Eep_data_Names[setting_code], setting_val, verify_val);
	return 1;
} else {
	PRF_GEN("Save Failed %s write %f read %f", Eep_data_Names[setting_code], setting_val, verify_val);
	return 0;
	Eeprom_fc=1;
}
}

void write_Dat_to_EEp_fn(void) {
    for (uint32_t setting_code = 0; setting_code < NUM_SET_ENUM; setting_code++) {
        float setting_val = EpD[setting_code][0].V1;
        uint8_t float_bytes[4];
        memcpy(float_bytes, &setting_val, sizeof(float));

        uint32_t address = (uint32_t)EpD[0][0].V1 + setting_code * 8;

        // Enable Write
        SPI4_WriteEnable();

        res_(CS_M95P32);
        delayA_1us(10);

        // Send Page Write Instruction
        SPI4_SendByte(CMD_PGWR);

        // Send 24-bit Address (MSB First)
        SPI4_SendByte((address >> 16) & 0xFF);
        SPI4_SendByte((address >> 8) & 0xFF);
        SPI4_SendByte(address & 0xFF);

        // Send First uint32_t (MSB First) - Setting Code
        for (int i = 3; i >= 0; i--) {
            SPI4_SendByte((setting_code >> (i * 8)) & 0xFF);
        }

        // Send Second uint32_t (MSB First) - Setting Value
        for (int i = 3; i >= 0; i--) {
            SPI4_SendByte(float_bytes[i]);
        }

        set_(CS_M95P32);

        // Wait for Write Operation to Complete
        while (SPI4_ReadStatusRegister() & 0x01) {
            delayA_1us(10);
        }
    }
}


void SPI4_EEP_ReadDataSettingsRegion(uint32_t address, uint32_t nm_fault) {
	uint32_t read32bit1;
	uint32_t read32bit2;
    res_(CS_M95P32);  // Activate chip select
    delayA_1us(10);
    SPI4_SendByte(CMD_READ);

		SPI4_SendByte((address >> 16) & 0xFF);  // A23-A16
		SPI4_SendByte((address >> 8) & 0xFF);   // A15-A8
		SPI4_SendByte(address & 0xFF);          // A7-A0

		for (uint32_t i = 0; i < NUM_SET_ENUM; i++) {
			read32bit1 = SPI4_ReceiveByte() << 24 | SPI4_ReceiveByte() << 16 | SPI4_ReceiveByte() << 8 | SPI4_ReceiveByte();
			read32bit2 = SPI4_ReceiveByte() << 24 | SPI4_ReceiveByte() << 16 | SPI4_ReceiveByte() << 8 | SPI4_ReceiveByte();

			if (read32bit1 != 0xffffffff && read32bit1 < 1000) {
				EpD[i][0].setting_id = read32bit1;
				EpD[i][1].setting_id = read32bit1;
				uint32_t float_raw=read32bit2;
				float float_value;
				memcpy(&float_value, &float_raw, sizeof(float));
				EpD[i][0].V1 = float_value;
				EpD[i][1].V1 = float_value;
			} else {
				PRF_GEN("\n\n\n\n\nread eeprom data id higher than 1000 %lu %lu %f", read32bit2, EpD[i][0].setting_id, EpD[i][0].V1);
			}
		}
    set_(CS_M95P32);  // Deactivate chip select
}

float SPI4_ReadDataSetting(uint32_t address) {
//	uint32_t read32bit1;
	uint32_t read32bit2;
    res_(CS_M95P32);  // Activate chip select
    delayA_1us(10);

    SPI4_SendByte(CMD_READ);

		SPI4_SendByte((address >> 16) & 0xFF);  // A23-A16
		SPI4_SendByte((address >> 8) & 0xFF);   // A15-A8
		SPI4_SendByte(address & 0xFF);          // A7-A0

			SPI4_ReceiveByte();
			SPI4_ReceiveByte();
			SPI4_ReceiveByte();
			SPI4_ReceiveByte();
			read32bit2 = SPI4_ReceiveByte() << 24 | SPI4_ReceiveByte() << 16 | SPI4_ReceiveByte() << 8 | SPI4_ReceiveByte();

			uint32_t float_raw=read32bit2;
			float float_value;
			memcpy(&float_value, &float_raw, sizeof(float));

    set_(CS_M95P32);  // Deactivate chip select
			return float_value;
}

void Record_Fault_Code(uint32_t state_code) {

static uint32_t prev_fault_code=0;


if (prev_fault_code!=state_code) {

array_fault_data[flt_array_index_next][0]=rtc_timestamp;
array_fault_data[flt_array_index_next][1]=state_code;

uint32_t address = flt_array_index_next * 8;
uint32_t timestamp = array_fault_data[flt_array_index_next][0];
flt_array_index_last=flt_array_index_next;
flt_disp_index=(flt_array_index_last-5+NUM_FAULT_RECORD)%NUM_FAULT_RECORD;
flt_array_index_next=(flt_array_index_next+1) % NUM_FAULT_RECORD;
//PRF_GEN("c flt indx next %u flt indx last %u flt_disp_index %u", flt_array_index_next, flt_array_index_last, flt_disp_index);

    // Step 1: Enable Write
    SPI4_WriteEnable();

    res_(CS_M95P32);  // Activate chip select
    delayA_1us(10);

    // Step 2: Send Page Write Instruction (0x0A)
    SPI4_SendByte(CMD_PGWR);

    // Step 3: Send 24-bit Address (MSB First)
    SPI4_SendByte((address >> 16) & 0xFF);  // A23-A16
    SPI4_SendByte((address >> 8) & 0xFF);   // A15-A8
    SPI4_SendByte(address & 0xFF);          // A7-A0

    // Step 4: Send First uint32_t (MSB First)
    for (int i = 3; i >= 0; i--)
    {
        SPI4_SendByte((timestamp >> (i * 8)) & 0xFF);
    }

    // Step 5: Send Second uint32_t (MSB First)
    for (int i = 3; i >= 0; i--)
    {
        SPI4_SendByte((state_code >> (i * 8)) & 0xFF);
    }

    set_(CS_M95P32);  // End Transmission

    PRF_GEN("Saved state_code %lu flt %s", state_code, state_list[state_code].name);

    while (SPI4_ReadStatusRegister() & 0x01) {
        delayA_1us(10);
    }

    // show unseen fault code warning
	if (currentPage == !FAULT_CODES_REPORT_pg && there_is_past_unseen_fault==0) {
		EpD[SET_UNSEEN_FLT][0].V1=1;
		Rec_Dat_to_EEp_f(SET_UNSEEN_FLT);
		there_is_past_unseen_fault=1;
	}
}
	prev_fault_code=state_code;
}

void SPI4_ReadDataFaultRegion(uint32_t address, uint32_t nm_fault) {
    res_(CS_M95P32);  // Activate chip select
    delayA_1us(10);
    // Step 1: Send Read Data Single Output Instruction (0x03)
    SPI4_SendByte(CMD_READ);

		// Step 2: Send 24-bit Address (MSB First)
		SPI4_SendByte((address >> 16) & 0xFF);  // A23-A16
		SPI4_SendByte((address >> 8) & 0xFF);   // A15-A8
		SPI4_SendByte(address & 0xFF);          // A7-A0

		// Update array_fault_data
		for (uint32_t i = 0; i < nm_fault; i++) {
			array_fault_data[i][0] = SPI4_ReceiveByte() << 24 | SPI4_ReceiveByte() << 16 | SPI4_ReceiveByte() << 8 | SPI4_ReceiveByte();
			array_fault_data[i][1] = SPI4_ReceiveByte() << 24 | SPI4_ReceiveByte() << 16 | SPI4_ReceiveByte() << 8 | SPI4_ReceiveByte();
			PRF_EEPFLT("i %d array_fault_data[i][0] %lu array_fault_data[i][1] %lu", array_fault_data[i][0], array_fault_data[i][1]);
		}
    set_(CS_M95P32);  // Deactivate chip select
}

void printFaultCodes(void) {
//	SPI4_ReadDataFaultRegion(FAULT_RECORD_START_ADDRESS, NUM_FAULT_RECORD);
for (int i = 0; i < NUM_FAULT_RECORD-1; i++) {
	convert_timestamp_to_date_string(array_fault_data[i][0], rtc_timestring, sizeof(rtc_timestring));
    if (array_fault_data[i][1] < NUM_STATE_NAMES) {
        PRF_GEN("%s %s", rtc_timestring, state_list[array_fault_data[i][1]].name);
    } else {
        PRF_GEN("%s Bos Kayit", rtc_timestring);
    }
    delay_1ms(1);
}
//PRF_GEN("c flt indx next %u flt indx last %u flt_disp_index %u", flt_array_index_next, flt_array_index_last, flt_disp_index);
}

void print_Eep_data_f(void) {
//	SPI4_EEP_ReadDataSettingsRegion(3145728, NUM_SET_ENUM);
    for (int i = 0; i < NUM_SET_ENUM; i++) {
		PRF_GEN("s1 %d %s \t%f", i, Eep_data_Names[EpD[i][0].setting_id], EpD[i][0].V1);
		delay_1ms(1);
    }
}
// 512 byte				0-1FF
// next 512 byte		200-3FF

// test enabling page write buffer

//This must be done to enable writing to any register in the chip:
//CS low, send write enable command, CS high
//The write enable (WREN) instruction sets the write
//enable latch (WEL) bit in the status register to 1.

//Status register is writable (if the WREN
//instruction has set the WEL bit)
//• BP2, BP1, BP0, and TB bits can be changed


