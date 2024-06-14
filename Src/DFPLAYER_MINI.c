/*
 * DFPLAYER_MINI.c
 *
 *  Created on: May 16, 2020
 *      Author: controllerstech
 */

#include "stm32f4xx_hal.h"
#include "stdio.h"

extern UART_HandleTypeDef huart1;  ///< External UART handle
#define DF_UART &huart1            ///< DFPlayer UART interface

#define Source      0x02           ///< Source: TF CARD

#define Previous_Key   GPIO_PIN_1  ///< GPIO pin for previous key
#define Previous_Port  GPIOA       ///< GPIO port for previous key
#define Pause_Key      GPIO_PIN_2  ///< GPIO pin for pause key
#define Pause_Port     GPIOA       ///< GPIO port for pause key
#define Next_Key       GPIO_PIN_3  ///< GPIO pin for next key
#define Next_Port      GPIOA       ///< GPIO port for next key

/*************************************** NO CHANGES AFTER THIS *************************************************/

int ispause =0;                    ///< Pause state flag
int isplaying=1;                   ///< Playing state flag

#define Start_Byte 0x7E            ///< Start byte for command
#define End_Byte   0xEF            ///< End byte for command
#define Version    0xFF            ///< Version byte
#define Cmd_Len    0x06            ///< Command length
#define Feedback   0x00            ///< Feedback: 0x01 for feedback, 0x00 for no feedback

/**
 * @brief Sends a command to the DFPlayer module.
 *
 * @param cmd Command byte
 * @param Parameter1 First parameter byte
 * @param Parameter2 Second parameter byte
 */
void Send_cmd (uint8_t cmd, uint8_t Parameter1, uint8_t Parameter2)
{
    uint16_t Checksum = Version + Cmd_Len + cmd + Feedback + Parameter1 + Parameter2;
    Checksum = 0-Checksum;

    uint8_t CmdSequence[10] = { Start_Byte, Version, Cmd_Len, cmd, Feedback, Parameter1, Parameter2, (Checksum>>8)&0x00ff, (Checksum&0x00ff), End_Byte};

    HAL_UART_Transmit(DF_UART, CmdSequence, 10, HAL_MAX_DELAY);
}

/**
 * @brief Plays the first track from the start.
 */
void DF_PlayFromStart(void)
{
    Send_cmd(0x03,0x00,0x01);
    HAL_Delay(200);
}

/**
 * @brief Initializes the DFPlayer module.
 *
 * @param volume Initial volume level
 */
void DF_Init (uint8_t volume)
{
    Send_cmd(0x3F, 0x00, Source);
    HAL_Delay(200);
    Send_cmd(0x06, 0x00, 20);
    HAL_Delay(500);
}

/**
 * @brief Plays the next track.
 */
void DF_Next (void)
{
    Send_cmd(0x01, 0x00, 0x00);
    HAL_Delay(200);
}

/**
 * @brief Pauses playback.
 */
void DF_Pause (void)
{
    Send_cmd(0x0E, 0, 0);
    HAL_Delay(200);
}

/**
 * @brief Plays the previous track.
 */
void DF_Previous (void)
{
    Send_cmd(0x02, 0, 0);
    HAL_Delay(200);
}

/**
 * @brief Resumes playback.
 */
void DF_Playback (void)
{
    Send_cmd(0x0D, 0, 0);
    HAL_Delay(200);
}

/**
 * @brief Checks the state of control keys and performs corresponding actions.
 */
void Check_Key (void)
{
    if (HAL_GPIO_ReadPin(Pause_Port, Pause_Key))
    {
        while (HAL_GPIO_ReadPin(Pause_Port, Pause_Key));
        if (isplaying)
        {
            ispause = 1;
            isplaying = 0;
            DF_Pause();
        }

        else if (ispause)
        {
            isplaying = 1;
            ispause = 0;
            DF_Playback();
        }
    }

    if (HAL_GPIO_ReadPin(Previous_Port, Previous_Key))
    {
        while (HAL_GPIO_ReadPin(Previous_Port, Previous_Key));
        DF_Previous();
    }

    if (HAL_GPIO_ReadPin(Next_Port, Next_Key))
    {
        while (HAL_GPIO_ReadPin(Next_Port, Next_Key));
        DF_Next();
    }
}
