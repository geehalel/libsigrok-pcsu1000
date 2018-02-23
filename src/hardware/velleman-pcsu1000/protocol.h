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

#ifndef LIBSIGROK_HARDWARE_VELLEMAN_PCSU1000_PROTOCOL_H
#define LIBSIGROK_HARDWARE_VELLEMAN_PCSU1000_PROTOCOL_H

#include <stdint.h>
#include <glib.h>
#include <ftdi.h>
#include <libsigrok/libsigrok.h>
#include "libsigrok-internal.h"

#define LOG_PREFIX "velleman-pcsu1000"

#define VELLEMAN_PCSU1000_VID 0x10cf
#define VELLEMAN_PCSU1000_PID 0x1000
#define VELLEMAN_PCSU1000_FIRMWARE "pcsu1000.bit"

#define NUM_CHANNELS            2

/* Must match the coupling table. */
enum couplings {
	COUPLING_AC = 0,
	COUPLING_DC,
	COUPLING_GND,
};

/* Must match the timebases table. */
enum time_bases {
	TIME_100ns = 0,
	TIME_200ns,
	TIME_500ns,
	TIME_1us,
	TIME_2us,
	TIME_5us,
	TIME_10us,
	TIME_20us,
	TIME_50us,
	TIME_1ms,
	TIME_2ms,
	TIME_5ms,
	TIME_10ms,
	TIME_20ms,
	TIME_50ms,
	TIME_100ms,
	TIME_200ms,
	TIME_500ms,
};

/* Must match the vdivs table. */
enum {
	VDIV_5MV,
	VDIV_10MV,
	VDIV_20MV,
	VDIV_50MV,
	VDIV_100MV,
	VDIV_200MV,
	VDIV_500MV,
	VDIV_1V,
	VDIV_2V,
};

enum trigger_slopes {
	SLOPE_POSITIVE = 0,
	SLOPE_NEGATIVE,
};

enum trigger_sources {
	TRIGGER_CH1 = 0,
	TRIGGER_CH2,
	TRIGGER_EXT,
};

enum capturestates {
	CAPTURE_EMPTY = 0,
	CAPTURE_FILLING = 1,
	CAPTURE_READY_8BIT = 2,
	CAPTURE_TIMEOUT = 127,
	CAPTURE_UNKNOWN = 255,
};

enum triggermodes {
	TRIGGERMODE_AUTO,
	TRIGGERMODE_NORMAL,
	TRIGGERMODE_SINGLE,
};

enum states {
	IDLE,
	NEW_CAPTURE,
	CAPTURE,
	FETCH_DATA,
	STOPPING,
};

#define DEFAULT_VOLTAGE         VDIV_1V
#define DEFAULT_FRAMESIZE       FRAMESIZE_SMALL
#define DEFAULT_TIMEBASE        TIME_1ms
#define DEFAULT_SAMPLERATE      SR_KHZ(1)
#define DEFAULT_TRIGGER_SOURCE  "CH1"
#define DEFAULT_COUPLING        COUPLING_DC
#define DEFAULT_CAPTURE_RATIO   100
#define DEFAULT_VERT_OFFSET     0.5
#define DEFAULT_VERT_TRIGGERPOS 0.5
#define FRAMESIZE_SMALL         (4 * 1024)


struct dev_context {
	/* FTDI stuff */
	struct ftdi_context ftdic;
	
	uint64_t limit_frames;
	uint64_t num_frames;
	GSList *enabled_channels;
	gboolean fw_uploaded;
	int epin_maxpacketsize;
	int capture_empty_count;
	int dev_state;

	/* Oscilloscope settings. */
	uint64_t samplerate;
	int timebase;
	gboolean ch_enabled[2];
	int voltage[2];
	int coupling[2];
	// voltage offset (vertical position)
	float voffset_ch1;
	float voffset_ch2;
	float voffset_trigger;
	uint16_t channel_levels[2][9][2];
	unsigned int framesize;
	gboolean filter[2];
	int triggerslope;
	char *triggersource;
	uint64_t capture_ratio;
	int triggermode;

	/* Frame transfer */
	unsigned int samp_received;
	unsigned int samp_buffered;
	unsigned int trigger_offset;
	unsigned char *framebuf;
};

SR_PRIV int velleman_pcsu1000_upload_firmware(struct sr_context *ctx,
			  struct dev_context *devc, const char *name);
SR_PRIV int velleman_pcsu1000_receive_data(int fd, int revents, void *cb_data);

#endif
