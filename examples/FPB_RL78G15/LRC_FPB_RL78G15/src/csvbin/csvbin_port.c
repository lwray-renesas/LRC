/**
 * @file csvbin_port.c
 */

#include "r_smc_entry.h"

/**
 * @brief Blocking write to output of buffer contents.
 * @param data - data to transmit.
 * @param len - length of data to transmit.
 */
void Csvbin_writebuffer_port(uint8_t *data, uint16_t len);

void Csvbin_writebuffer_port(uint8_t *data, uint16_t len)
{
	R_Config_UART0_Send_Blocking(data, len);
}
