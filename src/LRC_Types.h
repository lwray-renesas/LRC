/**
 * @addtogroup Porting
 * @{
 *
 * @file LRC_Types.h
 * @brief Type definitions for LRC.
 *
 * @details This file defines/declares the type definitions of the LRC codebase.
 *
 * @}
 */

#ifndef _LRC_TYPES_H
#define _LRC_TYPES_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/** @addtogroup Porting
 *  @{
 */

/** @brief fractional bits in fixed point arithmetic
 * @note this must be considered along side spl_t and acc_t & is recommended to be around half of spl_t bitwidth.
 */
#define FXP_FRAC_BITS (15)

/** @brief The size of the internal LRC_Channel buffer for windowing the Irms computation.
 * @note This number is used to size an array for every LRC_Channel declared of this number of spl_t.
 * Therefore it should be considered carefully with regards to RAM consumption.
 * @warning MUST BE LARGER THAN 2
 */
#define LRC_WINDOW_BUFFER_SIZE (101U)

/** @brief Raw ADC sample type
 * @details This type should accomodate the raw ADC sample type.
 * @note The bit width of this type needs to be able to accomodate squaring of the sample (2x sample size in bits)
 * OR [sample size in bits + FXP_FRAC_BITS], whichever is greater
 * e.g., with 10b samples & FXP_FRAC_BITS = 15, squaring gives us 10 * 2 = 20, but shifting gives us 10 + 15 = 25.
 * so the spl_t must be at least 25b
 */
typedef int32_t spl_t;

/** @brief Accumulator type
 * @details This type should accomodate the accumulation of the product of raw ADC sample types.
 * @note the bit width of this type must be able to accomodate the sum of the square of samples + left shifting of FXP_FRAC_BITS.
 * e.g., with spl_t needing at least 25bits & LRC_WINDOW_BUFFER_SIZE = 51 & FXP_FRAC_BITS = 15.
 * It must be at least ceil(log2(LRC_WINDOW_BUFFER_SIZE)) + 25 + 15.
 * = ceil(log2(51)) + 25 + 15 = ceil(5.67) + 25 + 15 = 46b
 */
typedef int64_t acc_t;

/** @}*/

/** @addtogroup API
 *  @{
 */

/** @addtogroup Types
 * @brief LRC Types
 * @details The LRC API relies on a set of unique data structure, enum's and other defined types to operate, here these are
 * outlined.
 *  @{
 */

/** @brief fixed point type alias
 * @details This is the size of the sample type as this is only used to store RMS, which will never exceed maximum spl_t.
 */
typedef spl_t fxp_t;

/**
 * @brief Trip characteristics
 * @details Holds information relevant to setting trip behaviour.
 */
typedef struct LRC_TripCharacteristics_str
{
  uint32_t persistence; /**< Consecutive computations that exceed threshold to cause a trip*/
  fxp_t threshold;      /**< Threshold for to be considered a trip*/
} LRC_TripCharacteristics;

/**
 * @brief Holds information relevant to measurment data
 */
typedef struct LRC_Measurement_str
{
  acc_t sum;                  /**< Current accumulated value*/
  uint32_t persistence_count; /**< Counter to count the number of persistent "Over Thresholds" have ocurred*/
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
  LRC_TripCharacteristics dc_trip; /**< mean/dc trip behaviour*/
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
    spl_t i_sample; /**< Raw ADC current sample */
  } inputs;

  /**
   * @brief Generic window structure
   * @details Holds sample buffer window
   */
  struct LRC_Window
  {
    spl_t spl_buffer[LRC_WINDOW_BUFFER_SIZE];    /**< Buffer to hold the samples */
    uint32_t wr_idx;                             /**< Write index of the buffer*/
    uint32_t rd_idx;                             /**< Read index of the buffer*/
  } window;

  LRC_Measurement ac_data; /**< measurement information for ac data*/
  LRC_Measurement dc_data; /**< measurement information for dc data*/

  fxp_t fp_coefficient;           /**< Coefficient for converting raw current value to a fixed point number format*/
} LRC_Channel;

/** @} */

/** @} */

#endif /* _LRC_TYPES_H */
