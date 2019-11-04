#include "menu.h"


#define VAR_Adjust_Menu_Y 50 //Permet de d�placer le menu dans le buffer SDL
#define START_StringMenu_Y WIN_H/2-(NBELEM(SMenuMain)*(FONT_GAP_Y))/2+VAR_Adjust_Menu_Y
#define NB_CHARA_LOGO 10
#define VAR_Adjust_Logo_Y -100



SMenu SMenuMain[]=
  {
    { MENU_Place_Cells,0,"PLACE CELLS" },
    { MENU_Rules,0,"RULES" },
    { MENU_Quit,0, "QUIT" }
  };


/*Recovery lenght of string*/
void SMenu_MenuInit()
{
  u32 i;

  SMenuGen_MenuInitFade();

  for(i=0;i< NBELEM(SMenuMain);i++)
    SMenuMain[i].nLg=Font_Print(0,0,SMenuMain[i].pTxt,Font_NoDispo);/*comme on r�cup�re la lenght de la string on met Font_NoDispo*/

  CursorInit(); 
  gMenu.nChoix=0;
}

void SMenuGen_MenuInitFade()
{
  gMenu.nOldMousePosX=WIN_W/2;
  gMenu.nOldMousePosY=WIN_H/2;
}

void CursorInit()
{
  gMenu.nCursPos=0;
  gMenu.nCursAccel=CURS_Acc;
}

void CursorMove()
{
  gMenu.nCursAccel-=0x20;
  gMenu.nCursPos+=gMenu.nCursAccel;
  if(gMenu.nCursPos<0) CursorInit();
}

u32 SMenu_Menu()
{
  s32 i;
  u32 nRetVal=MENU_Null;


/*========CHOIX MENU AVEC LA SOURIS 2eme METHODE=========*/

  s32 nLastChoix=gMenu.nChoix;  

  for(i=0;i<(s32)NBELEM(SMenuMain);i++)
    {
      if(ColBoxMouseMenu(i) >= 0)
	{
	  gMenu.nChoix=i;
	  break;
	}
      else
	gMenu.nChoix=-1;
    }

  if(nLastChoix!=gMenu.nChoix) 
    CursorInit();/*remet � la position de d�part*/

  if(pGene->nMouseButton & MouseBtnLeft)
    if(gMenu.nChoix >= 0)
      nRetVal=SMenuMain[gMenu.nChoix].menuVal;


/*============CHOIX MENU==AVEC LA SOURIS 1er METHODE=====*/

  //s32 nDiff;
  //static u8 nWait=0;

  /*if(nWait)
    nWait--;

  if(nWait==0)  
    { 
      nDiff=pGene->nMousePosY-gMenu.nOldMousePosY;
      if(ABS(nDiff)>5)
	{
	  u32 nLastChoix = gMenu.nChoix;
	  if(nDiff < 0)
	    {
	      if(gMenu.nChoix>0) gMenu.nChoix--;
	    }
	  else
	    if(gMenu.nChoix<NBELEM(SMenuMain)-1) gMenu.nChoix++;
	  if (nLastChoix != gMenu.nChoix)  
	    nWait=30;
	}
    }

  if(pGene->nMouseButton & MouseBtnLeft)
    nRetVal=SMenuMain[gMenu.nChoix].menuVal;

    SDL_WarpMouse(WIN_W/2,WIN_H/2);*/
/*=======================================================*/

/*====================Logo Statique======================*/
  
/*SprStoData(e_Spr_Logo,WIN_W/2-sprites[e_Spr_Logo].Lg/2,200,e_Prio_Logo);*/


/*==================LOGO QUI BOUGE=======================*/

	s32	pOfs[] = { 0, 25, 50, 75, 125, 150, 200, 225, 250, 275};//Gap between Character
	static	u8	nSin = 0;  
	for (i = 0; i < NB_CHARA_LOGO; i++)  
	{  
		s32	nMul = NB_CHARA_LOGO;  
		SprStoData(e_Spr_Logo_Character + i,
			   (WIN_W / 2) - (sprites[e_Spr_Logo_Character].Lg*NB_CHARA_LOGO)/2 + pOfs[i] + (pGene->pCos[(nSin + (i * nMul)) & 0xFF] / 8),  
			   (WIN_H/2) + VAR_Adjust_Logo_Y - (pGene->pSin[(nSin + (i * nMul)) & 0xFF] / 16), e_Prio_Logo_Character);  
	}  
	nSin -= 2;  

/*=======================================================*/



/*===============MENU====================================*/
	
	CursorMove();

	for(i=0;i<(s32)NBELEM(SMenuMain);i++)
	  {
	    Font_Print(WIN_W/2-SMenuMain[i].nLg/2,START_StringMenu_Y+(i*FONT_GAP_Y),SMenuMain[i].pTxt,0);

	    if (i == gMenu.nChoix)  
	      {  
		Font_Print((WIN_W / 2) - (SMenuMain[i].nLg / 2) - START_Curs + (gMenu.nCursPos>>8) , START_StringMenu_Y + (i * FONT_GAP_Y), ">", 0);  

		Font_Print((WIN_W / 2) + (SMenuMain[i].nLg / 2) + (START_Curs - FONT_W) - (gMenu.nCursPos>>8), START_StringMenu_Y + (i * FONT_GAP_Y), "<", 0);      
	      }  
	  }
 	return nRetVal;
}



s8 ColBoxMouseMenu(u32 i)
{

  s32 XMin, XMax, YMin, YMax;
  s32 XMouse, YMouse;

  XMin = WIN_W/2-SMenuMain[i].nLg/2;
  XMax = XMin + SMenuMain[i].nLg;
  YMin = START_StringMenu_Y + (i*FONT_GAP_Y);
  YMax = YMin + sprites[e_Spr_FontSmall+'A'-' '].Ht+GAP_Y;
  XMouse = pGene->nMousePosX;
  YMouse = pGene->nMousePosY;


  if(XMouse>=XMin && XMouse<=XMax && YMouse>=YMin && YMouse <=YMax)
    return i;
    
  else 
    return -1;
}

u32 menu()
{
  u32 nMenuVal=MENU_Null;

  pGene->pMenuBackground=pGene->pMenuBkg[0]; /*Background of menu per default*/

  SMenu_MenuInit();/*recovery lenght string of menu*/

  FrameInit();
  
  while(nMenuVal==MENU_Null)
    {
      pGene->nMouseButton=0;

      if(EventHandler()!=0)//permet de sortir de la boucle
      {
        nMenuVal=MENU_Quit;
        break;
      }

      SDL_BlitSurface(pGene->pMenuBackground,NULL, pGene->pScreen,NULL);/*Background StarCraft*/

      nMenuVal=SMenu_Menu();//put the Pos X et Y of string in the struct sto  

      SprDisplayAll();
      RenderFlip(1);
    }

  return nMenuVal;
}






