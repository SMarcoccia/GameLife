#include "scanline.h"

SStackPix *SStack;
SStackPix *TMP;



/*============POUR PLACE CELL============================*/

void ScanlineCreateInitPile(s32 x, s32 y)
{
  TMP=NULL;
  SStack=malloc(sizeof*SStack);
  SStack->x=x;
  SStack->y=y;
  SStack->next=NULL;
}

void ScanlineCreateCellPile(s32 x, s32 y)
{
  TMP=malloc(sizeof*TMP);
  TMP->next=SStack;
  SStack=TMP;
  SStack->x=x;
  SStack->y=y;
  TMP=NULL;
}

void ScanlineFreeCellPile()
{
  TMP=SStack;
  SStack=SStack->next;
  free(TMP);
  TMP=NULL;
}

void ScanlineFloodFill_PlaceCell(u8 nState)
{
  u8 clrPix=0;/*pixel en dure dans le buffer sdl*/
  u8 color=CLR_GREEN_IDX;/*color du future pixel*/
  u8 PixUp, PixDown;
  
  s32 x=pGene->nMousePosX;
  s32 y=pGene->nMousePosY;

  s32 X=x/CELL_W, Y=y/CELL_H;

  if(GetClrPixel(x,y)==CLR_WHITE_IDX)
    {
      if(y>0) y--;
      x--;
    }

  ScanlineCreateInitPile(x,y);

  if(SgCell.TabGame1[X][Y])
    {
      if(nState==e_Cell_Eraser)
	{
	  clrPix=CLR_GREEN_IDX;
	  color=CLR_BLACK_IDX;
	  SgCell.TabGame1[X][Y]=e_Is_Dead;
	}
    }
  else
    {
      if(nState==e_Cell_Paint)
	{
	  clrPix=CLR_BLACK_IDX;
	  color=CLR_GREEN_IDX;
	  SgCell.TabGame1[X][Y]=e_Is_Alive;
	}
      else 
	return;
    }

  while(SStack!=NULL)
    {
      x=SStack->x;
      y=SStack->y;

      ScanlineFreeCellPile();

      while(x>=0 && GetClrPixel(x,y)==clrPix)
	  x--;
      
      x++;

      PixUp=false;
      PixDown=false;

      while(x < WIN_W && GetClrPixel(x,y)==clrPix)
	{
	  PutPixel(x,y,color);
	  if(y>0)
	    {
	      if(GetClrPixel(x,y-1)==clrPix)
		{
		  if(!PixDown)
		    {
		      ScanlineCreateCellPile(x,y-1);
		      PixDown=true;
		    }
		}
	      else if(PixDown)
		PixDown=false;
	    }

	  if(y<WIN_H-1)
	    {
	      if(x==0 || GetClrPixel(x,y+1)==clrPix)
		{
		  if(!PixUp)
		    {
		      ScanlineCreateCellPile(x,y+1);
		      PixUp=true;
		    }
		}
	      else if(PixUp)
		PixUp=false;
	    }
	  x++;
	}
    }
}

void PutPixel(u32 x,u32 y,u8 CLR)
{
  u8* pix=(u8*)pGene->pScreen->pixels;
  SDL_LockSurface(pGene->pScreen);
  *(pix+(pGene->pScreen->pitch*y)+x)=CLR;
  SDL_UnlockSurface(pGene->pScreen);
}

u8 GetClrPixel(u32 x, u32 y)
{
  u8* pix=(u8*)pGene->pScreen->pixels;
  SDL_LockSurface(pGene->pScreen);
  u8 val=*(pix+(pGene->pScreen->pitch*y)+x);
  SDL_UnlockSurface(pGene->pScreen);

  return val;
}

u8 GetClrPixelMouse()
{
  u8* pix=(u8*)pGene->pScreen->pixels;
  SDL_LockSurface(pGene->pScreen);
  u8 val= *(pix+(pGene->pScreen->pitch*pGene->nMousePosY) + pGene->nMousePosX);
  SDL_UnlockSurface(pGene->pScreen);

 return val;
}

