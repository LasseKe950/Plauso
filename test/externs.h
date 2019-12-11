#ifndef EXTERNS_H
#define EXTERNS_H

//Using SDL, SDL_image, standard IO, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

//#define KORTIT_KYTKETTY

extern TTF_Font *gFont24;
extern TTF_Font *BigFont44;
extern TTF_Font *VeryBigFont88;
extern TTF_Font *font_norm_bold_18px;
extern TTF_Font *font_norm_bold_20px;
extern TTF_Font *font_norm_bold_42px;
extern TTF_Font *font_norm_bold_56px;
extern TTF_Font *font_norm_bold_62px;
extern TTF_Font *font_norm_bold_70px;
extern TTF_Font *font_norm_bold_80px;
extern TTF_Font *font_norm_regular_20px;
extern TTF_Font *font_norm_regular_25px;
extern TTF_Font *font_norm_regular_30px;
extern TTF_Font *font_norm_regular_35px;
extern TTF_Font *font_norm_regular_40px;
extern TTF_Font *font_norm_regular_50px;
extern TTF_Font *font_norm_bold_110px;
extern TTF_Font *font_norm_medium_16px;
extern TTF_Font *font_norm_light_40px;

//The window renderer
extern SDL_Renderer* gRenderer;
extern SDL_Rect gScreenRect;

extern int take_screen_shot_now;
extern void take_screen_shot(void);
extern void render_taken_screen_shot(void);
extern void release_screen_shot_texture(void);

//kallistus_nap.cpp
extern void nayta_alavalikko(int but_idx, int button_pressed);
extern void piilota_alavalikko(int but_idx);
extern int alavalikko_naytolla, esta_alavalikko_naytolle;

//yla_kuvat.cpp
extern void nayta_ylavalikko(int but_idx);
extern void piilota_ylavalikko(int but_idx);
extern int ylavalikko_naytolla;

//kotinakyma.cpp
extern int check_ylavalikko_valinta(int code);
extern void show_banner(int but_idx);
extern void show_ala_kahva(void);
extern void show_alempi_kahva(void);
extern int check_for_alavalikko(int x, int y, int but_idx); //Jos oli pyyhk.toiminto, niin alavalikko esiin/piiloon
extern int check_for_ylavalikko(int x, int y, int but_idx); //Jos oli pyyhk.toiminto, niin alavalikko esiin/piiloon
extern int button_pressed, wait_finger_up;
extern int prog_curr_hours, prog_curr_minutes; //bannerissa oleva aika hh:mm, hh=00..23

//kypsennys.cpp
extern void check_if_any_value_changed(void);
extern void cooking_is_ready(int hold_temper); //1=hold temperature
extern void nayta_aloita_painike(void);
extern void update_const_buttons_text(void); //tulostaa miinukset ja plussat -> helper_text_buffers jotka tekstit sitten laitetaan kyps/j‰‰hd buttoneihin

extern char helper_text_buffers[30][30];
extern int nappi_pohjassa, ala_painike_painettu;
extern int temp_reach_indicator;
extern void check_temp_reach_ind_time(void);
extern int sec_ctr;
extern int show_numpad;

//j‰‰hdytys.cpp
extern void vesi_miinus_painettu(void);
extern void vesi_plus_painettu(void);
extern void mix_plus_painettu(void);
extern void mix_miinus_painettu(void);
extern void chilling_is_ready(int hold_temp); //1=hold chill temperature

//ok_cancel.cpp
extern void set_ok_cancel_buttons(int ok_tausta); //cancel=0, OK=1
extern void set_ok_cancel_buttons2(int ok_tausta); //cancel=0, OK=1
extern void set_ok_cancel_text(int txt_group, int txt_index); //teksti mik‰ n‰ytet‰‰n ruudulla

//prof_vaihto.cpp
extern int show_dropdown, dropdown_selection[5];
extern int original_selection[5];
extern void prof_salasana_annettiin(void); //numpad_pieni kutsuu kun salasana on annettu ja se on numpad_string:iss‰

//uusi_ohjelma.cpp
extern void uusi_ohjelma_erakoko_annettiin(void);
extern void uusi_ohjelma_nimi_annetaan(void);

//syota_aloita_tiedot.cpp
extern void syota_aloita_tiedot_annettiin(void);
extern int tuotanto_maara, keitto_eran_nmo;

//numpad.cpp
extern char numpad_string[20];
//keypad.cpp
extern char keypad_string[100];

#define CODE_KYPSENNYS 100
#define CODE_JAAHDYTYS 101
#define CODE_SUOSIKIT  102
#define CODE_PUHDISTUS 103
#define CODE_OHJELMAT  104
#define CODE_YLLAPITO  105

#define CODE_DIETA_LOGO 150
#define CODE_ISO_ALUE_PAINETTU 151

#define CODE_TEMP_LUKEMA     200
#define CODE_AIKA_LUKEMA     201
#define CODE_VESI_LUKEMA     202
#define CODE_MIXSPEED_LUKEMA 203

#define CODE_TEMP_MIINUS     210
#define CODE_TEMP_PLUS       211
#define CODE_AIKA_MIINUS     212
#define CODE_AIKA_PLUS       213
#define CODE_VESI_MIINUS     214
#define CODE_VESI_PLUS       215
#define CODE_MIXSPEED_MIINUS 216
#define CODE_MIXSPEED_PLUS   217

#define CODE_KALLISTUS_ALAS  300
#define CODE_KALLISTUS_PULSE 301
#define CODE_KALLISTUS_YLOS  302

#define CODE_YLAPALKKI_HACCP    400
#define CODE_YLAPALKKI_MUNAK    401
#define CODE_YLAPALKKI_VAAKA    402
#define CODE_YLAPALKKI_SAMMUTA  403
#define CODE_YLAPALKKI_PROFIILI 404
#define CODE_YLAPALKKI_BT       405

#define DROPDOWN_EVENT_CODE1 500

#define MSG_TYPE_1SEC_MSG            600
#define MSG_TYPE_MIXER_PROG_CHG_MSG  601
#define MSG_TYPE_MIXER_SPEED_CHG_MSG 602

//kyps_icons.cpp
extern void draw_kyps_jaahd_cook_icons(void);
extern void draw_kyps_jaahd_chill_icons(void);
extern void draw_kyps_jaahd_time_icons(void);
extern void draw_kyps_jaahd_water_icons(void);
extern void draw_kyps_jaahd_mixer_icons(void);
extern void show_sidebar(void);

//sek_tapa_valinta.cpp
extern char * get_sekoitus_tapa_txt(void);
extern int MixerTask(int msg); //mixer.cpp

//errors
#define NUM_OF_CAN_MODULES 2
extern int can_comm_error[5];
extern int can_comm_err_counter[5];
extern int check_any_errors(void); //handler.cpp
extern void clear_all_errors(void);//handler.cpp

//ok_cancel.cpp
typedef struct ok_cancel_OK_funcs_s
{
    void (* ok_pressed)(int param1);
} OK_CANCEL_OK_FUNCS;

#define OK_CANCEL_NO_OK_FUNC  0
#define OK_CANCEL_OK_TEST2    1
#define OK_CANCEL_OK_TEST3    2
#define OK_CANCEL_OK_TEST4    3
#define OK_CANCEL_OK_POWEROFF 4
#define NUM_OK_CANCEL_OK_FUNCTIONS 5

extern void no_ok_func(int par1);
extern void test2_func(int par1);
extern void test3_func(int par1);
extern void test4_func(int par1);
extern void power_off_func(int par1);

extern int ok_cancel_ok_func_idx;

extern const ok_cancel_OK_funcs_s ok_funcs[NUM_OK_CANCEL_OK_FUNCTIONS];

//SDL_main.cpp
extern const int prog_ver_major;
extern const int prog_ver_minor;
//update_thread.cpp
extern char Prog_update_buffer1[100];
extern char Prog_update_buffer2[100];
extern int Prog_update_going;
//vaiheiden_luonti.cpp
extern int uuden_Vaiheen_teko;
extern void show_ohj_nimi(void);
extern void show_header_teksti(void);
extern void show_ohj_header(int but_idx);
extern void show_nayta_vaiheet_buttons(int but_idx); //N‰ytt‰‰ vaihe-buttonit allekkain, jos ollaan editoimassa vaihetta n‰ytt‰‰ yhden (editoitavan vaiheen) buttonin ruudun yl‰osassa
extern void show_nayta_vaiheet_buttons_icons(void);
extern void show_nayta_vaiheen_edit_icons(void);

#endif
