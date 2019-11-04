#include "render.h"

void RenderFlip(u8 nSync)
{
  if(nSync)
    FrameWait();
  SDL_Flip(pGene->pScreen);
}
