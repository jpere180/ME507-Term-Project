/**
 * @file motordriver.c
 * @brief Motor driver implementation.
 */

#include "motordriver.h"
#include <stdio.h>
#include <stdint.h>

/**
 * @brief Sets the duty cycle for the motor.
 *
 * This function adjusts the duty cycle for the motor to move it forward, backward, or stop it.
 *
 * @param motor Pointer to the motor driver structure.
 * @param duty_cycle The desired duty cycle for the motor. Positive values move the motor forward,
 * negative values move it backward, and zero stops the motor.
 */
void set_duty(MotDriv* motor, int32_t duty_cycle)
{
    //moving in the forward direction
    if (duty_cycle > 0) {
        __HAL_TIM_SET_COMPARE(motor->hal_tim, motor->channel1, 0);
        __HAL_TIM_SET_COMPARE(motor->hal_tim, motor->channel2, duty_cycle * 4);
    }//backwards direction
    else if (duty_cycle < 0) {
        __HAL_TIM_SET_COMPARE(motor->hal_tim, motor->channel1, -duty_cycle * 4);
        __HAL_TIM_SET_COMPARE(motor->hal_tim, motor->channel2, 0);
    }//braking/stop
    else {
        __HAL_TIM_SET_COMPARE(motor->hal_tim, motor->channel1, 0);
        __HAL_TIM_SET_COMPARE(motor->hal_tim, motor->channel2, 0);
    }
}

/**
 * @brief Enables the motor on the specified channel.
 *
 * This function starts the PWM signal on the given channel to enable the motor.
 *
 * @param motor Pointer to the motor driver structure.
 * @param channel The channel to enable (1 or 2).
 */
void mot_enable(MotDriv* motor, int32_t channel)
{
    //when the function is called turn on the motor to the set duty cycle
    if (channel == 1) {
        HAL_TIM_PWM_Start(motor->hal_tim, motor->channel1);
    }
    if (channel == 2) {
        HAL_TIM_PWM_Start(motor->hal_tim, motor->channel2);
    }
}

/**
 * @brief Disables the motor on the specified channel.
 *
 * This function stops the PWM signal on the given channel to disable the motor.
 *
 * @param motor Pointer to the motor driver structure.
 * @param channel The channel to disable (1 or 2).
 */
void mot_disable(MotDriv* motor, int32_t channel)
{
    //when the function is called turn off the motor by setting the duty cycle to zero
    if (channel == 1) {
        HAL_TIM_PWM_Stop(motor->hal_tim, motor->channel1);
    }
    if (channel == 2) {
        HAL_TIM_PWM_Stop(motor->hal_tim, motor->channel2);
    }
}

/**
 * @note
 * Go forward for motor 1:
 * HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
 *
 * Go backward for motor 1:
 * HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
 *
 * Go forward for motor 2:
 * HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
 *
 * Go backward for motor 2:
 * HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
 */
