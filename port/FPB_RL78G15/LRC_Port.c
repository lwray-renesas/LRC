/** \addtogroup Porting
 *  @{
 *
 * @file LRC_Port.c
 * @brief Porting file definitions for the LRC codebase.
 *
 * @details This file provides definitions of the LRC porting requirements -
 * everything in this file must be considered when porting between platforms.
 */

#include "LRC_Port.h"

#define BITS_SUB2 ((sizeof(acc_t) * 8) - 2)
#define BITS_DIV2 ((sizeof(acc_t) * 8) / 2)

void LRC_Channel_Reset_Hook(LRC_Channel *p_channel)
{
  (void)p_channel;
  /* TODO: Populate*/
}

acc_t LRC_SqrtAcc(acc_t acc)
{
  acc_t rem = 0, root = 0, acc_tmp = acc;
  for (uint8_t i = BITS_DIV2; i > 0; i--)
  {
    root <<= 1;
    rem = (rem << 2) | (acc_tmp >> BITS_SUB2);
    acc_tmp <<= 2;
    if (root < rem)
    {
      rem -= root | 1;
      root += 2;
    }
  }
  return (acc_t)(root >> 1);
}

void LRC_Trip(LRC_Channel *p_channel)
{
  (void)p_channel;
}

void LRC_NoTrip(LRC_Channel *p_channel)
{
  (void)p_channel;
}

void LRC_ADC_Init(void)
{ /* Nothing to do*/
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
