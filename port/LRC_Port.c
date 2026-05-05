/** \addtogroup Porting
 *  @{
 *
 * @file LRC_Port.c
 * @brief Porting file definitions for the LRC codebase.
 *
 * @details This file provides definitions of the LRC porting requirements - everything in this file must be considered when
 * porting between platforms.
 */

#include "LRC_Port.h"

void LRC_Channel_Reset_Hook(LRC_Channel *p_channel)
{
  (void)p_channel;
  /* TODO: Populate*/
}

void LRC_RMS_Computation_Hook(fxp_t *p_rms)
{
  (void)p_rms;
  /* TODO: Populate*/
}

spl_t LRC_SqrSpl(spl_t spl)
{
  (void)spl;
  /* TODO: Populate*/
}

acc_t LRC_SqrtAcc(acc_t acc)
{
  (void)acc;
  /* TODO: Populate*/
}

void LRC_Trip(LRC_Channel *p_channel)
{
  (void)p_channel;
  /* TODO: Populate*/
}

void LRC_NoTrip(LRC_Channel *p_channel)
{
  (void)p_channel;
  /* TODO: Populate*/
}

void LRC_ADC_Init(void)
{
  /* TODO: Populate*/
}

void LRC_ADC_Start(void)
{
  /* TODO: Populate*/
}

void LRC_ADC_Stop(void)
{
  /* TODO: Populate*/
}

/** @}*/
