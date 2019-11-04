#include "cell.h"


SMenu MenuGameCell[]=
  {
    {CELL_Game,0,"RETURN TO GAME"},
    {CELL_Place,0,"GO TO PLACE CELL"},
    {CELL_Rules,0,"GO TO RULES"},
    /*{CELL_Load,0,"LOAD (F7)"},
      {CELL_Save,0,"SAVE (F5)"},*/
    {CELL_Quit,0,"GO TO MAIN MENU"}
  };

SMenu MenuPlaceCell[]=
  {
    {CELL_Place,0,"RETURN TO PLACE CELL"},
    {CELL_Game,0,"START GAME"},
    {CELL_Rules,0,"GO TO RULE"},
    /*{CELL_Load,0,"LOAD (F7)"},
      {CELL_Save,0,"SAVE (F5)"},*/
    {CELL_Quit,0,"GO TO MAIN MENU"}
  };

#define NbElemGame  NBELEM(MenuGameCell)
#define NbElemPlace NBELEM(MenuPlaceCell)

#define MenuGame_Y  (NbElemGame*FONT_GAP_Y)
#define MenuPlace_Y (NbElemPlace*FONT_GAP_Y)

#define START_StringGame_Y WIN_H/2-(MenuGame_Y)/2
#define START_StringPlace_Y WIN_H/2-(MenuPlace_Y)/2

#define GapBkg_Y 15 
#define GapBkg_X 30

#define START_CursMenu 20

u32 ArrayGame[NbElemGame];
u32 ArrayPlace[NbElemPlace];

void CellBkgMenuPlace()
{
  u32 val=ArrayPlace[0]+GapBkg_X*2;

  pGene->pBkgRectSrc.x=WIN_W/2-(val/2);
  pGene->pBkgRectSrc.y=START_StringPlace_Y-GapBkg_Y;
  pGene->pBkgRectSrc.w=val;
  pGene->pBkgRectSrc.h=MenuPlace_Y+(GapBkg_Y*2)-GAP_Y;

  SDL_FillRect(pGene->pScreen,&(pGene->pBkgRectSrc),CLR_BLUE_IDX);
}

void CellMenuCellGameInit()
{
  pGene->nMouseButton=0;
  pGene->nKeyButton=0;

  u32 i;
  for(i=0;i<NbElemGame;i++)
    MenuGameCell[i].nLg=Font_Print(0,0,MenuGameCell[i].pTxt,Font_NoDispo);
  CursorInit();

  gMenu.nChoix=0;
}

void CellMenuCellGame()
{
  u32 i;
  u32 val=ArrayPlace[0]+GapBkg_X*2;

  pGene->pBkgRectSrc.x=WIN_W/2-(val/2);
  pGene->pBkgRectSrc.y=START_StringGame_Y-GapBkg_Y*2;
  pGene->pBkgRectSrc.w=val;
  pGene->pBkgRectSrc.h=START_StringGame_Y+GapBkg_Y*2;

  SDL_FillRect(pGene->pScreen,&(pGene->pBkgRectSrc),12);

  for(i=0;i<NBELEM(MenuGameCell);i++)
    Font_Print(WIN_W/2-(MenuGameCell[i].nLg/2),START_StringGame_Y+(i*FONT_GAP_Y),MenuGameCell[i].pTxt,0);
}

void CellMenuPlaceCellsInit()
{
  u32 i;

  for(i=0;i<NbElemPlace;i++)
    ArrayPlace[i]=MenuPlaceCell[i].nLg=Font_Print(0,0,MenuPlaceCell[i].pTxt,Font_NoDispo);

  ToolsBubble_Sort(NbElemPlace,ArrayPlace);/*pour cadrer le menu*/
  
  CursorInit();
  gMenu.nChoix=0;
  pGene->nKeyButton=0;
}

 u32 CellMenuPlaceCells()
  {
    s32 i=0, nRetVal=CELL_Null;

    CellBkgMenuPlace();

    for(i=0;i<NbElemPlace;i++)
      {
	Font_Print(WIN_W/2-(MenuPlaceCell[i].nLg/2),START_StringPlace_Y+(i*FONT_GAP_Y),MenuPlaceCell[i].pTxt,0);
	if(i==gMenu.nChoix)
	  {
		Font_Print(WIN_W/2-(MenuPlaceCell[i].nLg/2)-START_CursMenu+0,START_StringPlace_Y+(i*FONT_GAP_Y),">",0);
		Font_Print(WIN_W/2+(MenuPlaceCell[i].nLg/2)+(START_CursMenu-FONT_W)+0,START_StringPlace_Y+(i*FONT_GAP_Y),"<",0);
	  }
      }
    if(pGene->nMouseButton & MouseBtnRight)
      {
	nRetVal=CELL_Place;
	SgCell.nPhase=e_Game_Null;
	pGene->nMouseButton=0;
      }

    if(pGene->nKeyButton & KeyBtnReturn)
      {
	nRetVal=MenuPlaceCell[gMenu.nChoix].menuVal;
	SgCell.nPhase=e_Game_Null;
	pGene->nKeyButton=0;
      }

    return nRetVal;
  }


/*==================CELL NEXT GEN========================*/

void CellNextGen()
{
  u32 x, y, nb;

  for(y=0;y<SCR_CELL_H;y++)
    for(x=0;x<SCR_CELL_W;x++)
      {
	nb=CellCountNeighbors(x,y);

	if(SgCell.TabBirth[nb])
	  SgCell.TabGame2[x][y]=1;

	else if( ! SgCell.TabSurvival[nb])
	  SgCell.TabGame2[x][y]=0;
      }
  CellCopyTabGame2_To_1();
}

u32 CellCountNeighbors(s32 X, s32 Y)
{
  s32 x,y, xi=0, yi=0, i=0;

  for(y = Y-1; y<=Y+1; y++)/*pour Y et X <= X ou Y +1 c'est normal il faut faire comme �a*/
    for(x = X-1; x<=X+1; x++)
      if(x!=X || y!=Y)
	{
	  if(x>=0)
	    xi=x % SCR_CELL_W;
	  else 
	    xi=SCR_CELL_W-1;

	  if(y>=0)
	    yi=y % SCR_CELL_H;
	  else 
	    yi=SCR_CELL_H-1;

	  if(SgCell.TabGame1[xi][yi])
	    i++;
	}

  return i;
}

/*=================CELLS GAME============================*/

u8 CellGameCells()
{
  u32 nCellVal=CELL_Game;
  u32 nTimer2=0, nTimer3=0;
  u32 nValTime=FPS_Default, nValTime1=40;
  bool nValPause=false;

  FrameInit();

  while(nCellVal==CELL_Game)
    {
      while(1)
	{
	  switch(CellEventHandler(1))
	    {
	    case e_Game_Place_Cells : 
	      SgCell.nTime=FPS_Default;
	      return CELL_Place;

	    case e_Game_Rules :
	      SgCell.nTime=FPS_Default;
	      return CELL_Rules;

	    case e_Game_Quit :
	      SgCell.nTime=FPS_Default;
	      return CELL_Quit;
	    }

	  if(pGene->nMouseButton==MouseBtnRight)
	    {
	      SgCell.nTime=FPS_Default;
	      return CELL_Place;
	    }

	  nTimer3++;
	  if(nTimer3>=nValTime1)
	    {
	      CellFrameModular_Plus();
	      CellFrameModular_Minus();
	      nValTime=SgCell.nTime;
	      nTimer3=0;
	    }

      	  nTimer2=SDL_GetTicks()-nTimer1;
	  
	  if(nTimer2 >= nValTime && SgCell.nPhase!=e_Game_Pause)
	    {
	      SDL_BlitSurface(pGene->pGfxBackground,NULL,pGene->pScreen,NULL);
	      CellGameCells_Aux();
	      SprDisplayAll();
	      RenderFlip(0);
	      break;
	    }
	  SDL_Delay(3);

	  if(SgCell.nPhase==e_Game_Pause)
	    {
	      if(! nValPause)
		{
		  char* pStrPause="--PAUSE--";
		  u32 i=Font_Print(0,0,pStrPause,Font_NoDispo);
		  Font_Print((WIN_W/2)-(i/2),WIN_H/2-20,pStrPause,0);
		  SprDisplayAll();
		  RenderFlip(0);
		  /*a revoir*/SDL_Delay(2000);
		  nValPause=true;
		}
	      if(nValPause)
		{
		  SDL_BlitSurface(pGene->pGfxBackground,NULL,pGene->pScreen,NULL);
		  CellBrowsingGameBoardSto();
		  SprDisplayAll();
		  RenderFlip(0);
		}
	    }
	  if(SgCell.nPhase!=e_Game_Pause)
	    nValPause=false;
	}
      nTimer1=SDL_GetTicks();
    }
  return CELL_Place;
}

void CellGameCells_Aux()
{
  CellNextGen();
  CellBrowsingGameBoardSto();
}

void CellBrowsingGameBoardSto()
{
  u32 x,y;
  for(y=0;y<SCR_CELL_H;y++)
    for(x=0;x<SCR_CELL_W;x++)
      if(SgCell.TabGame1[x][y])
	SprStoData(e_Spr_Cell,x*CELL_W,y*CELL_H,e_Prio_Cell);
}


/*=======================================================*/

void CellBkgDisplay(u8 bkg)
{
  pGene->pGfxBackground=pGene->pGfxBkg[bkg];/*grid*/
  SDL_BlitSurface(pGene->pGfxBackground,NULL,pGene->pScreen,NULL);
}

u32 CellPlaceCells()
{
  u32 nCellVal=CELL_Null;
  //u32 nCellVal_2=CELL_Menu_Place;
  u32 nTime=0,nTime1=0/*,nTime2=0*/;
  bool nVal=true, nVal1=false;

  SgCell.nPhase=e_Game_Null;

  CellBkgDisplay(0);
  CellMenuPlaceCellsInit();

  CellResetTabGame_All();

  FrameInit();

  while(nCellVal==CELL_Null)
    {	  
      switch(CellEventHandler(0))//barre d'espace et return
	{
	case e_Game_Rules :
	  return CELL_Rules;
	case e_Game_Quit :
	  return CELL_Quit;
	}

      if(SgCell.nPhase==e_Game_Pause)
	{	  
	  if(nVal && (nTime++) == 20 && (pGene->Key[SDLK_UP] || pGene->Key[SDLK_DOWN]))
	    {
	      nVal=false;
	      nVal1=true;
	    }

	  if(nVal1 && (nTime1++)==10)
	    {	
	      if(pGene->Key[SDLK_UP])
		{
		  gMenu.nChoix--;
		  if(gMenu.nChoix<0)
		    gMenu.nChoix=NbElemPlace-1;
		}

	      if(pGene->Key[SDLK_DOWN])
		{
		  gMenu.nChoix++;
		  if(gMenu.nChoix >=NbElemPlace)
		    gMenu.nChoix=0;
		}
	      nTime1=0;
	    }

	  if( ! (pGene->Key[SDLK_UP]) && ( ! pGene->Key[SDLK_DOWN]))
	    {
	      nVal=true;
	      nVal1=false;
	      nTime=0;
	    }

	  nCellVal=CellMenuPlaceCells();
	  switch(nCellVal)
	    {
	    case CELL_Null :
	      break;
	    case CELL_Place :
	      SprDisplayAll();
	      RenderFlip(0);
	      SDL_BlitSurface(pGene->pGfxBackground,NULL,pGene->pScreen,NULL);
	      CellBrowsingGameBoardSto();
	      nCellVal=CELL_Null;
	      break;
	    case CELL_Game :
	      CellCopyTabGame1_To_2();
	      break;
	      /*case CELL_Rules :
		break;
		case CELL_Load :
		break;
		case CELL_Save :
		break;
		case CELL_Quit :
		break;*/
	    }
	}

      else 
	{
	  if(SgCell.nPhase==e_Game_Normal)
	    {
	      CellCopyTabGame1_To_2();
	      nCellVal=CELL_Game;
	    }
	  if(SgCell.nPhase!=e_Game_Pause)
	    CellPlaceCells_Aux();
	  
	}

      SprDisplayAll();
      RenderFlip(1);
    }

  return nCellVal;
}

void CellPlaceCells_Aux()
{
  u8 nClick=false;
  u8 nState=-1;

  if(pGene->nMouseButton & MouseBtnLeft)
    {
      nClick=true;
      nState=e_Cell_Paint;
    }
	
  else if(pGene->nMouseButton & MouseBtnRight)
    {
      nClick=true;
      nState=e_Cell_Eraser;
    }
      
  if(nClick)
    ScanlineFloodFill_PlaceCell(nState);
  //CellSprCellPlaceCells(nState);
}


void CellSprCellPlaceCells(u8 nState)
{
  s32 x=pGene->nMousePosX;
  s32 y=pGene->nMousePosY;
  u8 val=0;
  if(y<0) y=0;
  
  x=x/CELL_W;
  y=y/CELL_H;

  if(SgCell.TabGame1[x][y])
    {
      if(nState==e_Cell_Eraser)
	{
	  val=1;/*Spr black*/
	  SgCell.TabGame1[x][y]=e_Is_Dead;
	}
    }
  else
    {
      if(nState==e_Cell_Paint)
	{
	  val=0;
	  SgCell.TabGame1[x][y]=e_Is_Alive;
	}
      else 
	return;
    }

  SprStoData(e_Spr_Cell+val,x*CELL_W,y*CELL_H,e_Prio_Cell);
}

/*==================RULES================================*/

void CellTabRules_Reset()
{
  u32 i;
  for(i=0;i<SIZE_Tab_Birth;i++)
    SgCell.TabBirth[i]=0;

  for(i=0;i<SIZE_Tab_Survival;i++)
    SgCell.TabSurvival[i]=0;
}

void CellTabRules_Default()
{
  CellTabRules_Reset();

  SgCell.TabBirth[3]=1;
  SgCell.TabSurvival[2]=1;
  SgCell.TabSurvival[3]=1;
}

void CellFreeTabRules()
{
  free(SgCell.TabBirth);
  free(SgCell.TabSurvival);
}

void CellInitTabRules()
{
  SgCell.TabBirth=(u8*)calloc(SIZE_Tab_Birth,sizeof*SgCell.TabBirth);
  SgCell.TabSurvival=(u8*)calloc(SIZE_Tab_Survival,sizeof*SgCell.TabSurvival);

  SgCell.TabBirth[3]=1;
  SgCell.TabSurvival[2]=1;
  SgCell.TabSurvival[3]=1;
}

/*===============TABGAME=================================*/

void CellCopyTabGame2_To_1()
{
  u32 x,y;

  for(y=0;y<SCR_CELL_H;y++)
    for(x=0;x<SCR_CELL_W;x++)
      SgCell.TabGame1[x][y]=SgCell.TabGame2[x][y];
}

void CellCopyTabGame1_To_2()
{
  u32 x,y;

  for(y=0;y<SCR_CELL_H;y++)
    for(x=0;x<SCR_CELL_W;x++)
      SgCell.TabGame2[x][y]=SgCell.TabGame1[x][y];
}

void CellResetTabGame_All()
{
  u32 i,j;

  for(i=0;i<SCR_CELL_W;i++)
    for(j=0;j<SCR_CELL_H;j++)
      {
	SgCell.TabGame1[i][j]=0;
	SgCell.TabGame2[i][j]=0;
      }
}

void CellCreateInitTabGame_All()
{
  s32 i;

  SgCell.TabGame1    =(u8**)malloc(SCR_CELL_W*sizeof*SgCell.TabGame1);
  SgCell.TabGame2=(u8**)malloc(SCR_CELL_W*sizeof*SgCell.TabGame2);

  if(SgCell.TabGame1==NULL || SgCell.TabGame2==NULL)
    {
      fprintf(stderr,"failed alloc TabGame : %s", SDL_GetError());
      exit(1);
    }

  for(i=0;i<SCR_CELL_W;i++)
    {
      SgCell.TabGame1[i]=(u8*)calloc(SCR_CELL_H,sizeof*SgCell.TabGame1[i]);
      SgCell.TabGame2[i]=(u8*)calloc(SCR_CELL_H,sizeof*SgCell.TabGame2[i]);

      if(SgCell.TabGame1[i]==NULL)
	{
	  for(;i>=0;--i)
	    free(SgCell.TabGame1[i]);
	  fprintf(stderr,"failed alloc TabGame : %s", SDL_GetError());
	  exit(1);
	}

      if(SgCell.TabGame2[i]==NULL)
	{
	  for(;i>=0;--i)
	    free(SgCell.TabGame2[i]);
	  fprintf(stderr,"failed alloc TabGame : %s", SDL_GetError());
	  exit(1);
	}
    }
}

void CellFreeTabGame_All()
{
  u32 i;
  u32 varW=SCR_CELL_W;
    
  for(i=0;i<varW;i++)
    {
      free(SgCell.TabGame1[i]);
      free(SgCell.TabGame2[i]);

      SgCell.TabGame1[i]=NULL;
      SgCell.TabGame2[i]=NULL;
    }
  free(SgCell.TabGame1);
  free(SgCell.TabGame2);

  SgCell.TabGame1=NULL;
  SgCell.TabGame2=NULL;
}

/*======================================================*/

void ExgCellInit()
{
  SgExg.nExitCode=0;
  CellInit();
}

void CellInit()
{
  SgCell.nTime=FPS_Default;
  SgCell.nPhase=e_Game_Place_Cells;
  CellCreateInitTabGame_All();
  CellInitTabRules();
}

void CellsGameMenu()
{
  u32 nCellVal=CELL_Place;

  SgExg.nExitCode=0;/*ATTENTION ici avant il y avait ExgCellInit();*/

  pGene->nMouseButton=0;/*Note obliger de mettre sinon quand on clic sur place cell il garde en m�moire le 1 et allume le pixel quand on arrive devant la grille*/
  
  while(SgExg.nExitCode==0)
    {
      switch(nCellVal)
	{
	case CELL_Place :
	  nCellVal=CellPlaceCells();
	  break;
	case CELL_Game :
	  nCellVal=CellGameCells();
	  break;
	case CELL_Rules :
	  rules();
	  nCellVal=CELL_Quit;
	  break;
	case CELL_Load :
	  printf("CELL_Load\n");
	  break;
	case CELL_Save :
	  printf("CELL_Save\n");
	  break;
	case CELL_Quit :
	  SgExg.nExitCode=1;
	  break;
	}
    }
}

u32 CellEventHandler(u8 nInGame)
{
  SDL_Event event;
  static u32 nLastPhase;/*static permet de garder la valeur de nLastPhase*/

  while(SDL_PollEvent(&event))
    {
      switch(event.type)
	{
        case SDL_KEYDOWN :
	  pGene->Key[event.key.keysym.sym]=1;
	  
	  if(pGene->Key[SDLK_ESCAPE] || pGene->Key[SDLK_a])
	    return e_Game_Quit;/*return main menu*/
	  
	  if( ! nInGame && SgCell.nPhase==e_Game_Pause)
	    {
	      if(pGene->Key[SDLK_UP])
		{
		  gMenu.nChoix--;
		  if(gMenu.nChoix<0)
		    gMenu.nChoix=NbElemPlace-1;
		}

	      if(pGene->Key[SDLK_DOWN])
		{
		  gMenu.nChoix++;
		  if(gMenu.nChoix >=NbElemPlace)
		    gMenu.nChoix=0;
		}
	    }
	  
          if( ! nInGame && pGene->Key[SDLK_SPACE])
	    {
	      if(SgCell.nPhase==e_Game_Pause)
		{
		  SgCell.nPhase=nLastPhase;
		  gMenu.nChoix=0;
		  SDL_BlitSurface(pGene->pGfxBackground,NULL,pGene->pScreen,NULL);
		  CellBrowsingGameBoardSto();
		  SprDisplayAll();
		  RenderFlip(0);
		}
	      else
		{
		  nLastPhase=SgCell.nPhase;
		  SgCell.nPhase=e_Game_Pause;
		}
	    }

	  else if(nInGame && pGene->Key[SDLK_SPACE])
	    return e_Game_Place_Cells;

	  if( ! nInGame && pGene->Key[SDLK_RETURN])
	    {
	      if(SgCell.nPhase!=e_Game_Pause)
		SgCell.nPhase=e_Game_Normal;/*game*/
	      //pGene->nKeyButton=0;
	      else 
		pGene->nKeyButton=KeyBtnReturn;
	    }

	  if(nInGame && pGene->Key[SDLK_RETURN])
	    printf("--valider un choice---\n");

	  if(pGene->Key[SDLK_SEMICOLON])
	    return e_Game_Rules; /*semicolon == m renvoie au Menu des rules*/

	  /*Erase the board game more reset array */
	  if( ! nInGame && pGene->Key[SDLK_e])
	    {	
	      CellResetTabGame_All();
	      SDL_BlitSurface(pGene->pGfxBackground,NULL,pGene->pScreen,NULL);
	    }
	  
	  /*Pause*/
	  if(nInGame & pGene->Key[SDLK_p])
	    {
	      /*sort de la pause*/
	      if(SgCell.nPhase==e_Game_Pause)
		SgCell.nPhase=nLastPhase;

	      /*rentre en pause*/
	      else
		{
		  nLastPhase=SgCell.nPhase;
		  SgCell.nPhase=e_Game_Pause;
		}
	    }

	  break;

	case SDL_KEYUP :
	  pGene->Key[event.key.keysym.sym]=0;
	  pGene->nKeyButton=0;
	  break;

	case SDL_MOUSEBUTTONDOWN :
	  switch(event.button.button)
	    {
	       case SDL_BUTTON_LEFT :
		 pGene->nMouseButton=MouseBtnLeft;
		 break;
	       case SDL_BUTTON_RIGHT :
		 pGene->nMouseButton=MouseBtnRight;
		 break;
	    }
	  break;

	case SDL_MOUSEBUTTONUP :
	  switch(event.button.button)
	    {
	       case SDL_BUTTON_LEFT :
		 pGene->nMouseButton=0;
		 break;
	       case SDL_BUTTON_RIGHT :
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
	  break;
	}
    }

  return 0;
}
/*===================FRAME FRAME=========================*/

void CellFrameModular_Plus()
{
  if(pGene->Key[SDLK_KP_PLUS])
    {
      if(SgCell.nTime < 6)
	{
	  SgCell.nTime++;
	  printf("SgCell.nTime=%d\n",SgCell.nTime);
	}

      else if(SgCell.nTime > 5 && SgCell.nTime < 20)
	{
	  SgCell.nTime++;
	  printf("SgCell.nTime=%d\n",SgCell.nTime);
	}
	
      else if(SgCell.nTime >=20  && SgCell.nTime < 300)
	{
	  SgCell.nTime+=10;
	  printf("SgCell.nTime=%d\n",SgCell.nTime);
	}
      else if(SgCell.nTime >= 300 && SgCell.nTime < 1000)
	{
	  SgCell.nTime+=100;
	  printf("SgCell.nTime=%d\n",SgCell.nTime);
	}

      else if(SgCell.nTime >= 1000 && SgCell.nTime < 5000)
	{
	  SgCell.nTime+=250;
	  printf("SgCell.nTime=%d\n",SgCell.nTime);
	}
    }
}

void CellFrameModular_Minus()
{
  if(pGene->Key[SDLK_KP_MINUS] && SgCell.nTime >0)
    {
      if(SgCell.nTime < 6)
	{
	  SgCell.nTime--;
	  printf("SgCell.nTime=%d\n",SgCell.nTime);
	}

      else if(SgCell.nTime > 5 && SgCell.nTime<=20)
	{
	  SgCell.nTime--;
	  printf("SgCell.nTime=%d\n",SgCell.nTime);
	}
	
      else if(SgCell.nTime >= 20 && SgCell.nTime < 300)
	{
	  SgCell.nTime-=10;
	  printf("SgCell.nTime=%d\n",SgCell.nTime);
	}
      else if(SgCell.nTime >= 300 && SgCell.nTime <= 1000)
	{
	  SgCell.nTime-=100;
	  printf("SgCell.nTime=%d\n",SgCell.nTime);
	}
      else if(SgCell.nTime > 1000)
	{
	  SgCell.nTime-=250;
	  printf("SgCell.nTime=%d\n",SgCell.nTime);
	}
    }
}


