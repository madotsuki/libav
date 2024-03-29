/*
 * RTP input/output format
 * Copyright (c) 2002 Fabrice Bellard
 *
 * This file is part of Libav.
 *
 * Libav is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * Libav is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Libav; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <libavutil/opt.h>
#include "avformat.h"

#include "rtp.h"

//#define DEBUG

/* from http://www.iana.org/assignments/rtp-parameters last updated 05 January 2005 */
/* payload types >= 96 are dynamic;
 * payload types between 72 and 76 are reserved for RTCP conflict avoidance;
 * all the other payload types not present in the table are unassigned or
 * reserved
 */
static const struct
{
    int pt;
    const char enc_name[6];
    enum AVMediaType codec_type;
    enum CodecID codec_id;
    int clock_rate;
    int audio_channels;
} AVRtpPayloadTypes[]=
{
  {0, "PCMU",        AVMEDIA_TYPE_AUDIO,   CODEC_ID_PCM_MULAW, 8000, 1},
  {3, "GSM",         AVMEDIA_TYPE_AUDIO,   CODEC_ID_NONE, 8000, 1},
  {4, "G723",        AVMEDIA_TYPE_AUDIO,   CODEC_ID_NONE, 8000, 1},
  {5, "DVI4",        AVMEDIA_TYPE_AUDIO,   CODEC_ID_NONE, 8000, 1},
  {6, "DVI4",        AVMEDIA_TYPE_AUDIO,   CODEC_ID_NONE, 16000, 1},
  {7, "LPC",         AVMEDIA_TYPE_AUDIO,   CODEC_ID_NONE, 8000, 1},
  {8, "PCMA",        AVMEDIA_TYPE_AUDIO,   CODEC_ID_PCM_ALAW, 8000, 1},
  {9, "G722",        AVMEDIA_TYPE_AUDIO,   CODEC_ID_ADPCM_G722, 8000, 1},
  {10, "L16",        AVMEDIA_TYPE_AUDIO,   CODEC_ID_PCM_S16BE, 44100, 2},
  {11, "L16",        AVMEDIA_TYPE_AUDIO,   CODEC_ID_PCM_S16BE, 44100, 1},
  {12, "QCELP",      AVMEDIA_TYPE_AUDIO,   CODEC_ID_QCELP, 8000, 1},
  {13, "CN",         AVMEDIA_TYPE_AUDIO,   CODEC_ID_NONE, 8000, 1},
  {14, "MPA",        AVMEDIA_TYPE_AUDIO,   CODEC_ID_MP2, -1, -1},
  {14, "MPA",        AVMEDIA_TYPE_AUDIO,   CODEC_ID_MP3, -1, -1},
  {15, "G728",       AVMEDIA_TYPE_AUDIO,   CODEC_ID_NONE, 8000, 1},
  {16, "DVI4",       AVMEDIA_TYPE_AUDIO,   CODEC_ID_NONE, 11025, 1},
  {17, "DVI4",       AVMEDIA_TYPE_AUDIO,   CODEC_ID_NONE, 22050, 1},
  {18, "G729",       AVMEDIA_TYPE_AUDIO,   CODEC_ID_NONE, 8000, 1},
  {25, "CelB",       AVMEDIA_TYPE_VIDEO,   CODEC_ID_NONE, 90000, -1},
  {26, "JPEG",       AVMEDIA_TYPE_VIDEO,   CODEC_ID_MJPEG, 90000, -1},
  {28, "nv",         AVMEDIA_TYPE_VIDEO,   CODEC_ID_NONE, 90000, -1},
  {31, "H261",       AVMEDIA_TYPE_VIDEO,   CODEC_ID_H261, 90000, -1},
  {32, "MPV",        AVMEDIA_TYPE_VIDEO,   CODEC_ID_MPEG1VIDEO, 90000, -1},
  {32, "MPV",        AVMEDIA_TYPE_VIDEO,   CODEC_ID_MPEG2VIDEO, 90000, -1},
  {33, "MP2T",       AVMEDIA_TYPE_DATA,    CODEC_ID_MPEG2TS, 90000, -1},
  {34, "H263",       AVMEDIA_TYPE_VIDEO,   CODEC_ID_H263, 90000, -1},
  {-1, "",           AVMEDIA_TYPE_UNKNOWN, CODEC_ID_NONE, -1, -1}
};

int ff_rtp_get_codec_info(AVCodecContext *codec, int payload_type)
{
    int i = 0;

    for (i = 0; AVRtpPayloadTypes[i].pt >= 0; i++)
        if (AVRtpPayloadTypes[i].pt == payload_type) {
            if (AVRtpPayloadTypes[i].codec_id != CODEC_ID_NONE) {
                codec->codec_type = AVRtpPayloadTypes[i].codec_type;
                codec->codec_id = AVRtpPayloadTypes[i].codec_id;
                if (AVRtpPayloadTypes[i].audio_channels > 0)
                    codec->channels = AVRtpPayloadTypes[i].audio_channels;
                if (AVRtpPayloadTypes[i].clock_rate > 0)
                    codec->sample_rate = AVRtpPayloadTypes[i].clock_rate;
                return 0;
            }
        }
    return -1;
}

int ff_rtp_get_payload_type(AVFormatContext *fmt, AVCodecContext *codec)
{
    int i;
    AVOutputFormat *ofmt = fmt ? fmt->oformat : NULL;

    /* Was the payload type already specified for the RTP muxer? */
    if (ofmt && ofmt->priv_class) {
        int payload_type = av_get_int(fmt->priv_data, "payload_type", NULL);
        if (payload_type >= 0)
            return payload_type;
    }

    /* static payload type */
    for (i = 0; AVRtpPayloadTypes[i].pt >= 0; ++i)
        if (AVRtpPayloadTypes[i].codec_id == codec->codec_id) {
            if (codec->codec_id == CODEC_ID_H263)
                continue;
            if (codec->codec_id == CODEC_ID_PCM_S16BE)
                if (codec->channels != AVRtpPayloadTypes[i].audio_channels)
                    continue;
            return AVRtpPayloadTypes[i].pt;
        }

    /* dynamic payload type */
    return RTP_PT_PRIVATE + (codec->codec_type == AVMEDIA_TYPE_AUDIO);
}

const char *ff_rtp_enc_name(int payload_type)
{
    int i;

    for (i = 0; AVRtpPayloadTypes[i].pt >= 0; i++)
        if (AVRtpPayloadTypes[i].pt == payload_type) {
            return AVRtpPayloadTypes[i].enc_name;
        }

    return "";
}

enum CodecID ff_rtp_codec_id(const char *buf, enum AVMediaType codec_type)
{
    int i;

    for (i = 0; AVRtpPayloadTypes[i].pt >= 0; i++)
        if (!strcmp(buf, AVRtpPayloadTypes[i].enc_name) && (codec_type == AVRtpPayloadTypes[i].codec_type)){
            return AVRtpPayloadTypes[i].codec_id;
        }

    return CODEC_ID_NONE;
}
