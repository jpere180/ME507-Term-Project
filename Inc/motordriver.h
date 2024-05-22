/**
 * @file motordriver.h
 * @brief Motor driver header file.
 */

#ifndef INC_MOTORDRIVER_H_
#define INC_MOTORDRIVER_H_

#include <stdio.h>
#include <stdint.h>
#include "stm32l4xx_hal.h"

/**
 * @brief Motor driver structure.
 *
 * This structure contains information related to a specific motor driver channel.
 * It includes:
 * - The handle to the HAL timer object used for PWM generation.
 * - The pins and ports associated with the pins used by the driver.
 * - The channels on the timer associated with the PWM pins.
 * - Any other information needed for the above.
 */
struct {
    int32_t duty_cycle;          /**< Object for duty cycle */
    uint32_t channel1;           /**< Object for channel 1 */
    uint32_t channel2;           /**< Object for channel 2 */
    TIM_HandleTypeDef* hal_tim;  /**< Timer objects */
} typedef MotDriv;

/**
 * @brief Enables the motor on the specified channel.
 *
 * This function starts the PWM signal on the given channel to enable the motor.
 *
 * @param motor Pointer to the motor driver structure.
 * @param channel The channel to enable (1 or 2).
 */
void mot_enable(MotDriv* motor, int32_t channel);

/**
 * @brief Disables the motor on the specified channel.
 *
 * This function stops the PWM signal on the given channel to disable the motor.
 *
 * @param motor Pointer to the motor driver structure.
 * @param channel The channel to disable (1 or 2).
 */
void mot_disable(MotDriv* motor, int32_t channel);

/**
 * @brief Sets the duty cycle for the motor.
 *
 * This function adjusts the duty cycle for the motor to move it forward, backward, or stop it.
 *
 * @param motor Pointer to the motor driver structure.
 * @param duty_cycle The desired duty cycle for the motor. Positive values move the motor forward,
 * negative values move it backward, and zero stops the motor.
 */
void set_duty(MotDriv* motor, int32_t duty_cycle);

#endif /* INC_MOTORDRIVER_H_ */
