/*
 * This file is part of the libsigrok project.
 *
 * Copyright (C) 2018 geehalel <geehalel@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <config.h>
#include <glib.h>
#include <libusb.h>
#include "protocol.h"

static const char *channel_names[] = {
	"CH1", "CH2",
};

static struct sr_dev_inst *pcsu1000_dev_new()
{
	struct sr_dev_inst *sdi;
	struct sr_channel *ch;
	struct sr_channel_group *cg;
	struct dev_context *devc;
	unsigned int i;

	sdi = g_malloc0(sizeof(struct sr_dev_inst));
	sdi->status = SR_ST_INITIALIZING;
	sdi->vendor = g_strdup("Velleman");
	sdi->model = g_strdup("PCSU1000");

	/*
	 * Add only the real channels -- EXT isn't a source of data, only
	 * a trigger source internal to the device.
	 */
	for (i = 0; i < ARRAY_SIZE(channel_names); i++) {
		ch = sr_channel_new(sdi, i, SR_CHANNEL_ANALOG, TRUE, channel_names[i]);
		cg = g_malloc0(sizeof(struct sr_channel_group));
		cg->name = g_strdup(channel_names[i]);
		cg->channels = g_slist_append(cg->channels, ch);
		sdi->channel_groups = g_slist_append(sdi->channel_groups, cg);
	}

	devc = g_malloc0(sizeof(struct dev_context));
	devc->dev_state = IDLE;
	devc->timebase = DEFAULT_TIMEBASE;
	devc->samplerate = DEFAULT_SAMPLERATE;
	devc->ch_enabled[0] = TRUE;
	devc->ch_enabled[1] = TRUE;
	devc->voltage[0] = DEFAULT_VOLTAGE;
	devc->voltage[1] = DEFAULT_VOLTAGE;
	devc->coupling[0] = DEFAULT_COUPLING;
	devc->coupling[1] = DEFAULT_COUPLING;
	devc->voffset_ch1 = DEFAULT_VERT_OFFSET;
	devc->voffset_ch2 = DEFAULT_VERT_OFFSET;
	devc->voffset_trigger = DEFAULT_VERT_TRIGGERPOS;
	devc->framesize = DEFAULT_FRAMESIZE;
	devc->triggerslope = SLOPE_POSITIVE;
	devc->triggersource = g_strdup(DEFAULT_TRIGGER_SOURCE);
	devc->capture_ratio = DEFAULT_CAPTURE_RATIO;
	sdi->priv = devc;

	return sdi;
}


static GSList *scan(struct sr_dev_driver *di, GSList *options)
{
	struct drv_context *drvc;
	GSList *devices;
	struct dev_context *devc;
	struct sr_dev_inst *sdi;
	struct sr_usb_dev_inst *usb;
	struct sr_config *src;
	GSList *l;
	GSList *conn_devices;
	struct libusb_device_descriptor des;
	libusb_device **devlist;
	int i;
	const char *conn;
	char connection_id[64];

	devices = NULL;
	drvc = di->context;
	drvc->instances = NULL;
	conn = NULL;
	for (l = options; l; l = l->next) {
		src = l->data;
		if (src->key == SR_CONF_CONN) {
			conn = g_variant_get_string(src->data, NULL);
			break;
		}
	}
	if (conn)
		conn_devices = sr_usb_find(drvc->sr_ctx->libusb_ctx, conn);
	else
		conn_devices = NULL;

	/* Find all Velleman PCSU1000 devices and upload firmware to all of them. */
	libusb_get_device_list(drvc->sr_ctx->libusb_ctx, &devlist);
	for (i = 0; devlist[i]; i++) {
		if (conn) {
			usb = NULL;
			for (l = conn_devices; l; l = l->next) {
				usb = l->data;
				if (usb->bus == libusb_get_bus_number(devlist[i])
					&& usb->address == libusb_get_device_address(devlist[i]))
					break;
			}
			if (!l)
				/* This device matched none of the ones that
				 * matched the conn specification. */
				continue;
		}

		libusb_get_device_descriptor(devlist[i], &des);

		usb_get_port_path(devlist[i], connection_id, sizeof(connection_id));

		if (des.idVendor == VELLEMAN_PCSU1000_VID
			&& des.idProduct == VELLEMAN_PCSU1000_PID) {
			/* Device matches the Vid/Pid. */
			sr_dbg("Found a %s %s.", "Velleman", "PCSU1000");
			sdi = pcsu1000_dev_new();
			sdi->connection_id = g_strdup(connection_id);
			devices = g_slist_append(devices, sdi);
			devc = sdi->priv;
			//if (ezusb_upload_firmware(drvc->sr_ctx, devlist[i],
			//				USB_CONFIGURATION, prof->firmware) == SR_OK)
				/* Remember when the firmware on this device was updated */
			//	devc->fw_updated = g_get_monotonic_time();
			//else
			//	sr_err("Firmware upload failed");
			/* Dummy USB address of 0xff will get overwritten later. */
			sdi->conn = sr_usb_dev_inst_new(
				libusb_get_bus_number(devlist[i]), 0xff, NULL);
		}
	}
	libusb_free_device_list(devlist, 1);

	return std_scan_complete(di, devices);
}

static int dev_open(struct sr_dev_inst *sdi)
{
	(void)sdi;

	/* TODO: get handle from sdi->conn and open it. */

	return SR_OK;
}

static int dev_close(struct sr_dev_inst *sdi)
{
	(void)sdi;

	/* TODO: get handle from sdi->conn and close it. */

	return SR_OK;
}

static int config_get(uint32_t key, GVariant **data,
	const struct sr_dev_inst *sdi, const struct sr_channel_group *cg)
{
	int ret;

	(void)sdi;
	(void)data;
	(void)cg;

	ret = SR_OK;
	switch (key) {
	/* TODO */
	default:
		return SR_ERR_NA;
	}

	return ret;
}

static int config_set(uint32_t key, GVariant *data,
	const struct sr_dev_inst *sdi, const struct sr_channel_group *cg)
{
	int ret;

	(void)sdi;
	(void)data;
	(void)cg;

	ret = SR_OK;
	switch (key) {
	/* TODO */
	default:
		ret = SR_ERR_NA;
	}

	return ret;
}

static int config_list(uint32_t key, GVariant **data,
	const struct sr_dev_inst *sdi, const struct sr_channel_group *cg)
{
	int ret;

	(void)sdi;
	(void)data;
	(void)cg;

	ret = SR_OK;
	switch (key) {
	/* TODO */
	default:
		return SR_ERR_NA;
	}

	return ret;
}

static int dev_acquisition_start(const struct sr_dev_inst *sdi)
{
	/* TODO: configure hardware, reset acquisition state, set up
	 * callbacks and send header packet. */

	(void)sdi;

	return SR_OK;
}

static int dev_acquisition_stop(struct sr_dev_inst *sdi)
{
	/* TODO: stop acquisition. */

	(void)sdi;

	return SR_OK;
}

SR_PRIV struct sr_dev_driver velleman_pcsu1000_driver_info = {
	.name = "velleman-pcsu1000",
	.longname = "Velleman PCSU1000",
	.api_version = 1,
	.init = std_init,
	.cleanup = std_cleanup,
	.scan = scan,
	.dev_list = std_dev_list,
	.dev_clear = std_dev_clear,
	.config_get = config_get,
	.config_set = config_set,
	.config_list = config_list,
	.dev_open = dev_open,
	.dev_close = dev_close,
	.dev_acquisition_start = dev_acquisition_start,
	.dev_acquisition_stop = dev_acquisition_stop,
	.context = NULL,
};

SR_REGISTER_DEV_DRIVER(velleman_pcsu1000_driver_info);
