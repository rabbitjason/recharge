#ifndef _PRICE_TABLE_H_
#define _PRICE_TABLE_H_

typedef struct {
	char user_type[5];
	char sort;
	double one_price;
	unsigned long one_limit;
	double two_price;
	unsigned long two_limit;
	double three_price;
	unsigned long three_limit;	
} TieredPricing;

double PriceTable_GetPrice(const char *userType, long ladderGas, long gasCount);

void PriceTable_Load(const char* pszFilename);


#endif

