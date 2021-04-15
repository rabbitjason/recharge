#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <posapi.h>

#include "config.h"

static ConfigParam l_config_param;

bool Config_Init(const char *config_filename) 
{
	FILE *fp = NULL;
	char szLine[48] = {0};
	char value[8] = {0};
	char *ss = NULL;
	int v = 0;

	char seps[] = "=";
	char *token = NULL;


	sys_get_sn(l_config_param.machine_code, sizeof(l_config_param.machine_code));

	fp = fopen(config_filename, "r");
	if (fp == NULL)
	{
		return false;
	}

	memset (&l_config_param, 0, sizeof(ConfigParam));

	while (!feof(fp))
	{
		memset(value, 0, sizeof(value));
		memset(szLine, 0, sizeof(szLine));
		fgets(szLine, sizeof(szLine), fp);  //读取一行

		if (szLine[strlen(szLine) - 1] == '\n') //过滤\n
		{
			szLine[strlen(szLine) - 1] = '\0';
		}

		token = strtok(szLine, seps);
		if (token != NULL && strstr(token, "agent_code") != NULL)
		{
			strncpy(l_config_param.agent_code, strtok(NULL, seps), sizeof(l_config_param.agent_code));
		} 
		else if (token != NULL && strstr(token, "terminal_code") != NULL)
		{
			strncpy(l_config_param.terminal_code, strtok(NULL, seps), sizeof(l_config_param.terminal_code));
		}
	}

	fclose(fp);
	fp = NULL;

	return true;
}

char* Config_GetAgentCode()
{
	return l_config_param.agent_code;
}

char* Config_GetTerminalCode()
{
	return l_config_param.terminal_code;
}

char* Config_GetMachineCode()
{
	return l_config_param.machine_code;
}


