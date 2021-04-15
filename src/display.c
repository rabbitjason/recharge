#include "display.h"
#include <stdarg.h>
#include <unistd.h>
#include <posapi.h>

#include "card.h"
#include "network.h"
#include "lcd.h"
#include "pricetable.h"


void HexDump(const void *data, unsigned int len)
{
	char str[160] = {'\0'}, octet[10] = {'\0'};
	int ofs = 0, i = 0, k = 0;
	const unsigned char *buf = (const unsigned char *)data;	

	for (ofs = 0; ofs < len; ofs += 16) 
	{
		sprintf(str, "| %08x: ", ofs);
		for (i = 0; i < 16; i++)
		{
			if ((i + ofs) < len)
				sprintf(octet, "%02x ", buf[ofs + i]);
			else
				strcpy(octet, "   ");	
			strcat(str, octet);
		}
		strcat(str, "  ");
		k = strlen(str);
		for (i = 0; i < 16; i++)
		{
			if ((i + ofs) < len)
				str[k++] = (0x20 <= (buf[ofs + i]) &&  (buf[ofs + i]) <= 0x7E) ? buf[ofs + i] : '.';
			else
				str[k++] = ' ';
		}
		str[k] = '\0';
		Show(0, "%s|", str);
	}
}

void Show(int key, const char * format, ...)
{
	int k = 0;
	char buf[2048] = {'\0'};
	va_list arg;
	va_start(arg, format);
	vsnprintf(buf, sizeof(buf), format, arg);
	va_end(arg);

	lcd_clean();
	lcd_printf(ALG_LEFT, buf);
	lcd_flip();

	if (key >= 0)
	{
		//lcd_count_down_start();
	}
	
	if (key > 0)
	{
		k = kb_hit();
		k = kb_getkey();
		while (k != key && k != kCancelKey)
		{
			lcd_clean();
			lcd_printf(ALG_LEFT, kInvalidKey);
			lcd_flip();
			k = kb_hit();
			k = kb_getkey();
		}
		//lcd_count_down_stop();
	}
	else if (key == 0)
	{
		k = kb_hit();
		k = kb_getkey();
		//lcd_count_down_stop();
	}
}

int Display_UserInfo(const QueryResponseInfo *qrs, int allow_enter_key)
{
	int key = 0, select = 0;
	char menu[11][100] = {'\0'};
	int idx = 0;

	snprintf(menu[0], 100, "%s: %s", kUserName, qrs->user_name);
	snprintf(menu[1], 100, "%s: %s", kUserAddress, qrs->user_address);
	snprintf(menu[2], 100, "%s: %s %s", kGasCount, qrs->gas_count, kStere);
	snprintf(menu[3], 100, "%s: %d %s", kLadderGasCount, qrs->ladder_gas_count, kStere);
	snprintf(menu[4], 100, "%s: %s", kLastDate, qrs->last_date);
	snprintf(menu[5], 100, "%s: %s", kUserType, qrs->user_type);
	snprintf(menu[6], 100, "%s: %d %s", kUnsaveGasCount, qrs->unsave_gas_count, kStere);
	snprintf(menu[7], 100, "%s", "************************");

	if (allow_enter_key == 2) {
		if (atoi(qrs->gas_count) != 0 ) {
			allow_enter_key = 0;
			snprintf(menu[8], 100, "%s", "请按【取消】键返回主菜单");
			snprintf(menu[9], 100, "%s", "您卡上已有气量,无法购气");
		} else if (qrs->unsave_gas_count != 0) {
			allow_enter_key = 0;
			snprintf(menu[8], 100, "%s", "请按【取消】键返回主菜单");
			snprintf(menu[9], 100, "%s", "您有未圈存气量,无法购气");
			snprintf(menu[10], 100, "%s", "请选择【3.用户圈存】");
		} else {
			allow_enter_key = 1;
			snprintf(menu[8], 100, "%s", kInputEnterContinue);
		}
	} else if (allow_enter_key == 3) {
		if (qrs->unsave_gas_count == 0) {
			allow_enter_key = 0;
			snprintf(menu[8], 100, "%s", "请按【取消】键返回主菜单");
			snprintf(menu[9], 100, "%s", "您没有未圈存气量");
			snprintf(menu[10], 100, "%s", "请选择【2.扫码购气】");
		} else {
			allow_enter_key = 1;
			snprintf(menu[8], 100, "%s", kInputEnterContinue);
		}
	}
	

	while (select >= 0)
	{
		select = lcd_menu2(kUserInfo, menu, sizeof(menu)/100, select, allow_enter_key, &key);
	}
	
	return key;
}


void Display_Input(int *n)
{
	int loop = 1, key = 0, i = 0;
	char s[6] = {'\0'};
	s[0] = '0';
	*n = -1;
	while (loop)
	{
		lcd_clean();
		lcd_printf(ALG_CENTER, kInputGasCount);
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


int Display_Query(QueryRequestInfo *qrt, QueryResponseInfo *qrs)
{
	int ret = 0, key = 0;
	//Show(-1, kQuerying);
	memset(qrs, 0, sizeof(QueryResponseInfo));
	ret = Card_Read(qrt->card_data);
	if (ret)
	{
		Show(0, kReadFail);
		return -1;
	}
#if 0
	HexDump(qrt->card_data, kReadDataLen);
#endif

	ret = Network_QueryCard(qrt, qrs, 2001);
	if (ret != 0)
	{
		Show(0, qrs->response_desc);
		return -1;
	}
	
	//显示用户信息
	key = Display_UserInfo(qrs, 3);

	//beep(1000, 1000*3);
	return 0;
}


int Display_Pay(QueryRequestInfo *qrt, QueryResponseInfo *qrs, AlipayInfo *api, SaveCardInfo *sci)
{
	int ret = 0, key = 0, i = 0, n = 0, debug = 0, timeout = 0;
	const char img_path[] = "ali_qr_code.png"; //服务器会发送付款的二维码图片
	unsigned char data[512] = {'\0'};
	double gas_price = 0.0;
	PayInfo pi;
	PayInfoRes pir;

	unsigned long starttime = 0;
	
	memset(&pi, 0, sizeof(PayInfo));
	memset(&pir, 0, sizeof(PayInfoRes));

	memset(qrs, 0, sizeof(QueryResponseInfo));

	ret = Card_Read(qrt->card_data);
	if (ret)
	{
		Show(0, kReadFail);
		return -1;
	}
	
	ret = Network_QueryCard(qrt, qrs, 2001);
	if (ret)
	{
		memset(qrs, 0, sizeof(QueryResponseInfo));
		Show(0, kNetworkQueryCardFail);
		return 2;
	}

	//显示用户信息
	key = Display_UserInfo(qrs, 2);
	if ((unsigned int)key == 0x1B)//cancel
	{
		return;
	}
		
InputAgain:

	//提示输入购气量
	Display_Input(&n);
	if (n == -1)
	{
		return 3;
	}
	//确定后从网络获取对应金额、阶梯价格、二维码，但是只显示前二者
	sprintf(pi.gas_count, "%d", n);
	gas_price = PriceTable_GetPrice(qrs->user_type, qrs->ladder_gas_count, n);
	//if (gas_price > qrs->advance_payment) {
	//	gas_price -= qrs->advance_payment;
	//} else {
	//	gas_price = 0.0;
	//}
	
	config(&debug, &timeout);
	if (debug == 1)
	{
		Show(0, "调试模式: 费用: %0.2f 将改为0.01元", gas_price);
		gas_price = 0.01;
	}
	sprintf(pi.gas_money, "%0.2f", gas_price);

	pi.card_type = qrs->card_type;
	memcpy(pi.user_code, qrs->user_code, sizeof(pi.user_code));
	strncpy(pi.agent_code, qrt->agent_code, sizeof(pi.agent_code));
	strncpy(pi.terminal_code, qrt->terminal_code, sizeof(pi.terminal_code));
	strncpy(pi.machine_code, qrt->machine_code, sizeof(pi.machine_code));
	
	//显示应付金额
	lcd_clean();
	lcd_printf(ALG_CENTER, "%s: %d %s", kBuyGasCount, n, kStere);
	lcd_printf(ALG_CENTER, "%s: %s %s", kBuyGasMoney, pi.gas_money, kYuan);
	lcd_printf(ALG_CENTER, "************************");
	lcd_printf(ALG_CENTER, kInputCancelExit);
	lcd_printf(ALG_CENTER, kInputEnterContinue);
	lcd_flip();
	//用户按下确定才显示二维码
	key = 0;
	while (key != kCancelKey && key != kEnterKey)
	{
		key = kb_hit();
		key = kb_getkey();
	}
	if (key == kCancelKey)
	{
		goto InputAgain;
	}
	else //if (key == kEnterKey)
	{
		//显示上一步获取的二维码图片
		Show(-1, "正在下载支付二维码图片...");
		ret = Network_QueryPay(&pi, img_path, &pir);
		if (ret != 0)
		{
			//Show(0, "下载图片失败: %d" , ret);
			lcd_clean();
			lcd_printf(ALG_CENTER, "请按任意键返回主菜单");
			lcd_printf(ALG_CENTER, "支付未完成,选择【2.扫码购气】");
			lcd_printf(ALG_CENTER, "支付完成,选择【3.用户圈存】");
			lcd_printf(ALG_CENTER, "如有问题，请联系客服");
			lcd_flip();
			kb_hit();
			key = kb_getkey();
			return ret;
		}
	}
	lcd_show_picture(img_path);

	//leiz: skip key press, wait for server answer
#if 0
	//手机支付后，按任意键下一步
	kb_hit();
	key = kb_getkey();
#endif
	memcpy(api->gas_count, pi.gas_count, sizeof(api->gas_count));
	memcpy(api->gas_money, pi.gas_money, sizeof(api->gas_money));
	api->card_type = qrs->card_type;
	memcpy(api->user_code, qrs->user_code, sizeof(api->user_code));
	api->pay_success = '0';
	memcpy(api->card_data, qrt->card_data, sizeof(api->card_data));
	strncpy(api->agent_code, qrt->agent_code, sizeof(api->agent_code));
	strncpy(api->terminal_code, qrt->terminal_code, sizeof(api->terminal_code));
	strncpy(api->machine_code, qrt->machine_code, sizeof(api->machine_code));
	memcpy(api->pay_code, pir.pay_code, strlen(pir.pay_code));
	memcpy(api->alipay_code, pir.alipay_code, strlen(pir.alipay_code));
	
	//...
	//服务器查询支付宝的缴费流水号以及是否支付成功
	//Show(-1, kPaying);
	ret = Network_QueryAliPay(api, sci);
	starttime = GetTime();
	if (ret != 0)
	{
		lcd_clean();
		lcd_printf(ALG_CENTER, "请按任意键返回主菜单");
		lcd_printf(ALG_CENTER, "支付未完成,选择【2.扫码购气】");
		lcd_printf(ALG_CENTER, "支付完成,选择【3.用户圈存】");
		lcd_printf(ALG_CENTER, "如有问题，请联系客服");
		lcd_flip();
		kb_hit();
		key = kb_getkey();
		return 4;
	}

	lcd_clean();
	lcd_printf(ALG_CENTER, "%s", sci->response_desc);
	lcd_printf(ALG_CENTER, "************************");
	lcd_printf(ALG_LEFT, "%s", kPayNext);
	lcd_flip();

	key = 0;
	while (key != kCancelKey && key != kEnterKey)
	{
		key = kb_hit();
		key = kb_getkey();
	}
	
	if (key == kCancelKey)
	{
		return 5;
	}
	else //if (key == kEnterKey)
	{
		ret = Display_Save(qrt->card_data, api, sci);
	}
	return ret;
}

int Display_Save(const unsigned char *card_data, const AlipayInfo *api, const SaveCardInfo *sci)
{
	int i = 0, ret = 0, key = 0;
	unsigned char data[512] = {'\0'};
	SaveQueryInfo sqi; //created by AlipayInfo and SaveCardInfo
	SaveResponseInfo sri;
	memset(&sqi, 0, sizeof(SaveQueryInfo));
	memset(&sri, 0, sizeof(SaveResponseInfo));
	//如果没有购气则退出--如果已经购气却重启了程序待考虑流程
	if (sci->len == 0)
	{
		Show(0, kNetworkQueryPayFail);
		return 1;
	}
	
	if (card_data[0] == '\0')
	{
		Show(0, kQueryFirst);
		return 1;
	}
	//read card
	ret = Card_Read(data);
	if (ret)
	{
		Show(0, kReadFail);
		return 2;
	}
	//compare data
	for (i = 0; i < kReadDataLen; i++)
	{
		if (data[i] != card_data[i])
		{
			ret = 1;
			break;
		}
	}
	//卡错误则退出
	if (ret)
	{
		Show(0, kWrongCard);
		return 3;
	}
	//是否需要显示卡的信息和服务器返回的最新卡的信息

	
	
	//圈存三次
	i = 0;
	do
	{
		//write card
		ret = Card_Write(sci->card_data, sci->old_password, sci->new_password);
		if (ret)
		{
			i++;
			Show(-1, "%s\n%s: %d", kWriteFail,kErrorCount, i);
		}
	} while (ret != 0 && i < 3);
	
	if (ret)
	{
		Show(0, "%s\n%s: %d", kWriteFail, kErrorCount, i);
	}
	
	sqi.save_success = '0';
	
	//服务器存档
	sqi.card_type = sci->card_type;
	strncpy(sqi.user_code, sci->user_code, sizeof(sqi.user_code));
	memcpy(sqi.old_password, sci->old_password, sizeof(sqi.old_password));
	memcpy(sqi.new_password, sci->new_password, sizeof(sqi.new_password));
	//read card
	memset(data, 0, sizeof(data));
	ret = Card_Read(data);
	if (ret)
	{
		Show(0, kReadFail);
	}

	Show(-1, kSaveing);
	
	memcpy(sqi.card_data, data, sizeof(sqi.card_data));
	strncpy(sqi.agent_code, api->agent_code, sizeof(sqi.agent_code));
	strncpy(sqi.terminal_code, api->terminal_code, sizeof(sqi.terminal_code));
	strncpy(sqi.machine_code, api->machine_code, sizeof(sqi.machine_code));
	//strncpy(sqi.pay_code, api->pay_code, sizeof(sqi.pay_code));
	//strncpy(sqi.alipay_code, api->alipay_code, sizeof(sqi.alipay_code));
	
	
	ret = Network_SaveResult(&sqi, &sri);
	if (ret != 0)
	{
		lcd_clean();
		lcd_printf(ALG_CENTER, "%s", kSaveFailed);
		lcd_printf(ALG_CENTER, "************************");
		lcd_printf(ALG_CENTER, "%s", kInputAnyKeyExit);
		lcd_flip();

		key = kb_hit();
	    key = kb_getkey();
		return 5;
	}

	lcd_clean();
	lcd_printf(ALG_CENTER, "%s", kSaveSuccess);
	lcd_printf(ALG_CENTER, "%s: %s", kUserCode, sri.user_code);
	lcd_printf(ALG_CENTER, "%s: %s", kUserName, sri.user_name);
	lcd_printf(ALG_CENTER, "%s: %s", kUserAddress, sri.user_address);
	lcd_printf(ALG_CENTER, "%s: %s %s", kGasCount, sri.gas_count, kStere);
	lcd_printf(ALG_CENTER, "************************");
	lcd_printf(ALG_CENTER, "%s", kInputAnyKeyExit);
	lcd_flip();

	key = kb_hit();
    key = kb_getkey();
	return 0;
}

int Display_Save_Ex(QueryRequestInfo *qrt)
{
	unsigned char data[512] = {'\0'};
	int i = 0, ret = 0, key = 0;
	QueryResponseInfo qrst;
	QueryResponseInfo *qrs;
	SaveQueryInfo sqi; 
	SaveResponseInfo sri;
	qrs = &qrst;
	memset(qrs, 0, sizeof(QueryResponseInfo));
	memset(&sri, 0, sizeof(SaveResponseInfo));
	ret = Card_Read(qrt->card_data);
	if (ret)
	{
		Show(0, kReadFail);
		return -1;
	}

	ret = Network_QueryCard(qrt, qrs, 2004);
	if (ret != 0)
	{
		Show(0, qrs->response_desc);
		return -1;
	}

//	if (qrs->unsave_gas_count == 0)
//	{

//		Show(0, "未购买圈存气量,按任意键返回主菜单");
//		return -1;
//	}

	key = Display_UserInfo(qrs, 3);
	if ((unsigned int)key == 0x1B)//cancel
	{
		return;
	}

	

//圈存三次
	i = 0;
	do
	{
		//write card
		ret = Card_Write(qrs->card_data, qrs->old_password, qrs->new_password);
		if (ret)
		{
			i++;
			Show(-1, "%s\n%s: %d", kWriteFail,kErrorCount, i);
		}
	} while (ret != 0 && i < 3);
	
	if (ret)
	{
    	sqi.save_success = '1';
		Show(0, "%s\n%s: %d", kWriteFail, kErrorCount, i);
	} else {
		sqi.save_success = '0';
	}

	
	//服务器存档
	sqi.card_type = qrs->card_type;
	strncpy(sqi.user_code, qrs->user_code, sizeof(sqi.user_code));
	memcpy(sqi.old_password, qrs->old_password, sizeof(sqi.old_password));
	memcpy(sqi.new_password, qrs->new_password, sizeof(sqi.new_password));
	//read card
	memset(data, 0, sizeof(data));
	ret = Card_Read(data);
	if (ret)
	{
		Show(0, kReadFail);
	}

	Show(-1, kSaveing);

	memcpy(sqi.card_data, data, sizeof(sqi.card_data));
	strncpy(sqi.agent_code, qrt->agent_code, sizeof(sqi.agent_code));
	strncpy(sqi.terminal_code, qrt->terminal_code, sizeof(sqi.terminal_code));
	strncpy(sqi.machine_code, qrt->machine_code, sizeof(sqi.machine_code));
	//strncpy(sqi.pay_code, qrs->pay_code, sizeof(sqi.pay_code));
	//strncpy(sqi.alipay_code, qrs->alipay_code, sizeof(sqi.alipay_code));
	
	ret = Network_SaveResult(&sqi, &sri);
	if (ret != 0)
	{
		lcd_clean();
		lcd_printf(ALG_CENTER, "%s", kSaveFailed);
		lcd_printf(ALG_CENTER, "************************");
		lcd_printf(ALG_CENTER, "%s", kInputAnyKeyExit);
		lcd_flip();

		key = kb_hit();
	    key = kb_getkey();
		return 5;
	}

	lcd_clean();
	lcd_printf(ALG_CENTER, "%s", kSaveSuccess);
	lcd_printf(ALG_CENTER, "%s: %s", kUserName, sri.user_name);
	lcd_printf(ALG_CENTER, "%s: %s", kUserAddress, sri.user_address);
	lcd_printf(ALG_CENTER, "%s: %s %s", kGasCount, sri.gas_count, kStere);
	lcd_printf(ALG_CENTER, "************************");
	lcd_printf(ALG_CENTER, "%s", kInputAnyKeyExit);
	lcd_flip();

	key = kb_hit();
    key = kb_getkey();
	return 0;
}

