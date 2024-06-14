/**
 * @file Ultrasonic.c
 * @brief Ultrasonic implementation.
 */

#include "Ultrasonic.h"
#include <stdio.h>
#include <stdint.h>

/**
  * @brief  Initialize the Ultrasonic sensor.
  * @param  htim: Pointer to a TIM_HandleTypeDef structure that contains the configuration information for TIM Base module.
  * @retval None
  */
void Ultrasonic_Init(TIM_HandleTypeDef* htim)
{
    HAL_TIM_Base_Start(htim);
}

/**
  * @brief  Trigger the Ultrasonic sensor to send a pulse.
  * @param  htim: Pointer to a TIM_HandleTypeDef structure that contains the configuration information for TIM Base module.
  * @param  TRIG_PORT: GPIO Port used for the TRIG pin.
  * @param  TRIG_PIN: GPIO Pin number for the TRIG pin.
  * @retval None
  */
void Ultrasonic_Trigger(TIM_HandleTypeDef* htim, GPIO_TypeDef* TRIG_PORT, uint16_t TRIG_PIN)
{
    // Pull the TRIG pin high
    HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_SET);
    __HAL_TIM_SET_COUNTER(htim, 0);
    // Wait for 10 us
    while(__HAL_TIM_GET_COUNTER(htim) < 10);
    // Pull the TRIG pin low
    HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_RESET);
}

/**
  * @brief  Measure the distance using the Ultrasonic sensor.
  * @param  sensor: Pointer to the Ultrasonic structure.
  * @param  htim: Pointer to a TIM_HandleTypeDef structure that contains the configuration information for TIM Base module.
  * @param  ECHO_PORT: GPIO Port used for the ECHO pin.
  * @param  ECHO_PIN: GPIO Pin number for the ECHO pin.
  * @retval None
  */
void Ultrasonic_Measure(Ultrasonic* sensor, TIM_HandleTypeDef* htim, GPIO_TypeDef* ECHO_PORT, uint16_t ECHO_PIN)
{
    // Used to avoid infinite while loop (for timeout)
    sensor->pMillis = HAL_GetTick();
    // Wait for the echo pin to go high
    while (!(HAL_GPIO_ReadPin(ECHO_PORT, ECHO_PIN)) && sensor->pMillis + 10 > HAL_GetTick());
    sensor->Value1 = __HAL_TIM_GET_COUNTER(htim);

    sensor->pMillis = HAL_GetTick();
    while ((HAL_GPIO_ReadPin(ECHO_PORT, ECHO_PIN)) && sensor->pMillis + 50 > HAL_GetTick());
    sensor->Value2 = __HAL_TIM_GET_COUNTER(htim);

    sensor->Distance = (sensor->Value2 - sensor->Value1) * 0.034 / 2;
    HAL_Delay(50);
}
