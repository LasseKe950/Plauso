/* 
   valmis.cpp
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
#include "kettle_functions.h"

#define VIHR_ALUE_X 0
#define VIHR_ALUE_Y 70
#define VIHR_ALUE_W 800
#define VIHR_ALUE_H 650

#define BUTTON_ALUE_Y (VIHR_ALUE_Y+VIHR_ALUE_H)
#define BUTTON_ALUE_H (BUTTON_ALUE_Y + (1280-1022)) //alavalikko y=1022

#define RUOKA_VALMIS_TXT_x 233  //Tämä teksti keskitetään ruudun keskelle
#define RUOKA_VALMIS_TXT_Y 130

#define RESEPTI_TXT_x 44
#define RESEPTI_TXT_Y 196

#define VALMIS_BUTTON1_X 72  //Button 1 -> Kyps=HACCP tai Jäähd=tyhjä
#define VALMIS_BUTTON1_Y 760

#define VALMIS_BUTTON2_X 443 //Button 2 -> Kyps=TALLENNA tai Jäähd=HACCP
#define VALMIS_BUTTON2_Y 760

#define VALMIS_BUTTON3_X 72  //Button 3 -> Kyps+Jööhd=JATKA
#define VALMIS_BUTTON3_Y 892

#define VALMIS_BUTTON4_X 443 //Button 4 -> Kyps=JÄÄHDYTÄ tai Jäähd=TALLENNE
#define VALMIS_BUTTON4_Y 892

#define VALMIS_BUTTONS_W 300
#define VALMIS_BUTTONS_H 82

int NUM_BUTTON_IDX_var;

void handle_exit_valmis(int but);

int saved_x, saved_y;
//HACCP=1, JATKA=2, TALLENNA=3, JÄÄHDYTÄ=4
void set_valmis_buttons(void)
{
   SDL_Color txt_color = { 0x00, 0x0, 0x00, 0xff };
   SDL_Color colorgreen = { 0x3D, 0xAF, 0, 0 };

   if (menu_to_return == ID_KYPSENNYS)
   {
      Button_group_add_button(0, VALMIS_BUTTON1_X, VALMIS_BUTTON1_Y, VALMIS_BUTTONS_W, VALMIS_BUTTONS_H, 10);
      Button_group_setText(0, YLEIS_TXT_HACCP, TXT_GROUP_YLEIS, txt_color, font_norm_regular_35px);
      Button_group_add_images(0, image_ptrs[IMAGE_HARMAA], nullptr);

      Button_group_add_button(1, VALMIS_BUTTON3_X, VALMIS_BUTTON3_Y, VALMIS_BUTTONS_W, VALMIS_BUTTONS_H, 10);
      Button_group_setText(1, YLEIS_TXT_JATKA_PALAA, TXT_GROUP_YLEIS, colorgreen, font_norm_regular_35px);
      Button_group_add_images(1, image_ptrs[IMAGE_VALKOINEN_VIHREAT_REUNAT], nullptr);

      Button_group_add_button(2, VALMIS_BUTTON2_X, VALMIS_BUTTON2_Y, VALMIS_BUTTONS_W, VALMIS_BUTTONS_H, 10);
      Button_group_setText(2, YLEIS_TXT_TALLENNA, TXT_GROUP_YLEIS, txt_color, font_norm_regular_35px);
      Button_group_add_images(2, image_ptrs[IMAGE_HARMAA], nullptr);

      Button_group_add_button(3, VALMIS_BUTTON4_X, VALMIS_BUTTON4_Y, VALMIS_BUTTONS_W, VALMIS_BUTTONS_H, 10);
      Button_group_setText(3, YLEIS_TXT_JAAHDYTYS, TXT_GROUP_YLEIS, txt_color, font_norm_regular_35px);
      Button_group_add_images(3, image_ptrs[IMAGE_HARMAA], nullptr);
   }
   else
   {
      Button_group_add_button(0, VALMIS_BUTTON2_X, VALMIS_BUTTON2_Y, VALMIS_BUTTONS_W, VALMIS_BUTTONS_H, 10);
      Button_group_setText(0, YLEIS_TXT_HACCP, TXT_GROUP_YLEIS, txt_color, font_norm_regular_35px);
      Button_group_add_images(0, image_ptrs[IMAGE_HARMAA], nullptr);

      Button_group_add_button(1, VALMIS_BUTTON3_X, VALMIS_BUTTON3_Y, VALMIS_BUTTONS_W, VALMIS_BUTTONS_H, 10);
      Button_group_setText(1, YLEIS_TXT_JATKA_PALAA, TXT_GROUP_YLEIS, colorgreen, font_norm_regular_35px);
      Button_group_add_images(1, image_ptrs[IMAGE_VALKOINEN_VIHREAT_REUNAT], nullptr);

      Button_group_add_button(2, VALMIS_BUTTON4_X, VALMIS_BUTTON4_Y, VALMIS_BUTTONS_W, VALMIS_BUTTONS_H, 10);
      Button_group_setText(2, YLEIS_TXT_TALLENNA, TXT_GROUP_YLEIS, txt_color, font_norm_regular_35px);
      Button_group_add_images(2, image_ptrs[IMAGE_HARMAA], nullptr);
   }

}

char datebuff[30];
char timebuff[30];
static struct tm curr_time;

void save_valmis_aika(void)
{
   time_t mynow;
	time(&mynow);

	curr_time = *localtime(&mynow);
//	sprintf(datebuff, "%02d-%02d-%04d ", curr_time.tm_mon+1, curr_time.tm_mday, curr_time.tm_year+1900);
	sprintf(datebuff, "%02d/%02d/%04d ", curr_time.tm_mday, curr_time.tm_mon+1, curr_time.tm_year+1900);
	sprintf(timebuff, "%02d:%02d", curr_time.tm_hour, curr_time.tm_min);	//curr_time.tm_year+1900
   strcat(datebuff, timebuff);
}

#define Y_ADD 40
void show_valmis_texts(void)
{
   int y;
   SDL_Color txt_color = { 0xff, 0xff, 0xff, 0xff };
   char buff[50];
   strcpy(buff, (char*)ruoka_valmis_tekstit[RUOKA_VALMIS_TXT_RUOKA_ON_VALMIS]);
   render_text(buff, RUOKA_VALMIS_TXT_x, RUOKA_VALMIS_TXT_Y, &txt_color, font_norm_regular_35px);

   strcpy(buff, (char*)ruoka_valmis_tekstit[RUOKA_VALMIS_TXT_RESEPTI]);
   strcat(buff, ": ");
   render_text(buff, RESEPTI_TXT_x, RESEPTI_TXT_Y, &txt_color, font_norm_regular_25px);

   y = RESEPTI_TXT_Y + Y_ADD;
   strcpy(buff, (char*)ruoka_valmis_tekstit[RUOKA_VALMIS_TXT_ERANUMERO]);
   render_text(buff, RESEPTI_TXT_x, y, &txt_color, font_norm_regular_25px);

   //pvm ja kelloaika
   y += Y_ADD;
   render_text(datebuff, RESEPTI_TXT_x, y, &txt_color, font_norm_regular_25px);

   y += Y_ADD;
   strcpy(buff, (char*)ruoka_valmis_tekstit[RUOKA_VALMIS_TXT_RESEPTIN_SAANTO]);
   render_text(buff, RESEPTI_TXT_x, y, &txt_color, font_norm_regular_25px);

   y += Y_ADD;
   strcpy(buff, (char*)ruoka_valmis_tekstit[RUOKA_VALMIS_TXT_PAINO]);
   render_text(buff, RESEPTI_TXT_x, y, &txt_color, font_norm_regular_25px);

   y += Y_ADD;
   strcpy(buff, (char*)ruoka_valmis_tekstit[RUOKA_VALMIS_TXT_OHJELMAN_KOKONAISAIKA]);
   render_text(buff, RESEPTI_TXT_x, y, &txt_color, font_norm_regular_25px);

   y += Y_ADD;
   strcpy(buff, (char*)ruoka_valmis_tekstit[RUOKA_VALMIS_TXT_JAAHDYTYSAIKA]);
   render_text(buff, RESEPTI_TXT_x, y, &txt_color, font_norm_regular_25px);

   y += Y_ADD;
   strcpy(buff, (char*)ruoka_valmis_tekstit[RUOKA_VALMIS_TXT_JAAHDYTYSAIKA]); //Aika HAccp rajalla
   render_text(buff, RESEPTI_TXT_x, y, &txt_color, font_norm_regular_25px);

   y += Y_ADD;
   strcpy(buff, (char*)ruoka_valmis_tekstit[RUOKA_VALMIS_TXT_KOKONAISVESIMAARA]);
   render_text(buff, RESEPTI_TXT_x, y, &txt_color, font_norm_regular_25px);

   y += Y_ADD;
   y += Y_ADD;
   strcpy(buff, (char*)ruoka_valmis_tekstit[RUOKA_VALMIS_TXT_ENERGIAN_KULUTUS]);
   render_text(buff, RESEPTI_TXT_x, y, &txt_color, font_norm_regular_25px);

}

void draw_VALMIS(int param1)
{
   //vihreä alue
   boxRGBA(gRenderer, VIHR_ALUE_X, VIHR_ALUE_Y, VIHR_ALUE_X+VIHR_ALUE_W, VIHR_ALUE_Y+VIHR_ALUE_H, 0x3D, 0xAF, 0x00, 0xFF);
   //button alue
   boxRGBA(gRenderer, 0, BUTTON_ALUE_Y, 800, 1021, 0xf4, 0xf4, 0xf4, 0xFF); //button alue alavalikkoon asti, alavalikko y=1022

   show_valmis_texts();

   if (menu_to_return == ID_KYPSENNYS)
      NUM_BUTTON_IDX_var = 4;

   else
      NUM_BUTTON_IDX_var = 3;

   show_ala_kahva();

   for (int i=0; i< NUM_BUTTON_IDX_var; i++)
   {
      Button_group_render(i);
   }

   show_banner(NUM_BUTTON_IDX_var);
   if (alavalikko_naytolla)
      nayta_alavalikko(NUM_BUTTON_IDX_var+1, ala_painike_painettu);
}

void enter_VALMIS_pages(void)
{
   save_valmis_aika();
   alavalikko_naytolla = 1;
   set_valmis_buttons();
   draw_VALMIS(0);
}

int handler_VALMIS(int x, int y,  SDL_Event *e)
{
   if( e->type == SDL_MOUSEMOTION)
   {
      if (check_for_alavalikko(x, y, NUM_BUTTON_IDX_var+1))
         draw_VALMIS(0);
      ala_painike_painettu = button_pressed = 0;
   }
   else
   {
      if(e->type == SDL_MOUSEBUTTONDOWN)
      {
         ala_painike_painettu = button_pressed = 0;
         if ((x==-1) && (y==-1))
            set_valmis_buttons(); //Tuli kansi auki: päivitetään kaikki"
         int b = Button_group_check_for_key_press(x, y);
         if ((b >= CODE_KALLISTUS_ALAS && b <= CODE_KALLISTUS_YLOS))
            ala_painike_painettu = b;
         else if (b)
         {
            if (b == CODE_DIETA_LOGO)
            {
               handle_exit_valmis(b);
               return ID_KOTINAKYMA;
            }
            button_pressed = b;
         }
         else
         {
            saved_x = x;
            saved_y = y;
            if (saved_y < (VIHR_ALUE_Y + VIHR_ALUE_H))
               button_pressed = CODE_ISO_ALUE_PAINETTU;
         }
      }
      else if(e->type == SDL_MOUSEBUTTONUP)
      {
         if (button_pressed)
         {
            printf("Button %d Released!\n", button_pressed); //HACCP=1, JATKA=2, TALLENNA=3, JÄÄHDYTÄ=4
            if ((button_pressed == 2) && (menu_to_return < NUM_MENU_IDS))
            {
               if (menu_to_return == ID_KYPSENNYS)
               {
                  run_target_params.cook_time_minutes = 10;
                  run_target_params.cook_time_left_seconds = run_target_params.cook_time_minutes*60;
                  run_target_params.state |= TARGET_STATE_COOK_TEMP_CHANGED_BIT; //Jatketaan keittoa 10min
               }
               else if (menu_to_return == ID_JAAHDYTYS)
               {
                  run_target_params.state |= TARGET_STATE_CHILL_TEMP_CHANGED_BIT; //Jatketaan jäähd. 10min
                  run_target_params.chill_time_minutes = 10;
                  run_target_params.chill_time_left_seconds = run_target_params.chill_time_minutes*60;
               }
               return menu_to_return;
            }
            else if (button_pressed == 4)
            {
               if (menu_to_return == ID_KYPSENNYS)
               {
                  set_cooking(0); //Pitäis varmaan tallettaa suoritettu kypsennysvaihe ohjelmavaiheeksi
                  set_chilling(0);
                  return ID_JAAHDYTYS;
               }
               else
                  return ID_KOTINAKYMA; //Ei ole vielä tallenna toimintoja!
            }
            else
            {
               handle_exit_valmis(button_pressed);
               return ID_KOTINAKYMA; //Ei ole vielä haccp/tallenna toimintoja!
            }
         }
         ala_painike_painettu = button_pressed = 0;
      }
      draw_VALMIS(0);
   }
   return 0xffff;
}

void init_VALMIS(void)
{
}

void show_VALMIS(int show_hide)
{
   if (show_hide == 0)
   {
      return;
   }
   pyyhk_enabled = 1;
   ala_painike_painettu = button_pressed = 0;
   Button_group_empty();
   enter_VALMIS_pages();
}

void handle_exit_valmis(int but)
{
   if (menu_to_return == ID_KYPSENNYS)
      cooking_is_ready(0); //1=hold temperature
   else if (menu_to_return == ID_JAAHDYTYS)
      chilling_is_ready(0); //1=hold temperature
}
