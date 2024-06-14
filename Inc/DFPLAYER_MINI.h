/*
 * DFPLAYER_MINI.h
 *
 *  Created on: May 16, 2020
 *      Author: controllerstech
 */

#ifndef INC_DFPLAYER_MINI_H_
#define INC_DFPLAYER_MINI_H_

/**
 * @brief Sends a command to the DFPlayer module.
 *
 * @param cmd Command byte
 * @param Parameter1 First parameter byte
 * @param Parameter2 Second parameter byte
 */
void Send_cmd (uint8_t cmd, uint8_t Parameter1, uint8_t Parameter2);

/**
 * @brief Plays the first track from the start.
 */
void DF_PlayFromStart(void);

/**
 * @brief Initializes the DFPlayer module.
 *
 * @param volume Initial volume level
 */
void DF_Init (uint8_t volume);

/**
 * @brief Plays the next track.
 */
void DF_Next (void);

/**
 * @brief Pauses playback.
 */
void DF_Pause (void);

/**
 * @brief Plays the previous track.
 */
void DF_Previous (void);

/**
 * @brief Resumes playback.
 */
void DF_Playback (void);

/**
 * @brief Checks the state of control keys and performs corresponding actions.
 */
void Check_Key (void);

#endif /* INC_DFPLAYER_MINI_H_ */
