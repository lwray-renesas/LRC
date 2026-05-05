/** \addtogroup Porting
 * @brief LRC Porting Layer
 * @details The LRC porting layer is used to perform most (ideally all) porting activities between platforms to enable LRC to
 * run across different different cores. To perform a port, ensure you fulfill the functional requirements of the API in the
 * porting files - using an existing port as reference may help.
 *
 * @note The documentation here is based on empty "skeleton" porting files and should be used to aid with creating a new
 * port or navigating existing ports.
 *  @{
 *
 * @file LRC_Port.h
 * @brief Porting file declarations for the LRC codebase.
 *
 * @details This file provides declarations of the LRC porting requirements - everything in this file must be considered when
 * porting between platforms.
 * This file also includes LRC_Types.h for the core application.
 */

#ifndef _LRC_PORT_H
#define _LRC_PORT_H

#include "LRC_Types.h"

/** @brief Macro to prepare function/code block for a crticial section.
 * @details Generally stores interrupt state information for restoration on exit of critical section.
 */
#define LRC_CRITICAL_SECTION_PREPARE()

/** @brief Macro used to enter critical section
 * @details Generally disables interrupts.
 */
#define LRC_CRITICAL_SECTION_ENTER()

/** @brief Macro used to exit critical section
 * @details Generally restores interrupts state.
 */
#define LRC_CRITICAL_SECTION_EXIT()

/******************
 * HOOKS
 ******************/
/**
 * @brief Is called when resetting channel (i.e., when starting LRC)
 * @param[in] p_channel - pointer to the measurement channel being reset.
 *
 * @note You can use the channel ID to verify which channel is being reset.
 */
void LRC_Channel_Reset_Hook(LRC_Channel *p_channel);

/**
 * @brief Allows a developer to perform post processing on RMS Computations.
 * @details This is called BEFORE the trip comparisons but after RMS computation update.
 * Intended use here is to perform filtering like LPF.
 * @param[in] p_rms - pointer to the rms to work on.
 */
void LRC_RMS_Computation_Hook(fxp_t *p_rms);

/******************
 * MATHS
 ******************/
/**
 * @brief performs a square operation on the sample
 * @param[in] spl - Sample to perform the square on
 *
 * @return square of spl
 */
spl_t LRC_SqrSpl(spl_t spl);

/**
 * @brief performs a square root operation on the accumulator type.
 * @param[in] acc - Accumulator to perform the square root on.
 *
 * @return square root of acc
 */
acc_t LRC_SqrtAcc(acc_t acc);

/******************
 * TRIP BEHAVIOUR
 ******************/
/**
 * @brief Is called immeditely on detecting a trip on any given channel
 * @param[in] p_channel - pointer to the measurement channel
 *
 * @note You can use the channel ID to verify which channel has tripped.
 */
void LRC_Trip(LRC_Channel *p_channel);

/**
 * @brief Is called immeditely on detecting a no trip on any given channel
 * @param[in] p_channel - pointer to the measurement channel
 *
 * @note You can use the channel ID to verify which channel has not tripped.
 */
void LRC_NoTrip(LRC_Channel *p_channel);

/******************
 * DRIVERS
 ******************/
/** @brief Initialises ADC
 * @details Doesn't start it, just prepares it.
 */
void LRC_ADC_Init(void);

/** @brief Starts the ADC running
 * @details This function should start the ADC in a such a way that it results in a periodic "sampling complete" interrupt which
 * enters the ISR that calls LRC_CB_ADC.
 */
void LRC_ADC_Start(void);

/** @brief Stops the ADC running
 * @details This function should stop the ADC in a such a way that it stops the periodic "sampling complete" interrupt which
 * enters the ISR that calls LRC_CB_ADC.
 */
void LRC_ADC_Stop(void);

/**@} */

#endif /* _LRC_PORT_H */
