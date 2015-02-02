/*----------------------------------------------------------------------------
 Copyright:      Michael Kleiber
 Author:         Michael Kleiber
 Remarks:        
 known Problems: none
 Version:        25.04.2008
 Description:    DNS Client

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
#include "config.h"

#if USE_DNS
#ifndef _DNSCLIENT_H
	#define _DNSCLIENT_H

	//#define DNS_DEBUG usart_write
	#define DNS_DEBUG(...)

	#include <avr/io.h>
	#include <avr/pgmspace.h>
	#include "stack.h"
	#include "usart.h"
	#include "timer.h"

	#define DNS_CLIENT_PORT		1078
	#define DNS_SERVER_PORT		53

	#define DNS_IP_EEPROM_STORE 	42

	unsigned char dns_server_ip[4];
    unsigned char dns_resolved_ip[4];
	volatile unsigned int dns_timer;
	
	void dns_init(void);
    unsigned char dns_resolve (char *name);
    void dns_query(char *name);
	void dns_get(void);
	
	
#endif //_DNSCLIENT_H
#endif //USE_DNS

