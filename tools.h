#ifndef __TOOLS_H__
#define __TOOLS_H__

#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>



#define lf printf("\n");
#define lflf printf("\n\n");
#define true -1
#define false 0
#define NBELEM(tab) ((s8)sizeof(tab) / (s8)sizeof(tab[0]))
#define ABS(val) val < 0 ? -val : val


typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;

typedef u8 bool ;

void ToolsBubble_Sort(u32 NbElem, u32 *Array);
void ToolsSwap(u32 *Array1, u32 *Array2);

#endif
