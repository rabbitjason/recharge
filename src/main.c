#include <stdio.h>
#include <stdlib.h>
#include <led.h>
#include <posapi.h>
#include "lcd.h"
#include "const.h"
#include "display.h"
#include "gprs.h"
#include "pricetable.h"
#include "config.h"


void Display_InputPwd(int *n)
{
	int loop = 1, key = 0, i = 0;
	char s[6] = {'\0'};
	s[0] = '0';
	*n = -1;
	while (loop)
	{
		lcd_clean();
		lcd_printf(ALG_CENTER, "请输入退出密码");
		lcd_printf(ALG_CENTER, "************************");
		lcd_printf(ALG_CENTER, "%s", s);
		lcd_printf(ALG_CENTER, "************************");
		lcd_printf(ALG_CENTER, kInputClear);
		lcd_printf(ALG_CENTER, kInputEnterContinue);
		lcd_flip();
		key = kb_hit();
		key = kb_getkey();
		//Show(0, "key: %d", key);
		if (key == kCancelKey)
		{
			lcd_clean();
			lcd_printf(ALG_CENTER, kInputCancelReturn);
			lcd_printf(ALG_CENTER, kInputEnterExit);
			lcd_flip();
			key = 0;
			while (key != kCancelKey && key != kEnterKey)
			{
				key = kb_hit();
				key = kb_getkey();
			}
			if (key == kEnterKey)
			{
				loop = 0;
				break;
			}
		}
		else if (key == kEnterKey)
		{
			//Show(0, "金额: %s", s);
			*n = atoi(s);
			if (*n > 0)
			{
				loop = 0;
			}
		}
		else if (key == kClearKey)
		{
			memset(&s, 0, sizeof(s));
			s[0] = '0';
		}
		else if (key >= k0Key && key <= k9Key)
		{
			i = 0;
			if (s[0] == '0')
			{
				s[0] = '\0';
			}
			while ((s[i] != '\0') && i < 6)
			{
				i++;
			}
			if (i == 6)
			{
				i = 6 - 1;
			}
			s[i] = key;
			//Show(0, "i=%d, s=%s", i, s);
		}
	}
}



int main(int argc, char *argv[])
{
	//const char menu[][100] = {"1.查询余额", "2.扫码购气", "3.用户圈存"};
	//const char menu[][100] = {{kQuery}, {kPay}, {kSave}}; // no display
	char menu[3][100] = {'\0'};
	int ret = 0, select = 0;
	int password = 0;
	QueryRequestInfo qrt;
	QueryResponseInfo qrs;
	AlipayInfo api;
	SaveCardInfo sci;
	int repeat = 0;
	unsigned long now = 0;
	
	lcd_init(&argc, &argv);
	lcd_set_bk_color(COLOR_WHITE);
	lcd_set_font_color(COLOR_BLACK);
	led_set_brightness(LED_BACKLIGHT, LED_BRIGHTNESS_MAX);	
	

	Show(-1, kConnect);
	
	while(repeat < 3) 
	{
		GPRS_Close();
		ret = GPRS_Open();
		if (ret == 0)
		{
			break;
		}
		
		now = GetTime();
		while ((GetTime() - now) < 3000)
		{
			usleep(1000 * 10);
		}
		

		repeat = repeat + 1;
	}
	
	if (ret)
	{
		Show(0, kConnectFail);
		return 0;
	}
	
	strncpy(menu[0], kQuery, 100);
	strncpy(menu[1], kPay, 100);
	strncpy(menu[2], kSave, 100);
		
	memset(&qrt, 0, sizeof(QueryRequestInfo));
	memset(&qrs, 0, sizeof(QueryResponseInfo));
	memset(&api, 0, sizeof(AlipayInfo));
	memset(&sci, 0, sizeof(SaveCardInfo));
	
	strcpy(qrt.agent_code, "200001");
	strcpy(qrt.terminal_code, "0123456789");
	char* sn = sys_get_sn(qrt.machine_code, sizeof(qrt.machine_code));
	if (sn != NULL){
		//Show(0, "machine code:%s", sn);
	} else {
		Show(0, kGetMachineCodeFail);
		return 0;
	}

//	if (!Config_Init("config.txt")) 
//	{
//		Show(0, "初始化配置信息错误");
//		return 0;
//	}

	PriceTable_Load("price.txt");
	
	//double dbPrice = PriceTable_GetPrice("0026","120", "260" );
	//Show(0, "gas price:%0.2f", dbPrice);
	while (select >= 0)
	{
		select = lcd_menu(kTitle, menu, sizeof(menu)/100, select);
		switch (select)
		{
			case -1:
			{
				Display_InputPwd(&password);
				if (password == 852503) {
					Show(-1, "系统退出中...");
				} else {
					select = 0;
				}
			}
			break;
			case -2:
			{
				Show(-1, "操作异常，系统将自动退出");
			}
			break;
			case 0:
			{
				ret = Display_Query(&qrt, &qrs);
			}
			break;
			case 1:
			{
				ret = Display_Pay(&qrt, &qrs, &api, &sci);
			}
			break;
			case 2:
			{
				ret = Display_Save_Ex(&qrt);
			}
			break;
			default:
			{
				
			}
			break;
		}
	}
	GPRS_Close();
	
	lcd_uninit();
	
	return 0;
}
