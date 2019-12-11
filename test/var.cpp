/* File: var.cpp
   Purpose: variables
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "var.h"
#include "kettle_functions.h"
#include "externs.h"
#include "menu_ids.h"
#include "handler.h"

user_profiili_s user_profile;
kettle_options_s kettle_options;

/*Tehdasasetukset:*/
const kettle_options_s default_kettle_options={
	94,  //cook_temp
	3,  //chill_temp
	99,  //two_phase_chill
	70,  //haccp_high
	3,  //haccp_low
	15,  //cook_time_min
	 0, //chill_time_min
	60,  //chill_speed
	1,  //mix_prog
	5,  //short_mix_sec
	15, //long_mix_sec
	60,  //mix_short_pause_sec
	180, //mix_long_pause_sec
	0, //alarms
	0, //more_options
};

void load_kettle_options(void)
{
   kettle_options = default_kettle_options;
}

/* Oletus arvot:
   -tavoite lämpötila
   -keitto aika
   -vesimäärä
   -sekoitustapa
   -sekoitusnopeus
*/
target_data_s def_params;
target_data_s run_target_params;
target_data_s prepare_target_params;
target_data_s live_params; //Padan live-tila

signed short int input_registers[NUM_CARD_INPUT_REGISTERS * NUM_CARDS];
signed short int output_registers[NUM_CARD_OUTPUT_REGISTERS * NUM_CARDS];

unsigned int timeout_ctr;
unsigned char nayta_mitatut_arvot, current_valinta, pyyhk_enabled, trigger_button_up, long_key_press, kansi_avattiin, kansi_suljettiin;

signed int numpad_parameter_val;
signed int numpad_parameter_max_value;
signed int numpad_parameter_min_value;
unsigned char numpad_parameter_unit;
unsigned char numpad_target_parameter;

/*typedef struct
{
   int cook_mode;
   int cook_temps[3]; //C
   int cook_params[2];

   int chill_mode;
   int chill_temps[2];
   int chill_params[2];

   int water_fill_mode;
   int water_fill_amount_litres;
   int water_fill_amount_pulses;
   int water_fill_params[2];

   int mixer_mode;
   int mixer_speed;
   int mixer_pause_time_sec;
   int mixer_params[2];

   int tilt_mode;
   int tilt_speed;
   int tilt_params;
} target_data_s;*/

//Default arvot luetaan jostain asetuksista / käyttäjän asetuksista tms..
//Tässä laitettu vaan jotain arvoja näihin.
void get_default_cooking_parameters(void)
{
   memset(&def_params, 0, sizeof(def_params));
//   def_params.cook_temps[TARGET_COOK_FOOD_TEMP_IDX]  = 90;
//   def_params.cook_time_minutes = 60;
//   def_params.water_fill_amount_litres = 50;
//   def_params.mixer_speed = 0;
}

void load_run_parameters(void)
{
   get_default_cooking_parameters();
   run_target_params = def_params;
   clear_all_kettle_functions();
}

void update_live_parameters(void)
{
   //live_params... //Padan live-tila
   signed char s = (signed char)input_registers[INPUT_REG_CARD1_READ_FOOD_TEMP];
   live_params.cook_temps[TARGET_COOK_FOOD_TEMP_IDX] = (int)s;
   s = (signed char)input_registers[INPUT_REG_CARD1_READ_KETTLE_TEMP];
   live_params.cook_temps[TARGET_COOK_KETTLE_TEMP_IDX] = (int)s;

   //Jos bitti 0x8000 on 1 -> nollataan live_params.litrat. Ei sen pitäs olla koska update_live_parameters kutsutaan can-communikaation jälkeen jossa se nollataan mutta tässä tarkistus kuitenkin.
   //Vesimäärä menee siis: 
   //1.UI kutsuu set_water_fill(0) + set_water_fill(1):
   //   output_registers[OUTPUT_REG_CARD1_MITTAA_VESILITRAT] = run_target_params.water_fill_amount_litres + 0x8000;//bitti 0x8000 + vesimäärä => aloita tämän vesimäärän laskeminen nyt
   //2.Can lähettää tämän modulille jonka jälkeen nollaa bitin 0x8000
   //3.Moduli saa litratiedon ja kun 0x8000=1 se aloittaa litramittauksen (nollaa pulssit + litrat)
   if (output_registers[OUTPUT_REG_CARD2_MITTAA_VESILITRAT] & 0x8000)
      live_params.water_fill_amount_litres = 0;
   else
   {
      int prev = live_params.water_fill_amount_litres;
      live_params.water_fill_amount_litres = input_registers[INPUT_REG_CARD2_MITATUT_LITRAT];
      if (live_params.water_fill_amount_litres > prev)
         live_params.water_fill_amount_total_litres += (live_params.water_fill_amount_litres-prev);
   }
}

void set_numpad_parameter(int target_par)
{
   numpad_parameter_val = -1;
   numpad_target_parameter = target_par;
   switch (target_par)
   {
      case NUMPAD_TARGET_FOODTEMP:
         numpad_parameter_val = (signed int)prepare_target_params.cook_temps[TARGET_COOK_FOOD_TEMP_IDX];
         numpad_parameter_max_value = 125;
         numpad_parameter_min_value = 0;
         numpad_parameter_unit = NUMPAD_UNIT_C;
      break;

      case NUMPAD_TARGET_KETTLETEMP:
         numpad_parameter_val = (signed int)prepare_target_params.cook_temps[TARGET_COOK_KETTLE_TEMP_IDX];
         numpad_parameter_max_value = 125;
         numpad_parameter_min_value = 0;
         numpad_parameter_unit = NUMPAD_UNIT_C;
      break;

      case NUMPAD_TARGET_CHILLTEMP:
         numpad_parameter_val = (signed int)prepare_target_params.chill_temps[TARGET_CHILL_TEMP_IDX];
         numpad_parameter_max_value = 70;
         numpad_parameter_min_value = 0;
         numpad_parameter_unit = NUMPAD_UNIT_C;
      break;

      case NUMPAD_TARGET_COOK_TIME:
         numpad_parameter_val = (signed int)prepare_target_params.cook_time_minutes;
         numpad_parameter_max_value = 9959; //24*60;
         numpad_parameter_min_value = 0;
         numpad_parameter_unit = NUMPAD_UNIT_HH_MM;
      break;

      case NUMPAD_TARGET_CHILL_TIME:
         numpad_parameter_val = (signed int)prepare_target_params.chill_time_minutes;
         numpad_parameter_max_value = 9959; //24*60;
         numpad_parameter_min_value = 0;
         numpad_parameter_unit = NUMPAD_UNIT_HH_MM;
      break;

      case NUMPAD_TARGET_WATER:
         numpad_parameter_val = 0; //(signed int)prepare_target_params.water_fill_amount_litres;
         numpad_parameter_max_value = 500;
         numpad_parameter_min_value = 0;
         numpad_parameter_unit = NUMPAD_UNIT_L;
      break;

      case NUMPAD_TARGET_SEKOITUS_VALINTA:
         numpad_parameter_val = (signed int)prepare_target_params.mixer_speed;
         numpad_parameter_max_value = 200;
         numpad_parameter_min_value = 0;
         numpad_parameter_unit = NUMPAD_UNIT_RPM;
      break;
   }
}

void numpad_ok_pressed(void)
{
   if (numpad_parameter_val > numpad_parameter_max_value)
      return;
   if (numpad_parameter_val < numpad_parameter_min_value)
      return;

   if (menu_to_return == ID_VAIHEIDEN_LUONTI)
   {
      numpad_updates_vaiheen_parametri();
      return;
   }
   switch (numpad_target_parameter)
   {
      case NUMPAD_TARGET_FOODTEMP:
         prepare_target_params.cook_temps[TARGET_COOK_FOOD_TEMP_IDX] = numpad_parameter_val;
         prepare_target_params.state |= TARGET_STATE_COOK_TEMP_CHANGED_BIT;
      break;

      case NUMPAD_TARGET_KETTLETEMP:
         prepare_target_params.cook_temps[TARGET_COOK_KETTLE_TEMP_IDX] = numpad_parameter_val;
         prepare_target_params.state |= TARGET_STATE_KETTLE_TEMP_CHANGED_BIT;
      break;

      case NUMPAD_TARGET_CHILLTEMP:
         prepare_target_params.chill_temps[TARGET_CHILL_TEMP_IDX] = numpad_parameter_val;
         prepare_target_params.state |= TARGET_STATE_CHILL_TEMP_CHANGED_BIT;
      break;

      case NUMPAD_TARGET_COOK_TIME:
         run_target_params.cook_time_minutes = numpad_parameter_val;
         run_target_params.cook_time_left_seconds = run_target_params.cook_time_minutes*60;
      break;

      case NUMPAD_TARGET_CHILL_TIME:
         run_target_params.chill_time_minutes = numpad_parameter_val;
         run_target_params.chill_time_left_seconds = run_target_params.chill_time_minutes*60;
      break;

      case NUMPAD_TARGET_WATER:
         prepare_target_params.water_fill_amount_litres = numpad_parameter_val;
         prepare_target_params.state |= TARGET_STATE_WATER_FILL_CHANGED_BIT;
      break;

      case NUMPAD_TARGET_SEKOITUS_VALINTA:
         run_target_params.mixer_speed = numpad_parameter_val;
         live_params.state |= TARGET_STATE_SOMETHING_STARTED_BIT;
      break;
   }
   check_if_any_value_changed();
}
