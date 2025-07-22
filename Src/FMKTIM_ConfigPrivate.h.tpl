/*********************************************************************
 * @file        FMKTIM_ConfigPrivate.h
 * @brief       Template_BriefDescription.
 * @note        TemplateDetailsDescription.\n
 *
 * @author      xxxxxx
 * @date        jj/mm/yyyy
 * @version     1.0
 */
  
#ifndef FMKTIM_CONFIGPRIVATE_H_INCLUDED
#define FMKTIM_CONFIGPRIVATE_H_INCLUDED





    // ********************************************************************
    // *                      Includes
    // ********************************************************************
    #include "../FMKCFG_ConfigSpecific/FMKTIM_ConfigSpecific.h"
    #include "FMK_HAL/FMK_TIM/Src/FMK_TIM.h"
    #include "TypeCommon.h"
    // ********************************************************************
    // *                      Defines
    // ********************************************************************
    /* CAUTION : Automatic generated code section for Timer channels number: Start */
    #define FMKTIM_MAX_CHNL_TIMER_1 ((t_uint8)4)
    #define FMKTIM_MAX_CHNL_TIMER_2 ((t_uint8)4)
    #define FMKTIM_MAX_CHNL_TIMER_3 ((t_uint8)4)
    #define FMKTIM_MAX_CHNL_TIMER_4 ((t_uint8)4)
    #define FMKTIM_MAX_CHNL_TIMER_5 ((t_uint8)4)
    #define FMKTIM_MAX_CHNL_TIMER_6 ((t_uint8)1)
    #define FMKTIM_MAX_CHNL_TIMER_7 ((t_uint8)1)
    #define FMKTIM_MAX_CHNL_TIMER_8 ((t_uint8)4)
    #define FMKTIM_MAX_CHNL_TIMER_15 ((t_uint8)2)
    #define FMKTIM_MAX_CHNL_TIMER_16 ((t_uint8)1)
    #define FMKTIM_MAX_CHNL_TIMER_17 ((t_uint8)1)
    #define FMKTIM_MAX_CHNL_TIMER_20 ((t_uint8)4)
    /* CAUTION : Automatic generated code section for Timer channels number: End */


    #define FMKTIM_TIMER_IC_PSC          ((t_uint8)20)        /**<  for every InputCOmpare the Prescaler is a constant */
    #define FMKTIM_TIMER_IC_ARR          ((t_uint16)0xFFFF)       /**<  for every InputCOmpare the Prescaler is a constant */

    #define FMKTIM_TIMER_EVNT_MAX_LOW_PERIOD_MS ((t_uint16)60000)
    #define FMKTIM_TIMER_EVNT_PSC_HIGH          ((t_uint16)(20 * 1000) - (t_uint16)1) /**<  The prescaler use for evnt timer, having 1000Hz (1ms) */
    #define FMKTIM_TIMER_EVNT_PSC_LOW           ((t_uint16)65534)
    #define FMKTIM_TIME_BTWN_DIAG_MS            ((t_uint16)2000)   /**< Time between diagnostic for timer and channel in cyclic ope mode*/

    #define FMKTIM_INTERRUPT_LINE_UNUSED (0xFF)
    
    // ********************************************************************
    // *                      Types
    // ********************************************************************
    /* CAUTION : Automatic generated code section for Enum: Start */
    /**
    * @brief Number of timer enable in smt32xxx board.
    */
    typedef enum
    {
        FMKTIM_TIMER_1 = 0,                  /**< Reference for HAL timer_1, this timer has 4 channel(s) */
        FMKTIM_TIMER_2,                        /**< Reference for HAL timer_2, this timer has 4 channel(s) */
        FMKTIM_TIMER_3,                        /**< Reference for HAL timer_3, this timer has 4 channel(s) */
        FMKTIM_TIMER_4,                        /**< Reference for HAL timer_4, this timer has 4 channel(s) */
        FMKTIM_TIMER_5,                        /**< Reference for HAL timer_5, this timer has 4 channel(s) */
        FMKTIM_TIMER_6,                        /**< Reference for HAL timer_6, this timer has 1 channel(s) */
        FMKTIM_TIMER_7,                        /**< Reference for HAL timer_7, this timer has 1 channel(s) */
        FMKTIM_TIMER_8,                        /**< Reference for HAL timer_8, this timer has 4 channel(s) */
        FMKTIM_TIMER_15,                       /**< Reference for HAL timer_15, this timer has 2 channel(s) */
        FMKTIM_TIMER_16,                       /**< Reference for HAL timer_16, this timer has 1 channel(s) */
        FMKTIM_TIMER_17,                       /**< Reference for HAL timer_17, this timer has 1 channel(s) */
        FMKTIM_TIMER_20,                       /**< Reference for HAL timer_20, this timer has 4 channel(s) */
    
        FMKTIM_TIMER_NB,
    } t_eFMKTIM_Timer;

    /**< Number max of channel enable by timer */
     typedef enum
    {
        FMKTIM_CHANNEL_1 = 0x00,              // Reference to HAL channel 1
        FMKTIM_CHANNEL_2,              // f"Reference to HAL channel 2
        FMKTIM_CHANNEL_3,              // f"Reference to HAL channel 3
        FMKTIM_CHANNEL_4,              // f"Reference to HAL channel 4

        FMKTIM_CHANNEL_NB,
        FMKTIM_CHANNEL_ALL,
    } t_eFMKTIM_InterruptChnl;

    /* CAUTION : Automatic generated code section for Enum: End */

    ///@brief Timer Configuration Structure 
    typedef struct 
    {
        TIM_TypeDef * bspIstc_ps;         //---- pointor to timer bsp instance ----//
        const t_eFMKCPU_ClockPort c_clock_e;
        const t_eFMKCPU_IRQNType c_IRQNType_e; 
    } t_sFMKTIM_TimerCfg;
   /**
    *
    *	@brief    Get the Prescaler and ARR value For Timer Configured in PWM.\n
    *
    *	@param[in]  f_idxTimRccClock_u8     : enum value for Idx Rcc Clock Timer, value from @ref t_eFMKTIM_ClockPort
    *	@param[in]  f_timOscSrc_e           : enum value to know which bsp clock support the frequence based to the timer
    *	@param[in]  f_SysClockValues_ua8    : pointor to all sys clock value of the system
    *	@param[in]  f_RqstTimerFreq_u32     : the timer frequency requested
    *	@param[in]  f_RqstTimerFreq_u32     : Container for ARR register value 
    *	@param[in]  f_RqstTimerFreq_u32     : Container for Prescaler Value
    *	@param[in]  f_ITChannel_cb         : call back function to call
    *
    *  @retval RC_OK                             @ref RC_OK
    *  @retval RC_ERROR_PTR_NULL                 @ref RC_ERROR_PTR_NULL
    *  @retval RC_ERROR_PARAM_INVALID            @ref RC_ERROR_PARAM_INVALID
    *  @retval RC_ERROR_WRONG_RESULT             @ref RC_ERROR_WRONG_RESULT
    */
    typedef t_eReturnCode (t_cbFMKTIM_GetTimerInfoInit)(t_eFMKCPU_ClockPort f_timClock_e,
                                                        t_uint32    f_timerFreq_u32,
                                                        t_float32   f_RqstTimerFreq_f32,
                                                        t_uint32 * f_bspTimARR_pu32,
                                                        t_uint32 * f_bspTimPrescaler_pu32);
    /**
    *
    *	@brief      HAL Timer function for get the hardware timer state
    *	@note       This function repertory in stm32f00xx_hal_tim,
    * 
    *   @param[in] f_handleTimer_s : the bsp init structure
    *  
    *   @retval HAL_OK                     @ref HAL_OK
    *   @retval HAL_ERROR                  @ref HAL_ERROR
    */
    typedef HAL_TIM_StateTypeDef (t_cbFMKTIM_GetTimerState)(const TIM_HandleTypeDef *f_handleTimer_s);
    /**
    *
    *	@brief      HAL Timer function for Init
    *	@note       This function repertory in stm32f00xx_hal_tim,
    *               is used in blocking mode 
    * 
    *   @param[in] f_handleTimer_s : the bsp init structure
    *  
    *   @retval HAL_OK                     @ref HAL_OK
    *   @retval HAL_ERROR                  @ref HAL_ERROR
    */
    typedef HAL_StatusTypeDef (t_cbFMKTIM_TimerInitFunc)(TIM_HandleTypeDef *f_handleTimer_s, void * f_TimerCfg_pv);
    /**
    *
    *	@brief      HAL Timer function for Init
    *	@note       This function repertory in stm32f00xx_hal_tim,
    *               is used in blocking mode 
    * 
    *   @param[in] f_handleTimer_s : the bsp init structure
    * 
    *   @retval HAL_OK                     @ref HAL_OK
    *   @retval HAL_ERROR                  @ref HAL_ERROR
    */
    typedef HAL_StatusTypeDef (t_cbFMKTIM_TimerDeInitFunc)(TIM_HandleTypeDef *f_handleTimer_s);
    /**
    *
    *	@brief      HAL Timer function in Polling xay
    *	@note       This function repertory in stm32f00xx_hal_tim,
    *               is used in blocking mode 
    * 
    *	@param[in]      f_handleTimer_s : the bsp config structure 
    *	@param[out]     f_channel_u32   : the channel
    *
    *   @retval HAL_OK                     @ref HAL_OK
    *   @retval HAL_ERROR                  @ref HAL_ERROR 
    */
    typedef HAL_StatusTypeDef (t_cbFMKTIM_TimStartFuncModePolling)(TIM_HandleTypeDef *f_handleTimer_s, uint32_t f_channel_u32);
    /**
    *
    *	@brief      HAL Timer function in Interruption way  
    *	@note       This function repertory in stm32f00xx_hal_tim,
    *               is used in blocking mode 
    *
    *	@param[in]      f_handleTimer_s : the bsp config structure 
    *	@param[out]     f_channel_u32   : the channel 
    *
    *   @retval HAL_OK                     @ref HAL_OK
    *   @retval HAL_ERROR                  @ref HAL_ERROR
    */
    typedef HAL_StatusTypeDef (t_cbFMKTIM_TimStartFuncModeInterrupt)(TIM_HandleTypeDef *f_handleTimer_s, uint32_t f_channel_u32);
    /**
    *
    *	@brief      HAL Timer function in Polling xay
    *	@note       This function repertory in stm32f00xx_hal_tim,
    *               is used in blocking mode 
    *
    *	@param[in]      f_handleTimer_s : the bsp config structure 
    *	@param[out]     f_channel_u32   : the channel
    *
    *   @retval HAL_OK                     @ref HAL_OK
    *   @retval HAL_ERROR                  @ref HAL_ERROR
    */
    typedef HAL_StatusTypeDef (t_cbFMKTIM_TimStopFuncModePolling)(TIM_HandleTypeDef *f_handleTimer_s, uint32_t f_channel_u32);
    /**
    *
    *	@brief      HAL Timer function in Interruption way  
    *	@note       This function repertory in stm32f00xx_hal_tim,
    *               is used in blocking mode 
    *
    *	@param[in]      f_handleTimer_s : the bsp config structure 
    *	@param[out]     f_channel_u32   : the channel 
    *
    *   @retval HAL_OK                     @ref HAL_OK
    *   @retval HAL_ERROR                  @ref HAL_ERROR
    */
    typedef HAL_StatusTypeDef (t_cbFMKTIM_TimStopFuncModeInterrupt)(TIM_HandleTypeDef *f_handleTimer_s, uint32_t f_channel_u32);

    /**
    *
    *	@brief      HAL Timer function in DMA Mode  
    *	@note       This function repertory in stm32f00xx_hal_tim,
    *               is used in Dma mode 
    *
    *	@param[in]      f_handleTimer_s : the bsp config structure 
    *	@param[out]     f_channel_u32   : the channel 
    *	@param[out]     f_data_pu32     : Pointor to data
    *	@param[out]     f_channel_u32   : Lenght data  
    *
    *   @retval HAL_OK                     @ref HAL_OK
    *   @retval HAL_ERROR                  @ref HAL_ERROR
    */
    typedef HAL_StatusTypeDef (t_cbFMKTIM_TimStartFuncModeDMA)( TIM_HandleTypeDef *f_handleTimer_s,
                                                                uint32_t f_channel_u32, 
                                                                t_uint32 * f_data1_pu32, 
                                                                t_uint32 * f_data2_pu32, 
                                                                t_uint16 f_lenght_u16);

    typedef t_cbFMKTIM_TimStartFuncModeInterrupt t_cbFMKTIM_TimStopFuncModeDMA;

    //-----------------------------STRUCT TYPES---------------------------//
    /* CAUTION : Automatic generated code section for Structure: Start */

    /* CAUTION : Automatic generated code section for Structure: End */

    /**< Structure for repertory all HAL_TIM function */
    typedef struct
    {
        t_cbFMKTIM_GetTimerState             * GetTimerState_pcb;        /**< HAL_TIM function to get the timer state */
        t_cbFMKTIM_TimerInitFunc             * TimerInit_pcb;           /**< HAL_TIM function to set a timer init */
        t_cbFMKTIM_TimerDeInitFunc           * TimerDeInit_pcb;         /**< HAL_TIM function to set a timer deinit */
        t_cbFMKTIM_TimStartFuncModePolling   * StartFuncPoll_pcb;       /**< HAL_TIM function to start a timer_channel in polling mode */
        t_cbFMKTIM_TimStopFuncModePolling    * StopFuncPoll_pcb;        /**< HAL_TIM function to start a timer_channel in interruption mode */
        t_cbFMKTIM_TimStartFuncModeInterrupt * StartFuncInterrupt_pcb;  /**< HAL_TIM function to stop a timer_channel in polling mode */
        t_cbFMKTIM_TimStopFuncModeInterrupt  * StopFuncInterrupt_pcb;   /**< HAL_TIM function to stop a timer_channel in interruption mode */
        t_cbFMKTIM_TimStartFuncModeDMA       * StartFuncDma_pcb;        /**< HAL_TIM function to start a timer un DMA Mode */
        t_cbFMKTIM_TimStopFuncModeDMA        * StopFuncDma_pcb;         /**< HAL_TIM function to stop a timer un DMA Mode*/
        t_cbFMKTIM_GetTimerInfoInit          * GetTimerInfoInit_pcb;    /**< Specific Function to found the best PSC & ARR value for Freq and OscFreq */
    } t_sFMKTIM_TimChannelFunc;

    /**< Structure for timer channel configuration */
    typedef struct
    {
        t_eFMKTIM_Timer               timer_e;      /**< timer configuration */
        t_eFMKTIM_InterruptChnl       channel_e;    /**< channel configuration */
    } t_sFMKTIM_BspTimerCfg;

    typedef struct 
    {
        t_eFMKTIM_InterruptLineType type_e;
        t_uint8 ITLine_u8;
    } t_sFMKTIM_ChnlITLineMapping;

    
    // **********²**********************************************************
    // *                      Prototypes
    // ********************************************************************
    /**< timer information variable */

    // ********************************************************************
    // *                      Variables
    // ********************************************************************
    /**< Referencing all HAL_TIM function*/
    const t_sFMKTIM_TimChannelFunc c_FMKTIM_TimerFunc_apf[FMKTIM_HWTIM_CFG_NB] = 
    {//    Get Timer State                 Init Timer                       ²      DeInitTimer                   StartPolling Func                StopPolling Funnc             Start Interrupt Func             Stop Interrupt Func                Start Dma Func                         Stop Dma Func                     Get Init Timer Param
        {HAL_TIM_PWM_GetState,             FMKTIM_HAL_TIM_PWM_Init,                HAL_TIM_PWM_DeInit,           HAL_TIM_PWM_Start,               HAL_TIM_PWM_Stop,             HAL_TIM_PWM_Start_IT,           HAL_TIM_PWM_Stop_IT,               FMKTIM_HAL_TIM_PWM_Start_DMA,          HAL_TIM_PWM_Stop_DMA,            FMKTIM_GetPwmTimerInitParam},         // FMKTIM_CHANNEL_CFG_PWM 
        {HAL_TIM_IC_GetState,              FMKTIM_HAL_TIM_IC_Init,                 HAL_TIM_IC_DeInit,            HAL_TIM_IC_Start,                HAL_TIM_IC_Stop,              HAL_TIM_IC_Start_IT,            HAL_TIM_IC_Stop_IT,                FMKTIM_HAL_TIM_IC_Start_DMA,           HAL_TIM_IC_Stop_DMA,             FMKTIM_GetICTimerInitParam},          // FMKTIM_CHANNEL_CFG_IC
        {HAL_TIM_OC_GetState,              FMKTIM_HAL_TIM_OC_Init,                 HAL_TIM_OC_DeInit,            HAL_TIM_OC_Start,                HAL_TIM_OC_Stop,              HAL_TIM_OC_Start_IT,            HAL_TIM_OC_Stop_IT,                FMKTIM_HAL_TIM_OC_Start_DMA,           HAL_TIM_OC_Stop_DMA,             FMKTIM_GetOCTimerInitParam},          // FMKTIM_CHANNEL_CFG_OC
        {HAL_TIM_OnePulse_GetState,        FMKTIM_HAL_TIM_OnePulse_Init,           HAL_TIM_OnePulse_DeInit,      HAL_TIM_OnePulse_Start,          HAL_TIM_OnePulse_Stop,        HAL_TIM_OnePulse_Start_IT,      HAL_TIM_OnePulse_Stop_IT,          NULL_FUNCTION,                         NULL_FUNCTION,                   FMKTIM_GetOPTimerInitParam},          // FMKTIM_CHANNEL_CFG_OP
        {HAL_TIM_Base_GetState,            FMKTIM_HAL_TIM_Base_Init,               HAL_TIM_Base_DeInit,          FMKTIM_HAL_TIM_Base_Start,       FMKTIM_HAL_TIM_Base_Stop,     FMKTIM_HAL_TIM_Base_Start_IT,   FMKTIM_HAL_TIM_Base_Stop_IT,       FMKTIM_HAL_TIM_Base_Start_DMA,         FMKTIM_HAL_TIM_Base_Stop_DMA,    FMKTIM_GetEvntTimerInitParam},        // FMKTIM_CHANNEL_CFG_EVNT
        {HAL_TIM_Encoder_GetState,         FMKTIM_HAL_TIM_Encoder_Init,            HAL_TIM_Encoder_DeInit,       HAL_TIM_Encoder_Start,           HAL_TIM_Encoder_Stop,         HAL_TIM_Encoder_Start_IT,       HAL_TIM_Encoder_Stop_IT,           FMKTIM_HAL_TIM_Encoder_Start_DMA,      HAL_TIM_Encoder_Stop_DMA,        FMKTIM_GetECDRTimerInitParam},        // FMKTIM_CHANNEL_CFG_ECDR
        {HAL_TIM_PWM_GetState,             FMKTIM_HAL_TIM_PWM_Init,                HAL_TIM_PWM_DeInit,           HAL_TIM_PWM_Start,               HAL_TIM_PWM_Stop,             HAL_TIM_PWM_Start_IT,           HAL_TIM_PWM_Stop_IT,               FMKTIM_HAL_TIM_PWM_Start_DMA,          HAL_TIM_PWM_Stop_DMA,            FMKTIM_GetDacTimerInitParam},         // FMKTIM_CHANNEL_CFG_DAC 
    };

    /* CAUTION : Automatic generated code section for Variable: Start */
/**< timer configuration variable */
    t_sFMKTIM_TimerCfg c_FmkTim_TimersCfg_as[FMKTIM_TIMER_NB] = {
        [FMKTIM_TIMER_1] = {
        .bspIstc_ps = TIM1,
        .c_clock_e = FMKCPU_RCC_CLK_TIM1,
        .c_IRQNType_e = FMKCPU_NVIC_TIM1_UP_TIM16_IRQN
    },
        [FMKTIM_TIMER_2] = {
        .bspIstc_ps = TIM2,
        .c_clock_e = FMKCPU_RCC_CLK_TIM2,
        .c_IRQNType_e = FMKCPU_NVIC_TIM2_IRQN
    },
        [FMKTIM_TIMER_3] = {
        .bspIstc_ps = TIM3,
        .c_clock_e = FMKCPU_RCC_CLK_TIM3,
        .c_IRQNType_e = FMKCPU_NVIC_TIM3_IRQN
    },
        [FMKTIM_TIMER_4] = {
        .bspIstc_ps = TIM4,
        .c_clock_e = FMKCPU_RCC_CLK_TIM4,
        .c_IRQNType_e = FMKCPU_NVIC_TIM4_IRQN
    },
        [FMKTIM_TIMER_5] = {
        .bspIstc_ps = TIM5,
        .c_clock_e = FMKCPU_RCC_CLK_TIM5,
        .c_IRQNType_e = FMKCPU_NVIC_TIM5_IRQN
    },
        [FMKTIM_TIMER_6] = {
        .bspIstc_ps = TIM6,
        .c_clock_e = FMKCPU_RCC_CLK_TIM6,
        .c_IRQNType_e = FMKCPU_NVIC_TIM6_DAC_IRQN
    },
        [FMKTIM_TIMER_7] = {
        .bspIstc_ps = TIM7,
        .c_clock_e = FMKCPU_RCC_CLK_TIM7,
        .c_IRQNType_e = FMKCPU_NVIC_TIM7_DAC_IRQN
    },
        [FMKTIM_TIMER_8] = {
        .bspIstc_ps = TIM8,
        .c_clock_e = FMKCPU_RCC_CLK_TIM8,
        .c_IRQNType_e = FMKCPU_NVIC_TIM8_UP_IRQN
    },
        [FMKTIM_TIMER_15] = {
        .bspIstc_ps = TIM15,
        .c_clock_e = FMKCPU_RCC_CLK_TIM15,
        .c_IRQNType_e = FMKCPU_NVIC_TIM1_BRK_TIM15_IRQN
    },
        [FMKTIM_TIMER_16] = {
        .bspIstc_ps = TIM16,
        .c_clock_e = FMKCPU_RCC_CLK_TIM16,
        .c_IRQNType_e = FMKCPU_NVIC_TIM1_UP_TIM16_IRQN
    },
        [FMKTIM_TIMER_17] = {
        .bspIstc_ps = TIM17,
        .c_clock_e = FMKCPU_RCC_CLK_TIM17,
        .c_IRQNType_e = FMKCPU_NVIC_TIM1_TRG_COM_TIM17_IRQN
    },
        [FMKTIM_TIMER_20] = {
        .bspIstc_ps = TIM20,
        .c_clock_e = FMKCPU_RCC_CLK_TIM20,
        .c_IRQNType_e = FMKCPU_NVIC_TIM20_UP_IRQN
    },
};

    /**< General Purpose Timer Channel Mapping */
    t_sFMKTIM_BspTimerCfg c_FmkTim_ITLineIOMapp_as[FMKTIM_INTERRUPT_LINE_IO_NB] = {
        {FMKTIM_TIMER_1,                        FMKTIM_CHANNEL_1},    // FMKTIM_INTERRUPT_LINE_EVNT_11
        {FMKTIM_TIMER_1,                        FMKTIM_CHANNEL_2},    // FMKTIM_INTERRUPT_LINE_EVNT_12
        {FMKTIM_TIMER_1,                        FMKTIM_CHANNEL_3},    // FMKTIM_INTERRUPT_LINE_EVNT_13
        {FMKTIM_TIMER_1,                        FMKTIM_CHANNEL_4},    // FMKTIM_INTERRUPT_LINE_EVNT_14
        {FMKTIM_TIMER_2,                        FMKTIM_CHANNEL_1},    // FMKTIM_INTERRUPT_LINE_EVNT_21
        {FMKTIM_TIMER_2,                        FMKTIM_CHANNEL_2},    // FMKTIM_INTERRUPT_LINE_EVNT_22
        {FMKTIM_TIMER_2,                        FMKTIM_CHANNEL_3},    // FMKTIM_INTERRUPT_LINE_EVNT_23
        {FMKTIM_TIMER_2,                        FMKTIM_CHANNEL_4},    // FMKTIM_INTERRUPT_LINE_EVNT_24
        {FMKTIM_TIMER_3,                        FMKTIM_CHANNEL_1},    // FMKTIM_INTERRUPT_LINE_EVNT_31
        {FMKTIM_TIMER_3,                        FMKTIM_CHANNEL_2},    // FMKTIM_INTERRUPT_LINE_EVNT_32
        {FMKTIM_TIMER_3,                        FMKTIM_CHANNEL_3},    // FMKTIM_INTERRUPT_LINE_EVNT_33
        {FMKTIM_TIMER_3,                        FMKTIM_CHANNEL_4},    // FMKTIM_INTERRUPT_LINE_EVNT_34
        {FMKTIM_TIMER_4,                        FMKTIM_CHANNEL_1},    // FMKTIM_INTERRUPT_LINE_EVNT_41
        {FMKTIM_TIMER_4,                        FMKTIM_CHANNEL_2},    // FMKTIM_INTERRUPT_LINE_EVNT_42
        {FMKTIM_TIMER_4,                        FMKTIM_CHANNEL_3},    // FMKTIM_INTERRUPT_LINE_EVNT_43
        {FMKTIM_TIMER_4,                        FMKTIM_CHANNEL_4},    // FMKTIM_INTERRUPT_LINE_EVNT_44
        {FMKTIM_TIMER_5,                        FMKTIM_CHANNEL_1},    // FMKTIM_INTERRUPT_LINE_EVNT_51
        {FMKTIM_TIMER_5,                        FMKTIM_CHANNEL_2},    // FMKTIM_INTERRUPT_LINE_EVNT_52
        {FMKTIM_TIMER_5,                        FMKTIM_CHANNEL_3},    // FMKTIM_INTERRUPT_LINE_EVNT_53
        {FMKTIM_TIMER_5,                        FMKTIM_CHANNEL_4},    // FMKTIM_INTERRUPT_LINE_EVNT_54
        {FMKTIM_TIMER_8,                        FMKTIM_CHANNEL_1},    // FMKTIM_INTERRUPT_LINE_EVNT_61
        {FMKTIM_TIMER_8,                        FMKTIM_CHANNEL_2},    // FMKTIM_INTERRUPT_LINE_EVNT_62
        {FMKTIM_TIMER_8,                        FMKTIM_CHANNEL_3},    // FMKTIM_INTERRUPT_LINE_EVNT_63
        {FMKTIM_TIMER_8,                        FMKTIM_CHANNEL_4},    // FMKTIM_INTERRUPT_LINE_EVNT_64
        {FMKTIM_TIMER_15,                       FMKTIM_CHANNEL_1},    // FMKTIM_INTERRUPT_LINE_EVNT_71
        {FMKTIM_TIMER_15,                       FMKTIM_CHANNEL_2},    // FMKTIM_INTERRUPT_LINE_EVNT_72
        {FMKTIM_TIMER_16,                       FMKTIM_CHANNEL_1},    // FMKTIM_INTERRUPT_LINE_EVNT_81
        {FMKTIM_TIMER_17,                       FMKTIM_CHANNEL_1},    // FMKTIM_INTERRUPT_LINE_EVNT_91
        {FMKTIM_TIMER_20,                       FMKTIM_CHANNEL_1},    // FMKTIM_INTERRUPT_LINE_EVNT_101
        {FMKTIM_TIMER_20,                       FMKTIM_CHANNEL_2},    // FMKTIM_INTERRUPT_LINE_EVNT_102
        {FMKTIM_TIMER_20,                       FMKTIM_CHANNEL_3},    // FMKTIM_INTERRUPT_LINE_EVNT_103
        {FMKTIM_TIMER_20,                       FMKTIM_CHANNEL_4},    // FMKTIM_INTERRUPT_LINE_EVNT_104
    };

    /**< Event Purpose Timer Channel Mapping */
    t_sFMKTIM_BspTimerCfg c_FmkTim_ITLineEvntMapp_as[FMKTIM_INTERRUPT_LINE_EVNT_NB] = {
        {FMKTIM_TIMER_6,                       FMKTIM_CHANNEL_1},    // FMKTIM_INTERRUPT_LINE_EVNT_1
        {FMKTIM_TIMER_7,                       FMKTIM_CHANNEL_1},    // FMKTIM_INTERRUPT_LINE_EVNT_2
    };

    /**< Dac Purpose Timer Channel Mapping */
    t_sFMKTIM_BspTimerCfg c_FmkTim_ITLineDacMapp_as[FMKTIM_INTERRUPT_LINE_DAC_NB] = {
    };

    /**< Interrupt Line/Channel Mapping for IO IT Line */
    const t_sFMKTIM_ChnlITLineMapping c_FmkTim_ChnlItLineMapp[FMKTIM_TIMER_NB][FMKTIM_CHANNEL_NB] = {
        [FMKTIM_TIMER_1] = {
            {FMKTIM_INTERRUPT_LINE_TYPE_IO,                     FMKTIM_INTERRUPT_LINE_IO_11},    // FMKTIM_INTERRUPT_LINE_IO_11
            {FMKTIM_INTERRUPT_LINE_TYPE_IO,                     FMKTIM_INTERRUPT_LINE_IO_12},    // FMKTIM_INTERRUPT_LINE_IO_12
            {FMKTIM_INTERRUPT_LINE_TYPE_IO,                     FMKTIM_INTERRUPT_LINE_IO_13},    // FMKTIM_INTERRUPT_LINE_IO_13
            {FMKTIM_INTERRUPT_LINE_TYPE_IO,                     FMKTIM_INTERRUPT_LINE_IO_14},    // FMKTIM_INTERRUPT_LINE_IO_14
        },
        [FMKTIM_TIMER_2] = {
            {FMKTIM_INTERRUPT_LINE_TYPE_IO,                     FMKTIM_INTERRUPT_LINE_IO_21},    // FMKTIM_INTERRUPT_LINE_IO_21
            {FMKTIM_INTERRUPT_LINE_TYPE_IO,                     FMKTIM_INTERRUPT_LINE_IO_22},    // FMKTIM_INTERRUPT_LINE_IO_22
            {FMKTIM_INTERRUPT_LINE_TYPE_IO,                     FMKTIM_INTERRUPT_LINE_IO_23},    // FMKTIM_INTERRUPT_LINE_IO_23
            {FMKTIM_INTERRUPT_LINE_TYPE_IO,                     FMKTIM_INTERRUPT_LINE_IO_24},    // FMKTIM_INTERRUPT_LINE_IO_24
        },
        [FMKTIM_TIMER_3] = {
            {FMKTIM_INTERRUPT_LINE_TYPE_IO,                     FMKTIM_INTERRUPT_LINE_IO_31},    // FMKTIM_INTERRUPT_LINE_IO_31
            {FMKTIM_INTERRUPT_LINE_TYPE_IO,                     FMKTIM_INTERRUPT_LINE_IO_32},    // FMKTIM_INTERRUPT_LINE_IO_32
            {FMKTIM_INTERRUPT_LINE_TYPE_IO,                     FMKTIM_INTERRUPT_LINE_IO_33},    // FMKTIM_INTERRUPT_LINE_IO_33
            {FMKTIM_INTERRUPT_LINE_TYPE_IO,                     FMKTIM_INTERRUPT_LINE_IO_34},    // FMKTIM_INTERRUPT_LINE_IO_34
        },
        [FMKTIM_TIMER_4] = {
            {FMKTIM_INTERRUPT_LINE_TYPE_IO,                     FMKTIM_INTERRUPT_LINE_IO_41},    // FMKTIM_INTERRUPT_LINE_IO_41
            {FMKTIM_INTERRUPT_LINE_TYPE_IO,                     FMKTIM_INTERRUPT_LINE_IO_42},    // FMKTIM_INTERRUPT_LINE_IO_42
            {FMKTIM_INTERRUPT_LINE_TYPE_IO,                     FMKTIM_INTERRUPT_LINE_IO_43},    // FMKTIM_INTERRUPT_LINE_IO_43
            {FMKTIM_INTERRUPT_LINE_TYPE_IO,                     FMKTIM_INTERRUPT_LINE_IO_44},    // FMKTIM_INTERRUPT_LINE_IO_44
        },
        [FMKTIM_TIMER_5] = {
            {FMKTIM_INTERRUPT_LINE_TYPE_IO,                     FMKTIM_INTERRUPT_LINE_IO_51},    // FMKTIM_INTERRUPT_LINE_IO_51
            {FMKTIM_INTERRUPT_LINE_TYPE_IO,                     FMKTIM_INTERRUPT_LINE_IO_52},    // FMKTIM_INTERRUPT_LINE_IO_52
            {FMKTIM_INTERRUPT_LINE_TYPE_IO,                     FMKTIM_INTERRUPT_LINE_IO_53},    // FMKTIM_INTERRUPT_LINE_IO_53
            {FMKTIM_INTERRUPT_LINE_TYPE_IO,                     FMKTIM_INTERRUPT_LINE_IO_54},    // FMKTIM_INTERRUPT_LINE_IO_54
        },
        [FMKTIM_TIMER_6] = {
            {FMKTIM_INTERRUPT_LINE_TYPE_EVNT,                  FMKTIM_INTERRUPT_LINE_EVNT_1},    // FMKTIM_INTERRUPT_LINE_EVNT_1
            {FMKTIM_INTERRUPT_LINE_TYPE_NB,                     FMKTIM_INTERRUPT_LINE_UNUSED},    // NOT AVAILABLE ON HARDWARE
            {FMKTIM_INTERRUPT_LINE_TYPE_NB,                     FMKTIM_INTERRUPT_LINE_UNUSED},    // NOT AVAILABLE ON HARDWARE
            {FMKTIM_INTERRUPT_LINE_TYPE_NB,                     FMKTIM_INTERRUPT_LINE_UNUSED},    // NOT AVAILABLE ON HARDWARE
        },
        [FMKTIM_TIMER_7] = {
            {FMKTIM_INTERRUPT_LINE_TYPE_EVNT,                  FMKTIM_INTERRUPT_LINE_EVNT_2},    // FMKTIM_INTERRUPT_LINE_EVNT_2
            {FMKTIM_INTERRUPT_LINE_TYPE_NB,                     FMKTIM_INTERRUPT_LINE_UNUSED},    // NOT AVAILABLE ON HARDWARE
            {FMKTIM_INTERRUPT_LINE_TYPE_NB,                     FMKTIM_INTERRUPT_LINE_UNUSED},    // NOT AVAILABLE ON HARDWARE
            {FMKTIM_INTERRUPT_LINE_TYPE_NB,                     FMKTIM_INTERRUPT_LINE_UNUSED},    // NOT AVAILABLE ON HARDWARE
        },
        [FMKTIM_TIMER_8] = {
            {FMKTIM_INTERRUPT_LINE_TYPE_IO,                     FMKTIM_INTERRUPT_LINE_IO_61},    // FMKTIM_INTERRUPT_LINE_IO_61
            {FMKTIM_INTERRUPT_LINE_TYPE_IO,                     FMKTIM_INTERRUPT_LINE_IO_62},    // FMKTIM_INTERRUPT_LINE_IO_62
            {FMKTIM_INTERRUPT_LINE_TYPE_IO,                     FMKTIM_INTERRUPT_LINE_IO_63},    // FMKTIM_INTERRUPT_LINE_IO_63
            {FMKTIM_INTERRUPT_LINE_TYPE_IO,                     FMKTIM_INTERRUPT_LINE_IO_64},    // FMKTIM_INTERRUPT_LINE_IO_64
        },
        [FMKTIM_TIMER_15] = {
            {FMKTIM_INTERRUPT_LINE_TYPE_IO,                     FMKTIM_INTERRUPT_LINE_IO_71},    // FMKTIM_INTERRUPT_LINE_IO_71
            {FMKTIM_INTERRUPT_LINE_TYPE_IO,                     FMKTIM_INTERRUPT_LINE_IO_72},    // FMKTIM_INTERRUPT_LINE_IO_72
            {FMKTIM_INTERRUPT_LINE_TYPE_NB,                     FMKTIM_INTERRUPT_LINE_UNUSED},    // NOT AVAILABLE ON HARDWARE
            {FMKTIM_INTERRUPT_LINE_TYPE_NB,                     FMKTIM_INTERRUPT_LINE_UNUSED},    // NOT AVAILABLE ON HARDWARE
        },
        [FMKTIM_TIMER_16] = {
            {FMKTIM_INTERRUPT_LINE_TYPE_IO,                     FMKTIM_INTERRUPT_LINE_IO_81},    // FMKTIM_INTERRUPT_LINE_IO_81
            {FMKTIM_INTERRUPT_LINE_TYPE_NB,                     FMKTIM_INTERRUPT_LINE_UNUSED},    // NOT AVAILABLE ON HARDWARE
            {FMKTIM_INTERRUPT_LINE_TYPE_NB,                     FMKTIM_INTERRUPT_LINE_UNUSED},    // NOT AVAILABLE ON HARDWARE
            {FMKTIM_INTERRUPT_LINE_TYPE_NB,                     FMKTIM_INTERRUPT_LINE_UNUSED},    // NOT AVAILABLE ON HARDWARE
        },
        [FMKTIM_TIMER_17] = {
            {FMKTIM_INTERRUPT_LINE_TYPE_IO,                     FMKTIM_INTERRUPT_LINE_IO_91},    // FMKTIM_INTERRUPT_LINE_IO_91
            {FMKTIM_INTERRUPT_LINE_TYPE_NB,                     FMKTIM_INTERRUPT_LINE_UNUSED},    // NOT AVAILABLE ON HARDWARE
            {FMKTIM_INTERRUPT_LINE_TYPE_NB,                     FMKTIM_INTERRUPT_LINE_UNUSED},    // NOT AVAILABLE ON HARDWARE
            {FMKTIM_INTERRUPT_LINE_TYPE_NB,                     FMKTIM_INTERRUPT_LINE_UNUSED},    // NOT AVAILABLE ON HARDWARE
        },
        [FMKTIM_TIMER_20] = {
            {FMKTIM_INTERRUPT_LINE_TYPE_IO,                     FMKTIM_INTERRUPT_LINE_IO_101},    // FMKTIM_INTERRUPT_LINE_IO_101
            {FMKTIM_INTERRUPT_LINE_TYPE_IO,                     FMKTIM_INTERRUPT_LINE_IO_102},    // FMKTIM_INTERRUPT_LINE_IO_102
            {FMKTIM_INTERRUPT_LINE_TYPE_IO,                     FMKTIM_INTERRUPT_LINE_IO_103},    // FMKTIM_INTERRUPT_LINE_IO_103
            {FMKTIM_INTERRUPT_LINE_TYPE_IO,                     FMKTIM_INTERRUPT_LINE_IO_104},    // FMKTIM_INTERRUPT_LINE_IO_104
        },
    };

    /**< timer max channel variable */
    const t_uint8 c_FMKTIM_TimMaxChnl_ua8[FMKTIM_TIMER_NB] = {
        (t_uint8)FMKTIM_MAX_CHNL_TIMER_1,      // FMKTIM_TIMER_1
        (t_uint8)FMKTIM_MAX_CHNL_TIMER_2,      // FMKTIM_TIMER_2
        (t_uint8)FMKTIM_MAX_CHNL_TIMER_3,      // FMKTIM_TIMER_3
        (t_uint8)FMKTIM_MAX_CHNL_TIMER_4,      // FMKTIM_TIMER_4
        (t_uint8)FMKTIM_MAX_CHNL_TIMER_5,      // FMKTIM_TIMER_5
        (t_uint8)FMKTIM_MAX_CHNL_TIMER_6,      // FMKTIM_TIMER_6
        (t_uint8)FMKTIM_MAX_CHNL_TIMER_7,      // FMKTIM_TIMER_7
        (t_uint8)FMKTIM_MAX_CHNL_TIMER_8,      // FMKTIM_TIMER_8
        (t_uint8)FMKTIM_MAX_CHNL_TIMER_15,     // FMKTIM_TIMER_15
        (t_uint8)FMKTIM_MAX_CHNL_TIMER_16,     // FMKTIM_TIMER_16
        (t_uint8)FMKTIM_MAX_CHNL_TIMER_17,     // FMKTIM_TIMER_17
        (t_uint8)FMKTIM_MAX_CHNL_TIMER_20,     // FMKTIM_TIMER_20
    };

    /* CAUTION : Automatic generated code section for Variable: End */

/* CAUTION : Automatic generated code section for TIMx IRQHandler: Start */
/*********************************
 * TIM1_BRK_TIM15_IRQHandler
*********************************/
void TIM1_BRK_TIM15_IRQHandler(void)      {return HAL_TIM_IRQHandler(FMKTIM_PRIVATE_GetHandleTypeDef((t_uint8)FMKTIM_TIMER_15));}
/*********************************
 * TIM20_TRG_COM_IRQHandler
*********************************/
void TIM20_TRG_COM_IRQHandler(void)       {return HAL_TIM_IRQHandler(FMKTIM_PRIVATE_GetHandleTypeDef((t_uint8)FMKTIM_TIMER_16));}
/*********************************
 * TIM1_TRG_COM_TIM17_IRQHandler
*********************************/
void TIM1_TRG_COM_TIM17_IRQHandler(void)  {return HAL_TIM_IRQHandler(FMKTIM_PRIVATE_GetHandleTypeDef((t_uint8)FMKTIM_TIMER_17));}
/*********************************
 * TIM1_UP_TIM16_IRQHandler
*********************************/
void TIM1_UP_TIM16_IRQHandler(void)       {return HAL_TIM_IRQHandler(FMKTIM_PRIVATE_GetHandleTypeDef((t_uint8)FMKTIM_TIMER_1));}
/*********************************
 * TIM2_IRQHandler
*********************************/
void TIM2_IRQHandler(void)                {return HAL_TIM_IRQHandler(FMKTIM_PRIVATE_GetHandleTypeDef((t_uint8)FMKTIM_TIMER_2));}
/*********************************
 * TIM3_IRQHandler
*********************************/
void TIM3_IRQHandler(void)                {return HAL_TIM_IRQHandler(FMKTIM_PRIVATE_GetHandleTypeDef((t_uint8)FMKTIM_TIMER_3));}
/*********************************
 * TIM4_IRQHandler
*********************************/
void TIM4_IRQHandler(void)                {return HAL_TIM_IRQHandler(FMKTIM_PRIVATE_GetHandleTypeDef((t_uint8)FMKTIM_TIMER_4));}
/*********************************
 * TIM6_DAC_IRQHandler
*********************************/
void TIM6_DAC_IRQHandler(void)            {return HAL_TIM_IRQHandler(FMKTIM_PRIVATE_GetHandleTypeDef((t_uint8)FMKTIM_TIMER_6));}
/*********************************
 * TIM7_DAC_IRQHandler
*********************************/
void TIM7_DAC_IRQHandler(void)            {return HAL_TIM_IRQHandler(FMKTIM_PRIVATE_GetHandleTypeDef((t_uint8)FMKTIM_TIMER_7));}
/*********************************
 * TIM8_UP_IRQHandler
*********************************/
void TIM8_UP_IRQHandler(void)             {return HAL_TIM_IRQHandler(FMKTIM_PRIVATE_GetHandleTypeDef((t_uint8)FMKTIM_TIMER_8));}
/*********************************
 * TIM20_BRK_IRQHandler
*********************************/
void TIM20_BRK_IRQHandler(void)           {return HAL_TIM_IRQHandler(FMKTIM_PRIVATE_GetHandleTypeDef((t_uint8)FMKTIM_TIMER_20));}
/*********************************
 * TIM20_UP_IRQHandler
*********************************/
void TIM20_UP_IRQHandler(void)            {return HAL_TIM_IRQHandler(FMKTIM_PRIVATE_GetHandleTypeDef((t_uint8)FMKTIM_TIMER_20));}
/*********************************
 * TIM20_CC_IRQHandler
*********************************/
void TIM20_CC_IRQHandler(void)            {return HAL_TIM_IRQHandler(FMKTIM_PRIVATE_GetHandleTypeDef((t_uint8)FMKTIM_TIMER_20));}
/* CAUTION : Automatic generated code section for TIMx IRQHandler: End */
    //********************************************************************************
    //                      Public functions - Prototyupes
    //********************************************************************************

#endif // FMKTIM_CONFIGPRIVATE_H_INCLUDED           
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
