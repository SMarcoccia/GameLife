#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <math.h>

#include "tools.h"


#define WIN_W 800
#define WIN_H 600
#define BPP 8
#define FLAGS SDL_SWSURFACE | SDL_DOUBLEBUF
#define MENU_NbBkg 2
#define GFX_NbBkg 1
#define CLR_BLUE_IDX  0xD2 
#define CLR_GREEN_IDX 0xFA
#define CLR_BLACK_IDX 0x0
#define CLR_WHITE_IDX 0xFF

#define KeyBtnReturn 1
#define KeyBtnSpace 2
#define KeyBtnUP 3
#define KeyBtnDOWN 4

#define MouseBtnLeft 1
#define MouseBtnRight 2

typedef struct SGene
{
  SDL_Surface *pScreen;/*buffer SDL*/

  SDL_Surface* pMenuBkg[MENU_NbBkg]; /*Background of menu*/
  SDL_Surface* pMenuBackground;/*ptr on background*/

  SDL_Surface* pGfxBkg[GFX_NbBkg];
  SDL_Surface* pGfxBackground;

  SDL_Rect pBkgRectDst;/*Pos in Screen destination*/
  SDL_Rect pBkgRectSrc;/*Pos clip in background*/

  u8 Key[SDLK_LAST];
  s16 pSinCos[256+64];
  s16 *pCos;
  s16 *pSin;
  u32 nMousePosX, nMousePosY;
  u8 nMouseButton; /*state of button 1 or 2*/
  u8 nKeyButton;
}SGene;

typedef struct SExg 
{
 u32 nExitCode;

}SExg;

SExg SgExg;/*corresponds à struct SExg gExg dans include.h*/
SGene* pGene; /*la déclarer ici revient à la déclarer dans le main*/

void InitSDL_Create(SGene *G);
SGene* Global_Create();
void Global_Destroy(SGene* G);
          
#endif
