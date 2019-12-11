#ifndef VAR_H
#define VAR_H
/* File: var.cpp
   Purpose: variables
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define _1SEc_TIMERID_STR "1234"
#define _1SEc_TIMERID 1234

enum mix_modes
{
   MIX_MODES_EI_KAYNNISSA,
   MIX_MODES_KAHTEEN_SUUNTAAN_HIDAS,
   MIX_MODES_YHTEEN_SUUNTAAN,
   MIX_MODES_KAHTEEN_SUUNTAAN,
   MIX_MODES_YHTEEN_SUUNTAAN_TAUKO,
   MIX_MODES_KAHTEEN_SUUNTAAN_NOPEA,
   MIX_MODES_YHTEEN_SUUNTAAN_PITKA_TAUKO,
   MIX_MODES_KAHTEEN_SUUNTAAN_TAUKO,
   MIX_MODES_OMA_SEKOITUSTAPA,
   NUM_MIX_MODES //=1-8, 0=ei käynnissä
};

extern const int max_nopeudet[9]; //sek_tapa_valinta.cpp

/* Oletus arvot:
   -tavoite lämpötila
   -keitto aika
   -vesimäärä
   -sekoitustapa
   -sekoitusnopeus
*/
#define TARGET_COOK_FOOD_TEMP_IDX 0
#define TARGET_COOK_KETTLE_TEMP_IDX 1
#define TARGET_COOK_EXT_TEMP_IDX 2

#define TARGET_CHILL_TEMP_IDX 0

#define TARGET_STATE_COOK_TEMP_CHANGED_BIT   0x0001
#define TARGET_STATE_KETTLE_TEMP_CHANGED_BIT 0x0002
#define TARGET_STATE_CHILL_TEMP_CHANGED_BIT  0x0004
#define TARGET_STATE_WATER_FILL_CHANGED_BIT  0x0008
#define TARGET_STATE_MIXER_CHANGED_BIT       0x0010
#define TARGET_STATE_CHANGE_GOING_BITS  0x001F

#define TARGET_STATE_SOMETHING_STARTED_BIT   0x0080

//All normal modes = 0

//Special modes etc:
//#define TARGET_WATER_FILL_MODE_HOT_WATER 1

typedef struct
{
   int cook_mode;
   int cook_temps[3]; //C
   int cook_params[2];
   int cook_time_minutes;
   int cook_time_left_seconds;
   int cook_timeout;

   int state; //something_started; //Jos 1 -> Kypsennys/Jäähdytys alavalikossa "VALMIS" ja sekoitinsäädöt osittain piilossa.
   int temp_reached;

   int chill_mode;
   int chill_temps[2];
   int chill_params[2];
   int chill_time_minutes;
   int chill_time_left_seconds;
   int chill_timeout;

   int emptying_mode;
   int emptying_timeout;

   int water_fill_mode;
   int water_fill_amount_litres;
   int water_fill_amount_total_litres;
   int water_fill_params[2];
   int water_fill_timeout;

   int mixer_mode;
   int selected_mixer_mode;
   int mixer_speed;
   int mixer_pause_time_sec;
   int mixer_params[2];

   int tilt_mode; //1=UP, 2=Down
   int tilt_speed;
   int tilt_params;
} target_data_s;

extern target_data_s def_params;
extern target_data_s run_target_params;
extern target_data_s prepare_target_params;
extern target_data_s live_params; //Padan live-tila

/*----------user profile --------------*/
#define NUM_USER_LEVELS 3
typedef struct
{
   int user_level;
   int user_language;
} user_profiili_s;

extern user_profiili_s user_profile;

/*----------kettle options --------------*/
#define NUM_USER_LEVELS 3

typedef struct
{
	int cook_temp;
	int chill_temp;
	int two_phase_chill;
	int haccp_high;
	int haccp_low;
	int cook_time_min;
	int chill_time_min;	
	int chill_speed; /*min/C*/
	int mix_prog;
	int short_mix_sec;
	int long_mix_sec;
	int mix_short_pause_sec;
	int mix_long_pause_sec;	
	int alarms; //stop_beep_sec;
	int more_options; //pause_beep_sec;
} kettle_options_s;
extern kettle_options_s kettle_options;
extern const kettle_options_s default_kettle_options;
extern void load_kettle_options(void);

extern void get_default_cooking_parameters(void);
extern void load_run_parameters(void); //lataa defaultit -> ajo-parametreiksi
extern void update_live_parameters(void);

#define NUM_CARDS 3

/* Input registers, signed 16-bit values */
#define NUM_CARD_INPUT_REGISTERS 8

#define INPUT_REG_CARD1_DIGITAL_INPUTS     0
#define INPUT_REG_CARD1_ANALOG_INPUT_1     1
#define INPUT_REG_CARD1_ANALOG_INPUT_2     2
#define INPUT_REG_CARD1_READ_FOOD_TEMP     3
#define INPUT_REG_CARD1_READ_KETTLE_TEMP   4

#define INPUT_REG_CARD2_DIGITAL_INPUTS     (NUM_CARD_INPUT_REGISTERS + 0)
#define INPUT_REG_CARD2_ANALOG_INPUT_1     (NUM_CARD_INPUT_REGISTERS + 1)
#define INPUT_REG_CARD2_ANALOG_INPUT_2     (NUM_CARD_INPUT_REGISTERS + 2)
#define INPUT_REG_CARD2_MITATUT_LITRAT     (NUM_CARD_INPUT_REGISTERS + 3)
#define INPUT_REG_CARD2_MITATUT_LITRAT_HOT (NUM_CARD_INPUT_REGISTERS + 4)
#define INPUT_REG_CARD2_VESI_PULSSIT       (NUM_CARD_INPUT_REGISTERS + 5)

#define INPUT_REG_CARD3_DIGITAL_INPUTS     ((2*NUM_CARD_INPUT_REGISTERS) + 0)
#define INPUT_REG_CARD3_ANALOG_INPUT_1     ((2*NUM_CARD_INPUT_REGISTERS) + 1)
#define INPUT_REG_CARD3_ANALOG_INPUT_2     ((2*NUM_CARD_INPUT_REGISTERS) + 2)

extern signed short int input_registers[NUM_CARD_INPUT_REGISTERS * NUM_CARDS];

/* Output registers, signed 16-bit values */ 
#define NUM_CARD_OUTPUT_REGISTERS 8

#define OUTPUT_REG_CARD1_DIGITAL_OUTPUTS       0
#define OUTPUT_REG_CARD1_ANALOG_OUTPUT_1       1 //PWM 0...100% tai invertterin nopeus..
#define OUTPUT_REG_CARD1_ANALOG_OUTPUT_2       2

#define OUTPUT_REG_CARD2_DIGITAL_OUTPUTS       (NUM_CARD_OUTPUT_REGISTERS + 0)
#define OUTPUT_REG_CARD2_ANALOG_OUTPUT_1       (NUM_CARD_OUTPUT_REGISTERS + 1)
#define OUTPUT_REG_CARD2_ANALOG_OUTPUT_2       (NUM_CARD_OUTPUT_REGISTERS + 2)
#define OUTPUT_REG_CARD2_MITTAA_VESILITRAT     (NUM_CARD_OUTPUT_REGISTERS + 3) //bitti 0x8000 + vesimäärä => aloita tämän vesimäärän laskeminen nyt. Jos reg=0xC000 = avaa hana ja laske litroja kunnes bit 0x4000 on nolla.
#define OUTPUT_REG_CARD2_MITTAA_VESILITRAT_HOT (NUM_CARD_OUTPUT_REGISTERS + 4)

#define OUTPUT_REG_CARD3_DIGITAL_OUTPUTS   ((2*NUM_CARD_OUTPUT_REGISTERS) + 0)
#define OUTPUT_REG_CARD3_ANALOG_OUTPUT_1   ((2*NUM_CARD_OUTPUT_REGISTERS) + 1)
#define OUTPUT_REG_CARD3_ANALOG_OUTPUT_2   ((2*NUM_CARD_OUTPUT_REGISTERS) + 2)

extern signed short int output_registers[NUM_CARD_OUTPUT_REGISTERS * NUM_CARDS];

/*----------------------------------PROTO-PATA-----------------------------*/
/* ---------------------------------KORTTI # 1 pataosa---------------------*/
//kortin #1 (IO-box1) digi-inputit (8kpl): (rekisteri INPUT_REG_CARD1_DIGITAL_INPUTS), Pt1+2 on kortilla #1
#define	PAINEKYTKIN_IN_BIT_CARD1         0x0001  //Di1
#define	FLOAT1_IN_BIT_CARD1              0x0002  //Di2
#define  ALA_UIMURI_IN_BIT_CARD1          0x0004  // OPTIO, Di3
#define	VAST_YLIKUUM_SUOJA_IN_BIT_CARD1  0x0008  // OPTIO, Di4

//kortin #1 (IO-box1) 24V outputit (8kpl): (rekisterin OUTPUT_REG_CARD1_DIGITAL_OUTPUTS, bitit 4..11, 24V_Out1=0x0010 jne)
#define  TYHJENNYS_PUMPPU_OUT_BIT_CARD1     0x0010 // OPTIO
#define  EMPTYING_MV4_24V_OUT_BIT_CARD1     0x0020 // MV4
#define  BYBASS_MV6_24V_OUT_BIT_CARD1       0x0040 //Ohijuoksutus MV6
#define  ILMANPOISTIN_MV7_24V_OUT_BIT_CARD1 0x0080 // OPTIO MV7
#define  VESINOKKA_VALO_24V_OUT_BIT_CARD1   0x0100 // OPTIO

//kortin #1 (IO-box1) 3V outputit (4kpl), (rekisterin OUTPUT_REG_CARD1_DIGITAL_OUTPUTS, bitit 0..3)
/* ---------------------------------KORTTI # 1 ----------------------------*/

/* ---------------------------------KORTTI # 2 pääjalka------------------------*/
//kortin #2 (IO-box2) digi-inputit (8kpl): (rekisteri INPUT_REG_CARD2_DIGITAL_INPUTS), vesimittari on kortills #2
#define	EMERGENCY_STOP_IN_BIT_CARD2      0x0001  //Di1
#define	LID_SWITCH1_IN_BIT_CARD2         0x0002  //Di2
#define  LID_SWITCH2_IN_BIT_CARD2         0x0004
#define	KETTLE_POSITION_IN_BIT_CARD2     0x0008  //Padan asentokytkin Di4
#define	VESIMITT_PULSSIT_IN_BIT_CARD2    0x0010  //Di5
#define	KONTAKTORI_VIKA_IN_BIT_CARD2     0x0020  //Di6, Jos ei lämmitys-ohjausta ja Di6=0 --> kontaktori-vika
#define	JALKA_KYTKIN_IN_BIT_CARD2        0x0040  //Jalkakytkin Di7

//kortin #2 (IO-box2) 24V outputit (8kpl): (rekisterin OUTPUT_REG_CARD2_DIGITAL_OUTPUTS, bitit 4..11, 24V_Out1=0x0010 jne)
#define  INV_RELE_24V_OUT_BIT_CARD2        0x0010  //Invertterin rele kytkee kontaktorilla invertterille sähköt, tämä voi olla koko ajan päällä paitsi hätäseis-tilassa.
#define  TILT_UP_24V_OUT_BIT_CARD2         0x0020
#define  TILT_DOWN_24V_OUT_BIT_CARD2       0x0040
#define  HEAT_24V_OUT_BIT_CARD2            0x0080
#define  VAIPPAVESI_MV1_24V_OUT_BIT_CARD2  0x0100 //vaipan täyttö MV1
#define  JAAHD_VESI_MV2b_24V_OUT_BIT_CARD2 0x0200 // OPTIO CH3 pehmea vesi
#define  FOOD_WATER_COLD_24V_OUT_BIT_CARD2 0x0400 //Ruokavesi
#define  FOOD_WATER_HOT_24V_OUT_BIT_CARD2  0x0800 // OPTIO Ruokavesi

//kortin #2 (IO-box2) 3V outputit (4kpl), (rekisterin OUTPUT_REG_CARD2_DIGITAL_OUTPUTS, bitit 0..3)
#define	INVERTER_ON_3V_OUT_BIT_CARD2       0x0001 // invertteri "on"
#define	INVERTER_FWD_3V_OUT_BIT_CARD2	     0x0002 // ohjaus forward
#define	INVERTER_TURVASEK_RELE1_3V_OUT_BIT_CARD2 0x0004 // inv. turvanopeus
#define	INVERTER_TURVASEK_RELE2_3V_OUT_BIT_CARD2 0x0008 // inv. turvanopeus
//#define	INVERTER_0_10V_OUT   // DAC-OUT alakortti:pwm-lähtöpiikki. (Rekisteri OUTPUT_REG_CARD2_ANALOG_OUTPUT_1)
/* ---------------------------------KORTTI # 2 ----------------------------*/

/*----------------------------------PROTO-PATA-----------------------------*/

extern int disable_repeat, no_button_ctr;
extern unsigned int timeout_ctr;
extern unsigned char nayta_mitatut_arvot, current_valinta, pyyhk_enabled, trigger_button_up, long_key_press, kansi_avattiin, kansi_suljettiin;

#define NUMPAD_UNIT_C   1
#define NUMPAD_UNIT_HH_MM 2
#define NUMPAD_UNIT_L   3
#define NUMPAD_UNIT_RPM 4

#define NUMPAD_TARGET_FOODTEMP   1
#define NUMPAD_TARGET_KETTLETEMP 2
#define NUMPAD_TARGET_CHILLTEMP  3
#define NUMPAD_TARGET_COOK_TIME  4
#define NUMPAD_TARGET_CHILL_TIME 5
#define NUMPAD_TARGET_WATER      6
#define NUMPAD_TARGET_SEKOITUS_VALINTA  7

extern signed int numpad_parameter_val;
extern signed int numpad_parameter_max_value;
extern signed int numpad_parameter_min_value;
extern unsigned char numpad_parameter_unit;
extern unsigned char numpad_target_parameter;

extern void set_numpad_parameter(int target_par);
extern void numpad_ok_pressed(void);
extern void numpad_updates_vaiheen_parametri(void); //vaiheiden_luonti.cpp
extern void numpad_updates_vaiheen_sek_mode_parametri(int mode);
extern int numpad_selected_temp_mode;//0=Norm.COOK, 1=vaippaohjaus, 2=delta, 3=ext temp, 4=Chill
extern void numpad_updates_vaiheen_selected_temp_mode_parametri(void);

/*
//INPUT BITS 10 kpl:
#define  WATER_METER_IN_BIT   //vesimittarin pulssitulo, ak:	w_meter		PORTA,4

#define	KETTLE_POSITION_IN_BIT // ak: position PORTD,0     ;Padan asentokytkin
#define	MIXING_SWITCH_IN_BIT   // ak: mixing_sw	PORTD,1	; Antaa sekoitusluvan
#define	LID_SWITCH1_IN_BIT     // ak:	PORTD,2		         ; kansikytkin #1
#define  LID_SWITCH2_IN_BIT

#define	POWER_DOWN_IN_BIT      // ak:power_down	PORTD,3	; power_down -tieto
#define	CONTACTOR_IN_BIT       // ak: contactor	PORTD,4	; kontaktorin kärkitieto
#define	FLOAT1_IN_BIT          // ak: float		PORTD,5		; uimuritieto
#define	FLOAT2_IN_BIT

#define	EMERGENCY_STOP_IN_BIT  // ak:	PORTD,6		; hätä-seis

//OUTPUT BITS 11 kpl + 1 DAC-OUT
#define CHILL_OUT_BIT            //vaipan täyttö...ak: chill_out	LATB,4		; chill
#define EMPTYING_OUT_BIT         //ak: emptying	   LATE,0
#define COLD_WATER_OUT_BIT       //kylmä vesi vaippaan jäähdytyksessä/ohijuoksutuksessa (ak:cold_water	LATC,1)
#define FOOD_WATER_COLD_OUT_BIT  //ak: water_out	LATB,5		; Ruokavesi
#define FOOD_WATER_HOT_OUT_BIT

//Invertteri:
#define	INVERTER_ON_OUT_BIT  // invertteri "on"
#define	INVERTER_FWD_OUT_BIT	// ohjaus forward
#define	INVERTER_0_10V_OUT   // DAC-OUT alakortti:pwm-lähtöpiikki

#define	HEAT_OUT_BIT           // ak: heat		LATD,7		; lämmityksen ohjaus

#define	TILT_UP_OUT_BIT       //ak: tiltpwm_up	LATE,1	; kallistus ylös
#define	TILT_DOWN_OUT_BIT     //ak: tiltpwm_dwn	LATC,0	; kallistus alas
#define	TILT_LIMIT_EN_OUT_BIT //ak: limit_en	LATE,2		; kallistuksen enable

*/
#endif