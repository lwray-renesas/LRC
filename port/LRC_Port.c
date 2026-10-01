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

acc_t LRC_SqrtAcc(acc_t acc)
{
  (void)acc;
  /* TODO: Populate*/
}

void LRC_TripAC(LRC_Channel *p_channel)
{
  (void)p_channel;
}

void LRC_NoTripAC(LRC_Channel *p_channel)
{
  (void)p_channel;
}

#ifdef LRC_ENABLE_DC
void LRC_TripDC(LRC_Channel *p_channel)
{
  (void)p_channel;
}

void LRC_NoTripDC(LRC_Channel *p_channel)
{
  (void)p_channel;
}
#endif

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
