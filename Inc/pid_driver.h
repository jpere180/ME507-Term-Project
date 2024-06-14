/**
 * @file motor_driver.h
 * @brief
 * Header for Motor Driver can enable and disable the desired motor. The duty cycle can also be specified once motor is enabled.
 */

#ifndef _PID_DRIVER_H_
#define _PID_DRIVER_H_

#include <stdio.h>
#include <stdint.h>
#include "stm32f4xx_hal.h"

/**
 * @brief PID Controller data structure.
 *
 * This structure contains all necessary variables for PID control.
 */
typedef struct
{
    float p_gain;            ///< Proportional gain
    float i_gain;            ///< Integral gain
    float d_gain;            ///< Derivative gain
    int16_t last_error;      ///< Last error value
    int32_t error_integral;  ///< Integral of the error
    int16_t output;          ///< PID controller output
} PID_Driver;

/**
 * @brief Sets the PID gains.
 *
 * @param pid_instance Pointer to the PID_Driver structure.
 * @param p Proportional gain
 * @param i Integral gain
 * @param d Derivative gain
 */
void set_pid_gain(PID_Driver *pid_instance, float p, float i, float d);

/**
 * @brief Applies the PID algorithm.
 *
 * @param pid_instance Pointer to the PID_Driver structure.
 * @param input_error Current error value
 * @param sampling_rate Sampling rate for the PID calculations
 */
void apply_pid(PID_Driver *pid_instance, int16_t input_error, uint16_t sampling_rate);

#endif /* _PID_DRIVER_H_ */
