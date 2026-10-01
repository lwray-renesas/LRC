/**
 * @addtogroup Porting
 * @{
 *
 * @file LRC_Defs.h
 * @brief Porting layers type definitions for LRC.
 *
 * @details This file defines/declares the porting layer type definitions of the LRC codebase.
 *
 * @}
 */

#ifndef _LRC_DEFS_H
#define _LRC_DEFS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define LRC_PORT_ADC_BITS (12)

/** @addtogroup Porting
 *  @{
 */

#if 12 == LRC_PORT_ADC_BITS

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
 * @details This type should accommodate the raw ADC sample type.
 * @note The bit width of this type needs to be able to accommodate squaring of the sample (2x sample size in bits)
 * e.g., with 12b samples, squaring gives us 12 * 2 = 24,
 * so the spl_t must be at least 24b
 */
typedef int32_t spl_t;

/** @brief Accumulator type
 * @details This type should accommodate the accumulation of the product of raw ADC sample types.
 * @note the bit width of this type must be able to accommodate the sum of the square of samples.
 * e.g., with spl_t needing at least 20bits & LRC_WINDOW_BUFFER_SIZE = 101.
 * It must be at least ceil(log2(LRC_WINDOW_BUFFER_SIZE)) + 20.
 * = ceil(log2(51)) + 24 = ceil(5.67) + 24 = 30b
 */
typedef uint32_t acc_t;

/** @brief fixed point type alias
 * @details Because RMS is computed as sum of squared samples, averaged and square rooted - the output will always resolve to
 * within the ADC range. However, we must add together samples bits (10b ADC) and FXP_FRAC_BITS because we will shift the RMS
 * computation to get into FXP range. Here we can use 12b + 15b = 27b.
 */
typedef uint32_t fxp_t;

/** @brief fixed point type alias for double width.
 * @details With fixed point arithmetic (specifically division and multiplication) the value is shifted left by FXP_FRAC_BITS.
 * Meaning the fxp_dbl_t must be able to store the number of bits required of fxp_t + FXP_FRAC_BITS.
 * In our case this is 27b + 15b = 42b.
 */
typedef uint64_t fxp_dbl_t;

#elif 10 == LRC_PORT_ADC_BITS

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
 * @details This type should accommodate the raw ADC sample type.
 * @note The bit width of this type needs to be able to accommodate squaring of the sample (2x sample size in bits)
 * e.g., with 10b samples, squaring gives us 10 * 2 = 20,
 * so the spl_t must be at least 20b
 */
typedef int32_t spl_t;

/** @brief Accumulator type
 * @details This type should accommodate the accumulation of the product of raw ADC sample types.
 * @note the bit width of this type must be able to accommodate the sum of the square of samples.
 * e.g., with spl_t needing at least 20bits & LRC_WINDOW_BUFFER_SIZE = 101.
 * It must be at least ceil(log2(LRC_WINDOW_BUFFER_SIZE)) + 20.
 * = ceil(log2(51)) + 20 = ceil(5.67) + 20 = 26b
 */
typedef uint32_t acc_t;

/** @brief fixed point type alias
 * @details Because RMS is computed as sum of squared samples, averaged and square rooted - the output will always resolve to
 * within the ADC range. However, we must add together samples bits (10b ADC) and FXP_FRAC_BITS because we will shift the RMS
 * computation to get into FXP range. Here we can use 10b + 15b = 25b.
 */
typedef uint32_t fxp_t;

/** @brief fixed point type alias for double width.
 * @details With fixed point arithmetic (specifically division and multiplication) the value is shifted left by FXP_FRAC_BITS.
 * Meaning the fxp_dbl_t must be able to store the number of bits required of fxp_t + FXP_FRAC_BITS.
 * In our case this is 25b + 15b = 40b.
 */
typedef uint64_t fxp_dbl_t;

#elif 8 == LRC_PORT_ADC_BITS

  /** @brief fractional bits in fixed point arithmetic
   * @note this must be considered along side spl_t and acc_t & is recommended to be around half of spl_t bitwidth.
   */
  #define FXP_FRAC_BITS (8)

  /** @brief The size of the internal LRC_Channel buffer for windowing the Irms computation.
   * @note This number is used to size an array for every LRC_Channel declared of this number of spl_t.
   * Therefore it should be considered carefully with regards to RAM consumption.
   * @warning MUST BE LARGER THAN 2
   */
  #define LRC_WINDOW_BUFFER_SIZE (101U)

/** @brief Raw ADC sample type
 * @details This type should accommodate the raw ADC sample type.
 * @note The bit width of this type needs to be able to accommodate squaring of the sample (2x sample size in bits)
 * e.g., with 10b samples, squaring gives us 8 * 2 = 16,
 * so the spl_t must be at least 16b
 */
typedef int16_t spl_t;

/** @brief Accumulator type
 * @details This type should accommodate the accumulation of the product of raw ADC sample types.
 * @note the bit width of this type must be able to accommodate the sum of the square of samples.
 * e.g., with spl_t needing at least 20bits & LRC_WINDOW_BUFFER_SIZE = 101.
 * It must be at least ceil(log2(LRC_WINDOW_BUFFER_SIZE)) + 16.
 * = ceil(log2(51)) + 16 = ceil(5.67) + 16 = 24b
 */
typedef uint32_t acc_t;

/** @brief fixed point type alias
 * @details Because RMS is computed as sum of squared samples, averaged and square rooted - the output will always resolve to
 * within the ADC range. However, we must add together samples bits (8b ADC) and FXP_FRAC_BITS because we will shift the RMS
 * computation to get into FXP range. Here we can use 8b + 8b = 16b.
 */
typedef uint16_t fxp_t;

/** @brief fixed point type alias for double width.
 * @details With fixed point arithmetic (specifically division and multiplication) the value is shifted left by FXP_FRAC_BITS.
 * Meaning the fxp_dbl_t must be able to store the number of bits required of fxp_t + FXP_FRAC_BITS.
 * In our case this is 16b + 8b = 24b.
 */
typedef uint32_t fxp_dbl_t;

#else
  #error "Please select valid ADC sample bit count"
#endif /* LRC_PORT_ADC_BITS*/

/** @}*/

#endif /* _LRC_DEFS_H */
