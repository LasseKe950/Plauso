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

Uint32 mixer_task_1second_time;

int mix_speed_kun_kansi_avattiin, flag_kansi_suljettiin, kansi_suljettiin_speed_ramp_time;
int kansi_suljettiin_speed_aktiivinen; //t‰m‰ kertoo ett‰ rampitus on menossa kannen sulkemisen j‰lkeen (40 -> mix_speed_kun_kansi_avattiin)

static int mix_time;

int get_lid_closed_speed_ramp_time(void) //0..5sek
{
   return 0;
}

int MixerTask(int msg) 
{
	int prog, j;

   if (msg == MSG_TYPE_MIXER_PROG_CHG_MSG)
   {  //Jos sek. moodi vaihdetaan -> nollataan aika joka laskee sek.ohjelman eteen/taakse/tauko-aikaa
   	mix_time = 0;
   }

   prog = run_target_params.mixer_mode;
   if (prog == 0)
   {
      mix_speed_kun_kansi_avattiin = flag_kansi_suljettiin = 0;
      kansi_suljettiin_speed_aktiivinen = 0;
   	mix_time = 0;
      return 0;
   }

   //Jos msg=0 tsekataan onko 1s kulunut viime kutsusta, jos ei viel‰ ole niin menn‰‰n pois, jos msg != 0 niin hoidetaan mixer-tilakone
   if ((msg==0) && (SDL_TICKS_PASSED(SDL_GetTicks(), mixer_task_1second_time) == 0))
      return 0;
   mixer_task_1second_time = SDL_GetTicks() + 1000;

   if (msg==0)
   {
      if (kansi_suljettiin_speed_aktiivinen)//t‰m‰ kertoo ett‰ rampitus on menossa kannen sulkemisen j‰lkeen (40 -> mix_speed_kun_kansi_avattiin)
      {
         j = get_lid_closed_speed_ramp_time(); //0..5sek
         if (++kansi_suljettiin_speed_ramp_time < j)
            return 0;
         kansi_suljettiin_speed_ramp_time = 0;
         if (j==0)
            run_target_params.mixer_speed = mix_speed_kun_kansi_avattiin;
         else
            run_target_params.mixer_speed = kansi_suljettiin_speed_aktiivinen;
         //printf("Setting mixer speed --> %d target=%d\n", Set_mixer_speed, mix_speed_kun_kansi_avattiin);
         mixer_motor_vaihe = MIXER_MOTOR_RUN_ETEEN;
         mix_time = 0;
         if (run_target_params.mixer_speed >= mix_speed_kun_kansi_avattiin)
            kansi_suljettiin_speed_aktiivinen = 0;
         else
            kansi_suljettiin_speed_aktiivinen += 10;
         return 1;
      }
   }

	if (onko_kansi_auki() || !run_target_params.mixer_speed)
	{
      mix_time = 0; //Jos kansi auki tai nopeus=0 nollataan aika joka laskee sek.ohjelman eteen/taakse/tauko-aikaa
      if (mixer_motor_vaihe==MIXER_MOTOR_IN_PAUSE)
      {
         if (prog==7)
         {
				mixer_motor_vaihe = MIXER_MOTOR_RUN_ETEEN;
         }
      }
      return 1;
	}

//   MIX_MODES_KAHTEEN_SUUNTAAN_HIDAS,      1  M:"Suunnanvaihto, hidas", 4
//   MIX_MODES_YHTEEN_SUUNTAAN,             2  M:"Yhteen suuntaan", 1
//   MIX_MODES_KAHTEEN_SUUNTAAN,            3  M:"Suunnanvaihto", 2
//   MIX_MODES_YHTEEN_SUUNTAAN_TAUKO,       4  M:"Yhteen suuntaan, tauko", 6
//   MIX_MODES_KAHTEEN_SUUNTAAN_NOPEA,      5  M:"Suunnanvaihto, nopea", 3
//   MIX_MODES_YHTEEN_SUUNTAAN_PITKA_TAUKO, 6  M:"Yhteen suuntaan, pitk‰ tauko", 7
//   MIX_MODES_KAHTEEN_SUUNTAAN_TAUKO,      7  M:"Suunnanvaihto, tauko", 5
//   MIX_MODES_OMA_SEKOITUSTAPA,            8

	else if (msg==0)
	{
		++mix_time;
      printf("MixProg=%d mix_time=%d mix_phase=%d\n", prog, mix_time, mixer_motor_vaihe);

		if (prog==MIX_MODES_YHTEEN_SUUNTAAN) //1. Yhteensuuntaan Forward Only
		{
			if (mix_time >= 5)
			{
				mix_time=0;
				mixer_motor_vaihe = MIXER_MOTOR_RUN_ETEEN;
			}
		}
		else if (prog==MIX_MODES_KAHTEEN_SUUNTAAN_HIDAS) //2.Hidas suunnanvaihto = B eteen ja B taakse {"HIDAS", "SUUNNAN", "VAIHTO"}  MUUTETTU ==> MODE 4
		{	        //B = "CurrentProfileSt.long_mix_sec"
			if (mix_time >= kettle_options.long_mix_sec)
			{
				mix_time = 0;
				if (mixer_motor_vaihe == MIXER_MOTOR_RUN_ETEEN)
					mixer_motor_vaihe = MIXER_MOTOR_RUN_TAAKSE;
				else
					mixer_motor_vaihe = MIXER_MOTOR_RUN_ETEEN;
			}
		}
		else if (prog==MIX_MODES_KAHTEEN_SUUNTAAN_NOPEA) //3.Nopea suunnanvaihto = A eteen ja A taakse {"ETEEN", "TAAKSE", "NOPEA"}
		{              //A = "CurrentProfileSt.short_mix_sec"
			if (mix_time >= kettle_options.short_mix_sec)
			{
				mix_time = 0;
				if (mixer_motor_vaihe == MIXER_MOTOR_RUN_ETEEN)
					mixer_motor_vaihe = MIXER_MOTOR_RUN_TAAKSE;
				else
					mixer_motor_vaihe = MIXER_MOTOR_RUN_ETEEN;
			}
		}
		else if (prog==MIX_MODES_KAHTEEN_SUUNTAAN) //4.Suunnanvaihto = B eteen ja A taakse {"SUUNNAN", "VAIHTO", ""},  MUUTETTU ==> MODE 2
		{	             //A = "CurrentProfileSt.short_mix_sec", B = "CurrentProfileSt.long_mix_sec"
   		if (mixer_motor_vaihe == MIXER_MOTOR_IN_PAUSE)
         {
            mixer_motor_vaihe = MIXER_MOTOR_RUN_ETEEN;
            mix_time = 0;
         }
			if ((mixer_motor_vaihe == MIXER_MOTOR_RUN_ETEEN) && (mix_time >= kettle_options.long_mix_sec))
			{
				mix_time = 0;
				mixer_motor_vaihe = MIXER_MOTOR_RUN_TAAKSE;
			}
			else if ((mixer_motor_vaihe == MIXER_MOTOR_RUN_TAAKSE) && (mix_time >= kettle_options.short_mix_sec))
			{
				mix_time = 0;
				mixer_motor_vaihe = MIXER_MOTOR_RUN_ETEEN;
			}
		}
		else if (prog==MIX_MODES_YHTEEN_SUUNTAAN_TAUKO) //5.Yhteen suuntaan + 1/2 tauko = B eteen + C tauko	{"YHTEEN", "SUUNTAAN", "1/2 TAUKO"}   MUUTETTU ==> MODE 6
		{              //B = "CurrentProfileSt.long_mix_sec", C = "CurrentProfileSt.mix_half_pause_sec"
			if (mixer_motor_vaihe == MIXER_MOTOR_RUN_TAAKSE)
         {
            mixer_motor_vaihe = MIXER_MOTOR_RUN_ETEEN;
            mix_time = 0;
         }
			if ((mixer_motor_vaihe == MIXER_MOTOR_RUN_ETEEN) && (mix_time >= kettle_options.long_mix_sec))
			{
				mix_time = 0;
            mixer_motor_vaihe = MIXER_MOTOR_IN_PAUSE;
			}
			else if ((mixer_motor_vaihe == MIXER_MOTOR_IN_PAUSE) && (mix_time >= kettle_options.mix_short_pause_sec))
			{
				mix_time = 0;
				mixer_motor_vaihe = MIXER_MOTOR_RUN_ETEEN;
			}
		}
		else if (prog==MIX_MODES_KAHTEEN_SUUNTAAN_TAUKO) //6.Suunnanvaihto + 1/2 tauko = B eteen ja A taakse + C tauko	{"SUUNNAN", "VAIHTO", "1/2 TAUKO"},    MUUTETTU ==> MODE 5
		{              //A = "CurrentProfileSt.short_mix_sec", B = "CurrentProfileSt.long_mix_sec", C = "CurrentProfileSt.mix_half_pause_sec"
			if ((mixer_motor_vaihe == MIXER_MOTOR_RUN_ETEEN) && (mix_time >= kettle_options.long_mix_sec))
			{
				mix_time = 0;
				mixer_motor_vaihe = MIXER_MOTOR_RUN_TAAKSE;
			}
			else if ((mixer_motor_vaihe == MIXER_MOTOR_RUN_TAAKSE) && (mix_time >= kettle_options.short_mix_sec))
			{
				mix_time = 0;
				mixer_motor_vaihe = MIXER_MOTOR_IN_PAUSE;
			}
			else if ((mixer_motor_vaihe == MIXER_MOTOR_IN_PAUSE) && (mix_time >= kettle_options.mix_short_pause_sec))
			{
				mix_time = 0;
				mixer_motor_vaihe = MIXER_MOTOR_RUN_ETEEN;
			}
		}
		else if (prog==MIX_MODES_YHTEEN_SUUNTAAN_PITKA_TAUKO) //7.Yhteen suuntaan + tauko = B eteen + D tauko {"YHTEEN", "SUUNTAAN", "TAUKO"},
		{              //B = "CurrentProfileSt.long_mix_sec", D = "CurrentProfileSt.mix_pause_min"
			if (mixer_motor_vaihe == MIXER_MOTOR_RUN_TAAKSE)
         {
            mixer_motor_vaihe = MIXER_MOTOR_RUN_ETEEN;
            mix_time = 0;
         }
			if ((mixer_motor_vaihe == MIXER_MOTOR_RUN_ETEEN) && (mix_time >= kettle_options.long_mix_sec))
			{
				mix_time = 0;
				mixer_motor_vaihe = MIXER_MOTOR_IN_PAUSE;
			}
			else if ((mixer_motor_vaihe == MIXER_MOTOR_IN_PAUSE) && (mix_time >= (kettle_options.mix_long_pause_sec)))
			{
				mix_time = 0;
				mixer_motor_vaihe = MIXER_MOTOR_RUN_ETEEN;
			}
		}
	}
	return 0;
}

/*
   14.2.2012: en‰‰ 7 ohjelmaa!
	1. Yhteensuuntaan Forward Only
   2. Suunnanvaihto  Auto-Reverse
   3. Nopea suunnanvaihto Auto-Reverse, quick
   4. Hidas suunnanvaihto Auto-Reverse, slow
   5. Suunnanvaihto + pause  Auto-Rev. pause
   6. Yhteehsuuntaan + pause Forward, pause
   7. Yhteehsuuntaan + pitk‰ pause Forward, long pause
*/
