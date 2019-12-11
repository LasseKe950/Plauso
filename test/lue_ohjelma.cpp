/* 
   vaiheiden_luonti.cpp
   Uuden ohjelman luonti, vaiheet ja muistilaput

   valinnat --> set_ok_cancel_buttons()
   "TAKAISIN", x=71, y=1029 IMAGE_VALKOINEN_VIHREAT_REUNAT
   "TALLENNA", x=428, y=1029, IMAGE_VIHREA_LYHYT

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

void show_ohj_nimi(void);
void show_vaiheen_numero(void);

void show_ohj_header(int but_idx);
void show_note_uusi_vaihe_buttons(int but_idx);
void show_header_teksti(void);

void show_nayta_vaiheet_buttons(int but_idx); //Näyttää vaihe-buttonit allekkain, jos ollaan editoimassa vaihetta näyttää yhden (editoitavan vaiheen) buttonin ruudun yläosassa

void show_nayta_vaiheet_buttons_icons(void);
void show_nayta_vaiheen_edit_icons(void);

int check_vaihe_parameter_pressed(void); //Jos painettu iso button niin tsekakaan mistä kohtaa painettiin

#define NUM_BUTTON_IDX 5 //0=cancel, 1=Ok, 2=PRog header, 3=UUsi lappu, 4=Uusi vaihe, 5...XX lisätyt vaiheet
#define NUM_BUTTON_IDX_UUSI_VAIHE_MUOKKAUS 3 //Uusi vaihe lisätty: 0=cancel, 1=Ok, 2=Editoitava vaihe (jonka sisällä parametrit 2=TEMP, 3=AIKA, 4=VESI, 5=SEK.)

int uuden_Vaiheen_teko, saved_x_pos;

static int menu_before;

void draw_VAIHEIDEN_LUONTI(int param1)
{
 //  SDL_Color color = { 0x23, 0x23, 0x23, 0x0 };
 //  SDL_Color color2 = { 0xB4, 0xB4, 0xB4, 0x0 };

   SDL_Color colorwhite = { 0xff, 0xff, 0xff, 0xff };
   set_ok_cancel_buttons2(IMAGE_VIHREA_LYHYT);
   Button_group_setText(1, YLEIS_TXT_TALLENNA, TXT_GROUP_YLEIS, colorwhite, font_norm_regular_35px); //Muutetaan OK->TAllenna

   if (uuden_Vaiheen_teko != 0)
   {
      show_vaiheen_numero(); //NÄytetään VAIHE X otsikkorivillä
      show_nayta_vaiheet_buttons(2);
      for (int i=0; i< NUM_BUTTON_IDX_UUSI_VAIHE_MUOKKAUS; i++)
         Button_group_render(i);
      show_nayta_vaiheen_edit_icons(); //Näytetään vaiheen säädettävät parametrit, temp/aika/vesi/sek.
      show_banner(NUM_BUTTON_IDX_UUSI_VAIHE_MUOKKAUS);
   }
   else
   {
      show_ohj_nimi();
      show_ohj_header(2); //Button=2
      show_note_uusi_vaihe_buttons(3); //Buttons 3+4

   //   tmp_header.Vaihe_lkm = 6;
      show_nayta_vaiheet_buttons(5);

      for (int i=0; i< NUM_BUTTON_IDX + tmp_header.Vaihe_lkm; i++)
         Button_group_render(i);

      show_header_teksti();
      show_nayta_vaiheet_buttons_icons();

      show_banner(NUM_BUTTON_IDX + tmp_header.Vaihe_lkm);
   }
}

int handler_VAIHEIDEN_LUONTI(int x, int y,  SDL_Event *e)
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
         saved_x_pos = x;
      }
      else if(e->type == SDL_MOUSEBUTTONUP)
      {
         if (button_pressed) //0=cancel, 1=Ok, 2=PRog header, 3=UUsi lappu, 4=Uusi vaihe, 5...XX lisätyt vaiheet
         {
            --button_pressed;
            if (uuden_Vaiheen_teko == 0)
            {
               if (button_pressed == 0) //cancel
               {
                  if (menu_before < NUM_MENU_IDS)
                     return menu_before;
                  else
                     return ID_KOTINAKYMA;
               }
               else if (button_pressed == 1) //TALLENNA
               {
                  if (tmp_header.Vaihe_lkm)
                  {
                     int i = write_new_prog_file();
                     printf("write_new_prog_file returned %d\n", i);
                  }
                  if (menu_before < NUM_MENU_IDS)
                     return menu_before;
                  else
                     return ID_KOTINAKYMA;
               }
               else  if (button_pressed == 3) //3=UUsi lappu, 4=Uusi vaihe, 5...XX lisätyt vaiheet
               {
               }
               else if (button_pressed == 4)
               {
                  uuden_Vaiheen_teko = tmp_header.Vaihe_lkm + 1;
                  Button_group_empty();
               }
            }
            else //Uusi vaihe lisätty: 0=cancel, 1=Ok, 2=Editoitava vaihe (jonka sisällä parametrit 2=TEMP, 3=AIKA, 4=VESI, 5=SEK.)
            {
               if (button_pressed == 0) //cancel
               {
                  uuden_Vaiheen_teko = 0;
                  Button_group_empty();
               }
               else if (button_pressed == 1) //TALLENNA
               {
                  tmp_header.Vaihe_lkm++;
                  uuden_Vaiheen_teko = 0;
                  Button_group_empty();
               }
               else if (button_pressed == 2)
               {
                  int p = check_vaihe_parameter_pressed();
                  if (p)
                  {
                     set_numpad_parameter(show_numpad);
                     menu_to_return = ID_VAIHEIDEN_LUONTI;
                     if (show_numpad == NUMPAD_TARGET_SEKOITUS_VALINTA)
                        return ID_SEK_TAPA_VALINTA;
                     else
                        return ID_NUMPAD;
                  }
               }
            }
            button_pressed = 0;
         }
      }
      draw_VAIHEIDEN_LUONTI(0);
   }
   return 0xffff;
}

void init_VAIHEIDEN_LUONTI(void)
{
}

void show_VAIHEIDEN_LUONTI(int show_hide)
{
   if (show_hide == 0)
   {
      return;
   }
   if (menu_to_return != ID_VAIHEIDEN_LUONTI)
   {
      uuden_Vaiheen_teko = 0;
      menu_before = menu_to_return;
   }
   pyyhk_enabled = 0;
   button_pressed = 0;
   Button_group_empty();
   alavalikko_naytolla = 0;
   draw_VAIHEIDEN_LUONTI(0);
}

void show_ohj_nimi(void)
{
   if (strlen(tmp_ohj_nimi) == 0)
      return;
   SDL_Color txt_color = { 0x00, 0x00, 0x00, 0xff };
   render_text_center(tmp_ohj_nimi, 400, 40, &txt_color, font_norm_regular_40px);
}

void show_vaiheen_numero(void)
{
   char buff[50];
   SDL_Color txt_color = { 0x00, 0x00, 0x00, 0xff };
   sprintf(buff, "%s %d", yleis_tekstit[YLEIS_TXT_UUSI_VAIHE], uuden_Vaiheen_teko);
   render_text_center(buff, 400, 40, &txt_color, font_norm_regular_40px);
}

#define OHJ_HEADER_X 8
#define OHJ_HEADER_Y 104
#define OHJ_HEADER_W 782
#define OHJ_HEADER_H 80

#define OHJ_SIJAINTI_TXT_X 42
#define OHJ_SIJAINTI_TXT_Y 114

#define TYOK_SIJAINTI_TXT_X 42
#define TYOK_SIJAINTI_TXT_Y (114+26)

#define ERAK_SIJAINTI_TXT_X 42
#define ERAK_SIJAINTI_TXT_Y (114+26+26)

void show_ohj_header(int but_idx)
{
   SDL_Rect r;
   r.x = 0;
   r.y = 70;
   r.w = 803;
   r.h = 924;
   SDL_SetRenderDrawColor(gRenderer, 0xf4, 0xf4, 0xf4, 0xFF);
   SDL_RenderFillRect(gRenderer, &r);

   Button_group_add_button(but_idx, OHJ_HEADER_X, OHJ_HEADER_Y, OHJ_HEADER_W, OHJ_HEADER_H);
   Button_group_add_images(but_idx, image_ptrs[IMAGE_RESEPTIN_MUOKKAUS_PAINIKE], nullptr, OHJ_HEADER_X, OHJ_HEADER_Y);
}

void show_header_teksti(void)
{
   SDL_Color txt_color = { 0x00, 0x00, 0x00, 0xff };
   char buff[80];
   if (tmp_header.Oma_kansio < NUM_VALMIS_KANSIOT) //JOs alle NUM_VALMIS_KANSIOT niin kyseessä valmis-kansio [0..99]
   {
      sprintf(buff, "%s:%s", yleis_tekstit[YLEIS_TXT_SIJAINTI], (char*)kansio_nimet_tekstit[tmp_header.Oma_kansio]);
      render_text((char*)buff, OHJ_SIJAINTI_TXT_X, OHJ_SIJAINTI_TXT_Y, &txt_color, font_norm_regular_20px);
   }
   sprintf(buff, "%s:%s", yleis_tekstit[YLEIS_TXT_TYOKALU], (char*)tyokalut_tekstit[tmp_header.Tyokalu_idx]);
   render_text((char*)buff, TYOK_SIJAINTI_TXT_X, TYOK_SIJAINTI_TXT_Y, &txt_color, font_norm_regular_20px);

   sprintf(buff, "%s:%d kg", yleis_tekstit[YLEIS_TXT_VALMISTUSERAN_KOKO], tmp_header.Erakoko_kg);
   render_text((char*)buff, ERAK_SIJAINTI_TXT_X, ERAK_SIJAINTI_TXT_Y, &txt_color, font_norm_regular_20px);
}

#define UUSI_NOTE_Y 812
#define UUSI_VAIHE_Y 898

void show_note_uusi_vaihe_buttons(int but_idx)
{
   SDL_Color txt_color = { 0x00, 0x00, 0x00, 0xff };
   Button_group_add_button(but_idx, OHJ_HEADER_X, UUSI_NOTE_Y, OHJ_HEADER_W, OHJ_HEADER_H);
   Button_group_add_images(but_idx, image_ptrs[IMAGE_RESEPTIN_MUOKKAUS_PAINIKE], image_ptrs[IMAGE_MUISTIINPANOT_B_76px], OHJ_HEADER_X, UUSI_NOTE_Y);
   Button_groups[but_idx].icon_image_pos.y = 8;//VOidaan hienosäätää iconin paikkaa tarpeen mukaan
   ++but_idx;
   Button_group_add_button(but_idx, OHJ_HEADER_X, UUSI_VAIHE_Y, OHJ_HEADER_W, OHJ_HEADER_H, 8);
   Button_group_add_images(but_idx, image_ptrs[IMAGE_RESEPTIN_MUOKKAUS_PAINIKE], nullptr, OHJ_HEADER_X, UUSI_VAIHE_Y);
   Button_group_setText(but_idx, YLEIS_TXT_UUSI_VAIHE, TXT_GROUP_YLEIS, txt_color, font_norm_regular_35px);
}

#define VAIHE_1_Y 190

void show_nayta_vaiheet_buttons(int but_idx) //Näyttää vaihe-buttonit allekkain, jos ollaan editoimassa vaihetta näyttää yhden (editoitavan vaiheen) buttonin ruudun yläosassa
{
   int ypos = VAIHE_1_Y;
   int num;

   if (uuden_Vaiheen_teko != 0)
   {
      num = 1;
      ypos = OHJ_HEADER_Y;
   }
   else
      num = tmp_header.Vaihe_lkm;

   for (int i=0; i < num; i++)
   {
      Button_group_add_button(but_idx, OHJ_HEADER_X, ypos, OHJ_HEADER_W, OHJ_HEADER_H);
      Button_group_add_images(but_idx, image_ptrs[IMAGE_RESEPTIN_MUOKKAUS_PAINIKE], 0, OHJ_HEADER_X, ypos);
      ypos += 96;
      but_idx++;
   }
}

#define VIIVA_1_X 191
#define VIIVA_1_Y 200
#define VIIVA_H   72
#define VIIVA_2_X 385
#define VIIVA_3_X 570

#define VAIHE_1_Y_ICO  210
#define VAIHE_X_COOK 20
#define VAIHE_X_AIKA 210
#define VAIHE_X_VESI 400
#define VAIHE_X_SEKO 590

#define VAIHE_VALUE_X 80
#define VAIHE_VALUE_Y 10

void show_vaihe_values_on_button(int vaihe, int ypos)
{
   char buff[30];
   SDL_Color txt_color = { 0x00, 0x00, 0x00, 0xff };

   if (tmp_steps[ vaihe ].Temp1)
      sprintf(buff, "%d C", tmp_steps[ vaihe ].Temp1);
   else
      strcpy(buff, "-- C");
   render_text((char*)buff, VAIHE_X_COOK + VAIHE_VALUE_X, ypos + VAIHE_VALUE_Y, &txt_color, font_norm_regular_35px);

   if (tmp_steps[ vaihe ].Aika)
      sprintf(buff, "%d:%02d", tmp_steps[ vaihe ].Aika/60, tmp_steps[ vaihe ].Aika%60);
   else
      strcpy(buff, "--:--");
   render_text((char*)buff, VAIHE_X_AIKA + VAIHE_VALUE_X, ypos + VAIHE_VALUE_Y, &txt_color, font_norm_regular_35px);

   if (tmp_steps[ vaihe ].Vesi)
      sprintf(buff, "%d l", tmp_steps[ vaihe ].Vesi);
   else
      strcpy(buff, "-- l");
   render_text((char*)buff, VAIHE_X_VESI + VAIHE_VALUE_X, ypos + VAIHE_VALUE_Y, &txt_color, font_norm_regular_35px);

   if (tmp_steps[ vaihe ].Nopeus)
   {
      sprintf(buff, "%d", tmp_steps[ vaihe ].Nopeus);
   }
   else
      strcpy(buff, "--");
   render_text((char*)buff, VAIHE_X_SEKO + VAIHE_VALUE_X, ypos + VAIHE_VALUE_Y, &txt_color, font_norm_regular_35px);

   if (uuden_Vaiheen_teko && tmp_steps[ vaihe ].Sek_tapa) //Näytetään valittu sek.tapa
   {
      ypos += 45;
      sprintf(buff, "%s", sekoitustapa_tekstit[ tmp_steps[ vaihe ].Sek_tapa - 1]);
      render_text((char*)buff, VAIHE_X_SEKO + VAIHE_VALUE_X - 90, ypos + VAIHE_VALUE_Y, &txt_color, font_norm_medium_16px);
   }
}

void show_nayta_vaiheet_buttons_icons(void)
{
   int ypos = VAIHE_1_Y_ICO;
   int yypos= VIIVA_1_Y;
   Uint32 c;

   c = 0xFF<<24 | (0x70<<16) | (0x70<<8) | (0x70); //bgr

   for (int i=0; i < tmp_header.Vaihe_lkm; i++)
   {
      if (tmp_steps[i].Step_type == STEP_TYPE_CHILL_STEP)
         image_ptrs[IMAGE_SININEN_JAAHDYTYS_76px]->render(VAIHE_X_COOK,   ypos);
      else
         image_ptrs[IMAGE_MUSTA_LIEKKI_58px]->render(VAIHE_X_COOK,   ypos);
      image_ptrs[IMAGE_MUSTA_AJASTUS_58px]->render(VAIHE_X_AIKA,  ypos);
      image_ptrs[IMAGE_MUSTA_PISARA_58px]->render(VAIHE_X_VESI,   ypos);
      image_ptrs[IMAGE_MUSTA_SEKOITUS_58px]->render(VAIHE_X_SEKO, ypos);

      lineColor(gRenderer, VIIVA_1_X, yypos, VIIVA_1_X, yypos + VIIVA_H, c);
      lineColor(gRenderer, VIIVA_2_X, yypos, VIIVA_2_X, yypos + VIIVA_H, c);
      lineColor(gRenderer, VIIVA_3_X, yypos, VIIVA_3_X, yypos + VIIVA_H, c);

      show_vaihe_values_on_button(i, ypos);

      yypos += 96;
      ypos += 96;
   }
}

void show_nayta_vaiheen_edit_icons(void) //Näytetään vaiheen säädettävät parametrit, temp/aika/vesi/sek.
{
   int ypos = OHJ_HEADER_Y + 12;
   int yypos= OHJ_HEADER_Y + 9;
   Uint32 c;

   c = 0xFF<<24 | (0x70<<16) | (0x70<<8) | (0x70); //bgr

   if (tmp_steps[tmp_header.Vaihe_lkm].Step_type == STEP_TYPE_CHILL_STEP)
      image_ptrs[IMAGE_SININEN_JAAHDYTYS_76px]->render(VAIHE_X_COOK,   ypos);
   else
      image_ptrs[IMAGE_MUSTA_LIEKKI_58px]->render(VAIHE_X_COOK,   ypos);
   image_ptrs[IMAGE_MUSTA_AJASTUS_58px]->render(VAIHE_X_AIKA,  ypos);
   image_ptrs[IMAGE_MUSTA_PISARA_58px]->render(VAIHE_X_VESI,   ypos);
   image_ptrs[IMAGE_MUSTA_SEKOITUS_58px]->render(VAIHE_X_SEKO, ypos);

   lineColor(gRenderer, VIIVA_1_X, yypos, VIIVA_1_X, yypos + VIIVA_H, c);
   lineColor(gRenderer, VIIVA_2_X, yypos, VIIVA_2_X, yypos + VIIVA_H, c);
   lineColor(gRenderer, VIIVA_3_X, yypos, VIIVA_3_X, yypos + VIIVA_H, c);

   show_vaihe_values_on_button(tmp_header.Vaihe_lkm, ypos);
}

int check_vaihe_parameter_pressed(void) //Jos painettu iso button niin tsekakaan mistä kohtaa painettiin
{
   if (saved_x_pos < VIIVA_1_X)
   {
      if (tmp_steps[ tmp_header.Vaihe_lkm ].Step_type == STEP_TYPE_CHILL_STEP)
      {
         numpad_selected_temp_mode = 4;//0=Norm.COOK, 1=vaippaohjaus, 2=delta, 3=ext temp, 4=Chill
         prepare_target_params.chill_temps[TARGET_CHILL_TEMP_IDX] = 30;
         show_numpad = NUMPAD_TARGET_CHILLTEMP;
      }
      else //if (tmp_steps[ tmp_header.Vaihe_lkm ].Step_type == STEP_TYPE_COOK_STEP)
      {
         numpad_selected_temp_mode = 0;//0=Norm.COOK, 1=vaippaohjaus, 2=delta, 3=ext temp, 4=Chill
         prepare_target_params.cook_temps[TARGET_COOK_FOOD_TEMP_IDX] = 100;
         show_numpad = NUMPAD_TARGET_FOODTEMP;
      }
      return 1;
   }
   else if (saved_x_pos < VIIVA_2_X)
   {
      prepare_target_params.cook_time_minutes = 10;
      show_numpad = NUMPAD_TARGET_COOK_TIME;
      return 2;
   }
   else if (saved_x_pos < VIIVA_3_X)
   {
      show_numpad = NUMPAD_TARGET_WATER;
      return 3;
   }
   else
   {
      prepare_target_params.mixer_speed = 0;
      show_numpad = NUMPAD_TARGET_SEKOITUS_VALINTA;
      return 4;
   }
}

void numpad_updates_vaiheen_parametri(void)
{
   switch (numpad_target_parameter)
   {
      case NUMPAD_TARGET_FOODTEMP:
         tmp_steps[ tmp_header.Vaihe_lkm ].Temp1 = numpad_parameter_val;
      break;

      case NUMPAD_TARGET_KETTLETEMP:
         tmp_steps[ tmp_header.Vaihe_lkm ].Temp2 = numpad_parameter_val;
      break;

      case NUMPAD_TARGET_CHILLTEMP:
         tmp_steps[ tmp_header.Vaihe_lkm ].Temp1 = numpad_parameter_val;
      break;

      case NUMPAD_TARGET_COOK_TIME:
         tmp_steps[ tmp_header.Vaihe_lkm ].Aika = numpad_parameter_val;
         printf("AIKA = %d min\n", numpad_parameter_val);
      break;

      case NUMPAD_TARGET_CHILL_TIME:
         tmp_steps[ tmp_header.Vaihe_lkm ].Aika = numpad_parameter_val;
      break;

      case NUMPAD_TARGET_WATER:
         tmp_steps[ tmp_header.Vaihe_lkm ].Vesi = numpad_parameter_val;
      break;

      case NUMPAD_TARGET_SEKOITUS_VALINTA:
         tmp_steps[ tmp_header.Vaihe_lkm ].Nopeus = numpad_parameter_val;
      break;
   }
}

void numpad_updates_vaiheen_sek_mode_parametri(int mode)
{
   tmp_steps[ tmp_header.Vaihe_lkm ].Sek_tapa = mode;
   run_target_params.selected_mixer_mode = mode; //Muuten mixer numpad ei näytä modea
   if (mode)
      printf("Sek Mode = %d %s\n", mode, sekoitustapa_tekstit[mode - 1]);
}

void numpad_updates_vaiheen_selected_temp_mode_parametri(void)
{
   if (numpad_selected_temp_mode == 4)//0=Norm.COOK, 1=vaippaohjaus, 2=delta, 3=ext temp, 4=Chill
   {
      tmp_steps[ tmp_header.Vaihe_lkm ].Step_type = STEP_TYPE_CHILL_STEP;
      //numpad_target_parameter = NUMPAD_TARGET_CHILLTEMP;
   }
   else
   {
      tmp_steps[ tmp_header.Vaihe_lkm ].Step_type = STEP_TYPE_COOK_STEP;
      //numpad_target_parameter = NUMPAD_TARGET_FOODTEMP;
   }
}
