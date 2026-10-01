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

void LRC_TripAC(LRC_Channel *p_channel)
{
  (void)p_channel;
  /* SET GPIO*/
  fsp_err_t err = R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_09, BSP_IO_LEVEL_HIGH);
  if (FSP_SUCCESS != err)
  {
    __BKPT(0);
  }
}

void LRC_NoTripAC(LRC_Channel *p_channel)
{
  (void)p_channel;
  /* RESET GPIO*/
  fsp_err_t err = R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_01_PIN_09, BSP_IO_LEVEL_LOW);
  if (FSP_SUCCESS != err)
  {
    __BKPT(0);
  }
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
  fsp_err_t err = R_ADC_D_Open(&g_adc0_ctrl, &g_adc0_cfg);
  if (FSP_SUCCESS != err)
  {
    __BKPT(0);
  }

  err = R_ADC_D_ScanCfg(&g_adc0_ctrl, &g_adc0_channel_cfg);
  if (FSP_SUCCESS != err)
  {
    __BKPT(0);
  }

  err = R_TAU_Open(&g_timer0_ctrl, &g_timer0_cfg);
  if (FSP_SUCCESS != err)
  {
    __BKPT(0);
  }
}

void LRC_ADC_Start(void)
{
  fsp_err_t err = R_ADC_D_ScanStart(&g_adc0_ctrl);
  if (FSP_SUCCESS != err)
  {
    __BKPT(0);
  }

  err = R_TAU_Start(&g_timer0_ctrl);
  if (FSP_SUCCESS != err)
  {
    __BKPT(0);
  }
}

void LRC_ADC_Stop(void)
{
  fsp_err_t err = R_ADC_D_ScanStop(&g_adc0_ctrl);
  if (FSP_SUCCESS != err)
  {
    __BKPT(0);
  }

  err = R_TAU_Stop(&g_timer0_ctrl);
  if (FSP_SUCCESS != err)
  {
    __BKPT(0);
  }
}

/** @}*/
