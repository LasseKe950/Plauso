/* 
   uusi_ohjelma.cpp
   Uuden ohjelman luonti, annetaan perustiedot

   -iso rectangle x=0, y=70, w=800, h=1005-70, color=F4F4F4
   -"Valitse sijainti" teksti, x=18, y=157, Simplon Norm, Regular 25px, colo3=232323
     -valinnan alasvetovalintapainike, x=18,y=207, w=750, h=82, br=16, color=FFFFFF (valkoinen)

   -"Ohjelman nimi" teksti, x=18, y=157, Simplon Norm, Regular 25px, colo3=232323
     -kenttä, x=16, y=296, w=685, h=82, br=16, color=FFFFFF

   -"Työkalu"
   
   -"Valmistuserän koko"
   
   -"Reseptin kuvaus"

   valinnat --> set_ok_cancel_buttons()
   "TAKAISIN", x=71, y=1029 IMAGE_VALKOINEN_VIHREAT_REUNAT
   "TALLENNA", x=428, y=1029, IMAGE_VIHREA_LYHYT

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

void write_prog_header(void);

extern int current_dialog_id;

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

#define TEXT3_X 18
#define TEXT3_Y (477+15)

#define TEXT3_ALUE_X 18
#define TEXT3_ALUE_Y 526
#define TEXT3_ALUE_W 750
#define TEXT3_ALUE_H 82

#define TEXT4_X 18
#define TEXT4_Y (625+15)

#define TEXT4_ALUE_X 18
#define TEXT4_ALUE_Y 669
#define TEXT4_ALUE_W 362 //Lyhyt kenttä
#define TEXT4_ALUE_H 82

#define TEXT5_X 18
#define TEXT5_Y (773+15)

#define TEXT5_ALUE_X 18
#define TEXT5_ALUE_Y 822
#define TEXT5_ALUE_W 750
#define TEXT5_ALUE_H 120

#define NUM_BUTTON_IDX 7 //0=cancel, 1=Ok, 2=valitse sijainti-kenttä, 3=ohjelman nimi-kenttä, 4=työkalu, 5=valmistuserän koko, 6=reseptin kuvaus

static int menu_before;

static void show_dropdown_valinnat(void);
int erakoko_kg;
char ohjelman_nimi[100];

int deb;

void draw_UUSI_OHJELMA(int param1)
{
   SDL_Color color = { 0x23, 0x23, 0x23, 0x0 };
   SDL_Color color2 = { 0xB4, 0xB4, 0xB4, 0x0 };

      SDL_Color colorwhite = { 0xff, 0xff, 0xff, 0xff };
      set_ok_cancel_buttons2(IMAGE_VIHREA_LYHYT);
      Button_group_setText(1, YLEIS_TXT_TALLENNA, TXT_GROUP_YLEIS, colorwhite, font_norm_regular_35px); //Muutetaan OK->TAllenna

   //button alue
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

   if (show_dropdown != 1) //Ei piirretä kenttää jos dropdown on sen päällä, (jos kenttä=ohjelman nimi ja dd=valitse sijainti)
   {
      roundedBoxColor(gRenderer, TEXT2_ALUE_X, TEXT2_ALUE_Y, (TEXT2_ALUE_X+TEXT2_ALUE_W),  (TEXT2_ALUE_Y+TEXT2_ALUE_H), 16, c);
      Button_group_add_button(3, TEXT2_ALUE_X, TEXT2_ALUE_Y, TEXT2_ALUE_W, TEXT2_ALUE_H);
   }

   roundedBoxColor(gRenderer, TEXT3_ALUE_X, TEXT3_ALUE_Y, (TEXT3_ALUE_X+TEXT3_ALUE_W), (TEXT3_ALUE_Y+TEXT3_ALUE_H), 16, c);
   Button_group_add_button(4, TEXT3_ALUE_X, TEXT3_ALUE_Y, TEXT3_ALUE_W, TEXT3_ALUE_H);

   roundedBoxColor(gRenderer, TEXT4_ALUE_X, TEXT4_ALUE_Y, (TEXT4_ALUE_X+TEXT4_ALUE_W), (TEXT4_ALUE_Y+TEXT4_ALUE_H), 16, c);
   Button_group_add_button(5, TEXT4_ALUE_X, TEXT4_ALUE_Y, TEXT4_ALUE_W, TEXT4_ALUE_H);

   roundedBoxColor(gRenderer, TEXT5_ALUE_X, TEXT5_ALUE_Y, (TEXT5_ALUE_X+TEXT5_ALUE_W), (TEXT5_ALUE_Y+TEXT5_ALUE_H), 16, c);
   Button_group_add_button(6, TEXT5_ALUE_X, TEXT5_ALUE_Y, TEXT5_ALUE_W, TEXT5_ALUE_H);

   for (int i=0; i< NUM_BUTTON_IDX; i++)
      Button_group_render(i);

   //dropdown väkänen paikkaan x=731 y=245, w=12, h=7 ja myös paikkaan x=731 y=562, w=12, h=7
   image_ptrs[IMAGE_NUOLI_ALAS_32px]->render(725, 235);
   image_ptrs[IMAGE_NUOLI_ALAS_32px]->render(725, 552);
#if 0
   c = 0xFF<<24 | (0x23<<16) | (0x23<<8) | (0x23); //bgr
   for (int p=0; p<7; p++)
   {
      pixelColor(gRenderer, 731+p, 245+p, c);
      pixelColor(gRenderer, (731+12)-p, 245+p, c);
      pixelColor(gRenderer, 731+p, 245+p, c);
      pixelColor(gRenderer, (731+12)-p, 245+p, c);

      pixelColor(gRenderer, 731+p, 562+p, c);
      pixelColor(gRenderer, (731+12)-p, 562+p, c);
      pixelColor(gRenderer, 731+p, 562+p, c);
      pixelColor(gRenderer, (731+12)-p, 562+p, c);
   }
#endif

   render_text((char*)yleis_tekstit[YLEIS_TXT_VALITSE_SIJAINTI], TEXT1_X, TEXT1_Y, (SDL_Color *)&color, font_norm_regular_25px);
   render_text((char*)yleis_tekstit[YLEIS_TXT_OHJELMAN_NIMI], TEXT2_X, TEXT2_Y, (SDL_Color *)&color, font_norm_regular_25px);
   render_text((char*)yleis_tekstit[YLEIS_TXT_TYOKALU], TEXT3_X, TEXT3_Y, (SDL_Color *)&color, font_norm_regular_25px);
   render_text((char*)yleis_tekstit[YLEIS_TXT_VALMISTUSERAN_KOKO], TEXT4_X, TEXT4_Y, (SDL_Color *)&color, font_norm_regular_25px);
   render_text((char*)yleis_tekstit[YLEIS_TXT_RESEPTIN_KUVAUS], TEXT5_X, TEXT5_Y, (SDL_Color *)&color, font_norm_regular_25px);

   if (dropdown_selection[0] >= NUM_KANSIO_NIMET_TEKSTIT)
      dropdown_selection[0] = 0;
   render_text((char*)kansio_nimet_tekstit[dropdown_selection[0]], TEXT1_ALUE_X, TEXT1_ALUE_Y+(TEXT1_ALUE_H/2), (SDL_Color *)&color2, font_norm_regular_25px, 0);

   if (strlen(ohjelman_nimi) && !show_dropdown)
      render_text((char*)ohjelman_nimi, TEXT2_ALUE_X, TEXT2_ALUE_Y+(TEXT2_ALUE_H/2), (SDL_Color *)&color2, font_norm_regular_25px, 0);

   if (dropdown_selection[1] >= NUM_TYOKALUT_TEKSTIT)
      dropdown_selection[1] = 0;
   render_text((char*)tyokalut_tekstit[dropdown_selection[1]], TEXT3_ALUE_X, TEXT3_ALUE_Y+(TEXT3_ALUE_H/2), (SDL_Color *)&color2, font_norm_regular_25px, 0);

   char buff[15];
   sprintf(buff, "%d", erakoko_kg);
   render_text((char*)buff, TEXT4_ALUE_X, TEXT4_ALUE_Y+(TEXT4_ALUE_H/2), (SDL_Color *)&color2, font_norm_regular_25px, 0);

   show_banner(NUM_BUTTON_IDX);
}

int handler_UUSI_OHJELMA(int x, int y,  SDL_Event *e)
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
         if (show_dropdown)
            b = Dropdown_group_check_for_key_press(x, y);
         if (b == 0)
            b = Button_group_check_for_key_press(x, y);
         if (b == CODE_DIETA_LOGO)
            return ID_KOTINAKYMA;
         button_pressed = b;
      }
      else if(e->type == SDL_MOUSEBUTTONUP)
      {
         if (button_pressed) //1=cancel, 2=TALLENNA, 3=valitse sijainti-kenttä, 4=ohjelman nimi-kenttä, 5=työkalu, 6=valmistuserän koko, 7=reseptin kuvaus
         {
            if ((button_pressed >= DROPDOWN_EVENT_CODE1) && (button_pressed <= DROPDOWN_EVENT_CODE1 + 20))
            {
               if (show_dropdown==1)
                  dropdown_selection[0] = button_pressed - DROPDOWN_EVENT_CODE1;
               else if (show_dropdown==2)
                  dropdown_selection[1] = button_pressed - DROPDOWN_EVENT_CODE1;
               show_dropdown = 0;
            }
            else if (button_pressed == 1) //cancel
            {
               if ((menu_before == ID_LISTAA_KANSIO) || (menu_before == ID_OMAT_OHJELMAT))
                  return menu_before;
               else
                  return ID_KOTINAKYMA;
            }
            else if (button_pressed == 2) //TALLENNA
            {
               menu_to_return = ID_UUSI_OHJELMA;
               write_prog_header(); //Kirjoittaa header tietoja -> cook_prog.cpp
               return ID_VAIHEIDEN_LUONTI;
            }
            else if (button_pressed == 3)
            {
               if (show_dropdown)
                  show_dropdown = 0;
               else
                  show_dropdown = 1;
            }
            else if (button_pressed == 4) //Reseptin nimi
            {
               menu_to_return = ID_UUSI_OHJELMA;
               return ID_KEYPAD;
            }
            else if (button_pressed == 5)
            {
               if (show_dropdown)
                  show_dropdown = 0;
               else
                  show_dropdown = 2;
            }
            else if (button_pressed == 6) //Erä koko
            {
               menu_to_return = ID_UUSI_OHJELMA;
               return ID_NUMPAD_PIENI;
            }
            else if (button_pressed == 7) //Kuvaus
            {
            }

            button_pressed = 0;
         }
      }
      draw_UUSI_OHJELMA(0);
      if (show_dropdown)
         show_dropdown_valinnat();
   }
   return 0xffff;
}

void init_UUSI_OHJELMA(void)
{
}

void show_UUSI_OHJELMA(int show_hide)
{
   if (show_hide == 0)
   {
      return;
   }
   if ((menu_to_return != ID_UUSI_OHJELMA) && (menu_to_return != ID_VAIHEIDEN_LUONTI))
   {
      menu_before = menu_to_return;
      erakoko_kg = 0;
      memset(ohjelman_nimi, 0, sizeof(ohjelman_nimi));
      memset(tmp_steps, 0, sizeof(tmp_steps));
      dropdown_selection[0] = 0;
      dropdown_selection[1] = 0;
      if (menu_to_return == ID_LISTAA_KANSIO)
      {
         if (tmp_header.Oma_kansio < NUM_VALMIS_KANSIOT)//   -tmp_header.Oma_kansio sisältää indeksin valittun kansioon (kts. cook_prog.h)
            dropdown_selection[0] = tmp_header.Oma_kansio;
      }
   }

   pyyhk_enabled = 0;
   button_pressed = 0;
   Button_group_empty();
   Dropdown_group_empty();

   show_dropdown = alavalikko_naytolla = 0;
   draw_UUSI_OHJELMA(0);
}

#define SEL1_ALUE_X 18
#define SEL1_ALUE_Y 207
#define SEL1_ALUE_W 750
#define SEL1_ALUE_H 50

#define SEL2_ALUE_X 18
#define SEL2_ALUE_Y 526
#define SEL2_ALUE_W 750
#define SEL2_ALUE_H 50

void show_dropdown_valinnat(void)
{
   int i;
   int x, y;
   Uint32 c;
   SDL_Color color = { 0x23, 0x23, 0x23, 0x0 };
                    //blue       green      red
   c = 0xFF<<24 | (0xff<<16) | (0xff<<8) | (0xff); //bgr

   if (show_dropdown == 1)
   {
      x = SEL1_ALUE_X;
      y = SEL1_ALUE_Y;
      for (i=0; i < NUM_KANSIO_NIMET_TEKSTIT; i++)
      {
         boxColor(gRenderer, x, y, (x+SEL1_ALUE_W), (y+SEL1_ALUE_H), c);
         Dropdown_group_add_selection(i, x, y, SEL1_ALUE_W, SEL1_ALUE_H);
         Dropdown_group_add_event_code(i, DROPDOWN_EVENT_CODE1+i);
         render_text((char*)kansio_nimet_tekstit[i], x+10, y+6, (SDL_Color *)&color, font_norm_regular_25px, 0);
         y += (SEL1_ALUE_H+3);
      }
   }
   else if (show_dropdown == 2)
   {
      x = SEL2_ALUE_X;
      y = SEL2_ALUE_Y;
      for (i=0; i < NUM_TYOKALUT_TEKSTIT; i++)
      {
         boxColor(gRenderer, x, y, (x+SEL2_ALUE_W), (y+SEL2_ALUE_H), c);
         Dropdown_group_add_selection(i, x, y, SEL2_ALUE_W, SEL2_ALUE_H);
         Dropdown_group_add_event_code(i, DROPDOWN_EVENT_CODE1+i);
         render_text((char*)tyokalut_tekstit[i], x+10, y+6, (SDL_Color *)&color, font_norm_regular_25px, 0);
         y += (SEL1_ALUE_H+3);
      }
   }
}

void uusi_ohjelma_erakoko_annettiin(void)
{
   numpad_string[4] = 0;
   int i = sscanf(numpad_string, "%d", &erakoko_kg);
//   printf("uusi_ohjelma_erakoko i = %d era=%d\n", i, erakoko_kg);
}

void uusi_ohjelma_nimi_annetaan(void)
{
   Uint32 c;
   SDL_Color color2 = { 0xB4, 0xB4, 0xB4, 0x0 };
   c = 0xFF<<24 | (0xFF<<16) | (0xFF<<8) | (0xFF); //bgr
   roundedBoxColor(gRenderer, TEXT2_ALUE_X, TEXT2_ALUE_Y, (TEXT2_ALUE_X+TEXT2_ALUE_W),  (TEXT2_ALUE_Y+TEXT2_ALUE_H), 16, c);
   strcpy(ohjelman_nimi, keypad_string);
   render_text((char*)keypad_string, TEXT2_ALUE_X, TEXT2_ALUE_Y+(TEXT2_ALUE_H/2), (SDL_Color *)&color2, font_norm_regular_25px, 0);
}

void write_prog_header(void)
{
   clear_prog_header();
   set_prog_header_ohj_nimi(ohjelman_nimi);
   //set_prog_header_kansion_nimi((char*)kansio_nimet_tekstit[dropdown_selection[0]]);
   if (dropdown_selection[0] >= NUM_VALMIS_KANSIOT)
      return;
   set_prog_header_kansion_nimi((char*)valmis_kansio_nimet[dropdown_selection[0]], dropdown_selection[0]);
   set_prog_header_tyokalu_erakoko(dropdown_selection[1], erakoko_kg);

   printf("Set Prog header OK.\n");
/*
   cook_prog_step_s new_step;
   memset(&new_step, 0, sizeof(new_step));

   set_step_cook_params(100, 111, 60, &new_step);
   set_step_water_params(55, 0, &new_step);
   set_step_mixer_params(MIX_MODES_YHTEEN_SUUNTAAN, 80, &new_step);

   printf("Set step OK.\n");

   insert_new_step(&new_step);

   printf("Insert new step OK.\n");

   int i = write_new_prog_file();
   printf("write_new_prog_file returned %d\n", i);
*/
}

