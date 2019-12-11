/* 
   jaahdytys.cpp

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
#include "tekstit.h"
#include "var.h"
#include "image.h"
#include "button_group.h"
#include "externs.h"
#include "kettle_functions.h"
#include "cook_prog.h"

extern int sidebar_on_shift_buttons_x;

void handle_jaahd_button_press(int but);

#define TEMP_LUKEMA_IDX 0
#define AIKA_LUKEMA_IDX 1
#define VESI_LUKEMA_IDX 2
#define MIX_LUKEMA_IDX  3
#define TEMP_MIINUS_IDX 4
#define TEMP_PLUS_IDX   5
#define AIKA_MIINUS_IDX 6
#define AIKA_PLUS_IDX   7
#define VESI_MIINUS_IDX 8
#define VESI_PLUS_IDX   9
#define MIX_MIINUS_IDX  10
#define MIX_PLUS_IDX    11
#define LOPETA_IDX    12
#define NUM_BUTTON_IDX  13

const button_grp_0_s jaahd_Button_group[NUM_BUTTON_IDX] = {
   {TEMP_LUKEMA_L, TEMP_LUKEMA_T, TEMP_LUKEMA_W, TEMP_LUKEMA_H, TEMP_LUKEMA_IDX},
   {AIKA_LUKEMA_L, AIKA_LUKEMA_T, AIKA_LUKEMA_W, AIKA_LUKEMA_H, AIKA_LUKEMA_IDX},
   {VESI_LUKEMA_L, VESI_LUKEMA_T, VESI_LUKEMA_W, VESI_LUKEMA_H, VESI_LUKEMA_IDX},
   {MIX_LUKEMA_L, MIX_LUKEMA_T, MIX_LUKEMA_W, MIX_LUKEMA_H, MIX_LUKEMA_IDX},
   {TEMP_MIINUS_L, TEMP_MIINUS_T, TEMP_MIINUS_W, TEMP_MIINUS_H, TEMP_MIINUS_IDX},
   {TEMP_PLUS_L, TEMP_PLUS_T, TEMP_PLUS_W, TEMP_PLUS_H, TEMP_PLUS_IDX},
   {AIKA_MIINUS_L, AIKA_MIINUS_T, AIKA_MIINUS_W, AIKA_MIINUS_H, AIKA_MIINUS_IDX},
   {AIKA_PLUS_L, AIKA_PLUS_T, AIKA_PLUS_W, AIKA_PLUS_H, AIKA_PLUS_IDX},
   {VESI_MIINUS_L, VESI_MIINUS_T, VESI_MIINUS_W, VESI_MIINUS_H, VESI_MIINUS_IDX},
   {VESI_PLUS_L, VESI_PLUS_T, VESI_PLUS_W, VESI_PLUS_H, VESI_PLUS_IDX},
   {MIX_MIINUS_L, MIX_MIINUS_T, MIX_MIINUS_W, MIX_MIINUS_H, MIX_MIINUS_IDX},
   {MIX_PLUS_L, MIX_PLUS_T, MIX_PLUS_W, MIX_PLUS_H, MIX_PLUS_IDX},
   {LOPETA_L, LOPETA_T, LOPETA_W, LOPETA_H, LOPETA_IDX},
};

void update_chill_buttons_text(void)
{                   
   if (!run_target_params.chill_mode && !(prepare_target_params.state & TARGET_STATE_CHILL_TEMP_CHANGED_BIT))
      strcpy(helper_text_buffers[TEMP_LUKEMA_IDX], "--");
   else     //Huom! mitattu temp = live_params.cook_temps !
      sprintf(helper_text_buffers[TEMP_LUKEMA_IDX], "%d""\xB0""C", (nayta_mitatut_arvot&0x01) ? live_params.cook_temps[TARGET_COOK_FOOD_TEMP_IDX] : prepare_target_params.chill_temps[TARGET_CHILL_TEMP_IDX]);

   if (!run_target_params.chill_time_left_seconds)
      strcpy(helper_text_buffers[AIKA_LUKEMA_IDX], "--");
   else if ((nayta_mitatut_arvot&0x02)==0) //Näytetään AIKA asetuarvo
      sprintf(helper_text_buffers[AIKA_LUKEMA_IDX], "%02d:%02d", run_target_params.chill_time_minutes/60, run_target_params.chill_time_minutes%60);
   else
   {
      int minutes_left = 0;
      if (run_target_params.temp_reached)
      {
         minutes_left = run_target_params.chill_time_left_seconds/60;
         if (run_target_params.chill_time_left_seconds < run_target_params.chill_time_minutes*60)
            ++minutes_left;
      }
      else
      {
         minutes_left = run_target_params.chill_time_minutes;
      }
      check_temp_reach_ind_time();
      if (temp_reach_indicator)
         sprintf(helper_text_buffers[AIKA_LUKEMA_IDX], "%02d:%02d", minutes_left/60, minutes_left%60);
      else
         sprintf(helper_text_buffers[AIKA_LUKEMA_IDX], "%02d %02d", minutes_left/60, minutes_left%60);
   }

   if (!run_target_params.water_fill_mode && !(prepare_target_params.state & TARGET_STATE_WATER_FILL_CHANGED_BIT) && !run_target_params.water_fill_amount_litres)
      strcpy(helper_text_buffers[VESI_LUKEMA_IDX], "--");
   else if ((nayta_mitatut_arvot&0x04))
   {
      sprintf(helper_text_buffers[VESI_LUKEMA_IDX], "%d l", abs(run_target_params.water_fill_amount_litres - live_params.water_fill_amount_litres));
   }
   else
   {
      sprintf(helper_text_buffers[VESI_LUKEMA_IDX], "%d l", prepare_target_params.water_fill_amount_litres);
   }

   if (run_target_params.mixer_mode == 0)
      strcpy(helper_text_buffers[MIX_LUKEMA_IDX], "--");
   else
      sprintf(helper_text_buffers[MIX_LUKEMA_IDX], "%d", run_target_params.mixer_speed);

/*   if (live_params.state & TARGET_STATE_SOMETHING_STARTED_BIT) Poistettu turhana 10.12
      strcpy(helper_text_buffers[LOPETA_IDX], (char*)yleis_tekstit[YLEIS_TXT_LOPETA]);
   else
      strcpy(helper_text_buffers[LOPETA_IDX], (char*)yleis_tekstit[YLEIS_TXT_LOPETA]);*/
}

void set_jaahdytys_buttons(void)
{
   SDL_Color txt_color = { 0xff, 0xff, 0xff, 0xff };

   for (int i=0; i< NUM_BUTTON_IDX; i++)
   {
      if (i==LOPETA_IDX)
         continue;
      if ((live_params.state & TARGET_STATE_SOMETHING_STARTED_BIT) && alavalikko_naytolla && ((i==MIX_LUKEMA_IDX) || (i==MIX_MIINUS_IDX) || (i==MIX_PLUS_IDX)))
      {
         Button_groups[MIX_LUKEMA_IDX].button_set = 0;
         Button_groups[MIX_MIINUS_IDX].button_set = 0;
         Button_groups[MIX_PLUS_IDX].button_set = 0;
         continue;
      }

      Button_group_add_button(i, jaahd_Button_group[i].x - sidebar_on_shift_buttons_x, jaahd_Button_group[i].y, jaahd_Button_group[i].Width, jaahd_Button_group[i].Height);
      Button_group_setText(i, helper_text_buffers[i], txt_color, font_norm_bold_80px);
      if (((nayta_mitatut_arvot&0x01)&&run_target_params.chill_mode) && ((i == TEMP_MIINUS_IDX) || (i == TEMP_PLUS_IDX)))
         continue; //Jos pata saavuttanut tavoitelämpötilan, lämmön säädön plus/miinus tulevat näkyviin! Tämä siis tekemättä
      if (i==TEMP_LUKEMA_IDX)
         Button_group_add_images(i, image_ptrs[ ((nayta_mitatut_arvot&0x01)&&run_target_params.chill_mode) ? IMAGE_KYPS_JA_JAAH_SININEN : IMAGE_KYPS_JA_JAAH_HARMAA + (nappi_pohjassa&3)], 
                                                nullptr, temp_saato_rounded_rect.rect.x - sidebar_on_shift_buttons_x, TEMP_SAATO_BOX_Y);
      else if (i==AIKA_LUKEMA_IDX)
         Button_group_add_images(i, image_ptrs[IMAGE_KYPS_JA_JAAH_HARMAA + ((nappi_pohjassa&0x0C)>>2)], nullptr, temp_saato_rounded_rect.rect.x - sidebar_on_shift_buttons_x, AIKA_SAATO_BOX_Y);
      else if (i==VESI_LUKEMA_IDX)
         Button_group_add_images(i, image_ptrs[IMAGE_KYPS_JA_JAAH_HARMAA + ((nappi_pohjassa&0x30)>>4)], nullptr, temp_saato_rounded_rect.rect.x - sidebar_on_shift_buttons_x, VESI_SAATO_BOX_Y);
      else if (i==MIX_LUKEMA_IDX)
         Button_group_add_images(i, image_ptrs[IMAGE_KYPS_JA_JAAH_HARMAA + ((nappi_pohjassa&0xC0)>>6)], nullptr, temp_saato_rounded_rect.rect.x - sidebar_on_shift_buttons_x, MIXER_SAATO_BOX_Y);
      Button_group_render(i);
      Button_groups[i].button_disabled = ylavalikko_naytolla; //HUOM!
   }
}

void nayta_chilling_values(void)
{
   update_chill_buttons_text();
   set_jaahdytys_buttons();
   nayta_aloita_painike();
}

void draw_JAAHDYTYS(int param1)
{
   //TEMPER.SÄÄTÖ
   SDL_SetRenderDrawColor(gRenderer, Rect1.color.r, Rect1.color.g, Rect1.color.b, Rect1.color.a);
   SDL_RenderFillRect(gRenderer, &(Rect1.rect));

   //AIKA SÄÄTÖ
   SDL_RenderFillRect(gRenderer, &(Rect2.rect));

   //VESI SÄÄTÖ
   SDL_RenderFillRect(gRenderer, &(Rect3.rect));

   //MIXER SÄÄTÖ
   if ((live_params.state & TARGET_STATE_SOMETHING_STARTED_BIT) && alavalikko_naytolla) //Painikkeessa VALMIS-teksti ja se on ylempänä jotta painike + sekoitinalavalikko mahtuu näytölle
      SDL_RenderFillRect(gRenderer, &(Rect4_small.rect));
   else
      SDL_RenderFillRect(gRenderer, &(Rect4.rect));

   if (param1 == 0) //1 = jos dialog näytöllä
   {
      nayta_chilling_values();
      show_banner(NUM_BUTTON_IDX);
      if (alavalikko_naytolla)
         nayta_alavalikko(NUM_BUTTON_IDX+1, ala_painike_painettu);
      else if (ylavalikko_naytolla)
         nayta_ylavalikko(NUM_BUTTON_IDX+1);
   }
   draw_kyps_jaahd_chill_icons();
   draw_kyps_jaahd_time_icons();
   draw_kyps_jaahd_water_icons();
   draw_kyps_jaahd_mixer_icons();
   if (sidebar_on_shift_buttons_x)
      show_sidebar();
}

int handler_JAAHDYTYS(int x, int y,  SDL_Event *e)
{
   nappi_pohjassa = 0;
   if (e->type && (no_button_ctr<2))
      no_button_ctr = 2;

   if( e->type == SDL_MOUSEMOTION)
   {
      if (!wait_finger_up && check_for_alavalikko(x, y, NUM_BUTTON_IDX+1))
         wait_finger_up = 1;
      draw_JAAHDYTYS(0);
      ala_painike_painettu = button_pressed = 0;
      trigger_button_up = show_numpad = 0; //TST 10.12
   }
   else
   {
      if(e->type == SDL_MOUSEBUTTONDOWN)
      {
         ala_painike_painettu = button_pressed = 0;
         int b = Button_group_check_for_key_press(x, y);
         if ((b >= CODE_KALLISTUS_ALAS && b <= CODE_KALLISTUS_YLOS))
            ala_painike_painettu = b;
         else if ((b >= CODE_YLAPALKKI_HACCP) && (b <= CODE_YLAPALKKI_BT))
            button_pressed = b;
         else if (b)
         {
            if (b == CODE_DIETA_LOGO)
               return ID_KOTINAKYMA;
            trigger_button_up = show_numpad = 0;

            --b;
            if (((b == TEMP_LUKEMA_IDX) || (b == TEMP_MIINUS_IDX) || (b == TEMP_PLUS_IDX)) && (nayta_mitatut_arvot & 0x01) && run_target_params.chill_mode)
            { //Jos mitattu lämpötila napissa niin painallus ei tee mitään, vapautus tuo asetusarvon painikkeeseen..tai 3s painallus stoppaa chillin
               if (long_key_press)
               {
                  nayta_mitatut_arvot &= ~0x01;
                  set_chilling(0);
                  prepare_target_params = run_target_params;
                  timeout_ctr = 0;
                  trigger_button_up = 0;
                  disable_repeat = 100000;
               }
               else
               {
                  disable_repeat = 5; //Tämä triggää long_key_press:in noin 3sek päästä
                  trigger_button_up = ~0x01;
               }
            }
            else
            {
               timeout_ctr = 0;
               if (b == LOPETA_IDX)
                  button_pressed = b + 1;
               else
                  handle_jaahd_button_press(b);
            }
         }
      }
      else if(e->type == SDL_MOUSEBUTTONUP)
      {
         if (trigger_button_up)
         {
            nayta_mitatut_arvot &= trigger_button_up;
            timeout_ctr = trigger_button_up = 0;
         }
         else if (button_pressed)
         {
            int menu = check_ylavalikko_valinta(button_pressed);
            if (menu && (menu != 0xFFFF))
               return menu;
            else if (--button_pressed == LOPETA_IDX)
            {
               button_pressed = 0;
               menu_to_return = ID_JAAHDYTYS;
               if (alavalikko_naytolla) //Painikkeessa VALMIS-teksti
               {
                  chilling_is_ready(1); //1 = Hold chill temp
                  return ID_VALMIS;
               }
               else
               {
                  set_ok_cancel_text(TXT_GROUP_YLEIS, YLEIS_TXT_HALUATKO_LOPETTAA_OHJELMAN);
                  return ID_OK_CANCEL;
               }
            }
         }
         else if (show_numpad)
         {
//            nayta_mitatut_arvot = 0; //Näytetään kaikista asetetut arvot
            set_numpad_parameter(show_numpad);
            menu_to_return = ID_JAAHDYTYS;
            if (show_numpad == NUMPAD_TARGET_SEKOITUS_VALINTA)
               return ID_SEK_TAPA_VALINTA;
            else
               return ID_NUMPAD;
         }
         ala_painike_painettu = button_pressed = 0;
         wait_finger_up = 0;
      }
      else if (e->type == 0) //Timeout
      {
         if (++timeout_ctr > 8) //5Sec
         {
            check_if_any_value_changed();
            nayta_mitatut_arvot |= (0x02 + (run_target_params.chill_mode!=0)); //Näytetään Aikaa jäljellä + temp
            timeout_ctr = 0;
         }
      }
      draw_JAAHDYTYS(0);

      if ((e->type == 0) && (--no_button_ctr <= 0))
      {
         if ((live_params.state & TARGET_STATE_SOMETHING_STARTED_BIT) && !(prepare_target_params.state & TARGET_STATE_CHANGE_GOING_BITS))
         {
            if ( ((chilling_vaihe == CHILLING_READY)||!run_target_params.chill_mode) && (!run_target_params.water_fill_amount_litres || (water_fill_vaihe==WATER_FILL_VESIMAARA_OK)) )
            {
               chilling_is_ready(1); //1 = Hold chill temp
               menu_to_return = ID_JAAHDYTYS;
               return ID_VALMIS;
            }
         }
      }
   }
   return 0xffff;
}

void init_JAAHDYTYS(void)
{
}

void show_JAAHDYTYS(int show_hide)
{
   if (show_hide == 0)
   {
      return;
   }
   no_button_ctr = 8;
   nappi_pohjassa = 0;
   ala_painike_painettu = button_pressed = 0;
   pyyhk_enabled = 1;
   if (multi_step_prog_going)
   {
      sidebar_on_shift_buttons_x = 260;
   }
   else
   {
      sidebar_on_shift_buttons_x = 0;
      prepare_target_params = run_target_params;
   }
   Button_group_empty();
   timeout_ctr = 0;
   show_numpad = 0;
   update_const_buttons_text();
   set_jaahdytys_buttons();
   draw_JAAHDYTYS(0);
}

void handle_jaahd_button_press(int but)
{
   switch (but)
   {
      case TEMP_LUKEMA_IDX:
         if (!(nayta_mitatut_arvot&0x01))
         {
            show_numpad = NUMPAD_TARGET_CHILLTEMP;
            numpad_selected_temp_mode = 4;//0=Norm.COOK, 1=vaippaohjaus, 2=delta, 3=ext temp, 4=Chill
            nappi_pohjassa = 0x0003;
         }
         nayta_mitatut_arvot &= ~0x01;
      break;

      case AIKA_LUKEMA_IDX:
         show_numpad = NUMPAD_TARGET_CHILL_TIME;
         nappi_pohjassa = 0x000C;
      break;

      case VESI_LUKEMA_IDX:
         show_numpad = NUMPAD_TARGET_WATER;
         nappi_pohjassa = 0x0030;
         set_water_fill(0); //Stopataan vesi
         prepare_target_params.water_fill_amount_litres = 0;
      break;

      case MIX_LUKEMA_IDX:
         if (onko_kansi_auki()) //kansiauki?
            return;
         show_numpad = NUMPAD_TARGET_SEKOITUS_VALINTA;
         nappi_pohjassa = 0x00C0;
      break;

      case TEMP_MIINUS_IDX:
         if (!run_target_params.chill_mode && !(prepare_target_params.state & TARGET_STATE_CHILL_TEMP_CHANGED_BIT))
            prepare_target_params.chill_temps[TARGET_CHILL_TEMP_IDX] = 30+1;

         if (!(nayta_mitatut_arvot&0x01) && prepare_target_params.chill_temps[TARGET_CHILL_TEMP_IDX])
            --prepare_target_params.chill_temps[TARGET_CHILL_TEMP_IDX];

         prepare_target_params.state |= TARGET_STATE_CHILL_TEMP_CHANGED_BIT;
         nayta_mitatut_arvot &= ~0x01;
         nappi_pohjassa = 1;
      break;

      case TEMP_PLUS_IDX:
         if (!run_target_params.chill_mode && !(prepare_target_params.state & TARGET_STATE_CHILL_TEMP_CHANGED_BIT))
            prepare_target_params.chill_temps[TARGET_CHILL_TEMP_IDX] = 30-1;

         if (!(nayta_mitatut_arvot&0x01) && (prepare_target_params.chill_temps[TARGET_CHILL_TEMP_IDX] < 55))
            ++prepare_target_params.chill_temps[TARGET_CHILL_TEMP_IDX];

         prepare_target_params.state |= TARGET_STATE_CHILL_TEMP_CHANGED_BIT;
         nayta_mitatut_arvot &= ~0x01;
         nappi_pohjassa = 2;
      break;

      case AIKA_MIINUS_IDX:
         if (run_target_params.chill_time_minutes)
         {
            --run_target_params.chill_time_minutes;
            run_target_params.chill_time_left_seconds = run_target_params.chill_time_minutes*60;
         }
         nayta_mitatut_arvot &= ~0x02;
         nappi_pohjassa = 1<<2;
      break;

      case AIKA_PLUS_IDX:
         if (run_target_params.chill_time_minutes < 24*60)
         {
            ++run_target_params.chill_time_minutes;
            run_target_params.chill_time_left_seconds = run_target_params.chill_time_minutes*60;
         }
         nayta_mitatut_arvot &= ~0x02;
         nappi_pohjassa = 2<<2;
      break;

      case VESI_MIINUS_IDX:
         vesi_miinus_painettu();
      break;

      case VESI_PLUS_IDX:
         vesi_plus_painettu();
      break;

      case MIX_PLUS_IDX:
         mix_plus_painettu();
      break;

      case MIX_MIINUS_IDX:
         mix_miinus_painettu();
      break;

      case LOPETA_IDX:  //--> VALMIS-menuun
      break;
   }
}

void chilling_is_ready(int hold_temp)
{
   nayta_mitatut_arvot = 0; //Näytetään kaikista asetetut arvot
   live_params.state = 0; // &= ~TARGET_STATE_SOMETHING_STARTED_BIT;
   if (hold_temp == 0)
      set_chilling(0);
   set_water_fill(0);
   set_mixing(0);
   set_tilting(0);
   run_target_params.state = 0;
   prepare_target_params = run_target_params;
}

void vesi_plus_painettu(void)
{
   if (nayta_mitatut_arvot&0x04)
   {
      int was_going = run_target_params.water_fill_mode;
      set_water_fill(0); //Stopataan vesi
      prepare_target_params.water_fill_amount_litres = 0;
      if (was_going == 0) //Jos oli menossa niin nollataan, muuten lähdetään 1:stä
        prepare_target_params.water_fill_amount_litres = 1;
   }
   else if (prepare_target_params.water_fill_amount_litres < 500)
   {
      ++prepare_target_params.water_fill_amount_litres;
   }
   nayta_mitatut_arvot &= ~0x04;
   prepare_target_params.state |= TARGET_STATE_WATER_FILL_CHANGED_BIT;
   nappi_pohjassa = 2<<4;
}

void vesi_miinus_painettu(void)
{
   if (nayta_mitatut_arvot&0x04)
   {
      set_water_fill(0); //Stopataan vesi
      prepare_target_params.water_fill_amount_litres = 0;
   }
   else if (prepare_target_params.water_fill_amount_litres)
   {
      --prepare_target_params.water_fill_amount_litres;
   }
   nayta_mitatut_arvot &= ~0x04;
   prepare_target_params.state |= TARGET_STATE_WATER_FILL_CHANGED_BIT;
   nappi_pohjassa = 1<<4;
}

void mix_plus_painettu(void)
{
   if (onko_kansi_auki())
      return;

   int max_speed = max_nopeudet[run_target_params.mixer_mode];
   run_target_params.mixer_speed += 20;

   if (run_target_params.mixer_speed > max_speed)
      run_target_params.mixer_speed = max_speed;

   if (run_target_params.mixer_mode == 0)
   {
      run_target_params.mixer_speed = 20;
      if (run_target_params.selected_mixer_mode == 0)
         run_target_params.selected_mixer_mode = MIX_MODES_YHTEEN_SUUNTAAN;
      set_mixing(run_target_params.selected_mixer_mode); //starttaa valitulla modella JA asetetulla nopeudella.
   }
   nayta_mitatut_arvot &= ~0x08;
   nappi_pohjassa = 2<<6;
}

void mix_miinus_painettu(void)
{
   if (onko_kansi_auki())
      return;
   run_target_params.mixer_speed -= 20;
   if (run_target_params.mixer_speed <= 0)
   {
      set_mixing(0);
   }
   nayta_mitatut_arvot &= ~0x08;
   nappi_pohjassa = 1<<6;
}
