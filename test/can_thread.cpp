/* File: can_thread.cpp
   Purpose:

   L‰mmitys estetty:
      -Jos pata kallistettu
      -pohjaanpalamisen esto
   Vaippaan lasketaan vett‰ ennenkuin l‰mmitys kytket‰‰n p‰‰lle, uimuria tsekataan/viivett‰. Vaippavett‰ tutkitaan/lis‰t‰‰n koko l‰mmityksen ajan.

   Tyhjennys estetty:
      -Jos pata kallistettu
      -jos vaipan l‰mpˆ >55

   Vaipan t‰yttˆ estetty:
      -Jos pata kallistettu

   J‰‰hdytysveden otto estetty:
      -Jos pata kallistettu

   Kallistus estetty:
      -Jos kansi on kiinni
      -Jos tyhjennys menossa

   Sekoitus estettu:
      -Jos kansi on auki, pwm+ohjaukset pois, mahdollisesti eri kansikytkin-tietojen mukaan sallittu kuitenkin
      -Pulssiohjaus mahdollinen myˆs kansi auki, kun painetaan pulse-painiketta niin turvanopeus menee p‰‰lle

   Sekoitus, Speed-signaali invertterille, pwm? Suuntaohjaukset + on/off-rele ?

   Magneettiventtiileitten speksi. Mit‰ kaikkea tulee normaalien vesi/tyhjennys etc. lis‰ksi. CH5/CH4 j‰‰‰hdytys kuten M-ohjaimesssa?

   H‰t‰seis. Pys‰ytt‰nee kaiken ja l‰hdˆt nollataan.

   J‰‰hdytyksen aloitus vasta kun vaippa < 100C
   J‰‰hdytyksen etenemisen seuranta.
   2-vaihej‰‰hdytys Mohj: 1.vaihe avaa CHILL WATER X609:4, 2.vaihe avaa COLD WATER X609:5. Normaali 1-vaihej‰‰hdytys avaa COLD WATER X609:5
   2-vaihej‰‰hdytys vain jos J‰‰pankki k‰ytˆss‰? Onko n‰in? 
   J‰‰pankki k‰ytˆss‰ = CH4/CH5:ssa j‰‰hdytys vaakakortin io:lla. VaakaIO + alakortin ohjaukset yhdistet‰‰n uudessa.

   Kallistus ylˆs/alas + tiltback s‰‰dett‰v‰ll‰ ajalla:
   Virtarajan lukuviive k‰ynnistyksess‰. Rampitukset. Virtarajat.

   Kallistusnappuloihin pient‰ viivett‰.

   Sekoitusmoottorin ohjaus, eteen/taakse + paussit. Suunnanvaihdossa tauko. Ns. pulssiohjaus, Rampitukset. Virtarajat.

   Vedenmittaus, pulssianturi? Sen kalibrointi. Desilitran tarkkuus?

   Keiton alkuun vaipan tyhjennys??

   J‰‰hdytyksen j‰lkeen automaattinen vaipantyhjennys.
   Tyhjennyksen aloituksen j‰lkeen uimurin lukuun viivett‰ 1s. Tyhjennykseen aikaviive, 10min?

   Kontaktorivika ei katkaise l‰mmityst‰.
   Kontaktorin k‰rjet tutkitaan sekunnin v‰lein. K‰rki‰ ei tsekata jos keitto on p‰‰ll‰.

   Vedent‰ytˆn viive?

   Kansituloja n kpl.

   Pressure-sw?

   Uimurin polariteetti.

   Jos valittu "hˆyrypata", niin keiton aikana uimuritiedosta ei v‰litet‰. 

   Kuuman veden ohjaus.

   L‰mmˆns‰‰tˆ ruokal‰mmˆn, vaippal‰mmˆn, delta-t:n etc. perusteella.
   L‰mpˆtilojen offset-s‰‰tˆ?

   Alkutyhjennyksen viiveeksi vaihdettu 5 minuuttia. Alkutyhjennys siis?

   Ohijuoksutus. Tyhjennyshana auki ja kylm‰vesi kiert‰‰ vaipan l‰pi. Kallistus estetty.

   Diagnostiikkaa. Huoltolaskurit. Huoltolaskurin nollaus. Reletesti yms. Antureitten testi oikosulku/katkos.

   S‰hkˆkatkosuojaus.

   Softap‰ivitykset.

   Tuotantotesti.

*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include <linux/can.h>
#include <linux/can/raw.h>

#include <SDL.h>

#include "var.h"
#include "kettle_functions.h"
#include "externs.h"
#include "handler.h"
#include "menu_ids.h"

extern int iotest_function_on(void);

#define PULSES_PER_LITRA 671 //715

int can_comm_error[5];
int can_comm_err_counter[5];

extern int Stop_Threads;

struct can_frame frame;
int can_socket;

//return 0 if OK
int card1_read_inputs_message(void); //0x22 = Set Parameters / Read Inputs message
int card1_set_outputs_message(void); //0x11 = Set Outputs+Measures / Read Measured Values message
int card2_read_inputs_message(void);
int card2_set_outputs_message(void);

void handle_can_error(int ret, int mod)
{
#ifdef KORTIT_KYTKETTY
   if (ret == 0)
      can_comm_err_counter[mod] = 0;
   else if (++can_comm_err_counter[mod] > 5)
   {
      can_comm_err_counter[mod] = 5;
      can_comm_error[mod] = 1;
      printf("CAN COMMUNICATION ERROR module %d\n", mod);
   }
#else
   can_comm_err_counter[mod] = 0;
#endif
}

int can_communication_thread(void* data )
{
	struct sockaddr_can addr;
	struct ifreq ifr;
   int ret;
   struct timeval tv;

#ifndef KORTIT_KYTKETTY
      while (Stop_Threads == 0)
      {
         SDL_Delay(500);
      }
      printf("Can thread stopped\n");
      return 0;
#endif

reopen_can:
	const char *ifname = "can0";

   if((can_socket = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) 
   {
		perror("Error while opening socket");

can_socket_error:
      while (Stop_Threads == 0)
      {
         SDL_Delay(500);
         //N‰m‰ ehk‰ sittenkin --> can_thread:iin ?
         handle_cooking();
         handle_chilling();
         handle_water_fill();
         handle_emptying();
         handle_mixing();
         handle_tilting();
         update_live_parameters();
         handle_can_error(0, 0); //pc:ss‰ ei can toimi!
         handle_can_error(0, 1);
      }
		return 0;
	}

   tv.tv_sec = 1; // laitetaan receive timeout sekuntiin
   tv.tv_usec = 0;
   ret =  setsockopt(can_socket, SOL_SOCKET, SO_RCVTIMEO, (const void *)&tv, sizeof(struct timeval));
   printf("setsockopt returned %d\n", ret);

	strcpy(ifr.ifr_name, ifname);
	ioctl(can_socket, SIOCGIFINDEX, &ifr);
	
	addr.can_family  = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

	printf("%s at index %d\n", ifname, ifr.ifr_ifindex);
   printf("sizeof(struct can_frame)=%d\n", (int)sizeof(struct can_frame));

	if(bind(can_socket, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("Error in socket bind");
      goto can_socket_error;
		return -2;
	}

   for (int can=0; can < NUM_OF_CAN_MODULES; can++)
      can_comm_err_counter[can] = can_comm_error[can] = 0;

   while (Stop_Threads == 0)
   {
      SDL_Delay(200);

      ret = card1_read_inputs_message(); //READ modules INPUTS
      handle_can_error(ret, 0);
      ret = card2_read_inputs_message(); //READ modules INPUTS
      handle_can_error(ret, 1);

      if ((input_registers[INPUT_REG_CARD2_DIGITAL_INPUTS] & EMERGENCY_STOP_IN_BIT_CARD2) != 0) //Miten p‰in toimii!
         hataseis_painettu = 1;
      else
         hataseis_painettu = 0;

      if ((current_menu_id != ID_IOTEST) || iotest_function_on()) //Jos iotest-sivulla COOK/CHILL/WATER laitettu ON niin hoidetaan ko. funktiot
      {
         handle_cooking();
         handle_chilling();
         handle_water_fill();
         handle_emptying();
      }
      handle_mixing();
      handle_tilting();

      if (hataseis_painettu)
      {
         output_registers[OUTPUT_REG_CARD1_DIGITAL_OUTPUTS] = 0;
         output_registers[OUTPUT_REG_CARD1_ANALOG_OUTPUT_1]  = 0;
         output_registers[OUTPUT_REG_CARD1_ANALOG_OUTPUT_2]  = 0;
         output_registers[OUTPUT_REG_CARD2_DIGITAL_OUTPUTS] = 0;
         output_registers[OUTPUT_REG_CARD2_ANALOG_OUTPUT_1]  = 0;
         output_registers[OUTPUT_REG_CARD2_ANALOG_OUTPUT_2]  = 0;
      }

      ret = card1_set_outputs_message(); //WRITE module OUTPUTS
      handle_can_error(ret, 0);
      ret = card2_set_outputs_message(); //WRITE module OUTPUTS
      handle_can_error(ret, 1);

      update_live_parameters();

      if (can_comm_error[0] && can_comm_error[1])
      {
         SDL_Delay(1000);
         close (can_socket);
         printf("closing and reopening can socket..\n");
         SDL_Delay(1000);
         goto reopen_can;
      }
   }	
   printf("Can thread stopped\n");
	return 0;
}

int card2_set_outputs_message(void) //0x11 = Set Outputs+Measures / Read Measured Values message
{
	int nbytes, i;

   if (can_socket == 0)
      return -1;
   frame.can_id  = 0x083; //Io-kortti #1 osoite = 0x82, #2 osoite = 0x83
	frame.can_dlc = 8;

   frame.data[0] = 0x11;
   frame.data[1] = (output_registers[OUTPUT_REG_CARD2_DIGITAL_OUTPUTS] >> 8) & 0x00FF;
   frame.data[2] = output_registers[OUTPUT_REG_CARD2_DIGITAL_OUTPUTS] & 0x00FF;

   frame.data[3] = (output_registers[OUTPUT_REG_CARD2_ANALOG_OUTPUT_1] >> 8) & 0x00FF; //Mixer speed [0..100]%
   frame.data[4] = output_registers[OUTPUT_REG_CARD2_ANALOG_OUTPUT_1] & 0x00FF;

   frame.data[5] = (output_registers[OUTPUT_REG_CARD2_MITTAA_VESILITRAT] >> 8) & 0x00FF;
   frame.data[6] = output_registers[OUTPUT_REG_CARD2_MITTAA_VESILITRAT] & 0x00FF;
   output_registers[OUTPUT_REG_CARD2_MITTAA_VESILITRAT] &= 0x7FFF;//bitti 0x8000 + vesim‰‰r‰ => aloita t‰m‰n vesim‰‰r‰n laskeminen nyt, nollataan kun on l‰hetetty io:lle

   frame.data[7] = 0;

   nbytes = write(can_socket, &frame, sizeof(struct can_frame));
   if (nbytes != 16)
   {
#ifdef KORTIT_KYTKETTY
      printf("CAN WRITE ERROR nbytes=%d\n", nbytes);
#endif
      return -2;
   }

   for (i=0; i<8; i++)
      frame.data[i] = 0;

	nbytes = read(can_socket, &frame, sizeof(struct can_frame));
   if (nbytes != 16)
   {
#ifdef KORTIT_KYTKETTY
      printf("CAN READ ERROR nbytes=%d\n", nbytes);
#endif
      return -3;
   }

   if (frame.data[0] == 0x01)
   {
      unsigned short int s = frame.data[1];
      s <<= 8;
      s += frame.data[2];
      input_registers[INPUT_REG_CARD2_MITATUT_LITRAT] = s;

      s = frame.data[3];
      s <<= 8;
      s += frame.data[4];
      input_registers[INPUT_REG_CARD2_VESI_PULSSIT] = s; //Max on 65535!

//      printf("card2_set_outputs_message OK\n");
      return 0;
   }
   else
   {
      printf("Can message error card2_set_outputs_message.\n");
      return -11;
   }
}

int card1_set_outputs_message(void) //0x11 = Set Outputs+Measures / Read Measured Values message
{
	int nbytes, i;

   if (can_socket == 0)
      return -1;
   frame.can_id  = 0x082; //Io-kortti #1 osoite = 0x82, #2 osoite = 0x83
	frame.can_dlc = 8;

   // 8-tavua l‰hetet‰‰n:       
   //-indeksitavu                 0. 0x11  Message code
   //-1*8-bit digital 3V outputs  1.   3V outputit (4kpl): (rekisterin OUTPUT_REG_CARD1_DIGITAL_OUTPUTS, bitit 0..3)
   //-1*8-bit digital 24V outputs 2.   24V outputit (8kpl): (rekisterin OUTPUT_REG_CARD1_DIGITAL_OUTPUTS, bitit 4..11)
   //-1*16-bit Analog output      3,4  Mixer speed [0..100]%
   //-1*16-bit Measure            5,6
   //-1*8-bit extra-tavu          7.
   frame.data[0] = 0x11;
   frame.data[1] = (output_registers[OUTPUT_REG_CARD1_DIGITAL_OUTPUTS] >> 8) & 0x00FF;
   frame.data[2] = output_registers[OUTPUT_REG_CARD1_DIGITAL_OUTPUTS] & 0x00FF;

   frame.data[3] = (output_registers[OUTPUT_REG_CARD1_ANALOG_OUTPUT_1] >> 8) & 0x00FF;
   frame.data[4] = output_registers[OUTPUT_REG_CARD1_ANALOG_OUTPUT_1] & 0x00FF;

   frame.data[5] = 0; //K‰ytt‰m‰tˆn
   frame.data[6] = 0;
   frame.data[7] = 0;

   nbytes = write(can_socket, &frame, sizeof(struct can_frame));
   if (nbytes != 16)
   {
#ifdef KORTIT_KYTKETTY
      printf("CAN WRITE ERROR nbytes=%d\n", nbytes);
#endif
      return -2;
   }

   // 8-tavua vastaanoteteaan:
   //-1 indeksi              0
   //-1*16-bit MITATUT VESILITRAT  1,2
   //-1*16-bit VESIANTURI PULSSIT  3,4  Max on 65535!
   for (i=0; i<8; i++)
      frame.data[i] = 0;

	nbytes = read(can_socket, &frame, sizeof(struct can_frame));
   if (nbytes != 16)
   {
#ifdef KORTIT_KYTKETTY
      printf("CAN READ ERROR nbytes=%d\n", nbytes);
#endif
      return -3;
   }

   if (frame.data[0] == 0x01) //indeksi 0x01 for message type 0x11
   {
//            printf("card1_set_outputs_message OK\n");
      return 0;
   }
   else
   {
      printf("Can message error card1_set_outputs_message.\n");
      return -11;
   }
}

int card2_read_inputs_message(void) //0x22 = Set Parameters / Read Inputs message
{
	int nbytes, i;

   if (can_socket == 0)
      return -1;
   frame.can_id  = 0x083; //Io-kortti #2 osoite = 0x82, #2 osoite = 0x83
	frame.can_dlc = 8;

   frame.data[0] = 0x22;
   frame.data[1] = PULSES_PER_LITRA & 0xFF; //Data[1] = Pulsseja per litra 8 alinta bitti‰
   frame.data[2] = (PULSES_PER_LITRA >> 8) & 0xff; //Data[2] = Pulsseja per litra 3 ylinta bitti‰ -> 11 bitti‰ = 2048 max

   frame.data[3] = 0; //Jos alin bitti 1 => Hot Food Water
   frame.data[4] = 0;

   frame.data[5] = 0;
   frame.data[6] = 0;

   frame.data[7] = 0;

   nbytes = write(can_socket, &frame, sizeof(struct can_frame));
   if (nbytes != 16)
   {
#ifdef KORTIT_KYTKETTY
      printf("CAN WRITE ERROR nbytes=%d\n", nbytes);
#endif
      return -2;
   }

   for (i=0; i<8; i++)
      frame.data[i] = 0;

	nbytes = read(can_socket, &frame, sizeof(struct can_frame));

   if (nbytes != 16)
   {
#ifdef KORTIT_KYTKETTY
      printf("CAN READ ERROR nbytes=%d\n", nbytes);
#endif
      return -3;
   }

   if (frame.data[0] == 0x02) //indeksi 0x02 for message type 0x22
   {
      input_registers[INPUT_REG_CARD2_DIGITAL_INPUTS] = (signed short int)frame.data[5];
//      printf("card2_read_inputs_message OK\n");
      return 0;
   }
   else
   {
      printf("Can message error card2_read_inputs_message.\n");
      return -10;
   }
}

int card1_read_inputs_message(void) //0x22 = Set Parameters / Read Inputs message
{
	int nbytes, i;

   if (can_socket == 0)
      return -1;
   frame.can_id  = 0x082; //Io-kortti #1 osoite = 0x83, #2 osoite = 0x83
	frame.can_dlc = 8;

   // 8-tavua l‰hetet‰‰n:
   //-indeksitavu                 0. 0x22   Message code
   //-parametrej‰.. Esim Pulsseja per litra, max. volume, Tiltback time etc
   //-5*8-bit jotain....          3,4,5,6,7
   frame.data[0] = 0x22;
   frame.data[1] = 0;
   frame.data[2] = 0;

   frame.data[3] = 0;
   frame.data[4] = 0;

   frame.data[5] = 0;
   frame.data[6] = 0;

   frame.data[7] = 0;

   nbytes = write(can_socket, &frame, sizeof(struct can_frame));
   if (nbytes != 16)
   {
#ifdef KORTIT_KYTKETTY
      printf("CAN WRITE ERROR nbytes=%d\n", nbytes);
#endif
      return -2;
   }

   // 8-tavua vastaanoteteaan:
   //-1 indeksi                    0 = 0x02 for 0x22
   //-2 16-bit l‰mpˆtilaa    1,2,3,4
   //-1 8-bit digital inputs 5
   for (i=0; i<8; i++)
      frame.data[i] = 0;

	nbytes = read(can_socket, &frame, sizeof(struct can_frame));

   if (nbytes != 16)
   {
#ifdef KORTIT_KYTKETTY
      printf("CAN READ ERROR nbytes=%d\n", nbytes);
#endif
      return -3;
   }

   if (frame.data[0] == 0x02) //indeksi 0x02 for message type 0x22
   {
      signed short int s = frame.data[1];
      s <<= 8;
      s += frame.data[2];
      input_registers[INPUT_REG_CARD1_READ_FOOD_TEMP] = s;
      s = frame.data[3];
      s <<= 8;
      s += frame.data[4];
      input_registers[INPUT_REG_CARD1_READ_KETTLE_TEMP] = s;
//         printf("FoodTemp=%d JacketTemp=%d\n", input_registers[INPUT_REG_CARD1_READ_FOOD_TEMP], input_registers[INPUT_REG_CARD1_READ_KETTLE_TEMP]);
      input_registers[INPUT_REG_CARD1_DIGITAL_INPUTS] = (signed short int)frame.data[5];
//            printf("card1_read_inputs_message OK\n");
      return 0;
   }
   else
   {
      printf("Can message error card1_read_inputs_message.\n");
      return -10;
   }
}

/*Vesim‰‰r‰:
   Kun ui:ssa asetetaan vesim‰‰r‰, esim 100 -> OUTPUT_REG_CARD1_MITTAA_VESILITRAT=100, laitetaan msb=1, tarkoittaa "Uusi vesim‰‰r‰ asetettu".
   Can l‰hett‰‰ kortille tiedon ja nollaa msb:n l‰hetyksenj‰lkeen -> kortti saa tiedon ja asettaa uudeksi halutuksi 100 ja aloittaa vesim‰‰r‰n laskun alusta.
   Reg pysyy arvossa 100, jos vesi stopataan ui:ssa reg->0 -> kortti stoppaa veden. Aina kun se aloittaa veden niin se nollaa pulssit + mitatut litrat.
   UI:ssa vesi stopataan aina ennenkuin uusi vesim‰‰r‰ asetetaan, kortti ei v‰ltt‰m‰tt‰ t‰t‰ stoppausta huomaa mutta msb kertoo ett‰ aloitetaan alusta kun uusi tieto saadaan.

   AVAA HANA: voisi vaikka bitit 0xC000 yhdess‰ kertoa ett‰ avaa hana ja ala laskemaan litroja -> io alkaa laskemaan -> can nollaa msb:n -> io laskee kunnes bitti 0x4000 on nolla

   Kortin pit‰‰ p‰ivitt‰‰ INPUT_REG_CARD1_MITATUT_LITRAT ennenkuin se l‰hett‰‰ vastauksen card1_set_target_message-viestiin.
*/
