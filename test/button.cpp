/*This source code copyrighted by Lazy Foo' Productions (2004-2019)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <stdio.h>
#include <string>

#include <stdlib.h>

#include "handler.h"
#include "button.h"

SDL_Color colorb = { 0, 0, 255, 255 };

LButton::LButton(image_s * img_s_ptr, int btype)
{
   Text_mTexture = NULL;
   mTexture = NULL;
   txt_color = colorb;
   font = gFont24;
   image_s_ptr = img_s_ptr;
   if (image_s_ptr)
   {
      if (image_s_ptr->url)
         loadFromFile(image_s_ptr->url);
      mPosition.x = image_s_ptr->left;
      mPosition.y = image_s_ptr->top;
      rect.x = image_s_ptr->left;
      rect.y = image_s_ptr->top;
      rect.w = image_s_ptr->width;
      rect.h = image_s_ptr->height;
   }
   ButtonType = btype;
}

LButton::~LButton()
{
	//Deallocate
	free();
        printf("freeing button..\n");
}

void LButton::free()
{
	//Free texture if it exists
	if( Text_mTexture != NULL )
	{
		SDL_DestroyTexture( Text_mTexture );
		Text_mTexture = NULL;
	}
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
	}
}

void LButton::setfont(TTF_Font * f)
{
   font = f;
}

bool LButton::loadFromFile( std::string path )
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
      //Color key image    BLACK=0,0,0, WHITE=ff,ff,ff
      //SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0xff, 0xff, 0xff ) );

      //Create texture from surface pixels
      newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
      if( newTexture == NULL )
      {
         printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
      }
      else
      {
         //Get image dimensions
         set_size(loadedSurface->w, loadedSurface->h);
      }
      //Get rid of old loaded surface
      SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

void LButton::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
   //Set rendering space and render to screen
//        SDL_Rect renderQuad = { x, y, Width, Height };
   SDL_Rect renderQuad = { x, y, Width, Height };
   //Set clip rendering dimensions
   if( clip != NULL )
   {
      renderQuad.w = clip->w;
      renderQuad.h = clip->h;
   }

   //Render to screen
   if (mTexture)
      SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );

   if (Text_mTexture)
   {
   	SDL_Rect dst;
      int iW, iH;
      SDL_QueryTexture(Text_mTexture, NULL, NULL, &iW, &iH);
      if (Width)
         dst.x = x + (Width/2 - iW/2);
      else
         dst.x = x;
      if (Height)
         dst.y = y + (Height/2 - iH/2);
      else
         dst.y = y;
      dst.w = iW;
      dst.h = iH;
      SDL_RenderCopy( gRenderer, Text_mTexture, NULL, &dst);
   }
}

SDL_Texture* LButton::setText(const std::string &message)
{
   if (font == nullptr || !message.length())
      return nullptr;

   if (Text_mTexture)
   {
      SDL_DestroyTexture(Text_mTexture);
      Text_mTexture = nullptr;
   }
	//We need to first render to a surface as that's what TTF_RenderText
	//returns, then load that surface into a texture
//   printf("TTF_RenderText_Blended...\n");
   SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), txt_color);
   if (surf == nullptr)
   {
      printf("ERROR TTF_RenderText_Blended\n");
      //logSDLError(std::cout, "TTF_RenderText");
      return nullptr;
   }
//   printf("SDL_CreateTextureFromSurface...\n");
   Text_mTexture = SDL_CreateTextureFromSurface(gRenderer, surf);
   if (Text_mTexture == nullptr)
   {
      //logSDLError(std::cout, "CreateTexture");
      printf("ERROR CreateTexture\n");
   }
   //Clean up the surface
//   printf("SDL_FreeSurface...\n");
   SDL_FreeSurface(surf);
   return Text_mTexture;
}

SDL_Texture* LButton::setText(const char *str, SDL_Color txt_color, TTF_Font *font)
{
   if (font == nullptr || !strlen(str))
      return nullptr;

   if (Text_mTexture)
   {
      SDL_DestroyTexture(Text_mTexture);
      Text_mTexture = nullptr;
   }
	//We need to first render to a surface as that's what TTF_RenderText
	//returns, then load that surface into a texture
//   printf("TTF_RenderText_Blended...\n");
   SDL_Surface *surf = TTF_RenderText_Blended(font, /*message.c_str()*/ str, txt_color);
   if (surf == nullptr)
   {
      printf("ERROR TTF_RenderText_Blended\n");
      //logSDLError(std::cout, "TTF_RenderText");
      return nullptr;
   }
//   printf("SDL_CreateTextureFromSurface...\n");
   Text_mTexture = SDL_CreateTextureFromSurface(gRenderer, surf);
   if (Text_mTexture == nullptr)
   {
      //logSDLError(std::cout, "CreateTexture");
      printf("ERROR CreateTexture\n");
   }
   //Clean up the surface
//   printf("SDL_FreeSurface...\n");
   SDL_FreeSurface(surf);
   return Text_mTexture;
}

void LButton::set_size(int w, int h)
{
   Width = w;
   Height = h;
}

//Top left position
void LButton::set_position(SDL_Point pos)
{
   mPosition = pos;
}

void LButton::set_position_and_size(int x, int y, int w, int h, int button_idx)
{
   mPosition.x = x;
   mPosition.y = y;
   Width = w;
   Height = h;
}

int LButton::check_for_key_press(int x, int y)
{
   if( x < mPosition.x )
   	return 0;
	else if( x > mPosition.x + Width )
		return 0;
	else if( y < mPosition.y )
		return 0;
	else if( y > mPosition.y + Height )
		return 0;
   else
      return 1;
}
