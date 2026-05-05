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
#define LRC_WINDOW_BUFFER_SIZE (51U)

/** @brief Raw ADC sample type
 * @details This type should accomodate the raw ADC sample type.
 * @note Must have bitwidth large enough to contain the square of the RAW ADC reading.
 */
typedef int32_t spl_t;

/** @brief Accumulator type
 * @details This type should accomodate the accumulation of the product of raw ADC sample types.
 * It is recommended to be at least double the bit width of spl_t.
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
 * @brief Runtime LMA Configuration
 * @details Data structure containing the runtime configuration of LMA system wide operation.
 */
typedef struct LRC_Config_str
{
  /**
   * @brief Channel input samples
   * @details Holds raw ADC samples loaded prior to processing.
   */
  struct LRC_TripCharacteristics
  {
    uint32_t persistence; /**< Consecutive RMS computations that exceed threshold to cause a trip*/
    fxp_t threshold;      /**< Threshold for RMS to be considered a trip*/
  } trip;

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
   * @brief Generic accumulator structure
   * @details Holds accumulator state and sample count.
   */
  struct LRC_Accs
  {
    spl_t spl_sq_buffer[LRC_WINDOW_BUFFER_SIZE]; /**< Buffer to hold the squared values */
    uint32_t wr_idx;                             /**< Write index of the buffer*/
    uint32_t rd_idx;                             /**< Read index of the buffer*/
    acc_t sum_sq;                                /**< Current accumulated value*/
  } acc;
  uint32_t persistence_count; /**< Counter to count the number of persistent "RMS Over Thresholds" have ocurred*/
  fxp_t rms_raw;              /**< variable to store the latest RMS computed BEFORE coefficient adjustment*/
  fxp_t rms;                  /**< variable to store the latest RMS computed AFTER coefficient adjustment*/
  fxp_t fp_coefficient;       /**< Coefficient for converting raw RMS value to a fixed point number format*/
} LRC_Channel;

/** @} */

/** @} */

#endif /* _LRC_TYPES_H */
