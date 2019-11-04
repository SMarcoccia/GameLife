#ifndef _RULES_H__
#define _RULES_H__

#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>

#include "global.h"
#include "font.h"
#include "tools.h"
#include "loadSprites.h"
#include "event.h"
#include "cell.h"
#include "menu.h"




enum
  {
    //MENU_RULES_Null,// MENU_Null a pour objectif de rester dans la boucle while de la fonction menu qui se trouve dans le main.c
    MENU_RULES_Reset,
    MENU_RULES_Default,
    MENU_RULES_Place_Cells,
    MENU_RULES_Quit
  };


s8 RulesColBoxMouse_MenuRules(u8 i);
s8 RulesColMouse_MenuRules();

void RulesDisplayRulesReset();
void RulesResetRules();
void RulesDisplaySprRulesDefault();
void RulesRulesDefault_and_Display();/*Régle par défaut + display SprRule par Défaut*/
void RulesSprRulesDisplay();

 u32 RulesEventHandler();
  u8 RulesColBoxMouseRules(u32 i, u32 j);
void RulesColMouseSprRules();

void DisplayBkg();
void InitBkg(u32 i);
void RulesMenuRulesInit();
u8 RulesMenuRules();
u8 rules();

#endif
