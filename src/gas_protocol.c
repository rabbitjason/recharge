#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "gas_protocol.h"

char* StringSplitEx(char *strToken, const char *strDelimit )
{
	static char *token = NULL;
	static char *pos = NULL;
	
	if (pos != NULL && pos[0] == *strDelimit) {
		pos += 1;
		return "";
	}

	token = strtok(strToken, strDelimit);

	if (token != NULL)
	{
		pos = token + strlen(token) + 1;
	}

	return token;
}


int append_field(unsigned char *pos, const void* pvData, int len, bool isTail)
{
	unsigned char *p = pos;
	char ch = '|';
	memcpy(p, pvData, len);
	if (!isTail)
	{
		return len;	
	}
	p += len;
	memcpy(p, &ch, 1);
	return len + 1;
}


int PayInfoRes_Parse(PayInfoRes *pir, unsigned char *recvData)
{
	if (NULL == pir || NULL == recvData)
	{
		return -1;
	}

	char *token = NULL;
	char *pos = NULL;

	token = StringSplitEx((char*)recvData, "|"); 
	if (token != NULL)
	{
		pir->len = atoi(token);
	}

	token = StringSplitEx(NULL, "|"); 
	if (token != NULL)  
	{  	
		pir->response_code = atoi(token);
	} 


	token = StringSplitEx(NULL, "|");
	if (token != NULL)  
	{  
		strcpy(pir->pay_code, token);
	}

	token = StringSplitEx(NULL, "|");
	if (token != NULL)  
	{  
		strcpy(pir->alipay_code, token);
	}

	if (token != NULL)  
	{ 
		pos = token + strlen(token) + 1;
		memcpy(pir->pic_data, pos, pir->len);
	}

	return pir->len;
}



int QueryCardRes_Parse(QueryResponseInfo *qrs, const char *recvData)
{

	char szHexVal[3] = {0};
	int i = 0, j = 0;
		

	if (NULL == qrs || NULL == recvData)
	{
		return -1;
	}

	char *token = NULL;
	char *pos = NULL;

	token = StringSplitEx(recvData, "|"); 
	if (token != NULL)
	{
		qrs->len = atoi(token);
	}

	token = StringSplitEx(NULL, "|"); 
	if (token != NULL)  
	{  	
		qrs->response_code = atoi(token);
	} 

	token = StringSplitEx(NULL, "|");
	if (token != NULL)  
	{  
		strcpy(qrs->response_desc, token);
	}

	token = StringSplitEx(NULL, "|"); 
	if (token != NULL)  
	{  
		qrs->card_type = atoi(token);
	}
	token = StringSplitEx(NULL, "|");
	if (token != NULL)  
	{  
		strcpy(qrs->user_code, token);
	}

	token = StringSplitEx(NULL, "|");
	if (token != NULL)  
	{  
		strcpy(qrs->user_name, token);
	}

	token = StringSplitEx(NULL, "|");
	if (token != NULL)  
	{   
		strcpy(qrs->user_address, token);
	}

	token = StringSplitEx(NULL, "|"); 
	if (token != NULL)  
	{  
		strcpy(qrs->gas_count, token);
	}

	token = StringSplitEx(NULL, "|"); 
	if (token != NULL)  
	{
		qrs->ladder_gas_count = atol(token);
	}

	token = StringSplitEx(NULL, "|");
	if (token != NULL)  
	{
		qrs->unit_price = atof(token);
	}

	token = StringSplitEx(NULL, "|");
	if (token != NULL)  
	{
		strcpy(qrs->last_date, token);
	}
	

	token = StringSplitEx(NULL, "|");
	if (token != NULL)  
	{  
		qrs->buy_times = atoi(token);
	}

	token = StringSplitEx(NULL, "|");
	if (token != NULL)  
	{   
		strcpy(qrs->user_type, token);
	}

	token = StringSplitEx(NULL, "|"); 
	if (token != NULL)  
	{  
		strcpy(qrs->meter_code, token);
	}

	token = StringSplitEx(NULL, "|"); 
	if (token != NULL)  
	{  
		qrs->advance_payment = atof(token);
	}

	token = StringSplitEx(NULL, "|"); 
	if (token != NULL)
	{
		qrs->unsave_gas_count = atoi(token);
	}

	token = StringSplitEx(NULL, "|");
	if (token != NULL)  
	{  
		for (i = 0, j = 0; i < strlen(token); j++, i = i + 2)
		{
			memcpy(&szHexVal, token+i, 2);
			qrs->old_password[j] = (unsigned char)strtol(szHexVal, NULL, 16);
		}
	}

	token = StringSplitEx(NULL, "|");
	if (token != NULL)  
	{  
		for (i = 0, j = 0; i < strlen(token); j++, i = i + 2)
		{
			memcpy(&szHexVal, token+i, 2);
			qrs->new_password[j] = (unsigned char)strtol(szHexVal, NULL, 16);
		}
	}

	token = StringSplitEx(NULL, "|");
	if (token != NULL)  
	{
		for (i = 0, j = 0; i < strlen(token); j++, i = i + 2)
		{
			memcpy(&szHexVal, token+i, 2);
			qrs->card_data[j] = (unsigned char)strtol(szHexVal, NULL, 16);
		}
	}
	
	return qrs->len;

}

int QueryAliPayRes_Parse(SaveCardInfo* sci, unsigned char *recvData)
{

	char szHexVal[3] = {0};
	int i = 0, j = 0;
	
	if (NULL == sci || NULL == recvData)
	{
		return -1;
	}

	char *token = NULL;  

	token = strtok((char*)recvData, "|"); 
	if (token != NULL)
	{
		sci->len = atoi(token);
	}

	token = strtok(NULL, "|"); 
	if (token != NULL)  
	{  	
		sci->response_code = atoi(token);
	} 

	token = strtok(NULL, "|");
	if (token != NULL)  
	{  
		strcpy(sci->response_desc, token);
	}

	token = strtok(NULL, "|"); 
	if (token != NULL)  
	{  
		sci->card_type = atoi(token);
	}
	
	token = strtok(NULL, "|");
	if (token != NULL)  
	{  
		strcpy(sci->user_code, token);
	}

	token = strtok(NULL, "|");
	if (token != NULL)  
	{  
		for (i = 0, j = 0; i < strlen(token); j++, i = i + 2)
		{
			memcpy(&szHexVal, token+i, 2);
			sci->old_password[j] = (unsigned char)strtol(szHexVal, NULL, 16);
		}
	}

	token = strtok(NULL, "|");
	if (token != NULL)  
	{  
		for (i = 0, j = 0; i < strlen(token); j++, i = i + 2)
		{
			memcpy(&szHexVal, token+i, 2);
			sci->new_password[j] = (unsigned char)strtol(szHexVal, NULL, 16);
		}
	}

	token = strtok(NULL, "|");
	if (token != NULL)  
	{
		for (i = 0, j = 0; i < strlen(token); j++, i = i + 2)
		{
			memcpy(&szHexVal, token+i, 2);
			sci->card_data[j] = (unsigned char)strtol(szHexVal, NULL, 16);
		}
	}

	return sci->len;
	
}

int SaveResponseInfo_Parse(SaveResponseInfo* sri, unsigned char *recvData)
{
	if (NULL == sri || NULL == recvData)
	{
		return -1;
	}

	char *token = NULL;  

	token = strtok((char*)recvData, "|"); 
	if (token != NULL)
	{
		sri->len = atoi(token);
	}

	token = strtok(NULL, "|"); 
	if (token != NULL)  
	{  	
		sri->response_code = atoi(token);
	} 

	token = strtok(NULL, "|");
	if (token != NULL)  
	{  
		strcpy(sri->response_desc, token);
	}

	token = strtok(NULL, "|"); 
	if (token != NULL)  
	{  
		sri->card_type = atoi(token);
	}
	token = strtok(NULL, "|");
	if (token != NULL)  
	{  
		strcpy(sri->user_code, token);
	}

	token = strtok(NULL, "|");
	if (token != NULL)  
	{  
		strcpy(sri->user_name, token);
	}

	token = strtok(NULL, "|");
	if (token != NULL)  
	{   
		strcpy(sri->user_address, token);
	}

	token = strtok(NULL, "|"); 
	if (token != NULL)  
	{  
		strcpy(sri->gas_count, token);
	}

	return sri->len;
	
}
int SaveQueryInfo_Package(unsigned char *sendData, const SaveQueryInfo *sqi)
{
	unsigned char *pos = sendData;
	unsigned int *pLen = NULL;
	unsigned int len = 0;
	unsigned char field_value[64] = {0};

	char hexData[1024] = {0};
	char szHexValue[10] = {0};
	int i = 0;

	if (NULL == sqi || NULL == sendData)
	{
		return -1;
	}

	pLen = (unsigned int*)pos;
	len = append_field(pos, &sqi->len, sizeof(sqi->len), true);

	sprintf((char*)field_value, "%d", sqi->transaction_code);
	len += append_field(pos + len, field_value, strlen(field_value), true);

	sprintf((char*)field_value, "%d", sqi->card_type);
	len += append_field(pos + len, field_value, strlen(field_value), true);

	len += append_field(pos + len, &sqi->user_code, strlen(sqi->user_code), true);

	memset(hexData, 0, sizeof(hexData));
	for(i = 0; i < sizeof(sqi->old_password)/2; ++i)
	{
		memset(szHexValue, 0, 10);
		sprintf(szHexValue, "%02x", (unsigned char)sqi->old_password[i]);
		strcat(hexData, szHexValue);
	}
	len += append_field(pos + len, hexData, sizeof(sqi->old_password), true);

	memset(hexData, 0, sizeof(hexData));
	for(i = 0; i < sizeof(sqi->new_password)/2; ++i)
	{
		memset(szHexValue, 0, 10);
		sprintf(szHexValue, "%02x", (unsigned char)sqi->new_password[i]);
		strcat(hexData, szHexValue);
	}
	len += append_field(pos + len, hexData, sizeof(sqi->new_password), true);

	memset(hexData, 0, sizeof(hexData));
	for(i = 0; i < kReadDataLen; ++i)
	{
		memset(szHexValue, 0, 10);
		sprintf(szHexValue, "%02x", (unsigned char)sqi->card_data[i]);
		strcat(hexData, szHexValue);
	}
	len += append_field(pos + len, hexData, sizeof(sqi->card_data), true);

	len += append_field(pos + len, sqi->date, strlen(sqi->date), true);

	len += append_field(pos + len, sqi->time, strlen(sqi->time), true);

	len += append_field(pos + len, sqi->agent_code, strlen(sqi->agent_code), true);
	
	len += append_field(pos + len, sqi->terminal_code, strlen(sqi->terminal_code), true);

	len += append_field(pos + len, sqi->machine_code, strlen(sqi->machine_code), true);

	//len += append_field(pos + len, sqi->pay_code, strlen(sqi->pay_code), true);

	//len += append_field(pos + len, sqi->alipay_code, strlen(sqi->alipay_code), true);

	len += append_field(pos + len, &sqi->save_success, sizeof(sqi->save_success), false);

	*pLen = len;

	sprintf((char *)field_value, "%04d", len);
	append_field(pos, field_value, sizeof(sqi->len), true);

	return len;
	
}

int QueryAliPayReq_Package(unsigned char *sendData, const AlipayInfo *api)
{
	unsigned char *pos = sendData;
	unsigned int *pLen = NULL;
	unsigned int len = 0;
	unsigned char field_value[64] = {0};

	char hexData[1024] = {0};
	char szHexValue[10] = {0};
	int i = 0;

	if (NULL == api || NULL == sendData)
	{
		return -1;
	}

	pLen = (unsigned int*)pos;
	len = append_field(pos, &api->len, sizeof(api->len), true);

	sprintf((char*)field_value, "%d", api->transaction_code);
	len += append_field(pos + len, field_value, strlen(field_value), true);

	sprintf((char*)field_value, "%d", api->card_type);
	len += append_field(pos + len, field_value, strlen(field_value), true);

	len += append_field(pos + len, &api->user_code, strlen(api->user_code), true);

	len += append_field(pos + len, &api->gas_count, strlen(api->gas_count), true);

	len += append_field(pos + len, &api->gas_money, strlen(api->gas_money), true);

	len += append_field(pos + len, &api->pay_success, sizeof(api->pay_success), true);

	for(i = 0; i < sizeof(api->card_data)/2; ++i)
	{
		memset(szHexValue, 0, 10);
		sprintf(szHexValue, "%02x", (unsigned char)api->card_data[i]);
		strcat(hexData, szHexValue);
	}
	len += append_field(pos + len, hexData, sizeof(api->card_data), true);

	len += append_field(pos + len, api->date, strlen(api->date), true);

	len += append_field(pos + len, api->time, strlen(api->time), true);

	len += append_field(pos + len, api->agent_code, strlen(api->agent_code), true);
	
	len += append_field(pos + len, api->terminal_code, strlen(api->terminal_code), true);

	len += append_field(pos + len, api->machine_code, strlen(api->machine_code), true);

	len += append_field(pos + len, api->pay_code, strlen(api->pay_code), true);

	len += append_field(pos + len, api->alipay_code, strlen(api->alipay_code), false);

	*pLen = len;

	sprintf((char *)field_value, "%04d", len);
	append_field(pos, field_value, sizeof(api->len), true);

	return len;
	
}

int QueryCardReq_Package(unsigned char *sendData, const QueryRequestInfo *qrt)
{
	unsigned char *pos = sendData;
	unsigned int *pLen = NULL;
	unsigned int len = 0;
	unsigned char field_value[64] = {0};

	char hexData[1024] = {0};
	char szHexValue[10] = {0};
	int i = 0;

	if (NULL == qrt || NULL == sendData)
	{
		return -1;
	}

	pLen = (unsigned int*)pos;
	len = append_field(pos, &qrt->len, sizeof(qrt->len), true);

	sprintf((char*)field_value, "%04d", qrt->transaction_code);
	len += append_field(pos + len, field_value, sizeof(qrt->transaction_code), true);

	len += append_field(pos + len, &qrt->card_type, sizeof(qrt->card_type), true);

	
	for(i = 0; i < kReadDataLen; ++i)
	{
		memset(szHexValue, 0, 10);
		sprintf(szHexValue, "%02x", (unsigned char)qrt->card_data[i]);
		strcat(hexData, szHexValue);
	}
	//memcpy(qrt->card_data, hexData, 512);
	len += append_field(pos + len, hexData, 512, true);

	len += append_field(pos + len, qrt->date, strlen(qrt->date), true);

	len += append_field(pos + len, qrt->time, strlen(qrt->time), true);

	len += append_field(pos + len, qrt->agent_code, strlen(qrt->agent_code), true);

	len += append_field(pos + len, qrt->terminal_code, strlen(qrt->terminal_code), true);

	len += append_field(pos + len, qrt->machine_code, strlen(qrt->machine_code), false);

	*pLen = len;

	sprintf((char *)field_value, "%04d", len);
	append_field(pos, field_value, sizeof(qrt->len), true);

	return len;

}

int PayInfoReq_Package(unsigned char *sendData, const PayInfo *pi)
{
	unsigned char *pos = sendData;
	unsigned int *pLen = NULL;
	unsigned int len = 0;
	unsigned char field_value[64] = {0};

	char hexData[1024] = {0};
	char szHexValue[10] = {0};
	int i = 0;

	if (NULL == pi || NULL == sendData)
	{
		return -1;
	}

	pLen = (unsigned int*)pos;
	len = append_field(pos, &pi->len, sizeof(pi->len), true);

	sprintf((char*)field_value, "%d", pi->transaction_code);
	len += append_field(pos + len, field_value, strlen(field_value), true);

	sprintf((char*)field_value, "%d", pi->card_type);
	len += append_field(pos + len, field_value, strlen(field_value), true);

	len += append_field(pos + len, &pi->user_code, strlen(pi->user_code), true);

	len += append_field(pos + len, &pi->gas_count, strlen(pi->gas_count), true);

	len += append_field(pos + len, &pi->gas_money, strlen(pi->gas_money), true);

	len += append_field(pos + len, pi->date, strlen(pi->date), true);

	len += append_field(pos + len, pi->time, strlen(pi->time), true);

	len += append_field(pos + len, pi->agent_code, strlen(pi->agent_code), true);
	
	len += append_field(pos + len, pi->terminal_code, strlen(pi->terminal_code), true);

	len += append_field(pos + len, pi->machine_code, strlen(pi->machine_code), true);

	len += append_field(pos + len, pi->pay_code, strlen(pi->pay_code), true);

	len += append_field(pos + len, pi->alipay_code, strlen(pi->alipay_code), false);

	*pLen = len;

	sprintf((char *)field_value, "%04d", len);
	append_field(pos, field_value, sizeof(pi->len), true);

	return len;
}


