/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file main.c
 * @author Guilherme Kairalla Kolotelo
 * @author Bruno de Souza Ferreira
 * @version 1.0
 * @date 9 Sep 2016
 * @date 24 Nov 2016
 * @brief File containing the main entry point of the program.
 */


/* System includes */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_clock_manager.h"
#include "hal/mcg/mcg.h"
#include <stdbool.h>

/* Project includes */
#include "hal/adc/adc.h"
#include "hal/controller/controller.h"
#include "hal/diagnostics/diagnostics.h"
#include "hal/driver/driver.h"
#include "hal/encoder/encoder.h"
#include "hal/hmi/hmi.h"
#include "hal/ir_array/ir_array.h"
#include "hal/util/util.h"
#include "hal/vsense/vsense.h"
#include "hal/target_definitions.h"
#include "hal/util/tc_hal.h"

/* Globals */
volatile unsigned int uiFlagNextPeriod = 0;         /* cyclic executive flag */
float fsys_voltage = 0;
float fvoltage_correction = 0;
float fmotor_current_speed = MOTOR_FAST_SPEED;

void main_cyclicExecuteIsr(void)
{
    /* set the cyclic executive flag */
    uiFlagNextPeriod = 1;
}

/* Configuration structures for encoder instances and driver instances */
encoder_instance_t tencoderL = {
	/* Config section */
	.cEncoderInstance = 'L',
	.uiEncoderPortInstance = ENCODER_LW_PORT_INSTANCE,
	.uiEncoderPinNumber = ENCODER_LW_PIN_NUMBER,
	.uiEncoderPortAlt = ENCODER_LW_PORT_ALT,
	.uiEncoderTpmInstance = ENCODER_LW_TPM_INSTANCE,
	.uiEncoderTpmClkinSrc = ENCODER_LW_FTM_CLKIN_SRC,
	/* Encoder Hardware Setup section */
	.uiEncoderMaxPulseCount = ENCODER_LW_MAX_PULSE_COUNT,
	.uiEncoderPulseCount = ENCODER_LW_PULSE_COUNT,
	.uiEncoderAcqPeriodUs = ENCODER_LW_ACQ_PERIOD_US,

	/* Data section */
	.uiEncoderPulsesPerSecond = 0
};
encoder_instance_t tencoderR = {
	/* Config section */
	.cEncoderInstance = 'R',
	.uiEncoderPortInstance = ENCODER_RW_PORT_INSTANCE,
	.uiEncoderPinNumber = ENCODER_RW_PIN_NUMBER,
	.uiEncoderPortAlt = ENCODER_RW_PORT_ALT,
	.uiEncoderTpmInstance = ENCODER_RW_TPM_INSTANCE,
	.uiEncoderTpmClkinSrc = ENCODER_RW_FTM_CLKIN_SRC,
	/* Encoder Hardware Setup section */
	.uiEncoderMaxPulseCount = ENCODER_RW_MAX_PULSE_COUNT,
	.uiEncoderPulseCount = ENCODER_RW_PULSE_COUNT,
	.uiEncoderAcqPeriodUs = ENCODER_RW_ACQ_PERIOD_US,

	/* Data section */
	.uiEncoderPulsesPerSecond = 0
};
driver_instance_t tdriverL = {
	/* Config section */
	.cDriverInstance = 'L',
	.uiDriverPwmChAPortInstance = DRIVER_LW_CHA_PORT_INSTANCE,
	.uiDriverPwmChAPinNumber = DRIVER_LW_CHA_PIN_NUMBER,
	.uiDriverPwmChAPortAlt = DRIVER_LW_CHA_PORT_ALT,
	.uiDriverPwmChAChannelInstance = DRIVER_LW_CHA_INSTANCE,
	.uiDriverPwmChBPortInstance = DRIVER_LW_CHB_PORT_INSTANCE,
	.uiDriverPwmChBPinNumber = DRIVER_LW_CHB_PIN_NUMBER,
	.uiDriverPwmChBPortAlt = DRIVER_LW_CHB_PORT_ALT,
	.uiDriverPwmChBChannelInstance = DRIVER_LW_CHB_INSTANCE,
	//uiDriverTpmInstance = DRIVER_LW_TPM_INSTANCE,
	.uiDriverTpmClkinInstance = DRIVER_LW_TPM_CLK_SRC,
	.tTpmClkSrc = DRIVER_LW_TPM_CLK_SRC,
	.tTpmClkPrescaler = DRIVER_LW_TPM_CLK_PS, // kTpmDividedBy1

	.uiDriverEnPinNumber = DRIVER_LW_EN_PIN_NUMBER,
	.uiDriverEnPortAlt = DRIVER_LW_EN_PORT_ALT,
	.uiDriverEnPortInstance = DRIVER_LW_EN_PORT_INSTANCE,
	.uiDriverEnGpioInstance = DRIVER_LW_EN_GPIO_INSTANCE
};
driver_instance_t tdriverR = {
	/* Config section */
	.cDriverInstance = 'R',
	.uiDriverPwmChAPortInstance = DRIVER_RW_CHA_PORT_INSTANCE,
	.uiDriverPwmChAPinNumber = DRIVER_RW_CHA_PIN_NUMBER,
	.uiDriverPwmChAPortAlt = DRIVER_RW_CHA_PORT_ALT,
	.uiDriverPwmChAChannelInstance = DRIVER_RW_CHA_INSTANCE,
	.uiDriverPwmChBPortInstance = DRIVER_RW_CHB_PORT_INSTANCE,
	.uiDriverPwmChBPinNumber = DRIVER_RW_CHB_PIN_NUMBER,
	.uiDriverPwmChBPortAlt = DRIVER_RW_CHB_PORT_ALT,
	.uiDriverPwmChBChannelInstance = DRIVER_RW_CHB_INSTANCE,
	//uiDriverTpmInstance = DRIVER_RW_TPM_INSTANCE,
	.uiDriverTpmClkinInstance = DRIVER_RW_TPM_CLK_SRC,
	.tTpmClkSrc = DRIVER_RW_TPM_CLK_SRC,
	.tTpmClkPrescaler = DRIVER_RW_TPM_CLK_PS, // kTpmDividedBy1

	.uiDriverEnPinNumber = DRIVER_RW_EN_PIN_NUMBER,
	.uiDriverEnPortAlt = DRIVER_RW_EN_PORT_ALT,
	.uiDriverEnPortInstance = DRIVER_RW_EN_PORT_INSTANCE,
	.uiDriverEnGpioInstance = DRIVER_RW_EN_GPIO_INSTANCE
};
controller_instance_t tpidP = {
	/* Config section */
	.cControllerInstance = 'P'
};
controller_instance_t tpidL = {
	/* Config section */
	.cControllerInstance = 'L'
};

controller_instance_t tpidR = {
	/* Config section */
	.cControllerInstance = 'R'
};


/**
 * @brief Initializes board clock configurations.
 *
 */
void boardInit()
{
	mcg_clockInit();
}

/**
 * @brief Initializes board peripherals and component instances.
 * 
 */
void peripheralInit()
{
	/* Setup RGB LEDs for Status */
	SIM_SCGC5 |= (SIM_SCGC5_PORTB_MASK);
	SIM_SCGC5 |= (SIM_SCGC5_PORTD_MASK);
	PORTB_PCR18 = PORT_PCR_MUX(HMI_RED_LED_ALT); // Red
	PORTB_PCR19 = PORT_PCR_MUX(HMI_GREEN_LED_ALT); // Green
	PORTD_PCR1 = PORT_PCR_MUX(HMI_BLUE_LED_ALT);  // Blue
	PTB_BASE_PTR->PDDR |= 1 << HMI_RED_PIN_NUMBER;
	PTB_BASE_PTR->PDDR |= 1 << HMI_GREEN_PIN_NUMBER;
	PTD_BASE_PTR->PDDR |= 1 << HMI_BLUE_PIN_NUMBER;
	PTB_BASE_PTR->PCOR |= 1 << HMI_GREEN_PIN_NUMBER;
	PTB_BASE_PTR->PSOR |= 1 << HMI_RED_PIN_NUMBER;
	PTD_BASE_PTR->PSOR |= 1 << HMI_BLUE_PIN_NUMBER;

	/* Setup Buttons */
	SIM_SCGC5 |= (SIM_SCGC5_PORTD_MASK);
	PORTD_PCR2 = PORT_PCR_MUX(HMI_DIAG_BTN_PORT_ALT); // Diag. button
	PORTD_PCR3 = PORT_PCR_MUX(HMI_STARTSTOP_BTN_PORT_ALT); // Start button

	/* Setup peripherals */
	adc_initAdc();
	hmi_initHmi();
	ir_array_initArray();
	vsense_initVsense();
	ir_array_ledArrayOn();

	/* Setup peripheral instances */
	driver_initDriver(tdriverL);
	driver_appendDriver(tdriverR);
	encoder_initEncoder(tencoderL);
	encoder_initEncoder(tencoderR);

	/* Setup controller */
	controller_initPID(&tpidP, 5, 60, 20, 1000);
	controller_initPID(&tpidR, 100, 0.8, 0.8, 0.5);
	controller_initPID(&tpidL, 100, 0.5, 0.8, 0.5);

	/* Setup cyclic executive timer */
	tc_installLptmr0(CYCLIC_EXECUTIVE_PERIOD, main_cyclicExecuteIsr);
}

/**
 * @brief Main program entry point.
 *
 */
int main(void)
{
	boardInit();
	peripheralInit();

	while(1)
	{
		/* Indicate status */
		PTB_BASE_PTR->PCOR |= 1 << HMI_GREEN_PIN_NUMBER; // Turn on Green LED
		PTB_BASE_PTR->PSOR |= 1 << HMI_RED_PIN_NUMBER;   // Turn off Red LED
		PTD_BASE_PTR->PSOR |= 1 << HMI_BLUE_PIN_NUMBER;  // Turn off Blue LED

		while((GPIOD->PDIR & HMI_STARTSTOP_BTN_PDIR_MASK)) // While start button not pressed (grounded)
		{
			if(!(GPIOD->PDIR & HMI_DIAG_BTN_PDIR_MASK)) // If diag. button pressed (grounded)
				
			// IF ALL TESTS ARE NOT PASSED, THE SYSTEM IS HALTED!
			if(diagnostics_startDiagnostics() == true)
			{
				PTB_BASE_PTR->PSOR |= 1 << HMI_GREEN_PIN_NUMBER;
				PTB_BASE_PTR->PCOR |= 1 << HMI_RED_PIN_NUMBER;
				while(1);
			}

		}
		/* Start execution. Run until stop button is pressed */

		/* Calibrate IR Array */
		ir_array_calibrate(uiIrReadings);

		/* Get current voltage and calculate correction for proper motor operation */
		fsys_voltage = vsense_getV1();
		fvoltage_correction = MOTOR_MAX_VOLTAGE/fsys_voltage;

		/* Wait 3s for user to position robot on track. Blink Green LED for countdown. */
		for(int i=0; i<3; i++)
		{
			PTB_BASE_PTR->PTOR |= 1 << HMI_GREEN_PIN_NUMBER;
			for(int j=0; j<3; j++)util_genDelay100ms();
			PTB_BASE_PTR->PTOR |= 1 << HMI_GREEN_PIN_NUMBER;
			for(int j=0; j<8; j++)util_genDelay100ms();
		}
		PTB_BASE_PTR->PSOR |= 1 << HMI_GREEN_PIN_NUMBER;

		/* Pre-run resets */
		encoder_resetCounter(tencoderL);
		encoder_resetCounter(tencoderR);
		driver_enableDriver(tdriverR);
		driver_enableDriver(tdriverL);

		/* Uncomment if using diagnostics section below. */
		//hmi_initHmi();

		/* Declare local variables */
		float fpositionError, fleftWheelSpeed, frightWheelSpeed;
		float fpositionEffort, fleftWheelEffort, frightWheelEffort;
		bool bcommand_found = false;
		bool bcounter_on = false;
		bool bcommand_stop = false;
		bool bcommand_slow = false;
		int icounter = 0;


		while(1)
		{
			/* Turn on Blue LED. Start of execution cycle.*/
			PTD_BASE_PTR->PSOR |= 1 << HMI_BLUE_PIN_NUMBER;

			/* Take measurements */
			encoder_takeMeasurement(&tencoderL);
			encoder_takeMeasurement(&tencoderR);
			fpositionError = ir_array_getPosition();

			/* If known command detected proceed */
			if(-20 != fpositionError)
			{
				PTB_BASE_PTR->PSOR |= 1 << HMI_RED_PIN_NUMBER; 	 // Turn off Red LED, command is valid.
				PTB_BASE_PTR->PSOR |= 1 << HMI_GREEN_PIN_NUMBER; // Turn off Green LED
				/* Command bar detected */
				if(-10 == fpositionError)
				{
					/* Indicate status */
					PTB_BASE_PTR->PCOR |= 1 << HMI_GREEN_PIN_NUMBER; // Turn on Green LED
					/* set flags */
					bcommand_found = true;
					bcounter_on = true;
					/* proceed in a straight line */
					driver_setDriver2(tdriverR, 50, fvoltage_correction);
					driver_setDriver2(tdriverL, 50, fvoltage_correction);
					/* Avoid jitters */
					for(int j=0; j<1; j++)util_genDelay100ms();
				}
				else
				{
					/* Get measured values */
					fleftWheelSpeed = encoder_getAngularVelocity(tencoderL);
					frightWheelSpeed = encoder_getAngularVelocity(tencoderR);

					/* Update track PID Algorithm */
					fpositionEffort = controller_PIDUpdate(&tpidP, fpositionError, 0);

					/* Update motor PID algorithms */
					fleftWheelEffort = controller_PIDUpdate(&tpidL, fleftWheelSpeed, (fmotor_current_speed - fpositionEffort));
					frightWheelEffort = controller_PIDUpdate(&tpidR, frightWheelSpeed, (fmotor_current_speed + fpositionEffort));

					/* Avoid spinning in the opposite direction */
					if(frightWheelEffort < -10)
						frightWheelEffort = 0;
					if(fleftWheelEffort < -10)
						fleftWheelEffort = 0;

					/* Set Motors */
					driver_setDriver2(tdriverR, frightWheelEffort, fvoltage_correction);
					driver_setDriver2(tdriverL, fleftWheelEffort, fvoltage_correction);

					/* Command State Machine Behavior */
					if(bcommand_found)
					{
						if(bcommand_slow)
						{
							/* If in slow down area and command found again, disable slow down. */
							fmotor_current_speed = MOTOR_FAST_SPEED;
							bcommand_slow = false;
							bcounter_on = false;
						}
						else if(icounter > 1) // Command to STOP if second bar found again.
						{
							bcommand_stop = true;
						}
						bcommand_found = false;
					}

					if(bcounter_on){
						icounter++;
					}
					else
						icounter = 0;

					if(icounter > 8 && !bcommand_stop) // Command to Slow Down if second bar not found.
					{
						/* Slow down. */
						fmotor_current_speed = MOTOR_SLOW_SPEED;
						bcommand_slow = true;
						bcounter_on = false;
						icounter = 0;
					}
					else if(icounter > STOP_COUNTER_DISTANCE && bcommand_stop) // approx. 2 meters.
					{
						/* Stop and wait. */
						driver_setDriver(tdriverR, 0);
						driver_setDriver(tdriverL, 0);
						for(int i=0; i<10*STOP_WAIT_PERIOD; i++) util_genDelay100ms();
						bcommand_stop = false;
						bcounter_on = false;
						icounter = 0;
					}
				}
			}
			else
			/* If unknown command detected indicate status */
			{
				PTB_BASE_PTR->PCOR |= 1 << HMI_RED_PIN_NUMBER; // Turn on Red LED
				PTD_BASE_PTR->PCOR |= 1 << HMI_BLUE_PIN_NUMBER; // Turn off Blue LED
			}

			/* 
				Diagnostics Section. Comment this out to analyze variable on execution time.
			   	May need to increase cyclic period to accomodate printing time.
			   	Uncomment HMI initialization at the beginning.
			*/	
//			hmi_transmitSCSF("Pos",':',"", fpositionError);
//			hmi_transmitSCSF("RS ",':',"", frightWheelSpeed);
//			hmi_transmitSCSF("LS ",':',"", fleftWheelSpeed);
//			hmi_transmitSCSF("PE ",':',"", fpositionEffort);
//			hmi_transmitSCSF("RE ",':',"", frightWheelEffort);
//			hmi_transmitSCSF("LE ",':',"", fleftWheelEffort);
//			PRINTF("\r\n");
//			hmi_receive(); // Enables execution time changes of PID constants.
			

			/* Stop button press check loop */
			if(!(GPIOD->PDIR & HMI_STARTSTOP_BTN_PDIR_MASK))
			{
				util_genDelay10ms(); // Debounce
				if(!(GPIOD->PDIR & HMI_STARTSTOP_BTN_PDIR_MASK))
				{
					/* Turn on Red LED to indicate status */
					PTD_BASE_PTR->PSOR |= 1 << HMI_BLUE_PIN_NUMBER;
					PTB_BASE_PTR->PSOR |= 1 << HMI_GREEN_PIN_NUMBER;
					PTB_BASE_PTR->PCOR |= 1 << HMI_RED_PIN_NUMBER;
					/* Disable motors */
					driver_initDriver(tdriverL);
					driver_appendDriver(tdriverR);
					driver_disableDriver(tdriverR);
					driver_disableDriver(tdriverL);
					/* Wait 1 second for user to release button */
					for(int i=0; i<10; i++)util_genDelay100ms();
					break;
				}
			}

			/* Turn off Blue LED. End of execution cycle. */
			PTD_BASE_PTR->PCOR |= 1 << HMI_BLUE_PIN_NUMBER;

			/* Wait for next cycle */
			while(!uiFlagNextPeriod);
			uiFlagNextPeriod = 0;
		}
	}
}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
