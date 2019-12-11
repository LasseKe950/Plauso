/* 
   USO
   2019

   update_thread.cpp

   Etsit‰‰n uusi versio usb-tikulta, jos on niin n‰ytet‰‰n "P‰ivitys ja uudelleenk‰ynnistys?".
   */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/times.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/mount.h>
#include <SDL.h>

#include "menu_ids.h"
#include "handler.h"
#include "var.h"
#include "kettle_functions.h"
#include "externs.h"
#include "cook_prog.h"
#include "tekstit.h"

int read_version_info(void); //Jos palauttaa 0 niin p‰ivitet‰‰n

int ver_maj, ver_min;

char Prog_update_buffer1[100];
char Prog_update_buffer2[100];
int Prog_update_going;

int check_if_upgrade_found(void);

int usb_mounted, updated;

char global_buffer_100[100];

void do_update(void)
{
   //Kopioidaan tikulta kaikki hakemistosta "/usb/lmg_update/" --> /home/root/microwindows-0.92/src/src_132/MX/
            //sprintf(global_buffer_100, "ls -lrt /usb/lmg_update/*.*");
   //       sprintf(global_buffer_100, "cp /usb/lmg_update/* /home/root/microwindows-0.92/src/src_132/MX/");
            
            //sprintf(global_buffer_100, "cp /media/sdb1/lmg_update/* /media/mmcblk0p1/mw/microwindows-0.92/src/src_132/MX/");

            //v1.10 kopioidaan koko lmg_update/MX sis‰ltˆ -> MX-kansion alle

   //sprintf(global_buffer_100, "cp -r /media/sdb1/lmg_update/MX /media/mmcblk0p1/mw/microwindows-0.92/src/src_132/");
   sprintf(global_buffer_100, "cp -r /run/media/sdb1/uso_update/home/root /home/");
   printf("\n%s\n", global_buffer_100);

   system(global_buffer_100);
   umount("/run/media/sdb1");
   updated = 1;
   printf("Updated OK\n");

   strcpy(Prog_update_buffer2, (char*)yleis_tekstit[YLEIS_TXT_PAIVITYS_TEHTY_UUDELLEENKAYNNISTA]);
   printf("%s\n", Prog_update_buffer2);
   Prog_update_going = 2;
}

//LMG: linux n‰kˆj‰‰n automounttaa usbi-tikun -> /media/sdb1...ei kuitenkaan 100% aina
//7.11.2017 p‰‰tettiin ett‰ menn‰‰n t‰ll‰ oletuksella toistaiseksi
//joten k‰ytet‰‰n tuota paikkaa "/media/sdb1"
int check_usb(void)
{
   int ret;
//   ret = mount("/dev/sda1", "/media/sdb1", "vfat", 0, "");
//   if (ret != 0)
   {
      umount("/run/media/sdb1");
      SDL_Delay(500);
      ret = mount("/dev/sdb1", "/run/media/sdb1", "vfat", 0, "");
      if (ret != 0)
      {
//         printf("\nCould not mount usb!\n");
         usb_mounted = 0;
      }
   }
   if (ret==0)
   {
      usb_mounted = 1;
      printf("\nmount usb OK\n");
   }
   return usb_mounted;
}

int check_if_upgrade_found(void)
{
   if ( check_usb() == 0)
   {
//      printf("Checking usb -> No USB\n");
   }
   else
   {
      if ( ( access( "/run/media/sdb1/uso_update/version.txt", F_OK ) != -1) && ( access( "/run/media/sdb1/uso_update/demo_mx6", F_OK ) != -1) )
      {// file exists
         if ( read_version_info() == 0)
         {
            do_update();
         }
         return 0;
      } 
      else 
      {// file doesn't exist
         //3=EI L÷YTYNYT PƒIVITYSTƒ!
         printf("/run/media/sdb1/uso_update/demo_mx6 not found! TAI\n");
         printf("/run/media/sdb1/uso_update/version.txt not found!\n");
      }
   }
   return -1;
}

extern int Stop_Threads;

int update_thread(void* data )
{
   printf("Update thread started\n");
   updated = 0;
   Prog_update_going = 0;
   while (Stop_Threads == 0)
   {
      SDL_Delay(5000);
      if (current_menu_id == ID_KOTINAKYMA)
      {
         if (updated == 0)
         {
            if (!usb_mounted)
            {
               check_if_upgrade_found();
            }
         }
      }
   }
   printf("Update thread stopped\n");
   return 0;
}

int read_version_info(void) //Jos palauttaa 0 niin p‰ivitet‰‰n
{
	FILE *target;
	char buffer[30];

	target = fopen("/run/media/sdb1/uso_update/version.txt", "r");
	if (target==0)
	{
	  printf("fopen error /run/media/sdb1/uso_update/version.txt\n");
	  return -1;
	}
   if (fgets(buffer, 20, target) == NULL)
   {
	  printf("fgets error /run/media/sdb1/uso_update/version.txt\n");
	  return -1;
	}
   fclose(target);

   if (sscanf(buffer, "%d.%d", &ver_maj, &ver_min) != 2)
   {
	   printf("sscanf error \n");
      return -1;
   }
   char apub[20];
   sprintf(apub, " v%d.%d (v%d.%d)", ver_maj, ver_min, prog_ver_major, prog_ver_minor);
   strcpy(Prog_update_buffer1, (char*)yleis_tekstit[YLEIS_TXT_OHJELMA_VERSIO_LOYTYI]);
   strcat(Prog_update_buffer1, apub);

   printf("%s\n", Prog_update_buffer1);
   Prog_update_going = 1;

   int upd = 1;
   if (prog_ver_major > ver_maj)
      upd = 0;
   if ((prog_ver_major == ver_maj) && (prog_ver_minor >= ver_min))
      upd = 0;
   if (upd == 0)
   {
      strcpy(Prog_update_buffer2, (char*)yleis_tekstit[YLEIS_TXT_NYKYINEN_VERSIO_ON_JO_UUSIN]);
      printf("%s\n", Prog_update_buffer2);
      Prog_update_going = 2;
      return -1;
   }
   return 0;
}
