#ifndef BUTTON_H_
#define BUTTON_H

#if 0

//Using SDL, SDL_image, standard IO, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "img_header.h"

extern TTF_Font *gFont24;
extern TTF_Font *BigFont44;
extern TTF_Font *VeryBigFont88;
extern TTF_Font *font_norm_bold_20px;
extern TTF_Font *font_norm_bold_80px;
extern TTF_Font *font_norm_regular_50px;

//The window renderer
extern SDL_Renderer* gRenderer;
extern SDL_Rect gScreenRect;

#define BUTTON_TEXT_BUFF_LEN 20

class LButton
{
	public:
		//Initializes internal variables
      LButton(image_s * img_s_ptr, int btype=0);

      ~LButton();

      //Deallocates texture
      void free();

      //Loads image at specified path
      bool loadFromFile( std::string path );

      //Renders texture at given point
      void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

      //Sets top left position
      void setPosition( int x, int y );
      void set_position_and_size(int x, int y, int w, int h, int button_idx);

	   int check_for_key_press(int x, int y);

      SDL_Texture* setText(const std::string &message);
      SDL_Texture* setText(const char *str, SDL_Color txt_color, TTF_Font *font);
      SDL_Texture* Text_mTexture;

      SDL_Texture* mTexture;

      SDL_Color txt_color;
      SDL_Color rect_color;
      SDL_Rect rect;
      TTF_Font *font;

      void setfont(TTF_Font * f);

      int Width;
      int Height;
      int ButtonType;
      image_s * image_s_ptr;

      void set_size(int w, int h);
      //Top left position
      SDL_Point mPosition;
      void set_position(SDL_Point pos);

      char * name;
      char ButtonTextBuff[BUTTON_TEXT_BUFF_LEN];
};

#define BUTTON_TYPE_NORMAL 0
//#define BUTTON_TYPE_RECTANGLE 1
//#define BUTTON_TYPE_HELPER_BUTTON 2

//Button_group.cpp
extern void Button_group_free(void);
extern void Button_group_empty(void);
extern void Button_group_setfont(TTF_Font * f);
extern void Button_group_render(int idx);
extern SDL_Texture* Button_group_setText(const char *str, SDL_Color txt_color, TTF_Font *font);
extern void Button_group_add_button(int x, int y, int w, int h, int button_idx, int txt_y_offset=0);
extern int Button_group_check_for_key_press(int x, int y);

//Global place to put buttongroop texts
#define MAX_LEN_BUTTONGROUP_TEXT 30
#define NUM_BUTTONGROUP_TEXT 20
extern char button_text_buffers[MAX_LEN_BUTTONGROUP_TEXT][NUM_BUTTONGROUP_TEXT];

#endif

#endif
