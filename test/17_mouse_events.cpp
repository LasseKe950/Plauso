/*This source code copyrighted by Lazy Foo' Productions (2004-2019)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <stdio.h>
#include <string>

#include <sys/types.h>
#include <sys/socket.h>

#include "termios.h"
#include "sys/signal.h"

#include <stdlib.h>

struct termios orig_termios;

void reset_terminal_mode()
{
    tcsetattr(0, TCSANOW, &orig_termios);
}

void set_conio_terminal_mode()
{
    struct termios new_termios;

    /* take two copies - one for now, one for later */
    tcgetattr(0, &orig_termios);
    memcpy(&new_termios, &orig_termios, sizeof(new_termios));

    /* register cleanup handler, and set the new terminal mode */
    atexit(reset_terminal_mode);
    cfmakeraw(&new_termios);
    tcsetattr(0, TCSANOW, &new_termios);
}

int kbhit()
{
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv);
}

/*
int getch()
{
    int r;
    unsigned char c;
    if ((r = read(0, &c, sizeof(c))) < 0) {
        return r;
    } else {
        return c;
    }
}
*/

//Screen dimension constants
//const int SCREEN_WIDTH = 640;
//const int SCREEN_HEIGHT = 480;

//Button constants
//const int BUTTON_WIDTH = 300;
//const int BUTTON_HEIGHT = 200;

TTF_Font *font = NULL;
SDL_Texture *image;

int init_ttf_osasto(void);
int quit_ttf_osasto(void);

const int BUTTON_WIDTH = 100;
const int BUTTON_HEIGHT = 100;

const int TOTAL_BUTTONS = 4;

enum LButtonSprite
{
	BUTTON_SPRITE_MOUSE_OUT = 0,
	BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
	BUTTON_SPRITE_MOUSE_DOWN = 2,
	BUTTON_SPRITE_MOUSE_UP = 3,
	BUTTON_SPRITE_TOTAL = 4
};

//Texture wrapper class
class LTexture
{
	public:
		//Initializes variables
		LTexture();

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile( std::string path );
		
		#ifdef _SDL_TTF_H
		//Creates image from font string
		bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
		#endif
		
		//Deallocates texture
		void free();

		//Set color modulation
		void setColor( Uint8 red, Uint8 green, Uint8 blue );

		//Set blending
		void setBlendMode( SDL_BlendMode blending );

		//Set alpha modulation
		void setAlpha( Uint8 alpha );
		
		//Renders texture at given point
		void render( int x, int y, SDL_Rect* clip = NULL, SDL_Texture* Text_mTexture = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

		//Gets image dimensions
		int getWidth();
		int getHeight();

	private:
		//The actual hardware texture
		SDL_Texture* mTexture;

		//Image dimensions
		int mWidth;
		int mHeight;
};

//The mouse button
class LButton
{
	public:
		//Initializes internal variables
		LButton();

		//Sets top left position
		void setPosition( int x, int y );

		//Handles mouse event
		void handleEvent( SDL_Event* e );
	
		//Shows button sprite
		void render();

      SDL_Texture* showText(const std::string &message);
		SDL_Texture* Text_mTexture;

	private:
		//Top left position
		SDL_Point mPosition;

		//Currently used global sprite
		LButtonSprite mCurrentSprite;
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Screen dimensions
SDL_Rect gScreenRect = { 0, 0, 320, 240 };

//Mouse button sprites
SDL_Rect gSpriteClips[ BUTTON_SPRITE_TOTAL ];
LTexture gButtonSpriteSheetTexture;

//Buttons objects
LButton gButtons[ TOTAL_BUTTONS ]; 

LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile( std::string path )
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
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

#ifdef _SDL_TTF_H
bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor )
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( font, textureText.c_str(), textColor );
	if( textSurface == NULL )
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}
	else
	{
		//Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
		if( mTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}
	
	//Return success
	return mTexture != NULL;
}
#endif

void LTexture::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
	//Modulate texture rgb
	SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending )
{
	//Set blending function
	SDL_SetTextureBlendMode( mTexture, blending );
}
		
void LTexture::setAlpha( Uint8 alpha )
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::render( int x, int y, SDL_Rect* clip, SDL_Texture* Text_mTexture, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
   SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
   if (Text_mTexture)
   {
   	SDL_Rect dst;
      int iW, iH;
      SDL_QueryTexture(Text_mTexture, NULL, NULL, &iW, &iH);
   	dst.x = x;
	   dst.y = y;
	   dst.w = iW;
	   dst.h = iH;
      SDL_RenderCopy( gRenderer, Text_mTexture, NULL, &dst);
   }
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

LButton::LButton()
{
	mPosition.x = 0;
	mPosition.y = 0;

	mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
}

void LButton::setPosition( int x, int y )
{
	mPosition.x = x;
	mPosition.y = y;
}


SDL_Color colorb = { 0, 0, 255, 255 };

SDL_Texture* LButton::showText(const std::string &message)
{
	if (font == nullptr)
      return nullptr;
	//We need to first render to a surface as that's what TTF_RenderText
	//returns, then load that surface into a texture
//   printf("TTF_RenderText_Blended...\n");
	SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), colorb);
	if (surf == nullptr){
      printf("ERROR TTF_RenderText_Blended\n");
		//logSDLError(std::cout, "TTF_RenderText");
		return nullptr;
	}
//   printf("SDL_CreateTextureFromSurface...\n");
	Text_mTexture = SDL_CreateTextureFromSurface(gRenderer, surf);
	if (Text_mTexture == nullptr){
		//logSDLError(std::cout, "CreateTexture");
      printf("ERROR CreateTexture\n");
	}
	//Clean up the surface
//   printf("SDL_FreeSurface...\n");
	SDL_FreeSurface(surf);
	return Text_mTexture;
}

void LButton::handleEvent( SDL_Event* e )
{
//   float fx, fy;
	//If mouse event happened
//	if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
	if( e->type == SDL_FINGERMOTION || e->type == SDL_FINGERDOWN || e->type == SDL_FINGERUP )
	{
		//Get mouse position
		int x, y;
//		SDL_GetMouseState( &x, &y );

		x = e->tfinger.x * gScreenRect.w;
		y = e->tfinger.y * gScreenRect.h;

//      fx = mPosition.x * e->tfinger.x;
//      fy = mPosition.y * e->tfinger.y;

		//Check if mouse is in button
		bool inside = true;

		//Mouse is left of the button
		if( x < mPosition.x )
		{
			inside = false;
		}
		//Mouse is right of the button
		else if( x > mPosition.x + BUTTON_WIDTH )
		{
			inside = false;
		}
		//Mouse above the button
		else if( y < mPosition.y )
		{
			inside = false;
		}
		//Mouse below the button
		else if( y > mPosition.y + BUTTON_HEIGHT )
		{
			inside = false;
		}
                                 //inside = true;
		//Mouse is outside button
		if( !inside )
		{
			mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT; //=0,red
         showText("Mouse out");
		}
		//Mouse is inside button
		else
		{
			//Set mouse over sprite
			switch( e->type )
			{
				case SDL_FINGERMOTION:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION; //=1,green
            showText("Motion");
            printf("Mouse motion x=%d y=%d\n", x, y);
				break;
			
				case SDL_FINGERDOWN:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN; //=2,yellow
            printf("Mouse down x=%d y=%d\n", x, y);
            showText("Finger down");
            printf("%.3f,%.3f\n", e->tfinger.x, e->tfinger.y);
				break;
				
				case SDL_FINGERUP:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_UP; //=3,blue
            printf("Mouse up x=%d y=%d\n", x, y);
            showText("Finger up");
				break;
			}
		}
	}
}
	
void LButton::render()
{
	//Show current button sprite
	gButtonSpriteSheetTexture.render( mPosition.x, mPosition.y, &gSpriteClips[ mCurrentSprite ], Text_mTexture);
	if (Text_mTexture != nullptr)
   {
  	   SDL_DestroyTexture(Text_mTexture);
      Text_mTexture = nullptr;
   }
//   printf("LButton::render()\n");
}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

      //Get device display mode
      SDL_DisplayMode displayMode;
      if( SDL_GetCurrentDisplayMode( 0, &displayMode ) == 0 )
      {
          gScreenRect.w = displayMode.w;
          gScreenRect.h = displayMode.h;
          printf("Width=%d Height=%d\n", displayMode.w, displayMode.h);
      }
      else
          printf("==Width=%d Height=%d\n", displayMode.w, displayMode.h);

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, gScreenRect.w, gScreenRect.h, SDL_WINDOW_SHOWN );
//		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_SOFTWARE);
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

#define BUTTON_OFFSET 0

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load sprites
//	if( !gButtonSpriteSheetTexture.loadFromFile( "button.png" ) )
	if( !gButtonSpriteSheetTexture.loadFromFile( "dots.png" ) )
	{
		printf( "Failed to load button sprite texture!\n" );
		success = false;
	}
	else
	{
		//Set sprites
/*
		for( int i = 0; i < BUTTON_SPRITE_TOTAL; ++i )
		{
			gSpriteClips[ i ].x = 0;
			gSpriteClips[ i ].y = i * 200;
			gSpriteClips[ i ].w = BUTTON_WIDTH;
			gSpriteClips[ i ].h = BUTTON_HEIGHT;
		}
*/
		//Set top left sprite
		gSpriteClips[ 0 ].x =   0;
		gSpriteClips[ 0 ].y =   0;
		gSpriteClips[ 0 ].w = 100;
		gSpriteClips[ 0 ].h = 100;

		//Set top right sprite
		gSpriteClips[ 1 ].x = 100;
		gSpriteClips[ 1 ].y =   0;
		gSpriteClips[ 1 ].w = 100;
		gSpriteClips[ 1 ].h = 100;
		
		//Set bottom left sprite
		gSpriteClips[ 2 ].x =   0;
		gSpriteClips[ 2 ].y = 100;
		gSpriteClips[ 2 ].w = 100;
		gSpriteClips[ 2 ].h = 100;

		//Set bottom right sprite
		gSpriteClips[ 3 ].x = 100;
		gSpriteClips[ 3 ].y = 100;
		gSpriteClips[ 3 ].w = 100;
		gSpriteClips[ 3 ].h = 100;

		//Set buttons in corners
		gButtons[ 0 ].setPosition( 0 + BUTTON_OFFSET, 0 + BUTTON_OFFSET );
		gButtons[ 1 ].setPosition( gScreenRect.w - (BUTTON_WIDTH + BUTTON_OFFSET), 0 + BUTTON_OFFSET );
		gButtons[ 2 ].setPosition( 0 + BUTTON_OFFSET, gScreenRect.h - (BUTTON_HEIGHT + BUTTON_OFFSET) );
		gButtons[ 3 ].setPosition( gScreenRect.w - (BUTTON_WIDTH + BUTTON_OFFSET), gScreenRect.h - (BUTTON_HEIGHT + BUTTON_OFFSET) );

//      gButtonSpriteSheetTexture.setAlpha( 20 ); //TEST
	}

	return success;
}

void close()
{
	//Free loaded images
	gButtonSpriteSheetTexture.free();

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

int main( int argc, char* args[] )
{
   int csctr=0;
#if 0
	//Start up SDL and create window
   FILE*newout = freopen("/dev/ttyUSB0", "w", stdout);
   if (!newout) { 
      perror("freopen");
   } 
   else
      stdout = newout;
   printf( "stdout --> ttyUSB0\n" );

   FILE*newin = freopen("/dev/ttyUSB0", "r", stdin);
   if (!newin) { 
      perror("freopen in");
   } 
   else
      stdin = newin;
   printf( "stdin <-- ttyUSB0\n" );
#endif

	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() || (init_ttf_osasto() == -1) )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

//	    	set_conio_terminal_mode();

			//While application is running
			while( !quit )
			{
				//Handle events on queue
//				while( SDL_PollEvent( &e ) != 0 )
				SDL_WaitEvent( &e );
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
					
					//Handle button events
					for( int i = 0; i < TOTAL_BUTTONS; ++i )
					{
						gButtons[ i ].handleEvent( &e );
					}
				}

//            if( e.type != SDL_FINGERMOTION && e.type != SDL_FINGERDOWN && e.type != SDL_FINGERUP )
//               continue;

//printf("Clear screen..%d\n", csctr++);
				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				//Render buttons
				for( int i = 0; i < TOTAL_BUTTONS; ++i )
				{
					gButtons[ i ].render();
				}

				//Update screen
				SDL_RenderPresent( gRenderer );

//            if (kbhit())
//            {
//    	    	   (void)getc(0); /* consume the character */
//               quit = true;
//            }
			}
		}
	}

	//Free resources and close SDL
	close();
	quit_ttf_osasto();

	return 0;
}

int load_font(const std::string &fontFile, int fontSize)
{
	//Open the font
	font = TTF_OpenFont(fontFile.c_str(), fontSize);
	if (font == nullptr){
//		logSDLError(std::cout, "TTF_OpenFont");
      printf("ERROR TTF_OpenFont\n");
		return 0;
	}	
   return 1;
}

int init_ttf_osasto(void)
{
	if (TTF_Init() != 0){
//		logSDLError(std::cout, "TTF_Init");
      printf("ERROR TTF_Init\n");
//		SDL_Quit();
		return -1;
	}
   if (load_font("ArialNarrowBold.ttf", 24) == 0)
   {
      printf("ERROR load font\n");
		return -1;
   }
   else
	   return 0;
}

int quit_ttf_osasto(void)
{
   if (font != nullptr)
	   TTF_CloseFont(font);
	if (image != nullptr)
	   	SDL_DestroyTexture(image);
   	TTF_Quit();
	return 0;
}
