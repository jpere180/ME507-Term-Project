/**
 * @file reflectance_array.h
 * @brief Reflectance array header file.
 */


#ifndef INC_REFLECTANCE_ARRAY_H_
#define INC_REFLECTANCE_ARRAY_H_

#include "stm32f4xx_hal.h"

/**
 * @brief Structure to represent a reflectance sensor array.
 *
 * This structure contains the necessary elements for handling a reflectance sensor array.
 */
struct {
    GPIO_TypeDef *portB; /**< GPIO port for sensor B */
    GPIO_TypeDef *portC; /**< GPIO port for sensor C */
    uint32_t pin1;       /**< GPIO pin for sensor 1 */
    uint32_t pin2;       /**< GPIO pin for sensor 2 */
    uint32_t pin3;       /**< GPIO pin for sensor 3 */
    uint32_t pin4;       /**< GPIO pin for sensor 4 */
    uint32_t pin5;       /**< GPIO pin for sensor 5 */
    uint32_t pin6;       /**< GPIO pin for sensor 6 */
    uint32_t pin7;       /**< GPIO pin for sensor 7 */
    uint32_t pin8;       /**< GPIO pin for sensor 8 */
    TIM_HandleTypeDef *timer; /**< Timer handle used for measurements */
} typedef reflect;

/**
 * @brief Calibrates the reflectance sensor array.
 *
 * @param ref Pointer to the reflect structure.
 */
void calibrate(reflect* ref); //,int cal_num);

/**
 * @brief Determines the direction based on sensor readings.
 *
 * @param ref Pointer to the reflect structure.
 * @return float Direction value.
 */
float direction(reflect* ref);

/**
 * @brief Reads the value from a specified sensor.
 *
 * @param ref Pointer to the reflect structure.
 * @param pin_num Pin number to read from.
 * @return uint32_t Sensor reading value.
 */
uint32_t read_sensor(reflect* ref, int pin_num);

#endif /* INC_REFLECTANCE_ARRAY_H_ */
