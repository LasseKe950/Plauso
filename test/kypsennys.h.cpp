/* 
   alku2.c 24. helmikuu 2012:
   Uusi aloitusruutu jossa pikakäyttö/helppo/suosikit/ohjelmat valinnat
   */
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <stdio.h>
#include <string>
#include <stdlib.h>

#include "menu_ids.h"
#include "handler.h"
#include "button.h"

#include "img_header.h"

const image_s Rect1 = {
   70,
   0,
   800,
   232,
   //background: rgba(88, 91, 96, 1) 0% 0% no-repeat padding-box;
   0,
   {88, 91, 96, 1},
   1,
};
LButton *rect1;

const image_s Rect2 = {
   301,
   0,
   800,
   232,
   //background: rgba(88, 91, 96, 1) 0% 0% no-repeat padding-box;
   0,
   {88, 91, 96, 1},
   1,
};
LButton *rect2;

const image_s Rect3 = {
   532,
   0,
   800,
   232,
   //background: rgba(88, 91, 96, 1) 0% 0% no-repeat padding-box;
   0,
   {88, 91, 96, 1},
   1,
};
LButton *rect3;

const image_s Rect4 = {
   767,
   0,
   800,
   232,
   //background: rgba(88, 91, 96, 1) 0% 0% no-repeat padding-box;
   0,
   {88, 91, 96, 1},
   1,
};
LButton *rect4;

const image_s ArtWork2 = {
    13,
    16,
    125,
    40,
   //background: transparent url('img/Artwork 2.png') 0% 0% no-repeat padding-box;
   "img/Artwork 2.png",
   {0,0,0,0},
    1,
};

LButton *aw2;

void enter_KYPSENNYS_pages(void)
{
}

int handler_KYPSENNYS(int x, int y,  SDL_Event *e)
{
//   printf("in handler LAUNCH\n");
#ifdef PC_UBUNTU
   if( e->type == SDL_MOUSEMOTION)
#else
   if( e->type == SDL_FINGERMOTION)
#endif
   {
#ifdef PC_UBUNTU
//      SDL_GetMouseState( &b4x, &b4y );
#else
#endif
   }
   else
   {
   }

   if (aw2 != nullptr)
      aw2->render(aw2->image_s_ptr->left, aw2->image_s_ptr->top);

   if (rect1 != nullptr)
      rect1->render(rect1->image_s_ptr->left, rect1->image_s_ptr->top);

   if (rect2 != nullptr)
      rect2->render(rect2->image_s_ptr->left, rect2->image_s_ptr->top);

   return 0xffff;
}

void init_KYPSENNYS(void)
{
   aw2 = new LButton((image_s *)&ArtWork2);
   rect1 = new LButton((image_s *)&Rect1, BUTTON_TYPE_RECTANGLE);
   rect2 = new LButton((image_s *)&Rect2, BUTTON_TYPE_RECTANGLE);
   if (aw2==nullptr || rect1==nullptr)
      printf("Error makin button!\n");
}

void show_KYPSENNYS(int show_hide)
{
   if (show_hide == 0)
   {
      return;
   }
}
