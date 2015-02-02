#include <util/delay.h>
#include <util/crc16.h>

#include "xbd_multipacket.h"
#include "xbh.h"
#include "XBH_version.h"
#include "xbh_utils.h"
#include "xbh_xbdcomm.h"

//#define XBH_DEBUG_TIMING

#define XBH_PROTO_VERSION "03"

const prog_char XBH_Rev[] = XBH_REVISION;

const prog_char XBHunk[] = "XBH"XBH_PROTO_VERSION"unk";

//Requests ('r') understood by XBH
const char XBHcdr[] PROGMEM = "XBH"XBH_PROTO_VERSION"cdr";
const char XBHexr[] PROGMEM = "XBH"XBH_PROTO_VERSION"exr";
const char XBHccr[] PROGMEM = "XBH"XBH_PROTO_VERSION"ccr";
const char XBHstr[] PROGMEM = "XBH"XBH_PROTO_VERSION"str";
const char XBHrcr[] PROGMEM = "XBH"XBH_PROTO_VERSION"rcr";
const char XBHsar[] PROGMEM = "XBH"XBH_PROTO_VERSION"sar";
const char XBHsbr[] PROGMEM = "XBH"XBH_PROTO_VERSION"sbr";
const char XBHdpr[] PROGMEM = "XBH"XBH_PROTO_VERSION"dpr";
const char XBHurr[] PROGMEM = "XBH"XBH_PROTO_VERSION"urr";
const char XBHrpr[] PROGMEM = "XBH"XBH_PROTO_VERSION"rpr";
const char XBHscr[] PROGMEM = "XBH"XBH_PROTO_VERSION"scr";
const char XBHsur[] PROGMEM = "XBH"XBH_PROTO_VERSION"sur";
const char XBHtcr[] PROGMEM = "XBH"XBH_PROTO_VERSION"tcr";
const char XBHsrr[] PROGMEM = "XBH"XBH_PROTO_VERSION"srr";	// (XBH) Subversion Revision Request
const char XBHtrr[] PROGMEM = "XBH"XBH_PROTO_VERSION"trr";	// (XBD) Target (Subversion) Revision Request
const char XBHlor[] PROGMEM = "XBH"XBH_PROTO_VERSION"lor";

//OK ('o') Responses from XBH
const char XBHcdo[] PROGMEM = "XBH"XBH_PROTO_VERSION"cdo";
const char XBHexo[] PROGMEM = "XBH"XBH_PROTO_VERSION"exo";
const char XBHcco[] PROGMEM = "XBH"XBH_PROTO_VERSION"cco";
const char XBHsto[] PROGMEM = "XBH"XBH_PROTO_VERSION"sto";
const char XBHrco[] PROGMEM = "XBH"XBH_PROTO_VERSION"rco";
const char XBHsao[] PROGMEM = "XBH"XBH_PROTO_VERSION"sao";
const char XBHsbo[] PROGMEM = "XBH"XBH_PROTO_VERSION"sbo";
const char XBHdpo[] PROGMEM = "XBH"XBH_PROTO_VERSION"dpo";
const char XBHuro[] PROGMEM = "XBH"XBH_PROTO_VERSION"uro";
const char XBHrpo[] PROGMEM = "XBH"XBH_PROTO_VERSION"rpo";
const char XBHsco[] PROGMEM = "XBH"XBH_PROTO_VERSION"sco";
const char XBHsuo[] PROGMEM = "XBH"XBH_PROTO_VERSION"suo";
const char XBHtco[] PROGMEM = "XBH"XBH_PROTO_VERSION"tco";
const char XBHsro[] PROGMEM = "XBH"XBH_PROTO_VERSION"sro";
const char XBHtro[] PROGMEM = "XBH"XBH_PROTO_VERSION"tro";
const char XBHloo[] PROGMEM = "XBH"XBH_PROTO_VERSION"loo";

//ACK ('a') Responses from XBH
const char XBHcda[] PROGMEM = "XBH"XBH_PROTO_VERSION"cda";

//Failed ('f') Responses from XBH (originates from bootloader)
const char XBHcdf[] PROGMEM = "XBH"XBH_PROTO_VERSION"cdf";
const char XBHrcf[] PROGMEM = "XBH"XBH_PROTO_VERSION"rcf";
const char XBHsaf[] PROGMEM = "XBH"XBH_PROTO_VERSION"saf";
const char XBHsbf[] PROGMEM = "XBH"XBH_PROTO_VERSION"sbf";
const char XBHdpf[] PROGMEM = "XBH"XBH_PROTO_VERSION"dpf";
const char XBHexf[] PROGMEM = "XBH"XBH_PROTO_VERSION"exf";
const char XBHurf[] PROGMEM = "XBH"XBH_PROTO_VERSION"urf";
const char XBHccf[] PROGMEM = "XBH"XBH_PROTO_VERSION"ccf";
const char XBHscf[] PROGMEM = "XBH"XBH_PROTO_VERSION"scf";
const char XBHsuf[] PROGMEM = "XBH"XBH_PROTO_VERSION"suf";
const char XBHtcf[] PROGMEM = "XBH"XBH_PROTO_VERSION"tcf";
const char XBHtrf[] PROGMEM = "XBH"XBH_PROTO_VERSION"trf";
const char XBHlof[] PROGMEM = "XBH"XBH_PROTO_VERSION"lof";

#define XBH_COMMAND_LEN (sizeof(XBHcdr)/sizeof(XBHcdr[0]) -1)


//XBD commands to send via TWI
const char XBDpfr[] PROGMEM = "XBD"XBH_PROTO_VERSION"pfr";
const char XBDexr[] PROGMEM = "XBD"XBH_PROTO_VERSION"exr";
const char XBDfdr[] PROGMEM = "XBD"XBH_PROTO_VERSION"fdr";
const char XBDsar[] PROGMEM = "XBD"XBH_PROTO_VERSION"sar";
const char XBDsbr[] PROGMEM = "XBD"XBH_PROTO_VERSION"sbr";
const char XBDvir[] PROGMEM = "XBD"XBH_PROTO_VERSION"vir";
const char XBDppr[] PROGMEM = "XBD"XBH_PROTO_VERSION"ppr";
const char XBDpdr[] PROGMEM = "XBD"XBH_PROTO_VERSION"pdr";
const char XBDurr[] PROGMEM = "XBD"XBH_PROTO_VERSION"urr";
const prog_char XBDrdr[]  = "XBD"XBH_PROTO_VERSION"rdr";
const prog_char XBDccr[]  = "XBD"XBH_PROTO_VERSION"ccr";
const prog_char XBDtcr[]  = "XBD"XBH_PROTO_VERSION"tcr";
const prog_char XBDsur[]  = "XBD"XBH_PROTO_VERSION"sur";
const prog_char XBDtrr[]  = "XBD"XBH_PROTO_VERSION"trr";

//OK ('o') Responses from bootloader
const char XBDpfo[] PROGMEM = "XBD"XBH_PROTO_VERSION"pfo";
const char XBDfdo[] PROGMEM = "XBD"XBH_PROTO_VERSION"fdo";
const char XBDppo[] PROGMEM = "XBD"XBH_PROTO_VERSION"ppo";
const char XBDpdo[] PROGMEM = "XBD"XBH_PROTO_VERSION"pdo";
const char XBDexo[] PROGMEM = "XBD"XBH_PROTO_VERSION"exo";
const char XBDuro[] PROGMEM = "XBD"XBH_PROTO_VERSION"uro";
const prog_char XBDrdo[]  = "XBD"XBH_PROTO_VERSION"rdo";
const prog_char XBDcco[]  = "XBD"XBH_PROTO_VERSION"cco";
const prog_char XBDtco[]  = "XBD"XBH_PROTO_VERSION"tco";
const prog_char XBDsuo[]  = "XBD"XBH_PROTO_VERSION"suo";
const prog_char XBDtro[]  = "XBD"XBH_PROTO_VERSION"tro";

// Version Information OK Responses
const char XBDviBL[] PROGMEM = "XBD"XBH_PROTO_VERSION"BLo";
const char XBDviAF[] PROGMEM = "XBD"XBH_PROTO_VERSION"AFo";



u16 resetTimerBase=10;	//10s is the initial base unit for resetTimer settings


#define XBD_PACKET_SIZE_MAX 255
u08 XBDCommandBuf[XBD_PACKET_SIZE_MAX];

u08 XBDResponseBuf[XBD_ANSWERLENG_MAX];


extern u32 xbd_recmp_datanext;
extern u32 xbd_recmp_dataleft;
extern volatile unsigned int resetTimer;

extern volatile unsigned long 	risingTimeStamp;
extern volatile unsigned char	risingIntCtr;
extern volatile unsigned int	risingTime;
extern volatile unsigned char   risingSeen;

extern volatile unsigned long 	fallingTimeStamp;
extern volatile unsigned char	fallingIntCtr;
extern volatile unsigned int	fallingTime;
extern volatile unsigned char   fallingSeen;




u08 XBH_HandleCodeDownloadRequest(const u08 *xbhcdr, u16 len)
{
        u16 ctr;
	u32 *p_u32 = (u32*)(XBDCommandBuf+XBD_COMMAND_LEN);
	u32 seqn=0,addr=0;
	u16 len_remain, fd_idx, numbytes;
	u08 buf[XBD_COMMAND_LEN+1];


/*
	for(u08 u=0;u<8;++u)
	{
			XBH_DEBUG("%c",xbhcdr[u]);
	}
*/
	constStringToBuffer (XBDCommandBuf, XBDpfr);

	//place and endian-convert ADDR (4 bytes)
	for(ctr=0; ctr < ADDRSIZE ; ++ctr)
	{
		/*
		XBDCommandBuf[XBD_COMMAND_LEN+ctr]=
				htoi(xbhcdr[6-(ctr*2)])<<4 |
				htoi(xbhcdr[6-(ctr*2)+1]);
		*/
		addr |= ( (u32)
				 (htoi(xbhcdr[(ctr*2)])<<4 |
				  htoi(xbhcdr[(ctr*2)+1]))
				) <<((3-ctr)*8);
	}

//	XBH_DEBUG("!!ADR: [%x]\n",addr);
	*p_u32 = HTONS32(addr);
//	XBH_DEBUG("!!!ADR: [%x]\n",*p_u32);

	//place LENG (in correct endianess)
	//(u32*)(XBDCommandBuf+XBD_COMMAND_LEN+ADDRSIZE);
	++p_u32;
	*p_u32 = HTONS32((u32)(len-ADDRSIZE));

	xbdSend(XBD_COMMAND_LEN+ADDRSIZE+LENGSIZE, XBDCommandBuf);
	xbdReceive(XBD_COMMAND_LEN, XBDResponseBuf);
//	XBH_DEBUG("pfr\n");


	
	constStringToBuffer(buf, XBDpfo);
	if(0 != memcmp(buf,XBDResponseBuf,XBH_COMMAND_LEN))
	{
	        XBH_ERROR("pfr fail\n");
		return 1;
	}


	constStringToBuffer (XBDCommandBuf, XBDfdr);
	len_remain=(len-ADDRSIZE);
	fd_idx = ADDRSIZE;
	p_u32 = (u32 *)&XBDCommandBuf[XBD_COMMAND_LEN];
	//XBH_DEBUG("A");
	while(len_remain != 0)
	{
		*p_u32 = HTONS32(seqn);
		++seqn;
		for(ctr=0; ctr < (len_remain<128?len_remain:128) ; ++ctr)
		{
			XBDCommandBuf[XBD_COMMAND_LEN+SEQNSIZE+ctr]=
				htoi(xbhcdr[((fd_idx+ctr)*2)])<<4 |
				htoi(xbhcdr[((fd_idx+ctr)*2)+1]);
		}
		numbytes=ctr;
	//	XBH_DEBUG("C");
		xbdSend(XBD_COMMAND_LEN+SEQNSIZE+numbytes, XBDCommandBuf);
		xbdReceive(XBD_COMMAND_LEN, XBDResponseBuf);
	//	XBH_DEBUG("D");
				
		constStringToBuffer(buf, XBDpfo);
	//	XBH_DEBUG("E");
		if(0 == memcmp(buf,XBDResponseBuf,XBH_COMMAND_LEN))
		{
	//		XBH_DEBUG("F");
			len_remain-=numbytes;
			fd_idx+=numbytes;
		}
		else
		{
		        XBH_ERROR("fdr fail\n");     
	//		XBH_DEBUG("H*");
			return 2;
		}	
	//	XBH_DEBUG("I");
	}
	//XBH_DEBUG("K*");
	return 0;

}

u08 XBH_HandleSetCommunicationRequest(const u08 requestedComm)
{
	XBH_DEBUG("SetComm\n");
	switch(requestedComm) {
		case 'U':
			xbdCommExit();
			xbdCommInit(COMM_UART);
		break;
		case 'O':
			xbdCommExit();
			xbdCommInit(COMM_UART_OVERDRIVE);
		break;
		case 'I':
      xbdCommExit();
      xbdCommInit(COMM_I2C);
	    break;
		case 'E':
      xbdCommExit();
      xbdCommInit(COMM_ETHERNET);
	    break;
		default:
			XBH_ERROR("Invalid set communication request %X\n",requestedComm);
			return 2;
		break;
	}
	return(0);
}

u08 XBH_HandleDownloadParametersRequest(const u08 *xbhdpr, u16 len)
{
	u16 ctr;
	u32 *p_u32 = (u32*)(XBDCommandBuf+XBD_COMMAND_LEN);//+TYPESIZE+ADDRSIZE);
	u32 seqn=0,temp=0;
	u16 len_remain, fd_idx, numbytes;
	u08 buf[XBD_COMMAND_LEN+1];
/*
	for(u08 u=0;u<20;++u)
	{
			XBH_DEBUG("%x ",xbhdpr[u]);
	}
*/
	constStringToBuffer (XBDCommandBuf, XBDppr);

	//place and endian-convert TYPE (4 bytes)
	for(ctr=0; ctr < TYPESIZE ; ++ctr)
	{
		/*
		XBDCommandBuf[XBD_COMMAND_LEN+ctr]=
				htoi(xbhdpr[6-(ctr*2)])<<4 |
				htoi(xbhdpr[6-(ctr*2)+1]);
		*/
		temp |= ( (u32)
				 (htoi(xbhdpr[(ctr*2)])<<4 |
				  htoi(xbhdpr[(ctr*2)+1]))
				) <<((3-ctr)*8);
	}
	*p_u32 = HTONS32(temp);
	p_u32+=TYPESIZE/4;
	temp=0;
	//place and endian-convert ADDR (4 bytes)
	for(ctr=0; ctr < ADDRSIZE ; ++ctr)
	{
		/*
		XBDCommandBuf[XBD_COMMAND_LEN+TYPESIZE+ctr]=
				htoi(xbhdpr[2*TYPESIZE+6-(ctr*2)])<<4 |
				htoi(xbhdpr[2*TYPESIZE+6-(ctr*2)+1]);
		*/
		temp |= ( (u32)
				 (htoi(xbhdpr[(TYPESIZE+ctr)*2])<<4 |
				  htoi(xbhdpr[(TYPESIZE+ctr)*2+1]))
				) <<((3-ctr)*8);
	}
	*p_u32 = HTONS32(temp);
	p_u32+=ADDRSIZE/4;
	temp=0;

	//place LENG (in correct endianess)
	*p_u32 =  HTONS32((u32)(len-ADDRSIZE-TYPESIZE));

/*
	XBH_DEBUG("XBDppr:\n");

	for(u08 u=0;u<XBD_COMMAND_LEN+TYPESIZE+ADDRSIZE+LENGSIZE;++u)
	{
			XBH_DEBUG("%x ",XBDCommandBuf[u]);
	}
	XBH_DEBUG("\r\n");
*/

	xbdSend(XBD_COMMAND_LEN+TYPESIZE+ADDRSIZE+LENGSIZE, XBDCommandBuf);
	xbdReceive(XBD_COMMAND_LEN, XBDResponseBuf);

	
	constStringToBuffer(buf, XBDppo);
	if(0 != memcmp(buf,XBDResponseBuf,XBH_COMMAND_LEN))
	{
		XBH_ERROR("XBDpp failed:%s",XBDResponseBuf);
		return 1;
	}


	constStringToBuffer (XBDCommandBuf, XBDpdr);
	len_remain=(len-TYPESIZE-ADDRSIZE);
	fd_idx = ADDRSIZE+TYPESIZE;
	p_u32 = (u32 *)&XBDCommandBuf[XBD_COMMAND_LEN];
//	XBH_DEBUG("A");
	while(len_remain != 0)
	{
		*p_u32 =  HTONS32(seqn);
		++seqn;
		for(ctr=0; ctr < (len_remain<128?len_remain:128) ; ++ctr)
		{
			XBDCommandBuf[XBD_COMMAND_LEN+SEQNSIZE+ctr]=
				htoi(xbhdpr[((fd_idx+ctr)*2)])<<4 |
				htoi(xbhdpr[((fd_idx+ctr)*2)+1]);
		}
		numbytes=ctr;
//		XBH_DEBUG("B");

	//	XBH_DEBUG("C");
		xbdSend(len=XBD_COMMAND_LEN+SEQNSIZE+numbytes, XBDCommandBuf);
		xbdReceive(XBD_COMMAND_LEN, XBDResponseBuf);
	//	XBH_DEBUG("D");
	/*		XBH_DEBUG("MSG to XBD, len=%i:\n",len);
		for(u08 u=0;u<len;++u)
		{
				XBH_DEBUG("%x ",XBDCommandBuf[u]);
		}
		XBH_DEBUG("\r\n");			
*/
		constStringToBuffer(buf, XBDpdo);
	//	XBH_DEBUG("E");
		if(0 == memcmp(buf,XBDResponseBuf,XBH_COMMAND_LEN))
		{
	//		XBH_DEBUG("F");
			len_remain-=numbytes;
			fd_idx+=numbytes;
		}
		else
		{
		        XBH_ERROR("pdr fail\n");
			return 2;
		}	
//		XBH_DEBUG("I");
	}
//	XBH_DEBUG("K*");
	return 0;

}

u08 XBH_HandleEXecutionRequest()
{
	constStringToBuffer (XBDCommandBuf, XBDexr);

	xbdSend(XBD_COMMAND_LEN, XBDCommandBuf);
	XBH_DEBUG("EXr\n");

	xbdReceive(XBD_COMMAND_LEN, XBDResponseBuf);
	
//	XBH_DEBUG("%s",((u08*)XBDResponseBuf));

	if(0 == memcmp_P(XBDResponseBuf,XBDexo,XBD_COMMAND_LEN))
	{	
		//XBH_DEBUG("->OK\n");
/*		XBH_DEBUG("Took %d s, %d IRQs, %d clocks.\n",
						risingTimeStamp-fallingTimeStamp,
						risingIntCtr-fallingIntCtr,
						risingTime-fallingTime);
*/		return 0;
	}
	else
	{
		XBH_ERROR("EXr fail\n");
		return 1;
	}
}

u08 XBH_HandleChecksumCalcRequest()
{
	constStringToBuffer (XBDCommandBuf, XBDccr);
	xbdSend(XBD_COMMAND_LEN, XBDCommandBuf);
	XBH_DEBUG("CCr\n");

	xbdReceive(XBD_COMMAND_LEN, XBDResponseBuf);
	
//	XBH_DEBUG("%s",((u08*)XBDResponseBuf));

	if(0 == memcmp_P(XBDResponseBuf,XBDcco,XBD_COMMAND_LEN))
	{	
/*		XBH_DEBUG("Took %d s, %d IRQs, %d clocks.\n",
				risingTimeStamp-fallingTimeStamp,
				risingIntCtr-fallingIntCtr,
				risingTime-fallingTime);
*/		return 0;
	}
	else
	{
		XBH_ERROR("CCr fail\n");
		return 1;
	}
}


u08 XBH_HandleUploadResultsRequest(u08* p_answer)
{
	u08 ret;

	constStringToBuffer (XBDCommandBuf, XBDurr);

	XBH_DEBUG("URr\n");
/*	for(u08 u=0;u<XBD_COMMAND_LEN;++u)
	{
			XBH_DEBUG("%x ",XBDCommandBuf[u]);
	}
	XBH_DEBUG("\r\n");
*/
	xbdSend(XBD_COMMAND_LEN, XBDCommandBuf);
//	_delay_ms(100);


	xbdReceive(XBD_ANSWERLENG_MAX-CRC16SIZE, XBDResponseBuf);
	
	XBD_recInitialMultiPacket(XBDResponseBuf,XBD_ANSWERLENG_MAX-CRC16SIZE,XBDuro, 1 /*hastype*/, 0/*hasaddr*/);

/*
	u08 XBDResponseBuf[XBD_ANSWERLENG_MAX];
	u08 XBHMultiPacketDecodeBuf[XBD_ANSWERLENG_MAX];

	for(u08 u=0;u<XBD_RESULTLEN_EBASH;++u)
	{
			XBH_DEBUG("%x ",XBDResponseBuf[u]);
	}
	XBH_DEBUG("\r\n");
*/

	constStringToBuffer (XBDCommandBuf, XBDrdr);

	do
	{
		xbdSend(XBD_COMMAND_LEN, XBDCommandBuf);
//		_delay_ms(100);

		XBH_DEBUG("RDr\n");
/*		for(u08 u=0;u<XBD_COMMAND_LEN;++u)
		{
				XBH_DEBUG("%x ",XBDCommandBuf[u]);
		}
		XBH_DEBUG("\r\n");		
*/
	//	XBH_DEBUG("1.xbd_recmp_dataleft: %i\n",xbd_recmp_dataleft);
		xbdReceive(XBD_ANSWERLENG_MAX-CRC16SIZE, XBDResponseBuf);
		ret=XBD_recSucessiveMultiPacket(XBDResponseBuf, XBD_ANSWERLENG_MAX-CRC16SIZE, p_answer, MTU_SIZE/2, XBDrdo);
	//	XBH_DEBUG("2.xbd_recmp_dataleft: %i\n",xbd_recmp_dataleft);
	//	XBH_DEBUG("ret: %i\n",ret);
	}
	while(xbd_recmp_dataleft != 0 && 0 == ret);



	if(ret==0 && 0 == memcmp_P(XBDResponseBuf,XBDrdo,XBD_COMMAND_LEN))
	{	
		s16 i;
		
		
		for(i=xbd_recmp_datanext-1;i>=0; --i)
		{
			p_answer[2*i] = ntoa(p_answer[i]>>4);
			p_answer[2*i+1] = ntoa(p_answer[i]&0xf);
		}
		return 0;
	}
	else
	{
		XBH_ERROR("RDR fail\n");
		return 0x80+ret;
	}
}


u08 XBH_HandleStartApplicationRequest()
{
        u16 trys;
	volatile u08 yes='y';
	volatile u08 no='n';

	trys=0;
	while(trys<3)
	{

		constStringToBuffer (XBDCommandBuf, XBDsar);
		xbdSend(XBD_COMMAND_LEN, XBDCommandBuf);
		_delay_ms(100);
		XBH_DEBUG("sar\n");

		constStringToBuffer (XBDCommandBuf, XBDvir);
		xbdSend(XBD_COMMAND_LEN, XBDCommandBuf);

		XBH_DEBUG("vir\n");

		xbdReceive(XBD_COMMAND_LEN, XBDResponseBuf);
	

		if(0 == memcmp_P(XBDResponseBuf,XBDviAF,XBD_COMMAND_LEN))
		{	
			return 0;
		}	
		else
		{
   		        XBH_WARN("Try %d: [%s]",trys,((u08*)XBDResponseBuf));
			++trys;
			XBH_HandleResetControlRequest(yes);
			_delay_ms(100);
			XBH_HandleResetControlRequest(no);
			_delay_ms(1000);
		}
	}


	XBH_ERROR("vir fail\n");
	return 1;

}

u08 XBH_HandleStartBootloaderRequest()
{
	u16 trys;
	volatile u08 yes='y';
	volatile u08 no='n';

	trys=0;
	while(trys<3)
	{

		constStringToBuffer (XBDCommandBuf, XBDsbr);
		xbdSend(XBD_COMMAND_LEN, XBDCommandBuf);

		_delay_ms(100);

		constStringToBuffer (XBDCommandBuf, XBDvir);
		xbdSend(XBD_COMMAND_LEN, XBDCommandBuf);


		XBH_DEBUG("vir\n");
		xbdReceive(XBD_COMMAND_LEN, XBDResponseBuf);
		if(0 == memcmp_P(XBDResponseBuf,XBDviBL,XBD_COMMAND_LEN))
		{	
			return 0;
		}
		else
		{
        		XBH_WARN("Try %d: [%s]",trys,((u08*)XBDResponseBuf));
			++trys;
			XBH_HandleResetControlRequest(yes);
			_delay_ms(100);
			XBH_HandleResetControlRequest(no);
			_delay_ms(1000);
		}
	}
	XBH_WARN("bl vir fail\n");
	return 1;
	
}

void XBH_HandleSTatusRequest(u08* p_answer)
{
	constStringToBuffer (XBDCommandBuf, XBDvir);
	xbdSend(XBD_COMMAND_LEN, XBDCommandBuf);
	xbdReceive(XBD_COMMAND_LEN, p_answer);
}

u08 XBH_HandleResetControlRequest(u08 param)
{
	//Disable pull-up in input, set to low level if output
	PORTB &= ~(_BV(PB0));

	if(param == 'y' || param == 'Y')
	{
		//set DDR bit -> output
		DDRB |= _BV(PB0);
	
		return 0;
	}
	
	if(param == 'n' || param == 'N')
	{
		//clear DDR bit -> input
		DDRB &=  ~(_BV(PB0));

	
		return 0;
	}

	XBH_ERROR("Invalid Reset Request\n");
	return 1;
}	

u08 XBH_HandleStackUsageRequest(u08* p_answer)
{

	u08 i;

	constStringToBuffer (XBDCommandBuf, XBDsur);
//	XBH_DEBUG("sur\n");

	memset(XBDResponseBuf,'-',XBD_COMMAND_LEN+NUMBSIZE);
	xbdSend(XBD_COMMAND_LEN, XBDCommandBuf);
	xbdReceive(XBD_COMMAND_LEN+NUMBSIZE, XBDResponseBuf);

//	XBH_DEBUG("Answer: [%s]",((u08*)XBDResponseBuf));

	if(0 == memcmp_P(XBDResponseBuf,XBDsuo,XBD_COMMAND_LEN))
	{	

		//Copy to Stack Usage information for transmission to the XBS
		for(i=0;i<4;++i)
		{
			p_answer[2*i] = ntoa(XBDResponseBuf[XBD_COMMAND_LEN+i]>>4);
			p_answer[2*i+1] = ntoa(XBDResponseBuf[XBD_COMMAND_LEN+i]&0xf);
		}
		return 0;
	}
	else
	{
		XBH_ERROR("sur response wrong [%s]\n",XBDResponseBuf);
		return 1;
	}
}

u08 XBH_HandleTimingCalibrationRequest(u08* p_answer)
{

	u08 i;

	constStringToBuffer (XBDCommandBuf, XBDtcr);
	memset(XBDResponseBuf,'-',XBD_COMMAND_LEN+NUMBSIZE);
	xbdSend(XBD_COMMAND_LEN, XBDCommandBuf);
	xbdReceive(XBD_COMMAND_LEN+NUMBSIZE, XBDResponseBuf);

	XBH_DEBUG("tcr\n");

//	XBH_DEBUG("Answer: [%s]",((u08*)XBDResponseBuf));

	if(0 == memcmp_P(XBDResponseBuf,XBDtco,XBD_COMMAND_LEN))
	{	

		for(i=0;i<4;++i)
		{
			p_answer[2*i] = ntoa(XBDResponseBuf[XBD_COMMAND_LEN+i]>>4);
			p_answer[2*i+1] = ntoa(XBDResponseBuf[XBD_COMMAND_LEN+i]&0xf);
		}
		return 0;
	}
	else
	{
		XBH_ERROR("Response wrong [%s]\n",XBDResponseBuf);
		return 1;
	}

}

void XBH_HandleRePorttimestampRequest(u08* p_answer)	
{

	u32 tmpvar;
	u08 i;
	s32 rising_fract, falling_fract;
	

#ifdef XBH_DEBUG_TIMING
	XBH_DEBUG("\nRP, rising: %d s, %d IRQs, clocks:",
						risingTimeStamp,
						risingIntCtr);
	XBD_debugOutHex32Bit(risingTime);

	XBH_DEBUG("\nRP, falling: %d s, %d IRQs, clocks:",
						fallingTimeStamp,
						fallingIntCtr);
	XBD_debugOutHex32Bit(fallingTime);
#endif

	rising_fract=risingIntCtr*OCRVAL+risingTime;

#ifdef XBH_DEBUG_TIMING
	XBH_DEBUG("\nrising_fract:");
	XBD_debugOutHex32Bit(rising_fract);
#endif

	falling_fract=fallingIntCtr*OCRVAL+fallingTime;
#ifdef XBH_DEBUG_TIMING
	XBH_DEBUG("\nfalling_fract:");
	XBD_debugOutHex32Bit(falling_fract);
#endif

	if(rising_fract < falling_fract)
	{
		rising_fract+=F_CPU;
		--risingTimeStamp;
	}

#ifdef XBH_DEBUG_TIMING
	XBH_DEBUG("\nrising_fract:");
	XBD_debugOutHex32Bit(rising_fract);

	XBH_DEBUG("RP, rising: %d s, %d IRQs, %d clocks.\n",
						risingTimeStamp,
						risingIntCtr,
						risingTime);
#endif


	tmpvar = (risingTimeStamp-fallingTimeStamp);
	for(i=0;i<8;++i)
	{
		*p_answer++ = ntoa((tmpvar>>(28-(4*i)))&0xf);
	}

#ifdef XBH_DEBUG_TIMING
	XBH_DEBUG("\ntmpvar:");
	XBD_debugOutHex32Bit(tmpvar);
#endif
					
	tmpvar = (rising_fract - falling_fract);
	for(i=0;i<8;++i)
	{
		*p_answer++ = ntoa((tmpvar>>(28-(4*i)))&0xf);
	}

#ifdef XBH_DEBUG_TIMING
	XBH_DEBUG("\ntmpvar:");
	XBD_debugOutHex32Bit(tmpvar);
#endif


	tmpvar = (F_CPU);
	for(i=0;i<8;++i)
	{
		*p_answer++ = ntoa((tmpvar>>(28-(4*i)))&0xf);
	}
	
#ifdef XBH_DEBUG_TIMING
	XBH_DEBUG("\ntmpvar:");
	XBD_debugOutHex32Bit(tmpvar);
#endif
}



void XBH_HandleSubversionRevisionRequest(u08* p_answer)	
{
	u08 i;
	u08 svnRevLen = strlen(XBH_Rev);


	// Report SVN Rev in 5 digits length
	for(i=0;i<5-svnRevLen;++i)
	{
		p_answer[i]='0';
	}
	constStringToBufferN(p_answer+i,XBH_Rev,5);

	p_answer[5]=',';

	for(i=0;i<6;++i)
	{
		p_answer[6+2*i]=ntoa(eeprom_mac[i]>>4);
		p_answer[6+2*i+1]=ntoa(eeprom_mac[i]&0xf);
	}
	p_answer[6+2*6+1]=0;
	return;

}

u08 XBH_HandleTargetRevisionRequest(u08* p_answer)
{

	u08 i;

	constStringToBuffer (XBDCommandBuf, XBDtrr);
	memset(XBDResponseBuf,'-',XBD_COMMAND_LEN+REVISIZE);
	xbdSend(XBD_COMMAND_LEN, XBDCommandBuf);
	xbdReceive(XBD_COMMAND_LEN+REVISIZE, XBDResponseBuf);

	XBH_DEBUG("trr\n");

//	XBH_DEBUG("Answer: [%s]",((u08*)XBDResponseBuf));

	if(0 == memcmp_P(XBDResponseBuf,XBDtro,XBD_COMMAND_LEN))
	{	
		for(i=0;i<REVISIZE;++i)
		{
			p_answer[i] = XBDResponseBuf[XBD_COMMAND_LEN+i];
		}
		return 0;
	}
	else
	{
	        XBH_ERROR("trr fail\n");
		return 1;
	}

}

u08 XBH_HandleLOopbackRequest(u08* p_answer)
{

	u08 i;

	constStringToBuffer (XBDCommandBuf, XBDtrr);
	memset(XBDResponseBuf,'-',XBD_COMMAND_LEN+REVISIZE);
	xbdSend(XBD_COMMAND_LEN, XBDCommandBuf);
	xbdReceive(XBD_COMMAND_LEN+REVISIZE, XBDResponseBuf);

	XBH_DEBUG("trr\n");

//	XBH_DEBUG("Answer: [%s]",((u08*)XBDResponseBuf));

	if(0 == memcmp_P(XBDResponseBuf,XBDtro,XBD_COMMAND_LEN))
	{	
		for(i=0;i<REVISIZE;++i)
		{
			p_answer[i] = XBDResponseBuf[XBD_COMMAND_LEN+i];
		}
		return 0;
	}
	else
	{
	        XBH_ERROR("loopback fail\n");
		return 1;
	}

}

u16 XBH_handle(const u08 *input, u16 input_len, u08* output) {
	u08 buf[XBH_COMMAND_LEN+1];
	u08 ret;
	constStringToBuffer (buf, XBHurr);
	if
	( 
		(0 == memcmp(buf,input,XBH_COMMAND_LEN)) 
	)
	{
		resetTimer=resetTimerBase*1;
		// XBH_DEBUG("## Proper 'u'pload 'r'esults request received.\n");
		
		ret=XBH_HandleUploadResultsRequest(&output[XBH_COMMAND_LEN]);

		resetTimer=0;		

		if(0 == ret)
		{
			constStringToBufferN (output, XBHuro, XBH_COMMAND_LEN);
			return (u16)XBH_COMMAND_LEN+xbd_recmp_datanext*2;
		}
		else
		{
			constStringToBuffer (output, XBHurf);
			output[XBH_COMMAND_LEN]= ntoa(ret>>4);
			output[XBH_COMMAND_LEN+1]= ntoa(ret&0xf);
			return (u16) XBH_COMMAND_LEN+2;
		}
	}

	constStringToBuffer (  buf, XBHcdr);
	if
	( 
		(0 == memcmp(buf,input,XBH_COMMAND_LEN)) 
	)
	{
		resetTimer=resetTimerBase*1;
		//XBH_DEBUG("## Proper 'c'ode 'd'ownload request received.\n");
//		XBH_DEBUG("cd\n");

		ret=XBH_HandleCodeDownloadRequest(&input[XBH_COMMAND_LEN],(input_len-XBH_COMMAND_LEN)/2);

		resetTimer=0;		


		if(0 == ret)
		{
			constStringToBufferN (output, XBHcdo, XBH_COMMAND_LEN);
			return (u16) XBH_COMMAND_LEN;
		}
		else
		{
		        XBH_ERROR("cd fail\n");
			constStringToBuffer (output, XBHcdf);
			return (u16) XBH_COMMAND_LEN;
		}	
	}

	constStringToBuffer (  buf, XBHdpr);
	if
	( 
		(0 == memcmp(buf,input,XBH_COMMAND_LEN)) 
	)
	{
	//	XBH_DEBUG("## Proper 'd'ownload 'p'arameters request received.\n");
		resetTimer=resetTimerBase*1;

		//prepare TWI transmission to XBD here
		ret=XBH_HandleDownloadParametersRequest(&input[XBH_COMMAND_LEN],(input_len-XBH_COMMAND_LEN)/2);

		resetTimer=0;		


		if(0 == ret)
		{
			constStringToBufferN (output, XBHdpo, XBH_COMMAND_LEN);
			return (u16) XBH_COMMAND_LEN;
		}
		else
		{
			constStringToBuffer (output, XBHdpf);
			return (u16) XBH_COMMAND_LEN;
		}
	}



	constStringToBuffer (  buf, XBHscr);
	if
	( 
		(0 == memcmp(buf,input,XBH_COMMAND_LEN)) 
	)
	{
		resetTimer=resetTimerBase*1;
		ret=XBH_HandleSetCommunicationRequest(input[XBH_COMMAND_LEN]);
		resetTimer=0;
		
		if(ret==0)	
			constStringToBufferN (output, XBHsco, XBH_COMMAND_LEN);
		else
		 	constStringToBuffer (output, XBHscf);
		
		return (u16) XBH_COMMAND_LEN;
	}	

	constStringToBuffer (  buf, XBHrpr);
	if
	( 
		(0 == memcmp(buf,input,XBH_COMMAND_LEN)) 
	)
	{
		resetTimer=resetTimerBase*1;
		XBH_HandleRePorttimestampRequest(&output[XBH_COMMAND_LEN]);
		resetTimer=0;
		
		constStringToBufferN (output, XBHrpo,XBH_COMMAND_LEN);
		return (u16) XBH_COMMAND_LEN+(3*TIMESIZE*2);
	}
	
	constStringToBuffer (  buf, XBHstr);
	if
	( 
		(0 == memcmp(buf,input,XBH_COMMAND_LEN)) 
	)
	{
		resetTimer=resetTimerBase*1;
		XBH_HandleSTatusRequest(&output[XBH_COMMAND_LEN]);
		resetTimer=0;
		
		constStringToBufferN (output, XBHsto,XBH_COMMAND_LEN);
		return (u16) 2*XBH_COMMAND_LEN;
	}

	constStringToBuffer (  buf, XBHrcr);
	if
	( 
		(0 == memcmp(buf,input,XBH_COMMAND_LEN)) 
	)
	{
		resetTimer=resetTimerBase*1;
		ret=XBH_HandleResetControlRequest(input[XBH_COMMAND_LEN]);
		resetTimer=0;		
		
		if(0 == ret)
		{
			constStringToBuffer (output, XBHrco);
			return (u16) XBH_COMMAND_LEN;
		}
		else
		{
			constStringToBuffer (output, XBHrcf);
			return (u16) XBH_COMMAND_LEN;
		}

	}

	constStringToBuffer (  buf, XBHsar);
	if
	( 
		(0 == memcmp(buf,input,XBH_COMMAND_LEN)) 
	)
	{
		resetTimer=resetTimerBase*2;		
		ret=XBH_HandleStartApplicationRequest();
		resetTimer=0;
		
		if(0 == ret)
		{
			constStringToBuffer (output, XBHsao);
			return (u16) XBH_COMMAND_LEN;
		}
		else
		{
			constStringToBuffer (output, XBHsaf);
			return (u16) XBH_COMMAND_LEN;
		}
	}

	constStringToBuffer (  buf, XBHsur);
	if
	( 
		(0 == memcmp(buf,input,XBH_COMMAND_LEN)) 
	)
	{
		resetTimer=resetTimerBase*1;
		ret=XBH_HandleStackUsageRequest(&output[XBH_COMMAND_LEN]);
		resetTimer=0;
		
		if(0 == ret)
		{
			constStringToBufferN (output, XBHsuo, XBH_COMMAND_LEN);
			return (u16) XBH_COMMAND_LEN+2*NUMBSIZE;
		}
		else
		{
			constStringToBuffer (output, XBHsuf);
			return (u16) XBH_COMMAND_LEN;
		}
	}


	constStringToBuffer (  buf, XBHtcr);
	if
	( 
		(0 == memcmp(buf,input,XBH_COMMAND_LEN)) 
	)
	{
		resetTimer=resetTimerBase*2;		
		ret=XBH_HandleTimingCalibrationRequest(&output[XBH_COMMAND_LEN]);
		resetTimer=0;
		
		if(0 == ret)
		{
			constStringToBufferN (output, XBHtco, XBH_COMMAND_LEN);
			return (u16) XBH_COMMAND_LEN+2*NUMBSIZE;
		}
		else
		{
			constStringToBuffer (output, XBHtcf);
			return (u16) XBH_COMMAND_LEN;
		}
	}

	constStringToBuffer (  buf, XBHsbr);
	if
	( 
		(0 == memcmp(buf,input,XBH_COMMAND_LEN)) 
	)
	{
		resetTimer=resetTimerBase*2;	
		ret=XBH_HandleStartBootloaderRequest();
		resetTimer=0;
		
		if(0 == ret)
		{
			constStringToBuffer (output, XBHsbo);
			return (u16) XBH_COMMAND_LEN;
		}
		else
		{
			constStringToBuffer (output, XBHsbf);
			return (u16) XBH_COMMAND_LEN;
		}
	}

	constStringToBuffer (  buf, XBHexr);
	if
	( 
		(0 == memcmp(buf,input,XBH_COMMAND_LEN)) 
	)
	{
		resetTimer=resetTimerBase*12;
		ret=XBH_HandleEXecutionRequest();
		resetTimer=0;

		if(0 == ret)
		{
			constStringToBuffer (output, XBHexo);
			return (u16) XBH_COMMAND_LEN;
		}
		else
		{
			constStringToBuffer (output, XBHexf);
			return (u16) XBH_COMMAND_LEN;
		}
	}


	constStringToBuffer (  buf, XBHccr);
	if
	( 
		(0 == memcmp(buf,input,XBH_COMMAND_LEN)) 
	)
	{
		resetTimer=resetTimerBase*60;
		ret=XBH_HandleChecksumCalcRequest();
		resetTimer=0;

		if(0 == ret)
		{
			constStringToBuffer (output, XBHcco);
			return (u16) XBH_COMMAND_LEN;
		}
		else
		{
			constStringToBuffer (output, XBHccf);
			return (u16) XBH_COMMAND_LEN;
		}
	}

	constStringToBuffer (  buf, XBHsrr);	
	if
	( 
		(0 == memcmp(buf,input,XBH_COMMAND_LEN)) 
	)
	{
		resetTimer=resetTimerBase;
		XBH_HandleSubversionRevisionRequest(&output[XBH_COMMAND_LEN]);
		resetTimer=0;

		constStringToBufferN (output, XBHsro,XBH_COMMAND_LEN);
		return (u16) XBH_COMMAND_LEN+5+1+12;		// 5 chars SVN Rev, ',' , 6*2 chars MAC address
	}

	
	constStringToBuffer (buf, XBHtrr);	
	if
	( 
		(0 == memcmp(buf,input,XBH_COMMAND_LEN)) 
	)
	{
		resetTimer=resetTimerBase*2;
		ret = XBH_HandleTargetRevisionRequest(&output[XBH_COMMAND_LEN]);
		resetTimer=0;
		if(0 == ret)
		{
			constStringToBufferN (output, XBHtro,XBH_COMMAND_LEN);
			return (u16) XBH_COMMAND_LEN+REVISIZE;
		}
		else
		{
			constStringToBuffer (output, XBHtrf);
			return (u16) XBH_COMMAND_LEN;
		}
	}
	constStringToBuffer (output, XBHunk);
	XBH_ERROR("UNKnown command received (len: %d):",input_len);		
	for(u08 u=0;u<input_len;++u)
	{
	  if(u % 32 == 0) XBH_DEBUG("\n");		
	  XBH_DEBUG("%x ",input[u]);
	}
	XBH_DEBUG("\n");		
	
	return (u16) XBH_COMMAND_LEN;
}
