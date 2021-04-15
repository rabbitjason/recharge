#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utilstool.h"

void Split_String(char** arrString, int arrSize, char* pszString, const char* pszToken)
{
	char* token = NULL;
	token = strtok(pszString, pszToken);
	int i = 0;
	while(token != NULL)
	{
		strcpy(arrString[i], token);
		++i;
		if (i >= arrSize) break;
		token = strtok(NULL, pszToken);
	}
}



