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

#define BUTTON_ALUE_X 33
#define BUTTON_ALUE_Y 488
#define BUTTON_ALUE_W 734
#define BUTTON_ALUE_H 286

#define TAKAISIN_BUTTON_X 72
#define TAKAISIN_BUTTON_Y 654

#define OK_BUTTON_X 429
#define OK_BUTTON_Y 654

#define BUTTONS_W 300
#define BUTTONS_H 82

#define NUM_BUTTON_IDX 2

#define HALUATKO_LOPETTAA_OHJELMAN_X 81
#define HALUATKO_LOPETTAA_OHJELMAN_Y 538

//   YLEIS_TXT_HALUATKO_LOPETTAA_OHJELMAN,
//   YLEIS_TXT_TAKAISIN,
//   YLEIS_TXT_OK,

void set_ok_cancel_buttons(void) //cancel=0, OK=1
{
   SDL_Color colorwhite = { 0xff, 0xff, 0xff, 0xff };
   SDL_Color colorgreen = { 0x3D, 0xAF, 0, 0 };

   Button_group_add_button(0, TAKAISIN_BUTTON_X, TAKAISIN_BUTTON_Y, BUTTONS_W, BUTTONS_H, 10);
   Button_group_setText(0, YLEIS_TXT_TAKAISIN, TXT_GROUP_YLEIS, colorgreen, font_norm_regular_35px);
   Button_group_add_images(0, image_ptrs[IMAGE_VALKOINEN_VIHREAT_REUNAT], nullptr);

   Button_group_add_button(1, OK_BUTTON_X, OK_BUTTON_Y, BUTTONS_W, BUTTONS_H, 10);
   Button_group_setText(1, YLEIS_TXT_OK, TXT_GROUP_YLEIS, colorwhite, font_norm_regular_35px);
   Button_group_add_images(1, image_ptrs[IMAGE_PUNAINEN], nullptr);
}

static void update_this(void)
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

   render_text((char*)yleis_tekstit[YLEIS_TXT_HALUATKO_LOPETTAA_OHJELMAN], HALUATKO_LOPETTAA_OHJELMAN_X, HALUATKO_LOPETTAA_OHJELMAN_Y, (SDL_Color *)&colorblack, font_norm_regular_40px);

   show_banner(NUM_BUTTON_IDX);
   if (alavalikko_naytolla)
      nayta_alavalikko(NUM_BUTTON_IDX+1, button_pressed);
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
            if (button_pressed == 1)
            {
               if (menu_to_return < NUM_MENU_IDS)
                  return menu_to_return;
               else
                  return ID_KOTINAKYMA;
            }
            else
            {
               if (menu_to_return == ID_KYPSENNYS)
                  cooking_is_ready();
               return ID_VALMIS; //EI välttämättä oikea paluupaikka!
            }
            button_pressed = 0;
         }
      }
      update_this();
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
   set_ok_cancel_buttons();
   update_this();
}
