#ifndef __LOADSPRITES_H__
#define __LOADSPRITES_H__

#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>

#include "global.h"
#include "cell.h"

#define SPR_REALLOC_UNIT 256        /*use to increase the size of sprites table*/
#define SPR_STOCK_MAX 5120 /*Note : Mettre 1 warning si dépasse le nombre de stock faire un stock max comme le buffer SDL*/
#define BACKGROUND 252 
#define ALPHA 253


typedef struct Sprites
{
  u32 RefX,RefY;
  u32 Lg, Ht;
  u8 *Gfx;
  u8 *Mask;
}Sprites;

typedef struct SSprStockage
{
  u32 SprNo; 
  s32 PosX, PosY;
  u32 Prio;
}SSprStockage;

SSprStockage SprSto[SPR_STOCK_MAX];
Sprites	*sprites;
u32 NbSprMax;             /*Max size allocated for the struct array. May be reallocated*/
u32 NbSpr;                /*number of captured sprite*/
u32 NbSprPlanche; 	
u32 NbSprSto;             /*Nb de sprites stockés pour affichage*/

void ErasePixel();
void SprStoData(u32 SprNo, s32 PosX, s32 PosY,u32 Prio);
Sprites* SprGetDesc(u32 SprNo);
void SprInitEngine();
void Sprites_LoadBMP(char* filename); /*Sprites recovery of a board */
void Sprites_Realloc();
void Sprites_Release();
void SprDisplayAll();
void SprDisplay(u32 SprNo, s32 PosX, s32 PosY,u32 nPrio);

enum
  {
    e_Prio_Cell,
    e_Prio_Logo,
    e_Prio_Logo_Character,
    e_Prio_Rules,
    e_Prio_Font
  };


enum
  {
    e_Spr_Cell=0,
    e_Spr_Logo=e_Spr_Cell+2,
    e_Spr_Logo_Character=e_Spr_Logo+1,
    e_Spr_NbRules=e_Spr_Logo_Character+10,
    e_Spr_CursorRules=e_Spr_NbRules+1,
    e_Spr_Rules=e_Spr_CursorRules+2,
    e_Spr_FontSmall=e_Spr_Rules+1,
    e_Spr_NEXT = e_Spr_FontSmall + 64
  };


#endif


