#ifndef __CELL_H__
#define __CELL_H__

#include <stdio.h>
#include <stdlib.h>

#include "global.h"
#include "loadSprites.h"
#include "tools.h"
#include "event.h"
#include "scanline.h"
#include "rules.h"
#include "menu.h"


/*jhg*/
/*correspond à breaker.h*/

#define CELL_W (9+1)
#define CELL_H (9+1)
#define SCR_CELL_W (WIN_W/CELL_W)
#define SCR_CELL_H (WIN_H/CELL_H)
#define SIZE_Tab_Birth 9
#define SIZE_Tab_Survival 9




enum
  {
    e_Cell_Paint,
    e_Cell_Eraser
  };

enum
  {
    e_Game_Null,
    e_Game_Normal, /*return or go to game*/
    e_Game_All_Clear,
    e_Game_Pause,
    e_Game_Place_Cells,/*goto place cell*/
    e_Game_Rules,/*Goto rules*/
    e_Game_Load,
    e_Game_Save,
    e_Game_Quit /*goto main menu*/
  };

enum
  {
    CELL_Null,
    CELL_Place,
    CELL_Menu_Place,
    CELL_Game,
    CELL_Rules,
    CELL_Load,
    CELL_Save,
    CELL_Quit
  };

typedef struct SCell
{  
  u32 nTime;
  u32 nPhase;
  u8** TabGame1;/*TabGame1 et 2:  tableaux pour le calcul de la next génération*/
  u8** TabGame2;
  u8* TabBirth;
  u8* TabSurvival;
}SCell;

SCell SgCell;

void CellBkgMenuPlace();
void CellMenuPlaceCellsInit();
u32 CellMenuPlaceCells();

void CellBkgMenuGame();
void CellMenuCellGamesInit();
void CellMenuCellGame();

void CellNextGen();
u32 CellCountNeighbors(s32 X, s32 Y);
void CellBkgDisplay(u8 bkg);
u32 CellPlaceCells();
void CellPlaceCells_Aux();
void CellSprCellPlaceCells(u8 nState);

void CellBrowsingGameBoardSto();
u8 CellGameCells();
void CellGameCells_Aux();


void CellTabRules_Default();/*if return so menu reset and valu per default*/
void CellTabRules_Reset();
void CellFreeTabRules();/*free in quit game*/
void CellInitTabRules();/*Value per default*/

void CellResetTabGame_All();
void CellFreeTabGame_All();
void CellCreateInitTabGame_All();
void CellCopyTabGame2_To_1();
void CellCopyTabGame1_To_2();

u32 CellEventHandler(u8 nInGame);
void ExgCellInit();/*correspond à ExgBrkInit du breaker.h*/
void CellsGame();
void CellInit();
void CellsGameMenu();

void CellFrameModular_Plus();
void CellFrameModular_Minus();


#endif
