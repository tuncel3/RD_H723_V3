/*
 * KS0108_Driver.c
 *
 *  Created on: Aug 25, 2024
 *      Author: enest
 */


#include <_KS0108_Driver.h>
#include <string.h>

inline extern void delayA_1us_g(uint32_t us)
{
    // Adjusted iteration count for 1 ms at 550 MHz
    uint32_t iterations_per_us = 200;

    // Calculate total iterations needed for the given ms
    uint32_t total_iterations = iterations_per_us * us;

    __asm volatile (
        "1: subs %0, %0, #1 \n"  // Decrement the counter
        "   bne 1b         \n"   // If not zero, branch back to label 1
        : "+r" (total_iterations) // Input/output: total_iterations register
        :                        // No input-only operands
        : "cc"                   // Clobbers condition flags
    );
}

  uint8_t   DisplayBuffer[KS0108_BUFFER_LENGTH];
  uint16_t	Column;
  uint8_t   Row;
  uint8_t	State;
  uint8_t   TriggerCounter;
  uint8_t   Busy;
  stFonts_t *SelectedFont;

 //---------------------------------------------------------------------------/
 // Function Name : GLCD_WriteCommand																				 /
 // Description   : Send Command to GLCD Display                              /
 // Argument      : Command			                                             /
 // Return			   : None                                                      /
 //---------------------------------------------------------------------------/
//  void GLCD_WriteCommand(uint8_t Command) {
//      SET_MODE_COMMAND_WRITE;
//
//      // Define static constant arrays to avoid re-initialization
//      static GPIO_TypeDef * const ports[8] = {GPIOD, GPIOC, GPIOC, GPIOC, GPIOA, GPIOA, GPIOA, GPIOA};
//      static const uint16_t pins[8] = {LL_GPIO_PIN_0, LL_GPIO_PIN_12, LL_GPIO_PIN_11, LL_GPIO_PIN_10,
//                                       LL_GPIO_PIN_15, LL_GPIO_PIN_12, LL_GPIO_PIN_11, LL_GPIO_PIN_10};
//
//      // Write each bit efficiently
//      for (int i = 0; i < 8; i++) {
//          if (Command & (1 << i)) {
//              ports[i]->BSRR = pins[i];  // Set bit
//          } else {
//              ports[i]->BSRR = (pins[i] << 16);  // Reset bit
//          }
//      }
//
//      EN1_g;
////      delayA_1us_g(1); // Add delay if needed
//      EN0_g;
//  }
  static inline void GLCD_WriteCommand(uint8_t cmd)
  {
      /* 1) RS=0, RW=0  → Komut-yazma modu */
      SET_MODE_COMMAND_WRITE;     //  ←  burası RS’i 0’a çeker

      /* 2) Veri pinlerini ayarla (D0-D7) */
      GPIOA->BSRR = bsrrA[cmd];   // PA15/12/11/10
      GPIOC->BSRR = bsrrC[cmd];   // PC12/11/10
      GPIOD->BSRR = bsrrD[cmd];   // PD0

      /* 3) Setup gecikmesi (≥140 ns) */
      for (volatile int k = 0; k < 12; k++) __NOP__;   // ≈170 ns @72 MHz

      /* 4) EN darbesi (≥450 ns) */
      EN1_g;                                // /WR ↑
      for (volatile int k = 0; k < 16; k++) __NOP__;  // ≈225 ns
      EN0_g;                                // /WR ↓
  }




   //---------------------------------------------------------------------------/
   // Function Name : GLCD_WriteData																						 /
   // Description   : Send Data to KS0108 Display                               /
   // Argument      : Data			                                                 /
   // Return			   : None                                                      /
   //---------------------------------------------------------------------------/

//  void GLCD_WriteData(uint8_t Data) {
//      SET_MODE_DATA_WRITE;
//
//      // Corrected: Removed "const" before GPIO_TypeDef *
//      static GPIO_TypeDef * const ports[8] = {GPIOD, GPIOC, GPIOC, GPIOC, GPIOA, GPIOA, GPIOA, GPIOA};
//      static const uint16_t pins[8] = {LL_GPIO_PIN_0, LL_GPIO_PIN_12, LL_GPIO_PIN_11, LL_GPIO_PIN_10,
//                                       LL_GPIO_PIN_15, LL_GPIO_PIN_12, LL_GPIO_PIN_11, LL_GPIO_PIN_10};
//
//      // Write each bit efficiently
//      for (int i = 0; i < 8; i++) {
//          if (Data & (1 << i)) {
//              ports[i]->BSRR = pins[i];  // Set bit
//          } else {
//              ports[i]->BSRR = (pins[i] << 16);  // Reset bit
//          }
//      }
//
//      EN1_g;
//      delayA_1us_g(1);
//      EN0_g;
//  }

#include "glcd_bsrr_tables.h"   // ↩︎ otomatik üretilen diziler

static inline void GLCD_WriteData(uint8_t d)
{
	      SET_MODE_DATA_WRITE;
    GPIOA->BSRR = bsrrA[d];   // bit4-7  → PA
    GPIOC->BSRR = bsrrC[d];   // bit1-3  → PC
    GPIOD->BSRR = bsrrD[d];   // bit0    → PD
    delayA_1us_g(1);

    EN1_g;              // /WR yüksek
    //      delayA_1us_g(1);
    EN0_g;              // /WR düşük
}


   //---------------------------------------------------------------------------/
   // Function Name : KS0108_RefreshGRAM         			      				           /
   // Description   : Refresh GRAM of GLCD Display                              /
   // Argument      : None	                                                     /
   // Return			   : None                                                      /
   //---------------------------------------------------------------------------/
  void GLCD_RefreshGRAM(void)
  {
      uint8_t i, j;

      for (i = 0; i < 8; i++)          /* 8 sayfa (8×8 = 64 satır) */
      {
          /* ---- Çip-1: sol 64 sütun ---- */
          SELECT_FIRST_CHIP;

          GLCD_WriteCommand(X_BASE_ADDRESS | i);   /* page      */
          GLCD_WriteCommand(Y_BASE_ADDRESS);       /* column 0  */

          for (j = 0; j < 64; j++)                 /* 64 bayt   */
              GLCD_WriteData(DisplayBuffer[KS0108_WIDTH * i + j]);

          /* ---- Çip-2: sağ 64 sütun ---- */
          SELECT_SECOND_CHIP;

          GLCD_WriteCommand(X_BASE_ADDRESS | i);
          GLCD_WriteCommand(Y_BASE_ADDRESS);

          /* buffer adresi  +64  kaydırıldı   */
          for (j = 0; j < 64; j++)
              GLCD_WriteData(DisplayBuffer[KS0108_WIDTH * i + 64 + j]);
      }
  }



   //---------------------------------------------------------------------------/
   // Function Name : GLCD_ClearScreen         			      				             /
   // Description   : Clear GLCD Display Screen                                 /
   // Argument      : Pixel Mode (Dark/Bright)  			                           /
   // Return			   : None                                                      /
   //---------------------------------------------------------------------------/
   void GLCD_ClearScreen(uint8_t Fill){
   	uint16_t i;
   	for (i = 0; i < KS0108_BUFFER_LENGTH; i++)
     {
       DisplayBuffer[i] = Fill;
   	}
   }

   //---------------------------------------------------------------------------/
   // Function Name : GLCD_SetFont         			      				                 /
   // Description   : Set Selected Font                                         /
   // Argument      : Selected Font Address 			                               /
   // Return			   : None                                                      /
   //---------------------------------------------------------------------------/
   void  GLCD_SetFont(stFonts_t *Font){
     SelectedFont = Font;
   }

   //---------------------------------------------------------------------------/
   // Function Name : GLCD_Init         			      				                     /
   // Description   : KS0108 Initialization                                     /
   // Argument      : None 			                                               /
   // Return			   : None                                                      /
   //---------------------------------------------------------------------------/
   void GLCD_Init(void){
     RST0_g;
     RW0_g;
     EN0_g;
     CS10_g;
     CS20_g;
     RS0_g;
     State=0;
     Column=0;
     Row=0;
     TriggerCounter=0;
     Busy=0;
     RST1_g;
     SELECT_FIRST_CHIP;
     GLCD_WriteCommand(X_BASE_ADDRESS);		// First Chip First Row
     GLCD_WriteCommand(Y_BASE_ADDRESS);		// First Chip First Column
     GLCD_WriteCommand(START_LINE);				// First Chip First Column
     GLCD_WriteCommand(DISPLAY_ON);				// First Chip First Column

     SELECT_SECOND_CHIP;
     GLCD_WriteCommand(X_BASE_ADDRESS);		// Second Chip First Row
     GLCD_WriteCommand(Y_BASE_ADDRESS);		// Second Chip First Column
     GLCD_WriteCommand(START_LINE);				// Second Chip First Column
     GLCD_WriteCommand(DISPLAY_ON);				// Second Chip First Column
     GLCD_SetFont(&KS0108_DEFAULT_FONT);
     GLCD_ClearScreen(0x00);
     GLCD_RefreshGRAM();
   }
   void GLCD_Init_while(void){	// can be optimized
	     SELECT_FIRST_CHIP;
	     GLCD_WriteCommand(X_BASE_ADDRESS);		// First Chip First Row
	     GLCD_WriteCommand(Y_BASE_ADDRESS);		// First Chip First Column
	     GLCD_WriteCommand(START_LINE);				// First Chip First Column
	     GLCD_WriteCommand(DISPLAY_ON);				// First Chip First Column

	     SELECT_SECOND_CHIP;
	     GLCD_WriteCommand(X_BASE_ADDRESS);		// Second Chip First Row
	     GLCD_WriteCommand(Y_BASE_ADDRESS);		// Second Chip First Column
	     GLCD_WriteCommand(START_LINE);				// Second Chip First Column
	     GLCD_WriteCommand(DISPLAY_ON);				// Second Chip First Column
   }

   void GLCD_DisplayChar(uint8_t Xpos, uint8_t Yrow, uint16_t Chr, uint8_t Mode)
   {
       uint16_t BufferIndex, TableIndexBase;
       uint8_t i, j;
       // Calculate the memory index where the character will be written
       BufferIndex = (Yrow * KS0108_WIDTH) + Xpos;
       // Calculate the starting index of the character in the font table
       TableIndexBase = ((SelectedFont->Width * SelectedFont->Height * Chr) >> 3);
       // Process each column of the character in a loop
       for (i = 0; i < SelectedFont->Width; i++)
       {
           j = SelectedFont->Line; // Height
           if (Mode)
           {
               // Get pixel values from the selected font table and write to the display buffer
               while (j--)
               {
                   DisplayBuffer[BufferIndex + (j * KS0108_WIDTH) + i] = SelectedFont->table[TableIndexBase + (SelectedFont->Line * i) + j];
               }
           }
           else
           {
               // Inverse writing mode, get pixel values and write to the display buffer inverted
               while (j--)
               {
                   DisplayBuffer[BufferIndex + (j * KS0108_WIDTH) + i] = ~SelectedFont->table[TableIndexBase + (SelectedFont->Line * i) + j];
               }
           }
       }
   }

   void GLCD_DisplayString(uint8_t Xpos, uint8_t Yrow, const char *pString)
   {
	   uint8_t Chr=0;
       // Loop until the end of the string
       while (*pString != '\0')
       {
           // If Xpos is greater than the screen width (KS0108_WIDTH)
           // reset Xpos and move Yrow to the next line
           if (Xpos > (KS0108_WIDTH - (SelectedFont->Width >> 1)))
           {
               Xpos = 0;
               Yrow += SelectedFont->Line;
           }

           // Check for multi-byte characters
           if ((*pString == 196 && *(pString + 1) == 159) || // ğ
               (*pString == 195 && *(pString + 1) == 188) || // ü
               (*pString == 197 && *(pString + 1) == 159) || // ş
               (*pString == 196 && *(pString + 1) == 177) || // ı
               (*pString == 195 && *(pString + 1) == 182) || // ö
               (*pString == 195 && *(pString + 1) == 167) || // ç
               (*pString == 196 && *(pString + 1) == 158) || // Ğ
               (*pString == 195 && *(pString + 1) == 156) || // Ü
               (*pString == 197 && *(pString + 1) == 158) || // Ş
               (*pString == 196 && *(pString + 1) == 176) || // İ
               (*pString == 195 && *(pString + 1) == 150) || // Ö
               (*pString == 195 && *(pString + 1) == 135))   // Ç
           {
               if (*pString == 196 && *(pString + 1) == 159) Chr = 97;
               else if (*pString == 195 && *(pString + 1) == 188) Chr = 98;
               else if (*pString == 197 && *(pString + 1) == 159) Chr = 99;
               else if (*pString == 196 && *(pString + 1) == 177) Chr = 100;
               else if (*pString == 195 && *(pString + 1) == 182) Chr = 101;
               else if (*pString == 195 && *(pString + 1) == 167) Chr = 102;
               else if (*pString == 196 && *(pString + 1) == 158) Chr = 103;
               else if (*pString == 195 && *(pString + 1) == 156) Chr = 104;
               else if (*pString == 197 && *(pString + 1) == 158) Chr = 105;
               else if (*pString == 196 && *(pString + 1) == 176) Chr = 106;
               else if (*pString == 195 && *(pString + 1) == 150) Chr = 107;
               else if (*pString == 195 && *(pString + 1) == 135) Chr = 108;
               pString++;
           }
           else
           {
               Chr = *pString - SelectedFont->Offset;
           }

           GLCD_DisplayChar(Xpos, Yrow, Chr, 1);

           // Move Xpos to the next character position
           Xpos += SelectedFont->Width;
           // Move to the next character
           pString++;
       }
   }

   void GLCD_PrintChar(uint8_t Xpos, uint8_t Ypos, char Chr, uint8_t Mode)
   {
       uint16_t BufferIndex, TableIndexBase;
     uint8_t i, DisplayData, Shift, Reserved;

     Shift = (Ypos % 8);
     BufferIndex = ((Ypos / 8) * KS0108_WIDTH) + Xpos;

     TableIndexBase = ((SelectedFont->Width * SelectedFont->Height * Chr) >> 3);

     for (i = 0; i < SelectedFont->Width; i++)
     {
         if(Mode)
         {
             DisplayData = SelectedFont->table[TableIndexBase +  (SelectedFont->Line * i)];
         }
         else
         {
             DisplayData = ~SelectedFont->table[TableIndexBase +  (SelectedFont->Line * i)];
         }

         Reserved = DisplayBuffer[BufferIndex + i];
         Reserved &= (0xFF >> (8 - Shift));
         DisplayBuffer[BufferIndex + i] = ((DisplayData << Shift) | Reserved);

         Reserved = DisplayBuffer[BufferIndex + 128 + i];
         Reserved &= ~Shift;
         DisplayBuffer[BufferIndex + 128 + i] = ((DisplayData >> (8 - Shift)) | Reserved);
     }
   }


void GLCD_PrintString(uint8_t Xpos, uint8_t Ypos, const char *pString) {
	uint8_t Chr=0;

	while (*pString != '\0') {
		if (Xpos > (KS0108_WIDTH - SelectedFont->Width)) {
			Xpos = 0;
			Ypos += SelectedFont->Height;

			if (Ypos > (KS0108_HEIGHT - 8)) {
				break;
			}
		}

		// Check for multi-byte characters
		if ((*pString == 196 && *(pString + 1) == 159) || // ğ
				(*pString == 195 && *(pString + 1) == 188) || // ü
				(*pString == 197 && *(pString + 1) == 159) || // ş
				(*pString == 196 && *(pString + 1) == 177) || // ı
				(*pString == 195 && *(pString + 1) == 182) || // ö
				(*pString == 195 && *(pString + 1) == 167) || // ç
				(*pString == 196 && *(pString + 1) == 158) || // Ğ
				(*pString == 195 && *(pString + 1) == 156) || // Ü
				(*pString == 197 && *(pString + 1) == 158) || // Ş
				(*pString == 196 && *(pString + 1) == 176) || // İ
				(*pString == 195 && *(pString + 1) == 150) || // Ö
				(*pString == 195 && *(pString + 1) == 135))   // Ç
				{
			if (*pString == 196 && *(pString + 1) == 159)
				Chr = 97;
			else if (*pString == 195 && *(pString + 1) == 188)
				Chr = 98;
			else if (*pString == 197 && *(pString + 1) == 159)
				Chr = 99;
			else if (*pString == 196 && *(pString + 1) == 177)
				Chr = 100;
			else if (*pString == 195 && *(pString + 1) == 182)
				Chr = 101;
			else if (*pString == 195 && *(pString + 1) == 167)
				Chr = 102;
			else if (*pString == 196 && *(pString + 1) == 158)
				Chr = 103;
			else if (*pString == 195 && *(pString + 1) == 156)
				Chr = 104;
			else if (*pString == 197 && *(pString + 1) == 158)
				Chr = 105;
			else if (*pString == 196 && *(pString + 1) == 176)
				Chr = 106;
			else if (*pString == 195 && *(pString + 1) == 150)
				Chr = 107;
			else if (*pString == 195 && *(pString + 1) == 135)
				Chr = 108;
			pString++;
		} else {
			Chr = *pString - SelectedFont->Offset;
		}

		GLCD_PrintChar(Xpos, Ypos, Chr, 1);

		Xpos += SelectedFont->Width;
		pString++;
	}
}



   void glcd_text57(uint8_t x, uint8_t y, char *textptr, uint8_t size, int8_t color) {
       uint32_t i, j, k, l, m;
       uint8_t pixelData[6]; // Adjust size if needed

       for (i = 0; textptr[i] != '\0'; ++i, ++x) {
           // Lookup the character in the font array
           if (textptr[i] == 196 && textptr[i+1]==159) {			// ğ
               memcpy(pixelData, &font6x8_Table[97*6], 6);
               if (textptr[i + 1] != '\0') {
                   i++;
               }
           }
           else if (textptr[i] == 195 && textptr[i+1]==188) {		// ü
               memcpy(pixelData, &font6x8_Table[98*6], 6);
               if (textptr[i + 1] != '\0') {
                   i++;
               }
           }
           else if (textptr[i] == 197 && textptr[i+1]==159) {		// ş
               memcpy(pixelData, &font6x8_Table[99*6], 6);
               if (textptr[i + 1] != '\0') {
                   i++;
               }
           }
           else if (textptr[i] == 196 && textptr[i+1]==177) {		// ı
               memcpy(pixelData, &font6x8_Table[100*6], 6);
               if (textptr[i + 1] != '\0') {
                   i++;
               }
           }
           else if (textptr[i] == 195 && textptr[i+1]==182) {		// ö
               memcpy(pixelData, &font6x8_Table[101*6], 6);
               if (textptr[i + 1] != '\0') {
                   i++;
               }
           }
           else if (textptr[i] == 195 && textptr[i+1]==167) {		// ç
               memcpy(pixelData, &font6x8_Table[102*6], 6);
               if (textptr[i + 1] != '\0') {
                   i++;
               }
           }
           else if (textptr[i] == 196 && textptr[i+1]==158) {		// Ğ
               memcpy(pixelData, &font6x8_Table[103*6], 6);
               if (textptr[i + 1] != '\0') {
                   i++;
               }
           }
           else if (textptr[i] == 195 && textptr[i+1]==156) {		// Ü
               memcpy(pixelData, &font6x8_Table[104*6], 6);
               if (textptr[i + 1] != '\0') {
                   i++;
               }
           }
           else if (textptr[i] == 197 && textptr[i+1]==158) {		// Ş
               memcpy(pixelData, &font6x8_Table[105*6], 6);
               if (textptr[i + 1] != '\0') {
                   i++;
               }
           }
           else if (textptr[i] == 196 && textptr[i+1]==176) {		// İ
               memcpy(pixelData, &font6x8_Table[106*6], 6);
               if (textptr[i + 1] != '\0') {
                   i++;
               }
           }
           else if (textptr[i] == 195 && textptr[i+1]==150) {		// Ö
               memcpy(pixelData, &font6x8_Table[107*6], 6);
               if (textptr[i + 1] != '\0') {
                   i++;
               }
           }
           else if (textptr[i] == 195 && textptr[i+1]==135) {		// Ç
               memcpy(pixelData, &font6x8_Table[108*6], 6);
               if (textptr[i + 1] != '\0') {
                   i++;
               }
           }
           else {
               memcpy(pixelData, &font6x8_Table[(textptr[i] - ' ') * 6], 6);
           }

           // Draw the character pixel by pixel
           for (j = 0; j < 6; ++j, x += size) {
               for (k = 0; k < 8 * size; ++k) {
                   if (pixelData[j] & (0x01 << k)) {
                       for (l = 0; l < size; ++l) {
                           for (m = 0; m < size; ++m) {
                               GLCD_Pixel(x + m, y + k * size + l, color ? 1 : 0);
                           }
                       }
                   } else {
                       for (l = 0; l < size; ++l) {
                           for (m = 0; m < size; ++m) {
                               GLCD_Pixel(x + m, y + k * size + l, color ? 0 : 1);
                           }
                       }
                   }
               }
           }
       }
   }


void glcd_text56(uint8_t x, uint8_t y, char *textptr) {
	uint32_t i, j, k, l, m;
	uint8_t pixelData[6]; // Adjust size if needed

	for (i = 0; textptr[i] != '\0'; ++i, ++x) {
		// Lookup the character in the font array

			memcpy(pixelData, &font6x8_Table[(textptr[i] - ' ') * 6], 6);


		// Check if the last three or last two elements are zeros, except for '1'
		uint8_t skipLastZero = (textptr[i] != '1')
				&& ((pixelData[3] == 0x00 && pixelData[4] == 0x00
						&& pixelData[5] == 0x00)
						|| (pixelData[4] == 0x00 && pixelData[5] == 0x00));

		for (j = 0; j < (skipLastZero ? 4 : 6); ++j, x += 1) {
			for (k = 0; k < 8 * 1; ++k) {
				if (pixelData[j] & (0x01 << k)) {
					for (l = 0; l < 1; ++l) {
						for (m = 0; m < 1; m++) {
							GLCD_Pixel(x + m, y + k * 1 + l, 1 ? 1 : 0);
						}
					}
				} else {
					for (l = 0; l < 1; ++l) {
						for (m = 0; m < 1; m++) {
							GLCD_Pixel(x + m, y + k * 1 + l, 1 ? 0 : 1);
						}
					}
				}
			}
		}
	}
}

void GLCD_Line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
	int16_t i, dx, dy, diff;
	// X ve Y koordinatları arasındaki farkları hesapla
  dx = mABS(x1 - x0); // Mutlak değer alınır
  dy = mABS(y1 - y0);

  // Eğer iki nokta dikey bir çizgi oluşturuyorsa
  if(dx == 0)
  {

    diff = mSIGNUM(y1 - y0); // Yön belirle

    // Başlangıç noktasından bitiş noktasına kadar her bir pikseli çiz
    for(i = 0; i <= dy; i++)
    {
    	GLCD_Pixel(x0, y0, 1); // Pikseli çiz
			y0 += diff; // Y koordinatını güncelle
    }
  }
  // Eğer iki nokta yatay bir çizgi oluşturuyorsa
  else if(dy == 0)
  {
    diff = mSIGNUM(x1 - x0); // Yön belirle

    // Başlangıç noktasından bitiş noktasına kadar her bir pikseli çiz
    for(i = 0; i <= dx; i++)
    {
    	GLCD_Pixel(x0, y0, 1); // Pikseli çiz
      x0 += diff; // X koordinatını güncelle
    }
  }
}

void GLCD_Pixel(uint8_t	x, uint8_t y, uint8_t Colour)
{
	uint8_t Column, Row, Shift;
	uint16_t BufferIndex;
	// X ve Y koordinatlarından sütun, satır ve kaydırma değerlerini hesapla
	Column = x;
	Row = y >> 3;  // 8'e bölme işlemi, bir bitin 8 piksele karşılık geldiğini belirtir
	Shift = y % 8; // Y koordinatındaki kalan değer, kaydırma miktarını belirtir

	// 127, 63
	// Ekran belleğindeki indeksi hesapla
	BufferIndex = (Row << 7) + Column; // Bir satır için 128 piksel var, bu yüzden (Row << 7) ifadesi kullanılır
	// Renk bilgisine göre pikseli ayarla
	if(Colour)
	{
		// Renk beyaz ise, ilgili biti ayarla (1)
		DisplayBuffer[BufferIndex] |= 0x01 << Shift;
	}
	else
	{
		// Renk siyah ise, ilgili biti temizle (0)
		DisplayBuffer[BufferIndex] &= ~(0x01 << Shift);
	}
}

//---------------------------------------------------------------------------/
// Function Name : GLCD_Rect         			      				                     /
// Description   : Belirtilen iki nokta arasında bir dikdörtgen çizer veya dolu bir dikdörtgen çizer                                   								 /
// Argument      : x0, y0: Dikdörtgenin bir köşesinin X ve Y koordinatları
//                 x1, y1: Dikdörtgenin karşı köşesinin X ve Y koordinatları
//Fill           : Dolu bir dikdörtgen mi yoksa çerçevesi mi çizileceğini belirten bayt (1: Dolu, 0: Çerçeve)
//Colour         : Dikdörtgenin rengi (1: Beyaz, 0: Siyah)
// Return	     : None                                                      /
//---------------------------------------------------------------------------/
void  GLCD_Rect(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t Fill, uint8_t Colour)
{
  if(Fill)
  {
  	uint8_t  i, xmin, xmax, ymin, ymax;

  	// Köşelerin minimum ve maksimum koordinatlarını belirle
    if(x0 < x1)
    {
      xmin = x0;
      xmax = x1;
    }
    else
    {
      xmin = x1;
      xmax = x0;
    }

    if(y0 < y1)
    {
      ymin = y0;
      ymax = y1;
    }
    else
    {
      ymin = y1;
      ymax = y0;
    }
    // Dolu bir dikdörtgen çiz
    for(; xmin <= xmax; ++xmin)
    {
      for(i = ymin; i <= ymax; ++i)
      {
      	GLCD_Pixel(xmin, i, Colour);// Her bir pikseli belirtilen renkte ayarla
      }
    }
  }
  // Sadece dikdörtgenin çerçevesini çiz
  else
  {
    GLCD_Line(x0, y0, x1, y0);// Üst kenarı çiz
    GLCD_Line(x0, y1, x1, y1);// Alt kenarı çiz
    GLCD_Line(x0, y0, x0, y1);// Sol kenarı çiz
    GLCD_Line(x1, y0, x1, y1);// Sağ kenarı çiz
  }
}

void  GLCD_Rect_E(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
	uint8_t Fill=0; uint8_t Colour=0;
  if(Fill)
  {
  	uint8_t  i, xmin, xmax, ymin, ymax;

  	// Köşelerin minimum ve maksimum koordinatlarını belirle
    if(x0 < x1)
    {
      xmin = x0;
      xmax = x1;
    }
    else
    {
      xmin = x1;
      xmax = x0;
    }

    if(y0 < y1)
    {
      ymin = y0;
      ymax = y1;
    }
    else
    {
      ymin = y1;
      ymax = y0;
    }
    // Dolu bir dikdörtgen çiz
    for(; xmin <= xmax; ++xmin)
    {
      for(i = ymin; i <= ymax; ++i)
      {
      	GLCD_Pixel(xmin, i, Colour);// Her bir pikseli belirtilen renkte ayarla
      }
    }
  }
  // Sadece dikdörtgenin çerçevesini çiz
  else
  {
    GLCD_Line(x0, y0, x1, y0);// Üst kenarı çiz
    GLCD_Line(x0, y1, x1, y1);// Alt kenarı çiz
    GLCD_Line(x0, y0, x0, y1);// Sol kenarı çiz
    GLCD_Line(x1, y0, x1, y1);// Sağ kenarı çiz
  }
}

//---------------------------------------------------------------------------/
// Function Name : GLCD_RectNEW
// Description   : Belirtilen iki nokta arasında bir dolu dikdörtgen çizer
// Argument      : x0, y0: Dikdörtgenin bir köşesinin X ve Y koordinatları
//                 x1, y1: Dikdörtgenin karşı köşesinin X ve Y koordinatları
//                 Colour: Dikdörtgenin rengi (1: Beyaz, 0: Siyah)
// Return        : None                                                     /
//---------------------------------------------------------------------------/
void GLCD_RectNEW(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t Colour)
{
  	uint8_t  i, xmin, xmax, ymin, ymax;
  	// Köşelerin minimum ve maksimum koordinatlarını belirle
    if(x0 < x1)
    {
      xmin = x0;
      xmax = x1;
    }
    else
    {
      xmin = x1;
      xmax = x0;
    }

    if(y0 < y1)
    {
      ymin = y0;
      ymax = y1;
    }
    else
    {
      ymin = y1;
      ymax = y0;
    }
    // Dolu bir dikdörtgen çiz
    for(; xmin <= xmax; ++xmin)
    {
      for(i = ymin; i <= ymax; ++i)
      {
      	GLCD_Pixel(xmin, i, Colour);// Her bir pikseli belirtilen renkte ayarla
      }
    }
}

//---------------------------------------------------------------------------/
// Function Name : GLCD_PrintImage       			      				                 /
// Description   : Print an image array to GLCD Display                      /
// Argument      : Image Array Address 				                               /
// Return			   : None                                                      /
//---------------------------------------------------------------------------/
void GLCD_PrintImage(const uint8_t *ImageAddress)
{
  uint16_t i;
  for (i = 0; i < KS0108_BUFFER_LENGTH; i++)
  {
    DisplayBuffer[i] = ImageAddress[i];
  }
}
