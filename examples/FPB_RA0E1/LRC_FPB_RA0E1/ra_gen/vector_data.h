/* generated vector header file - do not edit */
#ifndef VECTOR_DATA_H
#define VECTOR_DATA_H
#ifdef __cplusplus
        extern "C" {
        #endif
/* Number of interrupts allocated */
#ifndef VECTOR_DATA_IRQ_COUNT
#define VECTOR_DATA_IRQ_COUNT    (4)
#endif
/* ISR prototypes */
void fcu_frdyi_isr(void);
void adc_d_scan_end_isr(void);
void uarta_txi_isr(void);
void uarta_rxi_isr(void);

/* Vector table allocations */
#define VECTOR_NUMBER_FCU_FRDYI ((IRQn_Type) 11) /* FCU FRDYI (Flash ready interrupt) */
#define FCU_FRDYI_IRQn          ((IRQn_Type) 11) /* FCU FRDYI (Flash ready interrupt) */
#define VECTOR_NUMBER_ADC0_SCAN_END ((IRQn_Type) 31) /* ADC0 SCAN END (End of A/D scanning operation) */
#define ADC0_SCAN_END_IRQn          ((IRQn_Type) 31) /* ADC0 SCAN END (End of A/D scanning operation) */
#define VECTOR_NUMBER_UARTA0_TXI ((IRQn_Type) 40) /* UARTA0 TXI (UARTA0 transmission transfer end or buffer empty interrupt) */
#define UARTA0_TXI_IRQn          ((IRQn_Type) 40) /* UARTA0 TXI (UARTA0 transmission transfer end or buffer empty interrupt) */
#define VECTOR_NUMBER_UARTA0_RXI ((IRQn_Type) 41) /* UARTA0 RXI (UARTA0 reception transfer end) */
#define UARTA0_RXI_IRQn          ((IRQn_Type) 41) /* UARTA0 RXI (UARTA0 reception transfer end) */
/* The number of entries required for the ICU vector table. */
#define BSP_ICU_VECTOR_NUM_ENTRIES (42)

#ifdef __cplusplus
        }
        #endif
#endif /* VECTOR_DATA_H */
