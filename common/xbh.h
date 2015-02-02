#ifndef _XBH_H
#define _XBH_H

#include <avr/pgmspace.h>

#include "global.h"
#include "stack.h"

#define XBH_MAC_EEPROM_STORE	70	//6 byte
#define XBDIP_EEPROM_STORE      76      //4 byte
	
u08 XBH_HandleCodeDownloadRequest(const u08 *xbhcdr, u16 len);
u08 XBH_HandleSetCommunicationRequest(const u08 requestedComm);
u08 XBH_HandleDownloadParametersRequest(const u08 *xbhdpr, u16 len);
u08 XBH_HandleEXecutionRequest();
u08 XBH_HandleChecksumCalcRequest();
u08 XBH_HandleUploadResultsRequest(u08* p_answer);
u08 XBH_HandleStartApplicationRequest();
u08 XBH_HandleStartBootloaderRequest();
void XBH_HandleSTatusRequest(u08* p_answer);
u08 XBH_HandleResetControlRequest(u08 param);
u08 XBH_HandleStackUsageRequest(u08* p_answer);
u08 XBH_HandleTimingCalibrationRequest(u08* p_answer);
void XBH_HandleRePorttimestampRequest(u08* p_answer);
void XBH_HandleSubversionRevisionRequest(u08* p_answer);
u08 XBH_HandleTargetRevisionRequest(u08* p_answer);
u16 XBH_handle(const u08 *imput, u16 input_len, u08 *output);


#endif /* _XBH_H */
