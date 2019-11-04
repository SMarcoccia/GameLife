#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <stddef.h>
#include <math.h>
#include <windows.h>
#include <SDL/SDL.h>


#include "global.h"
#include "tools.h"
#include "event.h"
#include "loadSprites.h"
#include "menu.h"
#include "font.h"
#include "frame.h"
#include "preca.h"
#include "render.h"
#include "cell.h"
#include "scanline.h"
#include "rules.h"



int main(int argc, char **argv)
{
  freopen("CON", "w", stdout);

  u8 nLoop=1;
  u32 nMenuVal=MENU_Main;
  u32 i;

  pGene=Global_Create();
  SprInitEngine();  
  ExgCellInit();/*SgCell.nPhase init in SgGameInit in cell.c Correspond � ExgBrkInit*/
  Sprites_LoadBMP("gfx/Sprites.bmp");
  Sprites_LoadBMP("gfx/Rules.bmp");
  Sprites_LoadBMP("gfx/font_small.bmp");

  for(int i = 0; i < SDLK_LAST;i++)
		pGene->Key[i]=0;	

  char* Menu_BkgFilename[MENU_NbBkg]={"gfx/Bkg_Starcraft.bmp","gfx/Bkg_warhammer.bmp"};
  for(i=0;i<MENU_NbBkg;i++)
    {
      if((pGene->pMenuBkg[i]=SDL_LoadBMP(Menu_BkgFilename[i]))==NULL)
	{
	  fprintf(stderr,"In main.c the fct main : SDL_LoadBMP as failed : %s\n",SDL_GetError());
	  exit(1);
	}
    }

  char* pBkgLevFilename[GFX_NbBkg]={"gfx/grid.bmp"};

  for(i=0;i<GFX_NbBkg;i++)
    {
      if((pGene->pGfxBkg[i]=SDL_LoadBMP(pBkgLevFilename[i]))==NULL)
	{
	  fprintf(stderr,"SDL_LoadBMP as failed %s : %s\n",pBkgLevFilename[i],SDL_GetError());
	  exit(1);
	}
    }

  PrecaSinCos();

  /*infinity loop*/
  while(nLoop)
    {
      switch(nMenuVal)
	{
	  /*MENU_Main plac� avec the other in menu.h */
	case MENU_Main : 
	  nMenuVal=menu();
	  break;
	case MENU_Place_Cells :
	  CellsGameMenu();
	  nMenuVal=MENU_Main;
	  break;
	case MENU_Rules :
	  nMenuVal=rules();
	  break;
	case MENU_Quit : 
	  nLoop=0;
	  break;
	}
    }

  Sprites_Release();
  CellFreeTabRules();
  CellFreeTabGame_All();
  Global_Destroy(pGene);

  argc=0; argv=NULL;
  return EXIT_SUCCESS;
}
