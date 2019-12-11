/* 
   nayta_ohj_vaiheeti.cpp
   Ohjelma luettu kansiosta -> näytetään luetun ohjelman vaiheet

   valinnat --> set_ok_cancel_buttons()
   "TAKAISIN", x=71, y=1029 IMAGE_VALKOINEN_VIHREAT_REUNAT
   "ALOITA", x=428, y=1029, IMAGE_VIHREA_LYHYT

   Ohjelman nimi ylhäällä keskellä, y=15 font=Norm, regular 40
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

extern int current_dialog_id;

//apufunktiot -> vaiheiden_luonti.cpp

#define NUM_BUTTON_IDX 3 //0=cancel, 1=ALOITA, 3=Header

static int menu_before;

void draw_NAYTA_OHJ_VAIHEET(int param1)
{
   SDL_Color colorwhite = { 0xff, 0xff, 0xff, 0xff };
   set_ok_cancel_buttons2(IMAGE_VIHREA_LYHYT);
   Button_group_setText(1, YLEIS_TXT_ALOITA, TXT_GROUP_YLEIS, colorwhite, font_norm_regular_35px); //Muutetaan OK->ALOITA

   show_ohj_nimi();
   show_ohj_header(2); //Button=2

   uuden_Vaiheen_teko = 0;
   show_nayta_vaiheet_buttons(NUM_BUTTON_IDX);

   for (int i=0; i< NUM_BUTTON_IDX + tmp_header.Vaihe_lkm; i++)
      Button_group_render(i);

   show_header_teksti();
   uuden_Vaiheen_teko = 1; //Asetetaan "show_nayta_vaiheet_buttons_icons" varten
   show_nayta_vaiheet_buttons_icons();
   uuden_Vaiheen_teko = 0;

   show_banner(NUM_BUTTON_IDX + tmp_header.Vaihe_lkm);
}

int handler_NAYTA_OHJ_VAIHEET(int x, int y,  SDL_Event *e)
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
         if (button_pressed) //0=cancel, 1=ALOITA, 2=PRog header, 3...XX vaiheet
         {
            --button_pressed;
            if (button_pressed == 0) //cancel
            {
               if (menu_before < NUM_MENU_IDS)
                  return menu_before;
               else
                  return ID_KOTINAKYMA;
            }
            else if (button_pressed == 1) //ALOITA
            {
               return ID_SYOTA_ALOITA_TIEDOT;
            }
            button_pressed = 0;
         }
      }
      draw_NAYTA_OHJ_VAIHEET(0);
   }
   return 0xffff;
}

void init_NAYTA_OHJ_VAIHEET(void)
{
}

void show_NAYTA_OHJ_VAIHEET(int show_hide)
{
   if (show_hide == 0)
   {
      return;
   }
   pyyhk_enabled = 0;
   button_pressed = 0;
   Button_group_empty();
   alavalikko_naytolla = 0;
   menu_before = menu_to_return;
//   printf("in show_NAYTA_OHJ_VAIHEET..\n");
   draw_NAYTA_OHJ_VAIHEET(0);
}
