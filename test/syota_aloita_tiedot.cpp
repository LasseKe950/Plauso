/* 
   syota_aloita_tiedot.cpp

   Ohjelma luettu ja vaiheitten listaus näkymästä painettu ALOITA -> annetaan "Tuotantomäärä" ja "Eräkoko"
   jonka jälkeen ohjelman voi aloittaa ALOITA painikkeesta.

   valinnat --> set_ok_cancel_buttons()
   "TAKAISIN", x=71, y=1029 IMAGE_VALKOINEN_VIHREAT_REUNAT
   "ALOITA", x=428, y=1029, IMAGE_VIHREA_LYHYT

   TALLENNA vai ALOITA?

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
#include "cook_prog.h"

#define BOX_ALUE_X 0
#define BOX_ALUE_Y 70
#define BOX_ALUE_W 800
#define BOX_ALUE_H (1005-70)

#define TEXT1_X 18
#define TEXT1_Y (157+15)

#define TEXT1_ALUE_X 18
#define TEXT1_ALUE_Y 207
#define TEXT1_ALUE_W 750
#define TEXT1_ALUE_H 82

#define TEXT2_X 18
#define TEXT2_Y (314+15)

#define TEXT2_ALUE_X 18
#define TEXT2_ALUE_Y 367
#define TEXT2_ALUE_W 750
#define TEXT2_ALUE_H 82

int tuotanto_maara, keitto_eran_nmo;

#define NUM_BUTTON_IDX 4 //0=cancel, 1=Ok, 2=Tuotantoerän koko-kenttä, 3=Eränmo-kenttä

static int menu_before;

void draw_SYOTA_ALOITA_TIEDOT(int param1)
{
   SDL_Color color = { 0x23, 0x23, 0x23, 0x0 };
   SDL_Color color2 = { 0xB4, 0xB4, 0xB4, 0x0 };
   set_ok_cancel_buttons2(IMAGE_VIHREA_LYHYT);

   show_ohj_nimi();
   //button alue
//   roundedBoxRGBA(gRenderer, BUTTON_ALUE_X, BUTTON_ALUE_Y, (BUTTON_ALUE_X+BUTTON_ALUE_W), (BUTTON_ALUE_Y+BUTTON_ALUE_H), 0xf4, 0xf4, 0xf4, 0xFF); //button alue alavalikkoon asti, alavalikko y=1022
   //last par: color The color value of the rectangle to draw (0xRRGGBBAA).
   Uint32 c;
   c = 0xFF<<24 | (0xF4<<16) | (0xF4<<8) | (0xF4); //bgr
   boxColor(gRenderer, //ISO ALUE
                   BOX_ALUE_X, BOX_ALUE_Y, //x1 + y1
                    (BOX_ALUE_X+BOX_ALUE_W),  (BOX_ALUE_Y+BOX_ALUE_H), //x2 + y2
                   c);

   c = 0xFF<<24 | (0xFF<<16) | (0xFF<<8) | (0xFF); //bgr
   roundedBoxColor(gRenderer, TEXT1_ALUE_X, TEXT1_ALUE_Y, (TEXT1_ALUE_X+TEXT1_ALUE_W),  (TEXT1_ALUE_Y+TEXT1_ALUE_H), 16, c);
   Button_group_add_button(2, TEXT1_ALUE_X, TEXT1_ALUE_Y, TEXT1_ALUE_W, TEXT1_ALUE_H);

   roundedBoxColor(gRenderer, TEXT2_ALUE_X, TEXT2_ALUE_Y, (TEXT2_ALUE_X+TEXT2_ALUE_W),  (TEXT2_ALUE_Y+TEXT2_ALUE_H), 16, c);
   Button_group_add_button(3, TEXT2_ALUE_X, TEXT2_ALUE_Y, TEXT2_ALUE_W, TEXT2_ALUE_H);

   for (int i=0; i< NUM_BUTTON_IDX; i++)
   {
      Button_group_render(i);
   }

   render_text((char*)yleis_tekstit[YLEIS_TXT_TUOTANTOMAARA_KG], TEXT1_X, TEXT1_Y, (SDL_Color *)&color, font_norm_regular_25px);
   render_text((char*)yleis_tekstit[YLEIS_TXT_KEITTOERAN_NMO], TEXT2_X, TEXT2_Y, (SDL_Color *)&color, font_norm_regular_25px);

   char buff[15];
   sprintf(buff, "%d", tuotanto_maara);
   if (tuotanto_maara > 0)
      render_text((char*)buff, TEXT1_ALUE_X, TEXT1_ALUE_Y+(TEXT1_ALUE_H/2), (SDL_Color *)&color2, font_norm_regular_25px, 0);

   sprintf(buff, "%d", keitto_eran_nmo);
   if (keitto_eran_nmo > 0)
      render_text((char*)buff, TEXT2_ALUE_X, TEXT2_ALUE_Y+(TEXT2_ALUE_H/2), (SDL_Color *)&color2, font_norm_regular_25px, 0);

   show_banner(NUM_BUTTON_IDX);
}

int handler_SYOTA_ALOITA_TIEDOT(int x, int y,  SDL_Event *e)
{
   if( e->type == SDL_MOUSEMOTION)
   {
      button_pressed = 0;
   }
   else
   {
      if(e->type == SDL_MOUSEBUTTONDOWN)
      {
         int b = 0;
         b = Button_group_check_for_key_press(x, y);
         if (b == CODE_DIETA_LOGO)
            return ID_KOTINAKYMA;
         button_pressed = b;
      }
      else if(e->type == SDL_MOUSEBUTTONUP)
      {
         if (button_pressed) //1=cancel, 2=Ok, 3=Tuotantoerän koko-kenttä, 4=Eränmo-kenttä
         {
            if (button_pressed == 1) //cancel
            {
               if (menu_before < NUM_MENU_IDS)
                  return menu_before;
               else
                  return ID_KOTINAKYMA;
            }
            else if (button_pressed == 2) //OK --> Monivaihe ohjelma aloitetaan
            {
               current_prog_step = 0;
               if (tmp_header.Vaihe_lkm)
               {
                  multi_step_prog_going = 1;
                  load_new_program_step(); //tmp_steps[current_prog_step] --> prepare_target_params
                  if (tmp_steps[0].Step_type == STEP_TYPE_CHILL_STEP)
                     return ID_JAAHDYTYS;
                  else
                     return ID_KYPSENNYS;
               }
            }
            else if (button_pressed == 3)
            {
               menu_to_return = ID_SYOTA_ALOITA_TIEDOT;
               show_dropdown = 1;
               return ID_NUMPAD_PIENI;
            }
            else if (button_pressed == 4)
            {
               menu_to_return = ID_SYOTA_ALOITA_TIEDOT;
               show_dropdown = 2;
               return ID_NUMPAD_PIENI;
            }
            button_pressed = 0;
         }
      }
      draw_SYOTA_ALOITA_TIEDOT(0);
   }
   return 0xffff;
}

void init_SYOTA_ALOITA_TIEDOT(void)
{
}

void show_SYOTA_ALOITA_TIEDOT(int show_hide)
{
   if (show_hide == 0)
   {
      return;
   }
   if ((menu_to_return != ID_SYOTA_ALOITA_TIEDOT) && (menu_to_return != ID_NUMPAD_PIENI))
   {
      menu_before = menu_to_return;
      tuotanto_maara =  keitto_eran_nmo = 0;
   }

   pyyhk_enabled = 0;
   button_pressed = 0;
   Button_group_empty();

   alavalikko_naytolla = 0;
   draw_SYOTA_ALOITA_TIEDOT(0);
}

void syota_aloita_tiedot_annettiin(void) //numpad_pieni kutsuu
{
   int i;

   numpad_string[4] = 0;
   if (sscanf(numpad_string, "%d", &i) != 1)
      return;

   if (show_dropdown == 1)
      tuotanto_maara = i;
   else if (show_dropdown == 2)
      keitto_eran_nmo = i;
}
