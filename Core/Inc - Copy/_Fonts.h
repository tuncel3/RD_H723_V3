#ifndef __FONTS_H
#define __FONTS_H

#include "stdint.h"

extern const uint8_t font4x6_Table[];
extern const uint8_t font6x8_Table[];
extern const uint8_t  TEXT[51][5];
extern const uint8_t TEXT2[44][5];
extern const uint8_t font8x16_Table[];
extern const uint8_t font12x16_Table[];


typedef struct stFonts
{    
  const uint8_t *table;
  uint8_t Width;
  uint8_t Height;
  uint8_t Offset;
  uint8_t Line;
}stFonts_t;

extern stFonts_t stFont4x6 ;
extern stFonts_t stFont6x8;
extern stFonts_t stFont8x16 ;
extern stFonts_t stFont12x16;

#endif /* __FONTS_H */
