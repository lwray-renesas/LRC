/*
 * hpf.h
 *
 *  Created on: 22 Oct 2025
 *      Author: a5126135
 */

#ifndef HPF_HPF_H_
#define HPF_HPF_H_

#include "stdbool.h"
#include "stdint.h"

/** @brief Trapezoidal integration structure*/
typedef struct Hpf_str
{
  int32_t prev_input;
  int32_t prev_output;
  int64_t state;
  bool run_already;
} Hpf;

/**
 * @brief Performs HPF iteration.
 * @param p_hpf - pointer to hpf struct to work on.
 * @param input - latest input signal.
 * @return output of hpf.
 */
int32_t Hpf_run(Hpf *p_hpf, int32_t input);

/**
 * @brief Resets hpf
 * @param p_hpf - pointer to hpf struct to work on.
 */
void Hpf_reset(Hpf *p_hpf);

#endif /* HPF_HPF_H_ */
