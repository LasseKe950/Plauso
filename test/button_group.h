#ifndef BUTTON_GROUP_H_
#define BUTTON_GROUP_H

//Using SDL, SDL_image, standard IO, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#define MAX_LEN_BUTTONGROUP_TEXT 30
#define NUM_BUTTONGROUP_BUTTONS 50

#include "image.h"

typedef struct
{
   int x;
   int y;
   int Width;
   int Height;
   int button_set;
   int button_disabled;
   int button_txt_y_offset;
   int button_txt_x_offset;
   char button_text_buffer[MAX_LEN_BUTTONGROUP_TEXT];
   Image *background_image;
   Image *icon_image;
   SDL_Texture* Text_mTexture;
   SDL_Texture* Text_mTexture_2; //K‰ytet‰‰n aikan‰ytˆss‰ 00:00
   TTF_Font *font;
   SDL_Point bg_image_pos;
   SDL_Point icon_image_pos; //VOidaan hienos‰‰t‰‰ iconin paikkaa tarpeen mukaan
   int event_code;
} button_group_s;

//Button_group.cpp
extern button_group_s Button_groups[NUM_BUTTONGROUP_BUTTONS];
extern void Button_group_free(void);
extern void Button_group_empty(void);
extern void Button_group_setfont(TTF_Font * f);
extern void Button_group_render(int idx);
extern SDL_Texture* Button_group_setText(int button_idx, int txt_idx, int txt_group, SDL_Color txt_color, TTF_Font *font);
extern SDL_Texture* Button_group_setText(int button_idx, char *str, SDL_Color txt_color, TTF_Font *font);
extern void Button_group_add_button(int button_idx, int x, int y, int w, int h, int txt_y_offset=0);
extern void Button_group_add_images(int button_idx, Image * bg, Image * icon);
extern void Button_group_add_images(int button_idx, Image * bg, Image * icon, int x, int y);
extern void Button_group_add_event_code(int button_idx, int event_code);
extern int Button_group_check_for_key_press(int x, int y);

#define MAX_LEN_DROPDOWN_GROUP_TEXT 60
#define NUM_DROPDOWN_GROUP_BUTTONS 30

typedef struct
{
   int x;
   int y;
   int Width;
   int Height;
   int button_set;
   int button_disabled;
   int button_txt_y_offset;
   char button_text_buffer[MAX_LEN_DROPDOWN_GROUP_TEXT];
   TTF_Font *font;
   int event_code;
} dropdown_group_s;

//Button_group.cpp
extern dropdown_group_s Dropdown_groups[NUM_DROPDOWN_GROUP_BUTTONS];
extern void Dropdown_group_empty(void);
extern void Dropdown_group_setfont(TTF_Font * f);
extern void Dropdown_group_add_selection(int button_idx, int x, int y, int w, int h);
extern void Dropdown_group_add_event_code(int button_idx, int event_code);
extern int Dropdown_group_check_for_key_press(int x, int y);

#endif
