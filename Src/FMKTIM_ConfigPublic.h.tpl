/*********************************************************************
 * @file        FMKTIM_ConfigPublic.h
 * @brief       Template_BriefDescription.
 * @note        TemplateDetailsDescription.\n
 *
 * @author      mba
 * @date        31/08/2024
 * @version     1.0
 */
  
#ifndef FMKTIM_CONFIGPUBLIC_H_INCLUDED
#define FMKTIM_CONFIGPUBLIC_H_INCLUDED





    // ********************************************************************
    // *                      Includes
    // ********************************************************************
    #include "FMK_CFG/FMKCFG_ConfigFiles/FMKCPU_ConfigPublic.h"
    #include "TypeCommon.h"
    #include "Constant.h"
    
    // ********************************************************************
    // *                      Defines
    // ********************************************************************
    #define FMKTIM_TIMER_PWM_ARR_TARGET_16_BIT   ((t_uint32)0xF80C) // 63500
    #define FMKTIM_TIMER_PWM_ARR_TARGET_32_BIT   ((t_uint32)(CST_MAX_UINT_32BIT - (t_uint32)1)) 
    #define FMKTIM_FREQ_COMPUTE_DELTA_ACCEPTANCE ((t_float32)0.5)
    #define FMKTIM_ARR_LOW_LIMIT_16BIT  ((t_uint32)0xC350)     // 50000
    #define FMKTIM_ARR_HIGH_LIMIT_16BIT ((t_uint32)0xFFFE)     // 65534
    #define FMKTIM_ARR_LOW_LIMIT_32BIT  ((t_uint32)0xB2D05E00) // 3_000_000_000
    #define FMKTIM_ARR_HIGH_LIMIT_32BIT ((t_uint32)0xFFFFFFFE) // 4_294_967_295
    #define FMKTIM_WWDG_RESET_CFG  FMKTIM_WWDG_RESET_100MS /**< default watchdogs configuration */

    /**
    * @brief This define return True if the timer is a 32 Bits timers
    */
    #define FMKTIM_IS_32B_TIMER(f_timClock_e) \
        ((f_timClock_e) == FMKCPU_RCC_CLK_TIM2 ||\
         (f_timClock_e) == FMKCPU_RCC_CLK_TIM5)
    // ********************************************************************
    // *                      Types
    // ********************************************************************

    /**< typedef enum for channel mode */
    typedef enum 
    {
        FMKTIM_HWTIM_CFG_PWM = 0x0U,        /**< Timer configuration in PWM mode */
        FMKTIM_HWTIM_CFG_IC,                /**< Timer configuration in Input Capture mode */
        FMKTIM_HWTIM_CFG_OC,                /**< Timer configuration in Ouput Compare mode */
        FMKTIM_HWTIM_CFG_OP,                /**< Timer configuration in One Pulse mode */
        FMKTIM_HWTIM_CFG_EVNT,              /**< Timer configuration in Event mode */
        FMKTIM_HWTIM_CFG_ECDR,              /**< Timer configuration in encoder mode */
        FMKTIM_HWTIM_CFG_DAC,               /**< Timer configuration in Dac mode */

        FMKTIM_HWTIM_CFG_NB,                /**< Number of timer configuration mode*/
    } t_eFMKTIM_HwTimerCfg;

    //-----------------------------ENUM TYPES-----------------------------//
    /**< Interrupt Line Type */
    typedef enum 
    {
        FMKTIM_INTERRUPT_LINE_TYPE_IO = 0x00,       /**< Timer/Channel use for PWM, IC, OC, OP Purpose */
        FMKTIM_INTERRUPT_LINE_TYPE_EVNT,            /**< Timer/Channel use for Event purpose */
        FMKTIM_INTERRUPT_LINE_TYPE_DAC,             /**< Timmer/Channel use for DAC purpose */
        FMKTIM_INTERRUPT_LINE_TYPE_NB,
    } t_eFMKTIM_InterruptLineType;
    /* CAUTION : Automatic generated code section for Enum: Start */
    /**
    * @brief Number of General Purpose Interrupt Line, for PWM, Input-Compare, Output Compare, One sPulse.
    */
    typedef enum
    {
        FMKTIM_INTERRUPT_LINE_IO_11 = 0,                 /**< General Purpose Timer, Reference to Timer 1 Channel 1 */
        FMKTIM_INTERRUPT_LINE_IO_12,                       /**< General Purpose Timer, Reference to Timer 1 Channel 2 */
        FMKTIM_INTERRUPT_LINE_IO_13,                       /**< General Purpose Timer, Reference to Timer 1 Channel 3 */
        FMKTIM_INTERRUPT_LINE_IO_14,                       /**< General Purpose Timer, Reference to Timer 1 Channel 4 */
        FMKTIM_INTERRUPT_LINE_IO_21,                       /**< General Purpose Timer, Reference to Timer 2 Channel 1 */
        FMKTIM_INTERRUPT_LINE_IO_22,                       /**< General Purpose Timer, Reference to Timer 2 Channel 2 */
        FMKTIM_INTERRUPT_LINE_IO_23,                       /**< General Purpose Timer, Reference to Timer 2 Channel 3 */
        FMKTIM_INTERRUPT_LINE_IO_24,                       /**< General Purpose Timer, Reference to Timer 2 Channel 4 */
        FMKTIM_INTERRUPT_LINE_IO_31,                       /**< General Purpose Timer, Reference to Timer 3 Channel 1 */
        FMKTIM_INTERRUPT_LINE_IO_32,                       /**< General Purpose Timer, Reference to Timer 3 Channel 2 */
        FMKTIM_INTERRUPT_LINE_IO_33,                       /**< General Purpose Timer, Reference to Timer 3 Channel 3 */
        FMKTIM_INTERRUPT_LINE_IO_34,                       /**< General Purpose Timer, Reference to Timer 3 Channel 4 */
        FMKTIM_INTERRUPT_LINE_IO_41,                       /**< General Purpose Timer, Reference to Timer 4 Channel 1 */
        FMKTIM_INTERRUPT_LINE_IO_42,                       /**< General Purpose Timer, Reference to Timer 4 Channel 2 */
        FMKTIM_INTERRUPT_LINE_IO_43,                       /**< General Purpose Timer, Reference to Timer 4 Channel 3 */
        FMKTIM_INTERRUPT_LINE_IO_44,                       /**< General Purpose Timer, Reference to Timer 4 Channel 4 */
        FMKTIM_INTERRUPT_LINE_IO_51,                       /**< General Purpose Timer, Reference to Timer 5 Channel 1 */
        FMKTIM_INTERRUPT_LINE_IO_52,                       /**< General Purpose Timer, Reference to Timer 5 Channel 2 */
        FMKTIM_INTERRUPT_LINE_IO_53,                       /**< General Purpose Timer, Reference to Timer 5 Channel 3 */
        FMKTIM_INTERRUPT_LINE_IO_54,                       /**< General Purpose Timer, Reference to Timer 5 Channel 4 */
        FMKTIM_INTERRUPT_LINE_IO_61,                       /**< General Purpose Timer, Reference to Timer 8 Channel 1 */
        FMKTIM_INTERRUPT_LINE_IO_62,                       /**< General Purpose Timer, Reference to Timer 8 Channel 2 */
        FMKTIM_INTERRUPT_LINE_IO_63,                       /**< General Purpose Timer, Reference to Timer 8 Channel 3 */
        FMKTIM_INTERRUPT_LINE_IO_64,                       /**< General Purpose Timer, Reference to Timer 8 Channel 4 */
        FMKTIM_INTERRUPT_LINE_IO_71,                       /**< General Purpose Timer, Reference to Timer 15 Channel 1 */
        FMKTIM_INTERRUPT_LINE_IO_72,                       /**< General Purpose Timer, Reference to Timer 15 Channel 2 */
        FMKTIM_INTERRUPT_LINE_IO_81,                       /**< General Purpose Timer, Reference to Timer 16 Channel 1 */
        FMKTIM_INTERRUPT_LINE_IO_91,                       /**< General Purpose Timer, Reference to Timer 17 Channel 1 */
        FMKTIM_INTERRUPT_LINE_IO_101,                      /**< General Purpose Timer, Reference to Timer 20 Channel 1 */
        FMKTIM_INTERRUPT_LINE_IO_102,                      /**< General Purpose Timer, Reference to Timer 20 Channel 2 */
        FMKTIM_INTERRUPT_LINE_IO_103,                      /**< General Purpose Timer, Reference to Timer 20 Channel 3 */
        FMKTIM_INTERRUPT_LINE_IO_104,                      /**< General Purpose Timer, Reference to Timer 20 Channel 4 */
    
        FMKTIM_INTERRUPT_LINE_IO_NB,
    } t_eFMKTIM_InterruptLineIO;

    /**
    * @brief Number of Event Purpose Interrupt Line.
    */
    typedef enum
    {
        FMKTIM_INTERRUPT_LINE_EVNT_1 = 0,                  /**< Event Purpose Timer, Reference to Timer 6 Channel 1 */
        FMKTIM_INTERRUPT_LINE_EVNT_2,                        /**< Event Purpose Timer, Reference to Timer 7 Channel 1 */
    
        FMKTIM_INTERRUPT_LINE_EVNT_NB,
    } t_eFMKTIM_InterruptLineEvnt;

    /**
    * @brief Number of DAC Purpose Interrupt Line.
    */
    typedef enum
    {
    
        FMKTIM_INTERRUPT_LINE_DAC_NB,
    } t_eFMKTIM_InterruptLineDAC;

    /* CAUTION : Automatic generated code section for Enum: End */

    //-----------------------------TYPEDEF TYPES---------------------------//
    // ********************************************************************
    // *                      Prototypes
    // ********************************************************************
        
    // ********************************************************************
    // *                      Variables
    // ********************************************************************

    //********************************************************************************
    //                      Public functions - Prototyupes
    //********************************************************************************

#endif // FMKTIM_CONFIGPUBLIC_H_INCLUDED           
//************************************************************************************
// End of File
//************************************************************************************

/**
 *
 *	@brief
 *	@note   
 *
 *
 *	@param[in] 
 *	@param[out]
 *	 
 *
 *
 */
