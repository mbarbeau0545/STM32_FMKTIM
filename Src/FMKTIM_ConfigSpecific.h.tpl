/*********************************************************************
 * @file        FMKTIM_ConfigSpecific.h
 * @brief       Template_BriefDescription.
 * @note        TemplateDetailsDescription.\n
 *
 * @author      xxxxxx
 * @date        jj/mm/yyyy
 * @version     1.0
 */
  
#ifndef FMKTIM_CONFIGSPECIFIC_H_INCLUDED
#define FMKTIM_CONFIGSPECIFIC_H_INCLUDED





    // ********************************************************************
    // *                      Includes
    // ********************************************************************
    #include "FMK_CFG/FMKCFG_ConfigFiles/FMKTIM_ConfigPublic.h"
    #include "FMK_CFG/FMKCFG_ConfigFiles/FMKCPU_ConfigPublic.h"
    #include "FMK_HAL/FMK_TIM/Src/FMK_TIM.h"
    #include "TypeCommon.h"
    // ********************************************************************
    // *                      Defines
    // ********************************************************************

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
    


    //********************************************************************************
    //                      Public functions - Prototyupes
    //********************************************************************************
    /**
    *
    *	@brief    Get the Prescaler and ARR value For Timer Configured in PWM.\n
    *
    *	@param[in]  f_InterruptLine_e      : enum value for Interrupt Line, value from @ref t_eFMKTIM_InterruptLineIO
    *	@param[in]  f_MeasTrigger_e        : trigger for interruption, value from @ref t_eFMKTIM_ChnlMeasTrigger
    *	@param[in]  f_ITChannel_cb         : call back function to call
    *
    *  @retval RC_OK                             @ref RC_OK
    *  @retval RC_ERROR_PTR_NULL                 @ref RC_ERROR_PTR_NULL
    *  @retval RC_ERROR_PARAM_INVALID            @ref RC_ERROR_PARAM_INVALID
    *  @retval RC_ERROR_WRONG_RESULT              @ref RC_ERROR_WRONG_RESULT
    */
    t_eReturnCode FMKTIM_GetPwmTimerInitParam(  t_eFMKCPU_ClockPort f_timClock_e,
                                            t_uint32    f_timerFreqMhz_u32,
                                            t_float32  f_PwmFreq_f32,
                                            t_uint32 * f_bspTimARR_pu32,
                                            t_uint32 * f_bspTimPrescaler_pu32);
    /**
    *
    *	@brief    Get the Prescaler and ARR value For Timer Configured in Input Compar.\n
    *
    *	@param[in]  f_InterruptLine_e      : enum value for Interrupt Line, value from @ref t_eFMKTIM_InterruptLineIO
    *	@param[in]  f_MeasTrigger_e        : trigger for interruption, value from @ref t_eFMKTIM_ChnlMeasTrigger
    *	@param[in]  f_ITChannel_cb         : call back function to call
    *
    *  @retval RC_OK                             @ref RC_OK
    *  @retval RC_ERROR_PTR_NULL                 @ref RC_ERROR_PTR_NULL
    *  @retval RC_ERROR_PARAM_INVALID            @ref RC_ERROR_PARAM_INVALID
    *  @retval RC_ERROR_WRONG_RESULT              @ref RC_ERROR_WRONG_RESULT
    */
    t_eReturnCode FMKTIM_GetICTimerInitParam(   t_eFMKCPU_ClockPort f_timClock_e,
                                            t_uint32    f_timerFreqMhz_u32,
                                            t_float32  f_PwmFreq_f32,
                                            t_uint32 * f_bspTimARR_pu32,
                                            t_uint32 * f_bspTimPrescaler_pu32);
    /**
    *
    *	@brief    Get the Prescaler and ARR value For Timer Configured in Output Compare.\n
    *
    *	@param[in]  f_InterruptLine_e      : enum value for Interrupt Line, value from @ref t_eFMKTIM_InterruptLineIO
    *	@param[in]  f_MeasTrigger_e        : trigger for interruption, value from @ref t_eFMKTIM_ChnlMeasTrigger
    *	@param[in]  f_ITChannel_cb         : call back function to call
    *
    *  @retval RC_OK                             @ref RC_OK
    *  @retval RC_ERROR_PTR_NULL                 @ref RC_ERROR_PTR_NULL
    *  @retval RC_ERROR_PARAM_INVALID            @ref RC_ERROR_PARAM_INVALID
    *  @retval RC_ERROR_WRONG_RESULT              @ref RC_ERROR_WRONG_RESULT
    */
    t_eReturnCode FMKTIM_GetOCTimerInitParam(   t_eFMKCPU_ClockPort f_timClock_e,
                                            t_uint32    f_timerFreqMhz_u32,
                                            t_float32  f_PwmFreq_f32,
                                            t_uint32 * f_bspTimARR_pu32,
                                            t_uint32 * f_bspTimPrescaler_pu32);

    /**
    *
    *	@brief    Get the Prescaler and ARR value For Timer Configured in OP.\n
    *
    *	@param[in]  f_InterruptLine_e      : enum value for Interrupt Line, value from @ref t_eFMKTIM_InterruptLineIO
    *	@param[in]  f_MeasTrigger_e        : trigger for interruption, value from @ref t_eFMKTIM_ChnlMeasTrigger
    *	@param[in]  f_ITChannel_cb         : call back function to call
    *
    *  @retval RC_OK                             @ref RC_OK
    *  @retval RC_ERROR_PTR_NULL                 @ref RC_ERROR_PTR_NULL
    *  @retval RC_ERROR_PARAM_INVALID            @ref RC_ERROR_PARAM_INVALID
    *  @retval RC_ERROR_WRONG_RESULT              @ref RC_ERROR_WRONG_RESULT
    */
    t_eReturnCode FMKTIM_GetOPTimerInitParam(   t_eFMKCPU_ClockPort f_timClock_e,
                                            t_uint32    f_timerFreqMhz_u32,
                                            t_float32  f_PwmFreq_f32,
                                            t_uint32 * f_bspTimARR_pu32,
                                            t_uint32 * f_bspTimPrescaler_pu32);
    /**
    *
    *	@brief    Get the Prescaler and ARR value For Timer Configured in Event.\n
    *
    *	@param[in]  f_InterruptLine_e      : enum value for Interrupt Line, value from @ref t_eFMKTIM_InterruptLineIO
    *	@param[in]  f_MeasTrigger_e        : trigger for interruption, value from @ref t_eFMKTIM_ChnlMeasTrigger
    *	@param[in]  f_ITChannel_cb         : call back function to call
    *
    *  @retval RC_OK                             @ref RC_OK
    *  @retval RC_ERROR_PTR_NULL                 @ref RC_ERROR_PTR_NULL
    *  @retval RC_ERROR_PARAM_INVALID            @ref RC_ERROR_PARAM_INVALID
    *  @retval RC_ERROR_WRONG_RESULT              @ref RC_ERROR_WRONG_RESULT
    */
    t_eReturnCode FMKTIM_GetEvntTimerInitParam( t_eFMKCPU_ClockPort f_timClock_e,
                                            t_uint32    f_timerFreqMhz_u32,
                                            t_float32  f_EvntFreq_f32,
                                            t_uint32 * f_bspTimARR_pu32,
                                            t_uint32 * f_bspTimPrescaler_pu32);
    /**
    *
    *	@brief    Get the Prescaler and ARR value For Timer Configured in Encoder.\n
    *
    *	@param[in]  f_InterruptLine_e      : enum value for Interrupt Line, value from @ref t_eFMKTIM_InterruptLineIO
    *	@param[in]  f_MeasTrigger_e        : trigger for interruption, value from @ref t_eFMKTIM_ChnlMeasTrigger
    *	@param[in]  f_ITChannel_cb         : call back function to call
    *
    *  @retval RC_OK                             @ref RC_OK
    *  @retval RC_ERROR_PTR_NULL                 @ref RC_ERROR_PTR_NULL
    *  @retval RC_ERROR_PARAM_INVALID            @ref RC_ERROR_PARAM_INVALID
    *  @retval RC_ERROR_WRONG_RESULT              @ref RC_ERROR_WRONG_RESULT
    */
    t_eReturnCode FMKTIM_GetECDRTimerInitParam( t_eFMKCPU_ClockPort f_timClock_e,
                                            t_uint32    f_timerFreqMhz_u32,
                                            t_float32  f_rqstARRValue_u32,
                                            t_uint32 * f_bspARR_pu32,
                                            t_uint32 * f_bspTimPrescaler_pu32);
    /**
    *
    *	@brief    Get the Prescaler and ARR value For Timer Configured in Dac.\n
    *
    *	@param[in]  f_InterruptLine_e      : enum value for Interrupt Line, value from @ref t_eFMKTIM_InterruptLineIO
    *	@param[in]  f_MeasTrigger_e        : trigger for interruption, value from @ref t_eFMKTIM_ChnlMeasTrigger
    *	@param[in]  f_ITChannel_cb         : call back function to call
    *
    *  @retval RC_OK                             @ref RC_OK
    *  @retval RC_ERROR_PTR_NULL                 @ref RC_ERROR_PTR_NULL
    *  @retval RC_ERROR_PARAM_INVALID            @ref RC_ERROR_PARAM_INVALID
    *  @retval RC_ERROR_WRONG_RESULT              @ref RC_ERROR_WRONG_RESULT
    */
    t_eReturnCode FMKTIM_GetDacTimerInitParam( t_eFMKCPU_ClockPort f_timClock_e,
                                            t_uint32    f_timerFreqMhz_u32,
                                            t_float32  f_rqstARRValue_u32,
                                            t_uint32 * f_bspARR_pu32,
                                            t_uint32 * f_bspTimPrescaler_pu32);

    /**< This function has been made to filled in c_FMKTIM_TimerFunc_apf */
    /*********************************
     * FMKTIM_HAL_TIM_Base_Start
     *********************************/
    HAL_StatusTypeDef FMKTIM_HAL_TIM_Base_Start(TIM_HandleTypeDef *htim, uint32_t Channel);
    /*********************************
     * FMKTIM_HAL_TIM_Base_Stop
     *********************************/
    HAL_StatusTypeDef FMKTIM_HAL_TIM_Base_Stop(TIM_HandleTypeDef *htim, uint32_t Channel);
    /*********************************
     * FMKTIM_HAL_TIM_Base_Start_IT
     *********************************/
    HAL_StatusTypeDef FMKTIM_HAL_TIM_Base_Start_IT(TIM_HandleTypeDef *htim, uint32_t Channel);
    /*********************************
     * FMKTIM_HAL_TIM_Base_Stop_IT
     *********************************/
    HAL_StatusTypeDef FMKTIM_HAL_TIM_Base_Stop_IT(TIM_HandleTypeDef *htim, uint32_t Channel);
    /*********************************
     * FMKTIM_HAL_TIM_Base_Start_DMA
     *********************************/
    HAL_StatusTypeDef FMKTIM_HAL_TIM_Base_Start_DMA(TIM_HandleTypeDef *htim, 
                                                    uint32_t Channel, 
                                                    uint32_t *pdata1_pu32,
                                                    uint32_t *pdata2_pu32,
                                                    uint16_t lenght_u16);
    /*********************************
     * FMKTIM_HAL_TIM_Base_Stop_DMA
     *********************************/
    HAL_StatusTypeDef FMKTIM_HAL_TIM_Base_Stop_DMA(TIM_HandleTypeDef *htim, uint32_t Channel);
    /*********************************
     * FMKTIM_HAL_TIM_OC_Start_DMA
     *********************************/
    HAL_StatusTypeDef FMKTIM_HAL_TIM_OC_Start_DMA(  TIM_HandleTypeDef *htim, 
                                                    uint32_t Channel, 
                                                    uint32_t *pdata1_pu32,
                                                    uint32_t *pdata2_pu32,
                                                    uint16_t lenght_u16);
    /*********************************
     * FMKTIM_HAL_TIM_PWM_Start_DMA
     *********************************/
    HAL_StatusTypeDef FMKTIM_HAL_TIM_PWM_Start_DMA( TIM_HandleTypeDef *htim, 
                                                    uint32_t Channel, 
                                                    uint32_t *pdata1_pu32,
                                                    uint32_t *pdata2_pu32,
                                                    uint16_t lenght_u16);
    /*********************************
     * FMKTIM_HAL_TIM_IC_Start_DMA
     *********************************/
    HAL_StatusTypeDef FMKTIM_HAL_TIM_IC_Start_DMA(  TIM_HandleTypeDef *htim, 
                                                    uint32_t Channel, 
                                                    uint32_t *pdata1_pu32,
                                                    uint32_t *pdata2_pu32,
                                                    uint16_t lenght_u16);
    /*********************************
     * FMKTIM_HAL_TIM_Encoder_Start_DMA
     *********************************/
    HAL_StatusTypeDef FMKTIM_HAL_TIM_Encoder_Start_DMA( TIM_HandleTypeDef *htim, 
                                                        uint32_t Channel, 
                                                        uint32_t *pdata1_pu32,
                                                        uint32_t *pdata2_pu32,
                                                        uint16_t lenght_u16);
    /*********************************
     * FMKTIM_HAL_TIM_Base_Stop_DMA
     *********************************/
    HAL_StatusTypeDef FMKTIM_HAL_TIM_PWM_Init(TIM_HandleTypeDef *htim, void * f_TimerCfg_pv);
    /*********************************
     * FMKTIM_HAL_TIM_Base_Start
     *********************************/
    HAL_StatusTypeDef FMKTIM_HAL_TIM_OC_Init(TIM_HandleTypeDef *htim, void * f_TimerCfg_pv);
    /*********************************
     * FMKTIM_HAL_TIM_Base_Stop
     *********************************/
    HAL_StatusTypeDef FMKTIM_HAL_TIM_Base_Init(TIM_HandleTypeDef *htim, void * f_TimerCfg_pv);
    /*********************************
     * FMKTIM_HAL_TIM_Base_Start_IT
     *********************************/
    HAL_StatusTypeDef FMKTIM_HAL_TIM_IC_Init(TIM_HandleTypeDef *htim, void * f_TimerCfg_pv);
    /*********************************
     * FMKTIM_HAL_TIM_Base_Stop_IT
     *********************************/
    HAL_StatusTypeDef FMKTIM_HAL_TIM_OnePulse_Init(TIM_HandleTypeDef *htim, void * f_TimerCfg_pv);
    /*********************************
     * FMKTIM_HAL_TIM_Base_Stop_IT
     *********************************/
    HAL_StatusTypeDef FMKTIM_HAL_TIM_Encoder_Init(TIM_HandleTypeDef *htim, void * f_TimerCfg_pv);



#endif // FMKPU_CONFIGSPECIFIC_H_INCLUDED           
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
