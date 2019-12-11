#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <stdio.h>
#include <string>

#include <stdlib.h>

#include "handler.h"
#include "button_group.h"
#include "image.h"
#include "tekstit.h"

//Global place to put buttongroop texts
//char button_text_buffers[MAX_LEN_BUTTONGROUP_TEXT][NUM_BUTTONGROUP_TEXT];

static int num_buttons;

button_group_s Button_groups[NUM_BUTTONGROUP_BUTTONS];

void Button_group_free()
{
	//Free texture if it exists
   for (int i=0; i < NUM_BUTTONGROUP_BUTTONS; i++)
   {
   	if( Button_groups[i].Text_mTexture != NULL )
	   {
		   SDL_DestroyTexture( Button_groups[i].Text_mTexture );
		   Button_groups[i].Text_mTexture = NULL;
      }
	}
}

void Button_group_empty()
{
   num_buttons = 0;
   memset(Button_groups, 0, sizeof(Button_groups));
}

void Button_group_setfont(int i, TTF_Font * f)
{
   Button_groups[i].font = f;
}

void Button_group_render(int idx)
{
   if (!Button_groups[idx].button_set)
      return;
   if (Button_groups[idx].background_image)
   {
      if (Button_groups[idx].bg_image_pos.x)
         Button_groups[idx].background_image->render(Button_groups[idx].bg_image_pos.x, Button_groups[idx].bg_image_pos.y);
      else
         Button_groups[idx].background_image->render(Button_groups[idx].x, Button_groups[idx].y);
   }
   if (Button_groups[idx].icon_image)
   { //keskitetään ikoni 
	   SDL_Rect dst;
      int iW, iH;
      iW = Button_groups[idx].icon_image->Width;
      iH = Button_groups[idx].icon_image->Height;
      dst.x = Button_groups[idx].x + (Button_groups[idx].Width/2 - iW/2);
      dst.x += Button_groups[idx].icon_image_pos.x;//VOidaan hienosäätää iconin paikkaa tarpeen mukaan
      dst.y = Button_groups[idx].y + (Button_groups[idx].Height/2 - iH/2);
      dst.y += Button_groups[idx].icon_image_pos.y;//VOidaan hienosäätää iconin paikkaa tarpeen mukaan
      Button_groups[idx].icon_image->render(dst.x, dst.y);
   }
   if (Button_groups[idx].Text_mTexture)
   {
	   SDL_Rect dst;
      int iW, iH;
      SDL_QueryTexture(Button_groups[idx].Text_mTexture, NULL, NULL, &iW, &iH);
      if (Button_groups[idx].Width)
         dst.x = Button_groups[idx].x + (Button_groups[idx].Width/2 - iW/2);
      else
         dst.x = Button_groups[idx].x;
//      if (Button_groups[idx].button_txt_y_offset)
//         dst.y = Button_groups[idx].button_txt_y_offset;
//      else
      {
         if (Button_groups[idx].Height)
            dst.y = Button_groups[idx].y + (Button_groups[idx].Height/2 - iH/2);
         else
            dst.y = Button_groups[idx].y;
      }
      dst.y += Button_groups[idx].button_txt_y_offset;
      dst.x += Button_groups[idx].button_txt_x_offset;
      dst.w = iW;
      dst.h = iH;
      SDL_RenderCopy( gRenderer, Button_groups[idx].Text_mTexture, NULL, &dst);
   }
}

void Button_group_add_images(int button_idx, Image * bg, Image * icon)
{
//   if (bg != nullptr)
      Button_groups[button_idx].background_image = bg;
//   if (icon != nullptr)
      Button_groups[button_idx].icon_image = icon;
}

void Button_group_add_images(int button_idx, Image * bg, Image * icon, int x, int y)
{
   SDL_Point pos;
   if (x==-1)
      pos.x = Button_groups[button_idx].x;
   else
      pos.x = x;
   if (y==-1)
      pos.y = Button_groups[button_idx].y;
   else
      pos.y = y;
//   if (bg != nullptr)
   {
      Button_groups[button_idx].background_image = bg;
      Button_groups[button_idx].bg_image_pos = pos;
   }
//   if (icon != nullptr)
   {
      Button_groups[button_idx].icon_image = icon;
   }
}

SDL_Texture* Button_group_setText(int idx, int txt_idx, int txt_group, SDL_Color txt_color, TTF_Font *font)
{
   if ((txt_group == TXT_GROUP_YLEIS) && (txt_idx < NUM_YLEIS_TEKSTIT))
      strncpy(Button_groups[idx].button_text_buffer, (char*)yleis_tekstit[txt_idx], MAX_LEN_BUTTONGROUP_TEXT);
   else if ((txt_group == TXT_GROUP_SEKOITUSTAPA) && (txt_idx < NUM_SEKOITUSTAPA_TEKSTIT))
      strncpy(Button_groups[idx].button_text_buffer, (char*)sekoitustapa_tekstit[txt_idx], MAX_LEN_BUTTONGROUP_TEXT);
   else if ((txt_group == TXT_GROUP_KANSIO_NIMET) && (txt_idx < NUM_KANSIO_NIMET_TEKSTIT))
      strncpy(Button_groups[idx].button_text_buffer, (char*)kansio_nimet_tekstit[txt_idx], MAX_LEN_BUTTONGROUP_TEXT);
   else
      return nullptr;

   if (font == nullptr || !strlen(Button_groups[idx].button_text_buffer))
      return nullptr;

   if (Button_groups[idx].Text_mTexture)
   {
      SDL_DestroyTexture(Button_groups[idx].Text_mTexture);
      Button_groups[idx].Text_mTexture = nullptr;
   }
	//We need to first render to a surface as that's what TTF_RenderText
	//returns, then load that surface into a texture
//   printf("TTF_RenderText_Blended...\n");
   SDL_Surface *surf = TTF_RenderText_Blended(font, /*message.c_str()*/ Button_groups[idx].button_text_buffer, txt_color);
   if (surf == nullptr)
   {
      printf("ERROR TTF_RenderText_Blended\n");
      //logSDLError(std::cout, "TTF_RenderText");
      return nullptr;
   }
//   printf("SDL_CreateTextureFromSurface...\n");
   Button_groups[idx].Text_mTexture = SDL_CreateTextureFromSurface(gRenderer, surf);
   if (Button_groups[idx].Text_mTexture == nullptr)
   {
      //logSDLError(std::cout, "CreateTexture");
      printf("ERROR CreateTexture\n");
   }
   //Clean up the surface
//   printf("SDL_FreeSurface...\n");
   SDL_FreeSurface(surf);
   return Button_groups[idx].Text_mTexture;
}

SDL_Texture* Button_group_setText(int idx, char *str, SDL_Color txt_color, TTF_Font *font)
{
   if (font == nullptr || !strlen(str))
      return nullptr;

   if (Button_groups[idx].Text_mTexture)
   {
      SDL_DestroyTexture(Button_groups[idx].Text_mTexture);
      Button_groups[idx].Text_mTexture = nullptr;
   }
   SDL_Surface *surf = TTF_RenderText_Blended(font, /*message.c_str()*/ str, txt_color);
   if (surf == nullptr)
   {
      printf("ERROR TTF_RenderText_Blended\n");
      return nullptr;
   }

   Button_groups[idx].Text_mTexture = SDL_CreateTextureFromSurface(gRenderer, surf);
   if (Button_groups[idx].Text_mTexture == nullptr)
   {
      printf("ERROR CreateTexture\n");
   }
   SDL_FreeSurface(surf);
   return Button_groups[idx].Text_mTexture;
}

void Button_group_add_button(int button_idx, int x, int y, int w, int h, int txt_y_offset)
{
   if (Button_groups[button_idx].button_set)
      return;
   Button_groups[button_idx].x = x;
   Button_groups[button_idx].y = y;
   Button_groups[button_idx].Width = w;
   Button_groups[button_idx].Height = h;
   if (button_idx+1 > num_buttons)
      num_buttons = button_idx+1;
   Button_groups[button_idx].button_set = 1;
   Button_groups[button_idx].button_txt_y_offset = txt_y_offset;
}

void Button_group_add_event_code(int button_idx, int event_code)
{
   Button_groups[button_idx].event_code = event_code;
}

int Button_group_check_for_key_press(int x, int y)
{
   for (int i=0; i < num_buttons; i++)
   {
      if (!Button_groups[i].button_set || Button_groups[i].button_disabled)
         continue;
		//Mouse is left of the button
		if( x < Button_groups[i].x )
			continue;
		//Mouse is right of the button
		else if( x > Button_groups[i].x + Button_groups[i].Width )
			continue;
		//Mouse above the button
		else if( y < Button_groups[i].y )
			continue;
		//Mouse below the button
		else if( y > Button_groups[i].y + Button_groups[i].Height )
			continue;
      else
      {
         if (Button_groups[i].event_code)
         {
//            printf("Button Code = %d\n", Button_groups[i].event_code);
            return Button_groups[i].event_code;
         }
         else
         {
            printf("Button = %d\n", i+1);
            return i+1;
         }
      }
	}
   return 0;
}
