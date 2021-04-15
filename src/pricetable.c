#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "pricetable.h"
//#include "utilstool.h"
#define TABLE_ITEM_COUNT 50


static TieredPricing l_priceTable[TABLE_ITEM_COUNT] = {0};
static int l_tableItemCount =  0;

void init_PriceTable()
{
	int i = 0;
	for (i = 0; i < TABLE_ITEM_COUNT; i++){
		memset(&l_priceTable[i], 0, sizeof(TieredPricing));
	}
	l_tableItemCount = 0;
}

double PriceTable_GetPrice(const char *userType, long ladderGas, long gasCount)
{
	int i = 0;
	unsigned long ladder_gas = ladderGas;
	unsigned long gas_count = gasCount;
	unsigned long total_count = ladder_gas + gas_count;
	double pay_price = 0.0;

	for(i = 0; i < l_tableItemCount; i++)
	{
		if (l_priceTable[i].sort == '2' && strcmp(userType, l_priceTable[i].user_type) == 0)
		{
			if (total_count < l_priceTable[i].one_limit) {
				pay_price = (double)(gas_count * l_priceTable[i].one_price);
				break;
			} else if (total_count < l_priceTable[i].two_limit) {
				if (l_priceTable[i].one_limit > ladder_gas)
				{
					pay_price = (double)(l_priceTable[i].one_limit - ladder_gas)*l_priceTable[i].one_price;
					pay_price += (double)(total_count - l_priceTable[i].one_limit)*l_priceTable[i].two_price;
				} else {
					pay_price = (double)(total_count - ladder_gas)*l_priceTable[i].two_price;
				}	
				break;
			} else if (total_count < l_priceTable[i].three_limit) {
				if (ladder_gas < l_priceTable[i].one_limit) {
					pay_price = (double)(l_priceTable[i].one_limit - ladder_gas)*l_priceTable[i].one_price;
					pay_price += (double)(l_priceTable[i].two_limit - l_priceTable[i].one_limit)*l_priceTable[i].two_price;
					pay_price += (double)(total_count - l_priceTable[i].two_limit)*l_priceTable[i].three_price;
				} else if (ladder_gas < l_priceTable[i].two_limit) {
					pay_price = (double)(l_priceTable[i].two_limit - ladder_gas)*l_priceTable[i].two_price;
					pay_price += (double)(total_count - l_priceTable[i].two_limit)*l_priceTable[i].three_price;
				} else {
					pay_price += (double)(total_count - ladder_gas)*l_priceTable[i].three_price;
				}
				break;
			}
		}
	}


	pay_price = (pay_price*1000+0.5)/1000.0;

	return pay_price;
}

void PriceTable_Load(const char* pszFilename)
{
	char seps[]   = " |";
	char *token = NULL;
	char szLine[48] = {0};
	FILE* fp = NULL;
	int index = 0;

	init_PriceTable();
	fp = fopen(pszFilename, "r");
	if (NULL == fp)
	{
		return;
	}

	fgets(szLine, sizeof(szLine), fp);
	while(!feof(fp))
	{	
		token = strtok(szLine, seps);
		if (token != NULL)
		{
			strcpy(l_priceTable[index].user_type, token);
		}

		token = strtok(NULL, seps);
		if (token != NULL)
		{
			l_priceTable[index].sort = token[0];
		}

		token = strtok(NULL, seps);
		if (token != NULL)
		{
			l_priceTable[index].one_price = atof(token);
		}

		token = strtok(NULL, seps);
		if (token != NULL)
		{
			l_priceTable[index].one_limit = atol(token);
		}

		token = strtok(NULL, seps);
		if (token != NULL)
		{
			l_priceTable[index].two_price = atof(token);
		}

		token = strtok(NULL, seps);
		if (token != NULL)
		{
			l_priceTable[index].two_limit = atol(token);
		}

		token = strtok(NULL, seps);
		if (token != NULL)
		{
			l_priceTable[index].three_price = atof(token);
		}

		token = strtok(NULL, seps);
		if (token != NULL)
		{
			l_priceTable[index].three_limit = atol(token);
		}
		index++;
		l_tableItemCount++;

		fgets(szLine, sizeof(szLine), fp);
	}

	fclose(fp);

}

