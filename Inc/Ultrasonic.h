/**
 * @file Ultrasonic.h
 * @brief Ultrasonic header file.
 */

#ifndef INC_ULTRASONIC_H_
#define INC_ULTRASONIC_H_

#include <stdint.h>
#include "stm32f4xx_hal.h"

/**
 * @brief Structure to represent an ultrasonic sensor.
 *
 * This structure contains the necessary elements for handling an ultrasonic sensor.
 */
struct {
    uint32_t pMillis;    /**< Previous millis value */
    uint32_t Value1;     /**< First captured value */
    uint32_t Value2;     /**< Second captured value */
    uint16_t Distance;   /**< Measured distance in cm */
} typedef Ultrasonic;

/**
 * @brief Initializes the ultrasonic sensor.
 *
 * @param htim Timer handle used for the ultrasonic sensor.
 */
void Ultrasonic_Init(TIM_HandleTypeDef* htim);

/**
 * @brief Triggers the ultrasonic sensor to start a measurement.
 *
 * @param htim Timer handle used for the ultrasonic sensor.
 * @param TRIG_PORT GPIO port for the trigger pin.
 * @param TRIG_PIN GPIO pin for the trigger.
 */
void Ultrasonic_Trigger(TIM_HandleTypeDef* htim, GPIO_TypeDef* TRIG_PORT, uint16_t TRIG_PIN);

/**
 * @brief Measures the distance using the ultrasonic sensor.
 *
 * @param sensor Pointer to the Ultrasonic structure.
 * @param htim Timer handle used for the ultrasonic sensor.
 * @param ECHO_PORT GPIO port for the echo pin.
 * @param ECHO_PIN GPIO pin for the echo.
 */
void Ultrasonic_Measure(Ultrasonic* sensor, TIM_HandleTypeDef* htim, GPIO_TypeDef* ECHO_PORT, uint16_t ECHO_PIN);

#endif /* INC_ULTRASONIC_H_ */
