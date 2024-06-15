/**
 * @file DFPLAYER_MINI.h
 * @brief MP3 Header File.
 */

#ifndef INC_DFPLAYER_MINI_H_
#define INC_DFPLAYER_MINI_H_

#include <stdint.h>

/**
 * @brief Sends a command to the DFPlayer module.
 *
 * This function sends a command byte along with two parameter bytes
 * to the DFPlayer module to control its behavior.
 *
 * @param cmd Command byte to send to the DFPlayer module.
 * @param Parameter1 First parameter byte to send with the command.
 * @param Parameter2 Second parameter byte to send with the command.
 */
void Send_cmd (uint8_t cmd, uint8_t Parameter1, uint8_t Parameter2);

/**
 * @brief Plays the first track from the start.
 *
 * This function sends a command to the DFPlayer module to start
 * playing the first track from the beginning.
 */
void DF_PlayFromStart(void);

/**
 * @brief Initializes the DFPlayer module.
 *
 * This function initializes the DFPlayer module with a specified
 * volume level.
 *
 * @param volume Initial volume level (0-30).
 */
void DF_Init (uint8_t volume);

/**
 * @brief Plays the next track.
 *
 * This function sends a command to the DFPlayer module to skip to
 * the next track in the playlist.
 */
void DF_Next (void);

/**
 * @brief Pauses playback.
 *
 * This function sends a command to the DFPlayer module to pause
 * the current playback.
 */
void DF_Pause (void);

/**
 * @brief Plays the previous track.
 *
 * This function sends a command to the DFPlayer module to skip to
 * the previous track in the playlist.
 */
void DF_Previous (void);

/**
 * @brief Resumes playback.
 *
 * This function sends a command to the DFPlayer module to resume
 * the current playback.
 */
void DF_Playback (void);

/**
 * @brief Checks the state of control keys and performs corresponding actions.
 *
 * This function checks the state of control keys (e.g., play, pause, next, previous)
 * and performs the corresponding actions on the DFPlayer module.
 */
void Check_Key (void);

#endif /* INC_DFPLAYER_MINI_H_ */
