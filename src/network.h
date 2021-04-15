#ifndef _NETWORK_H_
#define _NETWORK_H_

#include "const.h"

int Network_QueryCard(QueryRequestInfo *qrt, QueryResponseInfo *qrs, int tran_code);

int Network_QueryPay(PayInfo *pi, const char *img_path, PayInfoRes *pir);

int Network_QueryAliPay(AlipayInfo *api, SaveCardInfo *sci);

int Network_SaveResult(SaveQueryInfo *sqi, SaveResponseInfo *sri);


#endif

