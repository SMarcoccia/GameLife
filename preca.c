#include "preca.h"

void PrecaSinCos()
{
  u32 i;

  for(i=0;i< 256 + 64 ;i++)
    pGene->pSinCos[i]=(s16)(cos(i*2*PI/256)*256);

  pGene->pCos=pGene->pSinCos;
  pGene->pSin=pGene->pSinCos + 64;
}


