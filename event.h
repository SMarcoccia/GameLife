#ifndef __EVENT_H__
#define __EVENT_H__

#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>

#include "global.h"
#include "cell.h"
#include "font.h"
#include "render.h"


enum
  {
    EVENT_Escap=1,
    EVENT_MOUSE_BtnRight,
  };

typedef struct SSaveScr
{
  SDL_Surface *pSave;
}SSaveScr;

void FreeBoardScreen();
void RestoreBoardScreen();
void SaveBoardScreen();
u32 EventHandler();

#endif
