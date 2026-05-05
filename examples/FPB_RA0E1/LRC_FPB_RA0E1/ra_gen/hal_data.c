/* generated HAL source file - do not edit */
#include "hal_data.h"
flash_lp_instance_ctrl_t g_flash0_ctrl;
const flash_cfg_t g_flash0_cfg =
{ .data_flash_bgo = true, .p_callback = rm_vee_flash_callback, .p_context = &g_vee0_ctrl, .ipl = (3),
#if defined(VECTOR_NUMBER_FCU_FRDYI)
    .irq                 = VECTOR_NUMBER_FCU_FRDYI,
#else
  .irq = FSP_INVALID_VECTOR,
#endif
        };
/* Instance structure to use this module. */
const flash_instance_t g_flash0 =
{ .p_ctrl = &g_flash0_ctrl, .p_cfg = &g_flash0_cfg, .p_api = &g_flash_on_flash_lp };
rm_vee_flash_instance_ctrl_t g_vee0_ctrl;

const rm_vee_flash_cfg_t g_vee0_cfg_ext =
{ .p_flash = &g_flash0 };

static uint16_t g_vee0_record_offset[16 + 1] =
{ 0 };

const rm_vee_cfg_t g_vee0_cfg =
{ .start_addr = BSP_FEATURE_FLASH_DATA_FLASH_START,
  .num_segments = 2,
  .total_size = BSP_DATA_FLASH_SIZE_BYTES,
  .ref_data_size = 0,
  .record_max_id = 16,
  .rec_offset = &g_vee0_record_offset[0],
  .p_callback = VEE_Callback,
  .p_context = NULL,
  .p_extend = &g_vee0_cfg_ext };

/* Instance structure to use this module. */
const rm_vee_instance_t g_vee0 =
{ .p_ctrl = &g_vee0_ctrl, .p_cfg = &g_vee0_cfg, .p_api = &g_rm_vee_on_flash };
uarta_instance_ctrl_t g_uart0_ctrl;

uarta_baud_setting_t g_uart0_baud_setting =
{
#if (BSP_CFG_UARTA0_CLOCK_SOURCE == BSP_CLOCKS_SOURCE_CLOCK_MAIN_OSC)

  /* Baud rate calculated with Actual_Error0.16%. */
  /* The permissible baud rate error range during reception: -5.19% ~ 5.18% */
  .utanck_clock_b.utasel = UARTA_CLOCK_SOURCE_MOSC,
  .utanck_clock_b.utanck = UARTA_CLOCK_DIV_8, .brgca = 130, .delay_time = 1
#elif (BSP_CFG_UARTA0_CLOCK_SOURCE == BSP_CLOCKS_SOURCE_CLOCK_HOCO)

/* Baud rate calculated with Actual_Error0.16%. */
/* The permissible baud rate error range during reception: -5.22% ~ 5.21% */
  .utanck_clock_b.utasel = UARTA_CLOCK_SOURCE_HOCO
, .utanck_clock_b.utanck = UARTA_CLOCK_DIV_8
, .brgca = 208
, .delay_time = 1
 #elif (BSP_CFG_UARTA0_CLOCK_SOURCE == BSP_CLOCKS_SOURCE_CLOCK_MOCO)

/* Baud rate calculated with Actual_Error0.16%. */ 
/* The permissible baud rate error range during reception: -5.22% ~ 5.21% */
  .utanck_clock_b.utasel = UARTA_CLOCK_SOURCE_MOCO
, .utanck_clock_b.utanck = UARTA_CLOCK_DIV_1
, .brgca = 208
, .delay_time = 1
 #elif ((BSP_CFG_UARTA0_CLOCK_SOURCE == BSP_CFG_FSXP_SOURCE) || (BSP_CFG_UARTA0_CLOCK_SOURCE == BSP_CLOCKS_SOURCE_CLOCK_LOCO))

/* Baud rate calculated with Actual_Error14.67%. */
/* The permissible baud rate error range during reception: -2.50% ~ 2.50% */
  .utanck_clock_b.utasel = 0
, .utanck_clock_b.utanck = UARTA_UTAnCK_LOCO_SETTING
, .brgca = 2
, .delay_time = 31
 #elif (BSP_CFG_UARTA0_CLOCK_SOURCE == BSP_CLOCKS_SOURCE_CLOCK_SUBCLOCK)

/* Baud rate calculated with Actual_Error14.67%. */
/* The permissible baud rate error range during reception: -2.50% ~ 2.50% */
  .utanck_clock_b.utasel = 0
, .utanck_clock_b.utanck = UARTA_UTAnCK_SOSC_SETTING
, .brgca = 2
, .delay_time = 31
 #endif
        };

/** UART extended configuration for UART on UARTA HAL driver */
const uarta_extended_cfg_t g_uart0_cfg_extend =
{ .transfer_dir = UARTA_DIR_BIT_LSB_FIRST, .transfer_level = UARTA_ALV_BIT_POSITIVE_LOGIC, .clock_output =
          UARTA_CLOCK_OUTPUT_DISABLED,
  .p_baud_setting = &g_uart0_baud_setting, };

/** UART interface configuration */
const uart_cfg_t g_uart0_cfg =
{ .channel = 0, .data_bits = UART_DATA_BITS_8, .parity = UART_PARITY_OFF, .stop_bits = UART_STOP_BITS_1, .p_callback =
          g_uart0_callback,
  .p_context = NULL, .p_extend = &g_uart0_cfg_extend,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
  .p_transfer_tx = NULL,
#else
                .p_transfer_tx       = &RA_NOT_DEFINED,
#endif
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
  .p_transfer_rx = NULL,
#else
                .p_transfer_rx       = &RA_NOT_DEFINED,
#endif
#undef RA_NOT_DEFINED
  .rxi_ipl = (2),
  .txi_ipl = (2), .eri_ipl = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_UARTA0_RXI)
                .rxi_irq             = VECTOR_NUMBER_UARTA0_RXI,
#else
  .rxi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_UARTA0_TXI)
                .txi_irq             = VECTOR_NUMBER_UARTA0_TXI,
#else
  .txi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_UARTA0_ERRI)
                .eri_irq             = VECTOR_NUMBER_UARTA0_ERRI,
#else
  .eri_irq = FSP_INVALID_VECTOR,
#endif
        };

/* Instance structure to use this module. */
const uart_instance_t g_uart0 =
{ .p_ctrl = &g_uart0_ctrl, .p_cfg = &g_uart0_cfg, .p_api = &g_uart_on_uarta };
tau_instance_ctrl_t g_timer0_ctrl;
const tau_extended_cfg_t g_timer0_extend =
{ .opirq = TAU_INTERRUPT_OPIRQ_BIT_RESET,
  .tau_func = TAU_FUNCTION_INTERVAL,
  .bit_mode = TAU_BIT_MODE_16BIT,
  .initial_output = TAU_PIN_OUTPUT_CFG_DISABLED,
  .input_source = TAU_INPUT_SOURCE_NONE,
  .tau_filter = TAU_INPUT_NOISE_FILTER_DISABLE,
  .trigger_edge = TAU_TRIGGER_EDGE_RISING,
  .operation_clock = TAU_OPERATION_CK00,
  /* Not used for 16-bit or lower 8-bit mode */
  .period_higher_8bit_counts = (uint16_t) 0x100,
  .higher_8bit_cycle_end_ipl = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_TAU0_TMI01H)
    .higher_8bit_cycle_end_irq       = VECTOR_NUMBER_TAU0_TMI01H,
#else
  .higher_8bit_cycle_end_irq = FSP_INVALID_VECTOR,
#endif
        };
const timer_cfg_t g_timer0_cfg =
{ .mode = (timer_mode_t) 0,
/* Actual Period: 0.0004000000 seconds. */
/* Minimum Period ~ Maximum Period: 0.0000000625 ~ 0.00204800 seconds. */.period_counts = (uint32_t) 0x3200,
  .duty_cycle_counts = 0, .source_div = (timer_source_div_t) BSP_CFG_TAU_CK00, .channel = 1, .p_callback = NULL,
  /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
  .p_context = (void*) &NULL,
#endif
  .p_extend = &g_timer0_extend,
  .cycle_end_ipl = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_TAU0_TMI01)
    .cycle_end_irq       = VECTOR_NUMBER_TAU0_TMI01,
#else
  .cycle_end_irq = FSP_INVALID_VECTOR,
#endif
        };
/* Instance structure to use this module. */
const timer_instance_t g_timer0 =
{ .p_ctrl = &g_timer0_ctrl, .p_cfg = &g_timer0_cfg, .p_api = &g_timer_on_tau };
adc_d_instance_ctrl_t g_adc0_ctrl;

/* ADC conversion time 57.5 us, conversion start time: 0.03125us */
const adc_d_extended_cfg_t g_adc0_cfg_extend =
{ .channel_mode = ADC_D_CHANNEL_MODE_SELECT,
  .operation_voltage = ADC_D_VOLTAGE_MODE_LOW_2,
  .conversion_clockdiv = ADC_D_CLOCK_DIV_16,
  .trigger_source = ADC_D_TRIGGER_SOURCE_TAU0_TMI01,
  .operation_trigger = ADC_D_TRIGGER_MODE_WAIT,
  .conversion_operation = ADC_D_CONVERSION_MODE_ONESHOT,
  .upper_lower_bound = ADC_D_BOUNDARY_IN_RANGE,
  .negative_vref = ADC_D_NEGATIVE_VREF_VSS,
  .positive_vref = ADC_D_POSITIVE_VREF_IVREF,
  .upper_bound_limit = 255,
  .lower_bound_limit = 0, };
const adc_cfg_t g_adc0_cfg =
{ .unit = 0, .mode = (adc_mode_t) 0, // Unused
  .resolution = ADC_RESOLUTION_12_BIT,
  .alignment = (adc_alignment_t) 0, // Unused
  .trigger = (adc_trigger_t) 0, // Unused
  .p_callback = adc_callback,
  .p_context = NULL,
  .p_extend = &g_adc0_cfg_extend,
#if defined(VECTOR_NUMBER_ADC0_SCAN_END)
    .scan_end_irq        = VECTOR_NUMBER_ADC0_SCAN_END,
#else
  .scan_end_irq = FSP_INVALID_VECTOR,
#endif
  .scan_end_ipl = (0),
  .scan_end_b_irq = FSP_INVALID_VECTOR,
  .scan_end_b_ipl = BSP_IRQ_DISABLED, };

const adc_d_channel_cfg_t g_adc0_channel_cfg =
{ .channel_input = ADC_CHANNEL_4 };
/* Instance structure to use this module. */
const adc_instance_t g_adc0 =
{ .p_ctrl = &g_adc0_ctrl, .p_cfg = &g_adc0_cfg, .p_channel_cfg = &g_adc0_channel_cfg, .p_api = &g_adc_on_adc_d };
void g_hal_init(void)
{
    g_common_init ();
}
