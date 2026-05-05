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
  return spl * spl;
}

acc_t LRC_SqrtAcc(acc_t acc)
{
  uint64_t rem = 0, root = 0, acc_tmp = (uint64_t)acc;

  for (uint8_t i = 32; i > 0; i--)
  {
    root <<= 1;
    rem = (rem << 2) | (acc_tmp >> (64 - 2));
    acc_tmp <<= 2;
    if (root < rem)
    {
      rem -= root | 1;
      root += 2;
    }
  }
  return (uint32_t)(root >> 1);
}

void LRC_Trip(LRC_Channel *p_channel)
{
  (void)p_channel;
  /* SET GPIO*/
  P2_bit.no0 = 0U;
}

void LRC_NoTrip(LRC_Channel *p_channel)
{
  (void)p_channel;
  /* RESET GPIO*/
  P2_bit.no0 = 1U;
}

void LRC_ADC_Init(void)
{
  /* Nothing to do*/
}

void LRC_ADC_Start(void)
{
  /* Star timer that triggers ADC*/
  R_Config_TAU0_0_Start();
}

void LRC_ADC_Stop(void)
{
  /* Stop ADC and timer that triggers ADC*/
  R_Config_ADC_Stop();
  R_Config_TAU0_0_Stop();
}

/** @}*/
