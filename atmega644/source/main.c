/*----------------------------------------------------------------------------
 Copyright:      Radig Ulrich  mailto: mail@ulrichradig.de
 Author:         Radig Ulrich
 Remarks:        
 known Problems: none
 Version:        24.10.2007
 Description:    Webserver uvm.

 Dieses Programm ist freie Software. Sie k�nnen es unter den Bedingungen der 
 GNU General Public License, wie von der Free Software Foundation ver�ffentlicht, 
 weitergeben und/oder modifizieren, entweder gem�� Version 2 der Lizenz oder 
 (nach Ihrer Option) jeder sp�teren Version. 

 Die Ver�ffentlichung dieses Programms erfolgt in der Hoffnung, 
 da� es Ihnen von Nutzen sein wird, aber OHNE IRGENDEINE GARANTIE, 
 sogar ohne die implizite Garantie der MARKTREIFE oder der VERWENDBARKEIT 
 F�R EINEN BESTIMMTEN ZWECK. Details finden Sie in der GNU General Public License. 

 Sie sollten eine Kopie der GNU General Public License zusammen mit diesem 
 Programm erhalten haben. 
 Falls nicht, schreiben Sie an die Free Software Foundation, 
 Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA. 
----------------------------------------------------------------------------*/

#include <avr/io.h>
#include <util/delay.h>
#include "config.h"
#include "usart.h"
#include "networkcard/enc28j60.h"
//#include "networkcard/rtl8019.h"
#include "stack.h"
#include "timer.h"
#include "wol.h"
#include "httpd.h"
#include "cmd.h"
#include "telnetd.h"
#include "ntp.h"
#include "base64.h"
#include "http_get.h"
#include "lcd.h"
#include "udp_lcd.h"
#include "analog.h"
#include "camera/cam.h"
#include "camera/servo.h"
#include "sendmail.h"


#include "dhcpc.h"
#include "dnsc.h"

#include "xbh_xbdcomm.h"
#include "udp_xbh.h"
#include "tcp_xbh.h"
#include "xbh_utils.h"
#include "XBH_version.h"

uint8_t mcusr_mirror __attribute__ ((section (".noinit")));

void get_mcusr(void) \
  __attribute__((naked)) \
  __attribute__((section(".init3")));
void get_mcusr(void)
{
  mcusr_mirror = MCUSR;
  MCUSR = 0;
  wdt_disable();
}

void WDT_off(void)
{

/* Clear WDRF in MCUSR */
MCUSR &= ~(1<<WDRF);
/* Write logical one to WDCE and WDE */
/* Keep old prescaler setting to prevent unintentional time-out */
WDTCSR |= (1<<WDCE) | (1<<WDE);
/* Turn off WDT */
WDTCSR = 0x00;

}
//----------------------------------------------------------------------------
//Hier startet das Hauptprogramm
int main(void)
{  
	//Konfiguration der Ausg�nge bzw. Eing�nge
	//definition erfolgt in der config.h
	DDRA = OUTA;
	DDRC = OUTC;
	DDRD = OUTD;
	
	WDT_off();

    unsigned long a;
	#if USE_SERVO
		servo_init ();
	#endif //USE_SERVO
	
    usart_init(BAUDRATE); // setup the UART
	
	#if USE_ADC
		ADC_Init();
	#endif
	
	xbh_debug_write("\n\rSystem Ready\n\r");
  xbh_debug_write("Compiliert am "__DATE__" um "__TIME__"\r\n");
  xbh_debug_write("Compiliert mit GCC Version "__VERSION__"\r\n");

	xbh_debug_write("\n\rSVN rev:"XBH_REVISION", date:"XBH_VERSION_DATE", time:"XBH_VERSION_TIME"\n\r");

	for(a=0;a<1000000;a++){asm("nop");};

    xbh_debug_write("\r\nLoading MAC Address from EEPROM.\r\n");
  	read_mac_address(); //get from EEPROM
	xbh_debug_write("MAC  %2x:%2x:%2x:%2x:%2x:%2x\r\n",eeprom_mac[0],eeprom_mac[1],eeprom_mac[2],eeprom_mac[3],eeprom_mac[4],eeprom_mac[5]);
 
	for(a=0;a<6;++a)
	{
		if(0xff != eeprom_mac[a]) break;
	}
	if(6==a)
	{
		xbh_debug_write("\r\nMAC address is all FF's. Network stack will not start!\r\nPlease change using the 'mac' command!");
		//Globale Interrupts einschalten
		sei(); 
		while(1)
		{
		    //Terminalcommandos auswerten
			if (usart_status.usart_ready && !usart_status.usart_ignore){
	            xbh_debug_write("\r\n");
				if(extract_cmd(&usart_rx_buffer[0]))
				{
					xbh_debug_write("Ready\r\n\r\n");
				}
				else
				{
					xbh_debug_write("ERROR\r\n\r\n");
				}
				usart_status.usart_ready =0;
			}
		
		}
	}


	//Applikationen starten
	stack_init();
//	httpd_init();
//	telnetd_init();
	
	//Spielerrei mit einem LCD
	#if USE_SER_LCD
		udp_lcd_init();
		lcd_init();
		lcd_clear();
		back_light = 1;
		lcd_print(0,0,"System Ready");
	#endif
	
	
	#if USE_XBH
		xbdCommInit(COMM_I2C);
		udp_xbh_init();
		tcp_xbh_init();
	#endif

	//Ethernetcard Interrupt enable
	ETH_INT_ENABLE;
	
	//Globale Interrupts einschalten
	sei(); 
	
	#if USE_CAM
		#if USE_SER_LCD
		lcd_print(1,0,"CAMERA INIT");
		#endif //USE_SER_LCD
	for(a=0;a<2000000;a++){asm("nop");};
	cam_init();
	max_bytes = cam_picture_store(CAM_RESOLUTION);
		#if USE_SER_LCD
		back_light = 0;
		lcd_print(1,0,"CAMERA READY");
		#endif //USE_SER_LCD
	#endif //USE_CAM

	xbh_debug_write("\r\nLoading IP Address from EEPROM.");
  	read_ip_addresses(); //get from EEPROM

    #if USE_DHCP
    dhcp_init();
    if ( dhcp() == 0)
    {
        save_ip_addresses();
    }
    else
    {
        xbh_debug_write("DHCP fail\r\n");
        read_ip_addresses(); //get from EEPROM
    }
    #endif //USE_DHCP
	
	
    xbh_debug_write("\r\nIP   %1i.%1i.%1i.%1i\r\n", myip[0]     , myip[1]     , myip[2]     , myip[3]);
    xbh_debug_write("MASK %1i.%1i.%1i.%1i\r\n", netmask[0]  , netmask[1]  , netmask[2]  , netmask[3]);
    xbh_debug_write("GW   %1i.%1i.%1i.%1i\r\n", router_ip[0], router_ip[1], router_ip[2], router_ip[3]);

    #if USE_DNS
    xbh_debug_write("DNS  %1i.%1i.%1i.%1i\r\n", dns_server_ip[0], dns_server_ip[1], dns_server_ip[2], dns_server_ip[3]);
    #endif //USE_DNS
    
	#if USE_NTP
        #if USE_DNS
        dns_init();
        if ( dns_resolve("1.de.pool.ntp.org") == 0) //resolve NTP server
        {
          for (unsigned char count = 0; count<4; count++)
          {
            eeprom_busy_wait ();
            eeprom_write_byte((unsigned char *)(NTP_IP_EEPROM_STORE + count),dns_resolved_ip[count]);
          }
        }
        else
        {
            xbh_debug_write("DNS Err.\r\n");
        }
        #endif //USE_DNS
   ntp_init(); 
   
   for(a=0;a<1000000;a++){asm("nop");};
   
   ntp_request();
   
    if ( ntp() != 0 )
	{
	  xbh_debug_write("NTP Err.\r\n");
	}
    else
	{
	  command_time();
	}
	#endif //USE_NTP
	
	#if USE_WOL
        wol_init();
	#endif //USE_WOL
    
    #if USE_MAIL
        mail_client_init();
	#endif //USE_MAIL  
		
	while(1)
	{
		#if USE_ADC
		ANALOG_ON;
		#endif
	    eth_get_data();
		
        //Terminalcommandos auswerten
		if (usart_status.usart_ready && !usart_status.usart_ignore){
            xbh_debug_write("\r\n");
			if(extract_cmd(&usart_rx_buffer[0]))
			{
				xbh_debug_write("Ready\r\n\r\n");
			}
			else
			{
				xbh_debug_write("ERROR\r\n\r\n");
			}
			usart_status.usart_ready =0;
		}
		
        //Wetterdaten empfangen (Testphase)
        #if GET_WEATHER
        http_request ();
        #endif
        
        //Empfang von Zeitinformationen
		#if USE_NTP
		if(!ntp_timer){
			ntp_timer = NTP_REFRESH;
			ntp_request();
		}
		#endif //USE_NTP
		
        //Versand von E-Mails
        #if USE_MAIL
        if (mail_enable == 1)
        {
            mail_enable = 0;
            mail_send();
        }
        #endif //USE_MAIL
        
        //Rechner im Netzwerk aufwecken
        #if USE_WOL
        if (wol_enable == 1)
        {
            wol_enable = 0;
            wol_request();
        }
        #endif //USE_WOL
        
        #if USE_DHCP
        if ( dhcp() != 0) //check for lease timeout
        {
            xbh_debug_write("dhcp lease renewal failed\r\n");
			RESET();
        }
        #endif //USE_DHCP
  
		//USART Daten f�r Telnetanwendung?
//		telnetd_send_data();
        
        if(ping.result)
        {
            xbh_debug_write("Get PONG: %i.%i.%i.%i\r\n",ping.ip1[0],ping.ip1[1],ping.ip1[2],ping.ip1[3]); 
            ping.result = 0;
        }
    }//while (1)
		
return(0);
}

