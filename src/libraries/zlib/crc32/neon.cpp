/*
 * Copyright 2017 The Chromium Authors
 * Use of this source code is governed by a BSD-style license that can be
 * found in the Chromium source repository LICENSE file.
 */

#include "neon.hpp"
#include <arm_acle.h>
#include <arm_neon.h>

#include <stdint.h>

#include "crc32.hpp"
#include "zlib.hpp"

z_crc_t armv8_crc32_little(
    const unsigned char *buf,
    z_size_t len,
    uint32_t crc) {

    uint32_t c = (uint32_t)~crc;

    while (len && ((uintptr_t)buf & 7)) {
        c = __crc32b(c, *buf++);
        --len;
    }

    const uint64_t *buf8 = (const uint64_t *)buf;

    while (len >= 64) {
        c = __crc32d(c, *buf8++);
        c = __crc32d(c, *buf8++);
        c = __crc32d(c, *buf8++);
        c = __crc32d(c, *buf8++);

        c = __crc32d(c, *buf8++);
        c = __crc32d(c, *buf8++);
        c = __crc32d(c, *buf8++);
        c = __crc32d(c, *buf8++);
        len -= 64;
    }

    while (len >= 8) {
        c = __crc32d(c, *buf8++);
        len -= 8;
    }

    buf = (const unsigned char *)buf8;

    while (len--) {
        c = __crc32b(c, *buf++);
    }

    return ~c;
}

void crc32_neon(config_t *config,
                input_t *input,
                output_t *output) {
    crc32_config_t *crc32_config = (crc32_config_t *)config;
    crc32_input_t *crc32_input = (crc32_input_t *)input;
    crc32_output_t *crc32_output = (crc32_output_t *)output;
    crc32_output->return_value[0] = armv8_crc32_little(crc32_input->buf, crc32_config->len, crc32_config->crc);
}