/**
  ******************************************************************************
  * @file    rsest.h
  * @author  Piak Electronic Design B.V.
  * @brief   This file is part of the Motor Control SDK.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2025 Piak Electronic Design B.V.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* rsest.h */

#ifndef _RSEST_H_
#define _RSEST_H_

#include "fixpmath.h"

#include <stddef.h>
#include <stdbool.h>

typedef struct _RSEST_Params_
{
    float_t         FullScaleCurrent_A;
    float_t         FullScaleVoltage_V;
    float_t         RatedCelcius;    	/* Temperature @ Rated resistance value (deg C) */
    float_t         AmbientCelcius;		/* Ambient Temperature (deg C) */
    float_t         RsRatedOhm;
    float_t         MotorSkinFactor;	/* Motor skin-factor at choosen injection frequency */
    float_t         F_calculate_Hz;
    float_t         m_copper_kg;		/* Total copper mass for heating estimation */
    float_t         coolingTau_s;
    float_t         correctionGain;
    float_t         injectGain;
} RSEST_Params;

typedef struct _RSEST_Obj_
{
	/* Scaling and conversion factors */
    float_t         Tbackground;                /* s */
    float_t         FullScaleCurrent_A;
    float_t         FullScaleImpedance_OHM;     /* FullScaleVoltage/FullScaleCurrent */
    float_t         FullScaleAdmittance_MHO;    /* FullScaleCurrent/FullScaleVoltage */
    float_t         FullScalePower_3ph_W;      	/* 1.5 * FullScaleCurrent * FullScaleVoltage */
    float_t         oneoverRs_rated_Mho;        /* updated when rated value is changed */

	fixp30_t			dither;

	/* Current vector magnitude */
	fixp30_t			IsquaredLPt;				/**< Current magnitude squared, I^2, per unit current */
	uint16_t		lpShiftI2t;					/**< Low pass filter constant for current magnitude squared. Bit shift filter. */

    /* Power-based resistance estimation */
	fixp30_t			PowerLP;					// Power, low pass filtered  (two phase: 2/3 of total power)
	fixp30_t			IsquaredLP;					// Squared current, Low pass filtered, required for R = P/I^2 estimation.
	uint16_t		lpShiftVI;					// Low pass filter bitshift for 'VI', voltage x current, used in power calculation
    float_t         rs_power_ohm;				// power-based resistance

    /* Decimation of background function */
    uint16_t        decimationCounter;
    bool            doBackground;               // go command to achieve decimation

    bool            flag_enableUpdate;
    bool            current_is_low;

	float_t       	rs_rated_ohm;				//
    float_t       	rs_ohm;						/**< Present estimated resistance */
    FIXP_scaled_t    Rs_pu_fps;                  /* output in pu_fps format */

    float_t       	rs_inject_ohm;				/**< Present estimated resistance plus injection-based correction */

    fixp30_t         rs_inject_pur;              // relative to  Rrated		RJ value 1.0 = rated, should be initial value
    											// with correction active, this value will remain around 1.0

    fixp_t       	correctionGain;
	fixp_t        	injectGain;

	/* Skin factor */
    fixp_t         	SkinFactor;                 // Skinfactor Rac/Rdc normally slightly larger than 1.
    fixp30_t         T_calc;                     // 1/fBackground = backgroundinterval in seconds, used by _setRsBandwidth_Hz()
    fixp30_t         incRateLimit;
    fixp30_t         incRfactor;

     float_t         TerminalPowerWatt;              // total Terminal Power for all three phases

     /* */
     fixp20_t         ratCelcius_pu;                   // ambient temperature (deg C) this is where Rs=Rrated.
     fixp20_t         ambCelcius_pu;
     fixp30_t         ambRfactor;
     fixp30_t         deltaRfactor;                   /* TempRfactor increment */
     fixp30_t         maxRfactor;                     /* Limit outside ZEST operation */
     fixp30_t         TempRfactor;                    /* Active Rs/Rrated value */
     fixp20_t         TempCelcius;                    // approximate temperature guess
     fixp30_t         Thermal_K;
     fixp30_t         Thermal_Leak;
     float_t		 copper_mass_kg;
     float_t		 coolingTau_s;
     float_t		 HeatCoeff;                       /* Heat coefficient (1/K ) */
     float_t		 SpecificHeat;            /* Specific heat capacity (J / kg /K) */

     fixp30_t		inject_term;
     fixp30_t		power_term;
     fixp30_t		leak_term;

    uint32_t        tuningTestCounter;			/* should increase by 1000 counts per second (when flag_enableTuning=true)*/
    bool		    flag_enableTuning;			/**< Enable additional calculations which assist in tuning thermal model */
    bool            flag_tuningSynchronize;
    fixp20_t         tuning_TempCelcius;
    fixp30_t         tuning_deltaRfactor;
    fixp30_t         tuning_TempRfactor;
    fixp30_t         tuning_incRfactor_LP;
} RSEST_Obj;

typedef RSEST_Obj* RSEST_Handle;

extern RSEST_Handle RSEST_init(void *pMemory, const size_t size);

extern void RSEST_setParams(RSEST_Handle handle, const RSEST_Params *pParams);

/* Interrupt function (synchronous with sensorless algorithm) */
extern void RSEST_run(RSEST_Handle handle, const Currents_Iab_t *pIab_pu, const Voltages_Uab_t *pUab_pu);

/* Background function (~1 kHz) now spread out in mainloop */
extern void RSEST_runBackground(RSEST_Handle handle);

extern void RSEST_runBackSlowed(RSEST_Handle handle, const bool injectActive, const fixp30_t CheckRs, const float_t Rsdelta);

/* Getters */

extern bool 		RSEST_getCurrentIsLow(const RSEST_Handle handle);

extern float_t		RSEST_getCopperMass_Kg(const RSEST_Handle handle);

extern float_t		RSEST_getCoolingTau_s(const RSEST_Handle handle);

extern fixp30_t      RSEST_getDeltaRFactor(const RSEST_Handle handle);

extern bool			RSEST_getDoBackGround(const RSEST_Handle handle);

extern float		RSEST_getHeatCoeff(const RSEST_Handle handle);

extern fixp_t		RSEST_getRsInjectGain(const RSEST_Handle handle);

extern float		RSEST_getRsInjectOhm(const RSEST_Handle handle);

extern float		RSEST_getRsOhm(const RSEST_Handle handle);

extern float		RSEST_getRsPowerOhm(const RSEST_Handle handle);

extern FIXP_scaled_t RSEST_getRsRated_fps(const RSEST_Handle handle);

extern FIXP_scaled_t	RSEST_getRs_pu_fps(const RSEST_Handle handle);

extern fixp_t 		RSEST_getSkinFactor(const RSEST_Handle handle);

extern float 		RSEST_getSpecificHeat(const RSEST_Handle handle);

extern fixp20_t 		RSEST_getTempCelcius_pu(const RSEST_Handle handle);

extern fixp20_t 		RSEST_getTuningTempCelcius_pu(const RSEST_Handle handle);

extern bool 		RSEST_getTuningState(RSEST_Handle handle);

extern bool 		RSEST_getTuningSync(RSEST_Handle handle);

/* Setters */

extern void RSEST_setAmbientCelcius_pu(RSEST_Handle handle, const fixp20_t value);

extern void RSEST_setCopperMass_Kg(RSEST_Handle handle, const float_t value);

extern void RSEST_setCoolingTau_s(RSEST_Handle handle, const float_t value);

extern void RSEST_setHeatCoeff(const RSEST_Handle handle, const float_t value);

extern void RSEST_setInjectGain(RSEST_Handle handle, const fixp_t value);

extern void RSEST_setRsOhm(RSEST_Handle handle, const float_t rs_ohm);

extern void RSEST_setRsRatedOhm(RSEST_Handle handle, const float_t value);

extern void RSEST_setRsToRated(RSEST_Handle handle);

extern void RSEST_setRs_pu_fps(RSEST_Handle handle, const FIXP_scaled_t rs_pu_fps);

extern void RSEST_setSkinFactor(RSEST_Handle handle, const float_t value);

extern void RSEST_setSpecificHeat(const RSEST_Handle handle, const float_t value);

extern void RSEST_setTempRfactor(RSEST_Handle handle, const float_t value);

extern void RSEST_setTuning_ON(RSEST_Handle handle, const float_t value);

extern void RSEST_setTuningSync(RSEST_Handle handle, const float_t value);

extern void RSEST_setUpdate_ON(RSEST_Handle handle, const bool value);

#endif /* _RSEST_H_ */

/* end of rsest.h */

/************************ (C) COPYRIGHT 2025 Piak Electronic Design B.V. *****END OF FILE****/
