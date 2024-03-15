/*
 * Copyright 2020 Kevin Weiss for Accelovant
 *
 * This file is subject to the terms and conditions of the MIT License. See the
 * file LICENSE in the top level directory for more details.
 * SPDX-License-Identifier:    MIT
 */

/**
 ******************************************************************************
 * @addtogroup DRIVER_INTERNAL
 * @{
 * @file            dac161s997_regs.h
 * @author          Kevin Weiss
 * @date            21.05.2020
 * @brief           Register control for the dac161s997
 ******************************************************************************
 */

#ifndef DAC161S997_REGS_H_
#define DAC161S997_REGS_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes *******************************************************************/
#include <stdint.h>
#include <errno.h>
#include "dac161s997_types.h"

/* Defines ********************************************************************/
/**
 * @defgroup DAC161S997_REGS
 * @{
 */
#define DAC161S997_XFR_REG                  0x01    /**< Command: transfer data into register (protected write mode) */
#define DAC161S997_NOP_REG                  0x02    /**< Command: does nothing except resetting the timeout timer */
#define DAC161S997_PROTECT_REG_WR_REG       0x03    /**< 1 means protected write mode in effect */
#define DAC161S997_DACCODE_REG              0x04    /**< 16 bit DAC code to be written */
#define DAC161S997_ERR_CONFIG_REG           0x05    /**< Configuration of error states */
#define DAC161S997_ERR_LOW_REG              0x06    /**< DAC code for the <4mA alarm level */
#define DAC161S997_ERR_HIGH_REG             0x07    /**< DAC code for the >20mA alarm level */
#define DAC161S997_RESET_REG                0x08    /**< Command: reset the chip */
#define DAC161S997_STATUS_REG               0x09    /**< Status of the chip */
/** @} */

/**
 * @defgroup DAC161S997_STATUS_REG
 * @{
 */
#define DAC161S997_STATUS_REG_LOOP_STS          0x0003  /**< Mask for loop errors */
#define DAC161S997_STATUS_REG_SPI_TIMEOUT_ERR   0x0004  /**< Mask for timeout errors */
#define DAC161S997_STATUS_REG_FERR_STS          0x0008  /**< Mask for frame errors */
/** @} */

/* Function prototypes ********************************************************/
/**
 * @brief    Reads register value from the dac161s997.
 *
 * @param[in]   dev         Device to read
 * @param[in]   addr        Address to read
 * @param[out]  data        Data that has been read
 *
 * @return      0           No errors occurred
 * @return      -ENOEXEC    The device did get expected values
 * @return      dac161s997_spi_xfer() defined errors
 */
int dac161s997_read_reg(dac161s997_dev_t *dev, uint8_t addr,
                            uint16_t *data);

/**
 * @brief    Writes register value to the dac161s997.
 *
 * @param[in]   dev         Device to write
 * @param[in]   addr        Address to write
 * @param[in]   data        Data to be written
 *
 * @return      0           No errors occurred
 * @return      -ENOEXEC    The device did get expected values
 * @return      dac161s997_spi_xfer() defined errors
 */
int dac161s997_write_reg(dac161s997_dev_t *dev, uint8_t addr,
                             uint16_t data);

#ifdef __cplusplus
}
#endif

#endif /* DAC161S997_REGS_H_ */
/** @} */
