#include "network.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <malloc.h>
#include <convert.h>

#include "gprs.h"
#include "tcp.h"
#include "display.h"
#include "gas_protocol.h"
#include "string_convert.h"
#include "lcd.h"
#include "config.h"



void write_TimeStamp(char *cur_date, char *cur_time)
{
	time_t timep;
	struct tm *p;
	time(&timep);
	p = localtime(&timep);
	char szValue[10] = {0};

	sprintf(szValue, "%4d%02d%02d", 1900 + p->tm_year, 1 + p->tm_mon, p->tm_mday);
	strncpy(cur_date, szValue, strlen(szValue));
	sprintf(szValue, "%02d%02d%02d", p->tm_hour, p->tm_min, p->tm_sec);
	strncpy(cur_time, szValue, strlen(szValue));

}

int Network_QueryCard(QueryRequestInfo *qrt, QueryResponseInfo *qrs, int tran_code)
{
	int ret = 0;
	unsigned char recv_data[1024] = {0};
	unsigned int recv_len = 1024;
	
	unsigned char send_data[1024] = {0};
	unsigned int send_len = 0;
	char *gb_text = NULL;
		
	memset(qrs, 0, sizeof(QueryResponseInfo));

	qrt->transaction_code = tran_code;
	qrt->card_type = '1';
	write_TimeStamp(qrt->date, qrt->time);
	
	//strcpy(qrt->agent_code, "200001");
	//strcpy(qrt->terminal_code, "0123456789");
	//strcpy(qrt->machine_code, "75038304");
	//strncpy(qrt->agent_code, Config_GetAgentCode(), sizeof(qrt->agent_code));
	//strncpy(qrt->terminal_code, Config_GetTerminalCode(), sizeof(qrt->terminal_code));
	//strncpy(qrt->machine_code, Config_GetMachineCode(), sizeof(qrt->machine_code));
	
	send_len = QueryCardReq_Package(send_data, qrt);
	ret = TCP_Get(recv_data, &recv_len, send_data, send_len);
	if (ret != 0)
	{
		return ret;
	}
	
	gb_text = string_convert_utf8togb(recv_data, recv_len);
	QueryCardRes_Parse(qrs, gb_text);
	
	ret = qrs->response_code;

	return ret;
}

int Network_QueryPay(PayInfo *pi, const char *img_path, PayInfoRes *pir)
{
	int ret = 0;
	unsigned char recv_data[4096] = {0};
	unsigned int recv_len = 4096;
		
	unsigned char send_data[2048] = {0};
	unsigned int send_len = 0;
	memset(pir, 0, sizeof(PayInfoRes));

	pi->transaction_code = 20021;
	write_TimeStamp(pi->date, pi->time);

	send_len = PayInfoReq_Package(send_data, pi);
	ret = TCP_Get(recv_data, &recv_len, send_data, send_len);
	//Show(0, "recv len: %d", recv_len);
	if (ret != 0)
	{
		return ret;
	}

	PayInfoRes_Parse(pir, recv_data);
	
	ret = pir->response_code;

	if (0 == ret) {
		FILE *fout = fopen(img_path, "wb");
		if (fout != NULL) {
			fwrite(pir->pic_data, 1, pir->len, fout);
			fflush(fout);
		}
		fclose(fout);
	} 
	
	return ret;
}

int Network_QueryAliPay(AlipayInfo *api, SaveCardInfo *sci)
{
	int ret = -1;
	int fd = -1;
	unsigned char recv_data[1024] = {'\0'};
	unsigned int recv_len = 0;
		
	unsigned char send_data[1024] = {'\0'};
	unsigned int send_len = 0;

	unsigned char szRecvBuf[1024] = {'\0'};
	int recvBufLen = 1024;// not null
	unsigned char *pos = recv_data;
	int time_out = 60 * 1; //wait 10 min
	unsigned long now = 0;
	char *gb_text = NULL;
	
	
	memset(sci, 0, sizeof(SaveCardInfo));
	
	api->transaction_code = 2002;
	write_TimeStamp(api->date, api->time);

//	memcpy(api->pay_code, api->date, sizeof(api->date));
//	memcpy(api->pay_code+strlen(api->pay_code), api->time, sizeof(api->time));
//	strcat(api->pay_code, "000001");
//	
//	
//	memcpy(api->alipay_code+strlen(api->alipay_code), api->date, sizeof(api->date));
//	memcpy(api->alipay_code+strlen(api->alipay_code), api->time, sizeof(api->time));

	send_len = QueryAliPayReq_Package(send_data, api);

	//leiz: skip key press, wait for server answer

	ret = TCP_WaitGet(recv_data, &recv_len, send_data, send_len);
	if (ret != 0)
	{
		//Show(0, "Network_QueryAliPay TCP_WaitGet failed: %d", ret);
		return ret;
	}
	gb_text = string_convert_utf8togb(recv_data, recv_len);
 
	QueryAliPayRes_Parse(sci, gb_text);

	ret = sci->response_code;
	return ret;


#if 0
	fd = tcp_connect(kServerAddr, kServerPort);
	if (fd == -1)
	{
		Show(0, "tcp_connect failed!");
		return -1;
	}
	ret = tcp_write(fd, send_data, send_len);
	if (ret != 0) {
		Show(0, "tcp_writet failed!");
		return ret;
	}
	now  = GetTime();
	do
	{
		memset(recv_data, 0, sizeof(recv_data));
		memset(szRecvBuf, 0, sizeof(szRecvBuf));
		recvBufLen = 1024;
		recv_len = 0;
		pos = NULL;
		pos = recv_data;
		
		ret = tcp_read2(fd, szRecvBuf, &recvBufLen, time_out);
		Show(-1, "Network_QueryAliPay tcp_read2 begin: recvBufLen=%d, recv_len=%d", recvBufLen, recv_len);
		while (ret == 0 && recvBufLen > 0)
		{
			recv_len += recvBufLen;
			memcpy(pos, szRecvBuf, recvBufLen);
			pos += recvBufLen;
			memset(szRecvBuf, 0, sizeof(szRecvBuf));
			recvBufLen = sizeof(szRecvBuf);
			ret = tcp_read2(fd, szRecvBuf, &recvBufLen, time_out);
		}
		if (recv_len > 0)
			{
				gb_text = (char*)malloc(recv_len * 4 + 1);
				memset(gb_text, 0, recv_len * 4 + 1);
				ret = charset_convert("UTF-8", "GB18030", recv_data, recv_len, gb_text, recv_len * 4 + 1); //leiz
				if (ret != 0) {
					free(gb_text);
					gb_text = NULL;
					Show(0, "Network_QueryAliPay utf8togb failed: %d", ret);
					return ret;
				}
				Show(0, "Network_QueryAliPay charset_convert end--string_len=%d", string_len(gb_text));
				d(recv_data, recv_len);
				free(gb_text);
				gb_text = NULL;
			}
		
		Show(-1, "Network_QueryAliPay tcp_read2 end: ret=%d, recv_len=%d, time=%d", ret, recv_len, (GetTime() - now)/1000);
		
		if (ret != 0)
		{
			Show(0, "tcp_read2 failed: %d", ret);
			tcp_close(fd);
			return ret;
		}
		if ((GetTime() - now) > 1000 * 60 * 10)
		{
			Show(0, "time out");
			tcp_close(fd);
			return -2;
		}
	}
	while (recv_len != 590);
#endif

#if 0
	gb_text = (char*)malloc(recv_len * 4 + 1);
	memset(gb_text, 0, recv_len * 4 + 1);
	Show(0, "Network_QueryAliPay charset_convert begin");
	ret = charset_convert("UTF-8", "GB18030", recv_data, recv_len, gb_text, recv_len * 4 + 1); //leiz
	if (ret != 0) {
		free(gb_text);
		gb_text = NULL;
		Show(0, "Network_QueryAliPay utf8togb failed: %d", ret);
		return ret;
	}
	Show(0, "Network_QueryAliPay charset_convert end");
	QueryAliPayRes_Parse(sci, gb_text);
	free(gb_text);
	gb_text = NULL;
	Show(0, "Network_QueryAliPay QueryAliPayRes_Parse end");
	tcp_close(fd);
	ret = sci->response_code;
	return ret;
#endif
}

int Network_SaveResult(SaveQueryInfo *sqi, SaveResponseInfo *sri)
{
	int ret = 0;
	unsigned char recv_data[1024] = {0};
	unsigned int recv_len = 1024;
		
	unsigned char send_data[1024] = {0};
	unsigned int send_len = 0;
	char *gb_text = NULL;
	//SaveResponseInfo sri;
	memset(sri, 0, sizeof(SaveResponseInfo));

	sqi->transaction_code = 2003;
	write_TimeStamp(sqi->date, sqi->time);
	
	send_len = SaveQueryInfo_Package(send_data, sqi);
	ret = TCP_Get(recv_data, &recv_len, send_data, send_len);
	if (ret != 0)
	{
		return ret;
	}
	gb_text = (char*)malloc(recv_len * 4 + 1);
	memset(gb_text, 0, recv_len * 4 + 1);
	ret = charset_convert("UTF-8", "GB18030", recv_data, recv_len, gb_text, recv_len * 4 + 1); //leiz
	if (ret != 0) {
		free(gb_text);
		gb_text = NULL;
		Show(0, "Network_SaveResult utf8togb failed: %d", ret);
		return ret;
	}
	SaveResponseInfo_Parse(sri, gb_text);
	free(gb_text);
	gb_text = NULL;

	ret = sri->response_code;
	return ret;
}

