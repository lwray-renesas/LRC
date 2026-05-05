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

  p_channel->acc.sum_sq = (acc_t)0;
  (void)memset(p_channel->acc.spl_sq_buffer, 0, LRC_WINDOW_BUFFER_SIZE * sizeof(spl_t));
  p_channel->acc.rd_idx = 1;
  p_channel->acc.wr_idx = 0;
  p_channel->persistence_count = 0;
  p_channel->rms = (fxp_t)0;

  LRC_Channel_Reset_Hook(p_channel);
}
/* END OF FUNCTION*/

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
	rms_raw_snapshot = p_channel->rms_raw;
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
 * @note This function expects all samples to be aligned  Q15.16 (32bit signed) format!
 *
 * @note RMS is computed using a block-based window
 *       (not sliding), with accumulation reset after each window
 */
void LRC_CB_ADC(void)
{
  LRC_Channel *p_channel = channel_list.p_first_channel;

  while (NULL != p_channel)
  {
    /****************************************************
     * COMPUTE SQUARE AND STORE IN WINDOW
     ****************************************************/
    p_channel->acc.spl_sq_buffer[p_channel->acc.wr_idx] = LRC_SqrSpl(p_channel->inputs.i_sample);

    /****************************************************
     * UPDATE ACCUMULATION WINDOW & RMS
     ****************************************************/
    /* Subtract the oldest reading*/
    if (p_channel->acc.rd_idx < LRC_WINDOW_BUFFER_SIZE)
    {
      p_channel->acc.sum_sq -= (acc_t)p_channel->acc.spl_sq_buffer[p_channel->acc.rd_idx];
    }
    /* Add the latest reading*/
    if (p_channel->acc.wr_idx < LRC_WINDOW_BUFFER_SIZE)
    {
      p_channel->acc.sum_sq += (acc_t)p_channel->acc.spl_sq_buffer[p_channel->acc.wr_idx];
    }
    /* Recompute RMS*/
    p_channel->rms_raw = (fxp_t)LRC_SqrtAcc(p_channel->acc.sum_sq / (LRC_WINDOW_BUFFER_SIZE));
    /* Convert RMS to fixed point notation */
    p_channel->rms_raw <<= FXP_FRAC_BITS;
    /* Apply coefficient & convert to fixed point notation*/
    p_channel->rms = LRC_FXP_DIV(p_channel->rms_raw, p_channel->fp_coefficient);

    /* Post processing, if applicable*/
    LRC_RMS_Computation_Hook(&(p_channel->rms));

    /***************************************************
     * TRIP CHECK
     ****************************************************/
    /* Check if a trip has occured*/
    if (p_channel->rms >= p_config->trip.threshold)
    {
      /* Increment persistence count and check if it has persisted enough to cause a trip*/
      ++p_channel->persistence_count;
      if (p_channel->persistence_count >= p_config->trip.persistence)
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
      /* No trip causes persistence counter to clear*/
      p_channel->persistence_count = 0;
      LRC_NoTrip(p_channel);
    }

    /****************************************************
     * UPDATE WINDOW INDEX'S
     ****************************************************/
    ++p_channel->acc.wr_idx;
    ++p_channel->acc.rd_idx;

    if (p_channel->acc.wr_idx > (LRC_WINDOW_BUFFER_SIZE - 1))
    {
      p_channel->acc.wr_idx = 0;
    }

    if (p_channel->acc.rd_idx > (LRC_WINDOW_BUFFER_SIZE - 1))
    {
      p_channel->acc.rd_idx = 0;
    }

    /* Next Channel*/
    p_channel = p_channel->p_next;
  }
}
