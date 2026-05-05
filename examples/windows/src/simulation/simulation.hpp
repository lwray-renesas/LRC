#ifndef _SIMULTAION_H_
#define _SIMULTAION_H_

#include <atomic>
#include <cstdint>
#include <memory>
#include <vector>

extern "C"
{
#include "LRC_Core.h"
  extern bool adc_running;
  extern bool tripped;
}

/** @brief interface param structure for simulation. */
typedef struct SimulationParams
{
  std::shared_ptr<std::vector<double>> p_current_samples; /**< Pointer to the current samples */
  double f_sample;                                        /**< sampling frequency*/
  double rms_signal;                                      /**< rms of signal*/
  double max_current;                                     /**< maximum sensible current in system*/
  uint32_t bit_width;                                     /**< number of ADC bits to simulate*/
  bool single_ended_adc;                                  /**< true if the ADC is single ended, false is differential*/

  bool rogowski;                     /**< flag to enable/disable rogowski on startup */
  std::atomic<bool> stop_simulation; /**< signal to stop the simulation*/
} SimulationParams;

/** @brief results of smiulation*/
typedef struct SimulationResults
{
  std::shared_ptr<std::vector<double>> p_current_samples; /**< Pointer to the current samples */
  std::shared_ptr<std::vector<int32_t>> p_adc_samples;    /**< Pointer to the adc samples */
  size_t trip_idx;                                        /**< Index for which LRC Tripped*/
  std::shared_ptr<std::vector<int32_t>> p_rms_buf; /**< Pointer to vector of computed RMS for every iteration of LRC_CB_ADC*/
  std::shared_ptr<std::vector<double>>
      p_rms_buf_dbl; /**< Pointer to vector of computed RMS for every iteration of LRC_CB_ADC converted to doubles*/
  double f_sample;   /**< sampling frequency of input signal*/
} SimulationResults;

/** @brief driver for our simulation
 * @param[in] p_sim_params - pointer to the simulation parameters.
 * @return shared pointer to simulation results.
 */
std::shared_ptr<SimulationResults> Simulation(const SimulationParams *const p_sim_params);

#endif /* _SIMULTAION_H_*/
