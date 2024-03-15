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
 * @brief			Forward declaration of types.
 *
 ******************************************************************************
 */

#ifndef DAC161S997_TYPES_H_
#define DAC161S997_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Function prototypes ********************************************************/
/**
 * @addtogroup PORTABLE
 * @{
 * @brief	Forward declaration of the dac161s997_dev_t type.
 *
 * User implemented argument to select the dac161s997 chip.
 *
 * @Warning Must be defined during port!
 */
typedef struct dac161s997_dev_t dac161s997_dev_t;
/** @} */

#ifdef __cplusplus
}
#endif

#endif /* DAC161S997_TYPES_H_ */
/** @} */

