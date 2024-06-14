/**
  ******************************************************************************
  * @file           : pid_driver.c
  * @brief          : PID driver
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

#include <pid_driver.h>

#define INTEGRAL_GAIN_MAX  2000000 /**< Maximum allowable value for the integral gain. */
#define PID_MAX  8000 /**< Maximum allowable output value from the PID controller. */

/**
  * @brief  Set the PID gains.
  * @param  pid_instance: Pointer to PID_Driver structure.
  * @param  p: Proportional gain.
  * @param  i: Integral gain.
  * @param  d: Derivative gain.
  * @retval None
  */
void set_pid_gain(PID_Driver *pid_instance, float p, float i, float d)
{
    pid_instance->p_gain = p;
    pid_instance->i_gain = i;
    pid_instance->d_gain = d;
}

/**
  * @brief  Reset the PID gains and integral error.
  * @param  pid_instance: Pointer to PID_Driver structure.
  * @retval None
  */
void reset_pid(PID_Driver *pid_instance)
{
    pid_instance->p_gain = 0;
    pid_instance->i_gain = 0;
    pid_instance->d_gain = 0;
    pid_instance->error_integral = 0;
}

/**
  * @brief  Apply the PID algorithm to compute the control output.
  * @param  pid_instance: Pointer to PID_Driver structure.
  * @param  input_error: Current error (desired - measured).
  * @param  sampling_rate: Rate at which the PID algorithm is executed.
  * @retval None
  */
void apply_pid(PID_Driver *pid_instance, int16_t input_error, uint16_t sampling_rate)
{
    pid_instance->error_integral += input_error;

    // Saturation block for integral
    if (pid_instance->error_integral > INTEGRAL_GAIN_MAX)
    {
        pid_instance->error_integral = INTEGRAL_GAIN_MAX;
    }

    if (pid_instance->error_integral < -INTEGRAL_GAIN_MAX)
    {
        pid_instance->error_integral = -INTEGRAL_GAIN_MAX;
    }

    // Compute PID output
    pid_instance->output = pid_instance->p_gain * input_error +
                           pid_instance->i_gain * (pid_instance->error_integral) / sampling_rate +
                           pid_instance->d_gain * sampling_rate * (input_error - pid_instance->last_error);

    // Output saturation
    if (pid_instance->output >= PID_MAX)
    {
        pid_instance->output = PID_MAX;
    }

    if (pid_instance->output <= -PID_MAX)
    {
        pid_instance->output = -PID_MAX;
    }

    // Update old error value
    pid_instance->last_error = input_error;
}
