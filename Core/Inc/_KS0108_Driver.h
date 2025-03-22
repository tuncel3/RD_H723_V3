#ifndef __KS0108_DRIVER_H
#define __KS0108_DRIVER_H

#include <_Fonts.h>
#include "stm32h7xx_ll_gpio.h"

#define res_ LL_GPIO_ResetOutputPin
#define set_ LL_GPIO_SetOutputPin
#define tog_ LL_GPIO_TogglePin
#define isSet_ LL_GPIO_IsInputPinSet

//---------------------------------------------------------------------------/
//          			          SSD1306 Hardware Definition  				             /
//---------------------------------------------------------------------------/
#define KS0108_WIDTH                             128
#define KS0108_HEIGHT                            64
#define YES                1
#define NO                 0
#define LCD_PAGE_HEIGHT    8
#define LCD_LINES          64
#define LCD_LINE_LENGTH    128

#define GLCD_WIDTH         128
#define SIZE1              1
#define SIZE2              2
#define SIZE3              3
#define SIZE4              4

#define KS0108_BUFFER_LENGTH                     ((KS0108_WIDTH * KS0108_HEIGHT) / 8)
#define KS0108_DEFAULT_FONT                      stFont6x8

#define mABS(x)           											 (((x) < 0) ? -(x) : (x))
#define mMAX(a,b)         											 ((a < b) ?  (b) : (a))
#define mSIGNUM(x)        											 (x > 0) ? 1 : ((x < 0) ? -1 : 0)


/********* LCD **********/

// FP LCD
#define TC_1_1_P GPIOD, LL_GPIO_PIN_3
#define TC_1_2_P GPIOD, LL_GPIO_PIN_2
#define TC_1_3_P GPIOD, LL_GPIO_PIN_1
#define TC_1_4_P GPIOD, LL_GPIO_PIN_0
#define TC_1_5_P GPIOC, LL_GPIO_PIN_12
#define TC_1_6_P GPIOC, LL_GPIO_PIN_11

#define TC_2_1_P GPIOB, LL_GPIO_PIN_6
#define TC_2_2_P GPIOB, LL_GPIO_PIN_5
#define TC_2_3_P GPIOB, LL_GPIO_PIN_4
#define TC_2_4_P GPIOB, LL_GPIO_PIN_3
#define TC_2_5_P GPIOD, LL_GPIO_PIN_7
#define TC_2_6_P GPIOD, LL_GPIO_PIN_6
#define TC_2_7_P GPIOD, LL_GPIO_PIN_5
#define TC_2_8_P GPIOD, LL_GPIO_PIN_4

#define GLCD_DATA_PORT  GPIOB

#define CS1 TC_1_4_P
#define CS2 TC_1_5_P
#define RS  TC_2_1_P
#define RW  TC_2_2_P
#define EN  TC_2_3_P
#define RST TC_1_6_P

#define DB0 TC_2_4_P
#define DB1 TC_2_5_P
#define DB2 TC_2_6_P
#define DB3 TC_2_7_P
#define DB4 TC_2_8_P
#define DB5 TC_1_1_P
#define DB6 TC_1_2_P
#define DB7 TC_1_3_P

#define CS11              set_(CS1)
#define CS10              res_(CS1)
#define CS21              set_(CS2)
#define CS20              res_(CS2)
#define RS1               set_(RS)
#define RS0               res_(RS)
#define RW1               set_(RW)
#define RW0               res_(RW)
#define EN1               set_(EN)
#define EN0               res_(EN)
#define RST1              set_(RST)
#define RST0              res_(RST)


#define SELECT_FIRST_CHIP       CS11, CS20
#define SELECT_SECOND_CHIP      CS21, CS10

#define SET_MODE_DATA_WRITE      RS1, RW0
#define SET_MODE_DATA_READ       RS1, RW1
#define SET_MODE_COMMAND_WRITE   RS0, RW0
#define SET_MODE_COMMAND_READ    RS0, RW1

#define START_LINE                0xC0
#define X_BASE_ADDRESS            0xB8
#define Y_BASE_ADDRESS            0x40
#define DISPLAY_ON                0x3F
#define DISPLAY_OFF               0x3E

extern  uint8_t   DisplayBuffer[KS0108_BUFFER_LENGTH];
extern uint16_t	Column;
extern uint8_t   Row;
extern uint8_t	State;
extern uint8_t   TriggerCounter;
extern uint8_t   Busy;
extern stFonts_t *SelectedFont;


extern  void __delay(void);
extern void   GLCD_WriteCommand(uint8_t Command);
extern void GLCD_WriteData(uint8_t Data);
extern void  GLCD_RefreshGRAM(void);
extern void GLCD_ClearScreen(uint8_t Fill);
extern void  GLCD_SetFont(stFonts_t *Font);
extern void GLCD_Init(void);
extern void GLCD_Init_while(void);

//extern void GLCD_DisplayChar(uint8_t Xpos, uint8_t Yrow, char Chr, uint8_t Mode);
extern void GLCD_DisplayString(uint8_t Xpos, uint8_t Yrow, const char *pString);
extern void GLCD_PrintChar(uint8_t Xpos, uint8_t Ypos, char Chr, uint8_t Mode);
extern void GLCD_PrintString(uint8_t Xpos, uint8_t Ypos, const char *pString);
extern void GLCD_PrintImage(const uint8_t *ImageAddress);
extern void GLCD_Pixel(uint8_t	x, uint8_t y, uint8_t Colour);
extern void GLCD_Line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
extern void  GLCD_Rect(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t Fill, uint8_t Colour);
extern void GLCD_RectNEW(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t Colour);
extern void glcd_text57(uint8_t x, uint8_t y,  char* textptr, uint8_t size, int8_t color);












#endif /* __KS0108_DRIVER_H */
