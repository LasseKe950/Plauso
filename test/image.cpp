//Using SDL, SDL_image, standard IO, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <stdio.h>
#include <string>
#include <stdlib.h>

#include "image.h"

Image::Image( std::string path )
{
   mTexture = NULL;
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
}

Image::~Image()
{
	//Deallocate
	free();
}

void Image::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
	}
}

void Image::set_size(int w, int h)
{
   Width = w;
   Height = h;
}


void Image::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
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
}

//Top left position
void Image::set_position(SDL_Point pos)
{
   mPosition = pos;
}
