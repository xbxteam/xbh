/*----------------------------------------------------------------------------
 Copyright:      Radig Ulrich  mailto: mail@ulrichradig.de
 Author:         Radig Ulrich
 Remarks:        
 known Problems: none
 Version:        24.10.2007
 Description:    Timer Routinen

 Dieses Programm ist freie Software. Sie können es unter den Bedingungen der 
 GNU General Public License, wie von der Free Software Foundation veröffentlicht, 
 weitergeben und/oder modifizieren, entweder gemäß Version 2 der Lizenz oder 
 (nach Ihrer Option) jeder späteren Version. 

 Die Veröffentlichung dieses Programms erfolgt in der Hoffnung, 
 daß es Ihnen von Nutzen sein wird, aber OHNE IRGENDEINE GARANTIE, 
 sogar ohne die implizite Garantie der MARKTREIFE oder der VERWENDBARKEIT 
 FÜR EINEN BESTIMMTEN ZWECK. Details finden Sie in der GNU General Public License. 

 Sie sollten eine Kopie der GNU General Public License zusammen mit diesem 
 Programm erhalten haben. 
 Falls nicht, schreiben Sie an die Free Software Foundation, 
 Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA. 
------------------------------------------------------------------------------*/
#ifndef _TIMER_H
	#define _TIMER_H

	#include <avr/io.h>
	#include <avr/wdt.h>
	#include "usart.h"
	#include "stack.h"
	#include "ntp.h"
    #include "cmd.h"
    #include "dhcpc.h"

	#if defined (__AVR_ATmega644__) || defined (__AVR_ATmega644P__)
		#define TIMSK TIMSK1
	#endif 

	// 250 interrupts per second
	#define OCRVAL (F_CPU/250L)

	volatile unsigned long time;
    volatile unsigned int  stack_watchdog;
    volatile unsigned char gp_timer;
	void timer_init (void);
 
#endif //_TIMER_H

