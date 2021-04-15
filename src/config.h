#ifndef _CONFIG_H_
#define _CONFIG_H_

typedef struct {
	char agent_code[7];				//代收点代码
	char terminal_code[50];			//终端网点代码
	char machine_code[20];			//机器码
} ConfigParam;


bool Config_Init(const char *config_filename);

char* Config_GetMachineCode();

char* Config_GetAgentCode();

char* Config_GetTerminalCode();


#endif