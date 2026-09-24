#include <hpf.h>
#include "LRC_Core.h"
#include "Menu.h"
#include "csvbin.h"
#include "Storage.h"
#include "hal_data.h"
#include "stdio.h"
#include "Trap_integrator.h"
#include "hpf.h"

#define COEFF_ID (0U)

/* LRC configuration*/
static LRC_Config lrc_cfg = {
                             .ac_trip =
                             {
                              .persistence = 1,
                              .threshold = LRC_FLOAT_TO_FXP(0.029f),
                             },
                             .dc_trip =
                             {
                              .persistence = 1,
                              .threshold = LRC_FLOAT_TO_FXP(0.029f),
                             },
};

static LRC_Channel lrc_channel;

static bool csvbin_switch = false;

/** @brief Perform channel coefficient computation*/
static void Coeff_compute(char *p_args);

/** @brief Prints the rms in both hex and float*/
static void Rms_print(char *p_args);

/** @brief Calls NVIC_SystemReset()*/
static void Reset(char *p_args);

/** @brief Switches to csvbin application*/
static void Switch_to_csvbin(char *p_args);

/* Menuing*/
Menu main_menu = {.p_name = "Main Menu"};

Menu_option coeff_option = {.p_cmd = "coeff",
                            .p_help = "Computes channel coefficient (similar to calibration)\r\n"
                                    "\t\t\t Arguments (space seperated): iref\r\n"
                                    "\t\t\t - iref = Value of the reference current being applied.\r\n",
                                    .option_type = ACTION,
                                    .option.action = &Coeff_compute};

Menu_option rms_option = {
                          .p_cmd = "rms", .p_help = "Prints RMS in both hex and as float", .option_type = ACTION, .option.action = &Rms_print};

Menu_option reset_option = {
                            .p_cmd = "reset", .p_help = "Calls NVIC_SystemReset()", .option_type = ACTION, .option.action = &Reset};

Menu_option csvbin_option = {
                             .p_cmd = "csvbin", .p_help = "Switches to csvbin application", .option_type = ACTION, .option.action = &Switch_to_csvbin};

const fxp_t *veeprom_coeff = 0;
static volatile fxp_t default_coeff = 0x1F61A904; /* Based on input signal +-250mV around 740mV offset = 30mA*/

static bool run_menu_app = true;

#define UART_BUF_SIZE (128)
uint8_t raw_buf[UART_BUF_SIZE];
csvbin_t csvbuf;
static bool adc_ready;
extern bool transmit_meta;
extern void Menu_uart_callback(uart_callback_args_t *p_args);
extern void Csvbin_uart_callback(uart_callback_args_t *p_args);
uart_callback_args_t uart_cb_args;
int32_t raw_adc;
int32_t hpf_adc;
int32_t int_adc;
Hpf l_hpf;

void hal_entry(void)
{
    fsp_err_t err = FSP_SUCCESS;

    /* Setup menu*/
    Menu_init(&main_menu);
    Menu_register_option(&main_menu, &coeff_option);
    Menu_register_option(&main_menu, &rms_option);
    Menu_register_option(&main_menu, &reset_option);
    Menu_register_option(&main_menu, &csvbin_option);

    /* Reset the integrator*/
    Trap_reset(&rogowski_integrator);

    Hpf_reset(&l_hpf);

    /* Default to menu uart*/
    R_UARTA_CallbackSet(&g_uart0_ctrl, Menu_uart_callback, NULL, &uart_cb_args);

    /* Setup CSVBIN*/
    Csvbin_init(&csvbuf, raw_buf, UART_BUF_SIZE);
    Csvbin_set_meta(&csvbuf, "#NADC,HPF,RMS\n", "#Ti32,i32,q17.15\n", "#Elittle\n");

    /* Clear Screen & Home cursor*/
    Menu_printf("\033[2J");
    Menu_printf("\033[H");
    Menu_printf("Initialising LRC...\r\n");

    /* Setup LRC*/
    LRC_Init(&lrc_cfg);
    LRC_ChannelRegister(&lrc_channel);

    /* Prepare*/
    Storage_startup();

    /* Load Stored Calibration if one exists*/
    if (Storage_read(COEFF_ID, (uint8_t **)&veeprom_coeff))
    {
        Menu_printf("\t\tCoefficient Found in VEEPROM!\r\n");
        Menu_printf("\t\tCoefficient: 0x%08X\r\n", *veeprom_coeff);
        LRC_ChannelCoeffSet(&lrc_channel, *veeprom_coeff);
    }
    else
    {
        Menu_printf("\t\tCoefficient NOT Found in VEEPROM - using default!\r\n");
        Menu_printf("\t\tCoefficient: 0x%08X\r\n", default_coeff);
        LRC_ChannelCoeffSet(&lrc_channel, default_coeff);
    }

    /* Clearup*/
    Storage_shutdown();

    /* Enable switch*/
    err = R_ICU_ExternalIrqOpen(&g_external_irq0_ctrl, &g_external_irq0_cfg);
    if(FSP_SUCCESS != err)
    {
        __BKPT(0);
    }

    err = R_ICU_ExternalIrqEnable(&g_external_irq0_ctrl);
    if(FSP_SUCCESS != err)
    {
        __BKPT(0);
    }

    Menu_printf("Done!\r\n");

    Menu_printf("Starting LRC...");

    LRC_Start();

    Menu_printf("Done!\r\n");

    Menu_print_help();

    while (1)
    {
        if(run_menu_app)
        {
#if 0
            LRC_CRITICAL_SECTION_PREPARE();
            fxp_t l_rms;
            LRC_CRITICAL_SECTION_ENTER();
            l_rms = lrc_channel.ac_data.output;
            LRC_CRITICAL_SECTION_EXIT();
            Menu_printf("\r\nRMS (float): %.3f [A]", LRC_FXP_TO_FLOAT(l_rms));
#else
            Menu_handler();
#endif
            if(csvbin_switch)
            {
                run_menu_app = false;
                R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_00_PIN_08, BSP_IO_LEVEL_HIGH);
                R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_00_PIN_09, BSP_IO_LEVEL_HIGH);
                R_UARTA_CallbackSet(&g_uart0_ctrl, Csvbin_uart_callback, NULL, &uart_cb_args);
            }
        }
        else
        {
            if(transmit_meta)
            {
                transmit_meta = false;
                Csvbin_transmit_meta(&csvbuf);
            }

            if(adc_ready)
            {
                adc_ready = false;

                __disable_irq();
                int32_t adc = raw_adc;
                int32_t hpf = hpf_adc;
                fxp_t rms = lrc_channel.ac_data.output;
                __enable_irq();

                if(CSVBIN_SUCCESS == Csvbin_add_field(&csvbuf, (uint8_t*)&adc, sizeof(adc)))
                {
                    if(CSVBIN_SUCCESS == Csvbin_add_field(&csvbuf, (uint8_t*)&hpf, sizeof(hpf)))
                    {
                        if(CSVBIN_SUCCESS == Csvbin_add_field(&csvbuf, (uint8_t*)&rms, sizeof(rms)))
                        {
                            Csvbin_end_row(&csvbuf);
                        }
                    }
                }
                Csvbin_transmit(&csvbuf);
            }
        }
    }
}

/* ADC callback function */
void adc_callback(adc_callback_args_t *p_args)
{
    (void)p_args;
    raw_adc = (int32_t)(R_ADC_D->ADCR[0] - 2048);
    hpf_adc = Hpf_run(&l_hpf, raw_adc);
    int_adc = Trap_integrate(&rogowski_integrator, hpf_adc);

#if 1
    lrc_channel.inputs.i_sample = (spl_t)hpf_adc;
#else
    lrc_channel.inputs.i_sample = (spl_t)int_adc;
#endif

    /* Enter LRC state machine*/
    LRC_CB_ADC();

    /* Signal to the app the ADC is ready*/
    adc_ready = true;
}

static void Coeff_compute(char *p_args)
{
    float l_iref = 0.0f;

    sscanf(p_args, "%f", &l_iref);

    if (l_iref >= 0.0f)
    {
        fxp_t l_coeff = LRC_ChannelCoeffCompute(&lrc_channel, l_iref);

        Menu_printf("\r\nReference Current: %.3f [A]", l_iref);
        Menu_printf("\r\nCoefficient computed: 0x%08X", l_coeff);

        Menu_printf("\r\nStoring coefficient in VEEPROM...");
        /* Prepare*/
        Storage_startup();

        Storage_write(COEFF_ID, (uint8_t *)&l_coeff, sizeof(fxp_t));

        /* Clearup*/
        Storage_shutdown();
        Menu_printf("Done!");

        Menu_printf("\r\nStopping LRC, updating coefficient & restarting...");
        LRC_Stop();
        LRC_ChannelCoeffSet(&lrc_channel, l_coeff);
        LRC_Start();
        Menu_printf("Done!");
    }
    else
    {
        Menu_printf("\r\nIncorrect argument, iref should be >= 0.0: %.3f", l_iref);
    }
}

static void Rms_print(char *p_args)
{
    (void)p_args;

    fxp_t l_rms = lrc_channel.ac_data.output;
    fxp_t l_dc = lrc_channel.dc_data.output;

    Menu_printf("\r\nRMS (float): %.3f [A]", LRC_FXP_TO_FLOAT(l_rms));
    Menu_printf("\r\nRMS (fixed point): 0x%08X [A]", l_rms);

    Menu_printf("\r\nDC (float): %.3f [A]", LRC_FXP_TO_FLOAT(l_dc));
    Menu_printf("\r\nDC (fixed point): 0x%08X [A]", l_dc);
}

static void Reset(char *p_args)
{
    (void)p_args;

    NVIC_SystemReset();
}

static void Switch_to_csvbin(char *p_args)
{
    (void)p_args;
    csvbin_switch = true;
}

/* Callback function */
void Switch_pressed_callback(external_irq_callback_args_t *p_args)
{
    (void)p_args;
    /* Switch between app states*/
    if(run_menu_app)
    {
        run_menu_app = false;
        R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_00_PIN_08, BSP_IO_LEVEL_HIGH);
        R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_00_PIN_09, BSP_IO_LEVEL_HIGH);
        Menu_cancel_transmit();
        R_UARTA_CallbackSet(&g_uart0_ctrl, Csvbin_uart_callback, NULL, &uart_cb_args);
    }
    else
    {
        run_menu_app = true;
        R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_00_PIN_08, BSP_IO_LEVEL_LOW);
        R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_00_PIN_09, BSP_IO_LEVEL_LOW);
        Csvbin_cancel_transmit();
        R_UARTA_CallbackSet(&g_uart0_ctrl, Menu_uart_callback, NULL, &uart_cb_args);
    }
}

