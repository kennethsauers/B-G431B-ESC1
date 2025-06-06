/**
  ******************************************************************************
  * @file    enc_align_ctrl.h
  * @author  Motor Control SDK Team, ST Microelectronics
  * @brief   This file contains all definitions and functions prototypes for the
  *          Encoder Alignment Control component of the Motor Control SDK.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  * @ingroup EncAlignCtrl
  */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef ENCALIGNCTRLCLASS_H
#define ENCALIGNCTRLCLASS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "mc_type.h"
#include "speed_torq_ctrl.h"
#include "virtual_speed_sensor.h"
#include "encoder_speed_pos_fdbk.h"

/** @addtogroup MCSDK
  * @{
  */

/** @addtogroup EncAlignCtrl
  * @{
  */

/**
  * @brief  This structure is used to handle an instance of  EncAlignCtrl component
  */

typedef struct
{
  SpeednTorqCtrl_Handle_t *pSTC;        /*!< Speed and torque controller object used by  EAC. */
  VirtualSpeedSensor_Handle_t *pVSS;    /*!< Virtual speed sensor object used by EAC. */
  ENCODER_Handle_t *pENC;               /*!< Encoder object used by EAC. */
  uint16_t hRemainingTicks;             /*!< Number of tick events remaining to complete the alignment. */
  bool EncAligned;                      /*!< This flag is true if the encoder has been aligned at least once, false if
                                             has not. */
  bool EncRestart;                      /*!< This flag is used to force a restart of the motor after the encoder
                                             alignment. It is true if a restart is programmed otherwise, it is false*/
  uint16_t hEACFrequencyHz;             /*!< EAC_Exec() function calling frequency, in Hz. */
  int16_t hFinalTorque;                 /*!< Motor torque reference imposed by STC at the end of programmed alignment.
                                             This value actually is the Iq current expressed in digit. */
  int16_t hElAngle;                     /*!< Electrical angle of programmed alignment expressed in s16degrees
                                             [(rotor angle unit)](measurement_units.md). */
  uint16_t hDurationms;                 /*!< Duration of the programmed alignment expressed in milliseconds.*/
  uint8_t bElToMecRatio;                /*!< Coefficient used to transform electrical to mechanical quantities and
                                             vice-versa. It usually coincides with motor pole pairs number. */
} EncAlign_Handle_t;


/* Exported functions ------------------------------------------------------- */

/*  Function used to initialize an instance of the EncAlignCtrl component */
void EAC_Init(EncAlign_Handle_t *pHandle, SpeednTorqCtrl_Handle_t *pSTC, VirtualSpeedSensor_Handle_t *pVSS,
              ENCODER_Handle_t *pENC);

/* Function used to start the encoder alignment procedure */
void EAC_StartAlignment(EncAlign_Handle_t *pHandle);

/* Function used to execute the encoder alignment controller */
bool EAC_Exec(EncAlign_Handle_t *pHandle);

/* Returns true if a restart after an encoder alignment has been requested */
bool EAC_GetRestartState(EncAlign_Handle_t *pHandle);

/**
  * @brief  It sets handler ENCrestart variable according to restart parameter
  * @param  pHandle: handler of the current instance of the EncAlignCtrl component.
  * @param  restart: Equal to true if a restart is programmed else false.
  */
static inline void EAC_SetRestartState(EncAlign_Handle_t *pHandle, bool restart)
{
#ifdef NULL_PTR_CHECK_ENC_ALI_CTRL
  if (NULL  == pHandle)
  {
    /* Nothing to do */
  }
  else
  {
#endif
    pHandle->EncRestart = restart;
#ifdef NULL_PTR_CHECK_ENC_ALI_CTRL
  }
#endif
}

/**
  * @brief  It returns true if the encoder has been aligned at least
  *         one time, false if hasn't been never aligned.
  * @param  pHandle: handler of the current instance of the EncAlignCtrl component.
  * @retval bool Encoder Aligned flag
  */
static inline bool EAC_IsAligned(EncAlign_Handle_t *pHandle)
{
#ifdef NULL_PTR_CHECK_ENC_ALI_CTRL
  return ((NULL == pHandle) ? false : pHandle->EncAligned);
#else
  return (pHandle->EncAligned);
#endif
}

/**
  * @}
  */

/**
  * @}
  */
#ifdef __cplusplus
}
#endif /* __cpluplus */

#endif /* ENCALIGNCTRLCLASS_H */

/******************* (C) COPYRIGHT 2025 STMicroelectronics *****END OF FILE****/
