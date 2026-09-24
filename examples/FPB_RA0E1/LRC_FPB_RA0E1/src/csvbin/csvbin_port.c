/**
 * @file csvbin_port.c
 */

#include "hal_data.h"

static bool uart_write_complete = true;
bool transmit_meta = false;

/**
 * @brief Blocking write to output of buffer contents.
 * @param data - data to transmit.
 * @param len - length of data to transmit.
 */
void Csvbin_writebuffer_port(uint8_t *data, uint16_t len);

/** @brief Cancels the wait for transmission complete*/
void Csvbin_cancel_transmit_port(void);

void Csvbin_writebuffer_port(uint8_t *data, uint16_t len)
{
    uart_write_complete = false;

    fsp_err_t err = R_UARTA_Write(&g_uart0_ctrl, data, len);
    if(FSP_SUCCESS != err)
    {
        __BKPT(0);
    }

    while(!uart_write_complete)
    {
        __NOP();
    }
}

void Csvbin_cancel_transmit_port(void)
{
    R_UARTA_Close(&g_uart0_ctrl);
    R_UARTA_Open(&g_uart0_ctrl, &g_uart0_cfg);
    uart_write_complete = true;
}

void Csvbin_uart_callback(uart_callback_args_t *p_args)
{
  if(UART_EVENT_RX_CHAR == p_args->event)
  {
      if('M' == p_args->data)
      {
          transmit_meta = true; /* Requested to transmit meta data*/
      }
  }
  else if(UART_EVENT_TX_COMPLETE == p_args->event)
  {
      uart_write_complete = true;
  }
  else
  {
      /* Do nothing*/
      __NOP();
  }
}
