/* 
   numpad.cpp
   numeronäppis jolla annetaan target parametrien arvoja.
   roundedBoxRGBA / roundedBoxColor tekee isomman pyöristetyn suorakulmion kuin roundedRectangleRGBA..?
   Jälkimmäinen siis joka ei täytä aluetta.

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

unsigned char mod_tst;
void dim_numero_kentta(int i);

void nayta_numpad_tausta(void);

void handle_numpad_button_press(int but);
char numpad_string[20];
char numpad_apu_string[20];
char numpad_time_string[10]; //näytetään aika muodossa hh:mm
int check_numpad_val;
int numpad_selected_temp_mode;

const int temp_mode_icons[5]={
   IMAGE_RUOKA_LAMPO_IKONI_B_45px,
   IMAGE_VAIPPA_LAMPO_IKONI_B_45px,
   IMAGE_DELTA_LAMPO_IKONI_B_45px,
   IMAGE_SENSOR_LAMPO_IKONI_B_45px,
   IMAGE_SININEN_JAAHDYTYS_76px,
};

#define TEMP_MODE_BUTTONS_Y 344

#define TEMP_MODE_BUTTON1_X 162
#define TEMP_MODE_BUTTON2_X 260
#define TEMP_MODE_BUTTON3_X 359
#define TEMP_MODE_BUTTON4_X 458
#define TEMP_MODE_BUTTON5_X 556

#define ISO_ALUE_X1 137
#define ISO_ALUE_Y1 144
#define ISO_ALUE_X2 (137+526)
#define ISO_ALUE_Y2 (144+992)
int key_outside_iso_alue(int x, int y)
{
   if ((x < ISO_ALUE_X1) || (x > ISO_ALUE_X2) || (y < ISO_ALUE_Y1) || (y > ISO_ALUE_Y2))
      return 1;
   else
      return 0;
}

void draw_NUMPAD(int param1)
{
   char buff[20];
   int but_idx=0;
   SDL_Color txt_color = { 0x00, 0x00, 0x00, 0xff };
   SDL_Color txt_color_w = { 0xff, 0xff, 0xff, 0xff };
   SDL_Color txt_color_g = { 0x80, 0x80, 0x80, 0xff };

   //last par: color The color value of the rectangle to draw (0xRRGGBBAA).
   Uint32 c;
   c = 0xFF<<24 | (0xF4<<16) | (0xF4<<8) | (0xF4); //bgr
   roundedBoxColor(gRenderer, //ISO ALUE
                   ISO_ALUE_X1, ISO_ALUE_Y1, //x1 + y1
                   ISO_ALUE_X2, ISO_ALUE_Y2, //x2 + y2
                   104, //border radius
                   c);
   SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_NONE);

   c = 0xFF<<24 | (0xFF<<16) | (0xFF<<8) | (0xFF); //bgr
   roundedBoxColor(gRenderer, //YLHÄÄLLÄ OLEVA NUMEROKENTTÄ
                   175, 174, //x1 + y1
                   175 + 454, 174 + 130, //x2 + y2
                   65, //border radius
                   c);
   SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_NONE);

   strcpy(buff, numpad_string);
   if (numpad_parameter_unit == NUMPAD_UNIT_C)
      strcat(buff, "\xB0""C");
   else if (numpad_parameter_unit == NUMPAD_UNIT_L)
      strcat(buff, " l");
   else if (numpad_parameter_unit == NUMPAD_UNIT_HH_MM)
   {
      for (int i=0, j=3; i < 4; i++)
      {
         if (numpad_string[i] == 0)
            numpad_time_string[j]='0';
         else
            numpad_time_string[j]=numpad_string[i];
         if (++j == 5)
            j = 0;
      }
      numpad_time_string[2]=':';
      numpad_time_string[5]=0;
      if (numpad_time_string[3] > '5') //kymmenet minuutit
         numpad_time_string[3] = '5';
      strcpy(buff, numpad_time_string); 
         printf("%s\n", buff);
   }
   Button_group_add_button(but_idx, 175, 174, 454, 130);
   Button_group_setText(but_idx, buff, txt_color, font_norm_bold_80px);
   Button_group_render(but_idx++); //Button 0 = Numerokenttä!

   int nx;
   int ny = 470 + 145/2;

   for (int j=0; j<4; j++)  //Näppäimet 12kpl
   {
      nx = 175 + 145/2;
      for (int i=0; i< 3; i++)
      {
         Button_group_add_button(but_idx, nx-72, ny-72, 2*72, 2*72);
         //Button 0 = Numerokenttä!
         if (but_idx < 10)
            sprintf(buff, "%d", but_idx);
         else if (but_idx == 10)
            strcpy(buff, "OK");
         else if (but_idx == 11)
            strcpy(buff, "0");
         else
            strcpy(buff, " ");
         if (but_idx == 10)
            Button_group_setText(but_idx, buff, txt_color_w, font_norm_bold_70px);
         else
            Button_group_setText(but_idx, buff, txt_color, font_norm_bold_80px);
         if (but_idx == 10)
            Button_group_add_images(but_idx, image_ptrs[IMAGE_OK_1], 0);
         else if (but_idx == 12)
         {
            Button_groups[but_idx].icon_image_pos.x = -4;
            //Button_groups[but_idx].icon_image_pos.y = (nappi_pohjassa==but_idx) ? -3 : 0;
            Button_group_add_images(but_idx, image_ptrs[IMAGE_BACKSPACE + (nappi_pohjassa==but_idx)], image_ptrs[IMAGE_VALKOINEN_RETURN_76px]);
         }
         else
            Button_group_add_images(but_idx, image_ptrs[IMAGE_NUMERO_POHJA + (nappi_pohjassa==but_idx)], 0);
         Button_group_render(but_idx++);

         nx += 335-175;
      }
      ny += 629-470;
   }

   if (numpad_parameter_unit == NUMPAD_UNIT_HH_MM)
   {
      dim_numero_kentta(mod_tst);
   }

//   numpad_selected_temp_mode = 0; //test

   if ((numpad_target_parameter == NUMPAD_TARGET_FOODTEMP) || (numpad_target_parameter == NUMPAD_TARGET_CHILLTEMP)) //ENtäs jäähdytys?
   {
      nx = TEMP_MODE_BUTTON1_X + (96/2);
      ny = TEMP_MODE_BUTTONS_Y + (96/2);
      for (int j=0; j<5; j++)  //temperature-mode buttonit 5kpl max (buttonin circle 81x81 px)
      {
         Button_group_add_button(but_idx, nx-(96/2), ny-(96/2), (96), (96));

         sprintf(buff, "%d", but_idx);

         if (j==numpad_selected_temp_mode)                                                    //j=4 jäähdytys ikonit ??? puuttuu ???
            Button_group_add_images(but_idx, image_ptrs[IMAGE_TEMP_VALINTA_LISATOIMINTO_VALITTU], image_ptrs[temp_mode_icons[j]]); //Musta valkoisella taustalla jos selected
         else
            Button_group_add_images(but_idx, image_ptrs[IMAGE_TEMP_VALINTA_LISATOIMINTO], image_ptrs[temp_mode_icons[j] + 1]); //+1 --> White jos ei selected
         Button_group_render(but_idx++);
         nx += 100;
      }
   }
   SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_NONE);

   if (param1 == 0)
      show_banner(but_idx);
}

void enter_NUMPAD_pages(void)
{
   draw_NUMPAD(0);
}

int handler_NUMPAD(int x, int y,  SDL_Event *e)
{
   nappi_pohjassa = 0;
   if( e->type == SDL_MOUSEMOTION)
   {
   }
   else
   {
      if(e->type == SDL_MOUSEBUTTONDOWN)
      {
//         printf("x=%d, y=%d\n", x, y);
         int b = Button_group_check_for_key_press(x, y);
         if (b == CODE_DIETA_LOGO)
            return ID_KOTINAKYMA;
         if (b > 1) //Button 0 = numerokenttä
         {
//            printf("Button %d Pressed!\n", b);
            --b;
            if (b == 10) //OK
            {
               if (numpad_parameter_unit == NUMPAD_UNIT_HH_MM)
               {//numpad_time_string "hh:mm"
                  int apu = numpad_time_string[0]-'0';
                  apu *= 600;
                  apu += ((numpad_time_string[1]-'0')*60);
                  apu += ((numpad_time_string[3]-'0')*10);
                  apu += numpad_time_string[4]-'0';
                  numpad_parameter_val = apu;
                  numpad_ok_pressed();
               }
               else if (sscanf(numpad_string, "%d", &numpad_parameter_val) == 1)
                  numpad_ok_pressed();
               return menu_to_return;
            }
            handle_numpad_button_press(b);
         }
         else if (key_outside_iso_alue(x, y))
            return menu_to_return;
      }
      else if (e->type == 0) //Timeout
      {
         if (++timeout_ctr > 10) //5Sec
            nayta_mitatut_arvot = 1;
      }
      draw_NUMPAD(0);
   }
   return 0xffff;
}

void init_NUMPAD(void)
{
}

void show_NUMPAD(int show_hide)
{
   if (show_hide == 0)
   {
      return;
   }
   mod_tst = 0;
   nappi_pohjassa = 0;
   pyyhk_enabled = 0;
   Button_group_empty();
   //sprintf(numpad_string, "%d", numpad_parameter_val);
   memset(numpad_string, 0, sizeof(numpad_string));
   memset(numpad_apu_string, 0, sizeof(numpad_apu_string));
   strcpy(numpad_time_string, "00:00");
   enter_NUMPAD_pages();
   if (menu_to_return >= NUM_MENU_IDS)
      menu_to_return = ID_KOTINAKYMA;
}

void check_numpad_value(char *buf)
{
   strcpy(numpad_apu_string, numpad_string);
   strcat(numpad_apu_string, buf);
   if (numpad_parameter_unit == NUMPAD_UNIT_HH_MM) //Ei hyväksytä minuuttikymmeniin kuin 0..5
   {
      if (numpad_apu_string[2] > '5')
         return;
   }

   if (sscanf(numpad_apu_string, "%d", &check_numpad_val) == 1)
   {
      if ((check_numpad_val >= numpad_parameter_min_value) && (check_numpad_val <= numpad_parameter_max_value))
         strcpy(numpad_string, numpad_apu_string);
      else
         strcpy(numpad_apu_string, numpad_string);
    }
//   printf("%s\n", numpad_string);
}

void handle_numpad_button_press(int but)
{
   char buf[20];
   if (but < 10)
   {
      if (strlen(numpad_string) >= 4)
         return;
      sprintf(buf, "%d", but);
      if ((numpad_string[0] == '0') && (numpad_parameter_unit != NUMPAD_UNIT_HH_MM))
         numpad_string[0] = 0;
      check_numpad_value(buf);
   }
   else if (but==11) //(but == 11) && ((numpad_string[0] == '0') && (numpad_parameter_unit != NUMPAD_UNIT_HH_MM)))
   { //11 = nolla
      if (strlen(numpad_string) >= 4)
         return;
      sprintf(buf, "%d", 0);
      check_numpad_value(buf);
   }
   else if (but == 12) //pyyhi
   {
      if (numpad_parameter_unit == NUMPAD_UNIT_HH_MM)
      {
         mod_tst = 0;
         strcpy(numpad_time_string, "00:00");
         memset(numpad_string, 0, sizeof(numpad_string));
         memset(numpad_apu_string, 0, sizeof(numpad_apu_string));
      }
      else if (strlen(numpad_string))
         numpad_string[strlen(numpad_string)-1] = 0;
   }
   else
   {
      if (numpad_parameter_unit == NUMPAD_UNIT_C)
      {
         if ((but >= 13) && (but <= 17)) //13=Norm.COOK, 14=vaippaohjaus, 15=delta, 16=ext temp, 17=Chill
         {
            if (menu_to_return == ID_VAIHEIDEN_LUONTI) //Ei toimi vielä kuin vaiheitten luonnissa!
            {
               numpad_selected_temp_mode = but - 13;
               numpad_updates_vaiheen_selected_temp_mode_parametri();
            }
         }
      }
   }
   nappi_pohjassa = but;
}

//   Button_group_add_button(but_idx, 175, 174, 454, 130);
void dim_numero_kentta(int par)
{
   //lätkästää osittain läpinäkyvä w layer nykynäytön päälle
   SDL_SetRenderDrawColor(gRenderer, 0xff, 0xff, 0xff, 0xB0);
   SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
   SDL_Rect rect;
   if (par==0)
      rect.x = 290;
   else
      rect.x = 410;
   rect.y = 195;
   rect.w = 100;
   rect.h = 80;
   SDL_RenderFillRect(gRenderer, &rect);
   SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_NONE);
}
