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
#include "timer.h"
#include "xbh_utils.h"
#include <util/delay.h>

volatile unsigned long time;

volatile unsigned char intCtr=0;
volatile unsigned int resetTimer=0;

volatile unsigned char risingSeen=0;
volatile unsigned char fallingSeen=0;


volatile unsigned long 	risingTimeStamp=0;
volatile unsigned char	risingIntCtr=0;
volatile unsigned int	risingTime=0;

volatile unsigned long 	fallingTimeStamp=0;
volatile unsigned char	fallingIntCtr=0;
volatile unsigned int	fallingTime=0;

//----------------------------------------------------------------------------
//Diese Routine startet und inizialisiert den Timer
void timer_init (void)
{
	/*
	old code: Divide CLKcpu by 1024 and have Timer1 compare match once per
	second.

	TCCR1B |= (1<<WGM12) | (1<<CS10 | 0<<CS11 | 1<<CS12);
	TCNT1 = 0;
	OCR1A = (F_CPU / 1024) - 1;
	TIMSK |= (1 << OCIE1A);
	*/

	//new code: Have Timer1 compare match at 64000, giving 250IRQs/s with
	//a prescale of 1. This gives maximum accuracy for ICP

	//PORTD |= _BV(PD6);	//enalbe internal pull-up on ICP1

	TCCR1B |= (1<<WGM12) | (1<<CS10 );	//CTC mode, full CLKcpu, ICP on falling edge
	TCNT1 = 0;
	OCR1A = OCRVAL;
	TIMSK |= (1 << OCIE1A) | (1<<ICIE1);

return;
};

//----------------------------------------------------------------------------
//Timer Compare Interrupt 

ISR (TIMER1_COMPA_vect)
{


	++intCtr;
	if( (F_CPU/OCRVAL) == intCtr)
	{
		//tick 1 second
		++time;
	    if((stack_watchdog++) > WTT)  //emergency reset of the stack
	    {
	        RESET();
		}
	    eth.timer = 1;
		#if USE_NTP
		ntp_timer--;
		#endif //USE_NTP
		#if USE_DHCP
		if ( dhcp_lease > 0 ) dhcp_lease--;
	    if ( gp_timer   > 0 ) gp_timer--;
	    #endif //USE_DHCP
	
		intCtr=0;

		if(resetTimer != 0)
		{		
			if(resetTimer==1)
			{
				XBH_WARN("Timeout occured!\n");
				emergencyXBHandXBDreset();
			}
			--resetTimer;
		}

	}
}


ISR (TIMER1_CAPT_vect, ISR_NAKED)
{
	
	 asm volatile (
        "\n\t"
        "push r0" "\n\t"
        "push r1" "\n\t"
        "lds r0,0x0086" "\n\t"	/*ICR1L*/
        "lds r1,0x0087" "\n\t"	/*ICR1H*/
				
				"push r16" "\n\t"
        "lds r16,0x0081" "\n\t"	/*read TCCR1B*/
				"sbrc r16,6" "\n\t"	/*test for ICES1*/
				"rjmp risingEdge" "\n\t"
				
				"sbr r16,0x40" "\n\t"
				"sts 0x0081,r16" "\n\t"	/*write back TCCR1B*/
				"push r17" "\n\t"				
				
				"ldi r17,1"	 "\n\t"	
				"sts fallingSeen,r17" "\n\t"	
				
				"lds r17,time" "\n\t"
				"sts fallingTimeStamp,r17" "\n\t"
				"lds r17,time+1" "\n\t"
				"sts fallingTimeStamp+1,17" "\n\t"
				"lds r17,time+2" "\n\t"
				"sts fallingTimeStamp+2,r17" "\n\t"
				"lds r17,time+3" "\n\t"
				"sts fallingTimeStamp+3,r17" "\n\t"
				
				"lds r17,intCtr" "\n\t"
				"sts fallingIntCtr,r17" "\n\t"
				
				"sts fallingTime,r0" "\n\t"				
				"sts fallingTime+1,r1" "\n\t"				
        
				"pop r17" "\n\t"		        
        "pop r16" "\n\t"		
        "pop r1" "\n\t"			
	      "pop r0" "\n\t"
				
				"reti" "\n\t"

				"risingEdge:" "\n\t"	
				
				"cbr r16,0x40" "\n\t"
				"sts 0x0081,r16" "\n\t"	/*write back TCCR1B*/
				"push r17" "\n\t"				
				
				"ldi r17,1"	 "\n\t"	
				"sts risingSeen,r17" "\n\t"	
				
				"lds r17,time" "\n\t"
				"sts risingTimeStamp,r17" "\n\t"
				"lds r17,time+1" "\n\t"
				"sts risingTimeStamp+1,17" "\n\t"
				"lds r17,time+2" "\n\t"
				"sts risingTimeStamp+2,r17" "\n\t"
				"lds r17,time+3" "\n\t"
				"sts risingTimeStamp+3,r17" "\n\t"
				
				"lds r17,intCtr" "\n\t"
				"sts risingIntCtr,r17" "\n\t"
				
				"sts risingTime,r0" "\n\t"				
				"sts risingTime+1,r1" "\n\t"				

				"pop r17" "\n\t"		        
        "pop r16" "\n\t"		
        "pop r1" "\n\t"			
	      "pop r0" "\n\t"
				
				"reti" "\n\t"

		   );

/*
	register unsigned int saveTime = ICR1;

	if( bit_is_clear(TCCR1B, ICES1) )	//IRQ came from falling edge
	{
		TCCR1B |= _BV(ICES1); //set ICES1 , next edge is rising	 
		fallingSeen=1;
		fallingTimeStamp=time;
		fallingIntCtr=intCtr;
		fallingTime=saveTime;
	}
	else	//IRQ came from rising edge
	{
		TCCR1B &= ~(_BV(ICES1)); //clear ICES1, next edge is falling	
		risingSeen=1;
		risingTimeStamp=time;
		risingIntCtr=intCtr;
		risingTime=saveTime;
	}		


	return;
*/		
/*
	if( bit_is_clear(TCCR1B, ICES1) )	//IRQ came from falling edge
	{
		TCCR1B |= _BV(ICES1); //set ICES1 , next edge is rising
		fallingTimeStamp=time;
		fallingIntCtr=intCtr;
		fallingTime=ICR1;

		return;
	}

	if( bit_is_set(TCCR1B, ICES1) )	//IRQ came from rising edge
	{
		TCCR1B &= ~(_BV(ICES1)); //clear ICES1, next edge is falling
		risingTimeStamp=time;
		risingIntCtr=intCtr;
		risingTime=ICR1;

		return;
	}
*/
}
