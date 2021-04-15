#include "gprs.h"
#include <directfb.h>
#include <posapi.h>
#include <wnet.h>
#include <ppp.h>
#include <sys/time.h>
#include <pthread.h>
#include "display.h"


#define PPP_ALG_PAP				0x01
#define PPP_ALG_CHAP				0x02
#define PPP_ALG_MSCHAPV1			0x04
#define PPP_ALG_MSCHAPV2			0x08

#define PPP_LINK_BUILDING			1032

volatile static int g_is_open = 0;

char gprs_chat_file[] = 
{
	"ABORT          'NO CARRIER'"		"\n"
	"ABORT          'NO DIALTONE'"		"\n"
	"ABORT          'ERROR'"			"\n"
	"ABORT          'NO ANSWER'"		"\n"
	"ABORT          'BUSY'"				"\n"
	"ABORT          'Invalid Login'"	"\n"
	"ABORT          'Login incorrect'"	"\n"
	"TIMEOUT        '60'"				"\n"
	"''             'ATZ'"				"\n"
	"'OK'           'AT+CGDCONT=1,\"IP\",\"CMNET\"'"	"\n"
	"'OK'           'ATDT*99***1#'"			"\n"
	"'CONNECT'      ''"					"\n"
};

int GPRS_IsOpen(void)
{
	return g_is_open;
}
int GPRS_Open(void)
{
	int retval = 0;
	
	struct timeval start,cur, diff, end;
	struct timezone tz;

	retval = wnet_power_on();
	if (retval){
		Show(0, "Power On Failed");
		goto END;
	}
	retval = wnet_init("/var/mux0");
	if (retval){
		Show(0, "Power On Failed");
		goto END;
	}
	gettimeofday(&start, &tz);
	diff.tv_sec = 60;
	diff.tv_usec = 0;
	timeradd(&start, &diff, &end);
	//Show(-1, "set attached...");
	//lcd_flip();

	while (1)
	{
		retval = wnet_set_attached(1);
		if (!retval)
		{
			break;
		}
		gettimeofday(&cur, &tz);
		if (timercmp(&cur, &end, <))
			usleep(10000);
		else {
			Show(0, "wnet_set_attached() failed %d", retval);
			//lcd_flip();
			//kb_getkey();
			break;
		}
	}
	if (retval)
	{
		goto END;
	}
	//Show(-1, "PPP logon...");
	//lcd_flip();
	retval = ppp_open("/var/mux1", gprs_chat_file, "wap", "wap", PPP_ALG_PAP, 30);
	if (0 != retval){
		Show(0, "PPP open failed %d", retval);
		//lcd_flip();
		//kb_getkey();
		goto END;
	}
	gettimeofday(&start, &tz);
	diff.tv_sec = 60;
	diff.tv_usec = 0;
	timeradd(&start, &diff, &end);
	//Show(-1, "PPP check...");
	//lcd_flip();
	while (1){
		retval = ppp_check("/var/mux1");
		if (0 == retval){
			//Show(-1, "PPP logon Ok");
			//lcd_flip();
			//kb_getkey();
			//break;
			g_is_open = 1;
			return retval;
		}
		else{
			gettimeofday(&cur, &tz);				
			if (timercmp(&cur, &end, < ))
				usleep(10000);
			else {
				Show(0, "PPP logon failed %d", retval);
				//lcd_flip();
				//kb_getkey();
				break;
			}
		}
	}
	
	ppp_close("/var/mux1");	

END:
	g_is_open = 0;
	wnet_power_down();
	return retval;
}

void GPRS_Close(void)
{
	//if (g_is_open != 0) //crash and restart
	{
		g_is_open = 0;
		ppp_close("/var/mux1");
		wnet_power_down();
	}
}

static void *GPRS_ThreadProc(void *p)
{
	int ret = 0;
    GPRS_Close();
	ret = GPRS_Open();
	if (ret == 0)
	{
		g_is_open = 1;
	}
	else
	{
		g_is_open = 0;
		Show(0, kConnectFail);
	}
    return NULL;
}

int GPRS_ThreadOpen()
{
	pthread_t pd;
	return pthread_create(&pd, NULL, GPRS_ThreadProc, NULL);
}

static IDirectFBEventBuffer 	*events	= NULL;

void GPRS_ThreadOpenWait()
{
	if (g_is_open == 0)
	{
		Show(-1, kConnect);
	}
	while (g_is_open == 0)
    {
#if 1
        usleep(100 * 1000);
#else
		//crash
		DFBWindowEvent  windowEvent;
		DFBResult ret;
		while(1){
			if (events->HasEvent(events) == DFB_OK){
				if (events->PeekEvent(events, DFB_EVENT(&windowEvent)) == DFB_OK) {
					if (/*windowEvent.type == DWET_KEYDOWN || */ windowEvent.type == DWET_KEYUP)
						return;

					ret = events->GetEvent(events, DFB_EVENT(&windowEvent));
					if(ret){
						DirectFBError("IDirectFBEventBuffer::GetEvent() failed", ret); 
					}
				}
			} else {
				break;
			}
		}
#endif
    }
}

