/**
 * @file        FMK_CPU.h
 * @brief       Template_BriefDescription.
 * @note        TemplateDetailsDescription.\n
 *
 * @author      xxxxxx
 * @date        jj/mm/yyyy
 * @version     1.0
 */
  
#ifndef FMKTIM_H_INCLUDED
#define FMKTIM_H_INCLUDED





    // ********************************************************************
    // *                      Includes
    // ********************************************************************
    #include "FMK_CFG/FMKCFG_ConfigFiles/FMKTIM_ConfigPublic.h"
    #include "TypeCommon.h"
    // ********************************************************************
    // *                      Defines
    // ********************************************************************
    #define FMKTIM_PWM_MAX_DUTY_CYLCE ((t_uint16)1000) /**< Max duty cycle allowed */
    #define FMKTIM_PWM_MIN_DUTY_CYLCE ((t_uint16)0)    /**< Min dutyCycle allowed */
    // ********************************************************************
    // *                      Types
    // ********************************************************************
    //-----------------------------ENUM TYPES-----------------------------//
    /* CAUTION : Automatic generated code section for Enum: Start */

    /* CAUTION : Automatic generated code section for Enum: End */

    /**< Enum for interruption channel trigger */
    typedef enum 
    {
        FMKTIM_LINE_MEAS_RISING_EDGE = 0x0U,        /**< The channel interrupt will be triggered on rising edge */
        FMKTIM_LINE_MEAS_FALLING_EDGE,              /**< The channel interrupt will be triggered on falling edge */
        FMKTIM_LINE_MEAS_BOTH_EDGE,                 /**< The channel interrupt will be triggered on both edge */

        FMKTIM_LINE_MEAS_NB                         /**< Number of trigger mode */
    }t_eFMKTIM_ChnlMeasTrigger;

    /**< Enum for timer channel  error */
    typedef enum
    {
        FMKTIM_ERRSTATE_OK = 0X00U,                  /**< No error detected */
        FMKTIM_ERRSTATE_INVALID_CHANNEL,             /**< Invalid canal */
        FMKTIM_ERRSTATE_TIMEOUT,                     /**< time waiting excedeed */
        FMKTIM_ERRSTATE_OVERFLOW,                    /**< overflow capacity */
        FMKTIM_ERRSTATE_UNDERFLOW,                   /**< Under flow capacity */
        FMKTIM_ERRSTATE_NOT_CONFIGURED,              /**< timer or channel not configured */
        FMKTIM_ERRSTATE_OFF_UNEXPECTED,              /**< channel is busy */
        FMKTIM_ERRSTATE_INIT_FAILED,                 /**< Failed during intitialize of channel */
        FMKTIM_ERRSTATE_UNKNOWN,                     /**< unknown error detected */

    } t_eFMKTIM_ErrorState;

    /**
    * @brief Slave Timer Channel Polarity.
    */
   typedef enum 
   {
       FMKTIM_LINE_POLARITY_LOW = 0x00,    /**<  The Output is active on Low level*/
       FMKTIM_LINE_POLARITY_HIGH,          /**<  The Output is active on High level*/
       
       FMKTIM_LINE_POLARITY_NB
   } t_eFMKTIM_LinePolarity;

    /**
     * @brief Enum to set bit for changing PWM Signal
     */
    enum 
    {
        FMKTIM_BIT_PWM_FREQUENCY = 0x00,
        FMKTIM_BIT_PWM_DUTYCYCLE,
        FMKTIM_BIT_PWM_NB_PULSES,
    };

    /**
     * @brief Enum to set bit for changing Ecdr Signal
     */
    enum 
    {
        FMKTIM_BIT_ECDR_DIRECTION = 0x00,
        FMKTIM_BIT_ECDR_POSTION,
    };
    /**
     * @brief Enum to set bit for updating Input Compare Line
     */
    enum 
    {
        FMKTIM_BIT_IC_STATE = 0x00,
        FMKTIM_BIT_IC_FREQUENCY,
        FMKTIM_BIT_IC_ARR_REGISTER,
        FMKTIM_BIT_IC_CCRX_REGISTER,
    };

    /**
     * @brief Input Capture Selection
     */
    typedef enum 
    {
        FMKTIM_ICSELECT_DIRECT_TI = 0x00,      /**< TIM Input 1, 2, 3 or 4 is selected to be
                                                         connected to IC1, IC2, IC3 or IC4, respectively */
        FMKTIM_ICSELECT_INDIRECT_TI,           /**< TIM Input 1, 2, 3 or 4 is selected to be
                                                           connected to IC2, IC1, IC4 or IC3, respectively */
        FMKTIM_ICSELECT_TRC,                    /**< TIM Input 1, 2, 3 or 4 is selected to be connected to TRC */
        FMKTIM_ICSELECT_NB,
    } t_eFMKTIM_ICSelection;

    /**
     * @brief Encoder Mode Selection
     */
    typedef enum 
    {
        FMKTIM_ECDR_MODE_TI1 = 0x00,                /**< Quadrature encoder mode 1, x2 mode, counts up/down on TI1FP1 edge depending on TI2FP2 level  */    
        FMKTIM_ECDR_MODE_TI2,                       /*!< Quadrature encoder mode 2, x2 mode, counts up/down on TI2FP2 edge depending on TI1FP1 level. */
        FMKTIM_ECDR_MODE_TI12,                      /**< Quadrature encoder mode 3, x4 mode, counts up/down on both TI1FP1 and TI2FP2 edges depending on the level of the other input */
#if defined(FMKCPU_STM32_ECU_FAMILY_G4)
        FMKTIM_ECDR_MODE_CLOCKPLUS_DIRECTION_X2,    /**< Encoder mode: Clock plus direction, x2 mode */
        FMKTIM_ECDR_MODE_CLOCKPLUS_DIRECTION_X1,    /**< Encoder mode: Clock plus direction, x1 mode, TI2FP2 edge sensitivity is set by CC2P */
        FMKTIM_ECDR_MODE_DIRECTIONAL_CLK_X2,        /**< Encoder mode: Directional Clock, x2 mode */
        FMKTIM_ECDR_MODE_DIRECTIONAL_CLK_X1_TI12,   /**< Encoder mode: Directional Clock, x1 mode, TI1FP1 and TI2FP2 edge sensitivity is set by CC1P and CC2P */
        FMKTIM_ECDR_MODE_X1_TI1,                    /**< Quadrature encoder mode: x1 mode, counting on TI1FP1 edges only, edge sensitivity is set by CC1P */
        FMKTIM_ECDR_MODE_X1_TI12,                   /**< Quadrature encoder mode: x1 mode, counting on TI2FP2 edges only, edge sensitivity is set by CC1P */
#endif // FMKCPU_STM32_ECU_FAMILY_G4
        FMKTIM_ECDR_MODE_NB,                        /**< Number of encoder mode */
    } t_eFMKTIM_EcdrMode;

    /**
     * @brief Encoder Input Polarity Selection
     */
    typedef enum 
    {
        FMKTIM_ECDR_IN_POLARITY_RISING = 0x00,     /**< Encoder input with rising edge polarity  */
        FMKTIM_ECDR_IN_POLARITY_FALLING,           /**< Encoder input with falling edge polarity */

        FMKTIM_ECDR_IN_POLARITY_NB,
    } t_eFMKTIM_EcdrInPolarity;
    /**
     * @brief Encoder Input Polarity Selection
     */
    typedef enum 
    {
        FMKTIM_ECDR_OPE_START_TI1 = 0X00,          /**< Call Bsp Start Function & Enable TI1 (Position or Direction) */
        FMKTIM_ECDR_OPE_START_TI2,                 /**< Call Bsp Start Function & Enable TI2 (Position or Direction) */
        FMKTIM_ECDR_OPE_START_BOTH,                /**< Call Bsp Start Function & Enable Both (Position and Direction) */
        FMKTIM_ECDR_OPE_STOP_TI1,                  /**< Call Bsp Start Function & Enable TI1 (Position or Direction) */
        FMKTIM_ECDR_OPE_STOP_TI2,                  /**< Call Bsp Start Function & Enable TI2 (Position or Direction) */
        FMKTIM_ECDR_OPE_STOP_BOTH,                 /**< Call Bsp Start Function & Enable Both (Position and Direction) */

        FMKTIM_ECDR_OPE_NB
    } t_eFMKTIM_EcdrOpe;

    typedef enum 
    {
        FMKTIM_IC_STATE_ENABLE = 0x00,
        FMKTIM_IC_STATE_DISABLE,

        FMKTIM_IC_STATE_NB,
    } t_eFMKTIM_ICState;

    typedef enum 
    {
        FMKTIM_EVNT_OPE_START_TIMER = 0x00,
        FMKTIM_EVNT_OPE_STOP_TIMER,

        FMKTIM_EVNT_OPE_NB,
    } t_eFMKTIM_EvntOpe;

    typedef struct
    {
        t_uint32 frequency_u32;         /**< update frequency value */
        t_uint16 dutyCycle_u16;         /**< update duty cycle value */
        t_uint16 nbPulses_u16;          /**< update nbPulses_u16 value */
    } t_sFMKTIM_PwmOpe;

    typedef struct 
    {
        t_uint32 frequency_u32;
        t_eFMKTIM_ICState IcState_e;
    } t_sFMKTIM_ICOpe;

    typedef struct 
    {
        t_uint32 position_u32;      /**< Encoder Position Value */
        t_uint8 direction_u8;       /**< Encoder Direction Value */
    } t_sFMKTIM_EncoderValue;
    
    typedef struct
    {
        t_float32 frequency_f32;         /**< get frequency value */
        t_uint16 dutyCycle_u16;         /**< get duty cycle value */
    } t_sFMKTIM_PwmValue;
    
    typedef struct 
    {
        t_float32 frequency_f32;         /**< get frequency value */
        t_uint32 ARR_Register_u32;
        t_uint16 CCRxRegister_u16;
    } t_sFMKTIM_ICValue;

    //-----------------------------TYPEDEF TYPES---------------------------//

    //-----------------------------STRUCT TYPES---------------------------//
    /* CAUTION : Automatic generated code section for Structure: Start */

    /* CAUTION : Automatic generated code section for Structure: End */
    /**
     * @brief Encoder Input Compare Line Configuration
     */
    typedef struct 
    {
        t_eFMKTIM_ICSelection Selection_e;
        t_eFMKTIM_EcdrInPolarity Polarity_e;
    } t_sFMKTIM_ICCfg;

    /**
     * @brief Encoder Configuration
     */
    typedef struct 
    {
        t_eFMKTIM_EcdrMode HwMode_e;        /**< The Encoder Mode */
        t_sFMKTIM_ICCfg IC1_s;              /**< Input Compare Line 1 Configuration */
        t_sFMKTIM_ICCfg IC2_s;              /**< Input Compare Line 2 Configuration */
    } t_sFMKTIM_EcdrCfg;

    /**
    *
    *	@brief  Call back function for channels
    *
    *	@param[in]      f_timer_e : timer channel 
    *	@param[out]     f_channel_e : channel which make the interruption
    *	 
    *
    */
   typedef void (t_cbFMKTIM_InterruptLine)(t_eFMKTIM_InterruptLineType f_InterruptType_e, t_uint8 f_InterruptLine_u8);
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
    *	@brief      Perform all Init action for this module.\n
    *	@note       Set to default value all globals structure that
    *                store information for each signals.\n
    *
    */
    t_eReturnCode FMKTIM_Init(void);
    /**
    *
    *	@brief      Perform all Cyclic action for this module.\n
    *	@note       In preOpe mode this module makes the configuration of the system clock and 
    *               watchdogs configuration.\n 
    *               In ope mode, this module makesdiag on timer channel currently used, and 
    *               called the appropriate callback if an error occured + call APPDEM
    *               to reference error.\n
    *
    */
    t_eReturnCode FMKTIM_Cyclic(void);
    /**
    *
    *	@brief Function to know the module state.\n 
    *
    *	@param[in]  f_State_pe : store the value, value from @ref t_eCyclicModState
    *
    *   @retval RC_OK                             @ref RC_OK
    *   @retval RC_ERROR_PTR_NULL                 @ref RC_ERROR_PTR_NUL
    */
    t_eReturnCode FMKTIM_GetState(t_eCyclicModState *f_State_pe);
    /**
    *
    *	@brief Function to update the module state.\n
    *
    *	@param[in]  f_State_e : the new value, value from @ref t_eCyclicModState
    *
    *   @retval RC_OK                             @ref RC_OK
    */
    t_eReturnCode FMKTIM_SetState(t_eCyclicModState f_State_e);
    /**
    *
    *	@brief      Configure an interrupt line in PWM configuration.\n
    *   @note       First, this configuration set the bsp timer cfg in PWM mode.\n
    *               Once it's done, this function configure the bsp channel in PWM mode too.\n
    *               IMPORTANT, the PWM generation is based on a timer configuration which share
    *               multiple channels, in a sense that, frequency is shared by all PWM channels.\n
    *               In result, the modification of the timer configuration reverbate for all channels.\n     
    *               For Instance, every FMKTIM_INTERRUPT_LINE_IO_0Y, Y belong to [0, Innfini] shared the same timer.\n
    *   @warning    Computation of the frequency takes about 3 µs
    *	@param[in]  f_InterruptLine_e      : enum value for Interrupt Line, value from @ref t_eFMKTIM_InterruptLineIO
    *	@param[in]  f_pwmFreq_u32          : the frequency timer.
    *
    *  @retval RC_OK                             @ref RC_OK
    *  @retval RC_ERROR_PARAM_INVALID            @ref RC_ERROR_PARAM_INVALID
    *  @retval RC_ERROR_WRONG_STATE              @ref RC_ERROR_WRONG_STATE
    *  @retval RC_ERROR_WRONG_RESULT             @ref RC_ERROR_WRONG_RESULT
    */
    t_eReturnCode FMKTIM_Set_PWMLineCfg(t_eFMKTIM_InterruptLineIO f_InterruptLine_e,
                                        t_uint32 f_pwmFreq_u32,
                                        t_eFMKTIM_LinePolarity f_linePolarity_e,
                                        t_cbFMKTIM_InterruptLine * f_PwmPulseFinished_pcb);
    /**
    *
    *	@brief      Configure an interrupt line in Ecdr configuration.\n
    *   @note       First, this configuration set the bsp timer cfg in Ecdr mode.\n
    *               .\n
    *   @warning    The encoder mode impose two use timer x CHANNEL 1 and 2 in order to perform
    *               configuration.\n
    *               Default RunMode configuration is polling 'cause it doesn't affect CPU speed : In
    *               encoder configuration, CPU is charged to put encoder position and direction directly in
    *               the right register. IT & DMA are used to get sample of position & direction which is not useful in 
    *               embeded system (I suppose).\n
    *
    *	@param[in]  f_InterruptLine1_e        : enum value for Interrupt Line 1, value from @ref t_eFMKTIM_InterruptLineIO
    *	@param[in]  f_EcdrCfg_s               : Encoder Configuration.
    *	@param[in]  f_ARRValue_u32            : ARR value link to one revolution of the motor, basically depending on the mode x1, x2, x4
                                                ARR value will be multiply but the ARR Value register is exactly the PPR, which means, the value return is
                                                exactly the position of the motor, 2 * PI radians normalize have to be made. 
    *
    *  @retval RC_OK                             @ref RC_OK
    *  @retval RC_ERROR_PARAM_INVALID            @ref RC_ERROR_PARAM_INVALID
    *  @retval RC_ERROR_WRONG_STATE              @ref RC_ERROR_WRONG_STATE
    *  @retval RC_ERROR_WRONG_RESULT             @ref RC_ERROR_WRONG_RESULT
    */
    t_eReturnCode FMKTIM_Set_EcdrLineCfg(t_eFMKTIM_InterruptLineIO f_InterruptLine_e,
                                            t_sFMKTIM_EcdrCfg f_EcdrCfg_s,
                                            t_uint32 f_ARRValue_u32);
    /**
    *
    *	@brief    Configure a timer channel in Input Compare configuration.\n
    *   @note     The IC Timer configuration is used to measure a signal frequency.\n
    *             This function initialize the timer in Input Compare confgiuration if the
    *             timer is not configured yet.\n
    *             IMPORTANT, the IC config is based on a timer configuration which share
    *             multiple channels, ina sense that, frequency is shared by all PWM channels.\n
    *             In result, the modification of the timer configuration reverbate for all channels.\n   
    *             Once the timer is init correctly, this function set f_channel_e configuration
    *             based on the interrupt mode gives by f_MeasTrigger_e.\n
    *             Finally, if the CPU detect the event, it will call the function f_ITLine_cb.\n
    *
    *	@param[in]  f_InterruptLine_e      : enum value for Interrupt Line, value from @ref t_eFMKTIM_InterruptLineIO
    *	@param[in]  f_MeasTrigger_e        : trigger for interruption, value from @ref t_eFMKTIM_ChnlMeasTrigger
    *	@param[in]  f_ITLine_cb         : call back function to call
    *
    *  @retval RC_OK                             @ref RC_OK
    *  @retval RC_ERROR_PTR_NULL                 @ref RC_ERROR_PTR_NULL
    *  @retval RC_ERROR_PARAM_INVALID            @ref RC_ERROR_PARAM_INVALID
    *  @retval RC_ERROR_ALREADY_CONFIGURED       @ref RC_ERROR_ALREADY_CONFIGURED
    *  @retval RC_ERROR_NOT_ALLOWED              @ref RC_ERROR_NOT_ALLOWED
    */
    t_eReturnCode FMKTIM_Set_ICLineCfg(  t_eFMKTIM_InterruptLineIO f_InterruptLine_e,
                                            t_eFMKTIM_ChnlMeasTrigger f_MeasTrigger_e,
                                            t_cbFMKTIM_InterruptLine * f_ITLine_cb);
    /**
    *
    *	@brief    Configure a timer channel on event configuration.\n
    *   @note     This function initialize the timer in event configuration if the
    *             timer is not configured yet.\n
    *             Once the timer configure is done, update the channel state using function 
    *             "FMKTIM_Set_InterruptLineOpe" and every f_periodms_u32 callback function is called.\n
    *             IMPORTANT, In hardware this is the Timer which manage the interruption with ARR register,
    *             which means, it is best that the timer is a basic timer with only one channel, 'cause the others will
    *             be unused.\n
    *
    *	@param[in]  f_EvntITLine_e      : enum value for Interrupt Line, value from @ref t_eFMKTIM_InterruptLineEvnt
    *	@param[in]  f_periodms_u32            : period before calling function, in millisecond
    *	@param[in]  f_ITLine_cb            : callback function to call
    *
    *  @retval RC_OK                             @ref RC_OK
    *  @retval RC_ERROR_PTR_NULL                 @ref RC_ERROR_PTR_NULL
    *  @retval RC_ERROR_PARAM_INVALID            @ref RC_ERROR_PARAM_INVALID
    *  @retval RC_ERROR_ALREADY_CONFIGURED       @ref RC_ERROR_ALREADY_CONFIGURED
    *  @retval RC_ERROR_NOT_ALLOWED              @ref RC_ERROR_NOT_ALLOWED
    */
    t_eReturnCode FMKTIM_Set_EvntTimerCfg(  t_eFMKTIM_InterruptLineEvnt f_EvntITLine_e,
                                            t_uint32 f_periodms_u32,
                                            t_cbFMKTIM_InterruptLine f_ITLine_cb);
    /**
    *
    *	@brief      Set a InterruptLine  state ON/OFF.\n
    *   @note       Using HAL_TIM function & Run mode Configured previously 
    *               in API CfgF Function, we start_poll/It/Dma the Line
    *           
    *
    *	@param[in]  f_ITLine_e              : enum value for the timer, value from @ref t_eFMKTIM_InterruptLineIO
    *	@param[in]  f_EcdrState_e            : enum value for the channel, value from @ref t_eFMKTIM_EcdrStartOption
    *
    *  @retval RC_OK                             @ref RC_OK
    *  @retval RC_ERROR_PARAM_INVALID            @ref RC_ERROR_PARAM_INVALID
    *  @retval RC_ERROR_WRONG_STATE              @ref RC_ERROR_WRONG_STATE
    */
    t_eReturnCode FMKTIM_Set_PwmLineValue(   t_eFMKTIM_InterruptLineIO f_Itline_e,
                                            t_sFMKTIM_PwmOpe f_PwmOpe_s,
                                            t_uint8 f_maskUpdate_u8);
    /**
    *
    *	@brief      Set a InterruptLine  state ON/OFF.\n
    *   @note       Using HAL_TIM function & Run mode Configured previously 
    *               in API CfgF Function, we start_poll/It/Dma the Line
    *           
    *
    *	@param[in]  f_ITLine_e              : enum value for the timer, value from @ref t_eFMKTIM_InterruptLineIO
    *	@param[in]  f_EcdrState_e            : enum value for the channel, value from @ref t_eFMKTIM_EcdrStartOption
    *
    *  @retval RC_OK                             @ref RC_OK
    *  @retval RC_ERROR_PARAM_INVALID            @ref RC_ERROR_PARAM_INVALID
    *  @retval RC_ERROR_WRONG_STATE              @ref RC_ERROR_WRONG_STATE
    */
    t_eReturnCode FMKTIM_Set_ICLineValue(   t_eFMKTIM_InterruptLineIO f_Itline_e,
                                            t_sFMKTIM_ICOpe         f_ICOpe_s,
                                            t_uint8                 f_mask_u8);
    /**
    *
    *	@brief      Set a InterruptLine  state ON/OFF.\n
    *   @note       Using HAL_TIM function & Run mode Configured previously 
    *               in API CfgF Function, we start_poll/It/Dma the Line
    *           
    *
    *	@param[in]  f_ITLine_e              : enum value for the timer, value from @ref t_eFMKTIM_InterruptLineIO
    *	@param[in]  f_EcdrState_e            : enum value for the channel, value from @ref t_eFMKTIM_EcdrStartOption
    *
    *  @retval RC_OK                             @ref RC_OK
    *  @retval RC_ERROR_PARAM_INVALID            @ref RC_ERROR_PARAM_INVALID
    *  @retval RC_ERROR_WRONG_STATE              @ref RC_ERROR_WRONG_STATE
    */
    t_eReturnCode FMKTIM_Set_EcdrLineState( t_eFMKTIM_InterruptLineIO f_Itline_e,
                                            t_eFMKTIM_EcdrOpe f_EcdrOpe);
    /**
    *
    *	@brief      Set a InterruptLine  state ON/OFF.\n
    *   @note       Using HAL_TIM function & Run mode Configured previously 
    *               in API CfgF Function, we start_poll/It/Dma the Line
    *           
    *
    *	@param[in]  f_ITLine_e              : enum value for the timer, value from @ref t_eFMKTIM_InterruptLineIO
    *	@param[in]  f_EcdrState_e            : enum value for the channel, value from @ref t_eFMKTIM_EcdrStartOption
    *
    *  @retval RC_OK                             @ref RC_OK
    *  @retval RC_ERROR_PARAM_INVALID            @ref RC_ERROR_PARAM_INVALID
    *  @retval RC_ERROR_WRONG_STATE              @ref RC_ERROR_WRONG_STATE
    */
    t_eReturnCode FMKTIM_Set_EvntLineState(  t_eFMKTIM_InterruptLineEvnt f_EvntITLine_e,
                                            t_eFMKTIM_EvntOpe f_EvntOpe);

    /**
    *
    *	@brief      Set a InterruptLine  state ON/OFF.\n
    *   @note       Using HAL_TIM function & Run mode Configured previously 
    *               in API CfgF Function, we start_poll/It/Dma the Line
    *           
    *
    *	@param[in]  f_ITLine_e              : enum value for the timer, value from @ref t_eFMKTIM_InterruptLineIO
    *	@param[in]  f_EcdrState_e            : enum value for the channel, value from @ref t_eFMKTIM_EcdrStartOption
    *
    *  @retval RC_OK                             @ref RC_OK
    *  @retval RC_ERROR_PARAM_INVALID            @ref RC_ERROR_PARAM_INVALID
    *  @retval RC_ERROR_WRONG_STATE              @ref RC_ERROR_WRONG_STATE
    */
    t_eReturnCode FMKTIM_Get_PwmLineValue(  t_eFMKTIM_InterruptLineIO f_Itline_e,
                                            t_sFMKTIM_PwmValue * f_PwmValue_ps,
                                            t_uint8 f_mask_u8);
    /**
    *
    *	@brief      Set a InterruptLine  state ON/OFF.\n
    *   @note       Using HAL_TIM function & Run mode Configured previously 
    *               in API CfgF Function, we start_poll/It/Dma the Line
    *           
    *
    *	@param[in]  f_ITLine_e              : enum value for the timer, value from @ref t_eFMKTIM_InterruptLineIO
    *	@param[in]  f_EcdrState_e            : enum value for the channel, value from @ref t_eFMKTIM_EcdrStartOption
    *
    *  @retval RC_OK                             @ref RC_OK
    *  @retval RC_ERROR_PARAM_INVALID            @ref RC_ERROR_PARAM_INVALID
    *  @retval RC_ERROR_WRONG_STATE              @ref RC_ERROR_WRONG_STATE
    */
    t_eReturnCode FMKTIM_Get_EcdrLineValue( t_eFMKTIM_InterruptLineIO f_Itline_e,
                                            t_uint32 * f_position_u32,
                                            t_uint32 * f_direction_u32);
    /**
    *
    *	@brief      Set a InterruptLine  state ON/OFF.\n
    *   @note       Using HAL_TIM function & Run mode Configured previously 
    *               in API CfgF Function, we start_poll/It/Dma the Line
    *           
    *
    *	@param[in]  f_ITLine_e              : enum value for the timer, value from @ref t_eFMKTIM_InterruptLineIO
    *	@param[in]  f_EcdrState_e            : enum value for the channel, value from @ref t_eFMKTIM_EcdrStartOption
    *
    *  @retval RC_OK                             @ref RC_OK
    *  @retval RC_ERROR_PARAM_INVALID            @ref RC_ERROR_PARAM_INVALID
    *  @retval RC_ERROR_WRONG_STATE              @ref RC_ERROR_WRONG_STATE
    */
    t_eReturnCode FMKTIM_Get_ICLineValue(   t_eFMKTIM_InterruptLineIO f_Itline_e,
                                            t_sFMKTIM_ICValue * ICValue_ps,
                                            t_uint8 f_mask_u8);

    /**
    *
    *	@brief      Add a callback function to a timer channel.\n
    *   @note       For the configuration where the callback is optionnal (PWM, OP,etc)
    *               This function allow to add a callback. 
    *               On PWM -> callback will be call whenever the PWM pulse generation (not implemented yet) is done 
    *               On OnePulse -> Not implemented
    *           
    *
    *	@param[in]  f_InterruptLine_e      : enum value for Interrupt Line, value from @ref t_eFMKTIM_InterruptLineIO
    *	@param[in]  f_ITLine_cb      : the frequency timer.
    *
    *  @retval RC_OK                             @ref RC_OK
    *  @retval RC_ERROR_PARAM_INVALID            @ref RC_ERROR_PARAM_INVALID
    *  @retval RC_ERROR_WRONG_STATE              @ref RC_ERROR_WRONG_STATE
    */
    t_eReturnCode FMKTIM_AddInterruptCallback(  t_eFMKTIM_InterruptLineIO f_InterruptLine_e,
                                                t_cbFMKTIM_InterruptLine *f_ITLine_cb);
    /**
    *
    *	@brief      Function to get the error code for a timer_channel
    *
    *	@param[in]  f_timer_e                : enum value for the timer, value from @ref t_eFMKTIM_Timer
    *	@param[in]  f_channel_e              : enum value for the channel, value from @ref t_eFMKTIM_InterruptChnl
    *	@param[in]  f_chnlErrInfo_pu16       : storage for channel error.\n
    *
    *  @retval RC_OK                             @ref RC_OK
    *  @retval RC_ERROR_PARAM_INVALID            @ref RC_ERROR_PARAM_INVALID
    *  @retval RC_ERROR_PTR_NULL                 @ref RC_ERROR_PTR_NULL
    *
    */
    t_eReturnCode FMKTIM_Get_LineErrorStatus(t_eFMKTIM_InterruptLineType f_ITLineType_e,
                                                t_uint32 f_IT_line_u8,
                                                t_uint16 *f_chnlErrInfo_pu16);
    /**
    *
    *	@brief      DO NOT USE, Function to get the Handle Type def for IRQN_Handler Function
    *
    *	@param[in]  f_timer_e                : enum value for the timer, value from @ref t_eFMKTIM_Timer
    *
    *
    */
    TIM_HandleTypeDef * FMKTIM_PRIVATE_GetHandleTypeDef(t_uint8 f_timer_u8);

#endif // FMKTIM_H_INCLUDED           
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
