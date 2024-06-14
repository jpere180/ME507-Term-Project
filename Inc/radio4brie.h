/*
 * radio4brie.h
 *
 *  Created on: Jun 10, 2024
 */

#ifndef INC_RADIO4BRIE_H_
#define INC_RADIO4BRIE_H_

#include <stdio.h>
#include <stdint.h>
#include "main.h"

/**
 * @brief Structure to represent a radio input capture.
 *
 * This structure contains the necessary elements for handling timer input capture.
 */
struct {
    TIM_HandleTypeDef *timer; /**< Timer handle used for input capture */
    uint32_t channel;         /**< Timer channel used for input capture */
} typedef radio;

/**
 * @brief Gets the value from the radio input capture.
 *
 * @param radio Pointer to the radio structure.
 * @return uint32_t Value captured by the timer.
 */
uint32_t get(radio* radio);

#endif /* INC_RADIO4BRIE_H_ */
