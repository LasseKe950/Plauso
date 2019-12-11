/* File: handler.h
   Purpose:
*/

typedef unsigned short int UWORD;

typedef struct menu_funcs_s
{
    void (* init)(void);
    int  (* handler)(int x, int y, int code, int wid);
    void (* show)(int show_hide);
} MENU_FUNCS;

typedef struct menu_struct
{
   UWORD id;
   struct menu_funcs_s funcs;
} MENU;

extern void create_them(void);
extern void start_graf(void);
extern int handle_code(int x, int y, UWORD code, int wid);

