#ifndef _WIFI_H_
#define _WIFI_H_

/* WIFI密码格式 */
typedef enum { 
    WIFI_KEY_TYPE_NOKEY,  /*不使用密码*/
    WIFI_KEY_TYPE_HEX,    /*使用十六进制的密码格式*/
    WIFI_KEY_TYPE_ASCII   /*使用ASCII的密码格式*/
} WIFI_KEY_TYPE;

/* WIFI安全加密模式 */
typedef enum { 
    WIFI_NET_SEC_NONE,       /*不使用安全加密模式*/
    WIFI_NET_SEC_WEP_OPEN,   /*使用开放密钥的WEP模式*/
    WIFI_NET_SEC_WEP_SHARED, /*使用共享密钥的WEP模式*/
    WIFI_NET_SEC_WPA,        /*使用WPA模式*/
    WIFI_NET_SEC_WPA2,        /*使用WPA2模式*/
    WIFI_NET_SEC_WPA_PSK,     /*使用WPA-PSK模式*/
    WIFI_NET_SEC_WPA2_PSK     /*使用WPA2-PSK模式*/
} WIFI_SEC_MODE;

typedef struct {
    char  SSID[124];   /*WIFI用户名*/
    int   SecMode;     /*安全加密模式，参见enum UWLNNETSEC*/
} ssid_info_t;

typedef struct {
    char  SSID[124];   /*WIFI用户名*/
    int   SecMode;     /*安全加密模式，参见enum UWLNNETSEC*/
    int   signal;
    char  pad[124];
} ssid_info_more_t;

/* WIFI通讯参数结构体 */
typedef struct {
    char  Pad[3];           /* 预留 */
    char  DHCP;             /*是否开启DHCP,0-否，1-是*/
    int   KeyType;          /*密码格式，参见enum UWLNKEYTYPE*/
    int   SecMode;          /*安全加密模式，参见enum UWLNNETSEC*/
    char  SSID[128];        /*WIFI用户名*/
    char  Pwd[64];         /*WIFI密码*/
    char  Pwd2[64];         /* WEP 密码2 */
    char  Pwd3[64];         /* WEP 密码3 */
    char  Pwd4[64];         /* WEP 密码4 */
    char  IP[16];           /*本地IP地址*/
    char  Gateway[16];	    /*本地网关*/
    char  Mask[16];         /*本地子网掩码*/
    char  Dns1[16];         /*DNS*/
    char  Dns2[16];         /*DNS*/
} ssid_connect_t;


typedef enum { 
	WR_DEVICE_UNAVAILABLE = -8,  /* 设备不可用 */
	WR_PARA_ERR      = -7,       /* 参数错*/
	WR_CONNECT_ERR   = -6,       /* 无法连接*/
    WR_CONNECTING    = -5,       /* 通讯连接中,PPP拨号中*/
	WR_LOGOFF        = -4,       /* 通讯链路断开*/
	WR_CANCEL        = -3,       /* 按键取消*/
	WR_TIMEOUT       = -2,       /* 超时*/
    WR_FAIL          = -1,       /* 失败*/
    WR_SUCCESS	    =  0,        /* 成功*/
    WR_LINK_OK       =  1,       /* 通讯链路OK */
} WififResultStatus;

int wifi_power_control(int to_open);
int wifi_power_control_force(int to_open);
int wifi_set_high_speed(int is_high_speed);
int wifi_scan_ssid(ssid_info_t ppszSSIDList[], int *pnNumList);
int wifi_scan_ssid_more(ssid_info_more_t ppszSSIDList[], int *pnNumList);
int wifi_connect_ssid(const ssid_connect_t *st_wificonfig);
int wifi_disconnect(void);
int wifi_get_status(void);
int wifi_get_signal(void);
int wifi_set_icon_on(int is_icon_on);



#endif /* _WIFI_H_ */

