/* 
   omat_ohjelmat.cpp
   -ruudulla omat ohjelma kansiot. Jossa valmiiksi luodut kansiot/kategoriat 12kpl (kansio_nimet_tekstit)
   -lis‰ksi "Lis‰‰ uusi kansio" menu-button
   -TAKAISIN + UUSI OHJELMA buttons
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
#include "cook_prog.h"

extern int sec_ctr;

int handle_omat_ohjelmat_button_press(int but);

static int menu_before;

//12 menu-buttonia + "Lis‰‰ uusi kansio" menu-button
#define KANSIO1_BUTTON_IDX         0
#define KANSIO12_BUTTON_IDX        11
#define LISAA_UUSI_KANS_BUTTON_IDX 12
#define TAKAISIN_BUTTON_IDX        13
#define UUSI_OHJELMA_BUTTON_IDX    14
#define NUM_BUTTON_IDX             15

#define TAKAISIN_BUTTON_L 72
#define TAKAISIN_BUTTON_T 1029 //Sama positio kuin ok_cancel2
#define TAKAISIN_BUTTON_H 82
#define TAKAISIN_BUTTON_W 300

#define UUSI_OHJELMA_BUTTON_L 429
#define UUSI_OHJELMA_BUTTON_T 1029 //Sama positio kuin ok_cancel2
#define UUSI_OHJELMA_BUTTON_H 82
#define UUSI_OHJELMA_BUTTON_W 300

const button_grp_s oobg[NUM_BUTTON_IDX] = {
   {MENU_BUTTONSMALL1_L, MENU_BUTTONSMALL1_T, MENU_BUTTONSMALL_W, MENU_BUTTONSMALL_H, 0,
         KANSIO_NIMET_TXT_KEITOT, TXT_GROUP_KANSIO_NIMET, 10, 
         IMAGE_MENU_BUTTON_SMALL_TAUSTA, IMAGE_MENU_BUTTON_SMALL_POHJASSA_TAUSTA, 0},

   {MENU_BUTTONSMALL2_L, MENU_BUTTONSMALL2_T, MENU_BUTTONSMALL_W, MENU_BUTTONSMALL_H, 1,
         KANSIO_NIMET_TXT_LAMPIMAT_KASTIKKEET, TXT_GROUP_KANSIO_NIMET, 10, 
         IMAGE_MENU_BUTTON_SMALL_TAUSTA, IMAGE_MENU_BUTTON_SMALL_POHJASSA_TAUSTA, 0},

   {MENU_BUTTONSMALL3_L, MENU_BUTTONSMALL3_T, MENU_BUTTONSMALL_W, MENU_BUTTONSMALL_H, 2,
         KANSIO_NIMET_TXT_KYLMAT_KASTIKKEET, TXT_GROUP_KANSIO_NIMET, 10, 
         IMAGE_MENU_BUTTON_SMALL_TAUSTA, IMAGE_MENU_BUTTON_SMALL_POHJASSA_TAUSTA, 0},

   {MENU_BUTTONSMALL4_L, MENU_BUTTONSMALL4_T, MENU_BUTTONSMALL_W, MENU_BUTTONSMALL_H, 3,
         KANSIO_NIMET_TXT_PUUROT, TXT_GROUP_KANSIO_NIMET, 10, 
         IMAGE_MENU_BUTTON_SMALL_TAUSTA, IMAGE_MENU_BUTTON_SMALL_POHJASSA_TAUSTA, 0},

   {MENU_BUTTONSMALL5_L, MENU_BUTTONSMALL5_T, MENU_BUTTONSMALL_W, MENU_BUTTONSMALL_H, 4,
         KANSIO_NIMET_TXT_RIISI, TXT_GROUP_KANSIO_NIMET, 10, 
         IMAGE_MENU_BUTTON_SMALL_TAUSTA, IMAGE_MENU_BUTTON_SMALL_POHJASSA_TAUSTA, 0},

   {MENU_BUTTONSMALL6_L, MENU_BUTTONSMALL6_T, MENU_BUTTONSMALL_W, MENU_BUTTONSMALL_H, 5,
         KANSIO_NIMET_TXT_PASTA, TXT_GROUP_KANSIO_NIMET, 10, 
         IMAGE_MENU_BUTTON_SMALL_TAUSTA, IMAGE_MENU_BUTTON_SMALL_POHJASSA_TAUSTA, 0},

   {MENU_BUTTONSMALL7_L, MENU_BUTTONSMALL7_T, MENU_BUTTONSMALL_W, MENU_BUTTONSMALL_H, 6,
         KANSIO_NIMET_TXT_PATARUOAT, TXT_GROUP_KANSIO_NIMET, 10, 
         IMAGE_MENU_BUTTON_SMALL_TAUSTA, IMAGE_MENU_BUTTON_SMALL_POHJASSA_TAUSTA, 0},

   {MENU_BUTTONSMALL8_L, MENU_BUTTONSMALL8_T, MENU_BUTTONSMALL_W, MENU_BUTTONSMALL_H, 7,
         KANSIO_NIMET_TXT_PERUNA, TXT_GROUP_KANSIO_NIMET, 10, 
         IMAGE_MENU_BUTTON_SMALL_TAUSTA, IMAGE_MENU_BUTTON_SMALL_POHJASSA_TAUSTA, 0},

   {MENU_BUTTONSMALL9_L, MENU_BUTTONSMALL9_T, MENU_BUTTONSMALL_W, MENU_BUTTONSMALL_H, 8,
         KANSIO_NIMET_TXT_JALKIRUOAT, TXT_GROUP_KANSIO_NIMET, 10, 
         IMAGE_MENU_BUTTON_SMALL_TAUSTA, IMAGE_MENU_BUTTON_SMALL_POHJASSA_TAUSTA, 0},

   {MENU_BUTTONSMALL10_L, MENU_BUTTONSMALL10_T, MENU_BUTTONSMALL_W, MENU_BUTTONSMALL_H, 9,
         KANSIO_NIMET_TXT_VANUKKAAT, TXT_GROUP_KANSIO_NIMET, 10, 
         IMAGE_MENU_BUTTON_SMALL_TAUSTA, IMAGE_MENU_BUTTON_SMALL_POHJASSA_TAUSTA, 0},

   {MENU_BUTTONSMALL11_L, MENU_BUTTONSMALL11_T, MENU_BUTTONSMALL_W, MENU_BUTTONSMALL_H, 10,
         KANSIO_NIMET_TXT_SEOKSET, TXT_GROUP_KANSIO_NIMET, 10, 
         IMAGE_MENU_BUTTON_SMALL_TAUSTA, IMAGE_MENU_BUTTON_SMALL_POHJASSA_TAUSTA, 0},

   {MENU_BUTTONSMALL12_L, MENU_BUTTONSMALL12_T, MENU_BUTTONSMALL_W, MENU_BUTTONSMALL_H, 11,
         KANSIO_NIMET_TXT_PESUOHJELMAT, TXT_GROUP_KANSIO_NIMET, 10, 
         IMAGE_MENU_BUTTON_SMALL_TAUSTA, IMAGE_MENU_BUTTON_SMALL_POHJASSA_TAUSTA, 0},

   {MENU_BUTTONSMALL15_L, MENU_BUTTONSMALL15_T, MENU_BUTTONSMALL_W, MENU_BUTTONSMALL_H, 12,
         YLEIS_TXT_LISAA_UUSI_KANSIO, TXT_GROUP_YLEIS, 10, 
         IMAGE_MENU_BUTTON_SMALL_TAUSTA, IMAGE_MENU_BUTTON_SMALL_POHJASSA_TAUSTA, 0},

   {TAKAISIN_BUTTON_L, TAKAISIN_BUTTON_T, TAKAISIN_BUTTON_W, TAKAISIN_BUTTON_H, 13,
         YLEIS_TXT_TAKAISIN, TXT_GROUP_YLEIS, 10, 
         IMAGE_VALKOINEN_VIHREAT_REUNAT, IMAGE_VALKOINEN_VIHREAT_REUNAT_POHJASSA, 0},

   {UUSI_OHJELMA_BUTTON_L, UUSI_OHJELMA_BUTTON_T,UUSI_OHJELMA_BUTTON_W, UUSI_OHJELMA_BUTTON_H, 14, 
         YLEIS_TXT_UUSI_OHJELMA, TXT_GROUP_YLEIS, 10, 
         IMAGE_VIHREA_LYHYT, IMAGE_VIHREA_LYHYT_POHJASSA, 0},
};

//Laitetaan tekstit ja icon jos buttonissa on semmoinen, background piirret‰‰n draw-funktiossa, joko painettu/ei painettu.
void set_omat_ohjelmat_buttons(void)
{
   SDL_Color txt_color_white = { 0xff, 0xff, 0xff, 0xff };
   SDL_Color colorgreen = { 0x3D, 0xAF, 0, 0 };

   for (int i=0; i< NUM_BUTTON_IDX; i++) //12 valmis-kansiota
   {
      Button_group_add_button(i, oobg[i].x, oobg[i].y, oobg[i].Width, oobg[i].Height, oobg[i].txt_offset);
      if (i==TAKAISIN_BUTTON_IDX)
         Button_group_setText(i, oobg[i].txt_idx, oobg[i].txt_group, colorgreen, font_norm_regular_35px);
      else if (i==UUSI_OHJELMA_BUTTON_IDX)
         Button_group_setText(i, oobg[i].txt_idx, oobg[i].txt_group, txt_color_white, font_norm_regular_35px);
      else
         Button_group_setText(i, oobg[i].txt_idx, oobg[i].txt_group, txt_color_white, font_norm_bold_20px);
      //if (oobg[i].icon_img_idx)
      //   Button_group_add_images(i, 0, image_ptrs[oobg[i].icon_img_idx]);
   }
}

#define HARMAA_TAUSTA_Y2 1005

void draw_OMAT_OHJELMAT(int param1)
{
   boxRGBA(gRenderer, 0, 70, 799, HARMAA_TAUSTA_Y2, 0x58, 0x5B, 0x60, 0xFF);
   if (alavalikko_naytolla)
      show_ala_kahva();
   else
      show_alempi_kahva();

   for (int i=0; i< NUM_BUTTON_IDX; i++)
   {
      if (button_pressed==i+1)
         Button_groups[i].background_image = image_ptrs[ oobg[i].button_pressed_back_ground_img_idx ];
      else
         Button_groups[i].background_image = image_ptrs[ oobg[i].back_ground_img_idx ];
      Button_group_render(i);
      Button_groups[i].button_disabled = ylavalikko_naytolla;
   }

   if (param1 == 0)
   {
      show_banner(NUM_BUTTON_IDX);
#if 0
      if (alavalikko_naytolla)
         nayta_alavalikko(NUM_BUTTON_IDX+1, button_pressed);
      else if (ylavalikko_naytolla)
         nayta_ylavalikko(NUM_BUTTON_IDX+1);
#endif
   }
}

int handler_OMAT_OHJELMAT(int x, int y,  SDL_Event *e)
{
   if( e->type == SDL_MOUSEMOTION)
   {
#if 0
      if (!wait_finger_up && check_for_alavalikko(x, y, NUM_BUTTON_IDX+1))
         wait_finger_up = 1;
      draw_OMAT_OHJELMAT(0);
#endif
      ala_painike_painettu = button_pressed = 0;
   }
   else
   {
      if(e->type == SDL_MOUSEBUTTONDOWN)
      {
         if ((x==-1) && (y==-1))
            set_omat_ohjelmat_buttons(); //Tuli kansi auki: p‰ivitet‰‰n kaikki"
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
            menu = handle_omat_ohjelmat_button_press(--button_pressed);
            if (menu != 0xFFFF)
               return menu;
            button_pressed = 0;
         }
         wait_finger_up = 0;
      }
      draw_OMAT_OHJELMAT(0);
   }
   return 0xffff;
}

void init_OMAT_OHJELMAT(void)
{
}

void show_OMAT_OHJELMAT(int show_hide)
{
   sec_ctr = 0;
   if (show_hide == 0)
   {
      return;
   }
   if (menu_to_return != ID_OMAT_OHJELMAT)
   {
      menu_before = menu_to_return;
   }
   button_pressed = 0;
   pyyhk_enabled = 0;
   alavalikko_naytolla = 0;
   esta_alavalikko_naytolle = 1;
   Button_group_empty();
   set_omat_ohjelmat_buttons();
   draw_OMAT_OHJELMAT(0);
}

int handle_omat_ohjelmat_button_press(int but)
{
   if ((but >= KANSIO1_BUTTON_IDX) && (but <= KANSIO1_BUTTON_IDX+11))
   {
      //printf("KANsio %s valittu\n", (char*)kansio_nimet_tekstit[but - KANSIO1_BUTTON_IDX]);
      //-Kansion oikea nimi (esim "VALMIS1") on kirjoitettu -> cook_progs.cpp/tmp_kansion_nimi[MAKS_LEN];
      //-tmp_header.Oma_kansio sis‰lt‰‰ indeksin valittun kansioon (kts. cook_prog.h)
      //-N‰ytˆll‰ n‰ytet‰‰n kansion kielivalinnan mukaisesti esim "KEITOT" -> (char*)kansio_nimet_tekstit[tmp_header.Oma_kansio])
      tmp_header.Oma_kansio = but - KANSIO1_BUTTON_IDX;
      set_prog_header_kansion_nimi((char*)valmis_kansio_nimet[tmp_header.Oma_kansio], tmp_header.Oma_kansio);
      menu_to_return = ID_OMAT_OHJELMAT;
      return ID_LISTAA_KANSIO;
   }
   switch (but)
   {
      case LISAA_UUSI_KANS_BUTTON_IDX:
      break;

      case TAKAISIN_BUTTON_IDX:
         return ID_KOTINAKYMA;
      break;

      case UUSI_OHJELMA_BUTTON_IDX:
         return ID_UUSI_OHJELMA;
      break;
   }
   return 0xFFFF;
}
