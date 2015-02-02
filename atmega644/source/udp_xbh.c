/*----------------------------------------------------------------------------
 Copyright:      Christian Wenzel-Benner  mailto: c-w-b@web.de
 Author:         Christian Wenzel-Benner
 Remarks:        
 known Problems: none
 Version:        25.04.2009
 Description:    1st try: echo rec'd UDP data back to sender IP, port 22594 (XB)

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
------------------------------------------------------------------------------*/
#include "udp_xbh.h"
#include "xbh.h"
#include "xbh_utils.h"
#include "xbh_xbdcomm.h"



void udp_xbh_init (void)
{
	//Port in Anwendungstabelle eintragen f�r eingehende  Daten!
	add_udp_app (UDP_XBH_PORT, (void(*)(unsigned char))udp_xbh_get);
}




void udp_xbh_get (unsigned char index)
{
	//lcd_print(0,0,"Working");

	unsigned int ip_byte[4];
	struct UDP_Header *udp;
        struct IP_Header  *ip;
	unsigned int src_port;

	u16 udp_data_len = (UDP_DATA_END_VAR)-(UDP_DATA_START);


        udp = (struct UDP_Header *)&eth_buffer[UDP_OFFSET];
        ip  = (struct IP_Header  *)&eth_buffer[IP_OFFSET];
  
	ip_byte[0]=(ip->IP_Srcaddr)&0xff;
	ip_byte[1]=((ip->IP_Srcaddr)>>8)&0xff;
	ip_byte[2]=((ip->IP_Srcaddr)>>16)&0xff;
	ip_byte[3]=((ip->IP_Srcaddr)>>24)&0xff;

	src_port = NTOHS((udp->udp_SrcPort));
	XBH_DEBUG("XBH DATA GET Bytes: %i\n",((UDP_DATA_END_VAR)-(UDP_DATA_START)));
//	XBH_DEBUG("** XBH DATA GET SrcPort: %i **\n",src_port);
//	XBH_DEBUG("** XBH DATA GET SrcIP: %i.%i.%i.%i **\n",
//		ip_byte[0],ip_byte[1],ip_byte[2],ip_byte[3]);	
	
/**/
/*	XBH_DEBUG("UDP rec'd:\n");
	for(u08 u=0;u<20;++u)
	{
		//	XBH_DEBUG("%x ",eth_buffer[UDP_DATA_START+u]);
		XBH_DEBUG("%c",eth_buffer[UDP_DATA_START+u]);
	}
	XBH_DEBUG("\n");
*/
	udp_data_len = XBH_handle(&eth_buffer[UDP_DATA_START], udp_data_len,&eth_buffer[UDP_DATA_START]);

	if (0 == udp_data_len) {
			XBH_ERROR("Unknown Request Rec'd\n");
	}

	create_new_udp_packet(  (unsigned int)  udp_data_len,
	                        (unsigned int)  UDP_XBH_PORT,
    	                    (unsigned int)  src_port,
         	                (unsigned long) ip->IP_Srcaddr);

	XBH_DEBUG("ENDE XBH\n");
	//lcd_print(0,0,"Idle");
}
