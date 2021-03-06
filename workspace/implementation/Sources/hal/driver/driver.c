/**
 * @file driver.c
 * @author Guilherme Kairalla Kolotelo
 * @author Bruno de Souza Ferreira
 * @version 1.1
 * @date 10 Jun 2016
 * @date 27 Sep 2016
 * @brief File containing the methods for the operation of an H-Bridge in bipolar operation mode.
 * @detail File containing the methods for the operation
 *         of an H-Bridge in bipolar operation mode.
 *         
 *         - Driver actuation through driver_setDriver goes from -100 to 100, the
 *           former being full reverse, and the latter, full steam ahead.
 *         - Driver max freq @ 100kHz.
 *         - Channel A on HighTrue and Channel B on LowTrue makes
 *           for a bipolar H-Bridge pair of control signals.
 */


/* Project Includes */
#include "driver.h"
#include "hal/target_definitions.h"

/* System Includes */


#include "fsl_port_hal.h"
#include "fsl_gpio_hal.h"
#include "fsl_interrupt_manager.h"
#include "fsl_tpm_hal.h"
#include "fsl_tpm_driver.h"
#include "fsl_pwm_driver.h"
#include "fsl_gpio_driver.h"
#include "fsl_clock_manager.h"

/* Nominal switching frequency */
/* 25kHz */
#define DRIVER_FREQUENCY            25000
/* Prescaler */
#define DRIVER_PRESCALER            kTpmDividedBy1



/**
 * @brief Initializes the driver with the load in idle (50% duty cycle).
 * 
 * @param driverInstance driver_instance_t struct.
 */
void driver_initDriver(driver_instance_t driverInstance)
{
    PORT_Type *driverEnPortBase = g_portBase[driverInstance.uiDriverEnPortInstance];
    PORT_Type *driverPwmChAPortBase = g_portBase[driverInstance.uiDriverPwmChAPortInstance];
    PORT_Type *driverPwmChBPortBase = g_portBase[driverInstance.uiDriverPwmChBPortInstance];
    GPIO_Type *driverEnGpioBase = g_gpioBase[driverInstance.uiDriverEnGpioInstance];

    /* Configure Pins */
    /* Channel pins */
    CLOCK_SYS_EnablePortClock(driverInstance.uiDriverPwmChAPortInstance);
    CLOCK_SYS_EnablePortClock(driverInstance.uiDriverPwmChBPortInstance);
    PORT_HAL_SetMuxMode(driverPwmChAPortBase, driverInstance.uiDriverPwmChAPinNumber, driverInstance.uiDriverPwmChAPortAlt);
    PORT_HAL_SetMuxMode(driverPwmChBPortBase, driverInstance.uiDriverPwmChBPinNumber, driverInstance.uiDriverPwmChBPortAlt);

    /* Enable pin */
    CLOCK_SYS_EnablePortClock(driverInstance.uiDriverEnPortInstance);
    PORT_HAL_SetMuxMode(driverEnPortBase, driverInstance.uiDriverEnPinNumber, driverInstance.uiDriverEnPortAlt);
    GPIO_HAL_SetPinDir(driverEnGpioBase, driverInstance.uiDriverEnPinNumber, DRIVER_EN_PIN_DIR);
    GPIO_HAL_ClearPinOutput(driverEnGpioBase, driverInstance.uiDriverEnPinNumber);

    /* Configure TPM module */
    /* Using 8MHz external clock source */
    CLOCK_SYS_SetTpmSrc(driverInstance.uiDriverTpmInstance, kClockTpmSrcOsc0erClk);

    /* Will be using USB serial over OpenSDA, must enable Debug Mode */
    tpm_general_config_t config =
    {
            .isDBGMode = DEBUG_MODE_ENABLE
    };

    TPM_DRV_Init(driverInstance.uiDriverTpmInstance, &config);
    TPM_DRV_SetClock(driverInstance.uiDriverTpmInstance, kTpmClockSourceModuleClk, driverInstance.tTpmClkPrescaler);

    driver_disableDriver(driverInstance);

    /* Configure PWM channels. 50% duty cycle is idle, since the driver is bipolar */
    tpm_pwm_param_t pwmA =
        {
                .mode = kTpmEdgeAlignedPWM,
                .edgeMode = kTpmHighTrue, /* High true, on when channel B is off */
                .uFrequencyHZ = DRIVER_FREQUENCY,
                .uDutyCyclePercent = 50,

        };
    tpm_pwm_param_t pwmB =
    {
            .mode = kTpmEdgeAlignedPWM,
            .edgeMode = kTpmLowTrue, /* Low true, on when channel A is off*/
            .uFrequencyHZ = DRIVER_FREQUENCY,
            .uDutyCyclePercent = 50,

    };

    TPM_DRV_PwmStart(driverInstance.uiDriverTpmInstance, &pwmA, driverInstance.uiDriverPwmChAChannelInstance);
    TPM_DRV_PwmStart(driverInstance.uiDriverTpmInstance, &pwmB, driverInstance.uiDriverPwmChBChannelInstance);


}

void driver_appendDriver(driver_instance_t driverInstance)
{
	PORT_Type *driverEnPortBase = g_portBase[driverInstance.uiDriverEnPortInstance];
	PORT_Type *driverPwmChAPortBase = g_portBase[driverInstance.uiDriverPwmChAPortInstance];
	PORT_Type *driverPwmChBPortBase = g_portBase[driverInstance.uiDriverPwmChBPortInstance];
	GPIO_Type *driverEnGpioBase = g_gpioBase[driverInstance.uiDriverEnGpioInstance];

	/* Configure Pins */
	/* Channel pins */
	CLOCK_SYS_EnablePortClock(driverInstance.uiDriverPwmChAPortInstance);
	CLOCK_SYS_EnablePortClock(driverInstance.uiDriverPwmChBPortInstance);
	PORT_HAL_SetMuxMode(driverPwmChAPortBase, driverInstance.uiDriverPwmChAPinNumber, driverInstance.uiDriverPwmChAPortAlt);
	PORT_HAL_SetMuxMode(driverPwmChBPortBase, driverInstance.uiDriverPwmChBPinNumber, driverInstance.uiDriverPwmChBPortAlt);

	/* Enable pin */
	CLOCK_SYS_EnablePortClock(driverInstance.uiDriverEnPortInstance);
	PORT_HAL_SetMuxMode(driverEnPortBase, driverInstance.uiDriverEnPinNumber, driverInstance.uiDriverEnPortAlt);
	GPIO_HAL_SetPinDir(driverEnGpioBase, driverInstance.uiDriverEnPinNumber, DRIVER_EN_PIN_DIR);
	GPIO_HAL_ClearPinOutput(driverEnGpioBase, driverInstance.uiDriverEnPinNumber);

	/* Configure TPM module */
	/* Using 8MHz external clock source */
	CLOCK_SYS_SetTpmSrc(driverInstance.uiDriverTpmInstance, kClockTpmSrcOsc0erClk);

	TPM_DRV_SetClock(driverInstance.uiDriverTpmInstance, kTpmClockSourceModuleClk, driverInstance.tTpmClkPrescaler);

	driver_disableDriver(driverInstance);

	/* Configure PWM channels. 50% duty cycle is idle, since the driver is bipolar */
	tpm_pwm_param_t pwmA =
		{
				.mode = kTpmEdgeAlignedPWM,
				.edgeMode = kTpmHighTrue, /* High true, on when channel B is off */
				.uFrequencyHZ = DRIVER_FREQUENCY,
				.uDutyCyclePercent = 50,

		};
	tpm_pwm_param_t pwmB =
	{
			.mode = kTpmEdgeAlignedPWM,
			.edgeMode = kTpmLowTrue, /* Low true, on when channel A is off*/
			.uFrequencyHZ = DRIVER_FREQUENCY,
			.uDutyCyclePercent = 50,

	};

	TPM_DRV_PwmStart(driverInstance.uiDriverTpmInstance, &pwmA, driverInstance.uiDriverPwmChAChannelInstance);
	TPM_DRV_PwmStart(driverInstance.uiDriverTpmInstance, &pwmB, driverInstance.uiDriverPwmChBChannelInstance);

}


/**
 * @brief Disables the driver by clearing the enable pin.
 * 
 * @param driverInstance driver_instance_t struct.
 */
void driver_disableDriver(driver_instance_t driverInstance)
{
    GPIO_Type *driverEnGpioBase = g_gpioBase[driverInstance.uiDriverEnGpioInstance];
    GPIO_HAL_ClearPinOutput(driverEnGpioBase, driverInstance.uiDriverEnPinNumber);
}


/**
 * @brief Enables the driver by setting the enable pin
 * 
 * @param driverInstance driver_instance_t struct.
 */
void driver_enableDriver(driver_instance_t driverInstance)
{
    GPIO_Type *driverEnGpioBase = g_gpioBase[driverInstance.uiDriverEnGpioInstance];
    GPIO_HAL_SetPinOutput(driverEnGpioBase, driverInstance.uiDriverEnPinNumber);
}


/**
 * @brief Sets duty cycle for Channel A only.
 * 
 * @param driverInstance driver_instance_t struct.
 * @param uiDutyCyclePercent Duty cycle in percentage.
 */
void driver_setChannelADutyCycle(driver_instance_t driverInstance, int uiDutyCyclePercent)
{
    TPM_Type *tpmBase = g_tpmBase[driverInstance.uiDriverTpmInstance];
    TPM_HAL_SetChnCountVal(tpmBase, driverInstance.uiDriverPwmChAChannelInstance, ((TPM_HAL_GetMod(tpmBase)*uiDutyCyclePercent)/100));
}


/**
 * @brief Sets duty cycle for Channel B only.
 * 
 * @param driverInstance driver_instance_t struct.
 * @param uiDutyCyclePercent Duty cycle in percentage.
 */
void driver_setChannelBDutyCycle(driver_instance_t driverInstance, int uiDutyCyclePercent)
{
    TPM_Type *tpmBase = g_tpmBase[driverInstance.uiDriverTpmInstance];
    TPM_HAL_SetChnCountVal(tpmBase, driverInstance.uiDriverPwmChBChannelInstance, ((TPM_HAL_GetMod(tpmBase)*uiDutyCyclePercent)/100));
}


/**
 * @brief Sets duty cycle for H-Bridge operation. Both ChA and ChB.
 * 
 * @param driverInstance driver_instance_t struct.
 * @param uiDutyCyclePercent Duty cycle in percentage (0% is full reverse, 100% is full ahead).
 */
void driver_setHBridgeDutyCycle(driver_instance_t driverInstance, int uiDutyCyclePercent)
{
    TPM_Type *tpmBase = g_tpmBase[driverInstance.uiDriverTpmInstance];
    uint32_t uichannelCnV = (TPM_HAL_GetMod(tpmBase)*uiDutyCyclePercent)/100;
    TPM_HAL_SetChnCountVal(tpmBase, driverInstance.uiDriverPwmChAChannelInstance, uichannelCnV);
    TPM_HAL_SetChnCountVal(tpmBase, driverInstance.uiDriverPwmChBChannelInstance, uichannelCnV);
}


/**
 * @brief Sets the driver from -100 to 100, the former being full reverse, and the latter, full steam ahead.
 * 
 * @param driverInstance driver_instance_t struct.
 * @param input -100 to 100.
 */
void driver_setDriver(driver_instance_t driverInstance, int input)
{
    /* Cap out-of-bound input */
    if(input < -100)
        input = -100;
    if(input > 100)
        input = 100;
    input = (input + 100)/2;
    driver_setHBridgeDutyCycle(driverInstance, input);
}

/**
 * @brief setDriver but takes another argument to multiply duty cycle
 * 
 * @param driverInstance driver_instance_t struct.
 * @param input -100 to 100.
 * @param factor to multiply input.
 */
void driver_setDriver2(driver_instance_t driverInstance, int input, float factor)
{
    /* Cap out-of-bound input */
    if(input < -100)
        input = -100;
    if(input > 100)
        input = 100;
    input = (int)((float)input*factor);
    input = (input + 100)/2;
    driver_setHBridgeDutyCycle(driverInstance, input);
}
