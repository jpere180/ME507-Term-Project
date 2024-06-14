/**
  ******************************************************************************
  * @file           : motor_driver.c
  * @brief          : Motor driver
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#include "motor_driver2.h"
#include "main.h"

#define Max_PWM 500 /**< Maximum allowable duty cycle for the specified motor. */

/**
  * @brief  Enable the motor with EN pins.
  * @param  motor: Pointer to Motor_Driver structure.
  * @retval None
  */
void enable_motor(Motor_Driver *motor) {
	HAL_GPIO_WritePin(motor->gpio_port_en, motor->gpio_pin_en, GPIO_PIN_SET);
	HAL_TIM_PWM_Start(motor->htim, motor->channel);
}

/**
  * @brief  Disable the motor.
  * @param  motor: Pointer to Motor_Driver structure.
  * @retval None
  */
void disable_motor(Motor_Driver *motor) {
	HAL_GPIO_WritePin(motor->gpio_port_en, motor->gpio_pin_en, GPIO_PIN_RESET);
}

/**
  * @brief  Set the duty cycle for the motor.
  * @param  motor: Pointer to Motor_Driver structure.
  * @param  pulse: Duty cycle value (0 to Max_PWM).
  * @retval None
  */
void set_duty(Motor_Driver *motor, int32_t pulse) {
	/*if (pulse > Max_PWM) {
		pulse = 499;
	} else if (pulse < 0) {
		pulse = 0;
	}*/

	__HAL_TIM_SET_COMPARE(motor->htim, motor->channel, pulse);
}
