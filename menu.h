#ifndef __MENU_H__
#define __MENU_H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL/SDL.h>

#include "global.h"
#include "tools.h"
#include "font.h"
#include "event.h"

#define START_Curs 50
#define CURS_Acc 0x300 /*speed of curse*/


typedef struct SMenuGen
{
  s8 nChoix;
  u32 nOldMousePosX, nOldMousePosY;
  s16 nCursPos;
  s16 nCursAccel;/*Accélération */
}SMenuGen;

SMenuGen gMenu;

typedef struct SMenu
{
  u32 menuVal;       
  u32 nLg;           /*length of the string in pixels*/
  char *pTxt;
}SMenu;

enum
  {
    MENU_Null,// MENU_Null a pour objectif de rester dans la boucle while de la fonction menu qui se trouve dans le main.c
    MENU_Main,
    MENU_Place_Cells,
    MENU_Pause,
    MENU_Erase,
    MENU_Load,
    MENU_Save,
    MENU_Rules,
    MENU_Quit
  };



void CursorInit();
void CursorMove();
void SMenuGen_MenuInitFade();
void SMenu_MenuInit();
u32 SMenu_Menu();
s8 ColBoxMouseMenu(u32 i);
u32 menu();

#endif
