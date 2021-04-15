#ifndef _CONST_H_
#define _CONST_H_

static const char kTitle[] = "��ֻ���ȼ��������ֵ";
static const char kQuery[] = "1.��ѯ���";
static const char kPay[] = "2.ɨ�빺��";
static const char kSave[] = "3.�û�Ȧ��";

static const char kUserInfo[] = "�û���Ϣ";


static const char kQuerying[] = "���ڶ��������Ժ�...";

static const char kGasing[] = "����������...";
static const char kPaying[] = "֧��������...";
static const char kPayFailed[] = "������������޷�Ȧ�棬�뼰ʱ��Ӫҵ����ʵ��������ϵ���ͷ��绰��";
static const char kPayNext[] = "�밴��ȷ�ϡ�������Ȧ�����!";


static const char kSaveing[] = "Ȧ�洦����...";
static const char kSaveSuccess[] = "��ϲ��: Ȧ��ɹ�!";
static const char kSaveFailed[] = "Ȧ��ʧ�ܣ��뼰ʱ��Ӫҵ����ʵ��������ϵ**���ͷ��绰��!";


static const char kInsertCard[] = "�����ſ�, �ٰ����������!";
static const char kErrorCount[] = "�������";

static const char kInvalidCard[] = "������Ч����������!";
static const char kWrongCard[] = "�˿���ƥ�䣬������!";
static const char kQueryFirst[] = "���Ȳ�ѯ�˿�!";
static const char kReadFail[] = "����ʧ��!";
static const char kWriteFail[] = "д��ʧ��!";

static const char kGPRSOpen[] = "���ڴ�GPRS�����Ժ�...";
static const char kGPRSOpenFail[] = "��GPRSʧ��!";

static const char kConnect[] = "�������������Ժ�...";
static const char kConnectFail[] = "����ʧ��!";


static const char kInvalidKey[] = "����������Ч������������!";
static const char kNetworkQueryCardFail[] = "��ѯ����Ϣʧ�ܣ�������!";
static const char kNetworkQueryMoneyFail[] = "��ѯ���ʧ�ܣ�������!";
static const char kNetworkQueryPayFail[] = "ɨ�빺��ʧ�ܣ�������!";
static const char kNetworkSaveFail[] = "����浵ʧ�ܣ�������!";

static const char kGetMachineCodeFail[] = "��ȡ������ʧ��";

static const char kExitHint[] = "�Ƿ��˳�?";

//static const char kServerAddr[] = "221.232.69.208";
//static const unsigned short kServerPort = 12000;

static const char kServerAddr[] = "120.25.121.156"; 
static const unsigned short kServerPort = 12020;

static const char kCountDown[] = "����ʱ";


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


static const char kYuan[] = "Ԫ";
static const char kStere[] = "������";


static const char kCardType[] = "������";
static const char kUserCode[] = "�û�����";
static const char kUserName[] = "�û�����";
static const char kUserAddress[] = "������ַ";
static const char kGasCount[] = "��������";
static const char kLadderGasCount[] = "�������������";
static const char kUnitPrice[] = "����������";
static const char kLastDate[] = "�������ʱ��";
static const char kBuyTimes[] = "��������";
static const char kUserType[] = "�û�����";
static const char kMeterCode[] = "���";
static const char kAdvancePayment[] = "Ԥ����";
static const char kUnsaveGasCount[] = "δȦ������";


static const char kCardTypeValue[][16] = {"�ȷ濨", "��", "�ȷ��"};
static const char kUserTypeValue[][16] = {"��ͨ�û�", "��ҵ�û�"};

static const char kInputGasCount[] = "�����빺����(��λ: ������)";
static const char kInputClear[] = "����밴���������";
static const char kInputEnterContinue[] = "�����밴��ȷ�ϡ���";
static const char kInputCancelExit[] = "�˳��밴��ȡ������";
static const char kInputEnterExit[] = "�˳��밴��ȷ�ϡ���";
static const char kInputCancelReturn[] = "�����밴��ȡ������";
static const char kInputAnyKeyExit[] = "�밴������˳�";




static const char kBuyGasCount[] = "��������";
static const char kBuyGasMoney[] = "Ӧ�����";




static const int kStartAddr = 0;
static const int kReadDataLen = 256; //512 will cause failed
static const int kDataLen = 512;

//��ѯ����
typedef struct {
	unsigned int len;				//���ĳ���
	int transaction_code;			//���״��� 1001
	char card_type;					//������
	unsigned char card_data[512];	//����Ϣ
	char date[9];					//�������� YYYYMMDD
	char time[7];					//����ʱ�� hhmmss
	char agent_code[7];				//���յ����
	char terminal_code[50];			//�ն��������
	char machine_code[10];			//������
} QueryRequestInfo;

//��ѯ��Ӧ
typedef struct {
	unsigned int len;				//���ĳ���
	int response_code;				//��Ӧ��
	char response_desc[51];			//��Ӧ����
	unsigned int card_type;			//������
	char user_code[11];				//�û�����
	char user_name[51];				//�û�����
	char user_address[101];			//������ַ
	char gas_count[11];				//��������  6λ����4λС��
	long ladder_gas_count;			//�������������
	float unit_price;				//ȼ������  6λ����4λС��
	char last_date[51];				//����������� YYYYMMDD
	unsigned int buy_times;			//��������
	char user_type[5];				//�û�����
	char meter_code[11];			//���
	float advance_payment;			//Ԥ����  6λ����4λС��
	long unsave_gas_count;			//�ѹ���δȦ������
	unsigned char old_password[6];	//������
	unsigned char new_password[6];	//������
	unsigned char card_data[512];	//����Ϣ
	
} QueryResponseInfo;


//��������/��Ӧ
typedef struct {
	unsigned int len;				//���ĳ���
	int transaction_code;			//���״��� 20021
	char card_type;					//������
	char user_code[10];				//�û�����
	char gas_count[10];				//��������  6λ����4λС��
	char gas_money[10];				//������  6λ����4λС��  ����ʱΪ�գ����Ʒ�����������Ӧ
	char date[9];					//�������� YYYYMMDD
	char time[7];					//����ʱ�� hhmmss
	char agent_code[7];				//���յ����
	char terminal_code[50];			//�ն��������
	char machine_code[10];			//������
	char pay_code[18];				//�ɷ���ˮ��  ����ʱΪ�գ����Ʒ�����������Ӧ
	char alipay_code[12];			//֧����������  ����ʱΪ�գ����֧���ɹ������Ʒ�����������Ӧ
} PayInfo;

typedef struct {
	unsigned int len;				//���ĳ���
	int response_code;				//��Ӧ��
	char pay_code[19];				//�ɷ���ˮ��  ����ʱΪ�գ����Ʒ�����������Ӧ
	char alipay_code[13];			//֧����������  ����ʱΪ�գ����֧���ɹ������Ʒ�����������Ӧ
	unsigned char pic_data[4096];	//��ά��ͼƬ
} PayInfoRes;


//������ʵ����/��Ӧ
typedef struct {
	unsigned int len;				//���ĳ���
	int transaction_code;			//���״��� 1002
	unsigned int card_type;			//������
	char user_code[10];				//�û�����
	char gas_count[10];				//��������  6λ����4λС��
	char gas_money[10];				//������  6λ����4λС��
	char pay_success;				//�Ƿ�֧���ɹ�  0ʧ�ܣ�1�ɹ�
	unsigned char card_data[512];	//����Ϣ
	char date[9];					//�������� YYYYMMDD
	char time[7];					//����ʱ�� hhmmss
	char agent_code[7];				//���յ����
	char terminal_code[50];			//�ն��������
	char machine_code[10];			//������
	char pay_code[50];				//�ɷ���ˮ��
	char alipay_code[50];			//֧����������  ����ʱΪ�գ����֧���ɹ������Ʒ�����������Ӧ
} AlipayInfo;

//д����Ϣ  �ڹ�����ʵ��Ӧ���ĺ��������Ӧ
typedef struct {
	unsigned int len;				//���ĳ���
	int response_code;				//��Ӧ��
	char response_desc[51];		//��Ӧ����
	unsigned int card_type;			//������
	char user_code[11];			//�û�����
	unsigned char old_password[6];	//������
	unsigned char new_password[6];	//������
	unsigned char card_data[512];	//����Ϣ
} SaveCardInfo;

//Ȧ���ѯ
typedef struct {
	unsigned int len;				//���ĳ���
	int transaction_code;			//���״��� 2003
	unsigned int card_type;			//������
	char user_code[10];				//�û�����
	unsigned char old_password[6];	//������
	unsigned char new_password[6];	//������
	unsigned char card_data[512];	//����Ϣ
	char date[9];					//�������� YYYYMMDD
	char time[7];					//����ʱ�� hhmmss
	char agent_code[7];				//���յ����
	char terminal_code[50];			//�ն��������
	char machine_code[10];			//������
	char save_success;				//�Ƿ�Ȧ��ɹ�  1 - ʧ�ܣ�0 - �ɹ�
	//char pay_code[50];				//�ɷ���ˮ��
	//char alipay_code[50];			//֧����������
} SaveQueryInfo;

//Ȧ����Ӧ
typedef struct {
	unsigned int len;			//���ĳ���
	int response_code;			//��Ӧ��
	char response_desc[51];		//��Ӧ����
	char card_type;				//������
	char user_code[11];			//�û�����
	char user_name[51];			//�û�����
	char user_address[101];		//������ַ
	char gas_count[11];			//��������  6λ����4λС��
} SaveResponseInfo;

#endif

