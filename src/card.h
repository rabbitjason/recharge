#ifndef _CARD_H_
#define _CARD_H_

#include "const.h"


int Card_Read(unsigned char *card_data);

int Card_Write(const unsigned char *data, const unsigned char *old_pwd, const unsigned char *new_pwd);

#endif

