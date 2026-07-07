#include "simulation.hpp"
#include <algorithm>
#include <atomic>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <math.h>
#include <thread>
#include <vector>

static bool was_tripped = false;

/** @brief parameters passed to the driver thread*/
typedef struct DriverParams
{
  std::shared_ptr<std::vector<int32_t>> p_current_samples; /**< Pointer to the current samples */
  std::shared_ptr<std::vector<int32_t>> p_rms_buf;         /**< Pointer to store the rms computations after each ADC callback */
  std::shared_ptr<std::vector<double>>
      p_rms_buf_dbl;     /**< Pointer to store the rms computations after each ADC callback (converted to double)*/
  std::shared_ptr<std::vector<int32_t>> p_dc_buf; /**< Pointer to store the dc data computations after each ADC callback */
  std::shared_ptr<std::vector<double>>
      p_dc_buf_dbl;     /**< Pointer to store the dc data computations after each ADC callback (converted to double)*/
  double f_sample;       /**< sampling frequency*/
  bool single_ended_adc; /**< flag to indicate the ADC should behave as single ended */
  size_t trip_idx;       /**< sample in which the simluation first tripped the RCD */

  std::atomic<bool> stop_driver_thread;    /**< Pointer to the variable for stopping/cancelling the driver thread*/
  std::atomic<bool> driver_thread_running; /**< Pointer to the variable for indicating the driver thread is running*/
  LRC_Channel channel;                     /**< Pointer to the measurement channel to work on*/
} DriverParams;

/**
 * @brief Generates ADC reading for simulation from vector of doubles.
 *
 * @param[in] p_current_samples - reference to shared pointer of vector of doubles containing current waveform to simulate.
 * @param[in] max_current - maximum system current that can be sensed by simulated ADC.
 * @param[in] bit_width - the simulated bit width of the ADC
 * @param[in] single_ended_adc        - true if ADC is unsigned (single-ended), false if unsigned (bipolar)
 *
 * @return shared pointer to a vector of int32_t samples reflecting ADC readings of waveform.
 */
static std::shared_ptr<std::vector<int32_t>> Generate_adc_samples(const std::shared_ptr<std::vector<double>> &p_current_samples,
                                                                  double max_current, uint32_t bit_width, bool single_ended_adc)
{
  auto adc_samples = std::make_shared<std::vector<int32_t>>();

  // Sanity checks
  if (p_current_samples && !p_current_samples->empty() && max_current > 0.0 && bit_width >= 2 && bit_width <= 31)
  {
    adc_samples->reserve(p_current_samples->size());

    if (single_ended_adc)
    {
      // ---- Unsigned (single-ended) ADC ----
      // Range: [0, 2^N - 1]
      const int32_t max_code = (1u << bit_width) - 1;
      const int32_t min_code = 0;

      // Map [-max_current, +max_current] --> [0, max_code]
      const double counts_per_amp = static_cast<double>(max_code) / (2.0 * max_current);
      const double dc_offset = static_cast<double>(max_code) / 2.0;

      for (double current : *p_current_samples)
      {
        double clamped_current = std::clamp(current, -max_current, max_current);
        double raw = (current * counts_per_amp) + dc_offset;
        int32_t adc_code = static_cast<int32_t>(std::lround(raw));
        adc_code = std::clamp(adc_code, min_code, max_code);
        adc_samples->push_back(adc_code);
      }
    }
    else
    {
      // ---- Signed (bipolar) ADC ----
      // Range: [-2^(N-1), +2^(N-1)-1]
      const int32_t max_code = (1u << (bit_width - 1)) - 1;
      const int32_t min_code = -(1u << (bit_width - 1));

      const double counts_per_amp = static_cast<double>(max_code) / max_current;

      for (double current : *p_current_samples)
      {
        double clamped_current = std::clamp(current, -max_current, max_current);
        double raw = clamped_current * counts_per_amp;
        int32_t adc_code = static_cast<int32_t>(std::lround(raw));
        adc_code = std::clamp(adc_code, min_code, max_code);
        adc_samples->push_back(adc_code);
      }
    }
  }

  return adc_samples;
}

/**
 * @brief Fucntions to simulate the asynchronous nature of MCU's and interrupts - effectviely runs the ADC callback
 * Runs on new thread!
 *
 * @param[in] - driver parameters - a way to pass arguments to the thread.
 */
static void Driver_thread(std::shared_ptr<DriverParams> drvr_params)
{
  size_t sample = 0;
  double sleep_time_ms = 0.00;

  const double sample_period = 1.00 / drvr_params->f_sample;

  drvr_params->driver_thread_running = true;

  while (sample < drvr_params->p_current_samples->size() && !(drvr_params->stop_driver_thread))
  {
    if (adc_running)
    {
      // Remove offset manually if we are simulating a single ended ADC
      if (drvr_params->single_ended_adc)
      {
        drvr_params->channel.inputs.i_sample = static_cast<spl_t>((*drvr_params->p_current_samples)[sample] - 2048);
      }
      else
      {
        drvr_params->channel.inputs.i_sample = static_cast<spl_t>((*drvr_params->p_current_samples)[sample]);
      }

      // TODO: Test a HPF here

      LRC_CB_ADC();

      if ((!was_tripped) && (tripped))
      {
        was_tripped = true;
        drvr_params->trip_idx = sample;
      }

      drvr_params->p_rms_buf->push_back(drvr_params->channel.ac_data.output);
      drvr_params->p_rms_buf_dbl->push_back(drvr_params->channel.ac_data.output / ((double)(1 << FXP_FRAC_BITS)));

      drvr_params->p_dc_buf->push_back(drvr_params->channel.dc_data.output);
      drvr_params->p_dc_buf_dbl->push_back(drvr_params->channel.dc_data.output / ((double)(1 << FXP_FRAC_BITS)));

      std::cout << std::flush << "\rRMS: " << std::fixed << std::setprecision(6)
                << LRC_FXP_TO_FLOAT(drvr_params->channel.ac_data.output)
                << std::flush;

      ++sample;
    }

    sleep_time_ms += (sample_period * 1000.00);

    if (sleep_time_ms > 20.0)
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<long long>(sleep_time_ms)));
      sleep_time_ms = 0.00;
    }
  }

  drvr_params->driver_thread_running = false;
}

std::shared_ptr<SimulationResults> Simulation(const SimulationParams *sim_params)
{
  auto results = std::make_shared<SimulationResults>();
  auto drv_params = std::make_shared<DriverParams>();

  drv_params->trip_idx = 0;
  drv_params->p_rms_buf = std::make_shared<std::vector<int32_t>>();
  drv_params->p_rms_buf_dbl = std::make_shared<std::vector<double>>();
  drv_params->p_dc_buf = std::make_shared<std::vector<int32_t>>();
  drv_params->p_dc_buf_dbl = std::make_shared<std::vector<double>>();
  drv_params->f_sample = sim_params->f_sample;
  drv_params->single_ended_adc = sim_params->single_ended_adc;

  // Convert to normalised 12bit ADC readings
  drv_params->p_current_samples = Generate_adc_samples(sim_params->p_current_samples, sim_params->max_current,
                                                       sim_params->bit_width, sim_params->single_ended_adc);

  if (sim_params->rogowski)
  {
    // TODO: Handle rogowski processing where necessary
  }

  tripped = false;
  was_tripped = false;

  // Config
  auto p_config = std::make_unique<LRC_Config>();
  p_config->ac_trip.threshold = (fxp_t)(0.029 * ((double)(1 << FXP_FRAC_BITS)));
  p_config->ac_trip.persistence = 1;
  p_config->dc_trip.threshold = (fxp_t)(0.029 * ((double)(1 << FXP_FRAC_BITS)));
  p_config->dc_trip.persistence = 1;

  LRC_Init(p_config.get());

  LRC_ChannelRegister(&(drv_params->channel));

  // We acquire this with a manual verification
  // Feeding in a known 30mA signal, we then check what rms is computed with a full accumulator buffer.
  // We then perform rms/(0.03 * 32768) = calibrated coeefficient.
  LRC_ChannelCoeffSet(&(drv_params->channel), 0x13CADf72);

  LRC_Start();

  drv_params->driver_thread_running = false;
  drv_params->stop_driver_thread = false;
  std::thread driver_thread = std::thread(Driver_thread, drv_params);

  // Wait until the driver thread is running
  while (!drv_params->driver_thread_running)
  {
  }

  std::cout << "Live Measurement Output..." << std::endl << std::flush;

  int str_len = 0;

  while (drv_params->driver_thread_running)
  {
    // Update simulation state from calling thread in case we want to cancel
    drv_params->stop_driver_thread.store(sim_params->stop_simulation.load());

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }

  if (driver_thread.joinable())
  {
    driver_thread.join();
  }

  LRC_Stop();

  LRC_Deinit();

  std::cout << "\nSimulation Complete!\n";

  results->p_adc_samples = drv_params->p_current_samples;
  results->p_current_samples = sim_params->p_current_samples;
  results->p_rms_buf = drv_params->p_rms_buf;
  results->p_rms_buf_dbl = drv_params->p_rms_buf_dbl;
  results->p_dc_buf = drv_params->p_dc_buf;
  results->p_dc_buf_dbl = drv_params->p_dc_buf_dbl;
  results->trip_idx = drv_params->trip_idx;
  results->f_sample = drv_params->f_sample;

  return results;
}
