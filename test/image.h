#ifndef IMAGE_H
#define IMAGE_H
//Using SDL, SDL_image, standard IO, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

//The window renderer
extern SDL_Renderer* gRenderer;

class Image
{
	public:
		//Initializes internal variables
      Image(std::string path);

      ~Image();

      //Deallocates texture
      void free();

      //Renders texture at given point
      void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

      SDL_Texture* mTexture;

      int Width;
      int Height;
      void set_size(int w, int h);

      //Top left position
      SDL_Point mPosition;
      void set_position(SDL_Point pos);
};

#endif
