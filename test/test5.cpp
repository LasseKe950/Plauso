#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

int SCREEN_WIDTH;
int SCREEN_HEIGHT;

TTF_Font *font;
SDL_Texture *image;
int ctr;

void cleanup(SDL_Texture *background, SDL_Texture *image, SDL_Renderer *renderer, SDL_Window *window) {
	if (background != nullptr)
		SDL_DestroyTexture(background);
	if (image != nullptr)
		SDL_DestroyTexture(image);
	if (renderer != nullptr)
		SDL_DestroyRenderer(renderer);
	if (window != nullptr)
		SDL_DestroyWindow(window);
}

void logSDLError(std::ostream &os, const std::string &msg){
	os << msg << " error: " << SDL_GetError() << std::endl;
}

int load_font(const std::string &fontFile, int fontSize)
{
	//Open the font
	font = TTF_OpenFont(fontFile.c_str(), fontSize);
	if (font == nullptr){
		logSDLError(std::cout, "TTF_OpenFont");
		return 0;
	}	
	TTF_CloseFont(font);
   return 1;
}

/**
* Render the message we want to display to a texture for drawing
* @param message The message we want to display
* @param fontFile The font we want to use to render the text
* @param color The color we want the text to be
* @param fontSize The size we want the font to be
* @param renderer The renderer to load the texture in
* @return An SDL_Texture containing the rendered message, or nullptr if something went wrong
*/
SDL_Texture* renderText(const std::string &message, SDL_Color color, SDL_Renderer *renderer)
{
	if (font == nullptr)
      return nullptr;
	//We need to first render to a surface as that's what TTF_RenderText
	//returns, then load that surface into a texture
	SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);
	if (surf == nullptr){
		TTF_CloseFont(font);
		logSDLError(std::cout, "TTF_RenderText");
		return nullptr;
	}
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
	if (texture == nullptr){
		logSDLError(std::cout, "CreateTexture");
	}
	//Clean up the surface
	SDL_FreeSurface(surf);
	return texture;
}

/**
* Loads an image into a texture on the rendering device
* @param file The image file to load
* @param ren The renderer to load the texture onto
* @return the loaded texture, or nullptr if something went wrong.
*/
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren){
	SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());
	if (texture == nullptr){
		logSDLError(std::cout, "LoadTexture");
	}
	return texture;
}

/**
* Draw an SDL_Texture to an SDL_Renderer at some destination rect
* taking a clip of the texture if desired
* @param tex The source texture we want to draw
* @param ren The renderer we want to draw to
* @param dst The destination rectangle to render the texture to
* @param clip The sub-section of the texture to draw (clipping rect)
*		default of nullptr draws the entire texture
*/
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst,
	SDL_Rect *clip = nullptr)
{
	SDL_RenderCopy(ren, tex, clip, &dst);
}

/**
* Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
* the texture's width and height and taking a clip of the texture if desired
* If a clip is passed, the clip's width and height will be used instead of
*	the texture's
* @param tex The source texture we want to draw
* @param ren The renderer we want to draw to
* @param x The x coordinate to draw to
* @param y The y coordinate to draw to
* @param clip The sub-section of the texture to draw (clipping rect)
*		default of nullptr draws the entire texture
*/
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y,
	SDL_Rect *clip = nullptr)
{
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	if (clip != nullptr){
		dst.w = clip->w;
		dst.h = clip->h;
	}
	else {
		SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
	}
	renderTexture(tex, ren, dst, clip);
}

const std::string resPath = "Lesson3/";

int init_ttf_osasto(int w, int h)
{
	SCREEN_WIDTH = w;
	SCREEN_HEIGHT = h;

	if (TTF_Init() != 0){
		logSDLError(std::cout, "TTF_Init");
//		SDL_Quit();
		return 1;
	}
	ctr = 0;
   if (load_font(resPath + "sample.ttf", 32) == 0)
		return 1;
   else
	   return 0;
}

int show_ttf(SDL_Renderer *renderer, char *text, int color)
{
	//We'll render the string "TTF fonts are cool!" in white
	//Color is in RGBA format
	SDL_Color colorb = { 0, 0, 255, 255 };
	SDL_Color colorr = { 255, 0, 0, 255 };

	if (color == 0)
		image = renderText(text, colorb, 32, renderer);
	else
		image = renderText(text, colorr, 48, renderer);
	if (image == nullptr){
		TTF_Quit();
		return 1;
	}
	ctr = 1 - ctr;
	//Get the texture w/h so we can center it in the screen
	int iW, iH;
	SDL_QueryTexture(image, NULL, NULL, &iW, &iH);
	int x = SCREEN_WIDTH / 2 - iW / 2;
	int y = SCREEN_HEIGHT / 2 - iH / 2;

	//Note: This is within the program's main loop
	SDL_RenderClear(renderer);
	//We can draw our message as we do any other texture, since it's been
	//rendered to a texture
	renderTexture(image, renderer, x, y);
	SDL_RenderPresent(renderer);
	if (image != nullptr)
	   	SDL_DestroyTexture(image);
	image = nullptr;
	return 0;
}

int quit_ttf_osasto(void)
{
	if (image != nullptr)
	   	SDL_DestroyTexture(image);
   	TTF_Quit();
	return 0;
}

