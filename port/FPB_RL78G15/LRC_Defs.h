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

/** @addtogroup Porting
 *  @{
 */

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
typedef uint64_t acc_t;

/** @brief fixed point type alias
 * @details This is the size of the sample type as this is only used to store RMS, which will never exceed maximum spl_t.
 */
typedef uint32_t fxp_t;

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

/** @brief helper macro to perform FXP multiplication, output = a * b
 * With rounding
 * @param[in] a - fixed point input a
 * @param[in] b - fixed point input b
 * @return a * b
 */
#define LRC_FXP_MUL(a,b) \
    (fxp_t)((((acc_t)(a) * (acc_t)(b)) + ((acc_t)1 << (FXP_FRAC_BITS-1))) >> FXP_FRAC_BITS)

/** @brief helper macro to perform FXP division, output = a / b
 * With rounding
 * @param[in] a - fixed point input a
 * @param[in] b - fixed point input b
 * @return a / b
 */
#define LRC_FXP_DIV(a,b) \
    (fxp_t)((((acc_t)(a) << FXP_FRAC_BITS) + ((acc_t)(b) >> 1)) / (acc_t)(b))

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

/** @}*/

#endif /* _LRC_DEFS_H */
