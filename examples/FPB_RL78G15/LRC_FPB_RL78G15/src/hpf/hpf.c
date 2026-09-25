/*
 * Trap_integrator.c
 *
 *  Created on: 22 Oct 2025
 *      Author: a5126135
 */

#include <hpf.h>

#define HPF_A_Q16 65023      // round(0.99218 * 65536)

int32_t Hpf_run(Hpf *p_hpf, int32_t input)
{
    if (!p_hpf->run_already)
    {
        p_hpf->run_already = true;

        p_hpf->prev_input      = input;
        p_hpf->prev_output     = 0L;
        p_hpf->state = 0LL;
    }
    else
    {
        p_hpf->state = ((int64_t)HPF_A_Q16 *
                (p_hpf->state +
                 (((int64_t)input -
                   (int64_t)p_hpf->prev_input) << 16L)))
            >> 16;
        p_hpf->prev_input = input;
        p_hpf->prev_output = (int32_t)(p_hpf->state >> 16L);
    }

    return p_hpf->prev_output;
}

void Hpf_reset(Hpf *p_hpf)
{
    p_hpf->prev_input = 0L;
    p_hpf->prev_output = 0L;
    p_hpf->state = 0LL;
    p_hpf->run_already = false;
}
