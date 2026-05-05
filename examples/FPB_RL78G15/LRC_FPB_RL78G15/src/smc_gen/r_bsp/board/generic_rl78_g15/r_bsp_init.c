/*
* Copyright (c) 2022 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/***********************************************************************************************************************
* File Name    : r_bsp_init.c
* H/W Platform : GENERIC_RL78_G15
* Description  : 
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 31.05.2022 1.30     First Release
*         : 06.08.2025 1.92     Changed the disclaimer.
*                               Changed to set only valid bits of BSP_CFG_PIORxx.
***********************************************************************************************************************/
/*************************************************
 * Includes  <System Includes> , "Project Includes"
 *************************************************/
#include "platform.h"

/*************************************************
 * Macro definitions
 *************************************************/

/*************************************************
 * External function Prototypes
 *************************************************/

#if BSP_CFG_USER_WARM_START_CALLBACK_PRE_INITC_ENABLED != 0
/* If user is requesting warm start callback functions then these are the prototypes. */
void BSP_CFG_USER_WARM_START_PRE_C_FUNCTION (void);
#endif

#if BSP_CFG_USER_WARM_START_CALLBACK_POST_INITC_ENABLED != 0
/* If user is requesting warm start callback functions then these are the prototypes. */
void BSP_CFG_USER_WARM_START_POST_C_FUNCTION (void);
#endif

/* This prototype is used to suppress the warning message of LLVM compiler. */
void bsp_init_system (void);

/* This prototype is used to suppress the warning message of LLVM compiler. */
void bsp_init_hardware (void);

/*************************************************
 * Private global variables and functions
 *************************************************/

/*************************************************
 * Function definition
 *************************************************/
/*************************************************
 * Function name: bsp_init_system
 * Description  : BSP initial setting(clock)
 * Arguments    : none
 * Return value : none
**************************************************/
void bsp_init_system(void)
{
    /* Pin setting */
#if BSP_CFG_MCU_PART_PIN_NUM < 1
    /* 8-pin */
    PIOR0 = BSP_CFG_PIOR00 | (BSP_CFG_PIOR02 << 2U);

    /* The 8-pin products do not have the PIOR1, PIOR2 register. */

    PIOR3 = (BSP_CFG_PIOR32 << 2U);
#elif BSP_CFG_MCU_PART_PIN_NUM < 2
    /* 10-pin */
    PIOR0 = BSP_CFG_PIOR00 | (BSP_CFG_PIOR02 << 2U) | (BSP_CFG_PIOR03 << 3U);

    /* The 10-pin products do not have the PIOR1, PIOR2 register. */

    PIOR3 = BSP_CFG_PIOR30 | (BSP_CFG_PIOR32 << 2U);
#elif BSP_CFG_MCU_PART_PIN_NUM < 3
    /* 16-pin */
    PIOR0 = BSP_CFG_PIOR00 | (BSP_CFG_PIOR02 << 2U) | (BSP_CFG_PIOR03 << 3U) |
            (BSP_CFG_PIOR04 << 4U) | (BSP_CFG_PIOR05 << 5U) | (BSP_CFG_PIOR06 << 6U);

    PIOR1 = BSP_CFG_PIOR10 | (BSP_CFG_PIOR11 << 1U) | (BSP_CFG_PIOR12 << 2U) |
            (BSP_CFG_PIOR14 << 4U);

    PIOR2 = (BSP_CFG_PIOR21 << 1U) | (BSP_CFG_PIOR22 << 2U) | (BSP_CFG_PIOR23 << 3U) |
            (BSP_CFG_PIOR24 << 4U) | (BSP_CFG_PIOR26 << 6U);

    PIOR3 = BSP_CFG_PIOR30 | (BSP_CFG_PIOR31 << 1U) | (BSP_CFG_PIOR32 << 2U) |
            (BSP_CFG_PIOR34 << 4U);
#else
    /* 20-pin */
    PIOR0 = BSP_CFG_PIOR00 | (BSP_CFG_PIOR01 << 1U) | (BSP_CFG_PIOR02 << 2U) |
            (BSP_CFG_PIOR03 << 3U) | (BSP_CFG_PIOR04 << 4U) | (BSP_CFG_PIOR05 << 5U) |
            (BSP_CFG_PIOR06 << 6U) | (BSP_CFG_PIOR07 << 7U);

    PIOR1 = BSP_CFG_PIOR10 | (BSP_CFG_PIOR11 << 1U) | (BSP_CFG_PIOR12 << 2U) |
            (BSP_CFG_PIOR13 << 3U) | (BSP_CFG_PIOR14 << 4U);

    PIOR2 = BSP_CFG_PIOR20 | (BSP_CFG_PIOR21 << 1U) | (BSP_CFG_PIOR22 << 2U) |
            (BSP_CFG_PIOR23 << 3U) | (BSP_CFG_PIOR24 << 4U) | (BSP_CFG_PIOR25 << 5U) |
            (BSP_CFG_PIOR26 << 6U) | (BSP_CFG_PIOR27 << 7U);

    PIOR3 = BSP_CFG_PIOR30 | (BSP_CFG_PIOR31 << 1U) | (BSP_CFG_PIOR32 << 2U) |
            (BSP_CFG_PIOR33 << 3U) | (BSP_CFG_PIOR34 << 4U) | (BSP_CFG_PIOR35 << 5U);
#endif

    /* Clock initialization */
#if BSP_CFG_STARTUP_DISABLE == 0
    mcu_clock_setup();
#endif

    /* If the warm start Pre C runtime callback is enabled, then call it. */
#if BSP_CFG_USER_WARM_START_CALLBACK_PRE_INITC_ENABLED == 1
    BSP_CFG_USER_WARM_START_PRE_C_FUNCTION();
#endif
} /* End of function bsp_init_system() */

/*************************************************
 * Function name: bsp_init_hardware
 * Description  : BSP initial setting(Peripheral functions)
 * Arguments    : none
 * Return value : none
**************************************************/
void bsp_init_hardware(void)
{
    /* If the warm start Post C runtime callback is enabled, the call it. */
#if BSP_CFG_USER_WARM_START_CALLBACK_POST_INITC_ENABLED == 1
    BSP_CFG_USER_WARM_START_POST_C_FUNCTION();
#endif

    /* Peripheral function initialization */
    hdwinit();
} /* End of function bsp_init_hardware() */
