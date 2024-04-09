/*
 * Copyright 2020 Kevin Weiss for Accelovant
 */

/**
 ******************************************************************************
 * @addtogroup DRIVER
 * @{
 * @file			dac161s997_port.h
 * @author			Kevin Weiss
 * @date			21.05.2020
 * @brief			SPI API for the dac161s997 driver port.
 *
 ******************************************************************************
 */

#ifndef DAC161S997_PORT_H_
#define DAC161S997_PORT_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes *******************************************************************/
#include <stdint.h>
#include <errno.h>
#include <stddef.h>
#include "dac161s997_types.h"


/* Function prototypes ********************************************************/
/**
 * @addtogroup PORTABLE
 * @{
 * @brief	Handles the SPI transfer to dac161s997 chip.
 *
 * This checks the status of the device to find out the device is still working
 * or the output line is still connected.
 *
 * @pre		Device must be initialized with dac161s997_init
 *
 * @param[in]	dev			Device to xfer
 * @param[in]	tx_buf		Bytes to send on MOSI
 * @param[out]	rx_buf		Bytes read from MISO
 * @param[in]	size		Number of bytes to xfer
 *
 * @return		0			Status update successful
 * @return      depends on user implementation
 *
 * @Warning Must be implemented during port!
 */
int dac161s997_spi_xfer(dac161s997_dev_t *dev, uint8_t *tx_buf,
                            uint8_t *rx_buf, size_t size);
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* DAC161S997_PORT_H_ */
/** @} */
