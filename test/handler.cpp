/* File: handler.c
   Purpose:
*/
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <stdio.h>
#include <string>
#include <stdlib.h>

#include "menu_ids.h"
#include "handler.h"

#include "button_defs.h"
#include "button_group.h"
#include "var.h"
#include "externs.h"
#include "tekstit.h"
#include "kettle_functions.h"
#include "cook_prog.h"

extern int power_off_set;

Uint32 time_to_approve_next_tilt_command;

int check_any_errors(void);
int draw_dialog(int dialog_id);
void himmenna_nykyinen(int par);
int pulse_pressed;
int alku_viive;

#define _funcs(name) {init_##name, handler_##name, show_##name, draw_##name}

const struct menu_struct menus[NUM_MENU_IDS]=
{
   {ID_LAUNCH,   _funcs(LAUNCH), 0 },
   {ID_KOTINAKYMA, _funcs(KOTINAKYMA), 0 },
   {ID_KYPSENNYS, _funcs(KYPSENNYS), 0 },
   {ID_JAAHDYTYS, _funcs(JAAHDYTYS), 0 },
   {ID_NUMPAD, _funcs(NUMPAD), 1 }, //1 = dialog
   {ID_NUMPAD_PIENI, _funcs(NUMPAD_PIENI), 2 }, //1 = dialog, 2=myös mutta ei pimennetä ruudun yläosaa
   {ID_SEK_TAPA_VALINTA, _funcs(SEK_TAPA_VALINTA), 1 },
   {ID_IOTEST, _funcs(IOTEST), 0 },
   {ID_VALMIS, _funcs(VALMIS), 0 },
   {ID_OK_CANCEL, _funcs(OK_CANCEL), 1 },
   {ID_PROF_VAIHTO, _funcs(PROF_VAIHTO), 0 },
   {ID_OHJELMAT, _funcs(OHJELMAT), 0 },
   {ID_OMAT_OHJELMAT, _funcs(OMAT_OHJELMAT), 0 },
   {ID_UUSI_OHJELMA, _funcs(UUSI_OHJELMA), 0 },
   {ID_VAIHEIDEN_LUONTI, _funcs(VAIHEIDEN_LUONTI), 0 },
   {ID_LISTAA_KANSIO, _funcs(LISTAA_KANSIO), 0 },
   {ID_KEYPAD, _funcs(KEYPAD), 3 },
   {ID_NAYTA_OHJ_VAIHEET, _funcs(NAYTA_OHJ_VAIHEET), 0 },
   {ID_SYOTA_ALOITA_TIEDOT, _funcs(SYOTA_ALOITA_TIEDOT), 0 },
};

int current_menu_id, ms_timeout, prev_menu_id, menu_to_return, prev_errors;
int current_dialog_id;

char ip1[25];
char ip2[25];

int find_menu(UWORD menu_id);

int START_MENU;

void start_graf(void)
{
   current_dialog_id = 0;
   menu_to_return = ID_LAUNCH;
   current_menu_id = find_menu(START_MENU);
   if (current_menu_id < NUM_MENU_IDS)
	   menus[current_menu_id].funcs.show(1);
}

void create_them(void)
{
   int i=0;

   make_valmiskansiot();

   load_run_parameters(); //lataa defaultit -> ajo-parametreiksi
   load_kettle_options();

//   make_dirs();
   for (i=0; i< NUM_MENU_IDS; i++)
   {
      menus[i].funcs.init();
   }
   ip1[0] = 0;
   ip2[0] = 0;
//   if ( getipaddress(ip1, ip2) == -1) //ip_osoite.c
//   {
//      ip1[0] = 0;
//      ip2[0] = 0;
//   }

   time_to_approve_next_tilt_command = SDL_GetTicks() + 2000;

#ifdef KORTIT_KYTKETTY
   START_MENU = ID_LAUNCH;
#else
   START_MENU = ID_KOTINAKYMA;
//   START_MENU = ID_LAUNCH;
#endif
   start_graf();

   set_emptying(0);
   set_emptying(1); // vain ohjelman alussa/jäähdytksen jälkeen kutsutaan --> set_emptying(int start)
   alku_viive = 5;
}

int find_menu(UWORD menu_id)
{
   UWORD i;
   for (i=0; i< NUM_MENU_IDS; i++)
   {
      if (menus[i].id == menu_id)
         return i;
   }
   return START_MENU;
}

int handle_code(int x, int y, SDL_Event *e)
{
   int new_menu_id, force_menu_id;

//   SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_NONE);
   if (current_dialog_id == 0)
   {
      if ((current_menu_id == ID_LAUNCH) /*|| (current_menu_id == ID_NUMPAD)*/) 
         SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, 0 ); //Black
      else
         SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0 );
      SDL_RenderClear( gRenderer );
   }

   if ((current_menu_id != ID_LAUNCH) && !power_off_set)
   {
      if (kansi_avattiin) //can_thread asettaa
      {
         kansi_avattiin = 0;
         if (ylavalikko_naytolla)
         {//Jos ylavalikko on näytöllä lähetetään extra button-down event jotta näkymä päivittyy
            piilota_ylavalikko(-1);
            if (e->type == 0)
            {
               x = y = -1;
               e->type = SDL_MOUSEBUTTONDOWN;
            }
         }
   //      if (pyyhk_enabled)
         if (esta_alavalikko_naytolle)
            printf("alavalikko estetty\n");
         else
            alavalikko_naytolla = 1;
      }
      else if (kansi_suljettiin)
      {
         kansi_suljettiin = 0;
         if (alavalikko_naytolla)
         {
            piilota_alavalikko(-1);
            if (!e->type && !current_dialog_id)
            {
               x = y = -1;
               e->type = SDL_MOUSEBUTTONDOWN;
            }
         }
      }
   }

   //check any errors
   force_menu_id = 0;
   if (alku_viive)
      --alku_viive;
   if (!alku_viive && check_any_errors()) //handler.cpp
   {
      if (!prev_errors)
         clear_all_kettle_functions();
      prev_errors = 1;
      if (current_menu_id != ID_KOTINAKYMA)
         force_menu_id = ID_KOTINAKYMA;
      current_dialog_id = 0;
   }
   else
      prev_errors = 0;

   if (current_dialog_id)
   {
#if 0
      menus[current_menu_id].funcs.draw(1); //piirretään dialog nykyisen päälle
      himmenna_nykyinen();
#endif
      int ret = menus[current_dialog_id].funcs.handler(x, y, e);
      if ((ret != 0xFFFF) && (ret != current_dialog_id))
      {
                  SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0 );
                  SDL_RenderClear( gRenderer );
         current_menu_id = ret;
         menus[current_menu_id].funcs.show(1);
         menus[current_dialog_id].funcs.show(0);
         current_dialog_id = 0;
         time_to_approve_next_tilt_command = SDL_GetTicks() + 2000; //Ei hyväksutä tilt-komentoja dialogin jälkeen..jos poistui näitten näpp. kohdalta
      }
   }
   else
   {
      new_menu_id = menus[current_menu_id].funcs.handler(x, y, e);
      if ((new_menu_id < NUM_MENU_IDS) && menus[new_menu_id].dialog)
      {
         //printf("33333333333\n");
         current_dialog_id = new_menu_id;
      }
      else
         current_dialog_id = 0;

      if (force_menu_id)
      {
         new_menu_id = force_menu_id;
         current_dialog_id = 0;
      }

      if (current_dialog_id)
      {
         SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_NONE);
         menus[current_menu_id].funcs.draw(1); //piirretään dialog nykyisen päälle
         himmenna_nykyinen(menus[current_dialog_id].dialog);
         menus[current_dialog_id].funcs.show(1);
      }
      else if ((new_menu_id != 0xFFFF) && (new_menu_id != current_menu_id))
      {
         prev_menu_id = current_menu_id;
         if ((current_menu_id = find_menu(new_menu_id)) < NUM_MENU_IDS)
         {
            esta_alavalikko_naytolle = 0;
            menus[current_menu_id].funcs.show(1);
            menus[prev_menu_id].funcs.show(0);
         }
      }
   }

   if (e->type != SDL_MOUSEBUTTONDOWN)
   {
      set_tilting(0);
      pulse_pressed = 0;
   }
   else //Button Down
   { //Btw.TImeouttia ei tule jos button on pohjassa
      if (alavalikko_naytolla)
      {
         if (SDL_TICKS_PASSED(SDL_GetTicks(), time_to_approve_next_tilt_command))
         {
            int b = Button_group_check_for_key_press(x, y);
            if (b == CODE_KALLISTUS_ALAS)
            {
               time_to_approve_next_tilt_command = SDL_GetTicks() + 1000;
//               printf("TILT DOWN\n");
               run_target_params.tilt_mode = 2; //1=UP, 2=Down
               set_tilting(1);
               pulse_pressed = 0;
            }
            else if (b == CODE_KALLISTUS_YLOS)
            {
               time_to_approve_next_tilt_command = SDL_GetTicks() + 1000;
//               printf("TILT UP\n");
               run_target_params.tilt_mode = 1; //1=UP, 2=Down
               set_tilting(1);
               pulse_pressed = 0;
            }
            else if (b == CODE_KALLISTUS_PULSE)
            {
               time_to_approve_next_tilt_command = SDL_GetTicks() + 1000;
               set_mixing(0);
               mixer_motor_vaihe = MIXER_MOTOR_PULSED_STATE;
               pulse_pressed = 1;
               //printf("Pulsekytkin\n");
            }
         }
      }
   }
   if (!pulse_pressed && !onko_jalka_kytkin_painettu())
   {
      if (mixer_motor_vaihe == MIXER_MOTOR_PULSED_STATE)
         set_mixing(0);
   }
   if (SDL_TICKS_PASSED(SDL_GetTicks(), time_to_approve_next_tilt_command) && onko_jalka_kytkin_painettu()) //Turvasek.
   {
#ifdef KORTIT_KYTKETTY
      set_mixing(0);
      mixer_motor_vaihe = MIXER_MOTOR_PULSED_STATE;
#endif
   }

   return current_menu_id;
}

#if 0
SDL_Surface *sshotSurface = nullptr;
SDL_Texture *sshotTexture = nullptr;
int take_screen_shot_now;

void take_screen_shot(void)
{
   //sshotSurface = SDL_CreateRGBSurface(0, 800, 1280, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
   //sshotSurface = SDL_CreateRGBSurface(0, 800, 1280, 32, 0, 0, 0, 0);
   sshotSurface = SDL_CreateRGBSurfaceWithFormat(0, 800, 1280, 32, SDL_PIXELFORMAT_ARGB8888);
   if (sshotSurface != nullptr)
   {
      SDL_RenderReadPixels(gRenderer, NULL, SDL_PIXELFORMAT_ARGB8888, sshotSurface->pixels, sshotSurface->pitch);
      sshotTexture = SDL_CreateTextureFromSurface(gRenderer, sshotSurface);
      SDL_FreeSurface(sshotSurface); /* we got the texture now -> free surface */
      printf("sshotSurface OK\n");
   }
   else
      printf("sshotSurface = nullptr\n");
}

void render_taken_screen_shot(void)
{
   if (sshotTexture != nullptr)
      SDL_RenderCopy(gRenderer, sshotTexture, NULL, NULL);
   else
      printf("sshotTexture = nullptr\n");
}

void release_screen_shot_texture(void)
{
   if (sshotTexture != nullptr)
      SDL_DestroyTexture(sshotTexture);
}
#endif

int kont_error_ctr, ylik_error_ctr, painekyt_error_ctr;

//extern unsigned char water_measure_error, food_sensor_error, kettle_sensor_error, hataseis_painettu;
int check_any_errors(void)
{
   int err = 0;
   int my_can_comm_error = 0;

#ifndef KORTIT_KYTKETTY
   return 0;
#endif

   for (int can=0; can < NUM_OF_CAN_MODULES; can++)
   {
      if (can_comm_error[can])
      {
         my_can_comm_error = can+1; //Näytetään eka moduli missä virhe
         break;
      }
   }

   errors_paalla[ERROR_IDX_HATASEIS_PAINETTU] = hataseis_painettu ? 1 : 0;
   errors_paalla[ERROR_IDX_CAN_COMM_ERROR] = my_can_comm_error;
   errors_paalla[ERROR_IDX_VESIMITTARI_ERROR] = water_measure_error ? 1 : 0;

   if (food_sensor_error == 0)
      errors_paalla[ERROR_IDX_RUOKA_ANTURI_KATKOS_ERROR] = errors_paalla[ERROR_IDX_RUOKA_ANTURI_OIKOSULKU_ERROR] = 0;
   else if (food_sensor_error == 1) //1 = katko, 2=oikos
      errors_paalla[ERROR_IDX_RUOKA_ANTURI_KATKOS_ERROR] = 1;
   else if (food_sensor_error == 2)
      errors_paalla[ERROR_IDX_RUOKA_ANTURI_OIKOSULKU_ERROR] = 1;

   if (kettle_sensor_error == 0)
      errors_paalla[ERROR_IDX_VAIPPA_ANTURI_KATKOS_ERROR] = errors_paalla[ERROR_IDX_VAIPPA_ANTURI_OIKOSULKU_ERROR] = 0;
   else if (kettle_sensor_error == 1)
      errors_paalla[ERROR_IDX_VAIPPA_ANTURI_KATKOS_ERROR] = 1;
   else if (kettle_sensor_error == 2)
      errors_paalla[ERROR_IDX_VAIPPA_ANTURI_OIKOSULKU_ERROR] = 1;

   //Di6, Jos ei lämmitys-ohjausta ja Di6=0 --> kontaktori-vika
   if ((output_registers[OUTPUT_REG_CARD2_DIGITAL_OUTPUTS] & HEAT_24V_OUT_BIT_CARD2) == 0)
   {
      if ((input_registers[INPUT_REG_CARD2_DIGITAL_INPUTS] & KONTAKTORI_VIKA_IN_BIT_CARD2) == 0)
      {
         if (++kont_error_ctr > 2)
            kontaktori_vika = 1;
      }
      else
         kontaktori_vika = kont_error_ctr = 0;
   }            
   errors_paalla[ERROR_IDX_KONTAKTORI_VIKA] = kontaktori_vika;

   if ((input_registers[INPUT_REG_CARD1_DIGITAL_INPUTS] & PAINEKYTKIN_IN_BIT_CARD1) != 0)
   {
      if (++painekyt_error_ctr > 2)
         painekytkin_lauennut = 1;
   }
   else
      painekytkin_lauennut = painekyt_error_ctr = 0;
   errors_paalla[ERROR_IDX_PAINEKYTKIN_LAUENNUT] = painekytkin_lauennut;

   if ((input_registers[INPUT_REG_CARD1_DIGITAL_INPUTS] & VAST_YLIKUUM_SUOJA_IN_BIT_CARD1) != 0)
   {
      if (++ylik_error_ctr > 2)
         vast_ylikuum_suoja_lauennut = 1;
   }
   else
      vast_ylikuum_suoja_lauennut = ylik_error_ctr = 0;
   errors_paalla[ERROR_IDX_VAST_YLIKUUM_SUOJA_LAUENNUT] = vast_ylikuum_suoja_lauennut;

   if (my_can_comm_error || water_measure_error || food_sensor_error || kettle_sensor_error || hataseis_painettu ||
       kontaktori_vika || painekytkin_lauennut || vast_ylikuum_suoja_lauennut)
      err = 1;

   return err;
}

void clear_all_errors(void)
{
   for (int can=0; can < NUM_OF_CAN_MODULES; can++)
      can_comm_err_counter[can] = can_comm_error[can] = 0;
   water_measure_error = 0;
   food_sensor_error = 0;
   kettle_sensor_error = 0;

   kontaktori_vika = kont_error_ctr = 0;
   painekyt_error_ctr = painekytkin_lauennut = 0;
   vast_ylikuum_suoja_lauennut = ylik_error_ctr = 0;
}

void himmenna_nykyinen(int par)
{
   //lätkästää osittain läpinäkyvä musta layer nykynäytön päälle
   SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0xA7);
   SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
   SDL_Rect rect;
   rect.x = 0;
   if (par==2)
      rect.y = 527;
   else if (par==3)
      rect.y = 480;
   else
      rect.y = 0;
   rect.w = 800;
   rect.h = 1280;
   SDL_RenderFillRect(gRenderer, &rect);
   SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_NONE);
}

void himmenna_lisaa(int par)
{
   static unsigned char him = 0xA7; //0 = kokonaan läpinäkyvä?
   //lätkästää osittain läpinäkyvä musta layer nykynäytön päälle
   SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, him);
   if (him < 0xF0)
      him += 0x10;
   SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
   SDL_Rect rect;
   rect.x = 0;
   rect.y = 0;
   rect.w = 800;
   rect.h = 1280;
   SDL_RenderFillRect(gRenderer, &rect);
   SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_NONE);
}
