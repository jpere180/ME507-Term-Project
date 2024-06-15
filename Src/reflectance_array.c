 /**
 * @file reflectance_array.c
 * @brief Reflectance array source file.
 */

#include "reflectance_array.h"
#include "stm32f4xx_hal.h"

uint32_t cal_max[8] = {1, 1, 1, 1, 1, 1, 1, 1}; /**< Array to store maximum values for calibration. Initialized to small non-zero values. */
uint32_t cal_min[8] = {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF}; /**< Array to store minimum values for calibration. Initialized to large values. */
int pos_weight[8] = {1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000}; /**< Weighted positions for centroid calculation. */
int count_cal = 0; /**< Counter to track the number of calibration cycles. */

/**
  * @brief  Read a single reflectance sensor and calculate the decay time.
  * @param  ref: Pointer to the reflect structure.
  * @param  pin_num: Pin number within the reflectance sensor array (1 to 8).
  * @retval Decay time of the sensor in milliseconds.
  * @note   This function reads the decay time of a specified reflectance sensor,
  *         which is connected to a specified pin, and returns the decay time
  *         in milliseconds.
  */
uint32_t read_sensor(reflect *ref, int pin_num) {
    GPIO_TypeDef *port_read;    // Port to be read
    uint32_t pin_read;          // Pin to be read
    uint32_t meas, decay;       // Variables to determine and store decay time
    int check_st;               // Boolean to hold while state

    switch (pin_num) {
        case 1:
            port_read = ref->portB;
            pin_read = ref->pin1;
            break;
        case 2:
            port_read = ref->portB;
            pin_read = ref->pin2;
            break;
        case 3:
            port_read = ref->portB;
            pin_read = ref->pin3;
            break;
        case 4:
            port_read = ref->portB;
            pin_read = ref->pin4;
            break;
        case 5:
            port_read = ref->portC;
            pin_read = ref->pin5;
            break;
        case 6:
            port_read = ref->portC;
            pin_read = ref->pin6;
            break;
        case 7:
            port_read = ref->portC;
            pin_read = ref->pin7;
            break;
        case 8:
            port_read = ref->portC;
            pin_read = ref->pin8;
            break;
    }

    HAL_GPIO_WritePin(port_read, pin_read, GPIO_PIN_SET); // Set pin high
    HAL_Delay(1); // Delay to ensure LED reaches high state

    HAL_GPIO_DeInit(port_read, pin_read); // Deinitialize pin to let it decay

    GPIO_InitTypeDef GPIO_InitStruct = {0}; // GPIO struct to store changes and make input
    GPIO_InitStruct.Pin = pin_read;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(port_read, &GPIO_InitStruct); // Initialize GPIO as input

    meas = HAL_GetTick(); // Measure beginning of decay
    check_st = 1; // Set boolean true

    while (check_st == 1) {
        GPIO_PinState pinState = HAL_GPIO_ReadPin(port_read, pin_read); // Check if pin state has decayed to low
        if (pinState == GPIO_PIN_RESET) { // If pin has reached reset state, capture measurement of total decay time
            decay = HAL_GetTick() - meas;
            check_st = 0;

            HAL_GPIO_DeInit(port_read, pin_read); // Deinitialize input to turn back into an output in the reset state
            GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
            GPIO_InitStruct.Pull = GPIO_NOPULL;
            GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
            HAL_GPIO_Init(port_read, &GPIO_InitStruct);
            HAL_GPIO_WritePin(port_read, pin_read, GPIO_PIN_RESET);
        }
    }

    return decay; // Return the decay value
}

/**
  * @brief  Calculate the centroid position based on reflectance sensor readings.
  * @param  ref: Pointer to the reflect structure.
  * @retval Centroid position (range from 1000 to 8000).
  * @note   This function reads the reflectance sensor values, calculates the
  *         percentage of the maximum and minimum values, and computes the
  *         weighted average to estimate the centroid position.
  */
float direction(reflect *ref) {
    int readings[8];    // Array to store readings from each sensor
    int span[8];        // Array to store the total span from max to min for each sensor
    float percent[8];   // Array to store percent that each sensor sees
    float multi = 0;     // Sum for numerator
    float div = 0;       // Sum for denominator
    float direction;     // Centroid position

    for (int i = 0; i < 8; i++) {
        span[i] = cal_max[i] - cal_min[i]; // Determine total span of values picked up by sensor
        readings[i] = read_sensor(ref, i + 1); // Read a sensor
        if (readings[i] > cal_max[i]) { // Handle values slightly above or below max and min
            readings[i] = cal_max[i];
        } else if (readings[i] < cal_min[i]) {
            readings[i] = cal_min[i];
        }

        percent[i] = ((float)(readings[i] - cal_min[i]) / span[i]); // Calculate percent of total intensity for each sensor
        multi += percent[i] * pos_weight[i]; // Sum the percent times weight
        div += percent[i]; // Sum percents
    }
    direction = multi / div; // Calculate weighted average
    return direction;
}

/**
  * @brief  Calibrate the reflectance sensor by finding max and min values.
  * @param  ref: Pointer to the reflect structure.
  * @retval None
  * @note   This function calibrates the reflectance sensor by taking several
  *         readings to determine the maximum and minimum values for each sensor.
  *         It counts the number of calibration cycles for debugging purposes.
  *         This function assumes the sensor is moved back and forth over a black
  *         line and a surface to calibrate properly.
  */
void calibrate(reflect *ref) {
//  for(int i = 0; i < cal_num; i++) { // For loop to perform selected number of calibrations
    count_cal++; // Increment to track number of calibrations performed for debugging
    for (int h = 0; h < 8; h++) { // For loop to take a reading from each sensor in array and compare to max and min values
        int cal_read = read_sensor(ref, h + 1);
        if (cal_read > cal_max[h]) {
            cal_max[h] = cal_read;
        } else if (cal_read < cal_min[h]) {
            cal_min[h] = cal_read;
        }
    }
//  }
}
