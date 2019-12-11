/* 
   cook_prog.cpp
   Keitto-ohjelman kirjoitus/luku rutiinit.

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
#include "cook_prog.h"

cook_prog_step_s tmp_steps[MAKS_STEPS];
 
cook_prog_header_s tmp_header;

int next_prog_step;
int current_prog_step, multi_step_prog_going;

char tmp_ohj_nimi[MAKS_LEN];
char tmp_kansion_nimi[MAKS_LEN];
char tmp_ohj_kuvaus[MAKS_LEN];

char tmp_lappu[MAKS_LAPPU_LEN];

void clear_prog_header(void)
{
   tmp_ohj_nimi[0] = 0;
   tmp_kansion_nimi[0] = 0;
   memset(&tmp_header, 0, sizeof(tmp_header));
   next_prog_step = 0;
}

void set_prog_header_ohj_nimi(char * nimi)
{
   strncpy(tmp_ohj_nimi, nimi, MAKS_LEN-1);
}

void set_prog_header_kansion_nimi(char * nimi, int Valmiskansio_jos_alle_100)
{
   strncpy(tmp_kansion_nimi, nimi, MAKS_LEN-1);
   tmp_header.Oma_kansio = Valmiskansio_jos_alle_100;
}

void set_prog_header_kuvaus(char * kuvaus)
{
   strncpy(tmp_ohj_kuvaus, kuvaus, MAKS_LEN-1);
}

void set_prog_header_tyokalu_erakoko(int t, int e)
{
   tmp_header.Tyokalu_idx = t;
   tmp_header.Erakoko_kg = e;
}

void set_step_cook_params(int temp1, int temp2, int aika, cook_prog_step_s * new_step)
{
   new_step->Step_type = STEP_TYPE_COOK_STEP;
   new_step->Temp1 = temp1;
   new_step->Temp2 = temp2;
   new_step->Aika = aika;
}

void set_step_chill_params(int temp1, int temp2, int aika, cook_prog_step_s * new_step)
{
   new_step->Step_type = STEP_TYPE_CHILL_STEP;
   new_step->Temp1 = temp1;
   new_step->Temp2 = temp2;
   new_step->Aika = aika;
}

void set_step_water_params(int litrat, int litrat_hot, cook_prog_step_s * new_step)
{
   new_step->Vesi = litrat;
   new_step->Lvesi = litrat_hot;
}

void set_step_mixer_params(int mode, int speed, cook_prog_step_s * new_step)
{
   new_step->Sek_tapa = mode;
   new_step->Nopeus = speed;
}

void set_step_lappu_params(int fixed, char *oma, cook_prog_step_s * new_step)
{
   if (fixed)
      new_step->Step_type = STEP_TYPE_FIXED_NOTE + (fixed-1);
}

void insert_new_step(cook_prog_step_s * new_step)
{
   if (next_prog_step < MAKS_STEPS)
   {
      tmp_steps[next_prog_step++] = *new_step;
      tmp_header.Vaihe_lkm = next_prog_step;
   }
}

const char *valmis_kansio_nimet[NUM_VALMIS_KANSIOT] =
{
   "VALMIS1", "VALMIS2", "VALMIS3", "VALMIS4", "VALMIS5", "VALMIS6", "VALMIS7", "VALMIS8", "VALMIS9", "VALMIS10", "VALMIS11", "VALMIS12", 
};

void make_valmiskansiot(void)
{
	char buff[50];
   for (int i=0; i<NUM_VALMIS_KANSIOT; i++)
   {
      sprintf(buff, "mkdir -p %s\n", valmis_kansio_nimet[i]);
      system(buff);
   }
}

int write_new_prog_file(void)
{
	FILE *target;
	char buffer[200];
	int i;

   if (strlen(tmp_kansion_nimi)==0)
      return -1;
   if (strlen(tmp_ohj_nimi)==0)
      return -2;

//   if (tmp_header.Vaihe_lkm != next_prog_step)
//      return -3;
//
//   if (!next_prog_step)
//      return -4;

   sprintf(buffer, "%s/%s.txt", tmp_kansion_nimi, tmp_ohj_nimi);
   //sprintf(buffer, "%s.txt", tmp_ohj_nimi); //Ei vielä kansioita -> Kirj. juureen

	target = fopen(buffer,"w");
	if (target==0)
	{
	  printf("OUTPUT-tied. ei saa auki\n");
	  return -1;
	}

   sprintf(buffer, "%s,%s,", tmp_kansion_nimi, tmp_ohj_nimi);
   fprintf(target, "%s", buffer);

   sprintf(buffer, "%d,%d,%d,%d,", tmp_header.Oma_kansio, tmp_header.Vaihe_lkm, tmp_header.Tyokalu_idx, tmp_header.Erakoko_kg);
   fprintf(target, "%s\n", buffer); //Loppuun rivinvaihto

   for (i=0; i < tmp_header.Vaihe_lkm; i++)
   {
      if (tmp_steps[i].Step_type == STEP_TYPE_USER_NOTE)
      {
        sprintf(buffer, "%d,%s\n", tmp_steps[i].Step_type, tmp_lappu);
      }
      else
      {
         sprintf(buffer, "%d,%d,%d,%d,%d,%d,%d,%d\n", tmp_steps[i].Step_type, tmp_steps[i].Temp1, tmp_steps[i].Temp2, tmp_steps[i].Aika, 
                                                      tmp_steps[i].Vesi, tmp_steps[i].Lvesi,
                                                       tmp_steps[i].Sek_tapa, tmp_steps[i].Nopeus);
      }
      fprintf(target, "%s", buffer);
   }

	fclose (target);
//	printf("File CALIB_DAT_ANT%d.TXT written!\n", anturi+1);

   sqlite_insert_ohjelma(); //Lisätään ohjelma ohjelmat.db tietokantaan

   return 0;
}

char read_kansion_nimi[MAKS_LEN];
char read_ohj_nimi[MAKS_LEN];

// -Kansion oikea nimi (esim "VALMIS1") on kirjoitettu -> tmp_kansion_nimi[MAKS_LEN];
//  Ohjelman nimi --> sql_haku[i].haku1 --> tmp_ohj_nimi
int read_program_to_memory(void)
{
	FILE *target;
	char buffer[255];
	int i, v;

   printf("Luetaan ohjelma:%s/%s.txt", tmp_kansion_nimi, tmp_ohj_nimi);

   sprintf(buffer, "%s/%s.txt", tmp_kansion_nimi, tmp_ohj_nimi);
	target = fopen(buffer, "r");
	if (target==0)
	{
	  printf("fopen error \n");
	  return -1;
	}
   if (fgets(buffer, 250, target) == NULL)
      goto read_prog_to_mem_error;

   printf("\nEKA rivi=%s\n", buffer);

   char * p;
   p = strchr(buffer, (int)','); //Ohitetaan kansio + ohj kentät
   if (p == 0)
      goto read_prog_to_mem_error;
   p = strchr(++p, (int)','); //Ohitetaan kansio + ohj kentät
   if (p == 0)
      goto read_prog_to_mem_error;

   i = sscanf(++p, "%d,%d,%d,%d,", &tmp_header.Oma_kansio, &tmp_header.Vaihe_lkm, &tmp_header.Tyokalu_idx, &tmp_header.Erakoko_kg);
   printf("sscanf ret = %d\n", i);
   if (i == 4)
   {
      for (v = 0; v < tmp_header.Vaihe_lkm; v++)
      {
         if (fgets(buffer, 250, target) == NULL)
         {
            printf("fgets=NULL v=%d\n", v);
            goto read_prog_to_mem_error;
         }
         else if (v < MAKS_STEPS)
         {
            i = sscanf(buffer, "%d,%d,%d,%d,%d,%d,%d,%d", &tmp_steps[v].Step_type, &tmp_steps[v].Temp1, &tmp_steps[v].Temp2, &tmp_steps[v].Aika, 
                                                          &tmp_steps[v].Vesi, &tmp_steps[v].Lvesi, &tmp_steps[v].Sek_tapa, &tmp_steps[v].Nopeus);
            if (i != 8)
            {
               goto read_prog_to_mem_error;
            }
         }
      }
   }

   for (v = 0; v < tmp_header.Vaihe_lkm; v++)
   {
      printf("VAIHE %d: TYPE=%d, TEMP1=%d, TEMP2=%d, AIKA=%d, VESI=%d, LVESI=%d, SEK.TAPA=%d, NOPeUS=%d\n", v, 
                                                     tmp_steps[v].Step_type, tmp_steps[v].Temp1, tmp_steps[v].Temp2, tmp_steps[v].Aika, 
                                                     tmp_steps[v].Vesi, tmp_steps[v].Lvesi, tmp_steps[v].Sek_tapa, tmp_steps[v].Nopeus);
   }
read_prog_to_mem_error:
   fclose(target);
   return 0;
}

/*
  load_new_program_step kutsutaan kun uusi monivaiheohjelman vaihe ladataan ohjelman muistiin eli tmp_steps[current_prog_step] --> prepare_target_params
  -varsinaisesti aloitus tapahtuu sitten ALOITA-painikkeella

  Huom! Jäähd/kyps-tilan alussa tehdään --> prepare_target_params = run_target_params;
  Joten tuota ei voi tehdä tämän funktio-kutsun jälkeen!
*/

int load_new_program_step(void) //tmp_steps[current_prog_step] --> prepare_target_params
{
   memset(&prepare_target_params, 0, sizeof(prepare_target_params));

   if (current_prog_step >= MAKS_STEPS)
      return -1;
   if (current_prog_step >= tmp_header.Vaihe_lkm)
      return -2;

   int i = current_prog_step;
   if (tmp_steps[i].Step_type == STEP_TYPE_CHILL_STEP)
   {
      prepare_target_params.chill_temps[TARGET_CHILL_TEMP_IDX] = tmp_steps[i].Temp1;
      run_target_params.chill_time_minutes = prepare_target_params.chill_time_minutes = tmp_steps[i].Aika;
      prepare_target_params.state |= TARGET_STATE_CHILL_TEMP_CHANGED_BIT;
   }
   else if (tmp_steps[i].Step_type < STEP_TYPE_CHILL_STEP) //0..9 eri cook meiningit
   {
      prepare_target_params.cook_temps[TARGET_COOK_FOOD_TEMP_IDX] = tmp_steps[i].Temp1;
      run_target_params.cook_time_minutes = prepare_target_params.cook_time_minutes = tmp_steps[i].Aika;
      prepare_target_params.state |= TARGET_STATE_COOK_TEMP_CHANGED_BIT;
   }

   if (tmp_steps[i].Aika)
   {
   }

   prepare_target_params.water_fill_amount_litres = tmp_steps[i].Vesi;
   if (tmp_steps[i].Vesi)
   {
      prepare_target_params.state |= TARGET_STATE_WATER_FILL_CHANGED_BIT;
   }

   if (tmp_steps[i].Sek_tapa < NUM_MIX_MODES)
   {
      prepare_target_params.mixer_mode = tmp_steps[i].Sek_tapa;
      if (tmp_steps[i].Sek_tapa)
      {
         prepare_target_params.mixer_speed = tmp_steps[i].Nopeus;
         prepare_target_params.state |= TARGET_STATE_MIXER_CHANGED_BIT;
      }
   }
   return 0;
}
