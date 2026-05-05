/**
 * @file LRC_Core.h
 * @brief Core functionality declarations for LRC.
 *
 * @details This file provides declarations of the LRC core functionality.
 */

#ifndef _LRC_CORE_H
#define _LRC_CORE_H

#include "LRC_Port.h"

/** @addtogroup API
 * @brief LRC API
 * @details The LRC API acts to control the behaviour of LRC.
 *  @{
 */

/** @addtogroup Control
 * @brief LRC Control API
 * @details The LRC Control API is used to control the functionality of LRC, that is to say it defines LRC's behaviour. For
 * example starting and stopping operations.
 *  @{
 */

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
#define LRC_FLOAT_TO_FXP(in) ((fxp_t)((in) * ((float)(1 << FXP_FRAC_BITS))))

/** @brief Helper macro to convert fixed point types to floats
 * Mainly used in logging during development or value setting in code for things like trip thresholds.
 * @param[in] in - input value (fixed point type) for conversion to float.
 * @return fixed point equivalent.
 */
#define LRC_FXP_TO_FLOAT(in) ((float)((float)(in) / ((float)(1 << FXP_FRAC_BITS))))

/** @brief Initialises the Light-Weight Residual Current Framework according to the config.
 * @param[in] p_config_arg - pointer to the configuration structure.
 */
void LRC_Init(LRC_Config *const p_config_arg);

/** @brief Deinitialises LRC
 * @details Deconstructs the measurement channel linked list so callbacks have no structures to work on.
 * @warning Does NOT stop drivers operating - call LRC_Stop first if this is the desired behaviour.
 */
void LRC_Deinit(void);

/** @brief Registers a measurement channel to the library
 * @details Do once on power up.
 * @param[in] p_channel - pointer to the measurement channel
 */
void LRC_ChannelRegister(LRC_Channel *const p_channel);

/** @brief Function called to recompute the coefficient.
 * Computes the channel coefficient when provided a reference signal.
 * Typically used during development, however could form part of a factory calibration routine.
 * @param[in] p_channel - pointer to the channel to work on.
 * @param[in] i_ref - the reference current being provided.
 * @return The computed channel coefficient.
 */
fxp_t LRC_ChannelCoeffCompute(LRC_Channel *const p_channel, const float i_ref);

/** @brief Sets the channel coefficient for converting raw RMS values to fixed point currents.
 * @details The coefficient should convert raw ADC values to current values reflected in fixed point format.
 * @param[in] p_channel - pointer to the measurement channel
 * @param[in] coeff - coefficient applicable to the currrent channel.
 */
void LRC_ChannelCoeffSet(LRC_Channel *const p_channel, const fxp_t coeff);

/** @brief Starts LRC Operation
 * @details Starts the I RMS computation state machine and all associated drivers.
 */
void LRC_Start(void);

/** @brief Stops LRC Operation
 * @details Stops the I RMS computation state machine and all associated drivers.
 */
void LRC_Stop(void);

/** @} */

/** @} */

/** @addtogroup Callbacks
 * @brief LRC Interrupt Callbacks
 * @details The LRC Callbacks are the foundation of LRC's operation, these callbacks are installed in the appropraite interrupts
 * service routines (ISR's) and invoke LRC's core operation.
 *  @{
 */

/** @brief ADC CALLBACK - Processes the ADC samples according to the number of measurement channels registered.
 */
void LRC_CB_ADC(void);

/** @} */

#endif /* _LRC_CORE_H */
