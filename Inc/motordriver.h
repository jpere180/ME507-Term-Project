/*
 * motordriver.h
 */

#ifndef INC_MOTORDRIVER_H_
#define INC_MOTORDRIVER_H_

#include <stdio.h>
#include <stdint.h>
#include "stm32l4xx_hal.h"

//Include information to a specific motor driver channel:
//1. The handle to the HAL timer object used for PWM generation
//2. The pins and ports associated with the pins used by the driver
//3. What channels on the timer are associated with the PWM pins
//4. Any other information needed for the three above^^^

// Motor object data structure
struct {
	//object for duty_cycle
	int32_t duty_cycle;
	//making the channel an object;
	uint32_t channel1;
	//making the channel an object;
	uint32_t channel2;
	//Timer objects
	TIM_HandleTypeDef* hal_tim;
}

typedef MotDriv;

//Prototype to enable motor
void mot_enable(MotDriv* motor, int32_t channel);
//Prototype to disable motor
void mot_disable(MotDriv* motor,int32_t channel);
// Prototype for motor object "method"
void set_duty(MotDriv* motor, int32_t duty_cycle);

#endif /* INC_MOTORDRIVER_H_ */
