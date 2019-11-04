#ifndef __SCANLINE_H__
#define __SCANLINE_H__

#include <stdio.h>
#include <stdlib.h>

#include "global.h"
#include "cell.h"

enum{e_Is_Dead=0, e_Is_Alive=1};

typedef struct SStackPix SStackPix;
struct SStackPix
{
  u32 x, y;
  SStackPix* next;
};


void ScanlineFloodFill_Game();

void ScanlineCreateInitPile();
void ScanlineCreateCellPile(s32 x, s32 y);
void ScanlineFreeCellPile();
void ScanlineFloodFill_PlaceCell(u8 nState);

void PutPixel(u32 x,u32 y,u8 CLR);
u8 GetClrPixel(u32 x,u32 y);
u8 GetClrPixelMouse();

#endif
