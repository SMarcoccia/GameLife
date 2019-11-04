#include "font.h"

u32 Font_Print(s32 PosX, s32 PosY, char* pStr,u32 flags)
{
  char cChr;
  Sprites *sprites;
  s32 PosXOrg=PosX;

  while(*pStr)
    {
      cChr=*pStr++;
      cChr-=' ';
      if(cChr!=0)
	{
	  cChr--;
	  sprites=SprGetDesc(e_Spr_FontSmall+cChr);

	  if((flags & Font_NoDispo) == 0)
	    SprStoData(e_Spr_FontSmall+cChr, PosX, PosY,e_Prio_Font);/*Note e_Prio_Font se trouve in loadsprite.h*/
	}
      else
	sprites=SprGetDesc(e_Spr_FontSmall+'I'-' ');/*espace d'un I si c'est une phrase*/
      PosX+=sprites->Lg+2;
    }

  return PosX-PosXOrg;
}
