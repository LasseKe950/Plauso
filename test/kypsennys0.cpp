/* 
   alku2.c 24. helmikuu 2012:
   Uusi aloitusruutu jossa pikakäyttö/helppo/suosikit/ohjelmat valinnat

   roundedBoxRGBA / roundedBoxColor tekee isomman pyöristetyn suorakulmion kuin roundedRectangleRGBA..?
   Jälkimmäinen siis joka ei täytä aluetta.

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
#include "button.h"

#include "img_header.h"

#include "button_defs.h"
#include "kypsennys.h"
#include "tekstit.h"
#include "var.h"

#define NUM_KYPS_PALIKAT 1
LButton * kyps_palikat[NUM_KYPS_PALIKAT];

void nayta_aloita_painike(void)
{
   //last par: color The color value of the rectangle to draw (0xRRGGBBAA).
/*
   Uint32 c = 0xFF<<24 | (aloita_rounded_rect.color.b<<16) | (aloita_rounded_rect.color.g<<8) | (aloita_rounded_rect.color.r);
   roundedBoxColor(gRenderer,
                   aloita_rounded_rect.rect.x, aloita_rounded_rect.rect.y,
                   aloita_rounded_rect.rect.x + aloita_rounded_rect.rect.w, aloita_rounded_rect.rect.y + aloita_rounded_rect.rect.h,
                   aloita_rounded_rect.radius,
                   c);
*/
   roundedBoxRGBA(gRenderer,
         aloita_rounded_rect.rect.x + aloita_rounded_rect.rect.w, aloita_rounded_rect.rect.y, //Top Right
         aloita_rounded_rect.rect.x, aloita_rounded_rect.rect.y + aloita_rounded_rect.rect.h, //Bottom Left
         /*aloita_rounded_rect.radius,*/ 45,
         aloita_rounded_rect.color.r, aloita_rounded_rect.color.g, aloita_rounded_rect.color.b, 0xFF);

   SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_NONE);

   SDL_Color txt_color = { 0xf4, 0xf4, 0xf4, 0xff };
   helperbutton_NoImage->set_size(aloita_rounded_rect.rect.w, aloita_rounded_rect.rect.h);
   helperbutton_NoImage->setText((char*)yleis_tekstit[YLEIS_TXT_ALOITA], txt_color, font_norm_regular_50px);
   helperbutton_NoImage->render(aloita_rounded_rect.rect.x, aloita_rounded_rect.rect.y);

}

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
#define MIX_MIINUS_IDX 10
#define MIX_PLUS_IDX   11
#define NUM_BUTTON_IDX 12

void nayta_cooking_values(void)
{
   if (helperbutton_NoImage)
   {
      char buffer[10];
      //SDL_Point pos;
      //pos.x = TEMP_LUKEMA_L;
      //pos.y = TEMP_LUKEMA_T;
      //helperbutton_NoImage->set_position(pos);
      SDL_Color txt_color = { 0xff, 0xff, 0xff, 0xff };
      helperbutton_NoImage->set_position_and_size(TEMP_LUKEMA_L, TEMP_LUKEMA_T, TEMP_LUKEMA_W, TEMP_LUKEMA_H, TEMP_LUKEMA_IDX);
      sprintf(buffer, "%d""\xB0""C", run_target_params.food_temp);
      helperbutton_NoImage->setText(buffer, txt_color, font_norm_bold_80px);
      helperbutton_NoImage->render(TEMP_LUKEMA_L, TEMP_LUKEMA_T);
      //printf("TEMP LUKEMA x=%d y=%d\n", pos.x, pos.y);
      helperbutton_NoImage->set_position_and_size(AIKA_LUKEMA_L, AIKA_LUKEMA_T, AIKA_LUKEMA_W, AIKA_LUKEMA_H, AIKA_LUKEMA_IDX);
      sprintf(buffer, "%02d:%02d", run_target_params.cook_time_min/60, run_target_params.cook_time_min%60);
      helperbutton_NoImage->setText(buffer, txt_color, font_norm_bold_80px);
      helperbutton_NoImage->render(AIKA_LUKEMA_L, AIKA_LUKEMA_T);

      helperbutton_NoImage->set_position_and_size(VESI_LUKEMA_L, VESI_LUKEMA_T, VESI_LUKEMA_W, VESI_LUKEMA_H, VESI_LUKEMA_IDX);
      sprintf(buffer, "%d l", run_target_params.water_amount);
      helperbutton_NoImage->setText(buffer, txt_color, font_norm_bold_80px);
      helperbutton_NoImage->render(VESI_LUKEMA_L, VESI_LUKEMA_T);

      helperbutton_NoImage->set_position_and_size(MIX_LUKEMA_L, MIX_LUKEMA_T, MIX_LUKEMA_W, MIX_LUKEMA_H, MIX_LUKEMA_IDX);
      helperbutton_NoImage->setText("0", txt_color, font_norm_bold_80px);
      helperbutton_NoImage->render(MIX_LUKEMA_L, MIX_LUKEMA_T);

      helperbutton_NoImage->set_position_and_size(TEMP_MIINUS_L, TEMP_MIINUS_T, TEMP_MIINUS_W, TEMP_MIINUS_H, TEMP_MIINUS_IDX);
      helperbutton_NoImage->setText("-", txt_color, font_norm_bold_80px);
      helperbutton_NoImage->render(TEMP_MIINUS_L, TEMP_MIINUS_T);
      helperbutton_NoImage->set_position_and_size(TEMP_PLUS_L, TEMP_PLUS_T, TEMP_PLUS_W, TEMP_PLUS_H, TEMP_PLUS_IDX);
      helperbutton_NoImage->setText("+", txt_color, font_norm_bold_80px);
      helperbutton_NoImage->render(TEMP_PLUS_L, TEMP_PLUS_T);

      helperbutton_NoImage->set_position_and_size(AIKA_MIINUS_L, AIKA_MIINUS_T, AIKA_MIINUS_W, AIKA_MIINUS_H, AIKA_MIINUS_IDX);
      helperbutton_NoImage->setText("-", txt_color, font_norm_bold_80px);
      helperbutton_NoImage->render(AIKA_MIINUS_L, AIKA_MIINUS_T);
      helperbutton_NoImage->set_position_and_size(AIKA_PLUS_L, AIKA_PLUS_T, AIKA_PLUS_W, AIKA_PLUS_H, AIKA_PLUS_IDX);
      helperbutton_NoImage->setText("+", txt_color, font_norm_bold_80px);
      helperbutton_NoImage->render(AIKA_PLUS_L, AIKA_PLUS_T);

      helperbutton_NoImage->set_position_and_size(VESI_MIINUS_L, VESI_MIINUS_T, VESI_MIINUS_W, VESI_MIINUS_H, VESI_MIINUS_IDX);
      helperbutton_NoImage->setText("-", txt_color, font_norm_bold_80px);
      helperbutton_NoImage->render(VESI_MIINUS_L, VESI_MIINUS_T);
      helperbutton_NoImage->set_position_and_size(VESI_PLUS_L, VESI_PLUS_T, VESI_PLUS_W, VESI_PLUS_H, VESI_PLUS_IDX);
      helperbutton_NoImage->setText("+", txt_color, font_norm_bold_80px);
      helperbutton_NoImage->render(VESI_PLUS_L, VESI_PLUS_T);

      helperbutton_NoImage->set_position_and_size(MIX_MIINUS_L, MIX_MIINUS_T, MIX_MIINUS_W, MIX_MIINUS_H, MIX_MIINUS_IDX);
      helperbutton_NoImage->setText("-", txt_color, font_norm_bold_80px);
      helperbutton_NoImage->render(MIX_MIINUS_L, MIX_MIINUS_T);
      helperbutton_NoImage->set_position_and_size(MIX_PLUS_L, MIX_PLUS_T, MIX_PLUS_W, MIX_PLUS_H, MIX_PLUS_IDX);
      helperbutton_NoImage->setText("+", txt_color, font_norm_bold_80px);
      helperbutton_NoImage->render(MIX_PLUS_L, MIX_PLUS_T);

   }
}

static void update_this(void)
{
   for (int i=0; i< NUM_KYPS_PALIKAT; i++)
   {
      if (kyps_palikat[i] != nullptr)
         kyps_palikat[i]->render(kyps_palikat[i]->image_s_ptr->left, kyps_palikat[i]->image_s_ptr->top);
   }

   //TEMPER.SÄÄTÖ
   SDL_SetRenderDrawColor(gRenderer, Rect1.color.r, Rect1.color.g, Rect1.color.b, Rect1.color.a);
   SDL_RenderFillRect(gRenderer, &(Rect1.rect));

   roundedBoxRGBA(gRenderer,
         temp_saato_rounded_rect.rect.x - 4, temp_saato_rounded_rect.rect.y - 4,
         temp_saato_rounded_rect.rect.x + temp_saato_rounded_rect.rect.w, temp_saato_rounded_rect.rect.y + temp_saato_rounded_rect.rect.h,
         60, //temp_saato_rounded_rect.radius,
         0xFF, 0xFF, 0xFF, 0x29);
//   SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_NONE);

   roundedBoxRGBA(gRenderer,
         temp_saato_rounded_rect.rect.x + 4, temp_saato_rounded_rect.rect.y + 4,
         temp_saato_rounded_rect.rect.x + temp_saato_rounded_rect.rect.w, temp_saato_rounded_rect.rect.y + temp_saato_rounded_rect.rect.h,
         60,//temp_saato_rounded_rect.radius,
         0x00, 0x00, 0x00, 0xFF);
//   SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_NONE);

   //last par: color The color value of the rectangle to draw (0xRRGGBBAA).
   Uint32 c = 0xFF<<24 | (temp_saato_rounded_rect.color.b<<16) | (temp_saato_rounded_rect.color.g<<8) | (temp_saato_rounded_rect.color.r);
   roundedBoxColor(gRenderer,
                   temp_saato_rounded_rect.rect.x, temp_saato_rounded_rect.rect.y,
                   temp_saato_rounded_rect.rect.x + temp_saato_rounded_rect.rect.w, temp_saato_rounded_rect.rect.y + temp_saato_rounded_rect.rect.h,
                   60, //temp_saato_rounded_rect.radius,
                   c);
   SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_NONE);

   //AIKA SÄÄTÖ
   SDL_SetRenderDrawColor(gRenderer, Rect2.color.r, Rect2.color.g, Rect2.color.b, Rect2.color.a);
   SDL_RenderFillRect(gRenderer, &(Rect2.rect));

   roundedRectangleRGBA(gRenderer,
         temp_saato_rounded_rect.rect.x, AIKA_SAATO_BOX_Y,
         temp_saato_rounded_rect.rect.x + temp_saato_rounded_rect.rect.w, AIKA_SAATO_BOX_Y + temp_saato_rounded_rect.rect.h,
         temp_saato_rounded_rect.radius,
         0xFF, 0xFF, 0xFF, 66);
   SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_NONE);

   //VESI SÄÄTÖ
   SDL_SetRenderDrawColor(gRenderer, Rect3.color.r, Rect3.color.g, Rect3.color.b, Rect3.color.a);
   SDL_RenderFillRect(gRenderer, &(Rect3.rect));

   roundedRectangleRGBA(gRenderer,
         temp_saato_rounded_rect.rect.x, VESI_SAATO_BOX_Y,
         temp_saato_rounded_rect.rect.x + temp_saato_rounded_rect.rect.w, VESI_SAATO_BOX_Y + temp_saato_rounded_rect.rect.h,
         temp_saato_rounded_rect.radius,
         0xFF, 0xFF, 0xFF, 66);
   SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_NONE);

   //MIXER SÄÄTÖ
   SDL_SetRenderDrawColor(gRenderer, Rect4.color.r, Rect4.color.g, Rect4.color.b, Rect4.color.a);
   SDL_RenderFillRect(gRenderer, &(Rect4.rect));

   roundedRectangleRGBA(gRenderer,
         temp_saato_rounded_rect.rect.x, MIXER_SAATO_BOX_Y,
         temp_saato_rounded_rect.rect.x + temp_saato_rounded_rect.rect.w, MIXER_SAATO_BOX_Y + temp_saato_rounded_rect.rect.h,
         temp_saato_rounded_rect.radius,
         0xFF, 0xFF, 0xFF, 66);
   SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_NONE);

#if 0
   //last par: color The color value of the rectangle to draw (0xRRGGBBAA).
   Uint32 c = 0xFF<<24 | (temp_saato_rounded_rect.color.b<<16) | (temp_saato_rounded_rect.color.g<<8) | (temp_saato_rounded_rect.color.r);
   roundedBoxColor(gRenderer,
                   temp_saato_rounded_rect.rect.x, temp_saato_rounded_rect.rect.y,
                   temp_saato_rounded_rect.rect.x + temp_saato_rounded_rect.rect.w, temp_saato_rounded_rect.rect.y + temp_saato_rounded_rect.rect.h,
                   temp_saato_rounded_rect.radius,
                   c);
#endif
#if 0
   roundedRectangleColor (gRenderer,
                   temp_saato_rounded_rect.rect.x, temp_saato_rounded_rect.rect.y,
                   temp_saato_rounded_rect.rect.x +  + temp_saato_rounded_rect.rect.w + 3, temp_saato_rounded_rect.rect.y +  + temp_saato_rounded_rect.rect.y + 3,
                   temp_saato_rounded_rect.radius,
                   0xFFF0F0F0);
#endif

   nayta_aloita_painike();
   nayta_cooking_values();
}

void enter_KYPSENNYS_pages(void)
{
   kyps_palikat[0] = button_DietaLogoB;
   update_this();
}

int handler_KYPSENNYS(int x, int y,  SDL_Event *e)
{
//   printf("in handler LAUNCH\n");
#ifdef PC_UBUNTU
   if( e->type == SDL_MOUSEMOTION)
#else
   if( e->type == SDL_FINGERMOTION)
#endif
   {
   }
   else
   {
#ifdef PC_UBUNTU
      if(e->type == SDL_MOUSEBUTTONDOWN)
#else
      if(e->type == SDL_FINGERDOWN)
#endif
      {
         printf("x=%d, y=%d\n", x, y);
      }
   }

   update_this();
   return 0xffff;
}

void init_KYPSENNYS(void)
{
}

void show_KYPSENNYS(int show_hide)
{
   if (show_hide == 0)
   {
      return;
   }
   load_run_parameters(); //lataa defaultit -> ajo-parametreiksi
   enter_KYPSENNYS_pages();
}
