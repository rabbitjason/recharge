#ifndef qpboc_h_
#define qpboc_h_

#include "emvapi.h"

#define EMV_QPBOC_OK						    0
#define EMV_QPBOC_TRANSACTION_DECLIEN           1
#define EMV_QPBOC_TRANSACTION_TERMINATED        2
#define EMV_QPBOC_APP_BLOCKED                   3
#define EMV_QPBOC_CANCELED                 	    4
#define EMV_QPBOC_CONNECT_TRANSACTION_NEEDED 	5
#define EMV_QPBOC_NEED_PBOC_TRANSACTION         6
#define EMV_QPBOC_NEED_RETRY                    7

#define EMV_QPBOC_ICC_ERROR			            8
#define EMV_QPBOC_INVALID_DATA                  9
#define EMV_QPBOC_DATA_MISSING                  10
#define EMV_QPBOC_DATA_DUPLICATE                11
#define EMV_QPBOC_MEMORY_OVERFLOW			    12
#define EMV_QPBOC_MEMORY_NO_ENOUGH			    13
#define EMV_QPBOC_PROGRAMING_ERROR              14
#define EMV_QPBOC_COMMUNICATION_ERROR           15 
#define EMV_QPBOC_CONDITIONS_NOT_SATISFIED      16
#define EMV_QPBOC_CARD_EXPIRED                  17
#define EMV_QPBOC_CARD_IN_BLACK_LIST            18
#define EMV_QPBOC_CANNOT_CONNECT_DECLINE        19
#define EMV_QPBOC_NO_APP                        20

#define EMV_QPBOC_EX_NO_MORE_RECORD             21



struct qpboc_parameters
{
	uint8_t 	m_TransactionProperty[4];	
	uint8_t     m_StatusCheckSupported;
};

typedef enum
{
	EMV_MSD,
		EMV_QPBOC,
		EMV_PBOC
}EMV_APP_TYPE;

typedef EMV_APP_TYPE * LP_EMV_APP_TYPE;

#ifdef __cplusplus
extern "C" {
#endif

typedef int (*fn_qpboc_detect_broken)(uint32_t DetectHandle); //If broken detected, return none zero


void emv_qpboc_set_broken_detect_func(fn_qpboc_detect_broken  pFn_DetectBroken, uint32_t DetectHandle);

int emv_qpboc_get_broken_source(void);

int emv_qpboc_set_parameter(const struct qpboc_parameters * parameters);
int emv_qpboc_get_parameter(struct qpboc_parameters * parameters);
int emv_qpboc_pre_transaction(uint32_t uiTarnsCounter, uint8_t TransType, uint32_t Amount);
int emv_qpboc_pre_transaction_ex(uint32_t uiTarnsCounter, uint8_t TransType, uint32_t Amount, uint32_t AmountOther);

int emv_qpboc_read_data(int fd);

int emv_qpboc_complete(void);
int emv_qpboc_signature_needed(int * pSignature);
int emv_qpboc_get_app_type(LP_EMV_APP_TYPE pAppType);
int emv_qpboc_select_no_gpo(int fd);
int emv_qpboc_need_save_failed_log(void);



/*****************************************************************

�ǽӴ�ʽIC��С��֧����չӦ��

*****************************************************************/

#define CAPP_TRANS_NONE                		0x00
#define CAPP_TRANS_TIME_SECTION_PAYMENT		0x01
#define CAPP_TRANS_OFFLINE_PREAUTH     		0x02
#define CAPP_TRANS_OFFLINE_PREAUTH_FINISH 	0x03
//����
//-1, ��������0: OK  >0 ���׾ܾ�
typedef int (*fn_qpboc_read_capp_data)(int ifd, int read_capp_handle, uint32_t * puiTransAmount); //��capp�ص�����
//-1, ��������0: OK  >0 ���׾ܾ�
typedef int (*fn_qpboc_update_capp_cache)(int ifd, int update_capp_cache_handle); //дcapp�ص�����
//����0, OK, ����ֵ����
typedef int (*fn_qpboc_ext_calc_rmac)(int mac_handle, const unsigned char * pdata, int idata, unsigned char rmac[4]);
typedef int (*fn_qpboc_get_capp_data_amount)(int read_capp_handle, 										
										  unsigned char bAmount[6]); 
//����������¼��ʱʱ��
int qpboc_ext_set_mute_log_timeout(unsigned int uiTimeoutSec);
//��������������
int qpboc_ext_set_mute_log_max(unsigned int uiMaxLogCount);

int qpboc_ext_read_data(int 							fd, 
							   uint32_t 					uiTarnsCounter, 
							   uint8_t 						TransType,
							   uint8_t 						capp_trans_id, 
							   fn_qpboc_get_capp_data_amount      fn_get_capp_data_amount,
							   fn_qpboc_read_capp_data  	fn_read_capp,
							   int              			read_capp_handle,
							   fn_qpboc_update_capp_cache 	fn_update_capp_cache,
							   int              			update_capp_cache_handle);
//Ӧ�ó���ͨ���ýӿڶ���¼
int qpboc_ext_read_capp_record(int 					fd, 
										uint8_t			sfi, 
										int 					bNextRecord, 
										const uint8_t 	id[2], 
										fn_qpboc_ext_calc_rmac 	fn_calc_rmac,
										int                     mac_handle,
										uint8_t         * pRecord,
										uint32_t         * piuRecord
										);


int qpboc_ext_read_record(int 		 fd,
								  unsigned 	 sfi,
								  unsigned 	 recno,
								  uint8_t  * pRecord,
								  uint32_t   iuRecord);

//Ӧ�ó���ͨ���ýӿ�UPDATE CAPP CATCH				  
int qpboc_ext_update_capp_catch(int 					 fd, 
										  uint8_t 			     sfi, 
										  int 					 bNextRecord, 																			
										  const uint8_t   *      pRecord,
										  uint32_t               uiRecord,
										  fn_qpboc_ext_calc_rmac fn_calc_rmac,
										  int                    mac_handle);
//��ͨӦ��, APPEND CAPP
int qpboc_ext_append_record(int fd,
			                        uint8_t			sfi,
			                        const uint8_t   *pRecord,
			                        uint32_t            uiRecord,
			                        fn_qpboc_ext_calc_rmac 	fn_calc_rmac,
			                        int                     mac_handle);

int qpboc_ext_get_trans_prove(int fd, const uint8_t ATC[2], uint8_t TC[8]);

int qpboc_ext_read_balance_9F79(int fd, uint8_t bBalance[6]);
int qpboc_ext_read_balance_9F5D(int fd, uint8_t bBalance[6]);
int qpboc_ext_read_deductible_limit(int fd, uint8_t bLimit[6]);
int qpboc_ext_read_deductible_amount(int fd, uint8_t bAmount[6]);


/*****************************************************************

�ǽӴ�ʽIC����ǿС��֧��

*****************************************************************/
#define TERMINAL_KEY_MODULES_MAX_SIZE   256
#define TERMINAL_KEY_EXPONENTS_MAX_SIZE 3
#pragma pack(1)
typedef struct{	
	unsigned char capk_index;
	unsigned char private_key_exponents[TERMINAL_KEY_MODULES_MAX_SIZE];
	unsigned int  uiprivate_key_exponents;
	unsigned char key_modules[TERMINAL_KEY_MODULES_MAX_SIZE];
	unsigned int  uikey_modules;
	unsigned char public_key_exponents[TERMINAL_KEY_EXPONENTS_MAX_SIZE];
	unsigned int  uipublic_key_exponents;
	unsigned char public_key_remainder[TERMINAL_KEY_MODULES_MAX_SIZE];
	unsigned int  uipublic_key_remainder;
	unsigned char public_key_certificate[TERMINAL_KEY_MODULES_MAX_SIZE];
	unsigned int  uipublic_key_certificate;
}enhanced_terminal_key;
#pragma pack()

int qpboc_enhanced_read_data(int 							fd, 
									   uint32_t 					uiTarnsCounter, 
									   uint8_t 						TransType,	
								   	   uint8_t 						capp_trans_id, 
								   	   fn_qpboc_get_capp_data_amount      fn_get_capp_data_amount,
								   	   fn_qpboc_read_capp_data  	fn_read_capp,
									   int              			read_capp_handle,
									   fn_qpboc_update_capp_cache 	fn_update_capp_cache,
									   int              			update_capp_cache_handle,
								       enhanced_terminal_key * 		pTerminalKey);
#ifdef __cplusplus
}
#endif

#endif