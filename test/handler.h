/* File: handler.h
   Purpose:
*/
#ifndef HANDLER_H_
#define HANDLER_H

//#define PC_UBUNTU

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

typedef unsigned short int UWORD;

typedef struct menu_funcs_s
{
    void (* init)(void);
    int  (* handler)(int x, int y, SDL_Event *e);
    void (* show)(int show_hide);
    void (* draw)(int param1);
} MENU_FUNCS;

typedef struct menu_struct
{
   UWORD id;
   struct menu_funcs_s funcs;
   int dialog;
} MENU;

extern void create_them(void);
extern void start_graf(void);
extern int handle_code(int x, int y, SDL_Event *e);
extern int current_menu_id, ms_timeout, prev_menu_id, menu_to_return;

#endif
