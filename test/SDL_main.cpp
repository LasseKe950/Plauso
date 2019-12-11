/* stop X -> sudo init 3

 source /opt/fslc-x11/2.5.2/environment-setup-armv7at2hf-neon-fslc-linux-gnueabi

 wlan:
   connmanctl disable wifi
   ifconfig wlan0 up
   wpa_supplicant -B -Dnl80211 -iwlan0 -c/etc/wpa_supplicant.conf
   ifconfig eth0 down
   udhcpc -iwlan0

 wpa_supplicant.conf:issa wlan-tiedot...salaamattomina?

 Can:
   ip link set can0 up type can bitrate 83000

 X ja näytön kääntö, pitää käynnistää x ensin taustalle
   startx &
   export DISPLAY=:0
   xrandr --output 'DISP3 BG' --rotate left
   ./demo_mx6

*/

//Using SDL, SDL_image, standard IO, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <stdio.h>
#include <string>

#include <stdlib.h>

#include "handler.h"
#include "menu_ids.h"

#include "button_defs.h"
#include "tekstit.h"
#include "var.h"
#include "externs.h"
#include "kettle_functions.h"

const int prog_ver_major = 0;
const int prog_ver_minor = 11;

Uint32 time_to_approve_next_button_event;

int motion_ctr, motion_first_pos_x, motion_first_pos_y;
int power_off_set;

void debug_show_tilatiedot(void);
void test(void);
extern int can_communication_thread(void* data );
extern int sqlite_thread(void* data );
extern int update_thread(void* data );

int Stop_Threads = 0;

int init_ttf_osasto(void);
int quit_ttf_osasto(void);

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Nämä esitelty externs.h
TTF_Font *gFont24 = NULL;
TTF_Font *BigFont44 = NULL;
TTF_Font *VeryBigFont88 = NULL;
TTF_Font *font_norm_bold_18px = NULL;
TTF_Font *font_norm_bold_20px = NULL;
TTF_Font *font_norm_bold_42px = NULL;
TTF_Font *font_norm_bold_56px = NULL;
TTF_Font *font_norm_bold_62px = NULL;
TTF_Font *font_norm_bold_80px = NULL;
TTF_Font *font_norm_bold_110px = NULL;
TTF_Font *font_norm_bold_70px = NULL;
TTF_Font *font_norm_regular_20px = NULL;
TTF_Font *font_norm_regular_25px = NULL;
TTF_Font *font_norm_regular_30px = NULL;
TTF_Font *font_norm_regular_35px = NULL;
TTF_Font *font_norm_regular_40px = NULL;
TTF_Font *font_norm_regular_50px = NULL;
TTF_Font *font_norm_medium_16px = NULL;
TTF_Font *font_norm_light_40px = NULL;

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Screen dimensions
//SDL_Rect gScreenRect = { 0, 0, 1024, 768 };
//SDL_Rect gScreenRect = { 0, 0, 480, 800 };
SDL_Rect gScreenRect = { 0, 0, 800, 1280};
SDL_TimerID my_timer_id;

int disable_repeat;

bool init()
{
	//Initialization flag
	bool success = true;

        printf("SDL_GetCurrentVideoDriver: %s\n", SDL_GetCurrentVideoDriver());

	//Initialize SDL
        if( SDL_Init( SDL_INIT_TIMER ) < 0 )
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
                gWindow = SDL_CreateWindow( "Dieta USO", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, gScreenRect.w, gScreenRect.h, SDL_WINDOW_SHOWN );
//		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
                        gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_SOFTWARE);//SDL_RENDERER_ACCELERATED);
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

void close()
{
	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
 //Remove timer in case the call back was not called 
   SDL_RemoveTimer( my_timer_id );
}

Uint32 my_callbackfunc(Uint32 interval, void *param)
{
    SDL_Event event;
    SDL_UserEvent userevent;

    /* In this example, our callback pushes an SDL_USEREVENT event
    into the queue, and causes our callback to be called again at the
    same interval: */

    userevent.type = SDL_USEREVENT;
    userevent.code = 0;
    userevent.data1 = param;
    userevent.data2 = NULL;

    event.type = SDL_USEREVENT;
    event.user = userevent;

    SDL_PushEvent(&event);
    return(interval);
}


int main( int argc, char* args[] )
{
   int ret, x, y;
   int repeat_ctr=0;
   int chk_repeat=0;
   int repeat_key_press=0;
   int timeout;

   power_off_set = 0;
   if( !init() )
   {
      printf( "Failed to initialize!\n" );
   }
   else
   {
      //Load media
      if( init_ttf_osasto() == -1)
      {
         printf( "Failed to load media!\n" );
      }
      else
      {
         load_all_images(); //buttons_defs.cpp
         create_them();
         test();

         //Main loop flag
         bool quit = false;

			//Run the threads
			SDL_Thread* thread1 = SDL_CreateThread( can_communication_thread, "Thread 1", (void*)NULL);
			SDL_Thread* thread2 = SDL_CreateThread( sqlite_thread, "Thread 2", (void*)NULL);
			SDL_Thread* thread3 = SDL_CreateThread( update_thread, "Thread 3", (void*)NULL);

         //my_timer_id = SDL_AddTimer(1000, my_callbackfunc, (void*)_1SEc_TIMERID_STR);
         time_to_approve_next_button_event = SDL_GetTicks() + 100;

         //Event handler
         SDL_Event e;
         //While application is running
         while( !quit )
         {
            //Handle events on queue
            //while( SDL_PollEvent( &e ) != 0 )
            if (chk_repeat)
            {
               if (repeat_ctr < 1)
               {
                  timeout = 500;
               }
               else
               {
                  timeout = 50;
                  repeat_key_press = 1;
               }
               ++repeat_ctr;
            }
            else
            {
               timeout = 500;
            }

            ret = SDL_WaitEventTimeout( &e, timeout);
            if (ret == 0) //timeout
            {
               for (int can=0; can < NUM_OF_CAN_MODULES; can++)
               {
                  if (++can_comm_err_counter[can] > 20)
                  {
                     if (can_comm_error[can] == 0)
                        printf("%s can=%d\n", (char*)error_tekstit[ERROR_IDX_CAN_COMM_ERROR], can);
                     can_comm_error[can] = 1;
                  }
               }
               if (disable_repeat)
               {
                  if (--disable_repeat == 0)
                     long_key_press = chk_repeat = 1;
                                       printf("Disable repeat=%d\n", disable_repeat);
                  continue;
               }
               else if (!chk_repeat)
               {
                  e.type = 0;
               }
               else if (!repeat_key_press)
                  continue;
            }
            //SDL_WaitEvent( &e );
            {
               if( e.type == SDL_QUIT )
               {
                  quit = true;
               }
//               printf("Event = %d\n", e.type);

               if( e.type == SDL_MULTIGESTURE )
                  continue;

               if(e.type == SDL_MOUSEMOTION)
               {
                  if (pyyhk_enabled ==0)
                     continue;
                   //Get the mouse offsets 
                  x = e.motion.x; 
                  y = e.motion.y;

                  if (motion_ctr++ == 0)
                  {
                     motion_first_pos_x = x;
                     motion_first_pos_y = y;
                  }
                  else
                  {
                     if (y < (motion_first_pos_y-5)) //pyyhk. suunta ylös
                     {
                        motion_first_pos_y = y;
                        if (++motion_ctr > 10)
                        {
                           //printf("UP-PYYHK x=%d ... y=%d\n", x, y);
                           SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                           SDL_RenderClear( gRenderer );
                           motion_ctr = 0;
                           handle_code(0, 10000, &e);
                           SDL_RenderPresent( gRenderer );
                           time_to_approve_next_button_event = SDL_GetTicks() + 200;
                        }
                     }
                     else if (y > (motion_first_pos_y+5)) //pyyhk. suunta alas
                     {
                        motion_first_pos_y = y;
                        if (++motion_ctr > 10)
                        {
                           //printf("DOWN-PYYHK x=%d ... y=%d\n", x, y);
                           SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                           SDL_RenderClear( gRenderer );
                           motion_ctr = 0;
                           handle_code(0, -10000, &e);
                           SDL_RenderPresent( gRenderer );
                           time_to_approve_next_button_event = SDL_GetTicks() + 200;
                        }
                     }
                     else
                        motion_ctr = 0;
                  }
                  //printf("x=%d ... y=%d ctr=%d first_pos_y=%d\n", x, y, motion_ctr, motion_first_pos_y);
               }
               else if (SDL_TICKS_PASSED(SDL_GetTicks(), time_to_approve_next_button_event))
               {
                  motion_ctr = 0;
                  if( e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP)
                  {
                     SDL_GetMouseState( &x, &y );
                     if (e.type == SDL_MOUSEBUTTONDOWN)
                     {
                        //repeat_ctr = 0;
                        chk_repeat = 1;
                        repeat_key_press = 0;
//printf("MOUSEBTN DOwN x=%d y=%d\n", x, y);
                     }
                     else
                     {
                        repeat_ctr = chk_repeat = 0;
                        repeat_key_press = 0;
                        long_key_press = 0;
                     }
                  }
                  else
                     x = y = -1;

                  disable_repeat = 0;
                  handle_code(x, y, &e);
                  if (disable_repeat)
                  {
                     chk_repeat = 0;
                     printf("Disable repeat=%d\n", disable_repeat);
                  }
                  long_key_press = 0;
               }
            }
            //Update screen
            SDL_RenderPresent( gRenderer );

            if (power_off_set)
            {
               clear_all_kettle_functions();
               if (++power_off_set > 6)
                  quit = 1;
            }
         }
         Stop_Threads = 1;
         //Wait for threads to finish
	      SDL_WaitThread( thread1, NULL );
	      SDL_WaitThread( thread2, NULL );
	      SDL_WaitThread( thread3, NULL );
      }
   }
   //Free resources and close SDL
   close();
   quit_ttf_osasto();
   delete_all_images(); //buttons_defs.cpp

#ifdef KORTIT_KYTKETTY
   if (power_off_set)
      system("poweroff &");
#endif

   return 0;
}

/*
int load_font(const std::string &fontFile, int fontSize)
{
	//Open the font
	gFont = TTF_OpenFont(fontFile.c_str(), fontSize);
	if (gFont == nullptr){
//		logSDLError(std::cout, "TTF_OpenFont");
      printf("ERROR TTF_OpenFont\n");
		return 0;
	}	
   return 1;
}*/

int init_ttf_osasto(void)
{
    if (TTF_Init() != 0)
    {
//		logSDLError(std::cout, "TTF_Init");
        printf("ERROR TTF_Init\n");
//		SDL_Quit();
		return -1;
    }
/*
    gFont24 = TTF_OpenFont("ArialNarrowBold.ttf", 24);
    BigFont44 = TTF_OpenFont("ArialNarrowBold.ttf", 44);
    VeryBigFont88 = TTF_OpenFont("ArialNarrowBold.ttf", 150);
*/
    gFont24 = TTF_OpenFont("Simplon font/otf files/SimplonMono-Bold.otf", 24);
    BigFont44 = TTF_OpenFont("Simplon font/otf files/SimplonMono-Bold.otf", 44);
    font_norm_bold_18px = TTF_OpenFont("Simplon font/otf files/SimplonNorm-Bold.otf", 18);
    font_norm_bold_20px = TTF_OpenFont("Simplon font/otf files/SimplonNorm-Bold.otf", 20);

    font_norm_bold_80px = TTF_OpenFont("Simplon font/otf files/SimplonNorm-Bold.otf", 80);
    font_norm_bold_70px = TTF_OpenFont("Simplon font/otf files/SimplonNorm-Bold.otf", 70);
    font_norm_bold_110px = TTF_OpenFont("Simplon font/otf files/SimplonNorm-Bold.otf", 110);
    VeryBigFont88 = TTF_OpenFont("Simplon font/otf files/SimplonMono-Bold.otf", 100);

    font_norm_regular_20px = TTF_OpenFont("Simplon font/otf files/SimplonNorm-Regular.otf", 20);
    font_norm_regular_50px = TTF_OpenFont("Simplon font/otf files/SimplonNorm-Regular.otf", 50);
    font_norm_medium_16px = TTF_OpenFont("Simplon font/otf files/SimplonNorm-Medium.otf", 16);
    font_norm_regular_35px = TTF_OpenFont("Simplon font/otf files/SimplonNorm-Regular.otf", 35);

    font_norm_regular_25px = TTF_OpenFont("Simplon font/otf files/SimplonNorm-Regular.otf", 25);
    font_norm_regular_40px = TTF_OpenFont("Simplon font/otf files/SimplonNorm-Regular.otf", 40);
    font_norm_light_40px = TTF_OpenFont("Simplon font/otf files/SimplonNorm-Light.otf", 40);
    font_norm_bold_56px = TTF_OpenFont("Simplon font/otf files/SimplonNorm-Bold.otf", 56);

    font_norm_bold_62px = TTF_OpenFont("Simplon font/otf files/SimplonNorm-Bold.otf", 62);
    font_norm_bold_42px = TTF_OpenFont("Simplon font/otf files/SimplonNorm-Bold.otf", 42);
    font_norm_regular_30px = TTF_OpenFont("Simplon font/otf files/SimplonNorm-Regular.otf", 30);

    if (
       (gFont24 == nullptr) || (BigFont44 == nullptr) || (font_norm_bold_18px == nullptr) || (font_norm_bold_20px==nullptr) ||
       (font_norm_bold_80px == nullptr) || (font_norm_bold_70px == nullptr) || (font_norm_bold_110px == nullptr) || (VeryBigFont88==nullptr) ||
       (font_norm_regular_20px == nullptr) || (font_norm_regular_50px == nullptr) || (font_norm_medium_16px == nullptr) || (font_norm_regular_35px==nullptr) ||
       (font_norm_regular_25px == nullptr) || (font_norm_regular_40px == nullptr) || (font_norm_light_40px == nullptr) || (font_norm_bold_56px == nullptr) ||
       (font_norm_bold_62px == nullptr) || (font_norm_bold_42px == nullptr) || (font_norm_regular_30px == nullptr)
       )
    {
        printf("ERROR load font\n");
        return -1;
    }
    return 0;
}

int quit_ttf_osasto(void)
{
   if (gFont24 != nullptr) TTF_CloseFont(gFont24);
   if (BigFont44 != nullptr) TTF_CloseFont(BigFont44);
   if (font_norm_bold_18px != nullptr) TTF_CloseFont(font_norm_bold_18px);
   if (font_norm_bold_20px != nullptr) TTF_CloseFont(font_norm_bold_20px);

   if (font_norm_bold_80px != nullptr) TTF_CloseFont(font_norm_bold_80px);
   if (font_norm_bold_70px != nullptr) TTF_CloseFont(font_norm_bold_70px);
   if (font_norm_bold_110px != nullptr) TTF_CloseFont(font_norm_bold_110px);
   if (VeryBigFont88) TTF_CloseFont(VeryBigFont88);

   if (font_norm_regular_20px) TTF_CloseFont(font_norm_regular_20px);
   if (font_norm_regular_50px != nullptr) TTF_CloseFont(font_norm_regular_50px);
   if (font_norm_medium_16px != nullptr) TTF_CloseFont(font_norm_medium_16px);
   if (font_norm_regular_35px != nullptr) TTF_CloseFont(font_norm_regular_35px);

   if (font_norm_regular_25px != nullptr) TTF_CloseFont(font_norm_regular_25px);
   if (font_norm_regular_40px != nullptr) TTF_CloseFont(font_norm_regular_40px);
   if (font_norm_light_40px != nullptr) TTF_CloseFont(font_norm_light_40px);
   if (font_norm_bold_56px != nullptr) TTF_CloseFont(font_norm_bold_56px);

   if (font_norm_bold_62px != nullptr) TTF_CloseFont(font_norm_bold_62px);
   if (font_norm_bold_42px != nullptr) TTF_CloseFont(font_norm_bold_42px);
   if (font_norm_regular_30px != nullptr) TTF_CloseFont(font_norm_regular_30px);

  	TTF_Quit();
	return 0;
}

void render_text(char * str, int x, int y, SDL_Color *color_p, TTF_Font *font_p, int utf8)
{
   SDL_Rect dst;
   SDL_Surface *surf;
   if (!font_p || !color_p)
      return;
   if (utf8)
      surf = TTF_RenderUTF8_Blended(font_p, str, *color_p);
   else
      surf = TTF_RenderText_Blended(font_p, str, *color_p);
	if (surf == nullptr){
		return;
	}
	SDL_Texture *texture = SDL_CreateTextureFromSurface(gRenderer, surf);
	if (texture != nullptr)
   {
   	//Get the texture w/h so we can center it in the screen
	   SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
   	dst.x = x;
	   dst.y = y;
      SDL_RenderCopy(gRenderer, texture, nullptr, &dst);
      SDL_DestroyTexture(texture);
   }
	//Clean up the surface
	SDL_FreeSurface(surf);
}

void render_text_center(char * str, int x, int y, SDL_Color *color_p, TTF_Font *font_p, int utf8)
{
   SDL_Rect dst;
   SDL_Surface *surf;
   if (!font_p || !color_p)
      return;
   if (utf8)
      surf = TTF_RenderUTF8_Blended(font_p, str, *color_p);
   else
      surf = TTF_RenderText_Blended(font_p, str, *color_p);
	if (surf == nullptr){
		return;
	}
	SDL_Texture *texture = SDL_CreateTextureFromSurface(gRenderer, surf);
	if (texture != nullptr)
   {
   	//Get the texture w/h so we can center it in the screen
	   SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
   	dst.x = x - dst.w/2;
	   dst.y = y - dst.h/2;
      SDL_RenderCopy(gRenderer, texture, nullptr, &dst);
      SDL_DestroyTexture(texture);
   }
	//Clean up the surface
	SDL_FreeSurface(surf);
}


void test(void)
{
   printf("\nsizeof int =%d\n", (int)sizeof(int));
   printf("sizeof sint =%d\n", (int)sizeof(signed int));
   printf("sizeof long =%d\n\n", (int)sizeof(long));
}

void debug_show_tilatiedot(void)
{
   char buff[300];
   int i;
   SDL_Color color = { 0, 0, 0, 0x0 };
   strcpy(buff, "Pataosa  I(1-8)=");
   for (i=0; i<8; i++)
   {
      if (input_registers[INPUT_REG_CARD1_DIGITAL_INPUTS] & (1<<i))
         strcat(buff, "1");
      else
         strcat(buff, "0");
   }

   strcat(buff, " O(1-4)=");
   for (i=0; i<4; i++)
   {
      if (output_registers[OUTPUT_REG_CARD1_DIGITAL_OUTPUTS] &= ~(1<<i))
         strcat(buff, "1");
      else
         strcat(buff, "0");
   }

   strcat(buff, " 24V(1-8)=");
   for (; i<12; i++)
   {
      if (output_registers[OUTPUT_REG_CARD1_DIGITAL_OUTPUTS] &= ~(1<<i))
         strcat(buff, "1");
      else
         strcat(buff, "0");
   }
   render_text(buff, 20, 1001, &color, font_norm_regular_20px);

   strcpy(buff, "Pääjalka I(1-8)=");
   for (i=0; i<8; i++)
   {
      if (input_registers[INPUT_REG_CARD2_DIGITAL_INPUTS] & (1<<i))
         strcat(buff, "1");
      else
         strcat(buff, "0");
   }

   strcat(buff, " O(1-4)=");
   for (i=0; i<4; i++)
   {
      if (output_registers[OUTPUT_REG_CARD2_DIGITAL_OUTPUTS] &= ~(1<<i))
         strcat(buff, "1");
      else
         strcat(buff, "0");
   }

   strcat(buff, " 24V(1-8)=");
   for (; i<12; i++)
   {
      if (output_registers[OUTPUT_REG_CARD2_DIGITAL_OUTPUTS] &= ~(1<<i))
         strcat(buff, "1");
      else
         strcat(buff, "0");
   }

//   printf("%s\n", buff);
   render_text(buff, 20, 1021, &color, font_norm_regular_20px);
}
