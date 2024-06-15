/**
 * @file radio4brie.c
 * @brief Radio Driver.
 */

#include "radio4brie.h"
//comment
int rising = 1;         /**< Keeps track of edges for channel 1 */
uint32_t cap = 1500;    /**< Initializes PWM capture for channel 1 at neutral value (1500) */
uint32_t rise;  /**< Track ticks corresponding to rise edges */
uint32_t fall;  /**< Track ticks corresponding to fall edges */


/**
  * @brief  Get the PWM capture value for channel
  * @param  radio: Pointer to the radio structure
  * @retval Captured PWM value (range from 1000 to 2000)
  * @note   This function reads the captured PWM value for channel 1 and
  *         processes it to determine the pulse width.
  */
uint32_t get(radio *radio) {
    uint32_t value = HAL_TIM_ReadCapturedValue(radio->timer, radio->channel);	// read captured values

    if (rising) {		// capture first edge before moving to next
        rise = value;
        rising = 0;
    } else {			// once second edge is captured perform operations
        fall = value;
        rising = 1;

        if (fall > rise) {		// if fall value is larger than rise value simply subtract
            cap = fall - rise;
        } else {
            cap = (0xFFFF - rise) + fall;	// performs operation to handle when overflow occurs
        }

                if (cap >15250 && cap <16750){
                	// since doing in direct mode intial edge capture may not be rising as expected
                	// address by doing math in order to convert width- to desired pulse width
                	cap -= 14500;	// first subtract to get within 1000-2000 range
                	if (cap>1500){	// because -width is being measured values must be inversed to the other side of 1500 to get true pulse width
                		cap = cap - (2*(cap-1500));
                	}else if (cap<1500){
                		cap = cap + (2*(1500-cap));
                	}
                }else if (cap < 800 || cap > 2200) {	// for values out of expected range (errors)
                    cap = 1500; // if remote value is out of expected range set to neutral value
                }
        }
    return cap;
    }
