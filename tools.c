#include "tools.h"



void ToolsBubble_Sort(u32 NbElem,u32 *Array)
{
  u32 i;

  bool nSwap=true;

  while(nSwap)
    {
      nSwap=false;
      for(i=0;i<NbElem-1;i++)
	{
	  if(Array[i]<Array[i+1])
	    {
	      ToolsSwap(Array+i,Array+i+1);
	      nSwap=true;
	    }
	}
      NbElem--;
    }
}


void ToolsSwap(u32 *Array1, u32 *Array2)
{
  u32 tmp=*Array1;
  *Array1=*Array2;
  *Array2=tmp;
}
