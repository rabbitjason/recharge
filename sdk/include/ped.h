/******************************************************************************
 * pedapi.h
 * 
 * Copyright (c) 2012 by Li.Hua <lihua_338@163.com>.
 * 
 * DESCRIPTION: - 
 * 
 * Modification history
 * ----------------------------------------------------------------------------
 * Date         Version  Author       History
 * ----------------------------------------------------------------------------
 * 2012-08-28   1.0.0    Li.Hua       written
 ******************************************************************************/

#ifndef __PED_USER_API_H__
#define __PED_USER_API_H__

#include <stdint.h>

#ifdef __cplusplus 
extern "C" { 
#endif 

enum {
	PED_STATUS_LOCK			= 0,
	PED_STATUS_NORMAL,
	PED_STATUS_EXPIRED,
};

#define TYPE_PIN_INTERVAL		0x01		/* PIN Input interval */
#define TYPE_ACCOUNT_INTERVAL	0x03		/* Account data encrypt interval */

#define TDEA_DECRYPT            0x00000000	// TDES����
#define TDEA_ENCRYPT            0x00000001	// TDES����
#define TDEA_NONE               0x00000002	// �����ӽ��ܲ���(��ped��apiʹ��)

#define TDEA_MODE_ECB			0x00000000	// TDEA MODE ECB
#define TDEA_MODE_CBC			0x00000100	// TDEA MODE CBC
#define TDEA_MODE_CFB			0x00000200	// TDEA MODE CFB
#define TDEA_MODE_OFB			0x00000300	// TDEA MODE OFB

/* MasterKey ���ܡ�У�� PIN��MAC */
#define KEY_VERIFY_NONE			0x00000000	// ��
#define KEY_VERIFY_KVC			0x01000000	// KVC
#define KEY_VERIFY_CMAC			0x02000000	// CMAC
#define KEY_VERIFY_MASK			0xff000000	// ����

#define KEY_TYPE_SIEK			0x01	// The key to encrypt the internal sensitive infomation(internal use)
#define KEY_TYPE_MASTK			0x02	// MASTER KEY
#define KEY_TYPE_PINK			0x03	// PIN KEY
#define KEY_TYPE_MACK			0x04	// MAC KEY
#define KEY_TYPE_FIXPINK		0x05	// Fixed PIN KEY
#define KEY_TYPE_FIXMACK		0x06	// Fixed MAC KEY
#define KEY_TYPE_DUKPTK			0x07	// DUKPT KEY
#define KEY_TYPE_EMRKEY			0x08	// The key for securty magstripe reader
#define KEY_TYPE_KMMK			0x09	// The key for KMM
#define KEY_TYPE_EAK			0x0A	// Account Data KEY
#define KEY_TYPE_FIXEAK		    0x0B	// Fixed Account Data KEY
#if defined(CONFIG_CUSTOMER_EXADIGM)
#define KEY_TYPE_KEK			0x0C
#define KEY_TYPE_DEK			0x0D
#endif
#define KEY_TYPE_EXPIRED_KEY	0xFE	// The expired key
#define KEY_TYPE_INVALID		0xFF	// Invalid key

#define PIN_BLOCK_FORMAT_0		0x00	//  PIN BLOCK FORMAT 0
#define PIN_BLOCK_FORMAT_1		0x01	//  PIN BLOCK FORMAT 1
#define PIN_BLOCK_FORMAT_2		0x02	//  PIN BLOCK FORMAT 2
#define PIN_BLOCK_FORMAT_3		0x03	//  PIN BLOCK FORMAT 3
#define PIN_BLOCK_FORMAT_4		0x04	//  PIN BLOCK FORMAT 4
#define PIN_BLOCK_FORMAT_EPS	0x0A	//  PIN BLOCK FORMAT EPS

#define MAC_MODE_1				0x00	//  MAC method 1, TDES-TDES...TDES
#define MAC_MODE_2				0x01	//  MAC method 2, XOR...XOR...TDES
#define MAC_MODE_EMV			0x02	//  MAC for EMV EMV, DES-DES...TDES
#define MAC_MODE_CUP			0x03	//  MAC for CUP, XOR-XOR...TDES(left)-XOR-TDES...
#define MAC_MODE_DUKPT			0x04	//  MAC for DUKPT,Expand, XOR-XOR...TDES(left)-XOR-TDES...

#define PED_RET_OK              0x00	//  PED OK
#define PED_RET_BASE_NO			2000
#define PED_RET_LOCKED          (PED_RET_BASE_NO +  1)	//  PED Locked
#define PED_RET_ERROR           (PED_RET_BASE_NO +  2)	//  The others error
#define PED_RET_COMMERR         (PED_RET_BASE_NO +  3)	//  Communicate with PED failed
#define PED_RET_NEEDAUTH        (PED_RET_BASE_NO +  4)	//  Need auth before use sensitive service or expired
#define PED_RET_AUTHERR         (PED_RET_BASE_NO +  5)	//  PED auth failed
#define PED_RET_WEAK_KEY		(PED_RET_BASE_NO +  6)	//  weak length key
#define PED_RET_COLLISION_KEY	(PED_RET_BASE_NO +  7)	//  collision key
#define PED_RET_KEYINDEXERR     (PED_RET_BASE_NO +  8)	//  The index of key incorrect
#define PED_RET_NOKEY           (PED_RET_BASE_NO +  9)	//  No designated key in PED
#define PED_RET_KEYFULL         (PED_RET_BASE_NO + 10)	//  Key space is full
#define PED_RET_OTHERAPPKEY     (PED_RET_BASE_NO + 11)	//  The designated key is not belong to this APP
#define PED_RET_KEYLENERR       (PED_RET_BASE_NO + 12)	//  The key length error
#define PED_RET_NOPIN           (PED_RET_BASE_NO + 13)	//  Card holder press ENTER directly when enter PIN(no PIN)
#define PED_RET_CANCEL          (PED_RET_BASE_NO + 14)	//  Card holder press CANCEL to quit enter PIN
#define PED_RET_TIMEOUT         (PED_RET_BASE_NO + 15)	//  Timeout
#define PED_RET_NEEDWAIT        (PED_RET_BASE_NO + 16)	//  Two frequent between 2 sensitive API
#define PED_RET_KEYOVERFLOW     (PED_RET_BASE_NO + 17)	//  DUKPT KEY overflow
#define PED_RET_NOCARD          (PED_RET_BASE_NO + 18)	//  No ICC
#define PED_RET_ICCNOTPWRUP     (PED_RET_BASE_NO + 19)	//  ICC no power up
#define PED_RET_PARITYERR       (PED_RET_BASE_NO + 20)	//  The parity incorrect
#define PED_RET_UNSUPPORTED		(PED_RET_BASE_NO + 255)	//  can not support


typedef struct
{
    char		Model[16];		// �����ͺ�
    char		HwVer[16];		// Ӳ���汾��Ϣ
    char		SwVer[16];		// ����汾��Ϣ
    uint32_t	Status;			// PED״̬
    uint32_t	MaxBps;			// PED���֧�ֵ�ͨѶ������
    uint32_t	MaxAppNum;		// ����֧�ֵ�Ӧ����
    uint32_t	MasterKeyNum;	// ���MK�洢��
    uint32_t	PINKeyNum;		// ���PIN Key�洢��
    uint32_t	MACKeyNum;		// ���MAC Key�洢��
    uint32_t	FixPINKeyNum;	// ���Fix PIN Key�洢��
    uint32_t	FixMACKeyNum;	// ���Fix MAC Key�洢��
    uint32_t	DukptKeyNum;	// ���Dukpt Key�洢��
	uint32_t	Ctime;			// ��Կ����ʱ��
	uint32_t	MagKeyTime;		// �ſ���Կ����ʱ��
	uint32_t	MagKeyStatus;	// �ſ���Կ�Ƿ���Ч
	int16_t		MaxTemp;		// ��������
	int16_t		MinTemp;		// ��������
	int16_t		CurTemp;		// ��ǰCPU���¶�
    uint8_t		Res[154];		// Ԥ��
}PedConfig_t;   //  �������ⲿ��PED��Ϣ256 bytes

typedef struct {
     uint32_t	modlen;           // The module length of PIN encryption public key,
     uint8_t	mod[256];         // The module data of PIN encryption public key,
     uint32_t	explen;           // The exponet length of PIN encryption public key,
     uint8_t	exp[4];           // The exponet data of PIN encryption public key,
     uint8_t	iccrandom[8];     // The random data from ICC
} RsaPinKey_t;

typedef struct {
	uint8_t 	cla;
	uint8_t 	ins;
	uint8_t 	p1;
	uint8_t		p2;
	uint8_t 	leflg;
	uint8_t 	le;
	RsaPinKey_t rsakey;
} IccOfflinePinApdu_t;


/* ��ȡ����� */
uint32_t ped_get_random(void);
int ped_get_random_nr(uint32_t bytes, void *data);

/* ��ȡPED��������Ϣ */
int ped_get_config(PedConfig_t *config);

/* ��root��Ȩ�����ܵ��� */
int ped_get_status(uint32_t *status);

/* ���� PIN �����λ�úʹ�С */
int ped_set_pin_input_region(int x, int y, int width, int height);

/* ��ϵͳ������ʱ����ͨ������÷��ؾܾ� */
int ped_format(void);

/* ֻ�����ǰ�û���������Կ���ݣ���Ӱ�������û� */
int ped_clear_user_keys(void);

/* ɾ��ָ����һ����Կ */
int ped_delete_key(uint32_t KeyType, uint32_t KeyIndex);

/* ��ȡ��ʹ�����з���ļ��ʱ�� */
int ped_get_sensitive_timer(uint32_t SensitiveType);

/* ����PIN���볬ʱʱ�� 30~120 �� */
int ped_set_pin_input_timeout(uint32_t timeout);

/* ��ȡdukpt��KSN������һ�ο����������һ��PIN��MAC */
int ped_get_dukpt_ksn(uint32_t KeyIndex, void *Ksn);


/* ����Master Key���ܵ�PIN Key */
int ped_write_pin_key(uint32_t MasterKeyID, uint32_t PinKeyID,
					  uint32_t PinKeyLen,   const void *PinKey);
int ped_write_pin_key_ex(uint32_t Mode, uint32_t MasterKeyID, uint32_t DestKeyID,
						 uint32_t KeyLen, const void *KeyData);

/* ����Master Key���ܵ�MAC Key */
int ped_write_mac_key(uint32_t MasterKeyID, uint32_t MacKeyID,
					  uint32_t MacKeyLen,   const void *MacKey);
int ped_write_mac_key_ex(uint32_t Mode, uint32_t MasterKeyID, uint32_t DestKeyID, 
						 uint32_t KeyLen, const void *KeyData);

/* ����Master Key���ܵ�Account Data Key */
int ped_write_account_key_ex(uint32_t Mode, uint32_t MasterKeyID, uint32_t DestKeyID,
						 uint32_t KeyLen, const void *KeyData);

int ped_write_account_key(uint32_t MasterKeyID, uint32_t DestKeyID,
								uint32_t KeyLen,   const void *KeyData);

#if !defined(CONFIG_PCI_3_X)	
/* ��Կע�� */
int ped_write_master_key(uint32_t Mode, uint32_t MasterKeyID, uint32_t DestKeyID,
						 uint32_t KeyLen, const void *KeyData);

int ped_inject_pin_key(uint32_t PinKeyID, uint32_t PinKeyLen, const void *PinKey);

int ped_inject_mac_key(uint32_t MacKeyID, uint32_t MacKeyLen, const void *MacKey);

int ped_inject_account_key(uint32_t DataKeyID, uint32_t DataKeyLen, const void *DataKey);

int ped_create_dukpt_key(uint32_t KeyIndex, const void *InitKey, 
						 uint32_t KeyLen,   const void *Ksn);
#endif


/* �����û�PIN */
int ped_get_pin(uint32_t KeyIndex, uint32_t Mode, const char *ExpectPinLen, 
				const char *CardNo, void *PinBlock);

int ped_get_pin_fix(uint32_t KeyIndex, uint32_t Mode, const char *ExpectPinLen, 
					const char *CardNo, void *PinBlock);

int ped_get_pin_dukpt(uint32_t KeyIndex, uint32_t Mode, const char *ExpectPinLen,  
					  const void *CardNo, void *PinBlock);

int ped_icc_offline_plain_pin(int iccfd, const char *ExpectPinLen,
							  const IccOfflinePinApdu_t *icc,
							  uint32_t *rsplen, void *rspdata);

int ped_icc_offline_cipher_pin(int iccfd, const char *ExpectPinLen,
							   const IccOfflinePinApdu_t *icc,
							   uint32_t *rsplen, void *rspdata);

/* ��������MAC */
int ped_get_mac(uint32_t KeyIndex, uint32_t Mode, 
				uint32_t DataLen, const void *DataIn, void *MacOut);

int ped_get_mac_fix(uint32_t KeyIndex, uint32_t Mode,
					uint32_t DataLen, const void *DataIn, void *MacOut);


/*Account Data �ӽ���*/
int ped_account_encrypt(uint32_t Mode, uint32_t KeyIndex,
			  			uint32_t DataLen, const void *InBuf, void *OutBuf);

int ped_account_encrypt_fix(uint32_t Mode, uint32_t KeyIndex,
			  			uint32_t DataLen, const void *InBuf, void *OutBuf);

int ped_get_mac_dukpt(uint32_t KeyIndex, uint32_t Mode,
					  uint32_t DataLen, const void *DataIn, void *MacBlock);

/* ǩ����ר�ú�������ͨ�û����ûᱻ�ܾ� */
int ped_kmm_dencrypt(uint32_t Mode, uint32_t DataLen, const void *DataIn, void *DataOut);


#if !defined(CONFIG_PCI_3_X)	
int ped_tdea(uint32_t Mode, uint32_t KeyType, uint32_t KeyIndex,
			 uint8_t IV[8], uint32_t DataLen, const void *InBuf, void *OutBuf);
#endif

/* �ýӿ��ṩ����Կע�����ʹ�ã������߱�����rootȨ�ޣ���֤��Ҫ�ӿ� */
int ped_inject_key_by_root(uint32_t CUID, uint32_t KeyType, uint32_t KeyIndex,
						   uint32_t KeyLen, const void *PlaintextKey);

/* ��������Կ����8�ֽ����� */
int ped_encrypt_by_cipherkey(uint32_t Mode,	uint32_t MasterIndex,
					const void *CipherKeyData, const void *DataIn, void *DataOut);

/* ���ָ����Կ�Ƿ���� */
int ped_check_key(uint32_t KeyIndex, uint32_t KeyType, uint32_t Mode);

int ped_rsa_generate(int index, int keybits, int exponent, int *publen, void *pubkey);

int ped_rsa_inject_masterkey(uint32_t index,      uint32_t MkIndex,
							 uint32_t cipherlen,  const void *cipher,
							 const uint8_t n1[8], uint8_t n2[8]);

#if defined(CONFIG_CUSTOMER_EXADIGM)
int NP_MultiAddKEKbySlot(int key_index, unsigned char* kek_key, int  key_size);
int NP_MultiAddDEKbySlot(int key_index, unsigned char* dek_key, int key_size);
int NP_MultiReadKEKbySlot(int key_index, unsigned char* kek_key, int *key_size);
int NP_MultiReadDEKbySlot(int key_index, unsigned char* dek_key, int *key_size);
int NP_MultiDeleteKEKbySlot(int key_index);
int NP_MultiDeleteDEKbySlot(int key_index);
#endif

#ifdef __cplusplus 
}
#endif 

#endif /* __PED_USER_API_H__ */

