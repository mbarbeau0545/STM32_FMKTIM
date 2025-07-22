/*********************************************************************
 * @file        FMKTIM_ConfigSpecific.c
 * @brief       Template_BriefDescription.
 * @note        TemplateDetailsDescription.\n
 *
 * @author      xxxxxx
 * @date        jj/mm/yyyy
 * @version     1.0
 */






// ********************************************************************
// *                      Includes
// ********************************************************************
#include <math.h>
#include "FMKTIM_ConfigSpecific.h"
#include "../FMKCFG_ConfigFiles/FMKCPU_ConfigPublic.h"

// ********************************************************************
// *                      Defines
// ********************************************************************
#define FMKTIM_MAX_LOOP_DECREASING ((t_uint8)30)
#define FMKTIM_ARR_DECREASING_VAL_16B ((t_uint32)2000)
#define FMKTIM_ARR_DECREASING_VAL_32B ((t_uint32)20000)

// ********************************************************************
// *                      Types
// ********************************************************************
//-----------------------------ENUM TYPES-----------------------------//
/* CAUTION : Automatic generated code section for Enum: Start */

/* CAUTION : Automatic generated code section for Enum: End */

//-----------------------------TYPEDEF TYPES---------------------------//
//-----------------------------STRUCT TYPES---------------------------//
/* CAUTION : Automatic generated code section for Structure: Start */

/* CAUTION : Automatic generated code section for Structure: End */
// ********************************************************************
// *                      Prototypes
// ********************************************************************
	
// ********************************************************************
// *                      Variables
// ********************************************************************
/* CAUTION : Automatic generated code section for Timer Configuration: Start */


/* CAUTION : Automatic generated code section for Timer Configuration: End */
//********************************************************************************
//                      Local functions - Prototypes
//********************************************************************************

//********************************************************************************
//                      Public functions - Prototypes
//********************************************************************************

/*********************************
 * s_FMKTIM_Timer_GetPrescaler
 *********************************/
static inline t_float32 s_FMKTIM_Timer_GetPrescaler(t_uint32 osc_valMhz, t_uint32 ARRVal, t_float32 freqPwm) {
    return (t_float32)(((t_float32)osc_valMhz * CST_MHZ_TO_HZ) / (((t_float32)ARRVal + 1) * (t_float32)freqPwm));
}
/*********************************
 * s_FMKTIM_Timer_GetArrRegister
 *********************************/
static inline t_float32 s_FMKTIM_Timer_GetArrRegister(t_uint32 osc_valMhz, t_uint32 f_Prescaler_u32, t_float32 freqPwm) {
    return (t_float32)((((t_float32)osc_valMhz * CST_MHZ_TO_HZ) / (((t_float32)f_Prescaler_u32 + 1) * (t_float32)freqPwm)) - (t_float32)1);
}
/*********************************
 * s_FMKTIM_Timer_GeFreqPwm
 *********************************/
static inline t_float32  s_FMKTIM_Timer_GetFreqPwm(t_uint32 osc_valMhz, t_uint32 f_Prescaler_u32, t_uint32 f_ARRVal) {
    return (t_float32)(((t_float32)osc_valMhz * CST_MHZ_TO_HZ) / (((t_float32)f_Prescaler_u32 + 1) * ((t_float32)(f_ARRVal + 1))));
}
/*********************************
 * s_FMKTIM_DecomposeFloat
 *********************************/
// Fonction inline pour décomposer une valeur
static inline void  s_FMKTIM_DecomposeFloat(t_float32 f_value_f32,t_uint32 *f_partieEntiere_pu32, t_float32 *f_fractionleft_pf32 ) {

    *f_partieEntiere_pu32 = (t_uint32)((f_value_f32 >= 0) ? floorf(f_value_f32 + 0.5f) : ceilf(f_value_f32 - 0.5f)); // Arrondi vers l'entier le plus proche
    *f_fractionleft_pf32 = (t_float32)(f_value_f32 - (t_float32)(*f_partieEntiere_pu32)); // Partie fractionnelle
}
//****************************************************************************
//                      Public functions - Implementation
//********************************************************************************
/*********************************
 * FMKTIM_GetPwmTimerInitParam
 *********************************/
// 1 to 2 ms at 160MHz
t_eReturnCode FMKTIM_GetPwmTimerInitParam(  t_eFMKCPU_ClockPort f_timClock_e,
                                            t_uint32    f_timerFreqMhz_u32,
                                            t_float32  f_PwmFreq_f32,
                                            t_uint32 * f_bspTimARR_pu32,
                                            t_uint32 * f_bspTimPrescaler_pu32)
{
    t_eReturnCode Ret_e = RC_OK;
    t_uint32 maxARRValue_u32 =  (t_uint32)0;
    t_float32 targetARRValue_f32 = (t_float32)0;
    t_float32 freqMaxSupported_f32 = (t_uint16)0.0f;
    t_float32 prescalerTheo_f32 = (t_float32)0.0f;
    t_float32 deltaPrescaler_f32 = (t_float32)0.0f;
    //t_float32 freqPwmCompute_f32 = (t_float32)0.0f;
    //t_float32 deltaFreqPwm_f32 = (t_float32)0;

    t_uint32 maxNumberBit_u32   = (t_uint32)0;
    t_uint32 decreasingValue_u32 = (t_uint32)FMKTIM_ARR_DECREASING_VAL_16B;
    t_uint32 realPrescaler_u32 = (t_uint32)0;
    t_uint32 realARR_u32 = (t_uint32)0;
    
    t_uint8 LLI_u8;

    if((f_bspTimPrescaler_pu32 == (t_uint32 *)NULL)
    || (f_bspTimARR_pu32       == (t_uint32 *)NULL))
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }
    if(f_timClock_e >= FMKCPU_RCC_CLK_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
    }
    if(Ret_e == RC_OK)
    {
        // Timer depend on APB1 or APB2, if these clock were divided per 2 or more,
        // Hardware multiply by 2 the core freqency of the timer
        // In other word 
        if(FMKTIM_IS_32B_TIMER(f_timClock_e) == True)
        {
            maxNumberBit_u32 = (t_uint32)(CST_MAX_UINT_32BIT);
            maxARRValue_u32 = (t_uint32)(FMKTIM_TIMER_PWM_ARR_TARGET_32_BIT); // 32-bit target
            decreasingValue_u32 = FMKTIM_ARR_DECREASING_VAL_32B;
        }
        else 
        {   
            maxNumberBit_u32 = (t_uint32)(CST_MAX_UINT_16BIT);
            maxARRValue_u32 = (t_uint32)(FMKTIM_TIMER_PWM_ARR_TARGET_16_BIT); // 16-bit target
            decreasingValue_u32 = FMKTIM_ARR_DECREASING_VAL_16B;
        }
        //---------Know the max frequency supported with max Arr Value---------//
        targetARRValue_f32 = s_FMKTIM_Timer_GetArrRegister((t_uint32)f_timerFreqMhz_u32, (t_uint32)0, f_PwmFreq_f32);
        
        if(targetARRValue_f32 > (t_float32)maxARRValue_u32)
        {
            targetARRValue_f32 = (t_float32)maxARRValue_u32;
        }

        freqMaxSupported_f32 = (t_float32)(((t_float32)f_timerFreqMhz_u32 * CST_MHZ_TO_HZ) / ((t_uint32)targetARRValue_f32 + (t_float32)1));

        if(f_PwmFreq_f32 > (t_float32)freqMaxSupported_f32)
        {
            for(LLI_u8 = (t_uint8)0 ; 
                (LLI_u8 < FMKTIM_MAX_LOOP_DECREASING)
                && (f_PwmFreq_f32 > freqMaxSupported_f32); 
                LLI_u8++)
                {
                    //---------Compute New Max Frequency with new ARR value---------//
                    targetARRValue_f32 -= (t_float32)decreasingValue_u32;
                    freqMaxSupported_f32 = (t_float32)(((t_float32)f_timerFreqMhz_u32 * CST_MHZ_TO_HZ) / ((t_float32)targetARRValue_f32 + (t_float32)1));
                }
                if(LLI_u8 >= (t_uint8)FMKTIM_MAX_LOOP_DECREASING)
                {
                    Ret_e = RC_ERROR_WRONG_RESULT;
                }
        }
        if(Ret_e == RC_OK)
        {
            //---- Prescaler Théorique--------//
            prescalerTheo_f32 = s_FMKTIM_Timer_GetPrescaler((t_uint32)f_timerFreqMhz_u32, targetARRValue_f32, f_PwmFreq_f32);
            //---- Delta Prescaler--------//
            s_FMKTIM_DecomposeFloat(prescalerTheo_f32, &realPrescaler_u32, &deltaPrescaler_f32);

            //----Manage prescaler == 0 for DIV 0 --------//
            if(realPrescaler_u32 == (t_uint32)0)
            {
                realARR_u32 = (t_uint32)((((t_uint32)f_timerFreqMhz_u32 * CST_MHZ_TO_HZ) /
                                                    (t_uint32)(f_PwmFreq_f32)) - (t_uint32)1);
            }
            else 
            {

                //---- recalculate ARR value--------//
                realARR_u32 = (t_uint32)((t_float32)targetARRValue_f32 + (t_float32)((t_float32)(targetARRValue_f32/realPrescaler_u32) * deltaPrescaler_f32));
            }
            //----Verify Range Validity--------//
            if(realPrescaler_u32 > (t_uint32)(maxNumberBit_u32)
            || realARR_u32 > (t_uint32)maxNumberBit_u32
            || realARR_u32 == (t_uint32)0)
            {
                Ret_e = RC_ERROR_WRONG_RESULT;
            }
            else 
            {
                // to get faster calculation, I decide to not verify calculation...
                //----Verify Compute value--------//
                /*freqPwmCompute_f32 = s_FMKTIM_Timer_GetFreqPwm((t_uint32)OscTimSrcValue_u16, (realPrescaler_u32 - 1), realARR_u32);
                deltaFreqPwm_f32 = (t_float32)(f_PwmFreq_f32 - freqPwmCompute_f32);
                #warning('update condition 'cause now based on pourcentage')
                if (fabsf(deltaFreqPwm_f32) > (t_float32)FMKTIM_FREQ_COMPUTE_DELTA_ACCEPTANCE)
                {
                    Ret_e = RC_ERROR_WRONG_RESULT;
                }*/

                //----Copy values--------//
                if(Ret_e == RC_OK)
                {
                    *f_bspTimARR_pu32 = (t_uint32)(realARR_u32);
                    *f_bspTimPrescaler_pu32 = (t_uint32)(realPrescaler_u32 - 1);
                }
            }
        }
    }

    return Ret_e;  
}

/*********************************
 * FMKTIM_GetICTimerInitParam
 *********************************/
t_eReturnCode FMKTIM_GetICTimerInitParam(   t_eFMKCPU_ClockPort f_timClock_e,
                                            t_uint32    f_timerFreqMhz_u32,
                                            t_float32  f_PwmFreq_f32,
                                            t_uint32 * f_bspTimARR_pu32,
                                            t_uint32 * f_bspTimPrescaler_pu32)
{
    
    return FMKTIM_GetPwmTimerInitParam( f_timClock_e,
                                        f_timerFreqMhz_u32,
                                        f_PwmFreq_f32,
                                        f_bspTimARR_pu32,
                                        f_bspTimPrescaler_pu32);


    return RC_OK;
}

/*********************************
 * FMKTIM_GetOCTimerInitParam
 *********************************/
t_eReturnCode FMKTIM_GetOCTimerInitParam(   t_eFMKCPU_ClockPort f_timClock_e,
                                            t_uint32    f_timerFreqMhz_u32,
                                            t_float32  f_PwmFreq_f32,
                                            t_uint32 * f_bspTimARR_pu32,
                                            t_uint32 * f_bspTimPrescaler_pu32)
{
    return RC_ERROR_INSTANCE_NOT_INITIALIZED;
}

/*********************************
 * FMKTIM_GetOPTimerInitParam
 *********************************/
t_eReturnCode FMKTIM_GetOPTimerInitParam(   t_eFMKCPU_ClockPort f_timClock_e,
                                            t_uint32    f_timerFreqMhz_u32,
                                            t_float32  f_PwmFreq_f32,
                                            t_uint32 * f_bspTimARR_pu32,
                                            t_uint32 * f_bspTimPrescaler_pu32)
{
    return RC_ERROR_INSTANCE_NOT_INITIALIZED;
}

/*********************************
 * FMKTIM_GetEvntTimerInitParam
 *********************************/
t_eReturnCode FMKTIM_GetEvntTimerInitParam( t_eFMKCPU_ClockPort f_timClock_e,
                                            t_uint32    f_timerFreqMhz_u32,
                                            t_float32  f_EvntFreq_f32,
                                            t_uint32 * f_bspTimARR_pu32,
                                            t_uint32 * f_bspTimPrescaler_pu32)
{
    t_eReturnCode Ret_e = RC_OK;
    t_float32 lowPrescaler_f32  = (t_float32)0.0f;
    t_float32 highPrescaler_f32 = (t_float32)0.0f;
    t_float32 deltaFreq_f32     = (t_float32)0.0f;
    t_float32 freqCompute_f32    = (t_uint32)0.0f;

    t_uint32 realPrescaler_u32  = (t_uint32)0;
    t_uint32 realARR_u32        = (t_uint32)0;
    t_uint32 maxARRValue_u32    = (t_uint32)0;
    t_uint32 minARRValue_u32    = (t_uint32)0;
    t_uint32 maxNumberBit_u32   = (t_uint32)0;

    if((f_bspTimPrescaler_pu32 == (t_uint32 *)NULL)

    || (f_bspTimARR_pu32       == (t_uint32 *)NULL))
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }
    if(f_timClock_e >= FMKCPU_RCC_CLK_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
    }
    if(Ret_e == RC_OK)
    {
        // Timer depend on APB1 or APB2, if these clock were divided per 2 or more,
        // Hardware multiply by 2 the core freqency of the timer
        // In other word 
        if(FMKTIM_IS_32B_TIMER(f_timClock_e) == True)
        {
            maxNumberBit_u32 = (t_uint32)(CST_MAX_UINT_32BIT);
            minARRValue_u32 = (t_uint32)(FMKTIM_ARR_LOW_LIMIT_32BIT);
            maxARRValue_u32 = (t_uint32)(FMKTIM_ARR_HIGH_LIMIT_32BIT); // 32-bit target
        }
        else 
        {   
            maxNumberBit_u32 = (t_uint32)(CST_MAX_UINT_16BIT);
            minARRValue_u32 = (t_uint32)(FMKTIM_ARR_LOW_LIMIT_16BIT);
            maxARRValue_u32 = (t_uint32)(FMKTIM_ARR_HIGH_LIMIT_16BIT); // 16-bit target
        }
        //----------Calculate Low/High/Mean Prescaler---------//
        lowPrescaler_f32 = s_FMKTIM_Timer_GetPrescaler(f_timerFreqMhz_u32, minARRValue_u32, f_EvntFreq_f32);
        highPrescaler_f32 = s_FMKTIM_Timer_GetPrescaler(f_timerFreqMhz_u32, maxARRValue_u32, f_EvntFreq_f32);

        realPrescaler_u32 = (t_uint32)((lowPrescaler_f32 + highPrescaler_f32) / (t_float32)2.0f);

        //----------Calculate ARR with prescaler---------//
        realARR_u32 = s_FMKTIM_Timer_GetArrRegister(f_timerFreqMhz_u32, realPrescaler_u32, f_EvntFreq_f32);

        //----Varify Range Value--------//
        if(realARR_u32 > maxNumberBit_u32
        ||realPrescaler_u32 > maxNumberBit_u32
        || realARR_u32 == (t_uint32)0)
        {
            Ret_e = RC_ERROR_WRONG_RESULT;
        }
        else
        {
            //----Verify Compute value--------//
            freqCompute_f32 = s_FMKTIM_Timer_GetFreqPwm((t_uint32)f_timerFreqMhz_u32, realPrescaler_u32, realARR_u32);
            deltaFreq_f32 = (t_float32)(f_EvntFreq_f32 - freqCompute_f32);

            if (fabsf(deltaFreq_f32) > (t_float32)FMKTIM_FREQ_COMPUTE_DELTA_ACCEPTANCE)
            {
                Ret_e = RC_ERROR_WRONG_RESULT;
            }
            if(Ret_e == RC_OK)
            {
                *f_bspTimARR_pu32 = (t_uint32)realARR_u32;
                *f_bspTimPrescaler_pu32 = (t_uint32)(realPrescaler_u32);
            }
        }
    }

    return Ret_e;
}

/*********************************
 * FMKTIM_GetECDRTimerInitParam
 *********************************/
t_eReturnCode FMKTIM_GetECDRTimerInitParam( t_eFMKCPU_ClockPort f_timClock_e,
                                            t_uint32    f_timerFreqMhz_u32,
                                            t_float32  f_rqstARRValue_u32,
                                            t_uint32 * f_bspARR_pu32,
                                            t_uint32 * f_bspTimPrescaler_pu32)
{
    t_eReturnCode Ret_e = RC_OK;
    t_uint32 maxARRValue_u32    = (t_uint32)0;
    //t_uint32 minARRValue_u32    = (t_uint32)0;
    //t_uint32 maxNumberBit_u32   = (t_uint32)0;

    if((f_bspTimPrescaler_pu32 == (t_uint32 *)NULL)

    || (f_bspARR_pu32          == (t_uint32 *)NULL))
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }
    if(f_timClock_e >= FMKCPU_RCC_CLK_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
    }
    if(Ret_e == RC_OK)
    {
        if(FMKTIM_IS_32B_TIMER(f_timClock_e) == True)
        {
         
            maxARRValue_u32 = (t_uint32)(FMKTIM_ARR_HIGH_LIMIT_32BIT); // 32-bit target
        }
        else 
        {   
            maxARRValue_u32 = (t_uint32)(FMKTIM_ARR_HIGH_LIMIT_16BIT); // 16-bit target
        }
        if(Ret_e == RC_OK)
        {
            if(f_rqstARRValue_u32 > (t_uint32)maxARRValue_u32)
            {
                Ret_e = RC_ERROR_LIMIT_REACHED;
            }
            else 
            {
                *f_bspARR_pu32 = (t_uint32)(f_rqstARRValue_u32 - 1);
                *f_bspTimPrescaler_pu32 = (t_uint32)(0);
            }
        }
    }

    return Ret_e;
}

/*********************************
 * FMKTIM_GetDacTimerInitParam
 *********************************/
t_eReturnCode FMKTIM_GetDacTimerInitParam( t_eFMKCPU_ClockPort f_timClock_e,
                                            t_uint32    f_timerFreqMhz_u32,
                                            t_float32  f_rqstARRValue_u32,
                                            t_uint32 * f_bspARR_pu32,
                                            t_uint32 * f_bspTimPrescaler_pu32) 
{
    return RC_ERROR_INSTANCE_NOT_INITIALIZED;
}

/**< This function has been made to filled in c_FMKTIM_TimerFunc_apf */

/*********************************
 * FMKTIM_HAL_TIM_Base_Start
 *********************************/
HAL_StatusTypeDef FMKTIM_HAL_TIM_Base_Start(TIM_HandleTypeDef *htim, uint32_t Channel){UNUSED(Channel); return HAL_TIM_Base_Start(htim);}

/*********************************
 * FMKTIM_HAL_TIM_Base_Stop
 *********************************/
HAL_StatusTypeDef FMKTIM_HAL_TIM_Base_Stop(TIM_HandleTypeDef *htim, uint32_t Channel){UNUSED(Channel); return HAL_TIM_Base_Stop(htim);}

/*********************************
 * FMKTIM_HAL_TIM_Base_Start_IT
 *********************************/
HAL_StatusTypeDef FMKTIM_HAL_TIM_Base_Start_IT(TIM_HandleTypeDef *htim, uint32_t Channel){UNUSED(Channel); return HAL_TIM_Base_Start_IT(htim);}

/*********************************
 * FMKTIM_HAL_TIM_Base_Stop_IT
 *********************************/
HAL_StatusTypeDef FMKTIM_HAL_TIM_Base_Stop_IT(TIM_HandleTypeDef *htim, uint32_t Channel){UNUSED(Channel); return HAL_TIM_Base_Stop_IT(htim);}


/*********************************
 * FMKTIM_HAL_TIM_Base_Start_DMA
 *********************************/
HAL_StatusTypeDef FMKTIM_HAL_TIM_Base_Start_DMA(TIM_HandleTypeDef *htim, 
                                                uint32_t Channel, 
                                                uint32_t *pdata1_pu32,
                                                uint32_t *pdata2_pu32,
                                                uint16_t lenght_u16)
{
    UNUSED(Channel); 
    UNUSED(pdata2_pu32);
    return HAL_TIM_Base_Start_DMA(htim, pdata1_pu32, lenght_u16);
}
/*********************************
 * FMKTIM_HAL_TIM_OC_Start_DMA
 *********************************/
HAL_StatusTypeDef FMKTIM_HAL_TIM_OC_Start_DMA(  TIM_HandleTypeDef *htim, 
                                                uint32_t Channel, 
                                                uint32_t *pdata1_pu32,
                                                uint32_t *pdata2_pu32,
                                                uint16_t lenght_u16)
{
    UNUSED(pdata2_pu32);
    return HAL_TIM_OC_Start_DMA(htim,Channel ,pdata1_pu32, lenght_u16);
}
/*********************************
 * FMKTIM_HAL_TIM_PWM_Start_DMA
 *********************************/
HAL_StatusTypeDef FMKTIM_HAL_TIM_PWM_Start_DMA( TIM_HandleTypeDef *htim, 
                                                uint32_t Channel, 
                                                uint32_t *pdata1_pu32,
                                                uint32_t *pdata2_pu32,
                                                uint16_t lenght_u16)
{
    UNUSED(pdata2_pu32);
    return HAL_TIM_PWM_Start_DMA(htim, Channel, pdata1_pu32, lenght_u16);
}

/*********************************
 * FMKTIM_HAL_TIM_IC_Start_DMA
 *********************************/
HAL_StatusTypeDef FMKTIM_HAL_TIM_IC_Start_DMA(  TIM_HandleTypeDef *htim, 
                                                uint32_t Channel, 
                                                uint32_t *pdata1_pu32,
                                                uint32_t *pdata2_pu32,
                                                uint16_t lenght_u16)
{
    UNUSED(pdata2_pu32);
    return HAL_TIM_IC_Start_DMA(htim, Channel, pdata1_pu32, lenght_u16);
}

/*********************************
 * FMKTIM_HAL_TIM_Encoder_Start_DMA
 *********************************/
HAL_StatusTypeDef FMKTIM_HAL_TIM_Encoder_Start_DMA( TIM_HandleTypeDef *htim, 
                                                    uint32_t Channel, 
                                                    uint32_t *pdata1_pu32,
                                                    uint32_t *pdata2_pu32,
                                                    uint16_t lenght_u16)
{
    return HAL_TIM_Encoder_Start_DMA(htim, Channel, pdata1_pu32, pdata2_pu32, lenght_u16);
}

/*********************************
 * FMKTIM_HAL_TIM_Base_Stop_DMA
 *********************************/
HAL_StatusTypeDef FMKTIM_HAL_TIM_Base_Stop_DMA(TIM_HandleTypeDef *htim, uint32_t Channel)
{
    UNUSED(Channel); return HAL_TIM_Base_Stop_DMA(htim);
}

/*********************************
 * FMKTIM_HAL_TIM_PWM_Init
 *********************************/
HAL_StatusTypeDef FMKTIM_HAL_TIM_PWM_Init(TIM_HandleTypeDef *htim, void * f_TimerCfg_pv)
{
    UNUSED(f_TimerCfg_pv); 
    return HAL_TIM_PWM_Init(htim);
}

/*********************************
 * FMKTIM_HAL_TIM_OC_Init
 *********************************/
HAL_StatusTypeDef FMKTIM_HAL_TIM_OC_Init(TIM_HandleTypeDef *htim, void * f_TimerCfg_pv)
{
    UNUSED(f_TimerCfg_pv); 
    return HAL_TIM_OC_Init(htim);
}

/*********************************
 * FMKTIMHAL_TIM_Base_Init
 *********************************/
HAL_StatusTypeDef FMKTIM_HAL_TIM_Base_Init(TIM_HandleTypeDef *htim, void * f_TimerCfg_pv)
{
    UNUSED(f_TimerCfg_pv); 
    return HAL_TIM_Base_Init(htim);
}

/*********************************
 * FMKTIM_HAL_TIM_IC_Init
 *********************************/
HAL_StatusTypeDef FMKTIM_HAL_TIM_IC_Init(TIM_HandleTypeDef *htim, void * f_TimerCfg_pv)
{
    UNUSED(f_TimerCfg_pv); 
    return HAL_TIM_IC_Init(htim);
}

/*********************************
 * FMKTIM_HAL_TIM_OnePulse_Init
 *********************************/
HAL_StatusTypeDef FMKTIM_HAL_TIM_OnePulse_Init(TIM_HandleTypeDef *htim, void * f_TimerCfg_pv)
{
    t_uint32 onePUlse_u32 = (t_uint32)(&f_TimerCfg_pv);
    return HAL_TIM_OnePulse_Init(htim, (t_uint32)onePUlse_u32);
}

/*********************************
 * FMKTIM_HAL_TIM_OnePulse_Init
 *********************************/
HAL_StatusTypeDef FMKTIM_HAL_TIM_Encoder_Init(TIM_HandleTypeDef *htim, void * f_TimerCfg_pv)
{
    
    TIM_Encoder_InitTypeDef * bspEcdrCdg_ps = (TIM_Encoder_InitTypeDef *)(f_TimerCfg_pv);
    return HAL_TIM_Encoder_Init(htim, (TIM_Encoder_InitTypeDef *)bspEcdrCdg_ps);
}

//********************************************************************************
//                      HAL_Callback Implementation
//********************************************************************************
/**
 *
 *	@brief      Implementation of every timer IRQHandler wardware function.\n 
 *
 */
//********************************************************************************
//                      Local functions - Implementation
//********************************************************************************
           
//************************************************************************************
// End of File
//************************************************************************************

/**
 *
 *	@brief
 *	@note   
 *
 *
 *	@params[in] 
 *	@params[out]
 *	 
 *
 *
 */
