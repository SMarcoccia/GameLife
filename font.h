#ifndef __FONT_H__
#define __FONT_H__

#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>

#include "tools.h"
#include "loadSprites.h"
#include "menu.h"

#define FONT_H 15 /*Size cara*/
#define GAP_Y 12 /*gap entre 2 cara*/
#define FONT_GAP_Y (FONT_H+GAP_Y)
#define FONT_W 14 //size '<'



#define Font_NoDispo (1<<0)

/*u32 permet de récupérer la lenght de la string*/
u32 Font_Print(s32 PosX, s32 PosY, char* pStr,u32 flags);

#endif
