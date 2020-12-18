/*
 * Copyright 2020 Kevin Weiss for Accelovant
 *
 * This file is subject to the terms and conditions of the MIT License. See the
 * file LICENSE in the top level directory for more details.
 * SPDX-License-Identifier:    MIT
 */

/* Includes *******************************************************************/
#include <stdint.h>
#include <errno.h>
#include <assert.h>

#include "dac161s997.h"
#include "dac161s997_port.h"
#include "internal/dac161s997_regs.h"

/* Private defines ************************************************************/
#define _SPI_READ_MASK      0x80

/* Private functions *************************************************************/
static int _dac161s997_op_reg(dac161s997_dev_t *dev, uint8_t addr,
                                  uint16_t *data);

static void _inter_packet_delay();

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/
int dac161s997_write_reg(dac161s997_dev_t *dev,
                             uint8_t addr, uint16_t data)
{
    int err;
    uint16_t expect_data = data;

    err = _dac161s997_op_reg(dev, addr, &data);
    if (err) {
        return err;
    }
    if (data != expect_data) {
        return -ENOEXEC;
    }

    return err;
}

int dac161s997_read_reg(dac161s997_dev_t *dev, uint8_t addr,
                            uint16_t *data)
{
    return _dac161s997_op_reg(dev, addr | _SPI_READ_MASK, data);
}

static int _dac161s997_op_reg(dac161s997_dev_t *dev, uint8_t addr,
                                  uint16_t *data)
{
    int err = 0;
    uint8_t in_buf[3] = { 0 };
    uint8_t out_buf[3];

    /* Assert addr is in range */
    assert((addr & (~_SPI_READ_MASK)) > 0);
    assert((addr & (~_SPI_READ_MASK)) <= 9);

    out_buf[0] = addr;
    out_buf[1] = (*data >> 8);
    out_buf[2] = (*data & 0xFF);
    err =  dac161s997_spi_xfer(dev, out_buf, in_buf, 3);

    _inter_packet_delay();

    if (err) {
        return err;
    }

    out_buf[0] = DAC161S997_NOP_REG;
    out_buf[1] = 0x00;
    out_buf[2] = 0x00;
    err =  dac161s997_spi_xfer(dev, out_buf, in_buf, 3);
    _inter_packet_delay();
    if (in_buf[0] != addr) {
        return -ENOEXEC;
    }
    *data = ((uint16_t)in_buf[1] << 8) | in_buf[2];
    return err;
}

static void _inter_packet_delay()
{
    /* Simple delay that may not be needed */
    for (uint32_t i = 0; i < 50; i++) {}
}
