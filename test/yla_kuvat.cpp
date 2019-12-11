/* 
   yla_kuvat.cpp
   Ylhäältä vedettävä dropdown näkymä
   Alue on 772x228 (valk.pohja) x=14, y=0
   
   Painikkeet 84x84 paitsi bluetuut 53x84
   HAccp y=89, x=15, w=128 h=128
   Ajastus y=89, x=150, w=128 h=128
   VAaka y=89, x=286, w=128 h=128
   Sammuta y=89, x=421, w=128 h=128
   Profiili y=89, x=557, w=128 h=128
   BT y=89, x=872,  w=128 h=128

   Tässä tuli Sammuta GLoben paikalle..Globe oli kielenvalinta mutta se tehdään nyt Profiilin kautta.
   VAAKA näytetään vain jos padassa on vaaka-toiminto

   codet: 1=haccp, 2=munakello, 3=vaaka, 4=sammuta, 5=profiili, 6=bt
   CODE_YLAPALKKI_HACCP ... CODE_YLAPALKKI_BT
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

int ylavalikko_naytolla;
static int my_button_idx;

void nayta_ylavalikko(int but_idx)
{
   int x, im;
   int i = but_idx;
   x = 15;
   im = 0;
   if (ylavalikko_naytolla == 0)
   {//Ei piirretä mitään vielä tällä kerralla..tarttenee muuttaa joskus!
      ylavalikko_naytolla = 1;
//      return;                  MUUTETTU
   }

   SDL_Rect r;
   r.x = 0;
   r.y = 0;
   r.w = 800;
   r.h = 273;
   SDL_SetRenderDrawColor(gRenderer, 0xEF, 0xEF, 0xEF, 0);
   SDL_RenderFillRect(gRenderer, &r);

   my_button_idx = but_idx;
   for (; i< (but_idx + 6); i++) //1=haccp, 2=munakello, 3=vaaka, 4=sammuta, 5=profiili, 6=bt
   {
      Button_group_add_button(i, x, 89, 128, 128);
      Button_group_add_images(i, image_ptrs[IMAGE_YLAPALKKI_KUVAT_TOIMINTO1 + im], 0);
      Button_group_add_event_code(i, CODE_YLAPALKKI_HACCP + im);
      Button_group_render(i);
      im++;
      x += 135;
   }
}

void piilota_ylavalikko(int but_idx)
{
   int i = but_idx;
   if ((but_idx == -1) && (my_button_idx != -1))
      but_idx = my_button_idx; //Voidaan esim. can_threadista piilottaa ylavalikko jos kansi avataan ja yv on näytöllä
   for (; i< (but_idx + 6); i++)
   {
      Button_groups[i].button_set = 0;
   }
   ylavalikko_naytolla = 0;
   my_button_idx = -1;
}
