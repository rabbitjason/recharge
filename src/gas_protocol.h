#ifndef _GAS_PROTOCOL_H_
#define _GAS_PROTOCOL_H_

#include "const.h"

#define bool char
#define true 1
#define false 0


int QueryCardReq_Package(unsigned char *sendData, const QueryRequestInfo *qrt);

int QueryCardRes_Parse(QueryResponseInfo *qrt, const char *recvData);

int QueryAliPayReq_Package(unsigned char *sendData, const AlipayInfo *api);

int QueryAliPayRes_Parse(SaveCardInfo* sci, unsigned char *recvData);

int SaveResponseInfo_Parse(SaveResponseInfo* sri, unsigned char *recvData);

int SaveQueryInfo_Package(unsigned char *sendData, const SaveQueryInfo *sqi);

int PayInfoReq_Package(unsigned char *sendData, const PayInfo *pi);

int PayInfoRes_Parse(PayInfoRes *pir, unsigned char *recvData);


#endif

