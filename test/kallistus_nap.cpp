/* 
   kallistus_nap.cpp
   Alhaalta vedettävä kallistusnäppäimet näkymä
   Alue on 800x258 (valk.pohja) x=0, y=1022
   Kallistuspainikkeet "Ikonit PNG/Alavalikko/Toiminto1.png" ja 2+3
   Painikkeet 256x256

   Uudet:
   Kallistuspainike pohjat: Alavalikko/Buttonit/Tilting panel Vakio.png" ja "Tilting panel Pohjassa.png" 220x220
   Ikonit Alavalikko/Ikonit...

   */
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "SDL2_gfxPrimitives.h"

#include <stdio.h>
#include <string>
#include <stdlib.h>

#include "menu_ids.h"
#include "handler.h"

#include "button_defs.h"
#include "kypsennys.h"
#include "tekstit.h"
#include "var.h"
#include "button_group.h"
#include "externs.h"
#include "kettle_functions.h"

#define ISO_ALUE_X1 0
#define ISO_ALUE_Y1 1022
#define ISO_ALUE_X2 799
#define ISO_ALUE_Y2 1279

int alavalikko_naytolla, esta_alavalikko_naytolle;
static int my_button_idx;

void nayta_alavalikko(int but_idx, int button_pressed)
{
   int x, im, ico;
   int i = but_idx;
   x = 0;
   im = 0;
   if (alavalikko_naytolla == 0)
   {//Ei piirretä mitään vielä tällä kerralla..tarttenee muuttaa joskus!
      if (esta_alavalikko_naytolle)
         return;

      alavalikko_naytolla = 1;
//      return;                MUUTETTU
   }
   my_button_idx = but_idx;
   x = 20;
   for (ico = 0; i< (but_idx+3); i++, ico++)
   {
      Button_group_add_button(i, x, 1022, 220, 220);
      if (button_pressed == (CODE_KALLISTUS_ALAS + im))
         Button_group_add_images(i, image_ptrs[IMAGE_ALAVALIKKO_POHJA + 1], image_ptrs[IMAGE_ALAVALIKKO_POHJA + 2 + im]);
      else
         Button_group_add_images(i, image_ptrs[IMAGE_ALAVALIKKO_POHJA], image_ptrs[IMAGE_ALAVALIKKO_POHJA + 2 + im]);
      Button_group_add_event_code(i, CODE_KALLISTUS_ALAS + im); //CODE_KALLISTUS_ALAS, CODE_KALLISTUS_PULSE, CODE_KALLISTUS_YLOS

      if (ico > 0)
         Button_groups[i].icon_image_pos.x = -5;//VOidaan hienosäätää iconin paikkaa tarpeen mukaan

      if ((ico != 1) && !onko_kansi_auki()) //EI kallistusta jos kansi on kiinni
         Button_groups[i].button_set = 0;
      else
         Button_group_render(i);
      im++;
      x += (220 + 46);
   }
}

void piilota_alavalikko(int but_idx)
{
   int i = but_idx;
   if ((but_idx == -1) && (my_button_idx != -1))
      but_idx = my_button_idx; //Voidaan esim. can_threadista piilottaa alavalikko jos kansi suljetaan ja av on näytöllä
   for (; i< (but_idx+3); i++)
   {
      Button_groups[i].button_set = 0;
   }
   alavalikko_naytolla = 0;
   my_button_idx = -1;
}
