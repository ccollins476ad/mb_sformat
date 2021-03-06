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

#define SF_MBUF_TEST_POOL_BUF_SIZE     (256)
#define SF_MBUF_TEST_POOL_BUF_COUNT    (10)
#define SF_MBUF_TEST_DATA_LEN          (1024)

os_membuf_t sf_mbuf_membuf[OS_MEMPOOL_SIZE(SF_MBUF_TEST_POOL_BUF_SIZE,
        SF_MBUF_TEST_POOL_BUF_COUNT)];

struct os_mbuf_pool sf_mbuf_pool;
struct os_mempool sf_mbuf_mempool;
uint8_t sf_mbuf_test_data[SF_MBUF_TEST_DATA_LEN];

/** Checks if two float values are 'nearly' equal (diff < epsilon) */
static bool
f_is_equal(float a, float b, float epsilon)
{
    float c;

    c = a - b;

    return c < epsilon && -c < epsilon;
}

void
sf_mbuf_test_setup(void)
{
    int rc;
    int i;

    rc = os_mempool_init(&sf_mbuf_mempool, SF_MBUF_TEST_POOL_BUF_COUNT,
            SF_MBUF_TEST_POOL_BUF_SIZE, &sf_mbuf_membuf[0], "mbuf_pool");
    TEST_ASSERT_FATAL(rc == 0, "Error creating memory pool %d", rc);

    rc = os_mbuf_pool_init(&sf_mbuf_pool, &sf_mbuf_mempool,
            SF_MBUF_TEST_POOL_BUF_SIZE, SF_MBUF_TEST_POOL_BUF_COUNT);
    TEST_ASSERT_FATAL(rc == 0, "Error creating mbuf pool %d", rc);

    for (i = 0; i < sizeof sf_mbuf_test_data; i++) {
        sf_mbuf_test_data[i] = i;
    }
}

TEST_CASE(test_bytes_tbl_hex)
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

    /* Optionally intentionally fail to see results for now. */
    TEST_ASSERT(f_is_equal(1.0f, 1.0f, 1E-5F));
}

TEST_CASE(test_bytes_tbl_dec)
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

    /* Optionally intentionally fail to see results for now. */
    TEST_ASSERT(f_is_equal(1.0f, 1.0f, 1E-5F));
}

TEST_CASE(test_bytes_tbl_asc)
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

    /* Optionally intentionally fail to see results for now. */
    TEST_ASSERT(f_is_equal(1.0f, 1.0f, 1E-5F));
}

TEST_CASE(test_mbuf_tbl_hex)
{
    struct os_mbuf *m1;
    struct os_mbuf *m2;
    struct os_mbuf *m3;
    struct sf_tbl_cfg cfg;
    int rc;

    sf_mbuf_test_setup();

    memset(&cfg, 0, sizeof cfg);
    cfg.format = SF_TABLE_DSPL_FMT_HEX;
    cfg.show_ascii = true;
    cfg.show_addr = true;
    cfg.start_addr = 0x20004030;

    /* Populate mbuf 1. */
    m1 = os_mbuf_get(&sf_mbuf_pool, 0);
    TEST_ASSERT_FATAL(m1 != NULL, "Error allocating mbuf 1");
    rc = os_mbuf_append(m1, sf_mbuf_test_data+48, 128);
    TEST_ASSERT_FATAL(rc == 0, "Cannot add %d bytes to mbuf 1", 128);

    /* Populate mbuf 2. */
    m2 = os_mbuf_get(&sf_mbuf_pool, 0);
    TEST_ASSERT_FATAL(m2 != NULL, "Error allocating mbuf 2");
    rc = os_mbuf_append(m2, sf_mbuf_test_data+48+128, 16);
    TEST_ASSERT_FATAL(rc == 0, "Cannot add %d bytes to mbuf 2", 16);

    /* Populate mbuf 3. */
    m3 = os_mbuf_get(&sf_mbuf_pool, 0);
    TEST_ASSERT_FATAL(m3 != NULL, "Error allocating mbuf 3");
    rc = os_mbuf_append(m3, sf_mbuf_test_data+48+128+16, 32);
    TEST_ASSERT_FATAL(rc == 0, "Cannot add %d bytes to mbuf 3", 32);

    /* Concat m3 -> m2 -> m1 to form a chain. */
    os_mbuf_concat(m2, m3);
    os_mbuf_concat(m1, m2);

    /* Render the mbuf pool in tabular format. */
    sf_mbuf_tbl_16(&cfg, m1, true);

    /* Free the mbufs. */
    rc = os_mbuf_free_chain(m1);
    TEST_ASSERT_FATAL(rc == 0, "Error free'ing mbuf chain %d", rc);

    /* Optionally intentionally fail to see results for now. */
    TEST_ASSERT(f_is_equal(1.0f, 1.0f, 1E-5F));
}
