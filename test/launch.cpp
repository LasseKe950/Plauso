/* */
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <stdio.h>
#include <string>
#include <stdlib.h>

#include "menu_ids.h"
#include "handler.h"
#include "image.h"
#include "button_defs.h"
/*
const image_s MX_500 = {
    451,
    201,
    399,
    320,
    //background: transparent url('img/MX500 W.png') 0% 0% no-repeat padding-box;
   (char*)"img/MX500 W.png",
   {0,0,0,0},
    1,
};
LButton *B_MX_500;
*/

int sec_ctr;

void enter_LAUNCH_pages(void)
{
}

int handler_LAUNCH(int x, int y,  SDL_Event *e)
{
//   printf("in handler LAUNCH\n");
#ifdef PC_UBUNTU
   if( e->type == SDL_MOUSEMOTION)
#else
   if( e->type == SDL_FINGERMOTION)
#endif
   {
   }
   else
   {
   }

   if (image_ptrs[IMAGE_DIETA_MX500] != nullptr)
      image_ptrs[IMAGE_DIETA_MX500]->render(201, 451);

   if (++sec_ctr > 10)
   {
      //Valkoinen tausta
      //SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
      //Update screen
      //SDL_RenderPresent( gRenderer );
      return ID_KOTINAKYMA;
   }
   else
      return 0xffff;
}

void draw_LAUNCH(int param1)
{
}

void init_LAUNCH(void)
{
}

void show_LAUNCH(int show_hide)
{
   sec_ctr = 0;
   if (show_hide != 0)
   {
      return;
   }
}
