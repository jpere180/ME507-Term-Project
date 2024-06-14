/**
 * @file motor_driver.h
 * @brief
 * Header for Motor Driver can enable and disable the desired motor. The duty cycle can also be specified once motor is enabled.
 */

#ifndef _MOTOR_DRIVER2_H_
#define _MOTOR_DRIVER2_H_

#include <stdio.h>
#include <stdint.h>
#include "stm32f4xx_hal.h"

/**
 * @brief Motor object data structure.
 *
 * Create all the variables needed for the function in a struct.
 */
typedef struct {
    TIM_HandleTypeDef *htim;    ///< Specify the timer for the motor
    uint32_t channel;           ///< Specify the channel for the motor
    GPIO_TypeDef *gpio_port_en; ///< Specify GPIO port for enable
    uint16_t gpio_pin_en;       ///< Specify GPIO pin for enable
} Motor_Driver;

/**
 * @brief Enables the motor.
 *
 * @param motor Pointer to the Motor_Driver structure.
 */
void enable_motor(Motor_Driver *motor);

/**
 * @brief Disables the motor.
 *
 * @param motor Pointer to the Motor_Driver structure.
 */
void disable_motor(Motor_Driver *motor);

/*void duty_enable(Motor_Driver *motor);
void duty_disable(Motor_Driver *motor);*/

/**
 * @brief Sets the duty cycle for the motor.
 *
 * @param motor Pointer to the Motor_Driver structure.
 * @param pulse Pulse width value to set the duty cycle.
 */
void set_duty(Motor_Driver *motor, int32_t pulse);

#endif /* _MOTOR_DRIVER2_H_ */
