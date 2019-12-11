/* 
   kyps_icons.cpp
   -functiot jotka piirtää kypsennyksen/jäähdytyksen ikonit ja niihin liittyvät tekstit/mittausarvot etc. näytölle

   Nyös "sidebar" piirto täällä.
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
#include "image.h"
#include "button_group.h"
#include "externs.h"
#include "cook_prog.h"

#define LIEKKI_ICON_X 18
#define LIEKKI_ICON_Y 90

#define COOK_PYRAMIDI_ICON_X (LIEKKI_ICON_X + 6)
#define COOK_PYRAMIDI_ICON_Y (LIEKKI_ICON_Y + 75)

#define ASETUS_LAMPO_TEXT_X (COOK_PYRAMIDI_ICON_X + 8) /*Tämä ja sen yllä oleva pyramidi kuuluu yhteen*/
#define ASETUS_LAMPO_TEXT_Y (COOK_PYRAMIDI_ICON_Y + 55)

#define RUOKA_LAMPO_ICON_X (COOK_PYRAMIDI_ICON_X + 2)
#define RUOKA_LAMPO_ICON_Y (COOK_PYRAMIDI_ICON_Y+80 + 6)

#define RUOKA_LAMPO_TEXT_X (RUOKA_LAMPO_ICON_X + 50)
#define RUOKA_LAMPO_TEXT_Y (RUOKA_LAMPO_ICON_Y + 10)

#define VAIPPA_LAMPO_ICON_X (RUOKA_LAMPO_ICON_X + 110)
#define VAIPPA_LAMPO_ICON_Y (COOK_PYRAMIDI_ICON_Y+80 + 6)

#define VAIPPA_LAMPO_TEXT_X (VAIPPA_LAMPO_ICON_X + 50)
#define VAIPPA_LAMPO_TEXT_Y (VAIPPA_LAMPO_ICON_Y + 10)

//Cook osaston ikonit
void draw_kyps_jaahd_cook_icons(void)
{
   char buff[15];
   int i;

   if (ylavalikko_naytolla)
      return;

   SDL_Color txt_color = { 0xff, 0xff, 0xff, 0xff };
   //Renders texture at given point
   //   void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
   if (image_ptrs[IMAGE_PUNAINEN_LIEKKI_76px])
      image_ptrs[IMAGE_PUNAINEN_LIEKKI_76px]->render(LIEKKI_ICON_X, LIEKKI_ICON_Y);

   if (run_target_params.cook_mode)
   {
      if (image_ptrs[IMAGE_VALKOINEN_PYRAMIDI_58px]) //Pyramidi kuvaa mittausarvon nousua / laskua
         image_ptrs[IMAGE_VALKOINEN_PYRAMIDI_58px]->render(COOK_PYRAMIDI_ICON_X, COOK_PYRAMIDI_ICON_Y);

      i = run_target_params.cook_temps[TARGET_COOK_FOOD_TEMP_IDX]; //asetuslämpötila
      sprintf(buff, "%d °C", i);
      render_text(buff, ASETUS_LAMPO_TEXT_X, ASETUS_LAMPO_TEXT_Y, &txt_color, font_norm_regular_20px, 1); //last par=utf8, muuten ei astemerkki tulostu oikein..jostain syystä.?
   }

   if (image_ptrs[IMAGE_RUOKA_LAMPO_IKONI_W_45px])
      image_ptrs[IMAGE_RUOKA_LAMPO_IKONI_W_45px]->render(RUOKA_LAMPO_ICON_X, RUOKA_LAMPO_ICON_Y); //Tämän viereen tulostetaan ruoan lämpötila

   i = live_params.cook_temps[TARGET_COOK_FOOD_TEMP_IDX];
   sprintf(buff, "%d °C", i);
   render_text(buff, RUOKA_LAMPO_TEXT_X, RUOKA_LAMPO_TEXT_Y, &txt_color, font_norm_bold_20px, 1);

   if (image_ptrs[IMAGE_VAIPPA_LAMPO_IKONI_W_45px])
      image_ptrs[IMAGE_VAIPPA_LAMPO_IKONI_W_45px]->render(VAIPPA_LAMPO_ICON_X, VAIPPA_LAMPO_ICON_Y);//Tämän viereen tulostetaan vaipan lämpötila

   i = live_params.cook_temps[TARGET_COOK_KETTLE_TEMP_IDX];
   sprintf(buff, "%d °C", i);
   render_text(buff, VAIPPA_LAMPO_TEXT_X, VAIPPA_LAMPO_TEXT_Y, &txt_color, font_norm_bold_20px, 1);
}

//Chill osaston ikonit
void draw_kyps_jaahd_chill_icons(void)
{
   char buff[15];
   int i;

   if (ylavalikko_naytolla)
      return;

   SDL_Color txt_color = { 0xff, 0xff, 0xff, 0xff };
   //Renders texture at given point
   //   void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
   if (image_ptrs[IMAGE_SININEN_JAAHDYTYS_76px])
      image_ptrs[IMAGE_SININEN_JAAHDYTYS_76px]->render(LIEKKI_ICON_X, LIEKKI_ICON_Y);

   if (run_target_params.chill_mode)
   {
      if (image_ptrs[IMAGE_VALKOINEN_PYRAMIDI_NURIN_58px]) //Pyramidi kuvaa mittausarvon nousua / laskua
         image_ptrs[IMAGE_VALKOINEN_PYRAMIDI_NURIN_58px]->render(COOK_PYRAMIDI_ICON_X, COOK_PYRAMIDI_ICON_Y);

      i = run_target_params.chill_temps[TARGET_CHILL_TEMP_IDX]; //asetuslämpötila
      sprintf(buff, "%d °C", i);
      render_text(buff, ASETUS_LAMPO_TEXT_X, ASETUS_LAMPO_TEXT_Y, &txt_color, font_norm_regular_20px, 1); //last par=utf8, muuten ei astemerkki tulostu oikein..jostain syystä.?
   }

   if (image_ptrs[IMAGE_RUOKA_LAMPO_IKONI_W_45px])
      image_ptrs[IMAGE_RUOKA_LAMPO_IKONI_W_45px]->render(RUOKA_LAMPO_ICON_X, RUOKA_LAMPO_ICON_Y); //Tämän viereen tulostetaan ruoan lämpötila

   i = live_params.cook_temps[TARGET_COOK_FOOD_TEMP_IDX]; //Huom! mitattu temp = live_params.cook_temps !
   sprintf(buff, "%d °C", i);
   render_text(buff, RUOKA_LAMPO_TEXT_X, RUOKA_LAMPO_TEXT_Y, &txt_color, font_norm_bold_20px, 1);

   if (image_ptrs[IMAGE_VAIPPA_LAMPO_IKONI_W_45px])
      image_ptrs[IMAGE_VAIPPA_LAMPO_IKONI_W_45px]->render(VAIPPA_LAMPO_ICON_X, VAIPPA_LAMPO_ICON_Y);//Tämän viereen tulostetaan vaipan lämpötila

   i = live_params.cook_temps[TARGET_COOK_KETTLE_TEMP_IDX]; //Huom! mitattu temp = live_params.cook_temps !
   sprintf(buff, "%d °C", i);
   render_text(buff, VAIPPA_LAMPO_TEXT_X, VAIPPA_LAMPO_TEXT_Y, &txt_color, font_norm_bold_20px, 1);
}

#define IMAGE_VALKOINEN_AJASTUS_76px_X (20)
#define IMAGE_VALKOINEN_AJASTUS_76px_Y (326)

#define VALM_AIKA_TEXT_X (IMAGE_VALKOINEN_AJASTUS_76px_X)
#define VALM_AIKA_TEXT_Y (IMAGE_VALKOINEN_AJASTUS_76px_Y + 165)

//AIka osaston ikonit, extern int prog_curr_hours, prog_curr_minutes; //bannerissa oleva aika hh:mm, hh=00..23
void draw_kyps_jaahd_time_icons(void)
{
   char buff[50];
   char aika_buff[25];
   int left, h;
   SDL_Color txt_color = { 0xff, 0xff, 0xff, 0xff };

   if (image_ptrs[IMAGE_VALKOINEN_AJASTUS_76px])
      image_ptrs[IMAGE_VALKOINEN_AJASTUS_76px]->render(IMAGE_VALKOINEN_AJASTUS_76px_X, IMAGE_VALKOINEN_AJASTUS_76px_Y);

   strcpy(buff, (char*)yleis_tekstit[YLEIS_TXT_ARVIOITU_VALMISTUMIS_AIKA]);
   if (run_target_params.cook_mode)
   {
      if (run_target_params.temp_reached)
         left = (run_target_params.cook_time_left_seconds/60) + 1;
      else
         left = run_target_params.cook_time_minutes;
      left += ((prog_curr_hours * 60) + prog_curr_minutes);
      h = (left/60) % 24;
      left %= 60;
      if (h < 12)
         sprintf(aika_buff, " %02d:%02d am", h, left);
      else
         sprintf(aika_buff, " %02d:%02d pm", h-12, left);
      strcat(buff, aika_buff);
   }
   else if (run_target_params.chill_mode)
   {
      if (run_target_params.temp_reached)
         left = (run_target_params.chill_time_left_seconds/60) + 1;
      else
         left = run_target_params.chill_time_minutes;
      left += ((prog_curr_hours * 60) + prog_curr_minutes);
      h = (left/60) % 24;
      left %= 60;
      if (h < 12)
         sprintf(aika_buff, " %02d:%02d am", h, left);
      else
         sprintf(aika_buff, " %02d:%02d pm", h-12, left);
      strcat(buff, aika_buff);
   }
   render_text(buff, VALM_AIKA_TEXT_X, VALM_AIKA_TEXT_Y, &txt_color, font_norm_regular_20px);
}

#define IMAGE_SININEN_HANA_76px_X (20)
#define IMAGE_SININEN_HANA_76px_Y (565)

#define VESI_PYRAMIDI_ICON_X (IMAGE_SININEN_HANA_76px_X + 4)
#define VESI_PYRAMIDI_ICON_Y (IMAGE_SININEN_HANA_76px_Y + 75)

#define ASETUS_VESIMAARA_TEXT_X (VESI_PYRAMIDI_ICON_X + 8) /*Tämä ja sen yllä oleva pyramidi kuuluu yhteen*/
#define ASETUS_VESIMAARA_TEXT_Y (VESI_PYRAMIDI_ICON_Y + 55)

#define KOKONAIS_VESIMAARA_TEXT_X (IMAGE_SININEN_HANA_76px_X)
#define KOKONAIS_VESIMAARA_TEXT_Y (IMAGE_SININEN_HANA_76px_Y + 165)

//Vesi osaston ikonit
void draw_kyps_jaahd_water_icons(void)
{
   char buff[50];
   int i;
   SDL_Color txt_color = { 0xff, 0xff, 0xff, 0xff };

   if (image_ptrs[IMAGE_SININEN_HANA_76px])
      image_ptrs[IMAGE_SININEN_HANA_76px]->render(IMAGE_SININEN_HANA_76px_X, IMAGE_SININEN_HANA_76px_Y);

   if (run_target_params.water_fill_mode && (abs(run_target_params.water_fill_amount_litres - live_params.water_fill_amount_litres)))
   {
      if (image_ptrs[IMAGE_VALKOINEN_PYRAMIDI_58px]) //Pyramidi kuvaa mittausarvon nousua / laskua
         image_ptrs[IMAGE_VALKOINEN_PYRAMIDI_58px]->render(VESI_PYRAMIDI_ICON_X, VESI_PYRAMIDI_ICON_Y);

      i = run_target_params.water_fill_amount_litres; //asetus vesimäärä
      sprintf(buff, " %d l", i);
      render_text(buff, ASETUS_VESIMAARA_TEXT_X, ASETUS_VESIMAARA_TEXT_Y, &txt_color, font_norm_regular_20px, 1);
   }

   sprintf(buff, "%s:%d", (char*)yleis_tekstit[YLEIS_TXT_KOKONAISMAARA], live_params.water_fill_amount_total_litres);
   render_text(buff, KOKONAIS_VESIMAARA_TEXT_X, KOKONAIS_VESIMAARA_TEXT_Y, &txt_color, font_norm_regular_20px);
}

#define IMAGE_VALKOINEN_SEKOITUS_76px_X (20)
#define IMAGE_VALKOINEN_SEKOITUS_76px_Y (799)

#define IMAGE_MIXER_PAUSE_58px_Y (IMAGE_VALKOINEN_SEKOITUS_76px_Y+80)
#define IMAGE_MIXER_PAUSE_58px_X (30)

#define SEKOITUS_TEXT_X (IMAGE_VALKOINEN_SEKOITUS_76px_X)
#define SEKOITUS_TEXT_Y (IMAGE_VALKOINEN_SEKOITUS_76px_Y + 165)

//Mixer osaston ikonit
void draw_kyps_jaahd_mixer_icons(void)
{
   char buff[100];
   SDL_Color txt_color = { 0xff, 0xff, 0xff, 0xff };
   //Moodin mukaan vaihtuva ikoni mutta "img/58px" kansiossa on vain "Sekoitus W", onkohan väärästä paikasta ikoni otettu mutta mikä olisi oikea?
   if (image_ptrs[IMAGE_VALKOINEN_SEKOITUS_76px])
      image_ptrs[IMAGE_VALKOINEN_SEKOITUS_76px]->render(IMAGE_VALKOINEN_SEKOITUS_76px_X, IMAGE_VALKOINEN_SEKOITUS_76px_Y);

   if (alavalikko_naytolla && (live_params.state & TARGET_STATE_SOMETHING_STARTED_BIT))
      return;

   if (run_target_params.mixer_mode)
   {
      if (run_target_params.mixer_speed)
      {
         if (image_ptrs[IMAGE_PAUSE_58px])
            image_ptrs[IMAGE_PAUSE_58px]->render(IMAGE_MIXER_PAUSE_58px_X, IMAGE_MIXER_PAUSE_58px_Y);
      }
      else
      {
         if (image_ptrs[IMAGE_PLAY_58px])
            image_ptrs[IMAGE_PLAY_58px]->render(IMAGE_MIXER_PAUSE_58px_X, IMAGE_MIXER_PAUSE_58px_Y);
      }

      sprintf(buff, "%s: %s, %d", (char*)sekoitustapa_tekstit[SEKOITUSTAPA_TXT_SEKOITUSTAPA_JA_NOPEUS], get_sekoitus_tapa_txt(), run_target_params.mixer_speed);
      render_text(buff, SEKOITUS_TEXT_X, SEKOITUS_TEXT_Y, &txt_color, font_norm_regular_20px);
   }
}

/*======================S I D E B A R=======================*/
#define FULL_BASE_X 516
#define FULL_BASE_Y 96

//Sidebar center x pos = (535+128)
#define SIDEBAR_CENTER_X (535+128)

//#define SAANTO_JA_KOK_AIKA_TXT_X 575 //"Saanto 200kg" ja 
#define SAANTO_TXT_Y (146+22) //"Kokonaisaika 23min" sijoitetaan boxiin  eri riveille, center aligned
//#define SAANTO_JA_KOK_AIKA_TXT_W 178
#define KOK_AIKA_TXT_Y (SAANTO_TXT_Y+22)

#define EDISTYS_PALKKI_X 544
#define EDISTYS_PALKKI_Y 198

#define VAIHE_PAIKKA_X 535
#define VAIHE_PAIKKA_1_Y 255

//icons 1=Cook/chill, 2=Aika, 3=Vesi, 4=sek
#define ICON_1_3_X (VAIHE_PAIKKA_X+27) //Cook/CHill icon 
#define ICON_1_2_Y_OFFSET 28
#define ICON_2_4_X (VAIHE_PAIKKA_X+145)
#define ICON_3_4_Y_OFFSET 88
#define ICON_3_4_X_LISA_OFFSET 0 //24

#define TXT_X_OS 40
#define TXT_Y_3_4_OS 10

#define VAIHE_PAIKKA_2_Y (VAIHE_PAIKKA_1_Y+192)  //Tässä ei muistilappuja vielä ole otettu huomioon!
#define VAIHE_PAIKKA_3_Y (VAIHE_PAIKKA_2_Y+192)
#define VAIHE_PAIKKA_4_Y (VAIHE_PAIKKA_3_Y+192)

#define SEUR_VAIHE_ALKAA_TXT_Y 411

void show_sidebar(void)
{
   char buff[30];
   int ypos, kokaika;
   SDL_Color color = { 0x23, 0x23, 0x23, 0x0 };
   pyyhk_enabled = 0; // UP/Down pyyhkiminen estetty

   image_ptrs[IMAGE_SIVUPALKKI_FULL_BASE]->render(FULL_BASE_X, FULL_BASE_Y);

   if (tmp_header.Vaihe_lkm == 0)
      return;

   show_ohj_nimi(); //vaiheiden_luonti.cpp

   sprintf(buff, "%s %dkg", (char*)ruoka_valmis_tekstit[RUOKA_VALMIS_TXT_SAANTO], tuotanto_maara);
   render_text_center((char*)buff, SIDEBAR_CENTER_X, SAANTO_TXT_Y, (SDL_Color *)&color, font_norm_regular_20px);

   ypos = VAIHE_PAIKKA_1_Y;
   kokaika = 0;
   for (int i=0; i < tmp_header.Vaihe_lkm; i++)
   {
      if (i == current_prog_step)
         image_ptrs[IMAGE_SIVUPALKKI_COMB_FS]->render(VAIHE_PAIKKA_X, ypos); //Nykyinen vaihe, vihreä reuna + OK
      else
         image_ptrs[IMAGE_SIVUPALKKI_FOUR_SQ]->render(VAIHE_PAIKKA_X, ypos); //Seuraavat vaiheet

      if (tmp_steps[i].Step_type == STEP_TYPE_CHILL_STEP)
      {
         image_ptrs[IMAGE_SIVUPALKKI_ICON_CHILL]->render(ICON_1_3_X,   ypos + ICON_1_2_Y_OFFSET);
         sprintf(buff, "%d°C", tmp_steps[i].Temp1);
         render_text((char*)buff, ICON_1_3_X + TXT_X_OS, ypos + ICON_1_2_Y_OFFSET, (SDL_Color *)&color, font_norm_regular_25px, 1);
      }
      else if (tmp_steps[i].Step_type < STEP_TYPE_CHILL_STEP) //0..9 eri cook meiningit
      {
         image_ptrs[IMAGE_SIVUPALKKI_ICON_COOK]->render(ICON_1_3_X,   ypos + ICON_1_2_Y_OFFSET);
         sprintf(buff, "%d°C", tmp_steps[i].Temp1);
         render_text((char*)buff, ICON_1_3_X + TXT_X_OS, ypos + ICON_1_2_Y_OFFSET, (SDL_Color *)&color, font_norm_regular_25px, 1);
      }
      //else ...muistilaput
      if (tmp_steps[i].Aika)
      {
         image_ptrs[IMAGE_SIVUPALKKI_ICON_AIKA]->render(ICON_2_4_X,   ypos + ICON_1_2_Y_OFFSET);
         sprintf(buff, "%d:%02d", tmp_steps[i].Aika/60, tmp_steps[i].Aika%60);
         render_text((char*)buff, ICON_2_4_X + TXT_X_OS, ypos + ICON_1_2_Y_OFFSET, (SDL_Color *)&color, font_norm_regular_25px);
         kokaika += tmp_steps[i].Aika;
      }
      if (tmp_steps[i].Vesi)
      {
         image_ptrs[IMAGE_SIVUPALKKI_ICON_VESI]->render(ICON_1_3_X + ICON_3_4_X_LISA_OFFSET, ypos + ICON_3_4_Y_OFFSET);
         sprintf(buff, "%d l", tmp_steps[i].Vesi);
         render_text((char*)buff, ICON_1_3_X + ICON_3_4_X_LISA_OFFSET + TXT_X_OS, ypos + ICON_3_4_Y_OFFSET + TXT_Y_3_4_OS, (SDL_Color *)&color, font_norm_regular_25px);
      }
      if (tmp_steps[i].Sek_tapa)
      {
         image_ptrs[IMAGE_SIVUPALKKI_ICON_SEK]->render( ICON_2_4_X + ICON_3_4_X_LISA_OFFSET, ypos + ICON_3_4_Y_OFFSET + 5);
         sprintf(buff, "%d", tmp_steps[i].Nopeus);
         render_text((char*)buff, ICON_2_4_X + ICON_3_4_X_LISA_OFFSET + TXT_X_OS, ypos + ICON_3_4_Y_OFFSET + TXT_Y_3_4_OS, (SDL_Color *)&color, font_norm_regular_25px);
      }

      ypos += 192;
   }
   sprintf(buff, "%s %d min", (char*)ruoka_valmis_tekstit[RUOKA_VALMIS_TXT_KOKONAISAIKA], kokaika);
   render_text_center((char*)buff, SIDEBAR_CENTER_X, KOK_AIKA_TXT_Y, (SDL_Color *)&color, font_norm_regular_20px);

#if 0
   image_ptrs[IMAGE_SIVUPALKKI_FOUR_SQ]->render(VAIHE_PAIKKA_X, VAIHE_PAIKKA_2_Y); //Seuraavat vaiheet
   image_ptrs[IMAGE_SIVUPALKKI_ICON_CHILL]->render(ICON_1_3_X, VAIHE_PAIKKA_2_Y + ICON_1_2_Y_OFFSET);
   image_ptrs[IMAGE_SIVUPALKKI_ICON_AIKA]->render(ICON_2_4_X, VAIHE_PAIKKA_2_Y + ICON_1_2_Y_OFFSET);
   image_ptrs[IMAGE_SIVUPALKKI_ICON_VESI]->render(ICON_1_3_X + ICON_3_4_X_LISA_OFFSET, VAIHE_PAIKKA_2_Y + ICON_3_4_Y_OFFSET);
   image_ptrs[IMAGE_SIVUPALKKI_ICON_SEK]->render(ICON_2_4_X + ICON_3_4_X_LISA_OFFSET,  VAIHE_PAIKKA_2_Y + ICON_3_4_Y_OFFSET + 5);

   image_ptrs[IMAGE_SIVUPALKKI_FOUR_SQ]->render(VAIHE_PAIKKA_X, VAIHE_PAIKKA_3_Y);
   image_ptrs[IMAGE_SIVUPALKKI_ICON_COOK]->render(ICON_1_3_X, VAIHE_PAIKKA_3_Y + ICON_1_2_Y_OFFSET);
   image_ptrs[IMAGE_SIVUPALKKI_ICON_AIKA]->render(ICON_2_4_X, VAIHE_PAIKKA_3_Y + ICON_1_2_Y_OFFSET);
   image_ptrs[IMAGE_SIVUPALKKI_ICON_VESI]->render(ICON_1_3_X + ICON_3_4_X_LISA_OFFSET, VAIHE_PAIKKA_3_Y + ICON_3_4_Y_OFFSET);
   image_ptrs[IMAGE_SIVUPALKKI_ICON_SEK]->render(ICON_2_4_X + ICON_3_4_X_LISA_OFFSET,  VAIHE_PAIKKA_3_Y + ICON_3_4_Y_OFFSET + 5);

   image_ptrs[IMAGE_SIVUPALKKI_FOUR_SQ]->render(VAIHE_PAIKKA_X, VAIHE_PAIKKA_4_Y);
   image_ptrs[IMAGE_SIVUPALKKI_ICON_COOK]->render(ICON_1_3_X, VAIHE_PAIKKA_4_Y + ICON_1_2_Y_OFFSET);
   image_ptrs[IMAGE_SIVUPALKKI_ICON_AIKA]->render(ICON_2_4_X, VAIHE_PAIKKA_4_Y + ICON_1_2_Y_OFFSET);
   image_ptrs[IMAGE_SIVUPALKKI_ICON_VESI]->render(ICON_1_3_X + ICON_3_4_X_LISA_OFFSET, VAIHE_PAIKKA_4_Y + ICON_3_4_Y_OFFSET);
   image_ptrs[IMAGE_SIVUPALKKI_ICON_SEK]->render(ICON_2_4_X + ICON_3_4_X_LISA_OFFSET,  VAIHE_PAIKKA_4_Y + ICON_3_4_Y_OFFSET + 5);
#endif
}
