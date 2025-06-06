/**
  ******************************************************************************
  * @file    virtual_speed_sensor.h
  * @author  Motor Control SDK Team, ST Microelectronics
  * @brief   This file contains all definitions and functions prototypes for the
  *          Virtual Speed Sensor component of the Motor Control SDK.
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
  * @ingroup VirtualSpeedSensor
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef VIRTUALSPEEDSENSOR_H
#define VIRTUALSPEEDSENSOR_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "speed_pos_fdbk.h"

/** @addtogroup MCSDK
  * @{
  */

/** @addtogroup SpeednPosFdbk
  * @{
  */

/** @addtogroup VirtualSpeedSensor
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/**
  * @brief  This structure is used to handle an instance of the Virtual Speed
  *         sensor component.
  */
typedef struct
{
  SpeednPosFdbk_Handle_t _Super;
  int32_t wElAccDppP32;              /*!< Delta electrical speed expressed in [dpp](measurement_units.md) per speed
                                          sampling period to be applied each time is called
                                          SPD_calcAvrgMecSpeedUnit multiplied by scaling
                                          factor of 65536. */
  int32_t wElSpeedDpp32;             /*!< Electrical speed expressed in [dpp](measurement_units.md) multiplied by
                                          scaling factor 65536. */
  uint16_t hRemainingStep;           /*!< Number of steps remaining to reach the final
                                          speed. */
  int16_t hFinalMecSpeedUnit;        /*!< Backup of hFinalMecSpeedUnit to be applied in
                                          the last step.*/
  bool bTransitionStarted;           /*!< Retaining information about started Transition status.*/
  bool bTransitionEnded;             /*!< Retaining information about ended transition status.*/
  int16_t hTransitionRemainingSteps; /*!< Number of steps remaining to end
                                          transition from Virtual Speed Sensor module to other speed sensor modules */
  int16_t hElAngleAccu;              /*!< Electrical angle accumulator */
  bool bTransitionLocked;            /*!< Transition acceleration started */
  bool bCopyObserver;                /*!< Command to set Virtual Speed Sensor module output same as state observer */

  uint16_t hSpeedSamplingFreqHz;     /*!< Frequency (Hz) at which motor speed is to
                                          be computed. It must be equal to the frequency
                                          at which function SPD_calcAvrgMecSpeedUnit
                                          is called. */
  int16_t hTransitionSteps;          /*< Number of steps to perform the transition phase
                                         from from Virtual Speed Sensor module to other speed sensor modules.
                                         If the Transition PHase should last TPH milliseconds, and the FOC Execution
                                         Frequency is set to FEF kHz, then #hTransitionSteps = TPH * FEF */
} VirtualSpeedSensor_Handle_t;

/* It initializes the Virtual Speed Sensor component */
void VSS_Init(VirtualSpeedSensor_Handle_t *pHandle);

/* It clears Virtual Speed Sensor by re-initializing private variables*/
void VSS_Clear(VirtualSpeedSensor_Handle_t *pHandle);

/* It compute a theorical speed and update the theorical electrical angle */
int16_t VSS_CalcElAngle(VirtualSpeedSensor_Handle_t *pHandle, int16_t *pInputVars_str);

/* Computes the rotor average theoretical mechanical speed in the unit defined by SPEED_UNIT and
 * returns it in pMecSpeedUnit */
bool VSS_CalcAvrgMecSpeedUnit(VirtualSpeedSensor_Handle_t *pHandle, int16_t *hMecSpeedUnit);

/* Set the mechanical acceleration of virtual sensor */
void  VSS_SetMecAcceleration(VirtualSpeedSensor_Handle_t *pHandle, int16_t  hFinalMecSpeedUnit, uint16_t hDurationms);
/* Checks if the ramp executed after a VSPD_SetMecAcceleration command has been completed */
bool VSS_RampCompleted(VirtualSpeedSensor_Handle_t *pHandle);

/* Get the final speed of last setled ramp of virtual sensor expressed in 0.1Hz */
int16_t  VSS_GetLastRampFinalSpeed(VirtualSpeedSensor_Handle_t *pHandle);

/* Set the command to Start the transition phase from VirtualSpeedSensor to other SpeedSensor */
bool VSS_SetStartTransition(VirtualSpeedSensor_Handle_t *pHandle, bool bCommand);

/* Return the status of the transition phase */
bool VSS_IsTransitionOngoing(VirtualSpeedSensor_Handle_t *pHandle);

bool VSS_TransitionEnded(VirtualSpeedSensor_Handle_t *pHandle);

/* It sets istantaneous information on rotor electrical angle */
void VSS_SetElAngle(VirtualSpeedSensor_Handle_t *pHandle, int16_t hElAngle);

/**
  * @brief  Sets instantaneous information on VSS mechanical and electrical angle.
  * @param  pHandle: handler of the current instance of the VirtualSpeedSensor component.
  * @param  hMecAngle: instantaneous measure of rotor mechanical angle.
  * @retval none
  *
  * - Called during @ref RevUpCtrl "Rev-Up Control" and
  * @ref EncAlignCtrl "Encoder Alignment Controller procedure" initialization.
  */
static inline void VSS_SetMecAngle(VirtualSpeedSensor_Handle_t *pHandle, int16_t hMecAngle)
{
#ifdef NULL_PTR_CHECK_VIR_SPD_SEN
  if (MC_NULL == pHandle)
  {
    /* Nothing to do */
  }
  else
  {
#endif
    pHandle->hElAngleAccu = hMecAngle;
    pHandle->_Super.hMecAngle = pHandle->hElAngleAccu / ((int16_t)pHandle->_Super.bElToMecRatio);
    pHandle->_Super.hElAngle = hMecAngle;
#ifdef NULL_PTR_CHECK_VIR_SPD_SEN
  }
#endif
}

/**
  * @brief  Sets instantaneous information on rotor electrical angle same as copied by state observer.
  * @param  pHandle: handler of the current instance of the VirtualSpeedSensor component.
  * @retval none
  *
  * - Not used into current implementation.
  */
static inline void VSS_SetCopyObserver(VirtualSpeedSensor_Handle_t *pHandle)
{
#ifdef NULL_PTR_CHECK_VIR_SPD_SEN
  if (MC_NULL == pHandle)
  {
    /* Nothing to do */
  }
  else
  {
#endif
    pHandle->bCopyObserver = true;
#ifdef NULL_PTR_CHECK_VIR_SPD_SEN
  }
#endif
}

/** @} */
/** @} */
/** @} */

#ifdef __cplusplus
}
#endif /* __cpluplus */

#endif /* VIRTUALSPEEDSENSOR_H */

/************************ (C) COPYRIGHT 2025 STMicroelectronics *****END OF FILE****/
