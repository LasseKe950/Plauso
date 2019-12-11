/* File: tekstit.cpp
   Purpose: variables
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "tekstit.h"

const char * yleis_tekstit[NUM_YLEIS_TEKSTIT] = {
"ALOITA",
"LOPETA",
"VALMIS",
"KYPSENNYS",
"JÄÄHDYTYS",
"SUOSIKIT",
"PESU",
"OHJELMAT",
"LISÄTOIMINNOT",
"YLLÄPITO",
"OMAT OHJELMAT",
"AUTOCOOK",
"HACCP",
"JATKA/PALAA",
"TALLENNA",
"Haluatko lopettaa ohjelman?",
"Haluatko sammuttaa padan?",
"TAKAISIN",
"UUSI OHJELMA",
"OK",
"Käyttäjäprofiili",
"Salasana",
"Peruskäyttäjä",
"Superkäyttäjä",
"Huoltokäyttäjä",
"LISÄÄ UUSI KANSIO",
"Sijainti",
"Valitse sijainti",
"Ohjelman nimi",
"Työkalu",
"Reseptin kuvaus",
"Arvioitu valmistumisaika",
"Kokonaismäärä",
"Valmistuserän koko",
"Tuotantomäärä (kg)",
"Keittoerän numero",
"UUSI VAIHE",
"Käytetty eniten",
"Käytetty viimeksi",
"Aakkosjärjestys",
"vaihetta",
"Löytyi uusi versio",
"Ei tarvetta päivitykseen",
"Päivitys tehty, uudelleenkäynnistä!",
};

const char * sekoitustapa_tekstit[NUM_SEKOITUSTAPA_TEKSTIT]
{
"Kahteen suuntaan hidas",
"Yhteen suuntaan",
"Kahteen suuntaan",
"Yhteen suuntaan tauko",
"Kahteen suuntaan nopea",
"Yhteen suuntaan pitkä tauko",
"Kahteen suuntaan tauko",
"Oma sekoitustapa",
"Sekoitustapa ja -nopeus",
};

const char * ruoka_valmis_tekstit[NUM_RUOKA_VALMIS_TEKSTIT]
{
"RUOKA ON VALMIS!",
"Resepti",
"Eränumero",
"Reseptin saanto",
"Saanto",
"Paino",
"Ohjelman kokonaisaika",
"Kokonaisaika",
"Jäähdytysaika",
"Kokonaisvesimäärä",
"Energian kulutus",
};

const char * kansio_nimet_tekstit[NUM_KANSIO_NIMET_TEKSTIT]
{
"KEITOT",
"LÄMPIMÄT KASTIKKEET",
"KYLMÄT KASTIKKEET",
"PUUROT",
"RIISI",
"PASTA",
"PATARUOAT",
"PERUNA",
"JÄLKIRUOAT",
"VANUKKAAT",
"SEOKSET",
"PESUOHJELMAT",
};

const char * error_tekstit[NUM_ERROR_INDEX]
{
"CAN KOMMUNIKAATIOVIKA!",
"Ruoka-anturi katkos",
"Vaippa-anturi katkos",
"Ruoka-anturi oikosulku",
"Vaippa-anturi oikosulku",
"Vesimittari vika!",
"Hätäseis painettu!",
"Kontaktorivika!",
"Painekytkin lauennut!",
"Ylikuumenemissuoja lauennut!",
};

int errors_paalla[NUM_ERROR_INDEX];

const char * tyokalut_tekstit[NUM_TYOKALUT_TEKSTIT]
{
"Yleistyökalu",
"Vispausritilä ja pohjakaavin",
"Massakoukku ja pohjakaavin",
"Massakoukku",
};
