#include "rules.h"

#define MENU_RULES_Y 415
#define VAR_Adjust_Rules_Y -120
#define VAR_Adjust_SprRules_Y -50
#define START_StringRules_Y 22
#define PosSprOrg_X (WIN_W/2 - sprites[e_Spr_Rules].Lg/2)
#define PosSprOrg_Y (WIN_H/2 + VAR_Adjust_SprRules_Y)
#define PosCase1_Line1_X 380 
#define PosCase1_Line1_Y 306
#define Case_W 26
#define Case_H 44
#define ColumnBlack_W_H 3
#define CaseColumn_W (26+ColumnBlack_W_H)
#define CaseColumn_H (44+ColumnBlack_W_H)
#define NbColumnsTab 9
#define NbLinesTab 2


SMenu Rules[]=
  {
    {0,0,"REGLES : "},
    {0,0,"3 CELL VOISINES VIVANTES, 1 CELL NAIT ;"},
    {0,0,"2 OU 3 CELL VOISINES VIVANTES LA CELL SURVIE ;"},
    {0,0,"1 CELL VIVANTE NE POSSEDANT PAS"},
    {0,0,"2 OU 3 CELL VOISINES MEURT"},
    {0,0,"(PAR ISOLEMENT OU PAR SURPEUPLEMENT)."},
    {0,0,""},
    {0,0," - CHANGER LES REGLES -"}
  };

SMenu Menu_Rules[]=
  {
    {MENU_RULES_Reset,0,"RESET (R)"},
    {MENU_RULES_Default,0,"REGLES PAR DEFAUT (ESPACE)"},
    {MENU_RULES_Place_Cells,0,"PLACE CELLS (ENTREE)"},
    {MENU_RULES_Quit,0,"MENU (Q)"},
  };


void RulesDisplayRulesReset()
{
  u32 i,j;

  for(j=0;j<NbLinesTab;j++)
    for(i=0;i<NbColumnsTab;i++)
      SprStoData(e_Spr_NbRules,PosCase1_Line1_X+i*CaseColumn_W,PosCase1_Line1_Y+j*CaseColumn_H,e_Prio_Rules);
}

void RulesResetRules()
{
  CellTabRules_Reset();
  RulesDisplayRulesReset();
}

void RulesRulesDefault_and_Display()
{
  CellTabRules_Default();
  RulesDisplaySprRulesDefault();
}

void RulesDisplaySprRulesDefault()
{
  SprStoData(e_Spr_Rules,WIN_W/2-(sprites[e_Spr_Rules].Lg/2),WIN_H/2+VAR_Adjust_SprRules_Y,e_Prio_Rules);
}


void RulesSprRulesDisplay()
{
  u8 nVal,i,j;

  RulesDisplaySprRulesDefault();

  for(j=0;j<NbLinesTab;j++)
    for(i=0;i<NbColumnsTab;i++)
      {
	if(j==0)
	  nVal=SgCell.TabBirth[i];
	else
	  nVal=SgCell.TabSurvival[i];

	SprStoData(e_Spr_NbRules+nVal,PosCase1_Line1_X+i*CaseColumn_W,PosCase1_Line1_Y+j*CaseColumn_H,e_Prio_Rules);
      }
}

s8 RulesColBoxMouse_MenuRules(u8 i)
{
  s32 XMin, XMax, YMin, YMax;
  s32 XMouse, YMouse;

  XMin=WIN_W/2-Menu_Rules[i].nLg/2;
  XMax=XMin+Menu_Rules[i].nLg;
  YMin=MENU_RULES_Y+FONT_GAP_Y*i;
  YMax=YMin+FONT_GAP_Y;
  XMouse=pGene->nMousePosX;
  YMouse=pGene->nMousePosY;

  if(XMouse>=XMin && XMouse<=XMax && YMouse>=YMin && YMouse<=YMax)
    return 1;
  else 
    return -1;
}

s8 RulesColMouse_MenuRules()
{
  s8 i;
  s8 nLastChoix=gMenu.nChoix;

  for(i=0;i<NBELEM(Menu_Rules);i++)
    {
      if(RulesColBoxMouse_MenuRules(i)>=0)
	{
	  gMenu.nChoix=i;
	  break;
	}
      else
	gMenu.nChoix=-1;
    }

  if(pGene->nMouseButton & MouseBtnLeft)
    if(gMenu.nChoix>=0)
      {
	switch(Menu_Rules[gMenu.nChoix].menuVal)
	  {
	  case MENU_RULES_Reset :
	    RulesResetRules();
	    break;
	  case MENU_RULES_Default :
	    RulesRulesDefault_and_Display();
	    break;
	  case MENU_RULES_Place_Cells :
	    CellsGameMenu();/*ne renvoi rien*/
	    return MENU_Quit;
	  case MENU_RULES_Quit :
	    return MENU_Quit;
	  }
      }

  if(nLastChoix!=gMenu.nChoix)
    CursorInit();

  return MENU_Null;
}


void DisplayBkg()
{
  SDL_BlitSurface(pGene->pMenuBackground, NULL, pGene->pScreen, NULL);
}

void InitBkg(u32 i)
{
  pGene->pMenuBackground=pGene->pMenuBkg[i]; /*Background warhammer*/

  SDL_BlitSurface(pGene->pMenuBackground, NULL, pGene->pScreen, NULL);

}

void RulesMenuRulesInit(/*u8 nInitRules*/)
{
  s8 i;

  InitBkg(1);
  //RulesRulesDefault_and_Display();/*Si on la met remet tout les régles par défaut*/
  pGene->nMouseButton=0;
  pGene->nKeyButton=0;

  for(i=0;i<NBELEM(Rules);i++)
    Rules[i].nLg=Font_Print(0,0,Rules[i].pTxt,Font_NoDispo);
  for(i=0;i<NBELEM(Menu_Rules);i++)
    Menu_Rules[i].nLg=Font_Print(0,0,Menu_Rules[i].pTxt,Font_NoDispo);

  CursorInit();
  gMenu.nChoix=0;
}

u8 RulesMenuRules()
{
  s32 i;
  u8 nRetVal=MENU_Null;

  if(pGene->nKeyButton & KeyBtnReturn)
    {
      CellsGameMenu();
      pGene->nKeyButton=0;
      return MENU_Main;
    }

  else
    switch(RulesColMouse_MenuRules())
      {
      case MENU_Rules :
	nRetVal=MENU_Null;
	break;
      case MENU_Quit :
	RulesRulesDefault_and_Display();
	return MENU_Main;
      }

   DisplayBkg();
   RulesSprRulesDisplay();

  CursorMove();

  /*Liste des Régles du jeu de la vie par défaut*/
  for(i=0;i<NBELEM(Rules);i++)
    Font_Print(WIN_W/2-(Rules[i].nLg/2),START_StringRules_Y+(i*FONT_GAP_Y),Rules[i].pTxt,0);

  for(i=0;i<NBELEM(Menu_Rules);i++)
    {
      Font_Print(WIN_W/2-(Menu_Rules[i].nLg/2),MENU_RULES_Y+(i*FONT_GAP_Y),Menu_Rules[i].pTxt,0);
 
      if(i==gMenu.nChoix)
	{
	  Font_Print((WIN_W/2) - (Menu_Rules[i].nLg/2)-START_Curs+(gMenu.nCursPos>>8),MENU_RULES_Y+(i*FONT_GAP_Y),">",0);
	  Font_Print((WIN_W/2) + (Menu_Rules[i].nLg/2)+(START_Curs - FONT_W)-(gMenu.nCursPos>>8), MENU_RULES_Y + (i*FONT_GAP_Y),"<",0);
	}
    }

  RulesColMouseSprRules();
  return nRetVal;
}

void RulesColMouseSprRules()
{
  u32 i,j;
  u32 nPosCase_X=0;
  u32 nPosCase_Y=0;
  u8 val=0;


  if(pGene->nMouseButton & MouseBtnLeft)
    {
      for(j=0;j<NbLinesTab;j++)
	for(i=0;i<NbColumnsTab;i++)
	  {
	    if(RulesColBoxMouseRules(i,j))
	      {
		nPosCase_X=((pGene->nMousePosX-PosCase1_Line1_X)/CaseColumn_W)*CaseColumn_W+PosCase1_Line1_X;
		nPosCase_Y=((pGene->nMousePosY-PosCase1_Line1_Y)/CaseColumn_H)*CaseColumn_H+PosCase1_Line1_Y;

		if(j==0)
		  if(SgCell.TabBirth[i])
		    {
		      SgCell.TabBirth[i]=0;
		      val=0;
		    }
		  else
		    {
		      SgCell.TabBirth[i]=1;
		      val=1;
		    }

		else
		  if(SgCell.TabSurvival[i])
		    {
		      SgCell.TabSurvival[i]=0;
		      val=0;
		    }
		  else 
		    {
		      SgCell.TabSurvival[i]=1;
		      val=1;
		    }
		SprStoData(e_Spr_NbRules+val,nPosCase_X,nPosCase_Y,e_Prio_Rules);
		break;
	      }
	  }
    }
}

u8 RulesColBoxMouseRules(u32 i, u32 j)
{
  s32 XMin, XMax, YMin, YMax;
  s32 XMouse, YMouse;

  XMin=PosCase1_Line1_X+(CaseColumn_W*i);
  XMax=XMin+Case_W-1;
  YMin=PosCase1_Line1_Y+(CaseColumn_H*j);
  YMax=YMin+Case_H-1;
  XMouse=pGene->nMousePosX;
  YMouse=pGene->nMousePosY;

  if(XMouse>=XMin && XMouse<=XMax && YMouse>=YMin && YMouse<=YMax)
    return 1;
  else 
    return 0;
}

u32 RulesEventHandler()
{
  SDL_Event event;

  while(SDL_PollEvent(&event))
    {
      switch(event.type)
	{
	case SDL_KEYDOWN :
	  pGene->Key[event.key.keysym.sym]=1;
	  
	  if(pGene->Key[SDLK_ESCAPE] || pGene->Key[SDLK_a])
	    return 0;/*a == q*/

	  if(pGene->Key[SDLK_r])
	    RulesResetRules();

	  if(pGene->Key[SDLK_SPACE])
	    RulesRulesDefault_and_Display();

	  if(pGene->Key[SDLK_RETURN])
	    pGene->nKeyButton=KeyBtnReturn;
	  
	  break;

	case SDL_KEYUP :
	  pGene->Key[event.key.keysym.sym]=0;
	  break;

	case SDL_MOUSEBUTTONDOWN :
	  switch(event.button.button)
	    {
	       case SDL_BUTTON_LEFT :
		 pGene->nMouseButton=MouseBtnLeft;
		 break;
	       case SDL_BUTTON_RIGHT :
		 return 0;
	    }
	  break;

	case SDL_MOUSEBUTTONUP :
	  switch(event.button.button)
	    {
	       case SDL_BUTTON_LEFT :
		 pGene->nMouseButton=0;
		 break;
	    }
	  break;

	case SDL_MOUSEMOTION :
	  pGene->nMousePosX=event.motion.x;
	  pGene->nMousePosY=event.motion.y;
	  break;

	case SDL_QUIT :
	  exit(0); 
	}
    }
  return 1;
}

u8 rules()
{
  u8 nMenuVal=MENU_Null;

  RulesMenuRulesInit();

  FrameInit();

  while(nMenuVal==MENU_Null)
    {
      if(RulesEventHandler()==0)//echap et click right
	{
	  nMenuVal=MENU_Main;
	  break;
	}

      nMenuVal=RulesMenuRules();

      SprDisplayAll();
      RenderFlip(1);
      pGene->nMouseButton=0;
    }

  return nMenuVal;
}

