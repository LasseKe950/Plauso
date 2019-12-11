/* 
   Button_defs.cpp
   */
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <stdio.h>
#include <string>
#include <stdlib.h>

#include "menu_ids.h"
#include "handler.h"
#include "button_defs.h"
#include "image.h"

Image * image_ptrs[NUM_LOADED_IMAGES];

void load_all_images(void)
{
   image_ptrs[IMAGE_DIETA_MX500] = new Image("img/MX500 W.png");

   image_ptrs[IMAGE_DIETA_LOGO] = new Image("img/Ylabanneri/Painike etusivulle B.png");
   image_ptrs[IMAGE_MENU_BUTTON_TAUSTA] = new Image("Paavalikot/Päävalikon Iso.png");
   image_ptrs[IMAGE_MENU_BUTTON_POHJASSA_TAUSTA] = new Image("Paavalikot/Päävalikon Iso Pohjassa.png");
   image_ptrs[IMAGE_MENU_BUTTON_SMALL_TAUSTA] = new Image("Paavalikot/Omat Ohjelmat.png");
   image_ptrs[IMAGE_MENU_BUTTON_SMALL_POHJASSA_TAUSTA] = new Image("Paavalikot/Omat Ohjelmat Pohjassa.png");

   image_ptrs[IMAGE_KYPSENNYS_111px] = new Image("kotinakyma_ikonit/Keitto W.png");
   image_ptrs[IMAGE_JAAHDYTYS_111px] = new Image("kotinakyma_ikonit/Lampotila W.png");
   image_ptrs[IMAGE_SUOSIKIT_111px] = new Image("kotinakyma_ikonit/Suosikit W.png");
   image_ptrs[IMAGE_PUHDISTUS_111px] = new Image("kotinakyma_ikonit/Puhdistus W.png");
   image_ptrs[IMAGE_OHJELMAT_111px] = new Image("kotinakyma_ikonit/Ohjelmat W.png");
   image_ptrs[IMAGE_Ekstrat_111px] = new Image("kotinakyma_ikonit/Ekstrat W.png");
   image_ptrs[IMAGE_OMAT_OHJELMAT_111px] = new Image("img/110px/Omat Ohjelmat W.png");
   image_ptrs[IMAGE_AUTOCOOK_111px] = new Image("img/110px/Autocook W.png");

   image_ptrs[IMAGE_KYPS_JA_JAAH_HARMAA] = new Image("Kyps_ja_jaah/Harmaa.png");
   image_ptrs[IMAGE_KYPS_JA_JAAH_HARMAA_VASEN_POHJASSA] = new Image("Kyps_ja_jaah/Harmaa Vasen Pohjassa.png");
   image_ptrs[IMAGE_KYPS_JA_JAAH_HARMAA_OIKEA_POHJASSA] = new Image("Kyps_ja_jaah/Harmaa Oikea Pohjassa.png");
   image_ptrs[IMAGE_KYPS_JA_JAAH_HARMAA_TAYSIN_POHJASSA] = new Image("Kyps_ja_jaah/Harmaa Taysin Pohjassa.png");

   image_ptrs[IMAGE_KYPS_JA_JAAH_PUNAINEN] = new Image("Kyps_ja_jaah/Punainen.png");
   image_ptrs[IMAGE_KYPS_JA_JAAH_SININEN] = new Image("Kyps_ja_jaah/Sininen.png");

   image_ptrs[IMAGE_VIHREA_LYHYT] = new Image("vakiot/Vihreä Lyhyt .png");
   image_ptrs[IMAGE_VIHREA_LYHYT_POHJASSA] = new Image("vakiot/Vihreä Lyhyt Pohjassa.png");
   image_ptrs[IMAGE_VIHREA_PITKA] = new Image("vakiot/Vihreä Pitkä.png");
   image_ptrs[IMAGE_VIHREA_PITKA_POHJASSA] = new Image("vakiot/Vihreä Pitkä Pohjassa.png");
   image_ptrs[IMAGE_PUNAINEN_PITKA] = new Image("vakiot/Punainen Pitkä.png");
   image_ptrs[IMAGE_PUNAINEN_PITKA_POHJASSA] = new Image("vakiot/Punainen Pitkä Pohjassa.png");
   image_ptrs[IMAGE_ASETUKSET_YLAPAINIKE_HARMAA] = new Image("vakiot/Tiedonsiirto ja Asetukset Yläpainike Harmaa.png");
   image_ptrs[IMAGE_ASETUKSET_YLAPAINIKE_HARMAA_POHJASSA] = new Image("vakiot/Tiedonsiirto ja Asetukset Yläpainike Harmaa Pohjassa.png");
   image_ptrs[IMAGE_ASETUKSET_YLAPAINIKE_VALKOINEN] = new Image("vakiot/Tiedonsiirto ja Asetukset Yläpainike Valkoinen.png");
   image_ptrs[IMAGE_ASETUKSET_YLAPAINIKE_VALKOINEN_POHJASSA] = new Image("vakiot/Tiedonsiirto ja Asetukset Yläpainike Valkoinen Pohjassa.png");

   image_ptrs[IMAGE_HARMAA] = new Image("vakiot/Harmaa .png");
   image_ptrs[IMAGE_HARMAA_POHJASSA] = new Image("vakiot/Harmaa Pohjassa.png");
   image_ptrs[IMAGE_VALKOINEN_VIHREAT_REUNAT] = new Image("vakiot/Valkoinen Vihreät Reunat.png");
   image_ptrs[IMAGE_VALKOINEN_VIHREAT_REUNAT_POHJASSA] = new Image("vakiot/Valkoinen Vihreät Reunat Pohjassa.png");
   image_ptrs[IMAGE_PUNAINEN] = new Image("vakiot/Punainen Lyhyt.png");

   image_ptrs[IMAGE_PIENI_NUMERO_POHJA] = new Image("NumeronSyotto/Numero Pohja-1.png");
   image_ptrs[IMAGE_PIENI_NUMERO_POHJA_POHJASSA] = new Image("NumeronSyotto/Numero Pohja Pohjassa-1.png");
   image_ptrs[IMAGE_PIENI_BACKSPACE] = new Image("NumeronSyotto/Backspace.png");
   image_ptrs[IMAGE_PIENI_BACKSPACE_POHJASSA] = new Image("NumeronSyotto/Backspace Pohjassa.png");
   image_ptrs[IMAGE_PIENI_OK_1] = new Image("NumeronSyotto/OK-1.png");

   image_ptrs[IMAGE_NUMERO_POHJA] = new Image("numpadpainikkeet2/Numero Pohja.png");
   image_ptrs[IMAGE_NUMERO_POHJA_POHJASSA] = new Image("numpadpainikkeet2/Numero Pohja Pohjassa.png");
   image_ptrs[IMAGE_BACKSPACE] = new Image("numpadpainikkeet2/Backspace Painike.png");
   image_ptrs[IMAGE_BACKSPACE_POHJASSA] = new Image("numpadpainikkeet2/Backspace Painike Pohjassa.png");
   image_ptrs[IMAGE_OK_1] = new Image("numpadpainikkeet2/OK.png");

   image_ptrs[IMAGE_KIRJAIN_POHJA] = new Image("keypad/Kirjainpohja Norm.png");
   image_ptrs[IMAGE_KIRJAIN_POHJA_POHJASSA] = new Image("keypad/Kirjainpohja Norm Pohjassa.png");
   image_ptrs[IMAGE_KIRJAIN_BACKSPACE] = new Image("keypad/Backspace .png");
   image_ptrs[IMAGE_KIRJAIN_BACKSPACE_POHJASSA] = new Image("keypad/Backspace Pohjassa.png");
   image_ptrs[IMAGE_KIRJAIN_SPACE] = new Image("keypad/Space.png");
   image_ptrs[IMAGE_KIRJAIN_SPACE_POHJASSA] = new Image("keypad/Space Pohjassa.png");

   image_ptrs[IMAGE_KIRJAIN_OK] = new Image("keypad/OK.png");
   image_ptrs[IMAGE_MUSTA_RETURN_32px] = new Image("img/32px/Return B.png");
   image_ptrs[IMAGE_NUOLI_ALAS_32px] = new Image("img/32px/Nuoli Alas B.png");

   image_ptrs[IMAGE_ALAVALIKKO_POHJA] = new Image("Alavalikko/Buttonit/Tilting panel Vakio.png");
   image_ptrs[IMAGE_ALAVALIKKO_POHJA_PAINETTU] = new Image("Alavalikko/Buttonit/Tilting panel Pohjassa.png");
   image_ptrs[IMAGE_ALAVALIKKO_TOIMINTO1] = new Image("Alavalikko/Ikonit/Tilt Down B.png");
   image_ptrs[IMAGE_ALAVALIKKO_TOIMINTO2] = new Image("Alavalikko/Ikonit/Turvasekoitus B.png");
   image_ptrs[IMAGE_ALAVALIKKO_TOIMINTO3] = new Image("Alavalikko/Ikonit/Tilt Up B.png");

   image_ptrs[IMAGE_MUSTA_LIEKKI_58px] = new Image("img/58px/Lammitys B.png");
   image_ptrs[IMAGE_VALKOINEN_AJASTUS_58px] = new Image("img/58px/Ajastus W.png");
   image_ptrs[IMAGE_VALKOINEN_SEKOITUS_58px] = new Image("img/58px/Sekoitus W.png");
   image_ptrs[IMAGE_SININEN_HANA_58px] = new Image("img/58px/Hana S.png");
   image_ptrs[IMAGE_VALKOINEN_PYRAMIDI_58px] = new Image("img/58px/Pyramidi W.png");
   image_ptrs[IMAGE_VALKOINEN_PYRAMIDI_NURIN_58px] = new Image("img/58px/Pyramidi Nurin W.png");
   image_ptrs[IMAGE_PLAY_58px] = new Image("img/58px/Play W.png");
   image_ptrs[IMAGE_PAUSE_58px] = new Image("img/58px/Pause W.png");
   image_ptrs[IMAGE_MUSTA_AJASTUS_58px] = new Image("img/58px/Ajastus B.png");
   image_ptrs[IMAGE_MUSTA_PISARA_58px] = new Image("img/58px/Pisara B.png");
   image_ptrs[IMAGE_MUSTA_SEKOITUS_58px] = new Image("img/58px/Sekoitus B.png");

   image_ptrs[IMAGE_PUNAINEN_LIEKKI_76px] = new Image("img/76px/Lammitys P.png");
   image_ptrs[IMAGE_SININEN_JAAHDYTYS_76px] = new Image("img/76px/Jaahdytys S.png");
   image_ptrs[IMAGE_VALKOINEN_JAAHDYTYS_76px] = new Image("img/76px/Jaahdytys W.png");
   image_ptrs[IMAGE_VALKOINEN_AJASTUS_76px] = new Image("img/76px/Ajastus W.png");
   image_ptrs[IMAGE_VALKOINEN_SEKOITUS_76px] = new Image("img/76px/Sekoitus W.png");
   image_ptrs[IMAGE_SININEN_HANA_76px] = new Image("img/76px/Hana S.png");
   image_ptrs[IMAGE_VALKOINEN_PYRAMIDI_76px] = new Image("img/76px/Pyramidi W.png");
   image_ptrs[IMAGE_VALKOINEN_RETURN_76px] = new Image("img/76px/Return W.png");
   image_ptrs[IMAGE_MUISTIINPANOT_B_76px] = new Image("img/76px/Muistiinpanot B.png");

   image_ptrs[IMAGE_TEMP_VALINTA_LISATOIMINTO] = new Image("temp_sel_lisat/Lisatoiminto.png");
   image_ptrs[IMAGE_TEMP_VALINTA_LISATOIMINTO_POHJASSA] = new Image("temp_sel_lisat/Lisatoiminnot Pohjassa.png");
   image_ptrs[IMAGE_TEMP_VALINTA_LISATOIMINTO_VALITTU] = new Image("temp_sel_lisat/Lisatoiminnot Valittu.png");
   image_ptrs[IMAGE_TEMP_VALINTA_LISATOIMINTO_VALITTU_POHJASSA] = new Image("temp_sel_lisat/Lisatoiminnot Valittu Pohjassa.png");

   image_ptrs[IMAGE_RUOKA_LAMPO_IKONI_B_45px] = new Image("img/45px/Artwork 1 B.png"); //Ruoan lämpötila ikoni White
   image_ptrs[IMAGE_RUOKA_LAMPO_IKONI_W_45px] = new Image("img/45px/Artwork 1 W.png"); //Ruoan lämpötila ikoni White
   image_ptrs[IMAGE_VAIPPA_LAMPO_IKONI_B_45px] = new Image("img/45px/Artwork 4 B.png");//VAipan lämpötila ikoni White
   image_ptrs[IMAGE_VAIPPA_LAMPO_IKONI_W_45px] = new Image("img/45px/Artwork 4 W.png");//VAipan lämpötila ikoni White
   image_ptrs[IMAGE_DELTA_LAMPO_IKONI_B_45px] = new Image("img/45px/Artwork 3 B.png");
   image_ptrs[IMAGE_DELTA_LAMPO_IKONI_W_45px] = new Image("img/45px/Artwork 3 W.png");
   image_ptrs[IMAGE_SENSOR_LAMPO_IKONI_B_45px] = new Image("img/45px/Artwork 2 B.png");
   image_ptrs[IMAGE_SENSOR_LAMPO_IKONI_W_45px] = new Image("img/45px/Artwork 2 W.png");

   image_ptrs[IMAGE_SEKOITUSTAPA_VAALEA_BTN] = new Image("Sekoitustapa/Sekoitustapa Vaalea.png");
   image_ptrs[IMAGE_SEKOITUSTAPA_VAALEA_BTN] = new Image("Sekoitustapa/Sekoitustapa Vaalea Pohjassa.png");
   image_ptrs[IMAGE_SEKOITUSTAPA_VAALEA_VALITTU_BTN] = new Image("Sekoitustapa/Sekoitustapa Vaalea Valittu.png");

   image_ptrs[IMAGE_SEKOITUSTAPA_KUVA_YHTEEN_SUUNTAAN] = new Image("Sekoitustapa/Yhteen suuntaan sekoitus.png");
   image_ptrs[IMAGE_SEKOITUSTAPA_KUVA_KAHTEEN_SUUNTAAN] = new Image("Sekoitustapa/Kahteen suuntaan.png");
   image_ptrs[IMAGE_SEKOITUSTAPA_KUVA_KAHTEEN_SUUNTAAN_HIDAS] = new Image("Sekoitustapa/Kahteen suuntaan hidas.png");
   image_ptrs[IMAGE_SEKOITUSTAPA_KUVA_KAHTEEN_SUUNTAAN_NOPEA] = new Image("Sekoitustapa/Kahteen suuntaan nopea.png"); //Kahteen suuntaan tauko puuttuu?!?

   image_ptrs[IMAGE_YLAPALKKI_KUVAT_TOIMINTO1] = new Image("Dropdown/HACCP B.png");
   image_ptrs[IMAGE_YLAPALKKI_KUVAT_TOIMINTO2] = new Image("Dropdown/Ajastus B.png");
   image_ptrs[IMAGE_YLAPALKKI_KUVAT_TOIMINTO3] = new Image("Dropdown/Vaaka B.png");
   image_ptrs[IMAGE_YLAPALKKI_KUVAT_TOIMINTO4] = new Image("Dropdown/Turn Off B.png");
   image_ptrs[IMAGE_YLAPALKKI_KUVAT_TOIMINTO5] = new Image("Dropdown/Profiili B.png");
   image_ptrs[IMAGE_YLAPALKKI_KUVAT_TOIMINTO6] = new Image("Dropdown/Bluetooth B.png");

   image_ptrs[IMAGE_LIPPU_FIN] = new Image("Liput/fin.png");
   image_ptrs[IMAGE_LIPPU_ENG] = new Image("Liput/eng.png");
   image_ptrs[IMAGE_LIPPU_SWE] = new Image("Liput/swe.png");

   image_ptrs[IMAGE_RESEPTIN_MUOKKAUS_PAINIKE] = new Image("Reseptin_muokkaus/Reseptin Muokkaus Painike.png");
   image_ptrs[IMAGE_RESEPTIN_MUOKKAUS_PAINIKE_POHJASSA] = new Image("Reseptin_muokkaus/Reseptin Muokkaus Painike Pohjassa.png");

   image_ptrs[IMAGE_SIVUPALKKI_FULL_BASE] = new Image("sivupalkki/Pohjat/Full Base.png");
   image_ptrs[IMAGE_SIVUPALKKI_COMB_FS] = new Image("sivupalkki/Pohjat/Combination FS.png");
   image_ptrs[IMAGE_SIVUPALKKI_FOUR_SQ] = new Image("sivupalkki/Pohjat/four squares.png");

   image_ptrs[IMAGE_SIVUPALKKI_ICON_COOK] = new Image("sivupalkki/Ikonit/Warm up B.png");
   image_ptrs[IMAGE_SIVUPALKKI_ICON_CHILL] = new Image("sivupalkki/Ikonit/Cool down B.png");
   image_ptrs[IMAGE_SIVUPALKKI_ICON_AIKA] = new Image("sivupalkki/Ikonit/Timer B.png");
   image_ptrs[IMAGE_SIVUPALKKI_ICON_VESI] = new Image("sivupalkki/Ikonit/Add water B.png");
   image_ptrs[IMAGE_SIVUPALKKI_ICON_SEK] = new Image("sivupalkki/Ikonit/Mix B.png");
   image_ptrs[IMAGE_SIVUPALKKI_ICON_LAPPU] = new Image("sivupalkki/Ikonit/Muistilappu B.png");
}

void delete_all_images(void)
{
   for (int i=0; i < NUM_LOADED_IMAGES; i++)
   {
      if (image_ptrs[i])
         image_ptrs[i]->free();
   }
}

//Taustan pohjan rectanglet
//#include "taustan_4rects.h"
rectangle_s Rect1 = {
   {0, 70, 800, 232},
   //background: rgba(88, 91, 96, 1) 0% 0% no-repeat padding-box;
   {88, 91, 96, 1},
   1,
};

rectangle_s Rect2 = {
   {0, 70+1*232+1, 800, 232},
   //background: rgba(88, 91, 96, 1) 0% 0% no-repeat padding-box;
   {88, 91, 96, 1},
   1,
};

rectangle_s Rect3 = {
   {0, 70+2*232+2, 800, 232},
   //background: rgba(88, 91, 96, 1) 0% 0% no-repeat padding-box;
   {88, 91, 96, 1},
   1,
};

rectangle_s Rect4 = {
   {0, 70+3*232+3, 800, 232},
   //background: rgba(88, 91, 96, 1) 0% 0% no-repeat padding-box;
   {88, 91, 96, 1},
   1,
};

rectangle_s Rect4_small = {
   {0, 70+3*232+3, 800, 232-VALMIS_Y_OFFSET},
   //background: rgba(88, 91, 96, 1) 0% 0% no-repeat padding-box;
   {88, 91, 96, 1},
   1,
};

rounded_rectangle_s temp_saato_rounded_rect = {
   //L,  T,    W,  H
   {360, 137, 380, 110},
   //background: rgba(88, 91, 96, 1) 0% 0% no-repeat padding-box;
   {88,91,96, 1},
   64, //radius
   1,
};

rounded_rectangle_s aloita_rounded_rect = {
   //L,  T,    W,  H
   {50, 1038, 700, 82},
   //background: rgba(88, 91, 96, 1) 0% 0% no-repeat padding-box;
   {0x3D, 0xAF, 0x00, 1},
   64, //radius
   1,
};
