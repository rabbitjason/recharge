#ifndef _GPRS_H_
#define _GPRS_H_

int GPRS_Open(void);

void GPRS_Close(void);

int GPRS_IsOpen(void);

int GPRS_ThreadOpen();

void GPRS_ThreadOpenWait();

#endif

