/*
 * Trap_integrator.c
 *
 *  Created on: 22 Oct 2025
 *      Author: a5126135
 */

#include "Trap_integrator.h"

Trap_integrator rogowski_integrator = {0, 0, false};

#define HPF_A_Q16 65023      // round(0.99218 * 65536)

int32_t Trap_integrate(Trap_integrator *p_integrator, int32_t input)
{
    int32_t integrated = 0;

    if (!p_integrator->run_already)
    {
        p_integrator->run_already = true;

        p_integrator->prev_input      = input;
        p_integrator->prev_output     = 0;
    }
    else
    {
        integrated = (p_integrator->prev_output + ((input + p_integrator->prev_input) / 2));

        p_integrator->prev_input = input;
        p_integrator->prev_output = integrated;
    }

    return integrated;
}

void Trap_reset(Trap_integrator *p_integrator)
{
    p_integrator->prev_input = 0;
    p_integrator->prev_output = 0;
    p_integrator->run_already = false;
}
