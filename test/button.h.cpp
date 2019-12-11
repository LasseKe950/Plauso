#ifndef BUTTON_H_

//Using SDL, SDL_image, standard IO, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

extern TTF_Font *gFont;
//The window renderer
extern SDL_Renderer* gRenderer = NULL;

class LButton
{
	public:
		//Initializes internal variables
		LButton();

		~LButton();

		//Deallocates texture
		void free();

		//Loads image at specified path
		bool loadFromFile( std::string path );

		//Renders texture at given point
		void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

		//Sets top left position
		void setPosition( int x, int y );

		//Handles mouse event
		void handleEvent( SDL_Event* e );
	
		//Shows button sprite
		void render();

      SDL_Texture* setText(const std::string &message);
		SDL_Texture* Text_mTexture;

		SDL_Texture* mTexture;

      SDL_Color txt_color;
      TTF_Font *font;
      void setfont(TTF_Font * f);

	private:
		//Top left position
		SDL_Point mPosition;

		//Currently used global sprite
		LButtonSprite mCurrentSprite;
};

#endif