/* 
   kotinakyma.cpp
   */
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "SDL2_gfxPrimitives.h"

#include <stdio.h>
#include <string>
#include <stdlib.h>

#include <sys/time.h>
#include <sys/times.h>

#include "menu_ids.h"
#include "handler.h"

#include "button_defs.h"
#include "kypsennys.h"
#include "tekstit.h"
#include "var.h"
#include "image.h"
#include "button_group.h"
#include "externs.h"
#include "cook_prog.h"

extern int sec_ctr;
int button_pressed, wait_finger_up;
int prog_curr_hours, prog_curr_minutes;

int handle_kotinakyma_button_press(int but);

#define NUM_KOTINAKYMA_ICONS 6
Image * KOTINAKYMA_icons[NUM_KOTINAKYMA_ICONS];

#define KYPSENNYS_BUTTON_IDX 0
#define JAAHDYTYS_BUTTON_IDX 1
#define SUOSIKIT_BUTTON_IDX  2
#define PESU_BUTTON_IDX      3
#define OHJELMAT_BUTTON_IDX  4
#define LISATOIMINNOT_BUTTON_IDX  5
#define NUM_BUTTON_IDX  6

typedef struct
{
   int x;
   int y;
   int Width;
   int Height;
   int button_idx;
   int txt_idx;
   int txt_group;
} my_button_grp_s;

const my_button_grp_s kotinakyma_Button_group[NUM_BUTTON_IDX] = {
   {MENU_BUTTON1_L, MENU_BUTTON1_T, MENU_BUTTON_W_H, MENU_BUTTON_W_H, KYPSENNYS_BUTTON_IDX, YLEIS_TXT_KYPSENNYS, TXT_GROUP_YLEIS},
   {MENU_BUTTON2_L, MENU_BUTTON2_T, MENU_BUTTON_W_H, MENU_BUTTON_W_H, JAAHDYTYS_BUTTON_IDX, YLEIS_TXT_JAAHDYTYS, TXT_GROUP_YLEIS},
   {MENU_BUTTON3_L, MENU_BUTTON3_T, MENU_BUTTON_W_H, MENU_BUTTON_W_H, SUOSIKIT_BUTTON_IDX, YLEIS_TXT_SUOSIKIT, TXT_GROUP_YLEIS},
   {MENU_BUTTON4_L, MENU_BUTTON4_T, MENU_BUTTON_W_H, MENU_BUTTON_W_H, PESU_BUTTON_IDX, YLEIS_TXT_PESU, TXT_GROUP_YLEIS},
   {MENU_BUTTON5_L, MENU_BUTTON5_T, MENU_BUTTON_W_H, MENU_BUTTON_W_H, OHJELMAT_BUTTON_IDX, YLEIS_TXT_OHJELMAT, TXT_GROUP_YLEIS},
   {MENU_BUTTON6_L, MENU_BUTTON6_T, MENU_BUTTON_W_H, MENU_BUTTON_W_H, LISATOIMINNOT_BUTTON_IDX, YLEIS_TXT_LISATOIMINNOT, TXT_GROUP_YLEIS},
};

void set_kotinakyma_buttons(void)
{
   SDL_Color txt_color = { 0xff, 0xff, 0xff, 0xff };

   for (int i=0; i< NUM_BUTTON_IDX; i++)
   {
      Button_group_add_button(i, kotinakyma_Button_group[i].x, kotinakyma_Button_group[i].y, kotinakyma_Button_group[i].Width, kotinakyma_Button_group[i].Height, 80);
                              //kotinakyma_Button_group[i].y + (254-70));
      Button_group_setText(i, kotinakyma_Button_group[i].txt_idx, kotinakyma_Button_group[i].txt_group, txt_color, font_norm_bold_20px);
      Button_group_add_images(i, image_ptrs[IMAGE_MENU_BUTTON_TAUSTA], KOTINAKYMA_icons[i]);
      Button_groups[i].icon_image_pos.x = 10;
   }
}

#define HARMAA_TAUSTA_Y2 998
#define HARMAA_TAUSTA2_Y2 1225

#define INFO_ALUE_Y2  (610+368-1) //978 = alakahva y-50
#define INFO_ALUE2_Y2 (1205-1) //alempi kahva y-50
//alakahva: rectangleRGBA(gRenderer, x=375, y=1028, 375+50, 1028+4, 0xB4, 0xB4, 0xB4, 0xFF);
//alempi:   rectangleRGBA(gRenderer, x=375, y=1255, 375+50, 1255+4, 0xB4, 0xB4, 0xB4, 0xFF);

void draw_KOTINAKYMA(int param1)
{
   if (alavalikko_naytolla)
   {
      boxRGBA(gRenderer, 0, 70, 799, HARMAA_TAUSTA_Y2, 0x58, 0x5B, 0x60, 0xFF);
      show_ala_kahva();
   }
   else
   {
      boxRGBA(gRenderer, 0, 70, 799, HARMAA_TAUSTA2_Y2, 0x58, 0x5B, 0x60, 0xFF);
      show_alempi_kahva();
   }

   for (int i=0; i< NUM_BUTTON_IDX; i++)
   {
      Button_groups[i].background_image = (button_pressed==i+1) ? image_ptrs[IMAGE_MENU_BUTTON_POHJASSA_TAUSTA] : image_ptrs[IMAGE_MENU_BUTTON_TAUSTA];
      Button_group_render(i);
      Button_groups[i].button_disabled = ylavalikko_naytolla;
   }

   //Info alue
   roundedBoxRGBA(gRenderer,
      20, 610, 20+760-1, alavalikko_naytolla ? INFO_ALUE_Y2 : INFO_ALUE2_Y2,
         41,
         0xFF, 0xFF, 0xFF, 0xFF);

   if (check_any_errors())
   {
      SDL_Color txt_color = { 0xf0, 0x00, 0x00, 0xff };
      int err_y = 620;
      for (int i=0; i< NUM_ERROR_INDEX; i++)
      {
         if (errors_paalla[i])
         {
            render_text((char *)error_tekstit[i], 30, err_y,  &txt_color, font_norm_light_40px);
            if (i==ERROR_IDX_CAN_COMM_ERROR)
            {
               char buff[10];
               sprintf(buff, "%d", errors_paalla[ERROR_IDX_CAN_COMM_ERROR]);
               render_text((char *)buff, 600, err_y,  &txt_color, font_norm_light_40px);
            }
            err_y += 45;
         }
      }
   }
   else if (Prog_update_going == 2) //Päivitys tehty tai oli jos uusin versio
   {
      SDL_Color txt_color = { 0x0, 0xf0, 0x00, 0xff };
      //Prog_update_going = 0;
      int err_y = 620;
      render_text((char *)Prog_update_buffer1, 30, err_y,  &txt_color, font_norm_light_40px);
      err_y += 45;
      render_text((char *)Prog_update_buffer2, 30, err_y,  &txt_color, font_norm_light_40px);
   }

   if (param1 == 0)
   {
      show_banner(NUM_BUTTON_IDX);
      if (alavalikko_naytolla)
         nayta_alavalikko(NUM_BUTTON_IDX+1, button_pressed);
      else if (ylavalikko_naytolla)
         nayta_ylavalikko(NUM_BUTTON_IDX+1);
   }
}

int handler_KOTINAKYMA(int x, int y,  SDL_Event *e)
{
   if( e->type == SDL_MOUSEMOTION)
   {
      if (!wait_finger_up && check_for_alavalikko(x, y, NUM_BUTTON_IDX+1))
      {
         wait_finger_up = 1;
         draw_KOTINAKYMA(0);
      }
      else
      {
         draw_KOTINAKYMA(0); //TÄssä on yritetty välttää näytön piirtämistä joka pyyhkäisyllä mutta piirretään nyt kuitenkin, muuten välähtää aina valkoiseksi jos ei päivitetä..
      }
      ala_painike_painettu = button_pressed = 0;
   }
   else
   {
      if(e->type == SDL_MOUSEBUTTONDOWN)
      {
         if ((x==-1) && (y==-1))
            set_kotinakyma_buttons(); //Tuli kansi auki: päivitetään kaikki"
         else
            button_pressed = Button_group_check_for_key_press(x, y);
      }
      else if(e->type == SDL_MOUSEBUTTONUP)
      {
         if (button_pressed)
         {
            int menu;
            menu = check_ylavalikko_valinta(button_pressed);
            if (menu && (menu != 0xFFFF))
               return menu;
            menu = handle_kotinakyma_button_press(--button_pressed);
            if (menu != 0xFFFF)
               return menu;
            button_pressed = 0;
         }
//         printf("Finger up\n"); //finger up tulee motion eventin jälkeen, ylä/alavalikkoa ei näytetä toisensa perään ennenkuin on saatu finger-up
         wait_finger_up = 0;
      }
      draw_KOTINAKYMA(0);
   }
   return 0xffff;
}

void init_KOTINAKYMA(void)
{
}

void show_KOTINAKYMA(int show_hide)
{
   sec_ctr = 0;
   multi_step_prog_going = 0;
   if (show_hide == 0)
   {
      if (Prog_update_going == 2)
         Prog_update_going = 0;
      return;
   }
   if (prev_menu_id == ID_LAUNCH) //Tämä vähän onkelmallinen..koska näyttö maalataan mustaksi, MX500 kuva ei muuten toiminut..tarttee joskus tsekata paremmin.
   {
      //Valkoinen tausta
//      SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
//      SDL_RenderClear( gRenderer );
      //Update screen
//      SDL_RenderPresent( gRenderer );
      printf("---> valkoinen tausta...\n");
   }
   button_pressed = 0;
   pyyhk_enabled = 1;
   Button_group_empty();
//   update_kotinakyma_const_buttons_text();

   KOTINAKYMA_icons[0] = image_ptrs[IMAGE_KYPSENNYS_111px];
   KOTINAKYMA_icons[1] = image_ptrs[IMAGE_JAAHDYTYS_111px];
   KOTINAKYMA_icons[2] = image_ptrs[IMAGE_SUOSIKIT_111px];
   KOTINAKYMA_icons[3] = image_ptrs[IMAGE_PUHDISTUS_111px];
   KOTINAKYMA_icons[4] = image_ptrs[IMAGE_OHJELMAT_111px],
   KOTINAKYMA_icons[5] = image_ptrs[IMAGE_Ekstrat_111px];
   set_kotinakyma_buttons(); //Tämä vasta KOTINAKYMA_icons-asettelujen jälkeen!
   draw_KOTINAKYMA(0);
}

int handle_kotinakyma_button_press(int but)
{
   switch (but)
   {
      case KYPSENNYS_BUTTON_IDX:
         return ID_KYPSENNYS;
      break;

      case JAAHDYTYS_BUTTON_IDX:
         return ID_JAAHDYTYS;
      break;

      case SUOSIKIT_BUTTON_IDX:
      break;

      case PESU_BUTTON_IDX:
      break;

      case OHJELMAT_BUTTON_IDX:
         menu_to_return = current_menu_id;
         return ID_OHJELMAT;
      break;

      case LISATOIMINNOT_BUTTON_IDX:
         clear_all_errors();
         menu_to_return = current_menu_id;
         return ID_IOTEST;
      break;
   }
   return 0xFFFF;
}

void show_aika(void)
{
   struct tm curr_time;
   char timebuff[20];
   SDL_Color txt_color = { 0x00, 0x00, 0x00, 0xff };

   time_t mynow;
	time(&mynow);

	curr_time = *localtime(&mynow);
   if (curr_time.tm_hour < 12)
	   sprintf(timebuff, "%02d:%02d am", curr_time.tm_hour, curr_time.tm_min);	//curr_time.tm_year+1900
   else
	   sprintf(timebuff, "%02d:%02d pm", curr_time.tm_hour-12, curr_time.tm_min);	//curr_time.tm_year+1900
   render_text(timebuff, 628, 12, &txt_color, font_norm_light_40px);
   prog_curr_hours = curr_time.tm_hour;
   prog_curr_minutes = curr_time.tm_min;
}


void show_banner(int but_idx)
{
   int i = but_idx;
   Button_group_add_button(i, 13, 16, 126, 41); //Logo x=13, y=16, 126x41
   Button_group_add_images(i, image_ptrs[IMAGE_DIETA_LOGO], 0);
   Button_group_add_event_code(i, CODE_DIETA_LOGO); //CODE_DIETA_LOGO etc
   Button_group_render(i);
   show_aika();
}

void show_ala_kahva(void)
{
   rectangleRGBA(gRenderer, 375, 1028, 375+50, 1028+4, 0xB4, 0xB4, 0xB4, 0xFF);
   rectangleRGBA(gRenderer, 375, 1039, 375+50, 1039+4, 0xB4, 0xB4, 0xB4, 0xFF);
}

void show_alempi_kahva(void)
{
   rectangleRGBA(gRenderer, 375, 1255, 375+50, 1255+4, 0xB4, 0xB4, 0xB4, 0xFF);
   rectangleRGBA(gRenderer, 375, 1266, 375+50, 1266+4, 0xB4, 0xB4, 0xB4, 0xFF);
}

int check_for_alavalikko(int x, int y, int but_idx) //Jos oli pyyhk.toiminto, niin alavalikko esiin/piiloon
{
   if (x==0)
   {
      if (y==10000) 
      {
         if (ylavalikko_naytolla)
            piilota_ylavalikko(but_idx);
         else
            nayta_alavalikko(but_idx, 0); //printf("UP-PYYHK x=%d ... y=%d\n", x, y);
         return 1;
      }
      else if (y==-10000)
      {
         if (alavalikko_naytolla)
            piilota_alavalikko(but_idx); //printf("DOWN-PYYHK x=%d ... y=%d\n", x, y);
         else
            nayta_ylavalikko(but_idx);
         return 1;
      }
   }
   return 0;
}

int check_ylavalikko_valinta(int code)
{
   if ((code >= CODE_YLAPALKKI_HACCP) && (code <= CODE_YLAPALKKI_BT))
   {
      printf("Ylävälikkotoiminta %d!\n", code);
      if (code == CODE_YLAPALKKI_HACCP)
      {
         //menu_to_return = current_menu_id;
         //ok_cancel_ok_func_idx = 0;
         //return ID_OK_CANCEL;
      }
      else if (code == CODE_YLAPALKKI_MUNAK)
      {
         //menu_to_return = current_menu_id;
         //ok_cancel_ok_func_idx = 0;
         //return ID_OK_CANCEL;
      }
      else if (code == CODE_YLAPALKKI_PROFIILI)
      {
         menu_to_return = current_menu_id;
         return ID_PROF_VAIHTO;
      }
      else if (code == CODE_YLAPALKKI_SAMMUTA)
      {
         menu_to_return = current_menu_id;
         set_ok_cancel_text(TXT_GROUP_YLEIS, YLEIS_TXT_HALUATKO_SAMMUTTAA_LAITTEEN);
         ok_cancel_ok_func_idx = OK_CANCEL_OK_POWEROFF;
         return ID_OK_CANCEL;
      }

   }
   return 0;
}
