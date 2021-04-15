#ifndef _CONFIG_H_
#define _CONFIG_H_

typedef struct {
	char agent_code[7];				//���յ����
	char terminal_code[50];			//�ն��������
	char machine_code[20];			//������
} ConfigParam;


bool Config_Init(const char *config_filename);

char* Config_GetMachineCode();

char* Config_GetAgentCode();

char* Config_GetTerminalCode();


#endif