
#ifndef _BLUETOOTH_H_
#define _BLUETOOTH_H_

#define BLUETOOTH_API_SERVER        "/var/tmp/bluetooth_api_server_socket"

#undef  MAX_PAIRED_DEVICES          /* this definition is removed */
#define MAX_DEV_NAME_LEN            64
#define MAX_BADDR_LEN               32

enum BLUEZ_API_ID {
    GET_ADAPTER_INFO,
    LIST_PAIRED_DEVICE,

    MAX_BLUEZ_API_ID
};

struct remote_dev_info {
    char name[MAX_DEV_NAME_LEN];
    char bdaddr[MAX_BADDR_LEN];
};

struct local_dev_info {
    char name[MAX_DEV_NAME_LEN];
    char bdaddr[MAX_BADDR_LEN];
};

int bluez_list_paired_device_var(struct remote_dev_info *devinfo, int count);
int bluez_get_info(struct local_dev_info *devinfo);

int bluez_setup(void);

int bluez_power_down(void);

#endif

