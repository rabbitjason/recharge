#ifndef _CONST_H_
#define _CONST_H_

static const char kTitle[] = "武钢华润燃气自助充值";
static const char kQuery[] = "1.查询余额";
static const char kPay[] = "2.扫码购气";
static const char kSave[] = "3.用户圈存";

static const char kUserInfo[] = "用户信息";


static const char kQuerying[] = "正在读卡，请稍候...";

static const char kGasing[] = "购气处理中...";
static const char kPaying[] = "支付处理中...";
static const char kPayFailed[] = "购气金额有误无法圈存，请及时到营业厅核实处理，或联系（客服电话）";
static const char kPayNext[] = "请按【确认】键进行圈存操作!";


static const char kSaveing[] = "圈存处理中...";
static const char kSaveSuccess[] = "恭喜您: 圈存成功!";
static const char kSaveFailed[] = "圈存失败，请及时到营业厅核实处理，或联系**（客服电话）!";


static const char kInsertCard[] = "请插入磁卡, 再按任意键重试!";
static const char kErrorCount[] = "错误次数";

static const char kInvalidCard[] = "这是无效卡，请重试!";
static const char kWrongCard[] = "此卡不匹配，请重试!";
static const char kQueryFirst[] = "请先查询此卡!";
static const char kReadFail[] = "读卡失败!";
static const char kWriteFail[] = "写卡失败!";

static const char kGPRSOpen[] = "正在打开GPRS，请稍候...";
static const char kGPRSOpenFail[] = "打开GPRS失败!";

static const char kConnect[] = "正在联网，请稍候...";
static const char kConnectFail[] = "联网失败!";


static const char kInvalidKey[] = "您按下了无效按键，请重试!";
static const char kNetworkQueryCardFail[] = "查询卡信息失败，请重试!";
static const char kNetworkQueryMoneyFail[] = "查询金额失败，请重试!";
static const char kNetworkQueryPayFail[] = "扫码购气失败，请重试!";
static const char kNetworkSaveFail[] = "网络存档失败，请重试!";

static const char kGetMachineCodeFail[] = "获取机器码失败";

static const char kExitHint[] = "是否退出?";

//static const char kServerAddr[] = "221.232.69.208";
//static const unsigned short kServerPort = 12000;

static const char kServerAddr[] = "120.25.121.156"; 
static const unsigned short kServerPort = 12020;

static const char kCountDown[] = "倒计时";


static const int kCancelKey = 0x1B;
static const int kEnterKey = 0x0D;
static const int kClearKey = 0x08;

#if 0
static const int k0Key = 0x1E;
static const int k1Key = 0x1F;
static const int k2Key = 0x20;
static const int k3Key = 0x21;
static const int k4Key = 0x22;
static const int k5Key = 0x23;
static const int k6Key = 0x24;
static const int k7Key = 0x25;
static const int k8Key = 0x26;
static const int k9Key = 0x27;
#endif

static const int k0Key = 0x30;
static const int k1Key = 0x31;
static const int k2Key = 0x32;
static const int k3Key = 0x33;
static const int k4Key = 0x34;
static const int k5Key = 0x35;
static const int k6Key = 0x36;
static const int k7Key = 0x37;
static const int k8Key = 0x38;
static const int k9Key = 0x39;


static const char kYuan[] = "元";
static const char kStere[] = "立方米";


static const char kCardType[] = "卡类型";
static const char kUserCode[] = "用户号码";
static const char kUserName[] = "用户名称";
static const char kUserAddress[] = "用气地址";
static const char kGasCount[] = "卡上气量";
static const char kLadderGasCount[] = "本年阶梯用气量";
static const char kUnitPrice[] = "最新气单价";
static const char kLastDate[] = "最近购气时间";
static const char kBuyTimes[] = "购气次数";
static const char kUserType[] = "用户类型";
static const char kMeterCode[] = "表号";
static const char kAdvancePayment[] = "预付款";
static const char kUnsaveGasCount[] = "未圈存气量";


static const char kCardTypeValue[][16] = {"先锋卡", "金卡", "先锋金卡"};
static const char kUserTypeValue[][16] = {"普通用户", "商业用户"};

static const char kInputGasCount[] = "请输入购气量(单位: 立方米)";
static const char kInputClear[] = "输错请按【清除】键";
static const char kInputEnterContinue[] = "继续请按【确认】键";
static const char kInputCancelExit[] = "退出请按【取消】键";
static const char kInputEnterExit[] = "退出请按【确认】键";
static const char kInputCancelReturn[] = "返回请按【取消】键";
static const char kInputAnyKeyExit[] = "请按任意键退出";




static const char kBuyGasCount[] = "购买气量";
static const char kBuyGasMoney[] = "应付金额";




static const int kStartAddr = 0;
static const int kReadDataLen = 256; //512 will cause failed
static const int kDataLen = 512;

//查询请求
typedef struct {
	unsigned int len;				//报文长度
	int transaction_code;			//交易代码 1001
	char card_type;					//卡类型
	unsigned char card_data[512];	//卡信息
	char date[9];					//操作日期 YYYYMMDD
	char time[7];					//操作时间 hhmmss
	char agent_code[7];				//代收点代码
	char terminal_code[50];			//终端网点代码
	char machine_code[10];			//机器码
} QueryRequestInfo;

//查询响应
typedef struct {
	unsigned int len;				//报文长度
	int response_code;				//响应码
	char response_desc[51];			//响应描述
	unsigned int card_type;			//卡类型
	char user_code[11];				//用户号码
	char user_name[51];				//用户姓名
	char user_address[101];			//用气地址
	char gas_count[11];				//卡上气量  6位整数4位小数
	long ladder_gas_count;			//本年阶梯用气量
	float unit_price;				//燃气单价  6位整数4位小数
	char last_date[51];				//最近购气日期 YYYYMMDD
	unsigned int buy_times;			//购气次数
	char user_type[5];				//用户类型
	char meter_code[11];			//表号
	float advance_payment;			//预付款  6位整数4位小数
	long unsave_gas_count;			//已购买未圈存气量
	unsigned char old_password[6];	//老密码
	unsigned char new_password[6];	//新密码
	unsigned char card_data[512];	//卡信息
	
} QueryResponseInfo;


//购气请求/响应
typedef struct {
	unsigned int len;				//报文长度
	int transaction_code;			//交易代码 20021
	char card_type;					//卡类型
	char user_code[10];				//用户号码
	char gas_count[10];				//购买气量  6位整数4位小数
	char gas_money[10];				//购买金额  6位整数4位小数  请求时为空，由云服务器填充后响应
	char date[9];					//操作日期 YYYYMMDD
	char time[7];					//操作时间 hhmmss
	char agent_code[7];				//代收点代码
	char terminal_code[50];			//终端网点代码
	char machine_code[10];			//机器码
	char pay_code[18];				//缴费流水号  请求时为空，由云服务器填充后响应
	char alipay_code[12];			//支付宝订单号  请求时为空，如果支付成功则由云服务器填充后响应
} PayInfo;

typedef struct {
	unsigned int len;				//报文长度
	int response_code;				//响应码
	char pay_code[19];				//缴费流水号  请求时为空，由云服务器填充后响应
	char alipay_code[13];			//支付宝订单号  请求时为空，如果支付成功则由云服务器填充后响应
	unsigned char pic_data[4096];	//二维码图片
} PayInfoRes;


//购气核实请求/响应
typedef struct {
	unsigned int len;				//报文长度
	int transaction_code;			//交易代码 1002
	unsigned int card_type;			//卡类型
	char user_code[10];				//用户号码
	char gas_count[10];				//购买气量  6位整数4位小数
	char gas_money[10];				//购买金额  6位整数4位小数
	char pay_success;				//是否支付成功  0失败，1成功
	unsigned char card_data[512];	//卡信息
	char date[9];					//操作日期 YYYYMMDD
	char time[7];					//操作时间 hhmmss
	char agent_code[7];				//代收点代码
	char terminal_code[50];			//终端网点代码
	char machine_code[10];			//机器码
	char pay_code[50];				//缴费流水号
	char alipay_code[50];			//支付宝订单号  请求时为空，如果支付成功则由云服务器填充后响应
} AlipayInfo;

//写卡信息  在购气核实响应报文后紧接着响应
typedef struct {
	unsigned int len;				//报文长度
	int response_code;				//响应码
	char response_desc[51];		//响应描述
	unsigned int card_type;			//卡类型
	char user_code[11];			//用户号码
	unsigned char old_password[6];	//老密码
	unsigned char new_password[6];	//新密码
	unsigned char card_data[512];	//卡信息
} SaveCardInfo;

//圈存查询
typedef struct {
	unsigned int len;				//报文长度
	int transaction_code;			//交易代码 2003
	unsigned int card_type;			//卡类型
	char user_code[10];				//用户号码
	unsigned char old_password[6];	//老密码
	unsigned char new_password[6];	//新密码
	unsigned char card_data[512];	//卡信息
	char date[9];					//操作日期 YYYYMMDD
	char time[7];					//操作时间 hhmmss
	char agent_code[7];				//代收点代码
	char terminal_code[50];			//终端网点代码
	char machine_code[10];			//机器码
	char save_success;				//是否圈存成功  1 - 失败，0 - 成功
	//char pay_code[50];				//缴费流水号
	//char alipay_code[50];			//支付宝订单号
} SaveQueryInfo;

//圈存响应
typedef struct {
	unsigned int len;			//报文长度
	int response_code;			//响应码
	char response_desc[51];		//响应描述
	char card_type;				//卡类型
	char user_code[11];			//用户号码
	char user_name[51];			//用户姓名
	char user_address[101];		//用气地址
	char gas_count[11];			//卡上气量  6位整数4位小数
} SaveResponseInfo;

#endif

