#ifndef COOK_PROG_H
#define COOK_POG_H

/*======================================================================

Keitto-ohjelman rakenne:

RIVI 1 kentät:
| "KANSIO" | "Ohjelman nimi" | Oma kansio | Vaiheitten lkm | Työkalu | Eräkoko | Kuvaus | jne-

Kansio-sarakkeessa joko "VALMIS1" ... "VALMIS12" tai itsetehdyn kansion nimi, jos VALMIS niin nimi otetaan kieliteksteistä.
Työkalu on indeksi-numero työkaluihin.

Seuraavat RIVIt on vaihe/rivi, 1. sarake kertoo onko vaiheessa muistilappu vai onko Cook/Chill-ohjelmavaihe. Jos lappu niin kertoo onko valmis lappu vai oma tekemä.
Jos on omatekemä niin rivillä sitten muistilapun teksti...entä jos lapussa rivinvaihtoja?

| Cook  | Temp1 | temp2 | Aika | Vesi | LVesi | Sek.tapa | Nopeus |
| Chill | Temp1 | temp2 | Aika | Vesi | LVesi | Sek.tapa | Nopeus |
|  ---  |  ---  |  ---  | Aika | Vesi | LVesi | Sek.tapa | Nopeus |

tai 

| Valmis lappu / Lapun nmo |

tai

| Oma lappu | "Oma tekemän lapun teksti" |

=====================================================================*/

#define STEP_TYPE_COOK_STEP  1 //Kypsennyksen lämpötila optiot 1..9
#define STEP_TYPE_CHILL_STEP 10
#define STEP_TYPE_USER_NOTE  50
#define STEP_TYPE_FIXED_NOTE 100 //100 = eka fixed note jne-

typedef struct
{
   int Step_type;
   int Temp1;
   int Temp2;
   int Aika;
   int Vesi;
   int Lvesi;
   int Sek_tapa;
   int Nopeus;
} cook_prog_step_s;

typedef struct
{
   char * kansio;
   char * ohj_nimi;
   int Oma_kansio; //JOs alle NUM_VALMIS_KANSIOT niin kyseessä valmis-kansio
   int Vaihe_lkm;
   int Tyokalu_idx;
   int Erakoko_kg;
   int Suosikki;
   char * Kuvaus;
} cook_prog_header_s;

#define MAKS_STEPS 10
#define MAKS_LEN 50
#define MAKS_LAPPU_LEN 100

extern cook_prog_header_s tmp_header;
extern int current_prog_step, multi_step_prog_going; //Monivaiheohjelman menossa oleva vaihe
extern char tmp_ohj_nimi[MAKS_LEN];
extern char tmp_kansion_nimi[MAKS_LEN];
extern char tmp_ohj_kuvaus[MAKS_LEN];
extern char tmp_lappu[MAKS_LAPPU_LEN];
extern cook_prog_step_s tmp_steps[MAKS_STEPS];

extern void clear_prog_header(void);
extern void set_prog_header_ohj_nimi(char * nimi);
extern void set_prog_header_kansion_nimi(char * nimi, int Valmiskansio_jos_alle_100);
extern void set_prog_header_kuvaus(char * kuvaus);
extern void set_prog_header_tyokalu_erakoko(int t, int e);

extern void set_step_cook_params(int temp1, int temp2, int aika, cook_prog_step_s * new_step);
extern void set_step_chill_params(int temp1, int temp2, int aika, cook_prog_step_s * new_step);
extern void set_step_water_params(int litrat, int litrat_hot, cook_prog_step_s * new_step);
extern void set_step_mixer_params(int mode, int speed, cook_prog_step_s * new_step);
extern void set_step_lappu_params(int fixed, char *oma, cook_prog_step_s * new_step);

extern void insert_new_step(cook_prog_step_s * new_step);

extern int write_new_prog_file(void);

// -Kansion oikea nimi (esim "VALMIS1") on kirjoitettu -> tmp_kansion_nimi[MAKS_LEN];
//  Ohjelman nimi --> sql_haku[i].haku1 --> tmp_ohj_nimi
extern int read_program_to_memory(void);

extern int load_new_program_step(void); //tmp_steps[current_prog_step] --> prepare_target_params

#define NUM_VALMIS_KANSIOT 12
extern const char *valmis_kansio_nimet[NUM_VALMIS_KANSIOT];

extern void make_valmiskansiot(void);

typedef struct
{
   char haku1[21];
   char haku2[21];
   char haku3[21];
   char haku4[21];
} sql_fields_s;

extern int sql_haku_idx, sqlite_busy;
extern int sqlite_insert_ohjelma(void);
extern int sqlite_hae_ohjelmia(char *kansiosta);
#define SQLITE_HAKU_MAX 20
extern sql_fields_s sql_haku[SQLITE_HAKU_MAX];

#endif
