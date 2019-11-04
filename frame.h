#ifndef __FRAME_H__
#define __FRAME_H__

#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h>

#include "tools.h"
#include "cell.h"

#define FPS_Default (1000/60)

u32 nTimer1;


void FrameInit();
void FrameWait();


#endif


