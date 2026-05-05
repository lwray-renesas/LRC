#include "hal_data.h"
#include "LRC_Core.h"
#include "Menu.h"
#include "Storage.h"
#include "stdio.h"

#define COEFF_ID (0U)

/* LRC configuration*/
static LRC_Config lrc_cfg = {
                             .trip = {
                                      .persistence = 1,
                                      .threshold = LRC_FLOAT_TO_FXP(0.029f),
                             },
};

static LRC_Channel lrc_channel;

/** @brief Perform channel coefficient computation*/
static void Coeff_compute(char *p_args);

/** @brief Prints the rms in both hex and float*/
static void Rms_print(char *p_args);

/** @brief Calls NVIC_SystemReset()*/
static void Reset(char *p_args);

/* Menuing*/
Menu main_menu = {.p_name = "Main Menu"};

Menu_option coeff_option = { .p_cmd = "coeff",
                             .p_help = "Computes channel coefficient (similar to calibration)\r\n"
                                     "\t\t\t Arguments (space seperated): iref\r\n"
                                     "\t\t\t - iref = Value of the reference current being applied.\r\n",
                                     .option_type = ACTION, .
                                     option.action = &Coeff_compute
};

Menu_option rms_option = { .p_cmd = "rms",
                             .p_help = "Prints RMS in both hex and as float",
                                     .option_type = ACTION, .
                                     option.action = &Rms_print
};

Menu_option reset_option = { .p_cmd = "reset",
                             .p_help = "Calls NVIC_SystemReset()",
                                     .option_type = ACTION, .
                                     option.action = &Reset
};

const fxp_t *veeprom_coeff = 0;
static volatile fxp_t default_coeff = 0x1F61A904; /* Based on input signal +-250mV around 740mV offset = 30mA*/

void hal_entry(void)
{
    /* Setup menu*/
    Menu_init(&main_menu);
    Menu_register_option(&main_menu, &coeff_option);
    Menu_register_option(&main_menu, &rms_option);
    Menu_register_option(&main_menu, &reset_option);

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

    Menu_printf("Done!\r\n");

    Menu_printf("Starting LRC...");

    LRC_Start();

    Menu_printf("Done!\r\n");

    Menu_print_help();

    while(1)
    {
        Menu_handler();
    }
}

/* ADC callback function */
void adc_callback(adc_callback_args_t *p_args)
{
    (void)p_args;

    /* Using internal reference voltage, means bias is 0.74V (VBGR = 1.4V full swing)*/
    lrc_channel.inputs.i_sample = (spl_t) R_ADC_D->ADCR[0] - 2048;

    /* Enter LRC state machine*/
    LRC_CB_ADC();
}

static void Coeff_compute(char *p_args)
{
    float l_iref = 0.0f;

    sscanf(p_args, "%f", &l_iref);

    if(l_iref >= 0.0f)
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

    fxp_t l_rms = lrc_channel.rms;

    Menu_printf("\r\nRMS (float): %.3f [A]", LRC_FXP_TO_FLOAT(l_rms));
    Menu_printf("\r\nRMS (fixed point): 0x%08X [A]", l_rms);
}

static void Reset(char *p_args)
{
    (void)p_args;

    NVIC_SystemReset();
}
