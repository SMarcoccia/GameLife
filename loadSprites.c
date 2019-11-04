#include "loadSprites.h"

void ErasePixel()
{
  u8* CellPix=(u8*)pGene->pScreen->pixels;
  u32 i,j;
  SDL_LockSurface(pGene->pScreen);
  for(j=0;j<CELL_H;j++)
    for(i=0;i<CELL_W;i++)
      *(CellPix+(pGene->pScreen->pitch*(pGene->nMousePosY+j-CELL_H/2))+(pGene->nMousePosX+i-CELL_W/2))=CLR_BLACK_IDX;

  SDL_UnlockSurface(pGene->pScreen);
}


Sprites* SprGetDesc(u32 SprNo)
{
  return &sprites[SprNo];
}

void Sprites_Release()
{
  u32 i;
  for(i=0;i<NbSpr;i++)
    free(sprites[i].Gfx);
  free(sprites);
}


void Sprites_Realloc()
{
  sprites=(Sprites*)realloc(sprites,(NbSprMax+=SPR_REALLOC_UNIT)*sizeof*sprites);
  if(sprites==NULL)
    {
      printf("Sprites_Realloc has failed");
      Sprites_Release();
      exit(1);
    }
}


void Sprites_LoadBMP(char* filename)
{
  u32 x,y, ix=0, iy=0, ii, ij, ik, LgExt=0, HtExt=0;
  SDL_Surface* board=NULL;
  SDL_Color *colors=NULL;
  u8 *pix=NULL;
  u32 NbSprPlanche=0;

  board = SDL_LoadBMP(filename);
  if(board==NULL)
    {
      fprintf(stderr,"Loading the board sprites failed: %s\n",SDL_GetError());
      atexit(SDL_Quit);
    }

  colors=board->format->palette->colors; 
  pix=(u8*)board->pixels;
  SDL_SetColors(pGene->pScreen, colors, 0, 256);



#ifdef DEBUG
   printf("w=%d / h=%d\n",board->w,board->h);lf;
#endif

  /*Récupération des sprites d'une planche.*/
  for(y=0;y<(u32)board->h;y++)
    for(x=0;x<(u32)board->w;x++)
      {
	if(*(pix +(board->pitch*y)+x)==ALPHA)
	  {
	    
	    if(NbSpr>=NbSprMax)
	      Sprites_Realloc();
	      
	    ix=x+1;
	    while(*(pix +(board->pitch*y)+ix)==ALPHA)
	      LgExt=ix++;

	    iy=y+1;
	    while(*(pix +(board->pitch*iy)+x)==ALPHA) 
	      HtExt=iy++;


	    sprites[NbSpr].RefX=x+1;/*On rajoute 1 pour enlever le cadre ALPHA*/
	    sprites[NbSpr].RefY=y+1;
	    sprites[NbSpr].Lg=LgExt-(x+1);
	    sprites[NbSpr].Ht=HtExt-(y+1);

	    /*Création d'une surface Gfx + Mask*/	    

	    sprites[NbSpr].Gfx=(u8*)malloc(sprites[NbSpr].Lg*sprites[NbSpr].Ht*2);
	    
	    if(sprites[NbSpr].Gfx==NULL) 
	      {
		fprintf(stderr," Gfx : malloc failed : %s",SDL_GetError());
		exit(1);
	      }

	    sprites[NbSpr].Mask=sprites[NbSpr].Gfx+sprites[NbSpr].Lg*sprites[NbSpr].Ht;
	    ik=0;
	    /*Recovery and sprite generation of Mask*/
	    for(ij=0;ij<HtExt-(y+1);ij++){
	      for(ii=0;ii<LgExt-(x+1);ii++)
		{
		  sprites[NbSpr].Gfx[ik]=*(pix+(board->pitch*(y+ij+1))+(x+ii+1));
		  sprites[NbSpr].Mask[ik]=sprites[NbSpr].Gfx[ik]==ALPHA ? 0:255;
		    ik++;
		}
	  }

	    /*Erase SPR of the board*/
	    for(iy=y;iy<=HtExt;iy++)
	      for(ix=x;ix<=LgExt;ix++)
		*(pix+(board->pitch*iy)+ix)=BACKGROUND;
		  
	    sprites[NbSpr].RefX=0;
	    sprites[NbSpr].RefX=0;

	    NbSpr++;
	    NbSprPlanche++;
	  }
      }
  SDL_FreeSurface(board);
}

void SprInitEngine()
{
  sprites=NULL;
  NbSprMax=0;
  NbSpr=0;	  /* Nb de sprites capturés.*/
  NbSprSto=0;     /* Nb de sprites stockés pour affichage.*/
}

/*Cette fonction sert pour le positionnement du sprite a l'écran ; Question est-ce que toute les SprSto sont réinit à zéro ?*/
void SprStoData(u32 SprNo, s32 PosX, s32 PosY,u32 Prio)
{
  if(SprNo>=SPR_STOCK_MAX)
    {
      fprintf(stderr,"Sprites out of slot");
      return;
    }

  SprSto[NbSprSto].SprNo=SprNo;
  SprSto[NbSprSto].PosX=PosX;
  SprSto[NbSprSto].PosY=PosY;
  SprSto[NbSprSto].Prio=Prio;
  /*peut rajouter une struct pour le tri  copie de la struct SprSto*/
  NbSprSto++;
}

void SprDisplay(u32 SprNo, s32 PosX, s32 PosY,u32 nPrio)
{
  u32 i, j, ik=0;
  u8 *ScrPix = (u8 *)pGene->pScreen->pixels;
  nPrio=0;

  SDL_LockSurface(pGene->pScreen);
  for(j=0;j<sprites[SprNo].Ht;j++)
    for(i=0;i<sprites[SprNo].Lg;i++)
      {
	if(sprites[SprNo].Gfx[ik]!=ALPHA)
	    *(ScrPix+(pGene->pScreen->pitch*(PosY+j))+(PosX+i))=sprites[SprNo].Gfx[ik];
	ik++;
      }
  SDL_UnlockSurface(pGene->pScreen);
}

void SprDisplayAll()
{
  u32 i;

  for(i=0;i<NbSprSto;i++)
    SprDisplay(SprSto[i].SprNo,SprSto[i].PosX,SprSto[i].PosY,SprSto[i].Prio);

  NbSprSto=0;
}
