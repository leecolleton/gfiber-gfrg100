/*
 * Driver for Option High Speed Mobile Devices.
 *
 *   (c) 2008 Dan Williams <dcbw@redhat.com>
 *
 * Inspiration taken from sierra_ms.c by Kevin Lloyd <klloyd@sierrawireless.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/usb.h>

#include "usb.h"
#include "transport.h"
#include "option_ms.h"
#include "debug.h"

#define ZCD_FORCE_MODEM			0x01
#define ZCD_ALLOW_MS 			0x02

static unsigned int option_zero_cd = ZCD_FORCE_MODEM;
module_param(option_zero_cd, uint, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(option_zero_cd, "ZeroCD mode (1=Force Modem (default),"
		 " 2=Allow CD-Rom");

#define RESPONSE_LEN 1024

typedef struct rezero_msg_t {
    	const unsigned char *msg;
	int len;
} rezero_msg_t;

#define MAKE_MSG(arr...) ((unsigned char[]){arr})
#define MAKE_REZERO_MSG(arr...) { MAKE_MSG(arr), sizeof(MAKE_MSG(arr)) }

/* Both messages taken from the source code of usb_modeswitch utility
 * (http://www.draisberghof.de/usb_modeswitch/):
 * Mode switch from 0x19d2:0x2000 to
 * 0x19d2:0x{0001,0002,0015,0016,0017,0031,0037,0052,0055,0063,0064,0108,0128}
 */
static rezero_msg_t zte_msgs[] = {
	MAKE_REZERO_MSG(
		0x55, 0x53, 0x42, 0x43, 0x12, 0x34, 0x56, 0x78,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x1e,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	),
	MAKE_REZERO_MSG(
		0x55, 0x53, 0x42, 0x43, 0x12, 0x34, 0x56, 0x79,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x1b,
		0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	),
	{}
};

/* Huawei K3765
 * Message is taken from the source code of usb_modeswitch utility
 * (http://www.draisberghof.de/usb_modeswitch/):
 * Mode switch from 0x12d1:0x1520 to 0x12d1:0x1465
 */
static rezero_msg_t huawei_k3765_msgs[] = {
	MAKE_REZERO_MSG(
		0x55, 0x53, 0x42, 0x43, 0x12, 0x34, 0x56, 0x78,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11,
		0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	),
	{}
};

/* Vodafone (Huawei) K3806
 * Message is taken from the source code of usb_modeswitch utility
 * (http://www.draisberghof.de/usb_modeswitch/):
 * Mode switch from 0x12d1:0x14ad to 0x12d1:0x{14ae,14b0}
 */
static rezero_msg_t vodafone_huawei_k3806_msgs[] = {
	MAKE_REZERO_MSG(
		0x55, 0x53, 0x42, 0x43, 0x12, 0x34, 0x56, 0x78,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11,
		0x06, 0x20, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	),
	{}
};

/* Vodafone (ZTE) K3806-Z
 * Mode switch from 0x19d2:0x1013 to 0x19d2:0x1004
 */
static rezero_msg_t vodafone_zte_k3806z_msgs[] = {
	MAKE_REZERO_MSG(
		0x55, 0x53, 0x42, 0x43, 0x09, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0A, 0xFF,
                0x01, 0x01, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	),
	{}
};

/* Rezero message for Option devices */
static rezero_msg_t option_msgs[] = {
	MAKE_REZERO_MSG(
		0x55, 0x53, 0x42, 0x43, 0x78, 0x56, 0x34, 0x12,
		0x01, 0x00, 0x00, 0x00, 0x80, 0x00, 0x06, 0x01,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	),
	{}
};

static int send_rezero_msgs(struct us_data *us, rezero_msg_t *msg_list)
{
	char *buffer;
	int result;

	US_DEBUGP("Option MS: %s", "DEVICE MODE SWITCH\n");

	buffer = kzalloc(RESPONSE_LEN, GFP_KERNEL);
	if (buffer == NULL)
		return USB_STOR_TRANSPORT_ERROR;

	for (; msg_list->len; msg_list++)
	{
		memcpy(buffer, msg_list->msg, msg_list->len);
		result = usb_stor_bulk_transfer_buf(us,
				us->send_bulk_pipe,
				buffer, msg_list->len, NULL);
		if (result != USB_STOR_XFER_GOOD) {
			result = USB_STOR_XFER_ERROR;
			goto out;
		}

		/* Some of the devices need to be asked for a response, but we
		 * don't care what that response is.
		 */
		usb_stor_bulk_transfer_buf(us,
				us->recv_bulk_pipe,
				buffer, RESPONSE_LEN, NULL);
	}
	result = USB_STOR_XFER_GOOD;

out:
	kfree(buffer);
	return result;
}

static int option_set_rezero_msgs_by_vendor(char *vendor_name,
	rezero_msg_t **msg_list)
{
	static const struct {
		char *prefix;
		int len;
		rezero_msg_t *msgs;
	} vendor_msgs_map[] = {
#define MAP_ITEM(PREFIX, MSGS) \
	{ .prefix = (PREFIX), .len = sizeof(PREFIX) - 1, .msgs = (MSGS) }
		MAP_ITEM("Option", option_msgs),
		MAP_ITEM("ZCOPTION", option_msgs),
		MAP_ITEM("ZTE", zte_msgs),
		{ NULL}
#undef MAP_ITEM
	}, *i;

	for (i = &vendor_msgs_map[0]; i->prefix; i++)
	{
		if (!strncmp(vendor_name, i->prefix, i->len))
		{
			*msg_list = i->msgs;
			return 0;
		}
	}

	return 1;
}

static int option_set_rezero_msgs_by_descriptor(struct usb_device_descriptor *d,
	rezero_msg_t **msg_list)
{
	static const struct {
		__u16 idVendor;
		__u16 idProduct;
		rezero_msg_t *msgs;
	} id_msgs_map[] = {
#define MAP_ITEM(V, P, MF) { .idVendor = (V), .idProduct = (P), .msgs = (MF) }
		MAP_ITEM(0x12d1, 0x1520, huawei_k3765_msgs),
		MAP_ITEM(0x12d1, 0x14ad, vodafone_huawei_k3806_msgs),
		MAP_ITEM(0x19d2, 0x1013, vodafone_zte_k3806z_msgs),
		MAP_ITEM(0x19d2, 0x1014, vodafone_zte_k3806z_msgs),
		{ 0 }
#undef MAP_ITEM
	}, *i;
	__u16 idVendor = le16_to_cpu(d->idVendor);
	__u16 idProduct = le16_to_cpu(d->idProduct);

	for (i = &id_msgs_map[0]; i->idVendor; i++)
	{
		if (i->idVendor == idVendor && i->idProduct == idProduct)
		{
			*msg_list = i->msgs;
			return 0;
		}
	}

	return 1;
}

static int option_inquiry(struct us_data *us, rezero_msg_t **msg_list)
{
	const unsigned char inquiry_msg[] = {
	  0x55, 0x53, 0x42, 0x43, 0x12, 0x34, 0x56, 0x78,
	  0x24, 0x00, 0x00, 0x00, 0x80, 0x00, 0x06, 0x12,
	  0x00, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0x00,
	  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	char *buffer;
	int result;

	US_DEBUGP("Option MS: %s", "device inquiry for vendor name\n");

	buffer = kzalloc(0x24, GFP_KERNEL);
	if (buffer == NULL)
		return USB_STOR_TRANSPORT_ERROR;

	memcpy(buffer, inquiry_msg, sizeof(inquiry_msg));
	result = usb_stor_bulk_transfer_buf(us,
			us->send_bulk_pipe,
			buffer, sizeof(inquiry_msg), NULL);
	if (result != USB_STOR_XFER_GOOD) {
		result = USB_STOR_XFER_ERROR;
		goto out;
	}

	result = usb_stor_bulk_transfer_buf(us,
			us->recv_bulk_pipe,
			buffer, 0x24, NULL);
	if (result != USB_STOR_XFER_GOOD) {
		result = USB_STOR_XFER_ERROR;
		goto out;
	}

	/* Read the CSW */
	usb_stor_bulk_transfer_buf(us,
			us->recv_bulk_pipe,
			buffer, 13, NULL);

	result = option_set_rezero_msgs_by_vendor(buffer + 8, msg_list);
	if (!result)
		goto out;

	/* Try find the devices by vendor and product ids pair */
	result = option_set_rezero_msgs_by_descriptor(&us->pusb_dev->descriptor,
		msg_list);

out:
	kfree(buffer);
	return result;
}


int option_ms_init(struct us_data *us)
{
	int result;
	rezero_msg_t *msg_list;

	US_DEBUGP("Option MS: option_ms_init called\n");

	/* Additional test for vendor information via INQUIRY,
	 * because some vendor/product IDs are ambiguous
	 */
	result = option_inquiry(us, &msg_list);
	if (result != 0) {
		US_DEBUGP("Option MS: vendor is not Option or not determinable,"
			  " no action taken\n");
		return 0;
	} else
		US_DEBUGP("Option MS: this is a genuine Option device,"
			  " proceeding\n");

	/* Force Modem mode */
	if (option_zero_cd == ZCD_FORCE_MODEM) {
		US_DEBUGP("Option MS: %s", "Forcing Modem Mode\n");
		result = send_rezero_msgs(us, msg_list);
		if (result != USB_STOR_XFER_GOOD)
			US_DEBUGP("Option MS: Failed to switch to modem mode.\n");
		return -EIO;
	} else if (option_zero_cd == ZCD_ALLOW_MS) {
		/* Allow Mass Storage mode (keep CD-Rom) */
		US_DEBUGP("Option MS: %s", "Allowing Mass Storage Mode if device"
		          " requests it\n");
	}

	return 0;
}

