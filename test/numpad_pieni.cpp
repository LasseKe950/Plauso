/* 
   numpad_pieni.cpp
   numeronäppis jolla annetaan numero arvoja, esim salasana.

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

void handle_numpad2_button_press(int but);

#define ISO_ALUE_X1 210
#define ISO_ALUE_Y1 579
#define ISO_ALUE_X2 (210+395)
#define ISO_ALUE_Y2 (579+649)

static int key_outside_iso_alue(int x, int y)
{
   if ((x < ISO_ALUE_X1) || (x > ISO_ALUE_X2) || (y < ISO_ALUE_Y1) || (y > ISO_ALUE_Y2))
      return 1;
   else
      return 0;
}

void draw_NUMPAD_PIENI(int param1)
{
   char buff[20];
   int but_idx=0;
   SDL_Color txt_color = { 0x00, 0x00, 0x00, 0xff };
   SDL_Color txt_color_w = { 0xff, 0xff, 0xff, 0xff };

   //last par: color The color value of the rectangle to draw (0xRRGGBBAA).
   Uint32 c;
   c = 0xFF<<24 | (0xF4<<16) | (0xF4<<8) | (0xF4); //bgr
   roundedBoxColor(gRenderer, //ISO ALUE
                   ISO_ALUE_X1, ISO_ALUE_Y1, //x1 + y1
                   ISO_ALUE_X2, ISO_ALUE_Y2, //x2 + y2
                   38, //border radius
                   c);
   SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_NONE);

   c = 0xFF<<24 | (0xFF<<16) | (0xFF<<8) | (0xFF); //bgr
   roundedBoxColor(gRenderer, //YLHÄÄLLÄ OLEVA NUMEROKENTTÄ
                   230, 598, //x1 + y1
                   230 + 358, 598 + 106, //x2 + y2
                   55, //border radius 65
                   c);
   SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_NONE);

   if (strlen(numpad_string) == 0)
      strcpy(buff, " ");
   else
   {
      if (menu_to_return == ID_PROF_VAIHTO) //Salasanaa annetaan
      {
         Uint8 i;
         for (i=0; i<strlen(numpad_string); i++)
            buff[i] = '*';
         buff[i] = 0;
      }
      else
         strcpy(buff, numpad_string);
   }

   Button_group_add_button(but_idx, 230, 598, 358, 106);
   Button_group_setText(but_idx, buff, txt_color, font_norm_bold_80px);
   Button_group_render(but_idx++); //Button 0 = Numerokenttä!

   int nx;
   int ny = 724;

   for (int j=0; j<4; j++)  //Näppäimet 12kpl 107x107 107/2=53
   {
      nx = 230;
      for (int i=0; i< 3; i++)
      {
         Button_group_add_button(but_idx, nx, ny, 2*53, 2*53);
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
            Button_group_setText(but_idx, buff, txt_color_w, font_norm_bold_56px);
         else
            Button_group_setText(but_idx, buff, txt_color, font_norm_bold_62px);
         if (but_idx == 10)
            Button_group_add_images(but_idx, image_ptrs[IMAGE_PIENI_OK_1], 0);
         else if (but_idx == 12)
         {
            Button_groups[but_idx].icon_image_pos.y = 4;
            //Button_groups[but_idx].icon_image_pos.y = (nappi_pohjassa==but_idx) ? -3 : 0;
            Button_group_add_images(but_idx, image_ptrs[IMAGE_PIENI_BACKSPACE + (nappi_pohjassa==but_idx)], image_ptrs[IMAGE_VALKOINEN_RETURN_76px]);
         }
         else
            Button_group_add_images(but_idx, image_ptrs[IMAGE_PIENI_NUMERO_POHJA + (nappi_pohjassa==but_idx)], 0);
         Button_groups[but_idx].button_txt_x_offset = 6;
         Button_groups[but_idx].button_txt_y_offset = 10;
         Button_group_render(but_idx++);

         nx += ((356-230) - 2); //Buttoneitten välimatka = 2.buttonin x - 1.buttonin x
      }
      ny += (850-724) -2;
   }
}

int handler_NUMPAD_PIENI(int x, int y,  SDL_Event *e)
{
   nappi_pohjassa = 0;
   if( e->type == SDL_MOUSEMOTION)
   {
   }
   else
   {
      if(e->type == SDL_MOUSEBUTTONDOWN)
      {
         int b = Button_group_check_for_key_press(x, y);
         if (b == CODE_DIETA_LOGO)
            return ID_KOTINAKYMA;
         if (b > 1) //Button 0 = numerokenttä
         {
//            printf("Button %d Pressed!\n", b);
            --b;
            if (b == 10) //OK
            {
               if (menu_to_return == ID_PROF_VAIHTO) //Salasanaa annetaan
                  prof_salasana_annettiin();
               else if (menu_to_return == ID_UUSI_OHJELMA) //Eräkoko
                  uusi_ohjelma_erakoko_annettiin();
               else if (menu_to_return == ID_SYOTA_ALOITA_TIEDOT)
                  syota_aloita_tiedot_annettiin();
               return menu_to_return;
            }
            handle_numpad2_button_press(b);
         }
         else if (key_outside_iso_alue(x, y))
            return menu_to_return;
      }
      draw_NUMPAD_PIENI(0);
   }
   return 0xffff;
}

void init_NUMPAD_PIENI(void)
{
}

void show_NUMPAD_PIENI(int show_hide)
{
   if (show_hide == 0)
   {
      return;
   }
   nappi_pohjassa = 0;
   pyyhk_enabled = 0;
   Button_group_empty();
   memset(numpad_string, 0, sizeof(numpad_string));
   draw_NUMPAD_PIENI(0);
   if (menu_to_return >= NUM_MENU_IDS)
      menu_to_return = ID_KOTINAKYMA;
}

void handle_numpad2_button_press(int but)
{
   char buf[20];
   if (but < 10)
   {
      if (strlen(numpad_string) >= 4)
         return;
      sprintf(buf, "%d", but);
      strcat(numpad_string, buf);
   }
   else if (but==11) //(but == 11) && ((numpad_string[0] == '0') && (numpad_parameter_unit != NUMPAD_UNIT_HH_MM)))
   { //11 = nolla
      if (strlen(numpad_string) >= 4)
         return;
      sprintf(buf, "%d", 0);
      strcat(numpad_string, buf);
   }
   else if (but == 12) //pyyhi
   {
      if (strlen(numpad_string))
         numpad_string[strlen(numpad_string)-1] = 0;
   }
   nappi_pohjassa = but;
}
