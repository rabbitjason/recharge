#include "card.h"
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include "iccard.h"
#include "lcd.h"
#include "sle4442.h"
#include "display.h"


int Card_Read(unsigned char *card_data)
{
	int fd = -1;
	int ret = -1;
	int i = 0;
	int key = 0;
	uint8_t error_count = 0;
	//clear data
	memset(card_data, 0, kDataLen);
	
	/* open device */
	fd = iccard_open_cardslot("/dev/usercard");
	if (fd < 0) {
		Show(0, "iccard_open_cardslot failed: fd=%d, errno=%d, reason=%s", fd, errno, strerror(errno));
		goto END;
	}
	//check card in slot
	while (iccard_check_card_in_slot(fd) != 0)
	{
		i++;
		Show(-1, "%s\n%s:%d", kInsertCard, kErrorCount, i);

		key = kb_hit();
		key = kb_getkey();
		
		if ((key == kCancelKey) || i >= 5)
		{
			goto END;
		}
	}

	Show(-1, kQuerying);
	
	/* open sle4442 */
	ret = sle4442_open(fd);
	if (ret) {
		Show(0, "sle4442_open failed: ret=%d, errno=%d, reason=%s", ret, errno, strerror(errno));
		goto END;
	}
	/* verify pwd error count : max is 3 */
	ret = sle4442_read_error_count(fd, &error_count);
	if (ret || error_count >= 3)
	{
		Show(0, "sle4442_read_error_count failed: ret=%d, error_count=%d", ret, error_count);
		goto END;
	}
	/* read data */
	ret = sle4442_read(fd, kStartAddr, kReadDataLen, card_data);
	if (ret)
	{
		Show(0, "sle4442_read failed: ret=%d, errno=%d, reason=%s", ret, errno, strerror(errno));
		goto END;
	}

END:
	if (fd >=0)
	{
		sle4442_close(fd);
		iccard_close_cardslot(fd);
		fd = -1;
	}
	return ret;
}

int Card_Write(const unsigned char *data, const unsigned char *old_pwd, const unsigned char *new_pwd)
{
	int fd = -1;
	int ret = 0;
	int i = 0;
	int change_pwd = 0;
	uint8_t error_count = 0;

	/* open device */
	fd = iccard_open_cardslot("/dev/usercard");
	if (fd < 0) {
		Show(0, "iccard_open_cardslot failed: fd=%d, errno=%d, reason=%s", fd, errno, strerror(errno));
		goto END;
	}
	//check card in slot
	while (iccard_check_card_in_slot(fd) != 0)
	{
		Show(0, "%d", i);
		/*
		Show(0, kInsertCard);
		if (kb_hit())
		{
			key = kb_getkey();
			if (key == 0x1B)
			{
				goto END;
			}
		}
		*/
	}
	/* open sle4442 */
	ret = sle4442_open(fd);
	if (ret) {
		Show(0, "sle4442_open failed: ret=%d, errno=%d, reason=%s", ret, errno, strerror(errno));
		goto END;
	}
	/* verify old pwd */
	ret = sle4442_verify(fd, old_pwd);
	if (ret) {
		/* verify pwd error count : max is 3 */
		ret = sle4442_read_error_count(fd, &error_count);
		if (ret || error_count == 3)
		{
			Show(0, "sle4442_read_error_count failed: ret=%d, error_count=%d", ret, error_count);
			goto END;
		}
	}
	/* write data */
	ret = sle4442_write(fd, kStartAddr, kReadDataLen, data);
	if (ret) {
		Show(0, "sle4442_write failed: ret=%d, errno=%d, reason=%s", ret, errno, strerror(errno));
		goto END;
	}
	/* compare old pwd and new pwd*/
	for (i = 0; i < 3; i++)
	{
		if (old_pwd[i] != new_pwd[i])
		{
			change_pwd = 1;
			break;
		}
	}
	if (change_pwd == 1)
	{
		/* change pwd */
		ret = sle4442_change_key(fd, new_pwd);
		if (ret)
		{
			Show(0, "sle4442_change_key failed: ret=%d, errno=%d, reason=%s", ret, errno, strerror(errno));
		}
	}
	
END:
	if (fd >=0)
	{
		ret = sle4442_close(fd);
		ret = iccard_close_cardslot(fd);
		fd = -1;
	}
	return ret;
}

