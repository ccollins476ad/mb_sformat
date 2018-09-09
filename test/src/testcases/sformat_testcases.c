/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include <string.h>
#include "sformat_test_priv.h"

/** Checks if two float values are 'nearly' equal (diff < epsilon) */
static bool
f_is_equal(float a, float b, float epsilon)
{
    float c;

    c = a - b;

    return c < epsilon && -c < epsilon;
}

TEST_CASE(test_tbl_hex)
{
    int i;
    struct sf_tbl_cfg cfg;
    uint8_t test_data[256];

    memset(&cfg, 0, sizeof cfg);
    cfg.format = SF_TABLE_DSPL_FMT_HEX;
    cfg.show_ascii = true;
    cfg.show_addr = true;
    cfg.start_addr = 0x20004000;

    /* Populate test_data. */
    for (i=0; i<256; i++) {
        test_data[i] = i;
    };

    /* Render the first 64 bytes of flash memory. */
    sf_bytes_tbl_16(&cfg, &test_data[48], 80);

    /* Intentionally fail to see results for now. */
    TEST_ASSERT(f_is_equal(1.0f, 1.1f, 1E-5F));
}

TEST_CASE(test_tbl_dec)
{
    int i;
    struct sf_tbl_cfg cfg;
    uint8_t test_data[256];

    memset(&cfg, 0, sizeof cfg);
    cfg.format = SF_TABLE_DSPL_FMT_DEC;
    cfg.show_ascii = true;
    cfg.show_addr = true;
    cfg.start_addr = 0x20004000;

    /* Populate test_data. */
    for (i=0; i<256; i++) {
        test_data[i] = i;
    };

    /* Render the first 64 bytes of flash memory. */
    sf_bytes_tbl_16(&cfg, &test_data[48], 80);

    /* Intentionally fail to see results for now. */
    TEST_ASSERT(f_is_equal(1.0f, 1.1f, 1E-5F));
}

TEST_CASE(test_tbl_asc)
{
    int i;
    struct sf_tbl_cfg cfg;
    uint8_t test_data[256];

    memset(&cfg, 0, sizeof cfg);
    cfg.format = SF_TABLE_DSPL_FMT_ASC;
    cfg.show_ascii = true;
    cfg.show_addr = true;
    cfg.start_addr = 0x20004000;

    /* Populate test_data. */
    for (i=0; i<256; i++) {
        test_data[i] = i;
    };

    /* Render the first 64 bytes of flash memory. */
    sf_bytes_tbl_16(&cfg, &test_data[48], 80);

    /* Intentionally fail to see results for now. */
    TEST_ASSERT(f_is_equal(1.0f, 1.1f, 1E-5F));
}
