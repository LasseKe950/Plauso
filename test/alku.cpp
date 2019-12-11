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

#define Button1_X 0
#define Button1_Y 0

#define Button2_X 150
#define Button2_Y 0

#define Button3_X 300
#define Button3_Y 0

#define Button4_X 300
#define Button4_Y 300

LButton *Button1;
LButton *Button2;
LButton *Button3;
LButton *Button4;
int b4y, b4x;

void enter_ALKU_pages(void)
{
}

int handler_ALKU(int x, int y,  SDL_Event *e)
{
//   printf("in handler ALKU\n");
#ifdef PC_UBUNTU
   if( e->type == SDL_MOUSEMOTION)
#else
   if( e->type == SDL_FINGERMOTION)
#endif
   {
#ifdef PC_UBUNTU
      SDL_GetMouseState( &b4x, &b4y );
#else
      b4y = e->tfinger.y * gScreenRect.h;
      b4x = e->tfinger.x * gScreenRect.w;
#endif
   //b4y -= 4;
   }
   else
   {
      if (Button1 != nullptr)
         Button1->handleEvent( e );
      if (Button2 != nullptr)
         Button2->handleEvent( e );
      if (Button3 != nullptr)
         Button3->handleEvent( e );
      if (Button4 != nullptr)
         Button4->handleEvent( e );
      b4x = Button4_X;
      b4y = Button4_Y;
   }

   if (Button1 != nullptr)
      Button1->render(Button1_X, Button1_Y);
   if (Button2 != nullptr)
      Button2->render(Button2_X, Button2_Y);
   if (Button3 != nullptr)
      Button3->render(Button3_X, Button3_Y);
   if (Button4 != nullptr)
      Button4->render(b4x, b4y);

   return 0xffff;
}

void init_ALKU(void)
{
   Button1 = new LButton(Button1_X, Button1_Y, (char*)"B1", (char*)"but1.png");
   if (Button1 == nullptr)
      printf( "Failed to create new Button!\n" );
   Button2 = new LButton(Button2_X, Button2_Y, (char*)"B2", (char*)"but2.png");
   if (Button2 == nullptr)
      printf( "Failed to create new Button!\n" );
   Button3 = new LButton(Button3_X, Button3_Y, (char*)"B3", (char*)"but3.png");
   if (Button3 == nullptr)
      printf( "Failed to create new Button!\n" );

//   Button4 = new LButton(300, b4y=460, (char*)"B4", (char*)"but4.png");
   b4x = Button4_X;
   b4y = Button4_Y;
   Button4 = new LButton(Button4_X, Button4_Y, (char*)"B4", (char*)"but4.png");
   if (Button4 == nullptr)
      printf( "Failed to create new Button!\n" );
}

void show_ALKU(int show_hide)
{
   if (show_hide == 0)
   {
      return;
   }
}
