/* 
   sek_tapa_valinta.cpp
   näppis jolla annetaan sekoitustapa ja sekoitusnopeus
   Miten oli ajatus että valinta hyväksytään? Nyt tehty niin että kun valitsee sek.tavan niin valinnat (annettu nopeus ja sek.tapa) hyväksytään.
   --> Tässä näkymässä voidaan pysyä vaikka loputtomasti ja sekoitinta ohjataan ns. lennossa.
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
#include "kettle_functions.h"

extern void nayta_numpad_tausta(void);

extern int key_outside_iso_alue(int x, int y);
extern char numpad_string[20];

void handle_sek_tapa_valinta_button_press(int but);

#define ISO_ALUE_X1 137
#define ISO_ALUE_Y1 144
#define ISO_ALUE_X2 (137+526)
#define ISO_ALUE_Y2 (144+992)

#define SPEED_MIINUS_X 220
#define SPEED_MIINUS_Y 200
#define SPEED_BUTTON_W 50
#define SPEED_BUTTON_H 92

#define SPEED_PLUS_X  (800-SPEED_MIINUS_X-SPEED_BUTTON_W)
#define SPEED_PLUS_Y SPEED_MIINUS_Y

#define SPEED_VALUE_X (SPEED_MIINUS_X+100)
#define SPEED_VALUE_Y (SPEED_MIINUS_Y)

const int kuvatukset[9] =
{
   IMAGE_SEKOITUSTAPA_KUVA_KAHTEEN_SUUNTAAN,
   IMAGE_SEKOITUSTAPA_KUVA_YHTEEN_SUUNTAAN,
   IMAGE_SEKOITUSTAPA_KUVA_KAHTEEN_SUUNTAAN_HIDAS,
   IMAGE_SEKOITUSTAPA_KUVA_YHTEEN_SUUNTAAN,
   IMAGE_SEKOITUSTAPA_KUVA_KAHTEEN_SUUNTAAN_NOPEA,
   IMAGE_SEKOITUSTAPA_KUVA_YHTEEN_SUUNTAAN,
   -1,
   -1,
   -1,
};
//3=kahteen suuntaa, hidas. 4=Yhteen suuntaan, 5=Kahteen suuntaan,6=yhteen suuntaan tauko,7=kahteen suuntaan nopea, 8=yhteen suuntaan pitkä tauko,9=kahteen suuntaan tauko,10=omasek.
const int kuvatus_txt[9] =
{
   SEKOITUSTAPA_TXT_KAHTEEN_SUUNTAAN_HIDAS,
   SEKOITUSTAPA_TXT_YHTEEN_SUUNTAAN,
   SEKOITUSTAPA_TXT_KAHTEEN_SUUNTAAN,
   SEKOITUSTAPA_TXT_YHTEEN_SUUNTAAN_TAUKO,
   SEKOITUSTAPA_TXT_KAHTEEN_SUUNTAAN_NOPEA,
   SEKOITUSTAPA_TXT_YHTEEN_SUUNTAAN_PITKA_TAUKO,
   SEKOITUSTAPA_TXT_KAHTEEN_SUUNTAAN_TAUKO,
   SEKOITUSTAPA_TXT_OMA_SEKOITUSTAPA,
   -1,
};

void draw_SEK_TAPA_VALINTA(int param1)
{
   char buff[20];
   int but_idx=0;
   SDL_Color txt_color = { 0x00, 0x00, 0x00, 0xff };

   //last par: color The color value of the rectangle to draw (0xRRGGBBAA).
   Uint32 c;
   c = 0xFF<<24 | (0xF4<<16) | (0xF4<<8) | (0xF4); //bgr
   roundedBoxColor(gRenderer, //ISO ALUE
                   ISO_ALUE_X1, ISO_ALUE_Y1, //x1 + y1
                   ISO_ALUE_X2, ISO_ALUE_Y2, //x2 + y2
                   104, //border radius
                   c);
   SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_NONE);

   strcpy(buff, numpad_string);

   //Button 0 = Miinus-näppäin, Button 1 = Plus-näppäin
   //Buttons 2...10 ovat sekoitustavat. 
   Button_group_add_button(but_idx, SPEED_MIINUS_X, SPEED_MIINUS_Y, SPEED_BUTTON_W, SPEED_BUTTON_H);
   Button_group_setText(but_idx, (char*)"-", txt_color, font_norm_bold_110px);
   Button_group_render(but_idx++);

   Button_group_add_button(but_idx, SPEED_PLUS_X, SPEED_PLUS_Y, SPEED_BUTTON_W, SPEED_BUTTON_H);
   Button_group_setText(but_idx, (char*)"+", txt_color, font_norm_bold_110px);
   Button_group_render(but_idx++);

   sprintf(buff, "%3d", numpad_parameter_val);
   render_text(buff, SPEED_VALUE_X, SPEED_VALUE_Y, (SDL_Color *)&txt_color, font_norm_bold_110px);

   int nx;
   int ny = 342;
   int kuvax=0;

   for (int j=0; j<4; j++)  //Sek.tapa valinnat 8 kpl
   {
      nx = 175; //eka button x=175
      for (int i=0; i< 2; i++)
      {
         Button_group_add_button(but_idx, nx, ny, 210, 130, 55); //sek.valinta nappi w/h 210,130, txt_offset=55
         Button_group_setText(but_idx, kuvatus_txt[kuvax], TXT_GROUP_SEKOITUSTAPA, txt_color, font_norm_medium_16px);
         if (kuvatukset[kuvax] != -1)
         {
            if (run_target_params.selected_mixer_mode && ((run_target_params.selected_mixer_mode-1) == kuvax))
               Button_group_add_images(but_idx, image_ptrs[IMAGE_SEKOITUSTAPA_VAALEA_VALITTU_BTN], image_ptrs[kuvatukset[kuvax]]);
            else
               Button_group_add_images(but_idx, image_ptrs[IMAGE_SEKOITUSTAPA_VAALEA_BTN], image_ptrs[kuvatukset[kuvax]]);
         }
         else
         {
            if (run_target_params.selected_mixer_mode && ((run_target_params.selected_mixer_mode-1) == kuvax))
               Button_group_add_images(but_idx, image_ptrs[IMAGE_SEKOITUSTAPA_VAALEA_VALITTU_BTN], 0);
            else
               Button_group_add_images(but_idx, image_ptrs[IMAGE_SEKOITUSTAPA_VAALEA_BTN], 0);
         }
         ++kuvax;
         Button_group_render(but_idx++);

         nx = 415; //toka button x=415
      }
      ny += (500-342);
   }
   //OK-button
   Button_group_add_button(but_idx, (800/2)-(210/2), ny, 210, 130); //sek.valinta nappi w/h 210,130
   Button_group_setText(but_idx, (char*)"OK", txt_color, font_norm_bold_70px);
   Button_group_add_images(but_idx, image_ptrs[IMAGE_SEKOITUSTAPA_VAALEA_BTN], 0);
   Button_group_render(but_idx++);

   SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_NONE);
   show_banner(but_idx);
}

void enter_SEK_TAPA_VALINTA_pages(void)
{
   draw_SEK_TAPA_VALINTA(0);
}

int handler_SEK_TAPA_VALINTA(int x, int y,  SDL_Event *e)
{
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
            if (b==1)  //Button 1 = miinus
            {
               if (numpad_parameter_val > 20)
                  numpad_parameter_val -= 20;
               else
                  numpad_parameter_val = 0;
               if (menu_to_return != ID_VAIHEIDEN_LUONTI)
                  run_target_params.mixer_speed = numpad_parameter_val; //"Käyttäjän tekemät säädöt alkavat toimia heti"
            }
            else if (b==2)  //Button 2 = plus
            {
              if (menu_to_return == ID_VAIHEIDEN_LUONTI)
              {
                  if (numpad_parameter_val < 200)
                     numpad_parameter_val += 20;
                  if (numpad_parameter_val > 200)
                     numpad_parameter_val = 200;
              }
              else if (!onko_kansi_auki()) //Kansi kiinni?
               {
                  int max_speed = max_nopeudet[run_target_params.mixer_mode];
                  if (numpad_parameter_val < 200)
                     numpad_parameter_val += 20;
                  if (numpad_parameter_val > 200)
                     numpad_parameter_val = 200;
                  run_target_params.mixer_speed = numpad_parameter_val; //"Käyttäjän tekemät säädöt alkavat toimia heti"

                  if (run_target_params.mixer_speed > max_speed)
                     numpad_parameter_val = run_target_params.mixer_speed = max_speed;

                  if (run_target_params.selected_mixer_mode==0)
                  {
                     numpad_parameter_val = run_target_params.mixer_speed = 20;
                     set_mixing(MIX_MODES_YHTEEN_SUUNTAAN); //starttaa valitulla modella JA asetetulla nopeudella HUOM!, mode -> run_target_params.mixer_mode
                  }
               }
            }
            else //3=kahteen suuntaa, hidas. 4=Yhteen suuntaan, 5=Kahteen suuntaan,6=yhteen suuntaan tauko,7=kahteen suuntaan nopea, 8=yhteen suuntaan pitkä tauko,9=kahteen suuntaan tauko,10=omasek.
            {    //11=OK
               if (b==11)
               {
                  if (menu_to_return == ID_VAIHEIDEN_LUONTI)
                     numpad_updates_vaiheen_parametri();
                  return menu_to_return;
               }
               b -= 2;
               if (b < NUM_MIX_MODES)
               {
                  if (menu_to_return == ID_VAIHEIDEN_LUONTI)
                     numpad_updates_vaiheen_sek_mode_parametri(b);
                  else if (run_target_params.selected_mixer_mode != b)
                     set_mixing(b); // --> run_target_params.selected_mixer_mode = run_target_params.mixer_mode = b;
               }
            }
         }
         else if (key_outside_iso_alue(x, y))
            return menu_to_return;
      }
      draw_SEK_TAPA_VALINTA(0);
   }
   if (menu_to_return != ID_VAIHEIDEN_LUONTI)
      numpad_parameter_val = run_target_params.mixer_speed;
   return 0xffff;
}

void init_SEK_TAPA_VALINTA(void)
{
}

void show_SEK_TAPA_VALINTA(int show_hide)
{
   if (show_hide == 0)
   {
      return;
   }
   pyyhk_enabled = 0;
   Button_group_empty();
   //sprintf(numpad_string, "%d", numpad_parameter_val);
   memset(numpad_string, 0, sizeof(numpad_string));
   numpad_parameter_val = run_target_params.mixer_speed;
   enter_SEK_TAPA_VALINTA_pages();
   if (menu_to_return >= NUM_MENU_IDS)
      menu_to_return = ID_KOTINAKYMA;
}

void handle_sek_tapa_valinta_button_press(int but)
{
}

char * get_sekoitus_tapa_txt(void)
{
   if (run_target_params.selected_mixer_mode && (run_target_params.selected_mixer_mode<NUM_MIX_MODES))
   {
      return (char*)sekoitustapa_tekstit[run_target_params.selected_mixer_mode-1];
   }
   else
      return (char*)"";
}

//   MIX_MODES_EI_KAYNNISSA,
//   MIX_MODES_KAHTEEN_SUUNTAAN_HIDAS,      1  M:"Suunnanvaihto, hidas", 4
//   MIX_MODES_YHTEEN_SUUNTAAN,             2  M:"Yhteen suuntaan", 1
//   MIX_MODES_KAHTEEN_SUUNTAAN,            3  M:"Suunnanvaihto", 2
//   MIX_MODES_YHTEEN_SUUNTAAN_TAUKO,       4  M:"Yhteen suuntaan, tauko", 6
//   MIX_MODES_KAHTEEN_SUUNTAAN_NOPEA,      5  M:"Suunnanvaihto, nopea", 3
//   MIX_MODES_YHTEEN_SUUNTAAN_PITKA_TAUKO, 6  M:"Yhteen suuntaan, pitkä tauko", 7
//   MIX_MODES_KAHTEEN_SUUNTAAN_TAUKO,      7  M:"Suunnanvaihto, tauko", 5
//   MIX_MODES_OMA_SEKOITUSTAPA,            8

//Mohjain, mix-modes
//const char * mymixer_texts[7]={
//   "Yhteen suuntaan",              1m x
//   "Suunnanvaihto",                2m x
//   "Suunnanvaihto, nopea",         3m x
//   "Suunnanvaihto, hidas",         4m x
//   "Suunnanvaihto, tauko",         5m x
//   "Yhteen suuntaan, tauko",       6m x
//   "Yhteen suuntaan, pitkä tauko", 7m x
//};

//M-ohj:
//const int max_nopeudet[7]={
//   200, 160, 100, 160, 100, 100, 100 //v2.14 viimeiset 3 ohjelmaa: 40 -> 100
//};

const int max_nopeudet[9]={
// 200, 160, 100, 160, 100, 100, 100,
  //1    2    3    4    5    6    7
   0, 160, 200, 160, 100, 100, 100, 100, 200
// 1>4  2>1  3>2  4>6  5>3  6>7  7>5  8=oma
};

