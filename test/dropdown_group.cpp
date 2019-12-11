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

dropdown_group_s Dropdown_groups[NUM_DROPDOWN_GROUP_BUTTONS];

void Dropdown_group_empty()
{
   num_buttons = 0;
   memset(Dropdown_groups, 0, sizeof(Dropdown_groups));
}
void Dropdown_group_add_selection(int button_idx, int x, int y, int w, int h)
{
   Dropdown_groups[button_idx].x = x;
   Dropdown_groups[button_idx].y = y;
   Dropdown_groups[button_idx].Width = w;
   Dropdown_groups[button_idx].Height = h;
   if (button_idx+1 > num_buttons)
      num_buttons = button_idx+1;
   Dropdown_groups[button_idx].button_set = 1;
}

void Dropdown_group_add_event_code(int button_idx, int event_code)
{
   Dropdown_groups[button_idx].event_code = event_code;
}

int Dropdown_group_check_for_key_press(int x, int y)
{
   for (int i=0; i < num_buttons; i++)
   {
      if (!Dropdown_groups[i].button_set || Dropdown_groups[i].button_disabled)
         continue;
		//Mouse is left of the button
		if( x < Dropdown_groups[i].x )
			continue;
		//Mouse is right of the button
		else if( x > Dropdown_groups[i].x + Dropdown_groups[i].Width )
			continue;
		//Mouse above the button
		else if( y < Dropdown_groups[i].y )
			continue;
		//Mouse below the button
		else if( y > Dropdown_groups[i].y + Dropdown_groups[i].Height )
			continue;
      else
      {
         if (Dropdown_groups[i].event_code)
         {
            printf("Dropdown Code = %d\n", Dropdown_groups[i].event_code);
            return Dropdown_groups[i].event_code;
         }
         else
         {
            return i+1;
         }
      }
	}
   return 0;
}
