/*
   iotest.cpp
   ESC:illä poistutaan manuaal-moodiin
*/
#include <stdio.h>
#include <string>
#include <unistd.h>

//Using SDL, SDL Threads, SDL_image, standard IO, and, strings
#include <SDL.h>

//#include </home/lasse/var-fslc-yocto/build_x11/tmp/sysroots-components/armv7at2hf-neon-mx7d/libsdl2/usr/include/SDL2/SDL_thread.h>
#include <SDL_thread.h>
//#include </home/lasse/var-fslc-yocto/build_x11/tmp/sysroots-components/armv7at2hf-neon/libsdl2-image/usr/include/SDL2/SDL_image.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "menu_ids.h"
#include "handler.h"
#include "var.h"
#include "externs.h"
#include "kettle_functions.h"

typedef unsigned char     uint8;  /* etumerkitön 8 bittinen kokonaisluku     */
typedef unsigned short   uint16;  /* etumerkitön 16 bittinen kokonaisluku    */
typedef unsigned long    uint32;  /* etumerkitön 32 bittinen kokonaisluku    */
typedef char              sint8;  /* etumerkillinen 8 bittinen kokonaisluku  */
typedef short            sint16;  /* etumerkillinen 16 bittinen kokonaisluku */
typedef long             sint32;  /* etumerkillinen 32-bittinen kokonaisluku */
typedef unsigned int      LIPPU;  /* BOOLEAN  0=EPÄTOSI   !0=TOSI            */
typedef unsigned char      TAVU;  /* 8 bittiä                                */
typedef unsigned char   BOOLEAN;

typedef unsigned char u8;
typedef unsigned short int  u16;

void show_iotesti_valinnat(void);
void set_iotesti_valinnat(void);

void print_an1_motor_speed(char * buff, int ad);
void print_an0_pressure(char * buff);

#define ERROR_IO_KORTTI1_EI_VASTAA 0
#define ERROR_IO_KORTTI2_EI_VASTAA 1

int errors_on[20];

#define MY_COLOR_BLUE 0
#define MY_COLOR_RED 1
#define MY_COLOR_GREEN 2
#define MY_COLOR_BLACK 3
#define MY_COLOR_WHITE 4
#define MY_COLOR_GREY 5

#define MY_FONT_KESKI 0
#define MY_FONT_PIENI 1
#define MY_FONT_ISO   2
#define MY_FONT_VERY_BIG 3
#define SAMPLE_FONT 10

int check_iotest_mode_return(int x, int y); //Jos painetaan ruudun alaosaa --> iotest-moodista pois

//The window renderer
extern SDL_Renderer* gRenderer;

int check_valinta(int x, int y);
int valittu_saato, exit_io_test;

extern int timeout_cnt;

void init_IOTEST(void)
{
}

void show_IOTEST(int show_hide)
{
}

void draw_IOTEST(int param1);
int handler_IOTEST(int x, int y,  SDL_Event *e)
{
//   printf("In iotest..\n");
//   if (event_happened)
   set_iotesti_valinnat();

   {
      if(e->type == SDL_FINGERDOWN)
      {
//   	   x = e->tfinger.x * SCREEN_WIDTH; //gScreenRect.w;
//         y = e->tfinger.y * SCREEN_HEIGHT; //gScreenRect.h;
         //printf("Finger down x=%d, y=%d\n", x, y);
         valittu_saato = check_valinta(x, y);
         exit_io_test = check_iotest_mode_return(x, y);
         if (exit_io_test)
            return ID_KOTINAKYMA;
      }
      else if(e->type == SDL_MOUSEBUTTONDOWN)
      {
         //Get mouse position
//         SDL_GetMouseState( &x, &y );
         printf("Mouse x=%d y=%d\n", x, y);
         valittu_saato = check_valinta(x, y);
         exit_io_test = check_iotest_mode_return(x, y);
         if (exit_io_test)
            return ID_KOTINAKYMA;
	   }
      else if (e->type == SDL_MOUSEBUTTONUP||e->type == SDL_FINGERUP)
      {
//         if (exit_io_test)
//            prog_tila = TILA_MANUAL_VALINTA;
      }
   }
//   else exit_io_test=0;
//   renderText = 0;
   draw_IOTEST(0);

   return 0xFFFF;
}

void text_with_kehys(char * str, int x, int y, int val_box_idx);

#define CARD2_DIG_INPUTS_Y 466
#define CARD2_DIG_INPUTS_NAME_Y (CARD2_DIG_INPUTS_Y+30)
#define CARD2_DIG_OUTPUTS_Y 466
#define CARD2_DIG_OUTPUTS_NAME_Y (CARD2_DIG_OUTPUTS_Y+30)
#define CARD2_24V_OUTPUTS_Y 466
#define CARD2_24V_OUTPUTS_NAME_Y (CARD2_24V_OUTPUTS_Y+30)
#define CARD2_ANA_INPUTS_Y 466
#define CARD2_ANA_INPUTS_NAME_Y (CARD2_ANA_INPUTS_Y+30)
#define CARD2_ANA_OUTPUTS_Y 466
#define CARD2_ANA_OUTPUTS_NAME_Y (CARD2_ANA_OUTPUTS_Y+30)
#define CARD2_TEMP_INPUTS_Y 466
#define CARD2_TEMP_INPUTS_NAME_Y (CARD2_TEMP_INPUTS_Y+30)

#define Y_SHIFT 35

#define DIG_INPUTS_X 15
#define DIG_INPUTS_Y 66
#define DIG_INPUTS_FONT MY_FONT_KESKI

#define DIG_INPUTS_NAME_X (DIG_INPUTS_X)
#define DIG_INPUTS_NAME_Y (DIG_INPUTS_Y+30)
#define DIG_INPUTS_NAME_FONT MY_FONT_KESKI
const char * DIG_INPUTS_NAMES[8] = {"INPUT1", "INPUT2", "INPUT3", "INPUT4", "INPUT5", "INPUT6", "INPUT7", "INPUT8"};

#define DIG_OUTPUTS_X 130
#define DIG_OUTPUTS_Y 66
#define DIG_OUTPUTS_FONT MY_FONT_KESKI

#define DIG_OUTPUTS_NAME_X (DIG_OUTPUTS_X)
#define DIG_OUTPUTS_NAME_Y (DIG_OUTPUTS_Y+30)
#define DIG_OUTPUTS_NAME_FONT MY_FONT_KESKI
const char * DIG_OUTPUTS_NAMES[4] = {"OUTPUT1", "OUTPUT2", "OUTPUT3", "OUTPUT4"};

#define _24V_OUTPUTS_X 265
#define _24V_OUTPUTS_Y 66
#define _24V_OUTPUTS_FONT MY_FONT_KESKI

#define _24V_OUTPUTS_NAME_X (_24V_OUTPUTS_X)
#define _24V_OUTPUTS_NAME_Y (_24V_OUTPUTS_Y+30)
#define _24V_OUTPUTS_NAME_FONT MY_FONT_KESKI
const char * _24V_OUTPUTS_NAMES[8] = {"24V OUT1", "24V OUT2", "24V OUT3", "24V OUT4", "24V OUT5", "24V OUT6", "24V OUT7", "24V OUT8"};

#define ANA_INPUTS_X 400
#define ANA_INPUTS_Y 66
#define ANA_INPUTS_FONT MY_FONT_KESKI

#define ANA_INPUTS_NAME_X (ANA_INPUTS_X)
#define ANA_INPUTS_NAME_Y (ANA_INPUTS_Y+30)
#define ANA_INPUTS_NAME_FONT MY_FONT_KESKI
const char * ANA_INPUTS_NAMES[8] = {"A.IN1", "A.IN2", "A.IN3", "A.IN4", "A.IN5", "A.IN6", "A.IN7", "A.IN8"};

#define ANA_OUTPUTS_X 540
#define ANA_OUTPUTS_Y 66
#define ANA_OUTPUTS_FONT MY_FONT_KESKI

#define ANA_OUTPUTS_NAME_X (ANA_OUTPUTS_X)
#define ANA_OUTPUTS_NAME_Y (ANA_OUTPUTS_Y+30)
#define ANA_OUTPUTS_NAME_FONT MY_FONT_KESKI
const char * ANA_OUTPUTS_NAMES[4] = {"PWM 1", "A.OUT2", "A.OUT3", "A.OUT4"};
const char * CARD2_ANA_OUTPUTS_NAMES[4] = {"A.OUT1", "A.OUT2", "A.OUT3", "A.OUT4"};

#define TEMP_INPUTS_X 680
#define TEMP_INPUTS_Y 66
#define TEMP_INPUTS_FONT MY_FONT_KESKI

#define TEMP_INPUTS_NAME_X (TEMP_INPUTS_X)
#define TEMP_INPUTS_NAME_Y (TEMP_INPUTS_Y+30)
#define TEMP_INPUTS_NAME_FONT MY_FONT_KESKI
const char * TEMP_INPUTS_NAMES[8] = {"T1", "T2", "T3", "T4", "T5", "T6", "T7", "T8"};

#define CLR_ALL_OUTPUTS_X 360
#define CLR_ALL_OUTPUTS_Y 390
#define CLR_ALL_OUTPUTS_FONT MY_FONT_KESKI

#define DIG_OUTPUTS_VALINTA_IDX 0
#define _24V_OUTPUTS_VALINTA_IDX 4
#define ANA_OUTPUTS_VALINTA_IDX 12
#define CLR_ALL_OUTPUTS_VALINTA_IDX 14 //16

#define CARD2_DIG_OUTPUTS_VALINTA_IDX 15
#define CARD2_24V_OUTPUTS_VALINTA_IDX (15+4)
#define CARD2_ANA_OUTPUTS_VALINTA_IDX (15+12)

#define HEAT_ON_BOX_X 20
#define HEAT_ON_BOX_Y 800
#define HEAT_ON_VALINTA_IDX (15+14)

#define CHILL_ON_BOX_X 280
#define CHILL_ON_BOX_Y 800
#define CHILL_ON_VALINTA_IDX (15+14+1)

#define EMPTY_ON_BOX_X 540
#define EMPTY_ON_BOX_Y 800
#define EMPTY_ON_VALINTA_IDX (15+14+2)

#define MIX_FWD_BOX_X 20
#define MIX_FWD_BOX_Y 880
#define MIX_FWD_VALINTA_IDX (15+14+3)

#define MIX_REW_BOX_X 280
#define MIX_REW_BOX_Y 880
#define MIX_REW_VALINTA_IDX (15+14+4)

#define TILT_UP_BOX_X 20
#define TILT_UP_BOX_Y 960
#define TILT_UP_VALINTA_IDX (15+14+5)

#define TILT_DN_BOX_X 280
#define TILT_DN_BOX_Y 960
#define TILT_DN_VALINTA_IDX (15+14+6)

#define WATER_ON_BOX_X 280
#define WATER_ON_BOX_Y 1040
#define WATER_ON_VALINTA_IDX (15+14+7)

#define NUM_VALINTA (15+14+8) //+7 = heat/chill/empty...jne
SDL_Rect valinta_rects[NUM_VALINTA];
u16 valinta_outputs[NUM_VALINTA];
void clear_outputs(void);

const SDL_Color colorblue = { 0, 0, 255, 255 };
const SDL_Color colorred = { 255, 0, 0, 255 };
const SDL_Color colorblack = { 0, 0, 0, 0 };
const SDL_Color colorgreen = { 0, 255, 0, 0 };

extern TTF_Font *font_norm_bold_18px;

extern void render_text(char * str, int x, int y, SDL_Color *color_p, TTF_Font *font_p, int utf8=0);
int show_ttf(SDL_Renderer *renderer, char *text, int color, int x, int y, int font_koko)
{
   render_text(text, x, y, (SDL_Color *)&colorblack, font_norm_bold_18px, 1);
   return 0;
}

extern int show_ttf(SDL_Renderer *renderer, char *text, int color, int x, int y, int font_koko=0); //font_koko=1 jos pieni fontti, 2 jos isoin

void draw_IOTEST(int param1)
{
   int i, y;
   char buff[22];

   show_ttf(gRenderer, (char *)"IO TESTI TILA", MY_COLOR_BLACK, 300, 10, MY_FONT_VERY_BIG);
   if (errors_on[ERROR_IO_KORTTI1_EI_VASTAA])
   {
      show_ttf(gRenderer, (char *)"KORTTI 1 EI OK!", MY_COLOR_RED, 15, 38);
      clear_outputs();
   }
   else
      show_ttf(gRenderer, (char *)"KORTTI 1:", MY_COLOR_BLUE, 15, 38);
   /*---------DIG. INPUTS näyttö --------------------*/
   show_ttf(gRenderer, (char *)"Digit. tulot:",  MY_COLOR_BLACK, DIG_INPUTS_X, DIG_INPUTS_Y, DIG_INPUTS_FONT);
   for (i=0; i<8; i++)
   {
      show_ttf(gRenderer, (char *)DIG_INPUTS_NAMES[i],  MY_COLOR_BLACK, DIG_INPUTS_NAME_X, DIG_INPUTS_NAME_Y + i*Y_SHIFT, DIG_INPUTS_NAME_FONT);
      if (input_registers[INPUT_REG_CARD1_DIGITAL_INPUTS] & (1<<i))
         show_ttf(gRenderer, (char *)"=ON",  MY_COLOR_BLUE, DIG_INPUTS_NAME_X + 55, DIG_INPUTS_NAME_Y + i*Y_SHIFT, DIG_INPUTS_NAME_FONT);
      else
         show_ttf(gRenderer, (char *)"=__",  MY_COLOR_BLUE, DIG_INPUTS_NAME_X + 55, DIG_INPUTS_NAME_Y + i*Y_SHIFT, DIG_INPUTS_NAME_FONT);
   }
   /*---------DIG. OUTPUTS (4kpl) näyttö --------------------*/
   show_ttf(gRenderer, (char *)"Digit. lähdöt:",  MY_COLOR_BLACK, DIG_OUTPUTS_X, DIG_OUTPUTS_Y, DIG_OUTPUTS_FONT);
   for (i=0; i<4; i++)
   {
//      show_ttf(gRenderer, (char *)DIG_OUTPUTS_NAMES[i],  MY_COLOR_BLACK, DIG_OUTPUTS_NAME_X, DIG_OUTPUTS_NAME_Y + i*Y_SHIFT, DIG_OUTPUTS_NAME_FONT);
      text_with_kehys((char *)DIG_OUTPUTS_NAMES[i], DIG_OUTPUTS_NAME_X, DIG_OUTPUTS_NAME_Y + i*Y_SHIFT, DIG_OUTPUTS_VALINTA_IDX+i);
      if (valinta_outputs[DIG_OUTPUTS_VALINTA_IDX+i] == 0)
      {
         show_ttf(gRenderer, (char *)"=__",  MY_COLOR_BLUE, DIG_OUTPUTS_NAME_X + 72, DIG_OUTPUTS_NAME_Y + i*Y_SHIFT, DIG_OUTPUTS_NAME_FONT);
         output_registers[OUTPUT_REG_CARD1_DIGITAL_OUTPUTS] &= ~(1<<i);
      }
      else
      {
         show_ttf(gRenderer, (char *)"=ON",  MY_COLOR_BLUE, DIG_OUTPUTS_NAME_X + 72, DIG_OUTPUTS_NAME_Y + i*Y_SHIFT, DIG_OUTPUTS_NAME_FONT);
         output_registers[OUTPUT_REG_CARD1_DIGITAL_OUTPUTS] |= (1<<i);
      }
   }
   /*---------24V OUTPUTS (8kpl) näyttö --------------------*/
   show_ttf(gRenderer, (char *)"24V lähdöt:",  MY_COLOR_BLACK, _24V_OUTPUTS_X, _24V_OUTPUTS_Y, _24V_OUTPUTS_FONT);
   for (i=0; i<8; i++)
   {
      //show_ttf(gRenderer, (char *)_24V_OUTPUTS_NAMES[i],  MY_COLOR_BLACK, _24V_OUTPUTS_NAME_X, _24V_OUTPUTS_NAME_Y + i*Y_SHIFT, _24V_OUTPUTS_NAME_FONT);
      text_with_kehys((char *)_24V_OUTPUTS_NAMES[i], _24V_OUTPUTS_NAME_X, _24V_OUTPUTS_NAME_Y + i*Y_SHIFT, _24V_OUTPUTS_VALINTA_IDX+i);
      if (valinta_outputs[_24V_OUTPUTS_VALINTA_IDX+i] == 0)
      {
         show_ttf(gRenderer, (char *)"=__",  MY_COLOR_BLUE, _24V_OUTPUTS_NAME_X + 72, _24V_OUTPUTS_NAME_Y + i*Y_SHIFT, _24V_OUTPUTS_NAME_FONT);
         output_registers[OUTPUT_REG_CARD1_DIGITAL_OUTPUTS] &= ~(1<<(_24V_OUTPUTS_VALINTA_IDX+i));
      }
      else
      {
         show_ttf(gRenderer, (char *)"=ON",  MY_COLOR_BLUE, _24V_OUTPUTS_NAME_X + 72, _24V_OUTPUTS_NAME_Y + i*Y_SHIFT, _24V_OUTPUTS_NAME_FONT);
         output_registers[OUTPUT_REG_CARD1_DIGITAL_OUTPUTS] |= (1<<(_24V_OUTPUTS_VALINTA_IDX+i));
      }
   }
   /*---------ANAL. INPUTS näyttö --------------------*/
   show_ttf(gRenderer, (char *)"Analog. tulot:",  MY_COLOR_BLACK, ANA_INPUTS_X, ANA_INPUTS_Y, ANA_INPUTS_FONT);
   for (i=0; i<2; i++)
   {
      show_ttf(gRenderer, (char *)ANA_INPUTS_NAMES[i],  MY_COLOR_BLACK, ANA_INPUTS_NAME_X, ANA_INPUTS_NAME_Y + i*Y_SHIFT, ANA_INPUTS_NAME_FONT);
      sprintf(buff, "=%05d", input_registers[INPUT_REG_CARD1_ANALOG_INPUT_1 + i]);
      show_ttf(gRenderer, buff,  MY_COLOR_BLUE, ANA_INPUTS_NAME_X + 45, ANA_INPUTS_NAME_Y + i*Y_SHIFT, ANA_INPUTS_NAME_FONT);
   }
   /*---------ANAL. OUTPUTS (2kpl) näyttö --------------------*/
   show_ttf(gRenderer, (char *)"Analog. lähdöt:",  MY_COLOR_BLACK, ANA_OUTPUTS_X, ANA_OUTPUTS_Y, ANA_OUTPUTS_FONT);
   for (i=0; i<2; i++)
   {
      //show_ttf(gRenderer, (char *)ANA_OUTPUTS_NAMES[i],  MY_COLOR_BLACK, ANA_OUTPUTS_NAME_X, ANA_OUTPUTS_NAME_Y + i*Y_SHIFT, ANA_OUTPUTS_NAME_FONT);
      text_with_kehys((char *)ANA_OUTPUTS_NAMES[i], ANA_OUTPUTS_NAME_X, ANA_OUTPUTS_NAME_Y + i*Y_SHIFT, ANA_OUTPUTS_VALINTA_IDX+i);
      sprintf(buff, "=%05d", output_registers[OUTPUT_REG_CARD1_ANALOG_OUTPUT_1 + i]);
      show_ttf(gRenderer, buff,  MY_COLOR_BLUE, ANA_OUTPUTS_NAME_X + 60, ANA_OUTPUTS_NAME_Y + i*Y_SHIFT, ANA_OUTPUTS_NAME_FONT);
   }
   /*---------Temperatures näyttö --------------------*/
   show_ttf(gRenderer, (char *)"Lämpötilat:",  MY_COLOR_BLACK, TEMP_INPUTS_X, TEMP_INPUTS_Y, TEMP_INPUTS_FONT);
   for (y=0, i=0; i<2; i++)
   {
      signed char s;
      s = input_registers[INPUT_REG_CARD1_READ_FOOD_TEMP + i]; //16-bit signed value
      sprintf(buff, "=%d°C", s);
      show_ttf(gRenderer, (char *)TEMP_INPUTS_NAMES[y], MY_COLOR_BLACK, TEMP_INPUTS_NAME_X, TEMP_INPUTS_NAME_Y + y*Y_SHIFT, TEMP_INPUTS_NAME_FONT);
      show_ttf(gRenderer, buff,  MY_COLOR_BLUE, TEMP_INPUTS_NAME_X + 20, TEMP_INPUTS_NAME_Y + y*Y_SHIFT, TEMP_INPUTS_NAME_FONT);
      ++y;
   }

/*--------------------------------------------------------------------------------------------------------------------KORTTI 2-------------*/
   if (errors_on[ERROR_IO_KORTTI2_EI_VASTAA])
   {
      show_ttf(gRenderer, (char *)"KORTTI 2 EI OK!", MY_COLOR_RED, 15, 438);
//      clear_outputs();
   }
   else
      show_ttf(gRenderer, (char *)"KORTTI 2:", MY_COLOR_BLUE, 15, 438);
   /*---------DIG. INPUTS näyttö --------------------*/
   show_ttf(gRenderer, (char *)"Digit. tulot:",  MY_COLOR_BLACK, DIG_INPUTS_X, CARD2_DIG_INPUTS_Y, DIG_INPUTS_FONT);
   for (i=0; i<8; i++)
   {
      show_ttf(gRenderer, (char *)DIG_INPUTS_NAMES[i],  MY_COLOR_BLACK, DIG_INPUTS_NAME_X, CARD2_DIG_INPUTS_NAME_Y + i*Y_SHIFT, DIG_INPUTS_NAME_FONT);
      if (input_registers[INPUT_REG_CARD2_DIGITAL_INPUTS] & (1<<i))
         show_ttf(gRenderer, (char *)"=ON",  MY_COLOR_BLUE, DIG_INPUTS_NAME_X + 55, CARD2_DIG_INPUTS_NAME_Y + i*Y_SHIFT, DIG_INPUTS_NAME_FONT);
      else
         show_ttf(gRenderer, (char *)"=__",  MY_COLOR_BLUE, DIG_INPUTS_NAME_X + 55, CARD2_DIG_INPUTS_NAME_Y + i*Y_SHIFT, DIG_INPUTS_NAME_FONT);
   }
   /*---------DIG. OUTPUTS (4kpl) näyttö --------------------*/
   show_ttf(gRenderer, (char *)"Digit. lähdöt:",  MY_COLOR_BLACK, DIG_OUTPUTS_X, CARD2_DIG_OUTPUTS_Y, DIG_OUTPUTS_FONT);
   for (i=0; i<4; i++)
   {
//      show_ttf(gRenderer, (char *)DIG_OUTPUTS_NAMES[i],  MY_COLOR_BLACK, DIG_OUTPUTS_NAME_X, DIG_OUTPUTS_NAME_Y + i*Y_SHIFT, DIG_OUTPUTS_NAME_FONT);
      text_with_kehys((char *)DIG_OUTPUTS_NAMES[i], DIG_OUTPUTS_NAME_X, CARD2_DIG_OUTPUTS_NAME_Y + i*Y_SHIFT, CARD2_DIG_OUTPUTS_VALINTA_IDX+i);
      if (valinta_outputs[CARD2_DIG_OUTPUTS_VALINTA_IDX+i] == 0)
      {
         show_ttf(gRenderer, (char *)"=__",  MY_COLOR_BLUE, DIG_OUTPUTS_NAME_X + 72, CARD2_DIG_OUTPUTS_NAME_Y + i*Y_SHIFT, DIG_OUTPUTS_NAME_FONT);
         output_registers[OUTPUT_REG_CARD2_DIGITAL_OUTPUTS] &= ~(1<<i);
      }
      else
      {
         show_ttf(gRenderer, (char *)"=ON",  MY_COLOR_BLUE, DIG_OUTPUTS_NAME_X + 72, CARD2_DIG_OUTPUTS_NAME_Y + i*Y_SHIFT, DIG_OUTPUTS_NAME_FONT);
         output_registers[OUTPUT_REG_CARD2_DIGITAL_OUTPUTS] |= (1<<i);
      }
   }
   /*---------24V OUTPUTS (8kpl) näyttö --------------------*/
   show_ttf(gRenderer, (char *)"24V lähdöt:",  MY_COLOR_BLACK, _24V_OUTPUTS_X, CARD2_24V_OUTPUTS_Y, _24V_OUTPUTS_FONT);
   for (i=0; i<8; i++)
   {
      //show_ttf(gRenderer, (char *)_24V_OUTPUTS_NAMES[i],  MY_COLOR_BLACK, _24V_OUTPUTS_NAME_X, _24V_OUTPUTS_NAME_Y + i*Y_SHIFT, _24V_OUTPUTS_NAME_FONT);
      text_with_kehys((char *)_24V_OUTPUTS_NAMES[i], _24V_OUTPUTS_NAME_X, CARD2_24V_OUTPUTS_NAME_Y + i*Y_SHIFT, CARD2_24V_OUTPUTS_VALINTA_IDX+i);
      if (valinta_outputs[CARD2_24V_OUTPUTS_VALINTA_IDX+i] == 0)
      {
         show_ttf(gRenderer, (char *)"=__",  MY_COLOR_BLUE, _24V_OUTPUTS_NAME_X + 72, CARD2_24V_OUTPUTS_NAME_Y + i*Y_SHIFT, _24V_OUTPUTS_NAME_FONT);
         output_registers[OUTPUT_REG_CARD2_DIGITAL_OUTPUTS] &= ~(1<<(_24V_OUTPUTS_VALINTA_IDX+i));
      }
      else
      {
         show_ttf(gRenderer, (char *)"=ON",  MY_COLOR_BLUE, _24V_OUTPUTS_NAME_X + 72, CARD2_24V_OUTPUTS_NAME_Y + i*Y_SHIFT, _24V_OUTPUTS_NAME_FONT);
         output_registers[OUTPUT_REG_CARD2_DIGITAL_OUTPUTS] |= (1<<(_24V_OUTPUTS_VALINTA_IDX+i));
      }
   }
   /*---------ANAL. INPUTS näyttö --------------------*/
   show_ttf(gRenderer, (char *)"Analog. tulot:",  MY_COLOR_BLACK, ANA_INPUTS_X, CARD2_ANA_INPUTS_Y, ANA_INPUTS_FONT);
   for (i=0; i<2; i++)
   {
      show_ttf(gRenderer, (char *)ANA_INPUTS_NAMES[i],  MY_COLOR_BLACK, ANA_INPUTS_NAME_X, CARD2_ANA_INPUTS_NAME_Y + i*Y_SHIFT, ANA_INPUTS_NAME_FONT);
      sprintf(buff, "=%05d", input_registers[INPUT_REG_CARD2_ANALOG_INPUT_1 + i]);
      show_ttf(gRenderer, buff,  MY_COLOR_BLUE, ANA_INPUTS_NAME_X + 45, CARD2_ANA_INPUTS_NAME_Y + i*Y_SHIFT, ANA_INPUTS_NAME_FONT);
   }
   /*---------ANAL. OUTPUTS (2kpl) näyttö --------------------*/
   show_ttf(gRenderer, (char *)"Analog. lähdöt:",  MY_COLOR_BLACK, ANA_OUTPUTS_X, CARD2_ANA_OUTPUTS_Y, ANA_OUTPUTS_FONT);
   for (i=0; i<2; i++)
   {
      //show_ttf(gRenderer, (char *)ANA_OUTPUTS_NAMES[i],  MY_COLOR_BLACK, ANA_OUTPUTS_NAME_X, ANA_OUTPUTS_NAME_Y + i*Y_SHIFT, ANA_OUTPUTS_NAME_FONT);
      text_with_kehys((char *)CARD2_ANA_OUTPUTS_NAMES[i], ANA_OUTPUTS_NAME_X, CARD2_ANA_OUTPUTS_NAME_Y + i*Y_SHIFT, CARD2_ANA_OUTPUTS_VALINTA_IDX+i);
      sprintf(buff, "=%05d", output_registers[OUTPUT_REG_CARD2_ANALOG_OUTPUT_1 + i]);
      show_ttf(gRenderer, buff,  MY_COLOR_BLUE, ANA_OUTPUTS_NAME_X + 60, CARD2_ANA_OUTPUTS_NAME_Y + i*Y_SHIFT, ANA_OUTPUTS_NAME_FONT);
   }
   /*---------Temperatures näyttö --------Not for card 2------------*/
/*--------------------------------------------------------------------------------------------------------------------KORTTI 2-------------*/
   text_with_kehys((char *)"CLEAR OUTPUTS", CLR_ALL_OUTPUTS_X, CLR_ALL_OUTPUTS_Y, CLR_ALL_OUTPUTS_VALINTA_IDX);
   if (valinta_outputs[CLR_ALL_OUTPUTS_VALINTA_IDX] != 0)
   {
      clear_outputs();
   }

   if (valittu_saato)
   {
      sprintf(buff, "valittu=%d", valittu_saato);
      text_with_kehys(buff, 100, 400, 1000);
   }

   show_iotesti_valinnat();
}

//extern TTF_Font *font;


void text_with_kehys(char * str, int x, int y, int vali_idx)
{
   SDL_Rect dst;
   SDL_Surface *surf;
   if(((vali_idx >= HEAT_ON_VALINTA_IDX) && (vali_idx <= WATER_ON_VALINTA_IDX)) || (vali_idx == CLR_ALL_OUTPUTS_VALINTA_IDX))
      surf = TTF_RenderUTF8_Blended(BigFont44, str, colorblack);
   else
      surf = TTF_RenderUTF8_Blended(font_norm_bold_18px, str, colorblack);
	if (surf == nullptr){
		return;
	}
	SDL_Texture *texture = SDL_CreateTextureFromSurface(gRenderer, surf);
	if (texture != nullptr)
   {
   	//Get the texture w/h so we can center it in the screen
	   SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
   	dst.x = x;
	   dst.y = y;
	   //renderTexture(texture, gRenderer, dst);
      SDL_RenderCopy(gRenderer, texture, nullptr, &dst);
      if (vali_idx+1 == valittu_saato)
         SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
      else
         SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
      dst.x -= 2;
      dst.y -= 2;
      dst.w += 4;
      dst.h += 4;

      SDL_RenderDrawRect(gRenderer, &dst);
      if (vali_idx < NUM_VALINTA)
         valinta_rects[vali_idx] = dst;
      if (vali_idx+1 == valittu_saato)
      {
         dst.x--;
         dst.y--;
         dst.w += 2;
         dst.h += 2;
         SDL_RenderDrawRect(gRenderer, &dst);
      }
      SDL_DestroyTexture(texture);
   }
	//Clean up the surface
	SDL_FreeSurface(surf);
}

int check_valinta(int x, int y)
{
   int i;
   for (i=0; i< NUM_VALINTA; i++)
   {
      if ((x < valinta_rects[i].x) || (x > (valinta_rects[i].x+valinta_rects[i].w)))
         continue;
      if ((y < valinta_rects[i].y) || (y > (valinta_rects[i].y+valinta_rects[i].h)))
         continue;
      if (valinta_outputs[i])
      {
         valinta_outputs[i] = 0;
         if (i == HEAT_ON_VALINTA_IDX)
            set_cooking(0); //Cook OFF
         else if (i == CHILL_ON_VALINTA_IDX)
            set_chilling(0);
         else if (i == EMPTY_ON_VALINTA_IDX)
            set_emptying(0);
         else if (i == WATER_ON_VALINTA_IDX)
            set_water_fill(0);
         else if ((i == TILT_UP_VALINTA_IDX) || (i == TILT_DN_VALINTA_IDX))
            set_tilting(0);
         else if ((i == MIX_FWD_VALINTA_IDX) || (i == MIX_REW_VALINTA_IDX))
            set_mixing(0);
      }
      else
      {
         if ((i == TILT_UP_VALINTA_IDX) && (valinta_outputs[TILT_DN_VALINTA_IDX] || !onko_kansi_auki()) )
            return 0;
         if ((i == TILT_DN_VALINTA_IDX) && (valinta_outputs[TILT_UP_VALINTA_IDX] || !onko_kansi_auki()) )
            return 0;
         if ((i == MIX_FWD_VALINTA_IDX) && (valinta_outputs[MIX_REW_VALINTA_IDX] || onko_kansi_auki()) )
            return 0;
         if ((i == MIX_REW_VALINTA_IDX) && (valinta_outputs[MIX_FWD_VALINTA_IDX] || onko_kansi_auki()) )
            return 0;

         valinta_outputs[i] = 1;
         if (i != CLR_ALL_OUTPUTS_VALINTA_IDX)
            valinta_outputs[CLR_ALL_OUTPUTS_VALINTA_IDX] = 0;
         if (i == HEAT_ON_VALINTA_IDX)
         {
            run_target_params.cook_temps[TARGET_COOK_FOOD_TEMP_IDX] = 150;
            set_emptying(0);
            set_emptying(1); //vaipan tyhjennys
            set_cooking(1); //Cook ON
         }
         else if (i == CHILL_ON_VALINTA_IDX)
         {
            run_target_params.chill_temps[TARGET_CHILL_TEMP_IDX] = -150;
            set_chilling(1);
         }
         else if (i == EMPTY_ON_VALINTA_IDX)
            set_emptying(1);
         else if (i == WATER_ON_VALINTA_IDX)
         {
            set_water_fill(0);
            set_water_fill(run_target_params.water_fill_amount_litres = 5);
         }
         else if (i == TILT_UP_VALINTA_IDX)
         {
            run_target_params.tilt_mode = 1; //1=UP, 2=Down
            set_tilting(1);
         }
         else if (i == TILT_DN_VALINTA_IDX)
         {
            run_target_params.tilt_mode = 2; //1=UP, 2=Down
            set_tilting(1);
         }
         else if (i == MIX_FWD_VALINTA_IDX)
         {
            run_target_params.mixer_speed = 20;
            set_mixing(MIX_MODES_YHTEEN_SUUNTAAN);
            mixer_motor_vaihe = MIXER_MOTOR_RUN_ETEEN;
         }
         else if (i == MIX_REW_VALINTA_IDX)
         {
            run_target_params.mixer_speed = 20;
            set_mixing(MIX_MODES_YHTEEN_SUUNTAAN);
            mixer_motor_vaihe = MIXER_MOTOR_RUN_TAAKSE;
         }
      }
      return i+1;
   }
   return 0;
}

void clear_outputs(void)
{
   int i;
   output_registers[OUTPUT_REG_CARD1_DIGITAL_OUTPUTS] = 0; //4*dig + 8*24V lähtöä tässä, valintaboxit:DIG_OUTPUTS_VALINTA_IDX(4) + valinta_outputs[_24V_OUTPUTS_VALINTA_IDX](8)
   output_registers[OUTPUT_REG_CARD2_DIGITAL_OUTPUTS] = 0;
   for (i=0; i<4; i++)
      valinta_outputs[DIG_OUTPUTS_VALINTA_IDX+i] = 0;
   for (i=0; i<2; i++)
   {
      valinta_outputs[ANA_OUTPUTS_VALINTA_IDX+i] = 0;
      output_registers[OUTPUT_REG_CARD1_ANALOG_OUTPUT_1 + i] = 0;
      output_registers[OUTPUT_REG_CARD2_ANALOG_OUTPUT_1 + i] = 0;
   }
   for (i=0; i<8; i++)
   {
      valinta_outputs[_24V_OUTPUTS_VALINTA_IDX+i] = 0;
   }
   valinta_outputs[HEAT_ON_VALINTA_IDX] = 0;
   valinta_outputs[CHILL_ON_VALINTA_IDX] = 0;
   valinta_outputs[EMPTY_ON_VALINTA_IDX] = 0;
   valinta_outputs[MIX_FWD_VALINTA_IDX] = 0;
   valinta_outputs[MIX_REW_VALINTA_IDX] = 0;
   valinta_outputs[TILT_UP_VALINTA_IDX] = 0;
   valinta_outputs[TILT_DN_VALINTA_IDX] = 0;
   valinta_outputs[WATER_ON_VALINTA_IDX] = 0;
   load_run_parameters(); //Nollaa run parametrit defaulteihin
}
//
//hdmi iMx6/uboot:
//setenv videoargs 'setenv bootargs ${bootargs} video=mxcfb0:dev=hdmi,1920x1080M@60,if=RGB24 video=mxcfb1:dev=ldb,800x480M@60,if=RGB24;'
//

const int SCREEN_HEIGHT = 1280;
const int ANT_BOX_H = 144;
const int VALVES_BOX_Y = (5*ANT_BOX_H);

int check_iotest_mode_return(int x, int y) //Jos painetaan (x > 435) && (y 605 - 745)) --> iotest-moodista pois
{
   if ((x > 435) && (y > 605) && (y < 745))
   {
      return 1;
   }
   else
      return 0;
}

void set_iotesti_valinnat(void)
{
   int i;

   for (i=0; i<4; i++)
   {
      valinta_outputs[DIG_OUTPUTS_VALINTA_IDX+i] = output_registers[OUTPUT_REG_CARD1_DIGITAL_OUTPUTS] & (1<<i);
      valinta_outputs[CARD2_DIG_OUTPUTS_VALINTA_IDX+i] = output_registers[OUTPUT_REG_CARD2_DIGITAL_OUTPUTS] & (1<<i);
   }
   for (i=0; i<8; i++)
   {
      valinta_outputs[_24V_OUTPUTS_VALINTA_IDX+i] = output_registers[OUTPUT_REG_CARD1_DIGITAL_OUTPUTS] & (1<<(_24V_OUTPUTS_VALINTA_IDX+i));
      valinta_outputs[CARD2_24V_OUTPUTS_VALINTA_IDX+i] = output_registers[OUTPUT_REG_CARD2_DIGITAL_OUTPUTS] & (1<<(_24V_OUTPUTS_VALINTA_IDX+i));
   }

   valinta_outputs[HEAT_ON_VALINTA_IDX] = run_target_params.cook_mode;
   valinta_outputs[CHILL_ON_VALINTA_IDX] = run_target_params.chill_mode;
   valinta_outputs[EMPTY_ON_VALINTA_IDX] = run_target_params.emptying_mode;
   valinta_outputs[WATER_ON_VALINTA_IDX] = run_target_params.water_fill_mode;
   valinta_outputs[TILT_UP_VALINTA_IDX] = (run_target_params.tilt_mode == 1) ? 1 : 0; //1=UP, 2=Down;
   valinta_outputs[TILT_DN_VALINTA_IDX] = (run_target_params.tilt_mode == 2) ? 1 : 0;
   valinta_outputs[MIX_FWD_VALINTA_IDX] = (mixer_motor_vaihe == MIXER_MOTOR_RUN_ETEEN) ? 1 : 0;
   valinta_outputs[MIX_REW_VALINTA_IDX] = (mixer_motor_vaihe == MIXER_MOTOR_RUN_TAAKSE) ? 1 : 0;
}

void show_iotesti_valinnat(void)
{
   text_with_kehys(valinta_outputs[HEAT_ON_VALINTA_IDX] ? (char *)"COOK=1" : (char *)"COOK=0", HEAT_ON_BOX_X, HEAT_ON_BOX_Y, HEAT_ON_VALINTA_IDX);
   text_with_kehys(valinta_outputs[CHILL_ON_VALINTA_IDX] ? (char *)"CHILL=1" : (char *)"CHILL=0", CHILL_ON_BOX_X, CHILL_ON_BOX_Y, CHILL_ON_VALINTA_IDX);
   text_with_kehys(valinta_outputs[EMPTY_ON_VALINTA_IDX] ? (char *)"EMPTY=1" : (char *)"EMPTY=0", EMPTY_ON_BOX_X, EMPTY_ON_BOX_Y, EMPTY_ON_VALINTA_IDX);

   text_with_kehys(valinta_outputs[MIX_FWD_VALINTA_IDX] ? (char *)"M FWD=1" : (char *)"M FWD=0", MIX_FWD_BOX_X, MIX_FWD_BOX_Y, MIX_FWD_VALINTA_IDX);
   text_with_kehys(valinta_outputs[MIX_REW_VALINTA_IDX] ? (char *)"M REW=1" : (char *)"M REW=0", MIX_REW_BOX_X, MIX_REW_BOX_Y, MIX_REW_VALINTA_IDX);

   text_with_kehys(valinta_outputs[TILT_UP_VALINTA_IDX] ? (char *)"TILT UP=1" : (char *)"TILT UP=0", TILT_UP_BOX_X, TILT_UP_BOX_Y, TILT_UP_VALINTA_IDX);
   text_with_kehys(valinta_outputs[TILT_DN_VALINTA_IDX] ? (char *)"TILT DN=1" : (char *)"TILT DN=0", TILT_DN_BOX_X, TILT_DN_BOX_Y, TILT_DN_VALINTA_IDX);

   char buf[30];
   if (valinta_outputs[WATER_ON_VALINTA_IDX])
      sprintf(buf, "WATER=%d",run_target_params.water_fill_amount_litres-live_params.water_fill_amount_litres);
   else
      strcpy(buf, "WATER=0");
   text_with_kehys(buf, WATER_ON_BOX_X, WATER_ON_BOX_Y, WATER_ON_VALINTA_IDX);
   int i = input_registers[INPUT_REG_CARD2_VESI_PULSSIT];
   sprintf(buf, "Pulses:%d", i);
   show_ttf(gRenderer, buf, MY_COLOR_BLACK, WATER_ON_BOX_X + 200, WATER_ON_BOX_Y, MY_FONT_KESKI);
}

int iotest_function_on(void)
{
   if (valinta_outputs[HEAT_ON_VALINTA_IDX] || valinta_outputs[CHILL_ON_VALINTA_IDX] || valinta_outputs[WATER_ON_VALINTA_IDX])
      return 1;
   else
      return 0;
}