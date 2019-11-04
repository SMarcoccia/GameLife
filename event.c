#include "event.h"

SSaveScr SSave;

void FreeBoardScreen()
{
  SDL_FreeSurface(SSave.pSave);
  SSave.pSave=NULL;
}

void RestoreBoardScreen()
{
  u32 x,y;
  u8* pixScr=pGene->pScreen->pixels;
  u8* pixSave=SSave.pSave->pixels;

  for(y=0;y<WIN_H;y++)
    for(x=0;x<WIN_W;x++)
      *(pixScr+(pGene->pScreen->pitch*y)+x)=*(pixSave+(SSave.pSave->pitch*y)+x);
}

void SaveBoardScreen()
{
  u32 x,y;

  SSave.pSave=SDL_CreateRGBSurface(FLAGS,WIN_W,WIN_H,BPP,0,0,0,0);

  if( ! SSave.pSave)
    { 
      fprintf(stderr,"Error CreateRGBSurface in envent.c ; %s\n",SDL_GetError());
      exit(1);
    }

  u8* pixScr=pGene->pScreen->pixels;
  u8* pixSave=SSave.pSave->pixels;

  for(y=0;y<WIN_H;y++)
    for(x=0;x<WIN_W;x++)
      *(pixSave+(SSave.pSave->pitch*y)+x)=*(pixScr+(pGene->pScreen->pitch*y)+x);
}

u32 EventHandler()
{
  SDL_Event event;

  while(SDL_PollEvent(&event))
    {
      switch(event.type)
	{
	case SDL_KEYDOWN :
	  pGene->Key[event.key.keysym.sym]=1;
	  
	  if(pGene->Key[SDLK_ESCAPE])
	    return 1;

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
		 return 1;
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
