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
#include "protocol.h"

/*
 * Transform the firmware stream into a series of bitbang
 * pulses used to program the FPGA. Note that the *bb_cmd must be free()'d
 * by the caller of this function.
 * FT245RM data pins are linked to the XC3S50 using the following scheme
 * D0: CCLCK
 * D1: DIN
 * D2: PROG_B
 * D3: INIT_B
 */
static int velleman_pcsu1000_firmware_2_bitbang(unisgned char *firmware,
					uint8_t **bb_cmd, gsize *bb_cmd_size)
{
	
}

SR_PRIV int velleman_pcsu1000_upload_firmware(struct sr_context *ctx,
			  struct dev_context *devc, const char *name)
{
	unsigned char *firmware;
	size_t length;
	int ret;

	/* Avoid downloading the same firmware multiple times. */
	if (devc->fw_uploaded) {
		sr_info("Not uploading firmware file '%s' again.", name);
		return SR_OK;
	}
q	/* Max file size considered 64 kiB, 54908 config bytes with overhead
	 * XAPP452 from Xilinx gives 439264 configuration bits for the XC3S50
	 */
	firmware = sr_resource_load(ctx, SR_RESOURCE_FIRMWARE,
			name, &length, 1 << 16);
	if (!firmware)
		return SR_ERR;

	sr_info("Uploading firmware '%s'.", name);

	g_free(firmware);

	return SR_ERR;
}
SR_PRIV int velleman_pcsu1000_receive_data(int fd, int revents, void *cb_data)
{
	const struct sr_dev_inst *sdi;
	struct dev_context *devc;

	(void)fd;

	if (!(sdi = cb_data))
		return TRUE;

	if (!(devc = sdi->priv))
		return TRUE;

	if (revents == G_IO_IN) {
		/* TODO */
	}

	return TRUE;
}
