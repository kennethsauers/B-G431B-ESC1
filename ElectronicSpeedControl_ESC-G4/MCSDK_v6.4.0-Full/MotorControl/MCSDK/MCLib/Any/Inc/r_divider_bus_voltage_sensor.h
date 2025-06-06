/**
  ******************************************************************************
  * @file    r_divider_bus_voltage_sensor.h
  * @author  Motor Control SDK Team, ST Microelectronics
  * @brief   This file contains all definitions and functions prototypes for the
  *          Resistor Divider Bus Voltage Sensor component of the Motor Control SDK.
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
  * @ingroup RDividerBusVoltageSensor
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef RDIVIDER_BUSVOLTAGESENSOR_H
#define RDIVIDER_BUSVOLTAGESENSOR_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "regular_conversion_manager.h"
#include "bus_voltage_sensor.h"

/** @addtogroup MCSDK
  * @{
  */

/** @addtogroup BusVoltageSensor
  * @{
  */

/** @addtogroup RDividerBusVoltageSensor
  * @{
  */


/**
  * @brief  Rdivider class parameters definition
  */
typedef struct
{
  BusVoltageSensor_Handle_t _Super;          /*!< Bus voltage sensor component handle. */
  uint16_t       OverVoltageThreshold;       /*!< It represents the over voltage protection
                                                  intervention threshold. To be expressed
                                                  in digital value through formula:
                                                  hOverVoltageThreshold (digital value) =
                                                  Over Voltage Threshold (V) * 65536
                                                  / hConversionFactor */
  uint16_t       OverVoltageThresholdLow;    /*!< It represents the over voltage protection
                                                  intervention threshold low level for hysteresis
                                                  feature when "switch on brake resistor" is used.
                                                  To be expressed in digital value through formula:
                                                  hOverVoltageThresholdLow (digital value) =
                                                  Over Voltage Threshold Low(V) * 65536
                                                  / hConversionFactor */
  bool           OverVoltageHysteresisUpDir; /*!< "Switch on brake resistor" hysteresis direction. */
  uint16_t       UnderVoltageThreshold;      /*!< It represents the under voltage protection
                                                  intervention threshold. To be expressed
                                                  in digital value through formula:
                                                  hUnderVoltageThreshold (digital value)=
                                                  Under Voltage Threshold (V) * 65536
                                                  / hConversionFactor */

} RDivider_Handle_t;

/* Exported functions ------------------------------------------------------- */
void RVBS_Init(RDivider_Handle_t *pHandle);
uint16_t RVBS_CalcAvVbus(RDivider_Handle_t *pHandle, uint16_t rawValue);

/**
  * @}
  */

/**
  * @}
  */

/** @} */

#ifdef __cplusplus
}
#endif /* __cpluplus */

#endif /* RDIVIDER_BUSVOLTAGESENSOR_H */

/************************ (C) COPYRIGHT 2025 STMicroelectronics *****END OF FILE****/

