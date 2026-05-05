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

bool adc_running = false;
bool tripped = false;

typedef struct lpf_t
{
  fxp_t prev_output;
} lpf_t;

lpf_t channel0_lpf = {.prev_output = 0};

void LRC_Channel_Reset_Hook(LRC_Channel *p_channel)
{
  channel0_lpf.prev_output = 0;
}

void LRC_RMS_Computation_Hook(fxp_t *p_rms)
{
  static const fxp_t alpha = 0x2000;
  fxp_t new_part = (fxp_t)((int64_t)(((int64_t)(alpha) * (int64_t)(*p_rms)) >> FXP_FRAC_BITS));
  fxp_t old_part = (fxp_t)((int64_t)(((int64_t)(0x8000 - alpha) * (int64_t)(channel0_lpf.prev_output)) >> FXP_FRAC_BITS));
  *p_rms = new_part + old_part;
  channel0_lpf.prev_output = *p_rms;
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
  tripped = true;
}

void LRC_NoTrip(LRC_Channel *p_channel)
{
  (void)p_channel;
}

void LRC_ADC_Init(void)
{
}

void LRC_ADC_Start(void)
{
  adc_running = true;
}

void LRC_ADC_Stop(void)
{
  adc_running = false;
}

/** @}*/
