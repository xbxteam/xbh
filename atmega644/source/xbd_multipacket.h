#ifndef _XBD_MULTIPACKET_H
#define _XBD_MULTIPACKET_H

#include "global.h"
#include <avr/pgmspace.h>

#define ADDRSIZE 4
#define LENGSIZE 4
#define SEQNSIZE 4
#define TYPESIZE 4
#define TIMESIZE 4
#define NUMBSIZE 4
#define REVISIZE 5		//SVN revisions are 5 digit numbers

#define XBD_COMMAND_LEN ((u08) 8)
#define CRC16SIZE 2
#define XBD_ANSWERLENG_MAX 32
#define XBD_RESULTLEN_EBASH (XBD_COMMAND_LEN+1+2+CRC16SIZE)

u32 XBD_genSucessiveMultiPacket(const u08* srcdata, u08* dstbuf, u32 dstlenmax, const prog_char *code);
u32 XBD_genInitialMultiPacket(const u08* srcdata, u32 srclen, u08* dstbuf,const prog_char *code, u32 type, u32 addr);
u08 XBD_recSucessiveMultiPacket(const u08* recdata, u32 reclen, u08* dstbuf, u32 dstlenmax, const prog_char *code);
u08 XBD_recInitialMultiPacket(const u08* recdata, u32 reclen, const prog_char *code, u08 hastype, u08 hasaddr);

#endif /* _XBD_MULTIPACKET_H */