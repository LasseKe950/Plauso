/* File: kettle_functions.cpp
   Purpose:
      Grafiikka tilakoneet
   Documents:
      "Datan kulku v2.txt"

*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "var.h"

#define COOKING_UIMURI_ALHAALLA_CTR 2 /*uimuri-alhaalla tilaan laskuri -> otetaan vettä vaippaan*/
#define COOKING_UIMURI_YLHAALLA_CTR 2 /*uimuri-ylhaalla tilaan laskuri -> voidaan alkaa lämmittämään*/
#define EMPTYING_UIMURI_ALHAALLA_CTR 2

#define EMPTYING_TIMEOUT_SEC 300

extern void clear_all_kettle_functions(void);

extern int check_emergency_stop(void);
extern int check_errors(void);

extern void set_cooking(int start);
extern void handle_cooking(void);

extern void set_chilling(int start);
extern void handle_chilling(void);

extern void set_mixing(int start_mode); //mode = enum mix_modes
extern void handle_mixing(void);

extern void set_water_fill(int start);
extern void handle_water_fill(void);

extern void set_ohijuoksutus(int start);
extern void handle_ohijuoksutus(void);

extern void set_tilting(int start);
extern void handle_tilting(void);

extern void set_emptying(int start);
extern void handle_emptying(void);

extern void handle_haccp(void);

enum cooking_vaiheet
{
   COOKING_OFF,
   COOKING_VAIPAN_TYHJENNYS_ALUKSI, //timeout:illa
   COOKING_VAIPPAVEDEN_TAYTTO, //täytetään uimuriin asti ennenkuin vastukset voi laittaa päälle
   COOKING_VAIPPAVESI_OK,
   COOKING_READY,
   NUM_COOKING_VAIHEET,
};

enum chilling_vaiheet
{
   CHILLING_OFF,
   CHILLING_WAIT_IF_100C, //chill alkaa vasta kun vaippavesi alle 100C
   CHILLING_2_VAIHE_CHILL_VAIHE_1,
   CHILLING_2_VAIHE_CHILL_VAIHE_2,
   CHILLING_1_VAIHE_CHILL,
   CHILLING_READY,
   NUM_CHILLING_VAIHEET,
};

enum emptying_vaiheet
{
   EMPTYING_OFF,
   EMPTYING_WAIT_IF_55C, // Tyhjennys estetty jos vaipan lämpö >55
   EMPTYING_GOING,
   EMPTYING_TIMED_OUT,
   EMPTYING_READY,
   NUM_EMPTYING_VAIHEET,
};

enum water_fill_vaiheet
{
   WATER_FILL_OFF,
   WATER_FILL_MITTAA_VESIMAARA,
   WATER_FILL_MITTAA_VESIMAARA_HOT,
   WATER_FILL_VESIMAARA_OK,
   WATER_FILL_ERROR,
   NUM_WATER_FILL_VAIHEET,
};

enum mixer_motor_vaiheet
{
   MIXER_MOTOR_STOPPED,
   MIXER_MOTOR_RAMP_ETEEN,
   MIXER_MOTOR_RUN_ETEEN,
   MIXER_MOTOR_RAMP_TAAKSE,
   MIXER_MOTOR_RUN_TAAKSE,
   MIXER_MOTOR_IN_PAUSE,
   MIXER_MOTOR_PULSED_STATE,
   MIXER_MOTOR_ERROR,
   NUM_MIXER_MOTOR_VAIHEET,
};

enum tilt_motor_vaiheet
{
   TILT_MOTOR_STOPPED,
   TILT_MOTOR_RAMP_UP,
   TILT_MOTOR_RUN_UP,
   TILT_MOTOR_RAMP_DOWN,
   TILT_MOTOR_RUN_DOWN,
   TILT_MOTOR_ERROR,
   NUM_TILT_MOTOR_VAIHEET,
};

extern int cooking_vaihe, chilling_vaihe, emptying_vaihe, water_fill_vaihe;
extern int tilt_motor_vaihe, tilt_motor_dir;
extern int mixer_motor_vaihe;

extern int onko_kansi_auki(void);
extern int onko_kallistettu(void);
extern int onko_uimuri_ylhaalla(void);
extern int onko_jalka_kytkin_painettu(void);

extern unsigned char water_measure_error, food_sensor_error, kettle_sensor_error, hataseis_painettu, kontaktori_vika, painekytkin_lauennut, vast_ylikuum_suoja_lauennut;

//mixer.c
extern int mix_speed_kun_kansi_avattiin, flag_kansi_suljettiin, kansi_suljettiin_speed_ramp_time;
extern int kansi_suljettiin_speed_aktiivinen; //tämä kertoo että rampitus on menossa kannen sulkemisen jälkeen (40 -> mix_speed_kun_kansi_avattiin)
