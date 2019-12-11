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

#define Tausta1_X 0
#define Tausta1_Y 0

#define Tausta2_X 0
#define Tausta2_Y 384

#define INFO_TEXT_MAX_LEN 30

LButton *DEMO_tausta1;
LButton *DEMO_tausta2;

int info_button_minutes;
char info_button_text_row1[INFO_TEXT_MAX_LEN + 1];
char info_button_text_row2[INFO_TEXT_MAX_LEN + 1];
char info_button_time_row1[20];
char info_button_time_row2[20];

int update_info_button_time(void)
{
   struct tm curr_time;
   time_t mynow;
	time(&mynow);
	curr_time = *localtime(&mynow);

   if (1) //info_button_minutes != curr_time.tm_min)
   {
      //printf("AIKA=%02d.%02d.%d %02d:%02d\n",curr_time.tm_mday, curr_time.tm_mon+1, curr_time.tm_year+1900, curr_time.tm_hour, curr_time.tm_min);	
   	sprintf(info_button_time_row1, "%02d.%02d.", curr_time.tm_mday, curr_time.tm_mon+1);	
	   sprintf(info_button_time_row2, " %02d:%02d", curr_time.tm_hour, curr_time.tm_min);	
      strcat(info_button_time_row1, info_button_time_row2);
      sprintf(info_button_time_row2, ":%02d", curr_time.tm_sec);	
      strcat(info_button_time_row1, info_button_time_row2);
      info_button_minutes = curr_time.tm_min;
      return 1;
   }
   else
      return 0;
}

void enter_DEMO_pages(void)
{
}

int handler_DEMO(int x, int y,  SDL_Event *e)
{
//   printf("in handler ALKU\n");
#ifdef PC_UBUNTU
   if( e->type == SDL_MOUSEMOTION)
#else
   if( e->type == SDL_FINGERMOTION)
#endif
   {
   }
   else //Not motion
   {
      SDL_Color colorw = { 255, 255, 255, 0 };
      update_info_button_time();
      if (DEMO_tausta1 != nullptr)
      {
         DEMO_tausta1->setText(info_button_time_row1, colorw, VeryBigFont88);
      }
      if (DEMO_tausta2 != nullptr)
      {
         DEMO_tausta2->setText("Keitto on valmis!", colorw, BigFont44);
      }
   }

   if (DEMO_tausta1 != nullptr)
      DEMO_tausta1->render(Tausta1_X, Tausta1_Y);
   if (DEMO_tausta2 != nullptr)
      DEMO_tausta2->render(Tausta2_X, Tausta2_Y);

   return 0xffff;
}

void init_DEMO(void)
{
   DEMO_tausta1 = new LButton(Tausta1_X, Tausta1_Y, nullptr, (char*)"tausta1_1024x384.png", BUTTON_TYPE_CLOCK);
   if (DEMO_tausta1 == nullptr)
      printf( "Failed to create new Button!\n" );
   else
   {
      DEMO_tausta1->set_size(1024, 384);
   }
   DEMO_tausta2 = new LButton(Tausta2_X, Tausta2_Y, nullptr, (char*)"tausta2_1024x384.png", BUTTON_TYPE_CLOCK);
   if (DEMO_tausta2 == nullptr)
      printf( "Failed to create new Button!\n" );
   else
   {
      DEMO_tausta2->set_size(1024, 384);
   }

}

void show_DEMO(int show_hide)
{
   if (show_hide == 0)
   {
      return;
   }
}
