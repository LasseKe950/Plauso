/* 
   ok_cancel.cpp
   Näytetään lappu ja valinnat "OK" ja "TAKAISIN"
   */
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <sys/time.h>
#include <sys/times.h>

#include "SDL2_gfxPrimitives.h"

#include <stdio.h>
#include <string>
#include <stdlib.h>

#include "menu_ids.h"
#include "handler.h"

#include "button_defs.h"
#include "kypsennys.h"
#include "tekstit.h"
#include "var.h"
#include "button_group.h"
#include "externs.h"
#include "kettle_functions.h"

#define BUTTON_ALUE_X 33
#define BUTTON_ALUE_Y 488
#define BUTTON_ALUE_W 734
#define BUTTON_ALUE_H 286

#define TAKAISIN_BUTTON_X 72
#define TAKAISIN_BUTTON_Y 654

#define OK_BUTTON_X 429
#define OK_BUTTON_Y 654

#define TAKAISIN_BUTTON2_X 71
#define TAKAISIN_BUTTON2_Y 1029

#define OK_BUTTON2_X 428
#define OK_BUTTON2_Y 1029

#define BUTTONS_W 300
#define BUTTONS_H 82

#define NUM_BUTTON_IDX 2

#define KYSYMYKSEN_TEKSTIN_X 81
#define KYSYMYKSEN_TEKSTIN_Y 538

extern void himmenna_lisaa(int par);
extern void himmenna_nykyinen(int par);
extern int power_off_set;

int ok_cancel_txt_group, ok_cancel_txt_index;

void set_ok_cancel_text(int txt_group, int txt_index)
{
   if ((txt_group == TXT_GROUP_YLEIS) && (txt_index < NUM_YLEIS_TEKSTIT))
   {
      ok_cancel_txt_group = txt_group;
      ok_cancel_txt_index = txt_index;
   }
}

void set_ok_cancel_buttons(int ok_tausta) //cancel=0, OK=1
{
   SDL_Color colorwhite = { 0xff, 0xff, 0xff, 0xff };
   SDL_Color colorgreen = { 0x3D, 0xAF, 0, 0 };

   Button_group_add_button(0, TAKAISIN_BUTTON_X, TAKAISIN_BUTTON_Y, BUTTONS_W, BUTTONS_H, 10);
   Button_group_setText(0, YLEIS_TXT_TAKAISIN, TXT_GROUP_YLEIS, colorgreen, font_norm_regular_35px);
   Button_group_add_images(0, image_ptrs[IMAGE_VALKOINEN_VIHREAT_REUNAT], nullptr);

   Button_group_add_button(1, OK_BUTTON_X, OK_BUTTON_Y, BUTTONS_W, BUTTONS_H, 10);
   Button_group_setText(1, YLEIS_TXT_OK, TXT_GROUP_YLEIS, colorwhite, font_norm_regular_35px);
   if (ok_tausta < NUM_LOADED_IMAGES)
      Button_group_add_images(1, image_ptrs[ok_tausta], nullptr);
}

void set_ok_cancel_buttons2(int ok_tausta) //cancel=0, OK=1. Buttonit ruudun alaosaan
{
   SDL_Color colorwhite = { 0xff, 0xff, 0xff, 0xff };
   SDL_Color colorgreen = { 0x3D, 0xAF, 0, 0 };

   Button_group_add_button(0, TAKAISIN_BUTTON2_X, TAKAISIN_BUTTON2_Y, BUTTONS_W, BUTTONS_H, 10);
   Button_group_setText(0, YLEIS_TXT_TAKAISIN, TXT_GROUP_YLEIS, colorgreen, font_norm_regular_35px);
   Button_group_add_images(0, image_ptrs[IMAGE_VALKOINEN_VIHREAT_REUNAT], nullptr);

   Button_group_add_button(1, OK_BUTTON2_X, OK_BUTTON2_Y, BUTTONS_W, BUTTONS_H, 10); //TEXT y-offset = 10
   Button_group_setText(1, YLEIS_TXT_OK, TXT_GROUP_YLEIS, colorwhite, font_norm_regular_35px);
   if (ok_tausta < NUM_LOADED_IMAGES)
      Button_group_add_images(1, image_ptrs[ok_tausta], nullptr);
}

void draw_OK_CANCEL(int param1)
{
   SDL_Color colorblack = { 0x0, 0x0, 0x0, 0x0 };
   //button alue
//   roundedBoxRGBA(gRenderer, BUTTON_ALUE_X, BUTTON_ALUE_Y, (BUTTON_ALUE_X+BUTTON_ALUE_W), (BUTTON_ALUE_Y+BUTTON_ALUE_H), 0xf4, 0xf4, 0xf4, 0xFF); //button alue alavalikkoon asti, alavalikko y=1022
   //last par: color The color value of the rectangle to draw (0xRRGGBBAA).
   Uint32 c;
   c = 0xFF<<24 | (0xF4<<16) | (0xF4<<8) | (0xF4); //bgr
   roundedBoxColor(gRenderer, //ISO ALUE
                   BUTTON_ALUE_X, BUTTON_ALUE_Y, //x1 + y1
                    (BUTTON_ALUE_X+BUTTON_ALUE_W),  (BUTTON_ALUE_Y+BUTTON_ALUE_H), //x2 + y2
                   64, //border radius
                   c);

   for (int i=0; i< NUM_BUTTON_IDX; i++)
   {
      Button_group_render(i);
   }

   if ((ok_cancel_txt_group == TXT_GROUP_YLEIS) && (ok_cancel_txt_index < NUM_YLEIS_TEKSTIT))
      render_text((char*)yleis_tekstit[ok_cancel_txt_index], KYSYMYKSEN_TEKSTIN_X, KYSYMYKSEN_TEKSTIN_Y, (SDL_Color *)&colorblack, font_norm_regular_40px);

   if (param1 == 0)
   {
      show_banner(NUM_BUTTON_IDX);
      if (alavalikko_naytolla)
         nayta_alavalikko(NUM_BUTTON_IDX+1, button_pressed);
   }
}

int handler_OK_CANCEL(int x, int y,  SDL_Event *e)
{
   if( e->type == SDL_MOUSEMOTION)
   {
      button_pressed = 0;
   }
   else
   {
      if(e->type == SDL_MOUSEBUTTONDOWN)
      {
         int b = Button_group_check_for_key_press(x, y);
         if (b == CODE_DIETA_LOGO)
            return ID_KOTINAKYMA;
         button_pressed = b;
      }
      else if(e->type == SDL_MOUSEBUTTONUP)
      {
         if (button_pressed)
         {
            if (button_pressed == 1) //cancel=1, OK=2
            {
               if (menu_to_return < NUM_MENU_IDS)
                  return menu_to_return;
               else
                  return ID_KOTINAKYMA;
            }
            else
            {
#if 0
               if (shutdown_wanted)
               {
                  system("poweroff");
               }
               else
               {
                  if (menu_to_return == ID_KYPSENNYS)
                     cooking_is_ready(1); //1=hold temperature
                  return ID_VALMIS; //EI välttämättä oikea paluupaikka!
               }
#else
   
               if (ok_cancel_ok_func_idx < NUM_OK_CANCEL_OK_FUNCTIONS)
                  ok_funcs[ok_cancel_ok_func_idx].ok_pressed(0);

               ok_cancel_ok_func_idx = 0; //nollataan defaultiksi=no_ok_func
               if (power_off_set)
                  return 0xFFFF;

               if (menu_to_return == ID_KYPSENNYS)
               {
                  cooking_is_ready(1); //1=hold temperature
                  return ID_VALMIS;
               }
               else if (menu_to_return == ID_JAAHDYTYS)
               {
                  chilling_is_ready(1); //1=hold temperature
                  return ID_VALMIS;
               }
               else if (menu_to_return < NUM_MENU_IDS)
                  return menu_to_return;
               else
                  return ID_KOTINAKYMA;
#endif
            }
            button_pressed = 0;
         }
      }
      draw_OK_CANCEL(0);

      if (power_off_set)
         himmenna_lisaa(0);
   }
   return 0xffff;
}

void init_OK_CANCEL(void)
{
}

void show_OK_CANCEL(int show_hide)
{
   if (show_hide == 0)
   {
      return;
   }
   pyyhk_enabled = 0;
   button_pressed = 0;
   Button_group_empty();

   alavalikko_naytolla = 0;
   set_ok_cancel_buttons(IMAGE_PUNAINEN);
   draw_OK_CANCEL(0);
}

int ok_cancel_ok_func_idx;

void power_off_func(int par1)
{
   printf("TEST in power_off_func..\n");
   power_off_set = 1;
#if 0
   clear_all_kettle_functions();

   himmenna_nykyinen(0);
   //Update screen
   SDL_RenderPresent( gRenderer );

   system("poweroff &");
   while(1)
      SDL_Delay(1000);
#endif
}

void no_ok_func(int par1)
{
   printf("TEST in no_ok_func..\n");
}

void test2_func(int par1)
{
   printf("TEST in test2_func..\n");
}

void test3_func(int par1)
{
   printf("TEST in test3_func..\n");
}

void test4_func(int par1)
{
   printf("TEST in test4_func..\n");
}

const ok_cancel_OK_funcs_s ok_funcs[NUM_OK_CANCEL_OK_FUNCTIONS] =
{
   &no_ok_func, //default action, ok_cancel_ok_func_idx = 0 --> no_ok_func
   &test2_func,
   &test3_func,
   &test4_func,
   &power_off_func,
};

