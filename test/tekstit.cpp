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
"J��HDYTYS",
"SUOSIKIT",
"PESU",
"OHJELMAT",
"LIS�TOIMINNOT",
"YLL�PITO",
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
"K�ytt�j�profiili",
"Salasana",
"Perusk�ytt�j�",
"Superk�ytt�j�",
"Huoltok�ytt�j�",
"LIS�� UUSI KANSIO",
"Sijainti",
"Valitse sijainti",
"Ohjelman nimi",
"Ty�kalu",
"Reseptin kuvaus",
"Arvioitu valmistumisaika",
"Kokonaism��r�",
"Valmistuser�n koko",
"Tuotantom��r� (kg)",
"Keittoer�n numero",
"UUSI VAIHE",
"K�ytetty eniten",
"K�ytetty viimeksi",
"Aakkosj�rjestys",
"vaihetta",
"L�ytyi uusi versio",
"Ei tarvetta p�ivitykseen",
"P�ivitys tehty, uudelleenk�ynnist�!",
};

const char * sekoitustapa_tekstit[NUM_SEKOITUSTAPA_TEKSTIT]
{
"Kahteen suuntaan hidas",
"Yhteen suuntaan",
"Kahteen suuntaan",
"Yhteen suuntaan tauko",
"Kahteen suuntaan nopea",
"Yhteen suuntaan pitk� tauko",
"Kahteen suuntaan tauko",
"Oma sekoitustapa",
"Sekoitustapa ja -nopeus",
};

const char * ruoka_valmis_tekstit[NUM_RUOKA_VALMIS_TEKSTIT]
{
"RUOKA ON VALMIS!",
"Resepti",
"Er�numero",
"Reseptin saanto",
"Saanto",
"Paino",
"Ohjelman kokonaisaika",
"Kokonaisaika",
"J��hdytysaika",
"Kokonaisvesim��r�",
"Energian kulutus",
};

const char * kansio_nimet_tekstit[NUM_KANSIO_NIMET_TEKSTIT]
{
"KEITOT",
"L�MPIM�T KASTIKKEET",
"KYLM�T KASTIKKEET",
"PUUROT",
"RIISI",
"PASTA",
"PATARUOAT",
"PERUNA",
"J�LKIRUOAT",
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
"H�t�seis painettu!",
"Kontaktorivika!",
"Painekytkin lauennut!",
"Ylikuumenemissuoja lauennut!",
};

int errors_paalla[NUM_ERROR_INDEX];

const char * tyokalut_tekstit[NUM_TYOKALUT_TEKSTIT]
{
"Yleisty�kalu",
"Vispausritil� ja pohjakaavin",
"Massakoukku ja pohjakaavin",
"Massakoukku",
};
