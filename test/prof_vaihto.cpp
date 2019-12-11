/* 
   prof_vaihto.cpp
   Käyttäjäprofiilin vaihto dialogi. Tämä on erilainen kuin speksissä, tässä pohja on kuten "Käyttäjäprofiilin vaihto" screenissä. Tämä ei ole ns "dialogi".
   Ajatus on että tätä rakennetta voi käyttää kaikissa samantyyppisissä numeronsyöttö-menuissa. Kun painaa salasana kohtaa niin 
   näytölle tulee pikkunumeronäppis. Speksin mukaisessa ongelma oli että prof-vaihto-näkymä on itsessään jo dialogi joka pimentää muun osan taustasta
   ja siihen pitäs numeronäppis vielä saada...kyllä senkin voi tehdä sitten tarvittaessa.

   -iso rectangle x=0, y=70, w=800, h=1005-70, color=F4F4F4
   -"Käyttäjäprofiili" teksti, x=16, y=93, Simplon Norm, Regular 25px, colo3=232323
   -"Salasana" teksti, x=16, y=248, Simplon Norm, Regular 25px, colo3=232323
   -valinnan alasvetovalintapainike, x=16,y=144, w=424, h=82, br=16, color=FFFFFF (valkoinen)
   -salasana-kenttä, x=16, y=296, w=685, h=82, br=16, color=FFFFFF

   valinnat --> set_ok_cancel_buttons()
   "TAKAISIN", x=71, y=1029 IMAGE_VALKOINEN_VIHREAT_REUNAT
   "OK", x=428, y=1029, IMAGE_VIHREA_LYHYT

   Kielivalinnan lippu 120x120
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

#define BOX_ALUE_X 0
#define BOX_ALUE_Y 70
#define BOX_ALUE_W 800
#define BOX_ALUE_H (1005-70)

#define TEXT1_ALUE_X 16
#define TEXT1_ALUE_Y 144
#define TEXT1_ALUE_W 424
#define TEXT1_ALUE_H 82

#define TEXT1_X 16
#define TEXT1_Y 93

#define TEXT2_X 16
#define TEXT2_Y 248

#define TEXT2_ALUE_X 16
#define TEXT2_ALUE_Y 296
#define TEXT2_ALUE_W 685
#define TEXT2_ALUE_H 82

#define LIPPU_X  580
#define LIPPU_Y  122
#define LIPPU_W  120
#define LIPPU_H  120

#define NUM_BUTTON_IDX 5 //0=cancel, 1=Ok, 2=käyttäjä-kenttä, 3=salasana-kenttä, 4=kieli

int show_dropdown, password_ok;
int dropdown_selection[5];
int original_selection[5];
int menu_before, salasana_annettu;

void show_dropdown_valinnat(void);

void draw_PROF_VAIHTO(int param1)
{
   SDL_Color color = { 0x23, 0x23, 0x23, 0x0 };
   SDL_Color color2 = { 0xB4, 0xB4, 0xB4, 0x0 };
   set_ok_cancel_buttons2(IMAGE_VIHREA_LYHYT);
   //button alue
//   roundedBoxRGBA(gRenderer, BUTTON_ALUE_X, BUTTON_ALUE_Y, (BUTTON_ALUE_X+BUTTON_ALUE_W), (BUTTON_ALUE_Y+BUTTON_ALUE_H), 0xf4, 0xf4, 0xf4, 0xFF); //button alue alavalikkoon asti, alavalikko y=1022
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

   if (show_dropdown == 0)
   {
      roundedBoxColor(gRenderer, TEXT2_ALUE_X, TEXT2_ALUE_Y, (TEXT2_ALUE_X+TEXT2_ALUE_W),  (TEXT2_ALUE_Y+TEXT2_ALUE_H), 16, c);
      Button_group_add_button(3, TEXT2_ALUE_X, TEXT2_ALUE_Y, TEXT2_ALUE_W, TEXT2_ALUE_H);
   }

   Button_group_add_button(4, LIPPU_X, LIPPU_Y, LIPPU_W, LIPPU_H);
   Button_group_add_images(4, image_ptrs[IMAGE_LIPPU_FIN], nullptr);

   for (int i=0; i< NUM_BUTTON_IDX; i++)
   {
//      if (show_dropdown && (i==NUM_BUTTON_IDX-1)) //Oletus että dropdown valinnat menee salasana-kentän päälle (JOKA on viimeinen) joten sitä ei piirretä
//         break;
      Button_group_render(i);
   }

   //dropdown väkänen paikkaan x=409 y=181, w=12, h=7
   image_ptrs[IMAGE_NUOLI_ALAS_32px]->render(400, 171);
#if 0
   c = 0xFF<<24 | (0x23<<16) | (0x23<<8) | (0x23); //bgr
//   lineColor(gRenderer, 409,     181,     409+6,    181+7, c);
//   lineColor(gRenderer, 409+1,   181,     409+6+1,  181+7, c);
//   lineColor(gRenderer, 409+6,   181+7,   409+12,   181, c);
//   lineColor(gRenderer, 409+6+1, 181+7,   409+12+1, 181, c);
   for (int p=0; p<7; p++)
   {
      pixelColor(gRenderer, 409+p, 181+p, c);
      pixelColor(gRenderer, (409+12)-p, 181+p, c);
      pixelColor(gRenderer, 410+p, 181+p, c);
      pixelColor(gRenderer, (410+12)-p, 181+p, c);
   }
#endif

   render_text((char*)yleis_tekstit[YLEIS_TXT_KAYTTAJAPROFIILI], TEXT1_X, TEXT1_Y, (SDL_Color *)&color, font_norm_regular_25px);
   render_text((char*)yleis_tekstit[YLEIS_TXT_SALASANA], TEXT2_X, TEXT2_Y, (SDL_Color *)&color, font_norm_regular_25px);

   if (dropdown_selection[0] >= NUM_USER_LEVELS)
      dropdown_selection[0] = 0;
   render_text((char*)yleis_tekstit[YLEIS_TXT_PERUSKAYTTAJA + dropdown_selection[0]], TEXT1_ALUE_X, TEXT1_ALUE_Y+(TEXT1_ALUE_H/2), (SDL_Color *)&color2, font_norm_regular_25px, 0);

   if (salasana_annettu) //annettiin salasana
   {
      Uint8 i;
      char buff[6];
      for (i=0; i<strlen(numpad_string); i++)
         buff[i] = '*';
      buff[i] = 0;
      render_text((char*)buff, TEXT2_ALUE_X, TEXT2_ALUE_Y+(TEXT2_ALUE_H/2), (SDL_Color *)&color2, font_norm_regular_25px, 0);
   }

   show_banner(NUM_BUTTON_IDX);
}

int handler_PROF_VAIHTO(int x, int y,  SDL_Event *e)
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
         if (button_pressed) ////1=cancel, 2=Ok, 3=käyttäjä-kenttä, 4=salasana-kenttä, 5=kieli
         {
            if ((button_pressed >= DROPDOWN_EVENT_CODE1) && (button_pressed <= DROPDOWN_EVENT_CODE1 + 2))
            {
               dropdown_selection[0] = button_pressed - DROPDOWN_EVENT_CODE1;
               show_dropdown = 0;
               if ((button_pressed == DROPDOWN_EVENT_CODE1 + 1) || (button_pressed == DROPDOWN_EVENT_CODE1 + 2))
               {
                  menu_to_return = ID_PROF_VAIHTO;
                  return ID_NUMPAD_PIENI;
               }
               else if (button_pressed == DROPDOWN_EVENT_CODE1)
               {
                  password_ok = 1;
                  user_profile.user_level = 0;
               }
            }
            else if (button_pressed == 1) //cancel
            {
               user_profile.user_level = original_selection[0];
               if (menu_before < NUM_MENU_IDS)
                  return menu_before;
               else
                  return ID_KOTINAKYMA;
            }
            else if (button_pressed == 2) //OK, jos salasana annettu ja se on ok niin vaihdetaan profiili, paluu --> menu_to_return
            {
               if (menu_before < NUM_MENU_IDS)
                  return menu_before;
               else
                  return ID_KOTINAKYMA;
            }
            else if (button_pressed == 3)
            {
               show_dropdown = 1 - show_dropdown;
            }
            else if (button_pressed == 4)
            {
               //menu_to_return = ID_PROF_VAIHTO;
               //return ID_NUMPAD_PIENI;
            }
            button_pressed = 0;
         }
      }
      draw_PROF_VAIHTO(0);
      if (show_dropdown)
         show_dropdown_valinnat();
   }
   return 0xffff;
}

void init_PROF_VAIHTO(void)
{
}

void show_PROF_VAIHTO(int show_hide)
{
   if (show_hide == 0)
   {
      return;
   }
   if (menu_to_return != ID_PROF_VAIHTO)
   {
      menu_before = menu_to_return;
      salasana_annettu=0;
      original_selection[0] = user_profile.user_level;
   }

   pyyhk_enabled = 0;
   button_pressed = 0;
   Button_group_empty();
   Dropdown_group_empty();

   show_dropdown = alavalikko_naytolla = 0;
   dropdown_selection[0] = user_profile.user_level;
   draw_PROF_VAIHTO(0);
}

//#define TEXT1_ALUE_X 16
//#define TEXT1_ALUE_Y 144
//#define TEXT1_ALUE_W 424
//#define TEXT1_ALUE_H 82

#define SEL1_ALUE_X 16
#define SEL1_ALUE_Y (144+82+3)
#define SEL1_ALUE_W 424
#define SEL1_ALUE_H 50

void show_dropdown_valinnat(void)
{
   int i;
   int x, y;
   Uint32 c;
   SDL_Color color = { 0x23, 0x23, 0x23, 0x0 };
                    //blue       green      red
   c = 0xFF<<24 | (0xff<<16) | (0xff<<8) | (0xff); //bgr
   x = SEL1_ALUE_X;
   y = SEL1_ALUE_Y;
   for (i=0; i < 3; i++)
   {
      boxColor(gRenderer, x, y, (x+SEL1_ALUE_W), (y+SEL1_ALUE_H), c);
      Dropdown_group_add_selection(i, x, y, SEL1_ALUE_W, SEL1_ALUE_H);
      Dropdown_group_add_event_code(i, DROPDOWN_EVENT_CODE1+i);
      render_text((char*)yleis_tekstit[YLEIS_TXT_PERUSKAYTTAJA + i], x+10, y+6, (SDL_Color *)&color, font_norm_regular_25px, 0);
      y += (SEL1_ALUE_H+3);
   }
   salasana_annettu=0;
}

void prof_salasana_annettiin(void) //numpad_pieni kutsuu kun salasana on annettu ja se on numpad_string:issä
{
   salasana_annettu=1;
   password_ok = 0;
   if (dropdown_selection[0] == 1) //superkäytt
   {
      numpad_string[4] = 0;
      if (strcmp(numpad_string, "2018") == 0)
      {
         printf("Superkäyttäjän salasana OK\n");
         password_ok = 1;
         user_profile.user_level = 1;
      }
   }
   else if (dropdown_selection[0] == 2) //huolto
   {
      if (strcmp(numpad_string, "2019") == 0)
      {
         printf("Huollon salasana OK\n");
         password_ok = 1;
         user_profile.user_level = 2;
      }
   }
}
