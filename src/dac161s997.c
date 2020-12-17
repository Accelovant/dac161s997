/*
 * Copyright 2020 Kevin Weiss for Accelovant
 */


/* Includes *******************************************************************/
#include <stdint.h>
#include <string.h>
#include <errno.h>

#include "dac161s997.h"
#include "dac161s997_port.h"
#include "internal/dac161s997_regs.h"

/* Private macros *************************************************************/
/* maybe can use some more precision but uint32_t overflow could be an issue */
#define _NA_TO_DAC_TICKS(val)   (val / 366)

/* Private definitions ********************************************************/
#define _DAC_CHIP_RESET_CODE            0xC33C
#define _NOT_PROTECTED                  0
#define _PROTECTED                      1

/* Error levels as per NAMUR NE43 *********************************************/
#define DAC161S997_UNINIT_ALARM_NA  ((uint32_t)3300000)  /**< Uninitialized value in nA */
#define DAC161S997_FAIL_LO_ALARM_NA ((uint32_t)3600000)  /**< Lo error value in nA */
#define DAC161S997_SAT_LO_ALARM_NA  ((uint32_t)3800000)  /**< Lo saturation value in nA */
#define DAC161S997_SAT_HI_ALARM_NA  ((uint32_t)20500000) /**< Hi saturation value in nA */
#define DAC161S997_FAIL_HI_ALARM_NA ((uint32_t)21000000) /**< Hi error value in nA */

#define _ERR_CONFIG_SPI_TIMOUT_400MS    (7 << 1)

/******************************************************************************/
/* Functions                                                                  */
/******************************************************************************/
int dac161s997_init(dac161s997_dev_t *dev)
{
    int err = 0;

    err =
        dac161s997_write_reg(dev, DAC161S997_RESET_REG, _DAC_CHIP_RESET_CODE);
    if (err == -ENOEXEC) {
        return -ENXIO;
    }
    if (err) {
        return err;
    }

    /* We should not worry about protecting as we can catch errors and it
     * shouldn't be sharing communication with other devices.
     */
    err = dac161s997_write_reg(dev, DAC161S997_PROTECT_REG_WR_REG,
                               _NOT_PROTECTED);
    if (err) {
        return err;
    }

    err = dac161s997_write_reg(dev, DAC161S997_ERR_CONFIG_REG,
                               _ERR_CONFIG_SPI_TIMOUT_400MS);
    if (err) {
        return err;
    }

    err =
        dac161s997_write_reg(dev, DAC161S997_ERR_LOW_REG, _NA_TO_DAC_TICKS(
                                 DAC161S997_FAIL_LO_ALARM_NA));
    if (err) {
        return err;
    }

    err =
        dac161s997_write_reg(dev, DAC161S997_ERR_HIGH_REG, _NA_TO_DAC_TICKS(
                                 DAC161S997_FAIL_HI_ALARM_NA));
    if (err) {
        return err;
    }

    err = dac161s997_set_alarm(dev, DAC161S997_ALARM_LOW_FAIL);
    return err;
}

int dac161s997_set_output(dac161s997_dev_t *dev, int32_t n_amps)
{
    if (n_amps < DAC161S997_MIN_NA || n_amps > DAC161S997_MAX_NA) {
        return -EINVAL;
    }
    return dac161s997_write_reg(dev, DAC161S997_DACCODE_REG,
                                _NA_TO_DAC_TICKS(n_amps));
}

int dac161s997_set_alarm(dac161s997_dev_t *dev, DAC161S997_ALARM_t alarm)
{
    if (alarm == DAC161S997_ALARM_LOW_FAIL) {
        return dac161s997_write_reg(dev, DAC161S997_DACCODE_REG,
                                    _NA_TO_DAC_TICKS(DAC161S997_FAIL_LO_ALARM_NA));
    }
    else if (alarm == DAC161S997_ALARM_LOW_SAT) {
        return dac161s997_write_reg(dev, DAC161S997_DACCODE_REG,
                                    _NA_TO_DAC_TICKS(DAC161S997_SAT_LO_ALARM_NA));
    }
    else if (alarm == DAC161S997_ALARM_HIGH_SAT) {
        return dac161s997_write_reg(dev, DAC161S997_DACCODE_REG,
                                    _NA_TO_DAC_TICKS(DAC161S997_SAT_HI_ALARM_NA));
    }
    else if (alarm == DAC161S997_ALARM_HIGH_FAIL) {
        return dac161s997_write_reg(dev, DAC161S997_DACCODE_REG,
                                    _NA_TO_DAC_TICKS(DAC161S997_FAIL_HI_ALARM_NA));
    }
    return -EINVAL;
}

int dac161s997_get_status(dac161s997_dev_t *dev, uint32_t *status)
{
    uint16_t data;
    uint16_t alarm;
    int err = 0;

    /* Reset status each call so errors are not sticky */
    *status = 0;

    err = dac161s997_read_reg(dev, DAC161S997_STATUS_REG, &data);

    if (err == -ENOEXEC) {
        *status |= DAC161S997_STATUS_ABSENT;
    }
    if (err) {
        return err;
    }

    if (data & DAC161S997_STATUS_REG_LOOP_STS) {
        *status |= DAC161S997_STATUS_LOOP_ERR;
    }
    if (data & DAC161S997_STATUS_REG_SPI_TIMEOUT_ERR) {
        *status |= DAC161S997_STATUS_COM_TIMEOUT;
    }
    if (data & DAC161S997_STATUS_REG_FERR_STS) {
        *status |= DAC161S997_STATUS_FRAME_ERR;
    }

    err = dac161s997_read_reg(dev, DAC161S997_DACCODE_REG, &data);
    if (err) {
        return err;
    }

    err = dac161s997_read_reg(dev, DAC161S997_ERR_LOW_REG, &alarm);
    if (data == alarm) {
        *status |= DAC161S997_LO_ALARM_ERR;
    }
    if (err) {
        return err;
    }

    err = dac161s997_read_reg(dev, DAC161S997_ERR_HIGH_REG, &alarm);
    if (data == alarm) {
        *status |= DAC161S997_HI_ALARM_ERR;
    }
    return err;
}
