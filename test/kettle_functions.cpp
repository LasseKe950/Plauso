/* File: kettle_functions.cpp
   Purpose:
      Grafiikka tilakoneet
   Documents:
      "Datan kulku v2.txt"

*/
#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "menu_ids.h"
#include "handler.h"
#include "var.h"
#include "kettle_functions.h"
#include "externs.h"
#include "tekstit.h"

#include "vaihe_texts.cpp"

unsigned char water_measure_error, food_sensor_error, kettle_sensor_error, hataseis_painettu, kontaktori_vika, painekytkin_lauennut, vast_ylikuum_suoja_lauennut;

void Kiinnipalamisen_esto(void); //asettaa vaippa_max:in
int vaippa_max;

int cooking_vaihe, chilling_vaihe, emptying_vaihe, water_fill_vaihe;
int tilt_motor_vaihe, tilt_motor_dir;
int mixer_motor_vaihe;

int prev_cooking_vaihe, prev_chilling_vaihe, prev_emptying_vaihe, prev_water_fill_vaihe;
int prev_tilt_motor_vaihe;
int prev_mixer_motor_vaihe;
int prev_kansi_auki;

int cooking_uimuri_alhaalla_ctr;
int cooking_uimuri_ylhaalla_ctr;
int emptying_uimuri_alhaalla_ctr;
int temp_ok_ctr;

Uint32 cook_chill_time_next_second;
Uint32 water_fill_timeout;
int prev_litres;

#define CTRL_INTERVAL_MS 4000
Uint32 cook_chill_time_next_ctrl;

int tilt_motor_start_ctr;
Uint32 max_tilt_time_ctrl;

void clear_all_kettle_functions(void)
{
   set_cooking(0);
   set_chilling(0);
   set_emptying(0);
   set_water_fill(0);
   set_mixing(0);
   set_tilting(0);

   prev_cooking_vaihe = 0;
   prev_chilling_vaihe = 0;
   prev_emptying_vaihe = 0;
   prev_water_fill_vaihe = 0;
   prev_tilt_motor_vaihe = 0;
   prev_mixer_motor_vaihe = 0;
}

void set_cooking(int start)
{
   if (start)
   {
      if (chilling_vaihe)
         return;
      if (cooking_vaihe)
         return; //already on
      cooking_vaihe = COOKING_VAIPAN_TYHJENNYS_ALUKSI;
      if (emptying_vaihe == EMPTYING_OFF)
         set_emptying(1);
      run_target_params.cook_mode = 1;
      temp_ok_ctr = cook_chill_time_next_ctrl = 0;
   }
   else //stop
   {
      cooking_vaihe = 0;
      run_target_params.cook_mode = 0;
      run_target_params.temp_reached = 0;
      run_target_params.cook_time_minutes = 0;
      run_target_params.cook_time_left_seconds = 0;
   }
}

void handle_cooking(void)
{ //Jos pataa kallistetaan, lämmitys on estetty
   if (prev_cooking_vaihe != cooking_vaihe)
   {
      printf("%s\n", cook_vaihe_texts[cooking_vaihe]);
      prev_cooking_vaihe = cooking_vaihe;
      cooking_uimuri_ylhaalla_ctr = cooking_uimuri_alhaalla_ctr = 0;
   }
   if (cooking_vaihe)
      chilling_vaihe=0;

   switch(cooking_vaihe)
   {
      case COOKING_OFF:
         output_registers[OUTPUT_REG_CARD2_DIGITAL_OUTPUTS] &= ~(HEAT_24V_OUT_BIT_CARD2);
         return;
      break;

      case COOKING_VAIPAN_TYHJENNYS_ALUKSI: //timeout:illa
         output_registers[OUTPUT_REG_CARD2_DIGITAL_OUTPUTS] &= ~(HEAT_24V_OUT_BIT_CARD2 + VAIPPAVESI_MV1_24V_OUT_BIT_CARD2);
         if ((emptying_vaihe == EMPTYING_READY) || (emptying_vaihe == EMPTYING_TIMED_OUT))
         {
            emptying_vaihe = 0;
            cooking_vaihe = COOKING_VAIPPAVEDEN_TAYTTO;
            run_target_params.cook_timeout = 0;
            cooking_uimuri_ylhaalla_ctr = cooking_uimuri_alhaalla_ctr = 0;
         }
      break;

      case COOKING_VAIPPAVEDEN_TAYTTO: //täytetään uimuriin asti ennenkuin vastukset voi laittaa päälle
         output_registers[OUTPUT_REG_CARD2_DIGITAL_OUTPUTS] &= ~HEAT_24V_OUT_BIT_CARD2;
         output_registers[OUTPUT_REG_CARD2_DIGITAL_OUTPUTS] |= VAIPPAVESI_MV1_24V_OUT_BIT_CARD2; //Vaippaan vettä
         if (onko_uimuri_ylhaalla())
         {
            if (++cooking_uimuri_ylhaalla_ctr > COOKING_UIMURI_YLHAALLA_CTR)
               cooking_vaihe = COOKING_VAIPPAVESI_OK;
         }
         else
            cooking_uimuri_ylhaalla_ctr = 0;
      break;

      case COOKING_READY:
      case COOKING_VAIPPAVESI_OK: //lämmitetään moodin mukaan -> target-temp, tai takaisin vaippaveden täyttöön
         output_registers[OUTPUT_REG_CARD2_DIGITAL_OUTPUTS] &= ~VAIPPAVESI_MV1_24V_OUT_BIT_CARD2; //EI Vaippaan vettä

         if (SDL_TICKS_PASSED(SDL_GetTicks(), cook_chill_time_next_ctrl))
         {
            cook_chill_time_next_ctrl = SDL_GetTicks() + CTRL_INTERVAL_MS;
            if (live_params.cook_temps[TARGET_COOK_FOOD_TEMP_IDX] >= run_target_params.cook_temps[TARGET_COOK_FOOD_TEMP_IDX])
               output_registers[OUTPUT_REG_CARD2_DIGITAL_OUTPUTS] &= ~HEAT_24V_OUT_BIT_CARD2;
            else
               output_registers[OUTPUT_REG_CARD2_DIGITAL_OUTPUTS] |= HEAT_24V_OUT_BIT_CARD2;
            Kiinnipalamisen_esto();
         }

         if (onko_uimuri_ylhaalla() == 0) //uimuri alhaalla?
         {
            if (++cooking_uimuri_alhaalla_ctr > COOKING_UIMURI_ALHAALLA_CTR)
               cooking_vaihe = COOKING_VAIPPAVEDEN_TAYTTO;
         }
         else
            cooking_uimuri_alhaalla_ctr = 0;
      break;

      //case COOKING_READY:
      //   output_registers[OUTPUT_REG_CARD1_DIGITAL_OUTPUTS] &= ~(HEAT_24V_OUT_BIT_CARD1 + CHILL_24V_OUT_BIT_CARD1);
      //break;

      default:
         cooking_vaihe = 0;
         run_target_params.cook_mode = 0;
      break;
   }

   if (onko_kallistettu()) //Kallistettu --> Heat + Vaippavesi POIS
      output_registers[OUTPUT_REG_CARD2_DIGITAL_OUTPUTS] &= ~(HEAT_24V_OUT_BIT_CARD2 + VAIPPAVESI_MV1_24V_OUT_BIT_CARD2);

   if (cooking_vaihe)
   {
      if (live_params.cook_temps[TARGET_COOK_FOOD_TEMP_IDX] >= run_target_params.cook_temps[TARGET_COOK_FOOD_TEMP_IDX])
         ++temp_ok_ctr;
      else
         temp_ok_ctr = 0;
      if (temp_ok_ctr > 2) //Lämpötila täytyy olla pari kertaa ollut targetissa ennenkuin hyväksytään
      {
         if (run_target_params.temp_reached == 0)
         {
            printf("Cooking TEMP reached!\n");
            run_target_params.cook_time_left_seconds = run_target_params.cook_time_minutes*60;
            cook_chill_time_next_second = SDL_GetTicks() + 1000;
         }
         run_target_params.temp_reached = 1;
      }
   }
   if (run_target_params.temp_reached)
   {
      if (run_target_params.cook_time_left_seconds == 0)
      {
         if (cooking_vaihe != COOKING_READY)
            printf("Cooking ready!\n");
         cooking_vaihe = COOKING_READY;
      }
      else
      {
         if (SDL_TICKS_PASSED(SDL_GetTicks(), cook_chill_time_next_second))
         {
            cook_chill_time_next_second = SDL_GetTicks() + 1000;
            --run_target_params.cook_time_left_seconds;
            //printf("Seconds left=%d\n", run_target_params.cook_time_left_seconds);
         }
      }
   }
}

void set_chilling(int start)
{
   if (start)
   {
      if (cooking_vaihe)
         return;
      set_emptying(0); //Jos Chill niin ei mitään tyhjennyksiä alkuun!
      if (chilling_vaihe)
         return; //already on
      chilling_vaihe = CHILLING_WAIT_IF_100C;
      run_target_params.chill_mode = 1;
      temp_ok_ctr = cook_chill_time_next_ctrl = 0;
   }
   else //stop
   {
      chilling_vaihe = 0;
      run_target_params.chill_mode = 0;
      run_target_params.temp_reached = 0;
      run_target_params.chill_time_minutes = 0;
      run_target_params.chill_time_left_seconds = 0;
   }
}

void chilling_outs_off(void)
{
   if (!cooking_vaihe && !emptying_vaihe)
   {
      output_registers[OUTPUT_REG_CARD2_DIGITAL_OUTPUTS] &= ~VAIPPAVESI_MV1_24V_OUT_BIT_CARD2;
      output_registers[OUTPUT_REG_CARD1_DIGITAL_OUTPUTS] &= ~BYBASS_MV6_24V_OUT_BIT_CARD1;
   }
}

void chilling_outs_on(void)
{
   output_registers[OUTPUT_REG_CARD2_DIGITAL_OUTPUTS] |= VAIPPAVESI_MV1_24V_OUT_BIT_CARD2;
   output_registers[OUTPUT_REG_CARD1_DIGITAL_OUTPUTS] |= BYBASS_MV6_24V_OUT_BIT_CARD1;
}

void handle_chilling(void)
{ 
   //Jos pataa kallistetaan, jäähdytysveden otto estetty
   if (prev_chilling_vaihe != chilling_vaihe)
   {
      printf("%s\n", chill_vaihe_texts[chilling_vaihe]);
      prev_chilling_vaihe = chilling_vaihe;
   }

   if (chilling_vaihe)
      emptying_vaihe = cooking_vaihe = 0;

   switch(chilling_vaihe)
   {
      case CHILLING_OFF:
         chilling_outs_off();
         return;
      break;

      case CHILLING_WAIT_IF_100C: //Tähän timeout!
         if (live_params.cook_temps[TARGET_COOK_KETTLE_TEMP_IDX] < 100)
            chilling_vaihe = CHILLING_1_VAIHE_CHILL;
         chilling_outs_off();
      break;

      case CHILLING_2_VAIHE_CHILL_VAIHE_1:
      break;

      case CHILLING_2_VAIHE_CHILL_VAIHE_2:
      break;

      case CHILLING_1_VAIHE_CHILL:
      break;

      case CHILLING_READY:
         //output_registers[OUTPUT_REG_CARD1_DIGITAL_OUTPUTS] &= ~COLD_WATER_24V_OUT_BIT_CARD1;
      break;

      default:
         chilling_vaihe = 0;
         run_target_params.chill_mode = 0;
         chilling_outs_off();
      break;
   }

   if (chilling_vaihe)
   {  //Huom! mitattu temp = live_params.cook_temps !
      if (live_params.cook_temps[TARGET_COOK_FOOD_TEMP_IDX] <= run_target_params.chill_temps[TARGET_CHILL_TEMP_IDX])
         ++temp_ok_ctr;
      else
         temp_ok_ctr = 0;
   }
   if ((chilling_vaihe==CHILLING_2_VAIHE_CHILL_VAIHE_1) || (chilling_vaihe==CHILLING_2_VAIHE_CHILL_VAIHE_2) || (chilling_vaihe==CHILLING_1_VAIHE_CHILL) || (chilling_vaihe==CHILLING_READY))
   { //Chill aktiivinen, säädetään 4 sek. välein, seurataan jäähd. etenemistä --> Tähänkin timeout!
      if (SDL_TICKS_PASSED(SDL_GetTicks(), cook_chill_time_next_ctrl))
      {
         cook_chill_time_next_ctrl = SDL_GetTicks() + CTRL_INTERVAL_MS;
         if (temp_ok_ctr)
            chilling_outs_off();
         else
            chilling_outs_on();
      }
   }

   if (temp_ok_ctr > 2) //Lämpötila täytyy olla pari kertaa ollut targetissa ennenkuin hyväksytään.
   {
      if (run_target_params.temp_reached == 0)
      {
         run_target_params.chill_time_left_seconds = run_target_params.chill_time_minutes*60;
         printf("CHILLING TEMP reached!\n");
         cook_chill_time_next_second = SDL_GetTicks() + 1000;
      }
      run_target_params.temp_reached = 1;
   }
   if (run_target_params.temp_reached)
   {
      if (run_target_params.chill_time_left_seconds == 0)
      {
         if (chilling_vaihe != CHILLING_READY)
            printf("Chilling ready!\n");
         chilling_vaihe = CHILLING_READY;
      }
      else
      {
         if (SDL_TICKS_PASSED(SDL_GetTicks(), cook_chill_time_next_second))
         {
            cook_chill_time_next_second = SDL_GetTicks() + 1000;
            --run_target_params.chill_time_left_seconds;
            //printf("Seconds left=%d\n", run_target_params.cook_time_left_seconds);
         }
      }
   }
   if (onko_kallistettu()) //Kallistettu --> Heat + Vaippavesi POIS
      chilling_outs_off();
}

void set_emptying(int start)
{
   if (start)
   {
      if (emptying_vaihe && (emptying_vaihe != EMPTYING_TIMED_OUT))
         return; //already on
      emptying_vaihe = EMPTYING_WAIT_IF_55C;
      run_target_params.emptying_mode = 1;
      run_target_params.emptying_timeout = 0;
   }
   else //stop
   {
      emptying_vaihe = 0;
      run_target_params.emptying_mode = 0;
   }
}

void emptying_outs_off(void)
{
   if (chilling_vaihe==0)
      output_registers[OUTPUT_REG_CARD1_DIGITAL_OUTPUTS] &= ~(EMPTYING_MV4_24V_OUT_BIT_CARD1 | BYBASS_MV6_24V_OUT_BIT_CARD1 | TYHJENNYS_PUMPPU_OUT_BIT_CARD1);
   else
      output_registers[OUTPUT_REG_CARD1_DIGITAL_OUTPUTS] &= ~(EMPTYING_MV4_24V_OUT_BIT_CARD1 | TYHJENNYS_PUMPPU_OUT_BIT_CARD1);
   //MV7 pidetään aina avattuna paitsi 2v-jäähd. toisessa vaiheessa ??
   output_registers[OUTPUT_REG_CARD1_DIGITAL_OUTPUTS] |= ILMANPOISTIN_MV7_24V_OUT_BIT_CARD1;
}

void emptying_outs_on(void)
{
   output_registers[OUTPUT_REG_CARD1_DIGITAL_OUTPUTS] |= (EMPTYING_MV4_24V_OUT_BIT_CARD1 | BYBASS_MV6_24V_OUT_BIT_CARD1 | ILMANPOISTIN_MV7_24V_OUT_BIT_CARD1 | TYHJENNYS_PUMPPU_OUT_BIT_CARD1);
}

void handle_emptying(void)
{
   if (prev_emptying_vaihe != emptying_vaihe)
   {
      printf("%s\n", empty_vaihe_texts[emptying_vaihe]);
      prev_emptying_vaihe = emptying_vaihe;
      emptying_uimuri_alhaalla_ctr = 0;
   }

   switch(emptying_vaihe)
   {
      case EMPTYING_OFF:
         emptying_outs_off();
         run_target_params.emptying_mode = 0;
         return;
      break;

      case EMPTYING_WAIT_IF_55C: //odota että vaippaölämpö laskee alle 55C, timeout:illa
         emptying_outs_off();
         if (live_params.cook_temps[TARGET_COOK_KETTLE_TEMP_IDX] < 55)
            emptying_vaihe = EMPTYING_GOING;
         if (++run_target_params.emptying_timeout > EMPTYING_TIMEOUT_SEC)
            emptying_vaihe = EMPTYING_TIMED_OUT; //Log event
      break;

      case EMPTYING_GOING:
         emptying_outs_on();
         if (onko_uimuri_ylhaalla() == 0) //uimuri alhaalla?
         {
            if (++emptying_uimuri_alhaalla_ctr > EMPTYING_UIMURI_ALHAALLA_CTR)
               emptying_vaihe = EMPTYING_READY;
         }
         else
            emptying_uimuri_alhaalla_ctr = 0;
         if (++run_target_params.emptying_timeout > EMPTYING_TIMEOUT_SEC)
            emptying_vaihe = EMPTYING_TIMED_OUT; //Log event
      break;

      case EMPTYING_TIMED_OUT:
         emptying_outs_off();
         //emptying_vaihe = EMPTYING_OFF;
      break;

      case EMPTYING_READY:
         emptying_outs_off();
      break;

      default:
         emptying_vaihe = EMPTYING_OFF;
         run_target_params.emptying_mode = 0;
      break;
   }

   if (onko_kallistettu())
      emptying_outs_off();
}

void set_water_fill(int start)
{
   if (start)
   {
      water_measure_error = 0;
      water_fill_timeout = SDL_GetTicks() + 20000;
      if (water_fill_vaihe)
         return; //already on
      water_fill_vaihe = WATER_FILL_MITTAA_VESIMAARA;
      run_target_params.water_fill_mode = 1; //1=normaali, 2=hot
      output_registers[OUTPUT_REG_CARD2_MITTAA_VESILITRAT] = run_target_params.water_fill_amount_litres + 0x8000;//bitti 0x8000 + vesimäärä => aloita tämän vesimäärän laskeminen nyt
   }
   else //stop
   {
      water_fill_vaihe = 0;
      run_target_params.water_fill_amount_litres = 0;
      live_params.water_fill_amount_litres = 0;
      //run_target_params.water_fill_amount_total = 0;
      run_target_params.water_fill_mode = 0;
      output_registers[OUTPUT_REG_CARD2_MITTAA_VESILITRAT] = 0;
      output_registers[OUTPUT_REG_CARD2_MITTAA_VESILITRAT_HOT] = 0;
   }
}

void handle_water_fill(void)
{
   if (prev_water_fill_vaihe != water_fill_vaihe)
   {
      printf("%s\n", water_fill_vaihe_texts[water_fill_vaihe]);
      prev_water_fill_vaihe = water_fill_vaihe;
   }

   switch(water_fill_vaihe)
   {
      case WATER_FILL_OFF:
         output_registers[OUTPUT_REG_CARD2_MITTAA_VESILITRAT] = 0;
         output_registers[OUTPUT_REG_CARD2_MITTAA_VESILITRAT_HOT] = 0;
      break;

      case WATER_FILL_MITTAA_VESIMAARA:
      case WATER_FILL_MITTAA_VESIMAARA_HOT:
         if (live_params.water_fill_amount_litres >= run_target_params.water_fill_amount_litres)
         {
            water_fill_vaihe = WATER_FILL_VESIMAARA_OK;
            output_registers[OUTPUT_REG_CARD2_MITTAA_VESILITRAT] = 0;
            output_registers[OUTPUT_REG_CARD2_MITTAA_VESILITRAT_HOT] = 0;
            live_params.water_fill_amount_litres = run_target_params.water_fill_amount_litres = 0;
            prepare_target_params.water_fill_amount_litres = 0;
         }
         else if (prev_litres == live_params.water_fill_amount_litres)
         {
            if (SDL_TICKS_PASSED(SDL_GetTicks(), water_fill_timeout))
            { //WATER MEASURE ERROR!
               printf("%s\n", (char*)error_tekstit[ERROR_IDX_VESIMITTARI_ERROR]);
               water_fill_timeout = SDL_GetTicks() + 20000;
               water_measure_error = 1;
            }
         }
         else
         {
            prev_litres = live_params.water_fill_amount_litres;
            water_fill_timeout = SDL_GetTicks() + 20000;
         }
      break;

      break;

      case WATER_FILL_VESIMAARA_OK:
         if (live_params.water_fill_amount_litres < run_target_params.water_fill_amount_litres)
            water_fill_vaihe = WATER_FILL_MITTAA_VESIMAARA;
      break;

      case WATER_FILL_ERROR:
         water_fill_vaihe = 0;
      break;

      default:
         water_fill_vaihe = 0;
      break;
   }

   if (output_registers[OUTPUT_REG_CARD2_MITTAA_VESILITRAT] || output_registers[OUTPUT_REG_CARD2_MITTAA_VESILITRAT_HOT])
      run_target_params.water_fill_mode = 1;
   else
      run_target_params.water_fill_mode = 0;
}

void set_mixing(int start_mode)
{
   prev_kansi_auki = -1;
   if (start_mode && (start_mode < NUM_MIX_MODES)) //mode = enum mix_modes = 1-8, 0=ei käynnissä 
   {
      mixer_motor_vaihe = MIXER_MOTOR_RUN_ETEEN; //Lähtösuunta: kumpi lienee oikea?
      run_target_params.mixer_mode = start_mode;
      run_target_params.selected_mixer_mode = run_target_params.mixer_mode;
      MixerTask(MSG_TYPE_MIXER_PROG_CHG_MSG);
   }
   else //stop
   {
      mixer_motor_vaihe = 0;
      run_target_params.mixer_mode = 0;
      run_target_params.mixer_speed = 0;
   }
}

void handle_mixing(void)
{ //Jos kansi avataan, sekoitus loppuu
   if (prev_mixer_motor_vaihe != mixer_motor_vaihe)
   {
      printf("%s\n", mixer_motor_vaihe_texts[mixer_motor_vaihe]);
      prev_mixer_motor_vaihe = mixer_motor_vaihe;
   }

   MixerTask(0);

   switch(mixer_motor_vaihe)
   {
      case MIXER_MOTOR_STOPPED:
         run_target_params.mixer_mode = 0;
         run_target_params.mixer_speed = 0;
      break;

      case MIXER_MOTOR_RAMP_ETEEN:
      break;

      case MIXER_MOTOR_RUN_ETEEN:
         output_registers[OUTPUT_REG_CARD2_DIGITAL_OUTPUTS] |= INVERTER_FWD_3V_OUT_BIT_CARD2;
      break;

      case MIXER_MOTOR_RAMP_TAAKSE:
      break;

      case MIXER_MOTOR_RUN_TAAKSE:
         output_registers[OUTPUT_REG_CARD2_DIGITAL_OUTPUTS] &= ~INVERTER_FWD_3V_OUT_BIT_CARD2;
      break;

      case MIXER_MOTOR_IN_PAUSE:
         //run_target_params.mixer_speed = 0;
      break;

      case MIXER_MOTOR_PULSED_STATE:
         output_registers[OUTPUT_REG_CARD2_DIGITAL_OUTPUTS] |= 
                    (INVERTER_ON_3V_OUT_BIT_CARD2 | INVERTER_TURVASEK_RELE1_3V_OUT_BIT_CARD2 | INVERTER_TURVASEK_RELE2_3V_OUT_BIT_CARD2 | INVERTER_FWD_3V_OUT_BIT_CARD2); //Inverter on
         //run_target_params.mixer_speed = 20;
         output_registers[OUTPUT_REG_CARD2_ANALOG_OUTPUT_1] = 20 / 2;
         return;
      break;

      case MIXER_MOTOR_ERROR:
         mixer_motor_vaihe = 0;
      break;

      default:
         mixer_motor_vaihe = 0;
      break;
   }

   if (onko_kansi_auki())
   {
      if (prev_kansi_auki != 1)
      {
         printf("KANSI AUKI!\n");
         kansi_avattiin = 1;
         mix_speed_kun_kansi_avattiin = run_target_params.mixer_speed;
      }
      prev_kansi_auki = 1;
      run_target_params.mixer_speed = 0;
//      run_target_params.mixer_mode = 0;
   }
   else
   {
      if (prev_kansi_auki != 0)
      {
         printf("KANSI KIINNI!\n");
         kansi_suljettiin = 1;
      }
      prev_kansi_auki = 0;
   }

   if (run_target_params.mixer_mode==0)
      run_target_params.mixer_speed = 0;
   else
   {
      int max_speed = max_nopeudet[run_target_params.mixer_mode];
      if (run_target_params.mixer_speed > max_speed)
         run_target_params.mixer_speed = max_speed;
   }

   if (mixer_motor_vaihe == MIXER_MOTOR_IN_PAUSE)
      output_registers[OUTPUT_REG_CARD2_ANALOG_OUTPUT_1] = 0;
   else
      output_registers[OUTPUT_REG_CARD2_ANALOG_OUTPUT_1] = run_target_params.mixer_speed / 2;

   if (run_target_params.mixer_speed)
   {
      output_registers[OUTPUT_REG_CARD2_DIGITAL_OUTPUTS] |= INVERTER_ON_3V_OUT_BIT_CARD2; //Inverter on
   }
   else
   {
      output_registers[OUTPUT_REG_CARD2_DIGITAL_OUTPUTS] &= ~(INVERTER_ON_3V_OUT_BIT_CARD2 | INVERTER_TURVASEK_RELE1_3V_OUT_BIT_CARD2 | INVERTER_TURVASEK_RELE2_3V_OUT_BIT_CARD2);
   }

   if (current_menu_id != ID_IOTEST)
   {
      //Invertterin rele kytkee kontaktorilla invertterille sähköt, tämä voi olla koko ajan päällä paitsi hätäseis-tilassa.
      output_registers[OUTPUT_REG_CARD2_DIGITAL_OUTPUTS] |= INV_RELE_24V_OUT_BIT_CARD2;
   }
}

/* Kallistuksen ohjaus:
   handler.cpp:
   -jos joku muu event kuin "nappi pohjassa" --> kallistus pois, set_tilting(0)
   -jos Down/UP-nappi painettuna
*/
void set_tilting(int start)
{
   if (start)
   {
      if (onko_kansi_auki() == 0)//kansi kiinni?
      {
         tilt_motor_vaihe = 0;
         return;
      }
      if (run_target_params.tilt_mode == 1) //1=UP, 2=Down
      {
         tilt_motor_vaihe = TILT_MOTOR_RUN_UP;
      }
      else if (run_target_params.tilt_mode == 2) //1=UP, 2=Down
      {
         tilt_motor_vaihe = TILT_MOTOR_RUN_DOWN;
      }
      output_registers[OUTPUT_REG_CARD2_DIGITAL_OUTPUTS] &= ~(TILT_UP_24V_OUT_BIT_CARD2 + TILT_DOWN_24V_OUT_BIT_CARD2);
   }
   else
   {
      output_registers[OUTPUT_REG_CARD2_DIGITAL_OUTPUTS] &= ~(TILT_UP_24V_OUT_BIT_CARD2 + TILT_DOWN_24V_OUT_BIT_CARD2);
      run_target_params.tilt_mode = tilt_motor_vaihe = 0;
   }
}

void handle_tilting(void)
{
   if (prev_tilt_motor_vaihe != tilt_motor_vaihe)
   {
      printf("%s\n", tilt_motor_vaihe_texts[tilt_motor_vaihe]);
      prev_tilt_motor_vaihe = tilt_motor_vaihe;
      max_tilt_time_ctrl = SDL_GetTicks() + 25000; //25s 
   }

   // Kallistus estetty:
   //   -Jos kansi on kiinni
   //   -Jos tyhjennys menossa
   if (onko_kansi_auki() == 0)
      tilt_motor_vaihe = 0;
   if (emptying_vaihe && (emptying_vaihe != EMPTYING_READY) && (emptying_vaihe != EMPTYING_TIMED_OUT))
      tilt_motor_vaihe = 0;

   switch(tilt_motor_vaihe)
   {
      case TILT_MOTOR_STOPPED:
         output_registers[OUTPUT_REG_CARD2_DIGITAL_OUTPUTS] &= ~(TILT_UP_24V_OUT_BIT_CARD2 + TILT_DOWN_24V_OUT_BIT_CARD2);
         run_target_params.tilt_mode = 0;
      break;

      case TILT_MOTOR_RAMP_UP:
      break;

      case TILT_MOTOR_RUN_UP:
         output_registers[OUTPUT_REG_CARD2_DIGITAL_OUTPUTS] &= ~(TILT_UP_24V_OUT_BIT_CARD2 + TILT_DOWN_24V_OUT_BIT_CARD2);
         if (SDL_TICKS_PASSED(SDL_GetTicks(), max_tilt_time_ctrl))
            break;
         output_registers[OUTPUT_REG_CARD2_DIGITAL_OUTPUTS] |= TILT_UP_24V_OUT_BIT_CARD2;
      break;

      case TILT_MOTOR_RAMP_DOWN:
      break;

      case TILT_MOTOR_RUN_DOWN:
         output_registers[OUTPUT_REG_CARD2_DIGITAL_OUTPUTS] &= ~(TILT_UP_24V_OUT_BIT_CARD2 + TILT_DOWN_24V_OUT_BIT_CARD2);
         if (SDL_TICKS_PASSED(SDL_GetTicks(), max_tilt_time_ctrl))
            break;
         output_registers[OUTPUT_REG_CARD2_DIGITAL_OUTPUTS] |= TILT_DOWN_24V_OUT_BIT_CARD2;
      break;

      case TILT_MOTOR_ERROR:
      break;

      default:
         tilt_motor_vaihe = 0;
      break;
   }
}

//		-hätäseis. Pysäyttää kaiken ja lähdöt nollataan.
int check_emergency_stop(void)
{
   return 0;
}

//-anturiviat, kontaktori, kommunikaatio-error yms.
int check_errors(void)
{
   return 0;
}

//2-vaihejäähdytys Mohjain:
//  v2.53 2-vaihejäähdytys vaakakortin io:lla, vaihe 1 => avataan: MV1 (alakortti CHILL WATER X609:4) + MV6+MV7 (tässä), 
//  k20_outputs |= (VAAKA_OUTPUT_MV6 | VAAKA_OUTPUT_MV7); //out2=MV6 läpivuotoventtiili
//
//  v2.53 2-vaihejäähdytys vaakakortin io:lla, vaihe 2 => suljetaan: MV1 (alak.)+MV6+MV7, avataan: MV2 (alak), MV5 (alak), R3
//  out2=MV6 läpivuotoventtiili, out3=MV7 ilmaventtiili kiinni. MV7 pidetään aina avattuna paitsi 2v-jäähd. toisessa vaiheessa
//  k20_outputs &= ~(VAAKA_OUTPUT_MV6|VAAKA_OUTPUT_MV7);
//  if (Read_food_temp > Set_chill_temp)
//   k20_outputs |= VAAKA_OUTPUT_R3_JAAPANKKI;

/* Kiinnipalamisen esto 
- Jos asetuslämpö on > 99 niin vaippaa voidaan lämmittää asetuslämpöön asti.
- Jos asetuslämpö on [93...99] niin vaippaa voidaan lämmittää asetus +12C asti.
- Jos on [40...92] vaippa saa lämmetä 105 C:een.
- Jos asetuslämpö < 40, niin vaippa saa lämmetä 46C:een
*/
void Kiinnipalamisen_esto(void)
{
   int asetettu_temp = run_target_params.cook_temps[TARGET_COOK_FOOD_TEMP_IDX];
   if (asetettu_temp > 99) // 100 tai enemmän
      vaippa_max = asetettu_temp;
   else if (asetettu_temp > 92) //93..99
      vaippa_max = asetettu_temp + 12;
   else if (asetettu_temp > 39) //40..92
      vaippa_max = 105;
   else // alle 40
      vaippa_max = 46;
   if (live_params.cook_temps[TARGET_COOK_KETTLE_TEMP_IDX] > vaippa_max)
      output_registers[OUTPUT_REG_CARD2_DIGITAL_OUTPUTS] &= ~HEAT_24V_OUT_BIT_CARD2;
}

int onko_kansi_auki(void)
{
   if ((input_registers[INPUT_REG_CARD2_DIGITAL_INPUTS] & LID_SWITCH1_IN_BIT_CARD2) !=0 ) //kansiauki?
      return 1;
   else
      return 0;
}

int onko_kallistettu(void)
{
   if ((input_registers[INPUT_REG_CARD2_DIGITAL_INPUTS] & KETTLE_POSITION_IN_BIT_CARD2) != 0)
      return 1;
   else
      return 0;
}

int onko_uimuri_ylhaalla(void)
{
   if ((input_registers[INPUT_REG_CARD1_DIGITAL_INPUTS] & FLOAT1_IN_BIT_CARD1) !=0 ) //uimuri ylhaalla?
      return 1;
   else
      return 0;
}

int onko_jalka_kytkin_painettu(void)
{
   if ((input_registers[INPUT_REG_CARD2_DIGITAL_INPUTS] & JALKA_KYTKIN_IN_BIT_CARD2) == 0 )
      return 1;
   else
      return 0;
}