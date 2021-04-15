#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "const.h"


void HexDump(const void *data, unsigned int len);

void Show(int key, const char * format, ...);

int Display_Query(QueryRequestInfo *qrt, QueryResponseInfo *qrs);

int Display_Pay(QueryRequestInfo *qrt, QueryResponseInfo *qrs, AlipayInfo *api, SaveCardInfo *sci);

int Display_Save(const unsigned char *card_data, const AlipayInfo *api, const SaveCardInfo *sci);

int Display_Save_Ex(QueryRequestInfo *qrt);


#endif

