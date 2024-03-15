/*
 * Copyright 2020 Kevin Weiss for Accelovant
 */

/**
 ******************************************************************************
 * @addtogroup DRIVER
 * @{
 * @file			dac161s997.h
 * @author			Kevin Weiss
 * @date			21.05.2020
 * @brief			Driver for the dac161s997 to current
 *
 * This driver is designed to setup and output current 4/20 mA. It must
 * be initialized then polled less than the timeout value which can be set to
 * a maximum if 400ms. As this is a current loop, a loop must be connected at
 * all times otherwise an error will occur.
 ******************************************************************************
 */

#ifndef DAC161S997_H_
#define DAC161S997_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes *******************************************************************/
#include <stdint.h>
#include <errno.h>
#include "dac161s997_types.h"

/* Defines ********************************************************************/
#define DAC161S997_MIN_NA   ((uint32_t)2000000)     /**< Min valid nA */
#define DAC161S997_MAX_NA   ((uint32_t)24000000)    /**< Max valid nA */

#define DAC161S997_ALARM_LOW_FAIL_ERR   0x0100 /**< Error flag for low device failure */
#define DAC161S997_ALARM_LOW_SAT_ERR    0x0200 /**< Error flag for value set to lower bound saturation */
#define DAC161S997_ALARM_UNINIT_ERR     0x0400 /**< Error flag for device uninitialized */
#define DAC161S997_ALARM_HI_SAT_ERR     0x0800 /**< Error flag for value set to upper bound saturation */
#define DAC161S997_ALARM_HI_FAIL_ERR    0x1000 /**< Error flag for high device failure */

/**
 * @defgroup I420_STATUS_MASK
 * @{
 */
#define DAC161S997_STATUS_ABSENT        0x01    /**< Cannot communicate with device */
#define DAC161S997_STATUS_LOOP_ERR      0x02    /**< Error with 420 loop */
#define DAC161S997_STATUS_COM_TIMEOUT   0x04    /**< SPI communication timeout */
#define DAC161S997_STATUS_FRAME_ERR     0x08    /**< SPI frame error */
#define DAC161S997_LO_ALARM_ERR         0x10    /**< Output at low error value */
#define DAC161S997_HI_ALARM_ERR         0x20    /**< Output at high error value */
/** @} */

/* Typedefs *******************************************************************/
typedef enum {
    DAC161S997_ALARM_LOW_FAIL   = DAC161S997_ALARM_LOW_FAIL_ERR,    /**< Alarm for low device failure */
    DAC161S997_ALARM_LOW_SAT    = DAC161S997_ALARM_LOW_SAT_ERR,     /**< Alarm for value set to lower bound saturation */
    DAC161S997_ALARM_UNINIT     = DAC161S997_ALARM_UNINIT_ERR,      /**< Alarm for device uninitialized */
    DAC161S997_ALARM_HIGH_SAT   = DAC161S997_ALARM_HI_SAT_ERR,      /**< Alarm for value set to upper bound saturation */
    DAC161S997_ALARM_HIGH_FAIL  = DAC161S997_ALARM_HI_FAIL_ERR,     /**< Alarm for high device failure */
} DAC161S997_ALARM_t;       /**< DAC161S997 alarm types */

/* Function prototypes ********************************************************/
/**
 * @brief   Initialize the dac161s997 chip.
 *
 * Initial conditions and registers must be setup before using the driver
 * functions. This initializes registers, clears all previous states, and
 * allows the device to be used. If the initialization was not successful it
 * returns an error that should be used to indicate to the user a problem has
 * occurred.
 *
 * @param[in]	dev		device to initialize
 *
 * @return		0               No errors occurred
 * @return      -ENXIO			Device is not present
 * @return      -ENOEXEC		The device did get expected values
 * @return		errors from dac161s997_spi_xfer()
 */
int dac161s997_init(dac161s997_dev_t *dev);

/**
 * @brief	Sets current in nA to output.
 *
 * For example, if nA == 5000000 the output will be 5mA.
 *
 * If there is a problem setting the current such as out of range or the
 * device is no longer communicating, an error will return output will not
 * change.
 *
 * @pre		Device must be initialized with dac161s997_init
 *
 * @param[in]	dev			Device to select
 * @param[in]	n_amps		Temperature to set in nA
 *
 * @return		0			Status update successful
 * @return      -ENOEXEC	The device did get expected values
 * @return		-EINVAL		Value out of range
 * @return		errors from dac161s997_spi_xfer()
 */
int dac161s997_set_output(dac161s997_dev_t *dev, int32_t n_amps);

/**
 * @brief	Sets alarm values to output.
 *
 * Sets either high or low alarm parameters. This is used to indicate to the
 * device reading the output that a problem has occurred. The values that are
 * set are out of range of the standard 4-20mA.
 *
 * @pre		Device must be initialized with dac161s997_init
 *
 * @param[in]	dev			Device to select
 * @param[in]	alarm		Type of alarm to set {DAC161S997_ALARM_LOW, DAC161S997_ALARM_HIGH}
 *
 * @return		0			Status update successful
 * @return      -ENOEXEC	The device did get expected values
 * @return		-EINVAL		Value out of range
 * @return		errors from dac161s997_spi_xfer()
 */
int dac161s997_set_alarm(dac161s997_dev_t *dev, DAC161S997_ALARM_t alarm);

/**
 * @brief	returns the status of an i420 device
 *
 * This checks the status of the device to find out the device is still working
 * or the output line is still connected.
 *
 * @pre		Device must be initialized with dac161s997_init
 *
 * @param[in]	dev			Device to select
 * @param[out]	status		Status bits of @ref I420_STATUS_MASK
 *
 * @return		0			Status update successful
 * @return		errors from dac161s997_spi_xfer()
 */
int dac161s997_get_status(dac161s997_dev_t *dev, uint32_t *status);

#ifdef __cplusplus
}
#endif

#endif /* DAC161S997_H_ */
/** @} */
