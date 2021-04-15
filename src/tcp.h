#ifndef _TCP_H_
#define _TCP_H_

#include "const.h"

int tcp_connect(const char * pServerAddr, unsigned int port);

int tcp_read(int isocket, void * pvbuff, unsigned int * pisize);

int tcp_read2(int isocket, void * pvbuff, unsigned int * pisize, int seconds);

int tcp_write(int isocket, const void * pvdata, unsigned int isize);

void tcp_close(int isocket);

int TCP_Get(unsigned char *recvData, unsigned int *pRecvLen, const unsigned char *sendData, unsigned int sendLen);

int TCP_WaitGet(unsigned char *recvData, unsigned int *pRecvLen, const unsigned char *sendData, unsigned int sendLen);

#endif

