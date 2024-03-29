/*
 * Copyright (c) 2010 Mans Rullgard
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

#ifndef AVCODEC_ARM_ASM_OFFSETS_H
#define AVCODEC_ARM_ASM_OFFSETS_H

#ifndef __ASSEMBLER__
#include <stddef.h>
#define CHK_OFFS(s, m, o) struct check_##o {    \
        int x_##o[offsetof(s, m) == o? 1: -1];  \
    }
#endif

/* MpegEncContext */
#define Y_DC_SCALE               0xac
#define C_DC_SCALE               0xb0
#define AC_PRED                  0xb4
#define BLOCK_LAST_INDEX         0xb8
#define H263_AIC                 0xe8
#define INTER_SCANTAB_RASTER_END 0x130

#endif /* AVCODEC_ARM_ASM_OFFSETS_H */
