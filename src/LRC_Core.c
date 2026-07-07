/**
 * @file LRC_Core.c
 * @brief Core functionality definitions for LRC.
 *
 * @details This file provides definitions of the LRC core functionality exposed in LRC_Core header.
 */

#include "LRC_Core.h"
#include <math.h>
#include <string.h>

/* Locally Used Types*/

/**
 * @brief Internal type for handling measurement channel linked list
 * @details Exists for convenience.
 */
typedef struct LRC_ChannelList_str
{
  LRC_Channel *p_first_channel; /**< pointer to the first measurement channel in the list*/
  uint32_t count;               /**< number of measurement channels total*/
} LRC_ChannelList;

/* Static/Local Variable Declarations*/
static LRC_Config *p_config = NULL;                        /**< Internal copy of the meter configuration */
static LRC_ChannelList channel_list = {NULL, (uint32_t)0}; /**< Internal measurement channel list*/

/* Static/Local functions*/

/** @brief Complete hard reset on a measurement channel
 * @details Will reset the zero cross synch flag so we wait for the next full zero cross to be detected.
 * And resets all accumulators to zero.
 * @param[inout] p_channel - pointer to the measurement channel block to reset
 */
static void Channel_hard_reset(LRC_Channel *const p_channel)
{
  p_channel->inputs.i_sample = (spl_t)0;

  (void)memset(p_channel->window.spl_buffer, 0, LRC_WINDOW_BUFFER_SIZE * sizeof(spl_t));
  p_channel->window.rd_idx = 1;
  p_channel->window.wr_idx = 0;

  p_channel->ac_data.sum = (acc_t)0;
  p_channel->ac_data.persistence_count = 0;
  p_channel->ac_data.raw_output = (fxp_t)0;
  p_channel->ac_data.output = (fxp_t)0;

  p_channel->dc_data.sum = (acc_t)0;
  p_channel->dc_data.persistence_count = 0;
  p_channel->dc_data.raw_output = (fxp_t)0;
  p_channel->dc_data.output = (fxp_t)0;

  LRC_Channel_Reset_Hook(p_channel);
}
/* END OF FUNCTION*/

/** 
* @brief Checks for trip conditions & invokes trip handler if detected
* @param[in] p_channel - pointer to the relevant channel
* @param[in] p_measurement - pointer to the relevant measurement data
* @param[in] p_trip - pointer to the relevant trip characteristics.
*/
static void Channel_trip_check(LRC_Channel *const p_channel, LRC_Measurement* const p_measurement, LRC_TripCharacteristics* const p_trip)
{
  /* Check if a trip has occured*/
  if (p_measurement->output >= p_trip->threshold)
  {
    /* Increment persistence count and check if it has persisted enough to cause a trip*/
    ++p_measurement->persistence_count;
    if (p_measurement->persistence_count >= p_trip->persistence)
    {
      LRC_Trip(p_channel);
    }
    else
    {
      LRC_NoTrip(p_channel);
    }
  }
  else
  {
    /* No trip causes rms_persistence counter to clear*/
    p_measurement->persistence_count = 0;
    LRC_NoTrip(p_channel);
  }
}

/* Externally Available Functions*/

void LRC_Init(LRC_Config *const p_config_arg)
{
  p_config = p_config_arg;
  LRC_ADC_Init();
}

void LRC_Deinit(void)
{
  /* invalidate measurement channel list*/
  LRC_Channel *tmp = channel_list.p_first_channel;

  /* Walk the list*/
  while (NULL != tmp)
  {
    LRC_Channel *next = tmp->p_next;
    tmp->p_next = NULL;
    tmp->id = (uint32_t)0;
    tmp = next;
  }

  channel_list.p_first_channel = NULL;
  channel_list.count = (uint32_t)0;
}

void LRC_ChannelRegister(LRC_Channel *const p_channel)
{
  if (NULL == channel_list.p_first_channel)
  {
    channel_list.p_first_channel = p_channel;
  }
  else
  {
    LRC_Channel *tmp = channel_list.p_first_channel;
    while (NULL != tmp->p_next)
    {
      tmp = tmp->p_next;
    }
    tmp->p_next = p_channel;
  }

  p_channel->p_next = NULL;
  p_channel->id = channel_list.count;
  channel_list.count += 1;

  Channel_hard_reset(p_channel);
}

fxp_t LRC_ChannelCoeffCompute(LRC_Channel *const p_channel, const float i_ref)
{
  LRC_CRITICAL_SECTION_PREPARE();
  const fxp_t scaled_i_ref = LRC_FLOAT_TO_FXP(i_ref);
  fxp_t rms_raw_snapshot = 0;

  LRC_CRITICAL_SECTION_ENTER();
  rms_raw_snapshot = p_channel->ac_data.raw_output;
  LRC_CRITICAL_SECTION_EXIT();

  return LRC_FXP_DIV(rms_raw_snapshot, scaled_i_ref);
}

void LRC_ChannelCoeffSet(LRC_Channel *const p_channel, const fxp_t coeff)
{
  p_channel->fp_coefficient = coeff;
}

void LRC_Start(void)
{
  LRC_Channel *tmp = channel_list.p_first_channel;

  /* Reset measurement channels before starting LRC*/
  while (NULL != tmp)
  {
    Channel_hard_reset(tmp);
    tmp = tmp->p_next;
  }

  /* Start the ADC*/
  LRC_ADC_Start();
}

void LRC_Stop(void)
{
  LRC_ADC_Stop();
}

/** @details The ADC Callback handles:
 *  1. Sample processing and accumulation.
 *  2. Event detection & reaction to slow & fast trips according to the configpassed at initialisation.
 *
 * @note RMS & MEAN is computed using a sliding window
 */
void LRC_CB_ADC(void)
{
  LRC_Channel *p_channel = channel_list.p_first_channel;

  while (NULL != p_channel)
  {
    /****************************************************
     * STORE SAMPLE IN WINDOW
     ****************************************************/
    p_channel->window.spl_buffer[p_channel->window.wr_idx] = p_channel->inputs.i_sample;

    /****************************************************
     * UPDATE ACCUMULATORS & MEASUREMENTS
     ****************************************************/
    /* MEAN*/
    p_channel->dc_data.sum -= (acc_t)p_channel->window.spl_buffer[p_channel->window.rd_idx];
    p_channel->dc_data.sum += (acc_t)p_channel->window.spl_buffer[p_channel->window.wr_idx];
    p_channel->dc_data.raw_output = (fxp_t)((acc_t)(p_channel->dc_data.sum / (LRC_WINDOW_BUFFER_SIZE)));
    p_channel->dc_data.raw_output <<= FXP_FRAC_BITS;
    p_channel->dc_data.output = LRC_FXP_DIV(p_channel->dc_data.raw_output, p_channel->fp_coefficient);

    /* RMS*/
    p_channel->ac_data.sum -= (acc_t)LRC_SqrSpl(p_channel->window.spl_buffer[p_channel->window.rd_idx]);
    p_channel->ac_data.sum += (acc_t)LRC_SqrSpl(p_channel->window.spl_buffer[p_channel->window.wr_idx]);
    p_channel->ac_data.raw_output = (fxp_t)LRC_SqrtAcc(p_channel->ac_data.sum / (LRC_WINDOW_BUFFER_SIZE));
    p_channel->ac_data.raw_output <<= FXP_FRAC_BITS;
    p_channel->ac_data.output = LRC_FXP_DIV(p_channel->ac_data.raw_output, p_channel->fp_coefficient);

    /* Post processing of AC sample, if applicable*/
    LRC_RMS_Computation_Hook(&(p_channel->ac_data.output));

    /***************************************************
     * TRIP CHECK
     ****************************************************/
    Channel_trip_check(p_channel, &(p_channel->ac_data), &(p_config->ac_trip));
    Channel_trip_check(p_channel, &(p_channel->dc_data), &(p_config->dc_trip));

    /****************************************************
     * UPDATE WINDOW INDEX'S
     ****************************************************/
    ++p_channel->window.wr_idx;
    ++p_channel->window.rd_idx;

    if (p_channel->window.wr_idx > (LRC_WINDOW_BUFFER_SIZE - 1))
    {
      p_channel->window.wr_idx = 0;
    }

    if (p_channel->window.rd_idx > (LRC_WINDOW_BUFFER_SIZE - 1))
    {
      p_channel->window.rd_idx = 0;
    }

    /* Next Channel*/
    p_channel = p_channel->p_next;
  }
}
