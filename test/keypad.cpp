/* 
   keypad.cpp
   Näppäimistö.

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

void handle_keypad_button_press(int but);

const unsigned char key_taulu1[32] = {
   'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', 0xC5,
   'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 0xD6, 0xC4, 
   '-', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.'
};

char keypad_string[100];

#define ISO_ALUE_X1 26
#define ISO_ALUE_Y1 500
#define ISO_ALUE_X2 (26+737)
#define ISO_ALUE_Y2 (513+290)

static int key_outside_iso_alue(int x, int y)
{
   if ((x < ISO_ALUE_X1) || (x > ISO_ALUE_X2) || (y < ISO_ALUE_Y1) || (y > ISO_ALUE_Y2))
      return 1;
   else
      return 0;
}

void draw_KEYPAD(int param1)
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
                   47, //border radius
                   c);
   SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_NONE);

   int key_idx=0;
   int nx;
   int ny = 533;

   for (int j=0; j<4; j++)  //Näppäimet 3 * 11kpl
   {
      nx = 46;
      for (int i=0; i< 11; i++)
      {
         if (key_idx == 35) //Iso alue spacelle
            Button_group_add_button(but_idx, nx, ny, 70 + ((110-46)*6), 70);
         else
            Button_group_add_button(but_idx, nx, ny, 70, 70);
         if (key_idx < 32)
         {
            buff[0] = key_taulu1[key_idx];
            buff[1] = 0;
            Button_group_setText(but_idx, buff, txt_color, font_norm_bold_42px);
            Button_group_add_images(but_idx, image_ptrs[IMAGE_KIRJAIN_POHJA + (nappi_pohjassa==(but_idx+1))], 0);
         }
         else if (key_idx == 32)
         {
            Button_group_add_images(but_idx, image_ptrs[IMAGE_KIRJAIN_BACKSPACE + (nappi_pohjassa==(but_idx+1))], image_ptrs[IMAGE_MUSTA_RETURN_32px]);
         }
         else if (key_idx == 33)
         {
            Button_group_setText(but_idx, (char*)"shift", txt_color, font_norm_bold_20px);
            Button_group_add_images(but_idx, image_ptrs[IMAGE_KIRJAIN_POHJA + (nappi_pohjassa==(but_idx+1))], 0);
         }
         else if (key_idx == 34)
         {
            Button_group_setText(but_idx, (char*)".?12", txt_color, font_norm_bold_20px);
            Button_group_add_images(but_idx, image_ptrs[IMAGE_KIRJAIN_POHJA + (nappi_pohjassa==(but_idx+1))], 0);
         }
         else if (key_idx == 35)
         {
            Button_group_add_images(but_idx, image_ptrs[IMAGE_KIRJAIN_SPACE + (nappi_pohjassa==(but_idx+1))], 0);
            nx += ((110-46)*6);
         }
         else if (key_idx == 36)
         {
            Button_group_setText(but_idx, (char*)"ENTER", txt_color, font_norm_bold_20px);
            Button_group_add_images(but_idx, image_ptrs[IMAGE_KIRJAIN_POHJA + (nappi_pohjassa==(but_idx+1))], 0);
         }
         else if (key_idx == 37)
         {
            Button_group_setText(but_idx, (char*)"OK", txt_color_w, font_norm_bold_20px);
            Button_group_add_images(but_idx, image_ptrs[IMAGE_KIRJAIN_OK], 0);
         }

         Button_group_render(but_idx++);

         nx += (110-46);

         if (++key_idx >= 38)
            break;
      }
      ny += 598 - 533;
      if (key_idx >= 38)
         break;
   }

   SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_NONE);

   if (param1 == 0)
      show_banner(but_idx);
}

int handler_KEYPAD(int x, int y,  SDL_Event *e)
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
         if (b > 0)
         {
            printf("Button %d Pressed!\n", b);
            if (b == 38) //OK
            {
               return menu_to_return;
            }
            nappi_pohjassa = b;
            handle_keypad_button_press(--b);
         }
         else if (key_outside_iso_alue(x, y))
            return menu_to_return;
      }
      else if (e->type == 0) //Timeout
      {
      }
      draw_KEYPAD(0);
   }
   return 0xffff;
}

void init_KEYPAD(void)
{
}

void show_KEYPAD(int show_hide)
{
   if (show_hide == 0)
   {
      return;
   }
   nappi_pohjassa = 0;
   pyyhk_enabled = 0;
   Button_group_empty();
   //sprintf(numpad_string, "%d", numpad_parameter_val);
   memset(keypad_string, 0, sizeof(numpad_string));
   if (menu_to_return >= NUM_MENU_IDS)
      menu_to_return = ID_KOTINAKYMA;
   draw_KEYPAD(0);
}

void handle_keypad_button_press(int but)
{
   char ch[2];
   ch[0] = 0;
   if (but < 32)
   {
      ch[0] = key_taulu1[but];
   }
   else if (but == 32) //back
   {
      if (strlen(keypad_string))
         keypad_string[strlen(keypad_string)-1] = 0;
   }
   else if (but == 33) //shift
   {
   }
   else if (but == 34) //.?12 ei vielä tämmöstä ole
   {
   }
   else if (but == 35) //space
   {
      ch[0] = ' ';
   }
   ch[1] = 0;
   if (strlen(keypad_string) < 50)
      strcat(keypad_string, ch);
//   printf("keypad_string:%s\n", keypad_string);
   uusi_ohjelma_nimi_annetaan();
}
