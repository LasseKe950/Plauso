/* 
   ohjelmat.cpp
   -ruudulla "Omat ohjelmat/Suosikit/Autocook/Pesu" valintapainikkeet + "Takaisin" + "UUSI Ohjelma"
   */
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "SDL2_gfxPrimitives.h"

#include <stdio.h>
#include <string>
#include <stdlib.h>

#include <sys/time.h>
#include <sys/times.h>

#include "menu_ids.h"
#include "handler.h"

#include "button_defs.h"
#include "kypsennys.h"
#include "tekstit.h"
#include "var.h"
#include "image.h"
#include "button_group.h"
#include "externs.h"

extern int sec_ctr;

int handle_ohjelmat_button_press(int but);

static int my_button_idx;

//4 menu-buttonia + "Takaisin" + "UUSI Ohjelma"
#define OMAT_OHJELMAT_BUTTON_IDX 0
#define SUOSIKIT_BUTTON_IDX      1
#define AUTOCOOK_BUTTON_IDX      2
#define PESU_BUTTON_IDX          3
#define TAKAISIN_BUTTON_IDX      4
#define UUSI_OHJELMA_BUTTON_IDX  5
#define NUM_BUTTON_IDX           6

#define TAKAISIN_BUTTON_L 72
#define TAKAISIN_BUTTON_T 1029 //Sama positio kuin ok_cancel2
#define TAKAISIN_BUTTON_H 82
#define TAKAISIN_BUTTON_W 300

#define UUSI_OHJELMA_BUTTON_L 429
#define UUSI_OHJELMA_BUTTON_T 1029 //Sama positio kuin ok_cancel2
#define UUSI_OHJELMA_BUTTON_H 82
#define UUSI_OHJELMA_BUTTON_W 300

//button_grp_s m‰‰rittely butto_defs.h
const button_grp_s ohjelmat_Button_group[NUM_BUTTON_IDX] = {
   {MENU_BUTTON1_L, MENU_BUTTON1_T, MENU_BUTTON_W_H, MENU_BUTTON_W_H, 0, 
         YLEIS_TXT_OMAT_OHJELMAT, TXT_GROUP_YLEIS, 80, 
         IMAGE_MENU_BUTTON_TAUSTA, IMAGE_MENU_BUTTON_POHJASSA_TAUSTA, IMAGE_OMAT_OHJELMAT_111px},

   {MENU_BUTTON2_L, MENU_BUTTON2_T, MENU_BUTTON_W_H, MENU_BUTTON_W_H, 1, 
         YLEIS_TXT_SUOSIKIT, TXT_GROUP_YLEIS, 80, 
         IMAGE_MENU_BUTTON_TAUSTA, IMAGE_MENU_BUTTON_POHJASSA_TAUSTA, IMAGE_SUOSIKIT_111px},

   {MENU_BUTTON3_L, MENU_BUTTON3_T, MENU_BUTTON_W_H, MENU_BUTTON_W_H, 2, 
         YLEIS_TXT_AUTOCOOK, TXT_GROUP_YLEIS, 80, 
         IMAGE_MENU_BUTTON_TAUSTA, IMAGE_MENU_BUTTON_POHJASSA_TAUSTA, IMAGE_AUTOCOOK_111px},

   {MENU_BUTTON4_L, MENU_BUTTON4_T, MENU_BUTTON_W_H, MENU_BUTTON_W_H, 3, 
         YLEIS_TXT_PESU, TXT_GROUP_YLEIS, 80, 
         IMAGE_MENU_BUTTON_TAUSTA, IMAGE_MENU_BUTTON_POHJASSA_TAUSTA, IMAGE_PUHDISTUS_111px},

   {TAKAISIN_BUTTON_L, TAKAISIN_BUTTON_T, TAKAISIN_BUTTON_W, TAKAISIN_BUTTON_H, 4, 
         YLEIS_TXT_TAKAISIN, TXT_GROUP_YLEIS, 10, 
         IMAGE_VALKOINEN_VIHREAT_REUNAT, IMAGE_VALKOINEN_VIHREAT_REUNAT_POHJASSA, 0},

   {UUSI_OHJELMA_BUTTON_L, UUSI_OHJELMA_BUTTON_T,UUSI_OHJELMA_BUTTON_W, UUSI_OHJELMA_BUTTON_H, 5, 
         YLEIS_TXT_UUSI_OHJELMA, TXT_GROUP_YLEIS, 10, 
         IMAGE_VIHREA_LYHYT, IMAGE_VIHREA_LYHYT_POHJASSA, 0},
};

//Laitetaan tekstit ja icon jos buttonissa on semmoinen, background piirret‰‰n draw-funktiossa, joko painettu/ei painettu.
void set_ohjelmat_buttons(void)
{
   SDL_Color txt_color_white = { 0xff, 0xff, 0xff, 0xff };
   SDL_Color colorgreen = { 0x3D, 0xAF, 0, 0 };

   for (int i=0; i< NUM_BUTTON_IDX; i++)
   {
      Button_group_add_button(i, ohjelmat_Button_group[i].x, ohjelmat_Button_group[i].y, ohjelmat_Button_group[i].Width, ohjelmat_Button_group[i].Height, ohjelmat_Button_group[i].txt_offset);
      if (i==4)
         Button_group_setText(i, ohjelmat_Button_group[i].txt_idx, ohjelmat_Button_group[i].txt_group, colorgreen, font_norm_regular_35px);
      else if (i==5)
         Button_group_setText(i, ohjelmat_Button_group[i].txt_idx, ohjelmat_Button_group[i].txt_group, txt_color_white, font_norm_regular_35px);
      else
         Button_group_setText(i, ohjelmat_Button_group[i].txt_idx, ohjelmat_Button_group[i].txt_group, txt_color_white, font_norm_bold_20px);
      if (ohjelmat_Button_group[i].icon_img_idx)
      {
         Button_group_add_images(i, 0, image_ptrs[ohjelmat_Button_group[i].icon_img_idx]);
         Button_groups[i].icon_image_pos.x = 10; //Iconin paikka tarttee v‰h‰n hienos‰‰tˆ‰
      }
   }
}

#define HARMAA_TAUSTA_Y2 1005

void draw_OHJELMAT(int param1)
{
   boxRGBA(gRenderer, 0, 70, 799, HARMAA_TAUSTA_Y2, 0x58, 0x5B, 0x60, 0xFF);
   my_button_idx = NUM_BUTTON_IDX;
   if (alavalikko_naytolla)
   {
      show_ala_kahva();
//      my_button_idx -= 2; //Ei n‰ytet‰ "TAKAISIN"/"UUSI OHJELMA"
   }
   else
      show_alempi_kahva();

   for (int i=0; i< my_button_idx; i++)
   {
      if (button_pressed==i+1)
         Button_groups[i].background_image = image_ptrs[ ohjelmat_Button_group[i].button_pressed_back_ground_img_idx ];
      else
         Button_groups[i].background_image = image_ptrs[ ohjelmat_Button_group[i].back_ground_img_idx ];
      Button_group_render(i);
      Button_groups[i].button_disabled = ylavalikko_naytolla;
   }

   if (param1 == 0)
   {
      show_banner(my_button_idx);
#if 0
      if (alavalikko_naytolla)
      {
         nayta_alavalikko(my_button_idx+1, button_pressed);
      }
      else if (ylavalikko_naytolla)
         nayta_ylavalikko(my_button_idx+1);
#endif
   }
}

int handler_OHJELMAT(int x, int y,  SDL_Event *e)
{
   if( e->type == SDL_MOUSEMOTION)
   {
#if 0
      if (!wait_finger_up && check_for_alavalikko(x, y, my_button_idx+1))
         wait_finger_up = 1;
      draw_OHJELMAT(0);
#endif
      ala_painike_painettu = button_pressed = 0;
   }
   else
   {
      if(e->type == SDL_MOUSEBUTTONDOWN)
      {
         if ((x==-1) && (y==-1))
            set_ohjelmat_buttons(); //Tuli kansi auki: p‰ivitet‰‰n kaikki"
         button_pressed = Button_group_check_for_key_press(x, y);
      }
      else if(e->type == SDL_MOUSEBUTTONUP)
      {
         if (button_pressed)
         {
            if (button_pressed == CODE_DIETA_LOGO)
               return ID_KOTINAKYMA;
            int menu;
            menu = check_ylavalikko_valinta(button_pressed);
            if (menu && (menu != 0xFFFF))
               return menu;
            menu = handle_ohjelmat_button_press(--button_pressed);
            if (menu != 0xFFFF)
               return menu;
            button_pressed = 0;
         }
         wait_finger_up = 0;
      }
      draw_OHJELMAT(0);
   }
   return 0xffff;
}

void init_OHJELMAT(void)
{
}

void show_OHJELMAT(int show_hide)
{
   sec_ctr = 0;
   if (show_hide == 0)
   {
      return;
   }
   button_pressed = 0;
   pyyhk_enabled = 0;
   alavalikko_naytolla = 0;
   esta_alavalikko_naytolle = 1;
   Button_group_empty();
   set_ohjelmat_buttons();
   draw_OHJELMAT(0);
}

int handle_ohjelmat_button_press(int but)
{
   switch (but)
   {
      case OMAT_OHJELMAT_BUTTON_IDX:
         menu_to_return = current_menu_id;
         return ID_OMAT_OHJELMAT;
      break;

      case SUOSIKIT_BUTTON_IDX:
      break;

      case AUTOCOOK_BUTTON_IDX:
      break;

      case PESU_BUTTON_IDX:
      break;

      case TAKAISIN_BUTTON_IDX:
         return ID_KOTINAKYMA;
      break;

      case UUSI_OHJELMA_BUTTON_IDX:
      break;
   }
   return 0xFFFF;
}
