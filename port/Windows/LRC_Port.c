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

#define BITS_SUB2 ((sizeof(acc_t) * 8) - 2)
#define BITS_DIV2 ((sizeof(acc_t) * 8) / 2)

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

void LRC_TripAC(LRC_Channel *p_channel)
{
  tripped = true;
}

void LRC_NoTripAC(LRC_Channel *p_channel)
{
  (void)p_channel;
}

#ifdef LRC_ENABLE_DC
void LRC_TripDC(LRC_Channel *p_channel);
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
