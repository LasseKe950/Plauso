/* 
   listaa_kansio.cpp

   Listaa valitun kansion ohjelmat (kansio valittu)

   Kansion nimi ylhäällä keskellä, y=15 font=Norm, regular 40

   -Kansion oikea nimi (esim "VALMIS1") on kirjoitettu -> cook_progs.cpp/tmp_kansion_nimi[MAKS_LEN];
   -tmp_header.Oma_kansio sisältää indeksin valittun kansioon (kts. cook_prog.h)
   -Näytöllä näytetään kansion kielivalinnan mukaisesti esim "KEITOT" -> (char*)kansio_nimet_tekstit[tmp_header.Oma_kansio])

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

void show_kansion_nimi(void);
void show_sort_buttons(int but_idx); //Näyttää "Käytetty eniten", "Käytetty viimeksi", "Aakkosjärjestys" valinnat
void show_ohjelma_buttons(int but_idx); //Näyttää ohjelmalistauksen (ohjelmien_lkm)

int ohjelmien_lkm, prev_ohjelmien_lkm;

//0=cancel, 1=Ok, 2="Käytetty eniten", 3="Käytetty viimeksi", 4="Aakkosjärjestys", 5...XX ohjelmat listassa
#define NUM_BUTTON_IDX 5

static int menu_before;

void draw_LISTAA_KANSIO(int param1)
{
   SDL_Color colorwhite = { 0xff, 0xff, 0xff, 0xff };
   set_ok_cancel_buttons2(IMAGE_VIHREA_LYHYT);
   Button_group_setText(1, YLEIS_TXT_UUSI_OHJELMA, TXT_GROUP_YLEIS, colorwhite, font_norm_regular_35px); //Muutetaan OK->UUSI OHJELMAT

   show_kansion_nimi();
   show_sort_buttons(2); //Näyttää "Käytetty eniten", "Käytetty viimeksi", "Aakkosjärjestys" valinnat
   show_ohjelma_buttons(NUM_BUTTON_IDX);

   for (int i=0; i< NUM_BUTTON_IDX + ohjelmien_lkm; i++)
         Button_group_render(i);

   show_banner(NUM_BUTTON_IDX + ohjelmien_lkm);
}

int handler_LISTAA_KANSIO(int x, int y,  SDL_Event *e)
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
         if (button_pressed) //0=cancel, 1=UUSI OHJ, 2="Käytetty eniten", 3="Käytetty viimeksi", 4="Aakkosjärjestys", 5...XX ohjelmat listassa
         {
            --button_pressed;
            if (button_pressed == 0) //cancel
            {
               return ID_OMAT_OHJELMAT;
            }
            else if (button_pressed == 1) //UUSI OHJELMA
            {
               menu_to_return = ID_LISTAA_KANSIO;
               return ID_UUSI_OHJELMA;
            }
            else  if (button_pressed == 2)
            {
            }
            else if (button_pressed == 3)
            {
            }
            else if (button_pressed == 4)
            {
            }
            else
            {
               int selected_prog = button_pressed-5;
               if (selected_prog < ohjelmien_lkm)
               { // -Kansion oikea nimi (esim "VALMIS1") on kirjoitettu -> cook_progs.cpp/tmp_kansion_nimi[MAKS_LEN];
                 //  Ohjelman nimi --> sql_haku[i].haku1 --> tmp_ohj_nimi
                  strcpy(tmp_ohj_nimi, sql_haku[selected_prog].haku1);
                  if (read_program_to_memory() == 0)
                  {
                     menu_to_return = ID_LISTAA_KANSIO;
                     return ID_NAYTA_OHJ_VAIHEET;
                  }
               }
            }
            button_pressed = 0;
         }
      }
      if (prev_ohjelmien_lkm != ohjelmien_lkm)
      { //ekalla näkymän päivityksellä ei sqlite ole vielä hakenut ohjelmia
         Button_group_empty();
         prev_ohjelmien_lkm = ohjelmien_lkm;
      }
      draw_LISTAA_KANSIO(0);
   }
   return 0xffff;
}

void init_LISTAA_KANSIO(void)
{
}

void show_LISTAA_KANSIO(int show_hide)
{
   if (show_hide == 0)
   {
      return;
   }
   if (menu_to_return != ID_LISTAA_KANSIO)
   {
      menu_before = menu_to_return;
      prev_ohjelmien_lkm = ohjelmien_lkm = 0;
   }
   pyyhk_enabled = 0;
   button_pressed = 0;
   Button_group_empty();
   alavalikko_naytolla = 0;
   if (strlen(tmp_kansion_nimi))
      sqlite_hae_ohjelmia(tmp_kansion_nimi);
   draw_LISTAA_KANSIO(0);
}

void show_kansion_nimi(void)
{
   if (tmp_header.Oma_kansio < NUM_VALMIS_KANSIOT) //JOs alle NUM_VALMIS_KANSIOT niin kyseessä valmis-kansio [0..99]
   {
      SDL_Color txt_color = { 0x00, 0x00, 0x00, 0xff };
      render_text_center((char*)kansio_nimet_tekstit[tmp_header.Oma_kansio], 400, 40, &txt_color, font_norm_regular_40px);
   }
}

#define SORT1_X 33
#define SORT2_X 294
#define SORT3_X 555
#define SORT_Y  107
#define SORT_W  212
#define SORT_H  65

void show_sort_buttons(int but_idx) //Näyttää "Käytetty eniten", "Käytetty viimeksi", "Aakkosjärjestys" valinnat
{
   SDL_Rect r;
   SDL_Color txt_color = { 0x00, 0x00, 0x00, 0xff };
   r.x = 0;
   r.y = 70;
   r.w = 803;
   r.h = 924;
   SDL_SetRenderDrawColor(gRenderer, 0xf4, 0xf4, 0xf4, 0xFF);
   SDL_RenderFillRect(gRenderer, &r); //Väritetään ruudun Tausta jonka päälle buttonit sitten

   Button_group_add_button(but_idx, SORT1_X, SORT_Y, SORT_W, SORT_H, 10);
   Button_group_setText(but_idx, YLEIS_TXT_KAYTETTY_ENITEN, TXT_GROUP_YLEIS, txt_color, font_norm_regular_20px);
   Button_group_add_images(but_idx, image_ptrs[IMAGE_ASETUKSET_YLAPAINIKE_VALKOINEN], nullptr);

   but_idx++;

   Button_group_add_button(but_idx, SORT2_X, SORT_Y, SORT_W, SORT_H, 10);
   Button_group_setText(but_idx, YLEIS_TXT_KAYTETTY_VIIMEKSI, TXT_GROUP_YLEIS, txt_color, font_norm_regular_20px);
   Button_group_add_images(but_idx, image_ptrs[IMAGE_ASETUKSET_YLAPAINIKE_VALKOINEN], nullptr);

   but_idx++;

   Button_group_add_button(but_idx, SORT3_X, SORT_Y, SORT_W, SORT_H, 10); //10=txt y-offset
   Button_group_setText(but_idx, YLEIS_TXT_AAKKOSJARJESTYS, TXT_GROUP_YLEIS, txt_color, font_norm_regular_20px);
   Button_group_add_images(but_idx, image_ptrs[IMAGE_ASETUKSET_YLAPAINIKE_HARMAA], nullptr);
}

#define OHJELMAT_X 0
#define OHJELMA1_Y 216
#define OHJELMAT_H 96
#define OHJELMAT_W 800

void show_ohjelma_buttons(int but_idx) //Näyttää ohjelmalistauksen (ohjelmien_lkm)
{
   SDL_Rect r;
   int ypos = OHJELMA1_Y;
   if (sqlite_busy == 0)
   {
      char buff[100];
      SDL_Color txt_color = { 0x00, 0x00, 0x00, 0xff };
      for (int i=0; i < sql_haku_idx; i++)
      {
         r.x = OHJELMAT_X;
         r.y = ypos;
         r.w = OHJELMAT_W;
         r.h = OHJELMAT_H;
         SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
         SDL_RenderFillRect(gRenderer, &r);
         Button_group_add_button(but_idx++, OHJELMAT_X, ypos, OHJELMAT_W, OHJELMAT_H);
         //printf("Hakutulos %d=%s\n", i, sql_haku[i].haku1);
         strcpy(buff, sql_haku[i].haku1);
         render_text((char*)buff, 22, ypos+6 , &txt_color, font_norm_regular_30px);

         sprintf(buff, "%dkg, %d %s", atoi(sql_haku[i].haku2), atoi(sql_haku[i].haku3), (char*)yleis_tekstit[YLEIS_TXT_VAIHETTA]);
         render_text((char*)buff, 22, ypos+6+35 , &txt_color, font_norm_regular_30px);

         ypos += (OHJELMAT_H+2);
      }
      ohjelmien_lkm = sql_haku_idx;
   }
   else
      printf("sqlite busy !\n");
}
