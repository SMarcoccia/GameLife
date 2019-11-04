#include "global.h"
         
      
extern SGene* Global_Create()
{
  SGene *J=(SGene*)malloc(sizeof*J);
  InitSDL_Create(J);
  return J;
}

void Global_Destroy(SGene *G)
{
  SDL_FreeSurface(G->pScreen);
  G->pScreen=NULL;

  SDL_FreeSurface(G->pMenuBackground);
  G->pMenuBackground=NULL;
}

void InitSDL_Create(SGene *S)
{
  if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
      fprintf(stderr,"Unable to init SDL : %s",SDL_GetError());lf;
      atexit(SDL_Quit);
    }

     S->pScreen=SDL_SetVideoMode(WIN_W, WIN_H, BPP,FLAGS);

  if(S->pScreen==NULL)
    {
      fprintf(stderr,"Unable to display SDL  : %s",SDL_GetError());lf;
      exit(0);
    }

SDL_SetColorKey(S->pScreen,SDL_SRCCOLORKEY,SDL_MapRGB(S->pScreen->format,0xff,0x0,0xff));

  SDL_WM_SetCaption("Game of Life",NULL);
}


