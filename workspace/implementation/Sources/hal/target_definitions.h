/**
 * @file target_definitions.h
 * @author Guilherme Kairalla Kolotelo
 * @author Bruno de Souza Ferreira
 * @version 1.9
 * @date 25 Jun 2016
 * @date 21 Nov 2016
 * @brief File containing the pin and module definitions and constants.
 */

#ifndef SOURCES_TARGET_PINS_H_
#define SOURCES_TARGET_PINS_H_

/* system includes */
#include <MKL25Z4.h>
#include "fsl_gpio_driver.h"

/*                    General uC Definitions            */
/* Cyclic executive period in microseconds */
#define CYCLIC_EXECUTIVE_PERIOD			50 * 1000 //micro seconds
#define DEBUG_MODE_ENABLE				true
/* Clock gate control */
#define  CGC_CLOCK_DISABLED         	0x00U
#define  CGC_CLOCK_ENABLED          	0x01U
/* Project specific definitions */
#define CONST_2PI		                2 * 3.14159
#define MAX_RW_MOTOR_VELOCITY_PPS		110
#define MAX_LW_MOTOR_VELOCITY_PPS		110
#define ADC_10MS_MULTIPLE_WAIT_PERIOD	1
/*                 END OF General uC Definitions        */

/*                 Voltage sense definitions            */
#define VSENSE1_PORT_INSTANCE			PORTB_IDX
#define VSENSE2_PORT_INSTANCE			PORTB_IDX
#define VSENSE1_PIN_NUMBER				0U
#define VSENSE2_PIN_NUMBER				1U
#define VSENSE1_CH_NUMBER				0b01000
#define VSENSE2_CH_NUMBER				0b01010
#define VSENSE_PORT_ALT					kPortPinDisabled
#define VSENSE1_CORRECTION_FACTOR		0.00020088799
#define VSENSE2_CORRECTION_FACTOR		0.00032493882
#define VSENSE_RESISTOR_VALUE			0.52 * 1000 //Ohm * 1000
#define VSENSE_MIN_VOLTAGE				9.0	 		//V
/*              END OF Voltage sense definitions        */

/*                  User specific settings              */
#define MOTOR_MIN_CURR					500.0		//mA
#define MOTOR_MIN_VEL					60.0		//Minimum pulse counts
#define MOTOR_MAX_VOLTAGE				8.0			//V
#define MOTOR_FAST_SPEED				55.0		//Pulses Per Second
#define MOTOR_SLOW_SPEED				10.0		//Pulses Per Second
#define IR_MAX_DIFF 					0.2			//Absolute
#define IR_MIN_DIFF 					0.5			//Absolute
#define MAX_POSITION_EFFORT				0			//Absolute
#define STOP_COUNTER_DISTANCE			75			//75 Cycles -> Approx. 1 meter
#define STOP_WAIT_PERIOD				5			//Seconds
/*               END OF User specific settings           */


/*                     Driver Definitions                */
#define DRIVER_LW_CHA_INSTANCE			5U
#define DRIVER_LW_CHA_PIN_NUMBER		9U
#define DRIVER_LW_CHA_PORT_ALT			kPortMuxAlt3
#define DRIVER_LW_CHA_PORT_INSTANCE		PORTC_IDX
#define DRIVER_LW_CHB_INSTANCE			4U
#define DRIVER_LW_CHB_PIN_NUMBER		8U
#define DRIVER_LW_CHB_PORT_ALT			kPortMuxAlt3
#define DRIVER_LW_CHB_PORT_INSTANCE		PORTC_IDX

#define DRIVER_RW_CHA_INSTANCE			2U
#define DRIVER_RW_CHA_PIN_NUMBER		5U
#define DRIVER_RW_CHA_PORT_ALT			kPortMuxAlt3
#define DRIVER_RW_CHA_PORT_INSTANCE		PORTA_IDX
#define DRIVER_RW_CHB_INSTANCE			1U
#define DRIVER_RW_CHB_PIN_NUMBER		4U
#define DRIVER_RW_CHB_PORT_ALT			kPortMuxAlt3
#define DRIVER_RW_CHB_PORT_INSTANCE		PORTA_IDX

#define DRIVER_LW_TPM_CLKIN_INSTANCE
#define DRIVER_LW_TPM_INSTANCE			TPM0_IDX
#define DRIVER_LW_TPM_CLK_SRC			kTpmClockSourceModuleClk
#define DRIVER_LW_TPM_CLK_PS			kTpmDividedBy1

#define DRIVER_RW_TPM_CLKIN_INSTANCE
#define DRIVER_RW_TPM_INSTANCE			TPM0_IDX
#define DRIVER_RW_TPM_CLK_SRC			kTpmClockSourceModuleClk
#define DRIVER_RW_TPM_CLK_PS			kTpmDividedBy1

#define DRIVER_LW_EN_PIN_NUMBER			2U
#define DRIVER_LW_EN_PORT_ALT			kPortMuxAsGpio
#define DRIVER_LW_EN_PORT_INSTANCE		PORTA_IDX
#define DRIVER_LW_EN_GPIO_INSTANCE		GPIOA_IDX

#define DRIVER_RW_EN_PIN_NUMBER			1U
#define DRIVER_RW_EN_PORT_ALT			kPortMuxAsGpio
#define DRIVER_RW_EN_PORT_INSTANCE		PORTA_IDX
#define DRIVER_RW_EN_GPIO_INSTANCE		GPIOA_IDX

#define DRIVER_EN_PIN_DIR				kGpioDigitalOutput
/*                  END OF Driver Definitions            */

/*                     Encoder Definitions               */
#define ENCODER_RW_PIN_NUMBER			13U
#define ENCODER_RW_PORT_ALT				kPortMuxAlt4
#define ENCODER_RW_PORT_INSTANCE		PORTC_IDX
#define ENCODER_RW_TPM_INSTANCE			TPM1_IDX
#define ENCODER_RW_FTM_CLKIN_SRC		kSimTpmClkSel1 //FTM_CLKIN1
#define ENCODER_RW_MAX_PULSE_COUNT		5000
#define ENCODER_RW_PULSE_COUNT			27
#define ENCODER_RW_ACQ_PERIOD_US		CYCLIC_EXECUTIVE_PERIOD

#define ENCODER_LW_PIN_NUMBER			12U
#define ENCODER_LW_PORT_ALT				kPortMuxAlt4
#define ENCODER_LW_PORT_INSTANCE		PORTC_IDX
#define ENCODER_LW_TPM_INSTANCE			TPM2_IDX
#define ENCODER_LW_FTM_CLKIN_SRC		kSimTpmClkSel0 //FTM_CLKIN0
#define ENCODER_LW_MAX_PULSE_COUNT		5000
#define ENCODER_LW_PULSE_COUNT			27
#define ENCODER_LW_ACQ_PERIOD_US		CYCLIC_EXECUTIVE_PERIOD
/*                  END OF Encoder Definitions           */

/*                      HMI Definitions                  */
#define HMI_UART_PORT_INSTANCE      	PORTA_IDX
#define HMI_UART_PIN_RX             	1
#define HMI_UART_PIN_TX             	2
#define HMI_UART_PIN_ALT            	kPortMuxAlt2
#define HMI_UART_INSTANCE           	UART0_IDX
#define HMI_UART_BASE               	UART0
#define HMI_UART_BAUD               	115200
#define HMI_DIAG_BTN_PORT_ALT			kPortMuxAsGpio
#define HMI_DIAG_BTN_PIN_NUMBER			2
#define HMI_DIAG_BTN_PDIR_MASK			0b100
#define HMI_STARTSTOP_BTN_PORT_ALT		kPortMuxAsGpio
#define HMI_STARTSTOP_BTN_PIN_NUMBER	3
#define HMI_STARTSTOP_BTN_PDIR_MASK		0b1000
#define HMI_RED_LED_ALT					kPortMuxAsGpio
#define HMI_RED_PIN_NUMBER				18
#define HMI_GREEN_LED_ALT				kPortMuxAsGpio
#define HMI_GREEN_PIN_NUMBER			19
#define HMI_BLUE_LED_ALT				kPortMuxAsGpio
#define HMI_BLUE_PIN_NUMBER				1
/*                   END OF HMI Definitions              */

/*                   IR Array Definitions                */

const uint32_t ADC_IRX_EN_PORT_INSTANCE[6] = {PORTC_IDX, PORTC_IDX, PORTC_IDX, PORTC_IDX, PORTC_IDX, PORTC_IDX};

const uint32_t ADC_IRX_EN_GPIO_INSTANCE[6] = {GPIOC_IDX, GPIOC_IDX, GPIOC_IDX, GPIOC_IDX, GPIOC_IDX, GPIOC_IDX};

const uint32_t ADC_IRX_ADC_PORT_INSTANCE[6] = {PORTC_IDX, PORTC_IDX, PORTB_IDX, PORTE_IDX, PORTE_IDX, PORTE_IDX};

const uint32_t ADC_IRX_EN_PIN_NUMBER[6] = {6, 5, 4, 3, 0, 7};

const uint32_t ADC_IRX_ADC_PIN_NUMBER[6] = {2, 1, 2, 22, 21, 20};

const uint32_t ADC_IRX_ADC_CH_NUMBER[6] = {0b01011, 0b01111, 0b00111, 0b00011, 0b00100, 0b00000};
#define ADC_EN_PORT_ALT					kPortMuxAsGpio
#define ADC_ADC_PORT_ALT				kPortPinDisabled

/*                END OF IR Array Definitions            */

#endif /* SOURCES_TARGET_PINS_H_*/
