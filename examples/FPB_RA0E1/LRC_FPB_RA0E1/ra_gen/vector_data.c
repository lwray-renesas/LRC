/* generated vector source file - do not edit */
#include "bsp_api.h"
/* Do not build these data structures if no interrupts are currently allocated because IAR will have build errors. */
#if VECTOR_DATA_IRQ_COUNT > 0
        BSP_DONT_REMOVE const fsp_vector_t g_vector_table[BSP_ICU_VECTOR_NUM_ENTRIES] BSP_PLACE_IN_SECTION(BSP_SECTION_APPLICATION_VECTORS) =
        {
                        [2] = r_icu_isr, /* ICU IRQ0 (External pin interrupt 0) */
            [11] = fcu_frdyi_isr, /* FCU FRDYI (Flash ready interrupt) */
            [31] = adc_d_scan_end_isr, /* ADC0 SCAN END (End of A/D scanning operation) */
            [40] = uarta_txi_isr, /* UARTA0 TXI (UARTA0 transmission transfer end or buffer empty interrupt) */
            [41] = uarta_rxi_isr, /* UARTA0 RXI (UARTA0 reception transfer end) */
        };
        #if BSP_FEATURE_ICU_HAS_IELSR
        const bsp_interrupt_event_t g_interrupt_event_link_select[BSP_ICU_VECTOR_NUM_ENTRIES] =
        {
            [2] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ0,GROUP2), /* ICU IRQ0 (External pin interrupt 0) */
            [11] = BSP_PRV_VECT_ENUM(EVENT_FCU_FRDYI,GROUP3), /* FCU FRDYI (Flash ready interrupt) */
            [31] = BSP_PRV_VECT_ENUM(EVENT_ADC0_SCAN_END,GROUP7), /* ADC0 SCAN END (End of A/D scanning operation) */
            [40] = BSP_PRV_VECT_ENUM(EVENT_UARTA0_TXI,FIXED), /* UARTA0 TXI (UARTA0 transmission transfer end or buffer empty interrupt) */
            [41] = BSP_PRV_VECT_ENUM(EVENT_UARTA0_RXI,FIXED), /* UARTA0 RXI (UARTA0 reception transfer end) */
        };
        #endif
        #endif
