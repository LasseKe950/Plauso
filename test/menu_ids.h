//#ifndef MENU_ID
//#define MENU_ID

/*extern void init_ALKU(void);
extern int  handler_ALKU(int x, int y, SDL_Event *e);
extern void show_ALKU(int show_hide);*/

extern void draw_LAUNCH(int param1);
extern void init_LAUNCH(void);
extern int  handler_LAUNCH(int x, int y, SDL_Event *e);
extern void show_LAUNCH(int show_hide);

extern void draw_KOTINAKYMA(int param1);
extern void init_KOTINAKYMA(void);
extern int  handler_KOTINAKYMA(int x, int y, SDL_Event *e);
extern void show_KOTINAKYMA(int show_hide);

extern void draw_KYPSENNYS(int param1);
extern void init_KYPSENNYS(void);
extern int  handler_KYPSENNYS(int x, int y, SDL_Event *e);
extern void show_KYPSENNYS(int show_hide);

extern void draw_JAAHDYTYS(int param1);
extern void init_JAAHDYTYS(void);
extern int  handler_JAAHDYTYS(int x, int y, SDL_Event *e);
extern void show_JAAHDYTYS(int show_hide);

extern void draw_NUMPAD(int param1);
extern void init_NUMPAD(void);
extern int  handler_NUMPAD(int x, int y, SDL_Event *e);
extern void show_NUMPAD(int show_hide);

extern void draw_NUMPAD_PIENI(int param1);
extern void init_NUMPAD_PIENI(void);
extern int  handler_NUMPAD_PIENI(int x, int y, SDL_Event *e);
extern void show_NUMPAD_PIENI(int show_hide);

extern void draw_SEK_TAPA_VALINTA(int param1);
extern void init_SEK_TAPA_VALINTA(void);
extern int  handler_SEK_TAPA_VALINTA(int x, int y, SDL_Event *e);
extern void show_SEK_TAPA_VALINTA(int show_hide);

extern void draw_IOTEST(int param1);
extern void init_IOTEST(void);
extern int  handler_IOTEST(int x, int y, SDL_Event *e);
extern void show_IOTEST(int show_hide);

extern void draw_VALMIS(int param1);
extern void init_VALMIS(void);
extern int  handler_VALMIS(int x, int y, SDL_Event *e);
extern void show_VALMIS(int show_hide);

extern void draw_OK_CANCEL(int param1);
extern void init_OK_CANCEL(void);
extern int  handler_OK_CANCEL(int x, int y, SDL_Event *e);
extern void show_OK_CANCEL(int show_hide);

extern void draw_PROF_VAIHTO(int param1);
extern void init_PROF_VAIHTO(void);
extern int  handler_PROF_VAIHTO(int x, int y, SDL_Event *e);
extern void show_PROF_VAIHTO(int show_hide);

extern void draw_OHJELMAT(int param1);
extern void init_OHJELMAT(void);
extern int  handler_OHJELMAT(int x, int y, SDL_Event *e);
extern void show_OHJELMAT(int show_hide);

extern void draw_OMAT_OHJELMAT(int param1);
extern void init_OMAT_OHJELMAT(void);
extern int  handler_OMAT_OHJELMAT(int x, int y, SDL_Event *e);
extern void show_OMAT_OHJELMAT(int show_hide);

extern void draw_UUSI_OHJELMA(int param1);
extern void init_UUSI_OHJELMA(void);
extern int  handler_UUSI_OHJELMA(int x, int y, SDL_Event *e);
extern void show_UUSI_OHJELMA(int show_hide);

extern void draw_KEYPAD(int param1);
extern void init_KEYPAD(void);
extern int  handler_KEYPAD(int x, int y, SDL_Event *e);
extern void show_KEYPAD(int show_hide);

extern void draw_VAIHEIDEN_LUONTI(int param1);
extern void init_VAIHEIDEN_LUONTI(void);
extern int  handler_VAIHEIDEN_LUONTI(int x, int y, SDL_Event *e);
extern void show_VAIHEIDEN_LUONTI(int show_hide);

extern void draw_LISTAA_KANSIO(int param1);
extern void init_LISTAA_KANSIO(void);
extern int  handler_LISTAA_KANSIO(int x, int y, SDL_Event *e);
extern void show_LISTAA_KANSIO(int show_hide);

extern void draw_NAYTA_OHJ_VAIHEET(int param1);
extern void init_NAYTA_OHJ_VAIHEET(void);
extern int  handler_NAYTA_OHJ_VAIHEET(int x, int y, SDL_Event *e);
extern void show_NAYTA_OHJ_VAIHEET(int show_hide);

extern void draw_SYOTA_ALOITA_TIEDOT(int param1);
extern void init_SYOTA_ALOITA_TIEDOT(void);
extern int  handler_SYOTA_ALOITA_TIEDOT(int x, int y, SDL_Event *e);
extern void show_SYOTA_ALOITA_TIEDOT(int show_hide);

enum menu_id
{
   ID_LAUNCH,
   ID_KOTINAKYMA,
   ID_KYPSENNYS,
   ID_JAAHDYTYS,
   ID_NUMPAD,
   ID_NUMPAD_PIENI,
   ID_SEK_TAPA_VALINTA,
   ID_IOTEST,
   ID_VALMIS,
   ID_OK_CANCEL,
   ID_PROF_VAIHTO,
   ID_OHJELMAT,
   ID_OMAT_OHJELMAT,
   ID_UUSI_OHJELMA,
   ID_VAIHEIDEN_LUONTI,
   ID_LISTAA_KANSIO,
   ID_KEYPAD,
   ID_NAYTA_OHJ_VAIHEET,
   ID_SYOTA_ALOITA_TIEDOT,
   NUM_MENU_IDS
};

//#endif
