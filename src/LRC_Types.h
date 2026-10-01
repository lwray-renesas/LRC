/**
 * @file LRC_Types.h
 * @brief Type definitions for LRC.
 *
 * @details This file defines/declares the type definitions of the LRC codebase.
 */

#ifndef _LRC_TYPES_H
#define _LRC_TYPES_H

#include "LRC_Defs.h"

/** @addtogroup API
 *  @{
 */

/** @brief helper macro to perform FXP multiplication, output = a * b
 * With rounding
 * @param[in] a - fixed point input a
 * @param[in] b - fixed point input b
 * @return a * b
 */
#define LRC_FXP_MUL(a,b) \
    (fxp_t)((((fxp_dbl_t)(a) * (fxp_dbl_t)(b)) + ((fxp_dbl_t)1 << (FXP_FRAC_BITS-1))) >> FXP_FRAC_BITS)

/** @brief helper macro to perform FXP division, output = a / b
 * With rounding
 * @param[in] a - fixed point input a
 * @param[in] b - fixed point input b
 * @return a / b
 */
#define LRC_FXP_DIV(a,b) \
    (fxp_t)((((fxp_dbl_t)(a) << FXP_FRAC_BITS) + ((fxp_dbl_t)(b) >> 1)) / (fxp_dbl_t)(b))

/** @brief Helper macro to convert floats to fixed point types
 * Mainly used in logging during development or value setting in code for things like trip thresholds.
 * @param[in] in - input value (floating point) for conversion to fixed point.
 * @return floating point equivalent.
 */
#define LRC_FLOAT_TO_FXP(in) ((fxp_t)((in) * ((float)((fxp_t)1 << FXP_FRAC_BITS))))

/** @brief Helper macro to convert fixed point types to floats
 * Mainly used in logging during development or value setting in code for things like trip thresholds.
 * @param[in] in - input value (fixed point type) for conversion to float.
 * @return fixed point equivalent.
 */
#define LRC_FXP_TO_FLOAT(in) ((float)((float)(in) / ((float)((fxp_t)1 << FXP_FRAC_BITS))))

/** @addtogroup Types
 * @brief LRC Types
 * @details The LRC API relies on a set of unique data structure, enum's and other defined types to operate, here these are
 * outlined.
 *  @{
 */

/**
 * @brief Generic window structure
 * @details Holds sample buffer window
 */
typedef struct LRC_Window_str
{
  spl_t spl_buffer[LRC_WINDOW_BUFFER_SIZE];    /**< Buffer to hold the samples */
  uint16_t wr_idx;                             /**< Write index of the buffer*/
  uint16_t rd_idx;                             /**< Read index of the buffer*/
} LRC_Window;

/**
 * @brief Trip characteristics
 * @details Holds information relevant to setting trip behaviour.
 */
typedef struct LRC_TripCharacteristics_str
{
  uint16_t persistence; /**< Consecutive computations that exceed threshold to cause a trip*/
  fxp_t threshold;      /**< Threshold for to be considered a trip*/
} LRC_TripCharacteristics;

/**
 * @brief Holds information relevant to measurment data
 */
typedef struct LRC_Measurement_str
{
  LRC_Window window;		  /**< Window of samples for the measurment*/
  acc_t sum;                  /**< Current accumulated value*/
  uint16_t persistence_count; /**< Counter to count the number of persistent "Over Thresholds" have ocurred*/
  fxp_t raw_output;           /**< variable to store the latest measurement computed BEFORE coefficient adjustment*/
  fxp_t output;               /**< variable to store the latest measurement computed AFTER coefficient adjustment*/
} LRC_Measurement;

/**
 * @brief Runtime LMA Configuration
 * @details Data structure containing the runtime configuration of LMA system wide operation.
 */
typedef struct LRC_Config_str
{
  LRC_TripCharacteristics ac_trip; /**< rms/ac trip behaviour*/

#ifdef LRC_ENABLE_DC
  LRC_TripCharacteristics dc_trip; /**< mean/dc trip behaviour*/
#endif
} LRC_Config;

/**
 * @brief Measurement channel data
 * @details Container for all data associated with a single measurement channel.
 */
typedef struct LRC_Channel_str
{
  struct LRC_Channel_str *p_next; /**< Singly linked list of channels */
  uint32_t id;                    /**< Zero-indexed channel ID */

  /**
   * @brief Channel input samples
   * @details Holds raw ADC samples loaded prior to processing.
   */
  struct LRC_ChannelInputs
  {
    spl_t iac_sample; /**< Raw ADC current sample for AC processing*/
    spl_t idc_sample; /**< Raw ADC current sample for DC processing*/
  } inputs;

  LRC_Measurement ac_data; /**< measurement information for ac data*/

#ifdef LRC_ENABLE_DC
  LRC_Measurement dc_data; /**< measurement information for dc data*/
#endif

  fxp_t fp_coefficient;           /**< Coefficient for converting raw current value to a fixed point number format*/
} LRC_Channel;

/** @} */

/** @} */

#endif /* _LRC_TYPES_H */
