/* 
   prof_vaihto.cpp
   Käyttäjäprofiilin vaihto dialogi
   -iso rectangle x=33, y=199, w=734, h=575, borderadius=64, color=F4F4F4
   -"Käyttäjäprofiili" teksti, x=70, y=257, Simplon Norm, Regular 25px, colo3=232323
   -"Salasana" teksti, x=70, y=435, Simplon Norm, Regular 25px, colo3=232323
   -valinnan alasvetovalintapainike, x=69,y=308, w=424, h=82, br=16, color=FFFFFF (valkoinen)
   -salasana-kenttä, x=69, y=483, w=644, h=82, br=16, color=FFFFFF

   valinnat --> set_ok_cancel_buttons()
   "TAKAISIN", x=72, y=653 IMAGE_VALKOINEN_VIHREAT_REUNAT
   "OK", x=429, y=653, IMAGE_VIHREA_LYHYT

   */
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

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
#define BUTTON_ALUE_Y 199
#define BUTTON_ALUE_W 734
#define BUTTON_ALUE_H 575

#define NUM_BUTTON_IDX 2

#define TEXT1_ALUE_X 69
#define TEXT1_ALUE_Y 308
#define TEXT1_ALUE_W 424
#define TEXT1_ALUE_H 82

#define TEXT1_X 70
#define TEXT1_Y 257

#define TEXT2_ALUE_X 69
#define TEXT2_ALUE_Y 483
#define TEXT2_ALUE_W 644
#define TEXT2_ALUE_H 82

#define TEXT2_X 70
#define TEXT2_Y 435

extern void set_ok_cancel_buttons(int ok_tausta); //cancel=0, OK=1

void draw_PROF_VAIHTO(int param1)
{
   SDL_Color color = { 0x23, 0x23, 0x23, 0x0 };
   set_ok_cancel_buttons(IMAGE_VIHREA_LYHYT);
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

   c = 0xFF<<24 | (0xFF<<16) | (0xFF<<8) | (0xF4); //bgr
   roundedBoxColor(gRenderer,
                   TEXT1_ALUE_X, TEXT1_ALUE_Y, //x1 + y1
                    (TEXT1_ALUE_X+TEXT1_ALUE_W),  (TEXT1_ALUE_Y+TEXT1_ALUE_H), //x2 + y2
                   16, //border radius
                   c);

   roundedBoxColor(gRenderer,
                   TEXT2_ALUE_X, TEXT2_ALUE_Y, //x1 + y1
                    (TEXT2_ALUE_X+TEXT2_ALUE_W),  (TEXT2_ALUE_Y+TEXT2_ALUE_H), //x2 + y2
                   16, //border radius
                   c);

   for (int i=0; i< NUM_BUTTON_IDX; i++)
   {
      Button_group_render(i);
   }

   render_text((char*)yleis_tekstit[YLEIS_TXT_KAYTTAJAPROFIILI], TEXT1_X, TEXT1_Y, (SDL_Color *)&color, font_norm_regular_25px);
   render_text((char*)yleis_tekstit[YLEIS_TXT_SALASANA], TEXT2_X, TEXT2_Y, (SDL_Color *)&color, font_norm_regular_25px);
}

int handler_PROF_VAIHTO(int x, int y,  SDL_Event *e)
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
         if (button_pressed) //cancel=0, OK=1
         {
            if (button_pressed == 0)
            {
               if (menu_to_return < NUM_MENU_IDS)
                  return menu_to_return;
               else
                  return ID_KOTINAKYMA;
            }
            else //OK, jos salasana annettu ja se on ok niin vaihdetaan profiili, paluu --> menu_to_return
            {
               if (menu_to_return < NUM_MENU_IDS)
                  return menu_to_return;
               else
                  return ID_KOTINAKYMA;
            }
            button_pressed = 0;
         }
      }
      draw_PROF_VAIHTO(0);
   }
   return 0xffff;
}

void init_PROF_VAIHTO(void)
{
}

void show_PROF_VAIHTO(int show_hide)
{
   if (show_hide == 0)
   {
      return;
   }
   pyyhk_enabled = 0;
   button_pressed = 0;
   Button_group_empty();

   alavalikko_naytolla = 0;
   draw_PROF_VAIHTO(0);
}
