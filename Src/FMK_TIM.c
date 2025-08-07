/**
 * @file        FMK_CPU.c
 * @brief       Framework for Unit Processing Control.
 * @note        TemplateDetailsDescription.\n
 *
 * @author      mba
 * @date        25/08/2024
 * @version     1.0
 */

// ********************************************************************
// *                      Includes
// ********************************************************************
#include "./FMK_TIM.h"
#include "FMK_CFG/FMKCFG_ConfigFiles/FMKTIM_ConfigPrivate.h"
#include "FMK_HAL/FMK_CPU/Src/FMK_CPU.h"

#include "APP_CTRL/APP_SYS/Src/APP_SYS.h"
#include "3_APP/APP_CTRL/APP_SDM/Src/APP_SDM.h"
#include "Library/SafeMem/SafeMem.h"
#include "Constant.h"
// ********************************************************************
// *                      Defines
// ********************************************************************

// ********************************************************************
// *                      Types
// ********************************************************************

// ********************************************************************
// *                      Types
// ********************************************************************
/**< Enum  for channel state */
typedef enum
{
    FMKTIM_CHNLST_DISACTIVATED = 0x0U,       /**< the channel is disactivated by software, not running */
    FMKTIM_CHNLST_ACTIVATED,                 /**< The channel is acitvated by software, is running */

    FMKTIM_CHNLST_NB                         /**< Number of channel state */
} t_eFMKTIM_ChnlState;


/**< Enum for channel run mode */
typedef enum 
{
    FMKTIM_LINE_RUNMODE_POLLING = 0x0U,     /**< The channel is running in polling mode (standard mode) */
    FMKTIM_LINE_RUNMODE_INTERRUPT,          /**< The channel is running in interrupt mode, which means HAL error callback is active,
                                                among other things*/
    FMKTIM_LINE_RUNMODE_DMA,                /**< The channel is running in DMA Mode */

    FMKTIM_LINE_RUNMODE_NB                  /**< Number of channel run mode take in charge (DMA currently not available) */
} t_eFMKTIM_ChnlRunMode;

typedef enum 
{
    FMKTIM_BSP_CB_PERIOD_ELAPSED = 0x00,
    FMKTIM_BSP_CB_IC_CAPTURE,
    FMKTIM_BSP_CB_OC_DELAY_ELAPSED,
    FMKTIM_BSP_CB_PWM_PULSE_FINISHED,
    FMKTIM_BSP_CB_ERROR,

    FMKTIM_BSP_CB_NB,
} t_eFMKTIM_BspEvent;
//-----------------------------ENUM TYPES-----------------------------//

/* CAUTION : Automatic generated code section for Enum: Start */

/* CAUTION : Automatic generated code section for Enum: End */
//-----------------------------TYPEDEF TYPES---------------------------//
//-----------------------------STRUCT TYPES---------------------------//
typedef struct 
{
    t_uint32 * BufferAdd1_pu32; /**< Dma Buffer Address 1 */
    t_uint32 * BufferAdd2_pu32; /**< Dma Buffer Address 2 */
    t_uint16 bufferLen_u16;
    t_eFMKCPU_DmaRqst RqstId_e;
} t_sFMKTIM_ChnlDmaInfo;

/**< Structure of information on a timer-channel*/
typedef struct
{
    t_eFMKTIM_InterruptChnl selfId_e;                   /**< useful when API pass the all structure as argument */
    t_eFMKTIM_ChnlState State_e;                        /**< State of the channel */
    t_eFMKTIM_ChnlRunMode RunMode_e;                    /**< Run mode for the channel */
    t_sFMKTIM_ChnlDmaInfo DmaInfo_ps;
    t_eFMKTIM_ErrorState ErrState_e;                /**< Error state of the channel */
    t_cbFMKTIM_InterruptLine *chnl_cb;                  /**< Callback function of the channel */
    t_bool IsChnlConfigure_b;                           /**< Wether or not the channel has beeen configured */
    
} t_sFMKTIM_ChnlInfo;

/**< Structure of information on a timer*/
typedef struct
{
    TIM_HandleTypeDef * bspTimer_ps;                       /**< The Timer structure for HAL STM32*/
    t_eFMKTIM_HwTimerCfg HwCfg_e;                       /**< The hardware configuration of the timer */
    t_eFMKTIM_ErrorState errorId_e;                       /**< Flag to know if which error occured on the timer */
    t_uint16 timerFreqMHz_u32;                              /**< Timer frequency in Mhz */
    t_uint16 mskChnlState_u16;
    //const t_eFMKCPU_DmaRqst c_RqstDma_e;               /**< RCC clock related to the timer */
    t_bool IsTimerRunning_b;                            /**< Flag for timer is runnning */
    t_bool isConfigured_b;                              /**< flag timer is configured */  
    t_bool IsNVICTimerEnable_b;                         /**< flag timer NVCIC enable or not */
    t_bool ignoreInterrupt_b;                            /**< Flag to ignore interruption */
    t_bool errDetected_b;                               /**< Flag to know whnever an error is collected from the bsp */
    t_uint32 lastCbError_u32;                           /**< register the last time an error occured */             
    t_eFMKCPU_ClockPort c_clock_e;
    t_eFMKCPU_IRQNType c_IRQNType_e;

} t_sFMKTIM_TimerInfo;
/* CAUTION : Automatic generated code section for Structure: Start */

/* CAUTION : Automatic generated code section for Structure: End */



// ********************************************************************
// *                      Prototypes
// ********************************************************************

// ********************************************************************
// *                      Variables
// ********************************************************************
static t_eCyclicModState g_FmkTim_ModState_e = STATE_CYCLIC_CFG;

///@brief bsp Timer Instance Declaration 
TIM_HandleTypeDef g_bspTimerHandle_as[FMKTIM_TIMER_NB];

///@brief timer info container 
static t_sFMKTIM_TimerInfo g_TimerInfo_as[FMKTIM_TIMER_NB];
///@brief timer info access Read/Write 
static t_sSafeMem_BlockInfo g_sfmb_TimerInfo_as[FMKTIM_TIMER_NB];

/// @brief chanell info container 
static t_sFMKTIM_ChnlInfo g_TimChnlInfo_as[FMKTIM_TIMER_NB][FMKTIM_CHANNEL_NB];
///@brief channel info accessors
static t_sSafeMem_BlockInfo g_sfmb_TimChnlInfo_as[FMKTIM_TIMER_NB][FMKTIM_CHANNEL_NB];
//********************************************************************************
//                      Local functions - Prototypes
//********************************************************************************
/**
 *
 *	@brief      Function to set the bsp timer init container 
 *  @note       Depending on f_hwTimCfg_e and other variable, 
 *              this function filled f_timer_ps with cfg variable.\n
 *              This function set the rcc clock if needed and call HAL_Init function
 *
 *	@param[in]  f_timer_ps            : enum for the timer,value from @ref t_sFMKTIM_TimerInfo
 *	@param[in]  f_hwTimCfg_e           : enum for the hardware timer cfg,value from @ref t_eFMKTIM_HwTimerCfg
 *	@param[in]  f_InfoInit_u32       : the prescaler for the clock timer reference
 *	@param[in]  f_timerCfg_pv         : Specific Configuration for timer Init 
 *
 *  @retval RC_OK                             @ref RC_OK
 *  @retval RC_ERROR_PARAM_INVALID            @ref RC_ERROR_PARAM_INVALID
 *  @retval RC_ERROR_PTR_NULL                 @ref RC_ERROR_PTR_NULL
 *  @retval RC_ERROR_WRONG_RESULT             @ref RC_ERROR_WRONG_RESULT
 *
 */
static t_eReturnCode s_FMKTIM_Set_BspTimerInit( t_sFMKTIM_TimerInfo * f_timer_ps,
                                                t_eFMKTIM_HwTimerCfg f_hwTimCfg_e,
                                                t_uint32 f_InfoInit_u32,
                                                void * f_timerCfg_pv);
/**
    *
    *	@brief      Configure a timer channel in PWM configuration.\n
    *   @note       First, this configuration set the bsp timer cfg in PWM mode.\n
    *               Once it's done, this function configure the bsp channel in PWM mode too.\n
    *               IMPORTANT, the PWM generation is based on a timer configuration which share
    *               multiple channels, ina sense that, frequency is shared by all PWM channels.\n
    *               In result, the modification of the timer configuration reverbate for all channels.\n          
    *
    *	@param[in]  f_InterruptLine_e      : enum value for timer, value from @ref t_eFMKTIM_Timer
    *	@param[in]  f_channel_e            : enum value for channel, , value from @ref f_channel_e
    *	@param[in]  f_pwmFreq_u32          : the frequency timer.
    *
    *  @retval RC_OK                             @ref RC_OK
    *  @retval RC_ERROR_PARAM_INVALID            @ref RC_ERROR_PARAM_INVALID
    *  @retval RC_ERROR_WRONG_STATE              @ref RC_ERROR_WRONG_STATE
    *  @retval RC_ERROR_WRONG_RESULT             @ref RC_ERROR_WRONG_RESULT
    */
static t_eReturnCode s_FMKTIM_Set_PwmChannelCfg(t_sFMKTIM_TimerInfo * f_timerInfo_ps,
                                                t_sFMKTIM_ChnlInfo * f_chnlInfo_ps,
                                                t_uint32 f_pwmFreq_u32,
                                                t_eFMKTIM_LinePolarity f_linePolarity_e);
/**
*
*	@brief      Configure a timer channel in Encoder configuration.\n
*   @note       
*
*	@param[in]  f_InterruptLine_e      : enum value for timer, value from @ref t_eFMKTIM_Timer
*	@param[in]  f_EcdrCdg_ps           : Pointor to Encoder Configuration
*
*  @retval RC_OK                             @ref RC_OK
*  @retval RC_ERROR_PARAM_INVALID            @ref RC_ERROR_PARAM_INVALID
*  @retval RC_ERROR_WRONG_STATE              @ref RC_ERROR_WRONG_STATE
*  @retval RC_ERROR_WRONG_RESULT             @ref RC_ERROR_WRONG_RESULT
*/
static t_eReturnCode s_FMKTIM_Set_EcdrChannelCfg(   t_sFMKTIM_TimerInfo * f_timerInfo_ps,
                                                    t_sFMKTIM_EcdrCfg * f_EcdrCfg_ps,
                                                    t_uint32 f_ARRValue_u32);
/**
*
*	@brief      Get Bsp Information from Cfg and Filled Bsp structure.\n
*   @note       
*
*	@param[in]  f_EcdrCdg_ps           : Software Configuration for Encoders
*	@param[in]  f_BspEcdrInit_ps           : Bsp Configuration for Encoders
*
*  @retval RC_OK                             @ref RC_OK
*  @retval RC_ERROR_PARAM_INVALID            @ref RC_ERROR_PARAM_INVALID
*  @retval RC_ERROR_WRONG_STATE              @ref RC_ERROR_WRONG_STATE
*  @retval RC_ERROR_WRONG_RESULT             @ref RC_ERROR_WRONG_RESULT
*/
static t_eReturnCode s_FMKTIM_FilledBspEcdrInit(t_sFMKTIM_EcdrCfg * f_EcdrCdg_ps,
                                                TIM_Encoder_InitTypeDef * f_BspEcdrInit_ps);
                                                
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
*             Finally, if the CPU detect the event, it will call the function f_ITChannel_cb.\n
*
*	@param[in]  f_timer_e              : enum value for the priority, value from @ref t_eFMKTIM_Timer
*	@param[in]  f_channel_e            : enum value for the channel, value from @ref t_eFMKTIM_InterruptChnl
*	@param[in]  f_MeasTrigger_e        : trigger for interruption, value from @ref t_eFMKTIM_ChnlMeasTrigger
*	@param[in]  f_ITChannel_cb         : call back function to call
*
*  @retval RC_OK                             @ref RC_OK
*  @retval RC_ERROR_PTR_NULL                 @ref RC_ERROR_PTR_NULL
*  @retval RC_ERROR_PARAM_INVALID            @ref RC_ERROR_PARAM_INVALID
*  @retval RC_ERROR_ALREADY_CONFIGURED       @ref RC_ERROR_ALREADY_CONFIGURED
*  @retval RC_ERROR_NOT_ALLOWED              @ref RC_ERROR_NOT_ALLOWED
*/
static t_eReturnCode s_FMKTIM_Set_ICChannelCfg( t_sFMKTIM_TimerInfo * f_timerInfo_ps,
                                                t_sFMKTIM_ChnlInfo * f_chnlInfo_ps,
                                                t_eFMKTIM_ChnlMeasTrigger f_MeasTrigger_e,
                                                t_cbFMKTIM_InterruptLine f_ITChannel_cb);
/**
*
*	@brief    Configure a timer channel on event configuration.\n
*   @note     This function initialize the timer in event configuration if the
*             timer is not configured yet.\n
*             Once the timer configure is done, update the channel state using function 
*             "FMKTIM_Set_EvntTimerState" and every f_periodms_u32 callback function is called.\n
*             IMPORTANT, In hardware this is the Timer which manage the interruption with ARR register,
*             In software this is channel structure who manage interruption to make all timer identical 
*             in just in variable.\n
*
*	@param[in]  f_evntChannel_e           : enum value for event channel, value from @ref f_timer_e
*	@param[in]  f_periodms_u32            : period before calling function, in millisecond
*	@param[in]  f_ITChannel_cb            : callback function to call
*
*  @retval RC_OK                             @ref RC_OK
*  @retval RC_ERROR_PTR_NULL                 @ref RC_ERROR_PTR_NULL
*  @retval RC_ERROR_PARAM_INVALID            @ref RC_ERROR_PARAM_INVALID
*  @retval RC_ERROR_ALREADY_CONFIGURED       @ref RC_ERROR_ALREADY_CONFIGURED
*  @retval RC_ERROR_NOT_ALLOWED              @ref RC_ERROR_NOT_ALLOWED
*/
static t_eReturnCode s_FMKTIM_Set_EvntChannelCfg(   t_sFMKTIM_TimerInfo * f_timerInfo_ps,
                                                    t_sFMKTIM_ChnlInfo * f_chnlInfo_ps,
                                                    t_uint32 f_periodms_u32,
                                                    t_cbFMKTIM_InterruptLine f_ITChannel_cb);


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
static t_eReturnCode s_FMKTIM_GetEcdrARRValue(  t_eFMKTIM_EcdrMode f_EcdrMode_e,
                                                t_uint32 f_rqstArrValue_u32,
                                                t_uint32 * f_ArrValue_pu32);                               
/**
 *
 *	@brief      Function to set the state ON/OFF of timer channel
 *
 *	@param[in]  f_timer_e              : enum value for the priority, value from @ref t_eFMKTIM_Timer
 *	@param[in]  f_channel_e            : enum value for the channel, value from @ref t_eFMKTIM_InterruptChnl
 *	@param[in]  f_chnlState_e          : The state of the timer.\n
 *
 *  @retval RC_OK                             @ref RC_OK
 *  @retval RC_ERROR_PARAM_INVALID            @ref RC_ERROR_PARAM_INVALID
 *  @retval RC_ERROR_PARAM_NOT_SUPPORTED      @ref RC_ERROR_PARAM_NOT_SUPPORTED
 *  @retval RC_ERROR_WRONG_RESULT             @ref RC_ERROR_WRONG_RESULT
 *
 */
static t_eReturnCode s_FMKTIM_Set_HwChannelState( t_sFMKTIM_TimerInfo * f_timInfo_ps,
                                                  t_sFMKTIM_ChnlInfo * f_chnlInfo_ps,
                                                  t_eFMKTIM_ChnlRunMode f_runMode_e,
                                                  t_eFMKTIM_HwTimerCfg f_hwTimCfg_e,
                                                  t_eFMKTIM_ChnlState f_chnlState_e);

/**
*
*	@brief    Get the DutyCycle from a timer channel
*   @note     Get the value from CCRx register and convert it in dutycycle
*
*	@param[in]  f_InterruptLine_e      : enum value for Interrupt Line, value from @ref t_eFMKCPU_InterruptLineIO
*	@param[in]  f_dutyCycle_u16        : the dutycyle, value from [0, 1000]
*
*  @retval RC_OK                             @ref RC_OK
*  @retval RC_ERROR_PTR_NULL                 @ref RC_ERROR_PTR_NULL
*  @retval RC_ERROR_PARAM_INVALID            @ref RC_ERROR_PARAM_INVALID
*  @retval RC_ERROR_ALREADY_CONFIGURED       @ref RC_ERROR_ALREADY_CONFIGURED
*  @retval RC_WARNING_NO_OPERATION           @ref RC_WARNING_NO_OPERATION
*/
static t_eReturnCode s_FMKTIM_Get_CCRxValue(t_sFMKTIM_TimerInfo * f_timInfo_ps, 
                                            t_eFMKTIM_InterruptChnl f_channel_e,
                                            t_uint32 * f_comparedValue_pu32);

/**
 *
 *	@brief      Perform all cyclic operation
 *
 *  @retval RC_OK                             @ref RC_OK
 *  @retval RC_ERROR_PARAM_INVALID            @ref RC_ERROR_PARAM_INVALID
 *  @retval RC_ERROR_PTR_NULL                 @ref RC_ERROR_PTR_NULL
 */
static t_eReturnCode s_FMKTIM_Operational(void);


/**
 *
 *	@brief      Perform Diagnostic on Timer and channels
 *  @note       If a error is detected on Timer, every channel used from this timer 
 *              inherit the error.\n
 * 
 *  @retval RC_OK                             @ref RC_OK
 *  @retval RC_ERROR_PARAM_INVALID            @ref RC_ERROR_PARAM_INVALID
 *  @retval RC_ERROR_WRONG_STATE              @ref RC_ERROR_WRONG_STATE

 */
static t_eReturnCode s_FMKTIM_PerformDiagnostic(t_eFMKTIM_Timer f_timer_e);
/**
 *
 *	@brief      Check timer/channel status to make ope on it 
 * 
 *  @retval RC_OK                             @ref RC_OK
 *  @retval RC_ERROR_PARAM_INVALID            @ref RC_ERROR_PARAM_INVALID
 *  @retval RC_ERROR_WRONG_STATE              @ref RC_ERROR_WRONG_STATE

 */
static t_eReturnCode s_FMKTIM_RqstLineValidityOpe(  t_eFMKTIM_InterruptLineType f_ITLineType_e,
                                                    t_uint8 f_Itline_u8,
                                                    t_eFMKTIM_Timer * f_timer_e,
                                                    t_eFMKTIM_InterruptChnl * f_chnl_pe,
                                                    t_eFMKTIM_HwTimerCfg f_HwTypeSuppose_e);
/**
*
*	@brief      Function to get the bsp channel
*
*	@param[in]  f_channel_e              : enum value for the channel, value from @ref t_eFMKTIM_InterruptChnl
*	@param[in]  f_bspChnl_pu32           : storage for bsp channel.\n
*
*  @retval RC_OK                             @ref RC_OK
*
*/
static t_eReturnCode s_FMKTIM_Get_BspChannel(t_eFMKTIM_InterruptChnl f_channel_e, t_uint32 *f_bspChnl_pu32);
/**
*
*	@brief      Function to get the bsp Encoder Mode
*
*	@param[in]  f_EcdrMode_e       : enum value for the Encoder Mode, value from @ref t_eFMKTIM_EcdrMode
*	@param[in]  f_bspEcdrMode_pu32  : storage for bsp Encoder Mode.\n
*
*  @retval RC_OK                             @ref RC_OK
*
*/
static t_eReturnCode s_FMKTIM_GetBspEcdrMode(t_eFMKTIM_EcdrMode f_EcdrMode_e, t_uint32 *f_bspEcdrMode_pu32);
/**
*
*	@brief      Function to get the bsp Encoder Mode
*
*	@param[in]  f_ICEcdrSelect_e      : enum value for the Encoder IC Selection, value from @ref t_eFMKTIM_ICSelection
*	@param[in]  f_bspEcdrMode_pu32     : storage for bsp IC Selection.\n
*
*  @retval RC_OK                             @ref RC_OK
*
*/
static t_eReturnCode s_FMKTIM_GetBspEcdrSelection(t_eFMKTIM_ICSelection f_ICEcdrSelect_e, t_uint32 *f_bspICEcdrSelect_pu32);
/**
*
*	@brief      Function to get the bsp Encoder Mode
*
*	@param[in]  f_ICEcdrPolarity_e      : enum value for the Encoder IC Selection, value from @ref t_eFMKTIM_ICSelection
*	@param[in]  f_bspICEcdrPolarity_pu32     : storage for bsp IC Selection.\n
*
*  @retval RC_OK                             @ref RC_OK
*
*/
static t_eReturnCode s_FMKTIM_GetBspEcdrPolarity(t_eFMKTIM_EcdrInPolarity f_ICEcdrPolarity_e, t_uint32 *f_bspICEcdrPolarity_pu32);

/**
 *
 *	@brief      Function to get the bsp IRQN enum
 *
 *	@param[in]  f_IRQN_e              : enum value for the priority, value from @ref t_eFMKCPU_IRQNType
 *	@param[in]  f_bspIRQN_pe          : storage for NVIC priority.\n
 *
 *  @retval RC_OK                             @ref RC_OK
 *  @retval RC_ERROR_PARAM_INVALID            @ref RC_ERROR_PARAM_INVALID
 *  @retval RC_ERROR_PTR_NULL                 @ref RC_ERROR_PTR_NULL
 *  @retval RC_ERROR_PARAM_NOT_SUPPORTED      @ref RC_ERROR_PARAM_NOT_SUPPORTED
 *
 */
static t_eReturnCode s_FMKTIM_Get_BspICTrigger(t_eFMKTIM_ChnlMeasTrigger f_MeasTrigger_e, t_uint32 *f_bspMeasTrigger_u32);
/**
*
*	@brief      Function to redirect all callback function.\n
*  @note       Every HAL callback call this function, thsi function redirect to the right
*              user function depending on which timer is called
*
*	@param[in]  f_timerIstce_ps         : bsp instance timer which make interruption
*	@param[in]  f_HwTimcfg_e            : the timer configuration 
*	@param[in]  f_chnlState_e           : The state of the timer.\n
*
*  @retval RC_OK                             @ref RC_OK
*  @retval RC_ERROR_PARAM_INVALID            @ref RC_ERROR_PARAM_INVALID
*  @retval RC_ERROR_PTR_NULL                 @ref RC_ERROR_PTR_NULL
*
*/
static void FMKTIM_BspRqst_InterruptMngmt(TIM_HandleTypeDef *f_timerIstce_ps, t_eFMKTIM_BspEvent f_cbEvent_e);
/**
 *
 *	@brief      Function Timer and channel from a InterruptLine
 *
 *	@param[in]  f_ITLineType_e           : enum value for the type of interruptline, value from @ref t_eFMKTIM_InterruptLineType
 *	@param[in]  ITLine_u                 : union value for the interruptline, value from @ref t_uFMKTIM_InterruptLine
 *	@param[in]  f_timer_pe                : pointor enum value for containing timer, value from @ref t_eFMKTIM_Timer
 *	@param[in]  f_channel_pe              : pointor enum value for containing timer, value from @ref t_eFMKTIM_InterruptChnl
 *
 *  @retval RC_OK                             @ref RC_OK
 *  @retval RC_ERROR_PARAM_INVALID            @ref RC_ERROR_PARAM_INVALID
 *  @retval RC_ERROR_PTR_NULL                 @ref RC_ERROR_PTR_NULL
 *  @retval RC_ERROR_PARAM_NOT_SUPPORTED      @ref RC_ERROR_PARAM_NOT_SUPPORTED
 *
 */
static t_eReturnCode s_FMKTIM_Get_TimChnlFromITLine(t_eFMKTIM_InterruptLineType f_ITLineType_e,
                                                    t_uint32                    f_ITLine_u32,    
                                                    t_eFMKTIM_Timer             *f_timer_pe,
                                                    t_eFMKTIM_InterruptChnl     *f_channel_pe);

static t_eReturnCode s_FMKTIM_UpdateTimerFrequency( t_sFMKTIM_TimerInfo * f_timerInfo_ps, 
                                                    t_eFMKTIM_InterruptChnl f_chnl_e,
                                                    t_uint32 f_timFreq_u32); 

static t_eReturnCode s_FMKTIM_UpdateDutyCycle(  t_sFMKTIM_TimerInfo * f_timerInfo_ps, 
                                                t_eFMKTIM_InterruptChnl f_chnl_e,
                                                t_uint32 f_dutyCycle_u32);
//****************************************************************************
//                      Public functions - Implementation
//********************************************************************************
t_eReturnCode FMKTIM_Init(void)
{
    t_eReturnCode Ret_e = RC_OK;
    t_uint8 timIndex_u8;
    t_uint8 chnlIndex_u8;
    t_uint16 sysClkValueMHz_u16;
    t_uint16 timClkValueMHz_u16;
    t_eFMKCPU_SysClkOsc oscTimerSrc_e;
    t_sFMKTIM_TimerInfo * timerInfo_ps;
    t_sFMKTIM_ChnlInfo * chnlInfo_ps;

    Ret_e = FMKCPU_GetSysClkValue(FMKCPU_SYS_CLOCK_SYSTEM, &sysClkValueMHz_u16);

    if(Ret_e == RC_OK)
    {
        for (timIndex_u8 = (t_uint8)0 ; (timIndex_u8 < (t_eFMKTIM_Timer)FMKTIM_TIMER_NB) && (Ret_e == RC_OK) ; timIndex_u8++)
        {
            timerInfo_ps = (t_sFMKTIM_TimerInfo *)(&g_TimerInfo_as[timIndex_u8]);

            timerInfo_ps->IsNVICTimerEnable_b = (t_bool)False;
            timerInfo_ps->ignoreInterrupt_b = (t_bool)False;
            timerInfo_ps->isConfigured_b = (t_bool)False;
            timerInfo_ps->IsTimerRunning_b    = (t_bool)False;
            timerInfo_ps->HwCfg_e = FMKTIM_HWTIM_CFG_NB;
            timerInfo_ps->mskChnlState_u16 = (t_uint16)0;
            timerInfo_ps->c_clock_e = c_FmkTim_TimersCfg_as[timIndex_u8].c_clock_e;
            timerInfo_ps->c_IRQNType_e = c_FmkTim_TimersCfg_as[timIndex_u8].c_IRQNType_e;
            
            g_bspTimerHandle_as[timIndex_u8].Instance = (TIM_TypeDef *)c_FmkTim_TimersCfg_as[timIndex_u8].bspIstc_ps;
            timerInfo_ps->bspTimer_ps = (&g_bspTimerHandle_as[timIndex_u8]);

            Ret_e = SMB_SecureBlockInit(&g_sfmb_TimerInfo_as[timIndex_u8],
                                        &g_TimerInfo_as[timIndex_u8],
                                        sizeof(t_sFMKTIM_TimerInfo),
                                        5);
            if(Ret_e == RC_OK)
            {
                Ret_e = FMKCPU_GetOscRccSrc(timerInfo_ps->c_clock_e, &oscTimerSrc_e);
            }
            if(Ret_e == RC_OK)
            {
                Ret_e = FMKCPU_GetSysClkValue(oscTimerSrc_e, &timClkValueMHz_u16);
            }
            if(Ret_e == RC_OK)
            {
                //---- see documentation fore more info, but basically 
                //      the basic timer frequency is double whenever the sysclck value of the bus for 
                //      the timer is < to the sys clock ----//
                if(sysClkValueMHz_u16 > timClkValueMHz_u16)
                {
                    timerInfo_ps->timerFreqMHz_u32 = (t_uint32)((t_uint32)2 * (t_uint32)timClkValueMHz_u16);
                }
                else 
                {
                    timerInfo_ps->timerFreqMHz_u32 = (t_uint32)(timClkValueMHz_u16);
                }

                for (chnlIndex_u8 = (t_uint8)0 ; chnlIndex_u8 < (t_eFMKTIM_InterruptChnl)FMKTIM_CHANNEL_NB ; chnlIndex_u8++)
                {
                    chnlInfo_ps = (t_sFMKTIM_ChnlInfo *)(&g_TimChnlInfo_as[timIndex_u8][chnlIndex_u8]);
                    chnlInfo_ps->selfId_e = (t_eFMKTIM_InterruptChnl)chnlIndex_u8;
                    chnlInfo_ps->chnl_cb = NULL_FUNCTION;
                    chnlInfo_ps->ErrState_e = FMKTIM_ERRSTATE_OK;
                    chnlInfo_ps->IsChnlConfigure_b =  (t_bool)False;
                    chnlInfo_ps->RunMode_e = FMKTIM_LINE_RUNMODE_POLLING;
                    chnlInfo_ps->State_e = FMKTIM_CHNLST_DISACTIVATED;
                    chnlInfo_ps->DmaInfo_ps.BufferAdd1_pu32 = (t_uint32 *)NULL;
                    chnlInfo_ps->DmaInfo_ps.BufferAdd2_pu32 = (t_uint32 *)NULL;
                    chnlInfo_ps->DmaInfo_ps.bufferLen_u16 = (t_uint16)0;

                    Ret_e  = SMB_SecureBlockInit(   &g_sfmb_TimChnlInfo_as[timIndex_u8][chnlIndex_u8],
                                                    &g_TimChnlInfo_as[timIndex_u8][chnlIndex_u8],
                                                    sizeof(t_sFMKTIM_ChnlInfo),
                                                    5);
                }
            }
        }
    }

    return Ret_e;
}

/*********************************
 * FMKTIM_Cyclic
 *********************************/
t_eReturnCode FMKTIM_Cyclic(void)
{
    t_eReturnCode Ret_e = RC_OK;

    switch (g_FmkTim_ModState_e)
    {
        case STATE_CYCLIC_CFG:
        {
            g_FmkTim_ModState_e = STATE_CYCLIC_WAITING;
            break;
        }
        case STATE_CYCLIC_WAITING:
        {
            // nothing to do, just wait all module are Ope
            break;
        }
        case STATE_CYCLIC_PREOPE:
        {
            g_FmkTim_ModState_e = STATE_CYCLIC_OPE;
            break; 
        }
        case STATE_CYCLIC_OPE:
        {
            Ret_e = s_FMKTIM_Operational();
            if(Ret_e < RC_OK)
            {
                g_FmkTim_ModState_e = STATE_CYCLIC_ERROR;
            }
            break;
        }
        case STATE_CYCLIC_ERROR:
        {
            break;
        }
        
        case STATE_CYCLIC_BUSY:
        default:
            Ret_e = RC_OK;
            break;
    }
    return Ret_e;
}

/*********************************
 * FMKTIM_GetState
 *********************************/
t_eReturnCode FMKTIM_GetState(t_eCyclicModState *f_State_pe)
{
    t_eReturnCode Ret_e = RC_OK;

    if(f_State_pe == (t_eCyclicModState *)NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }
    if(Ret_e == RC_OK)
    {
        *f_State_pe = g_FmkTim_ModState_e;
    }

    return Ret_e;
}

/*********************************
 * FMKTIM_SetState
 *********************************/
t_eReturnCode FMKTIM_SetState(t_eCyclicModState f_State_e)
{

    g_FmkTim_ModState_e = f_State_e;

    return RC_OK;
}

/*********************************
 * FMKTIM_Set_PWMMLineCfg
 *********************************/
t_eReturnCode FMKTIM_Set_PWMLineCfg(t_eFMKTIM_InterruptLineIO f_InterruptLine_e,
                                    t_uint32 f_pwmFreq_u32,
                                    t_eFMKTIM_LinePolarity f_linePolarity_e,
                                    t_cbFMKTIM_InterruptLine * f_PwmPulseFinished_pcb)
{
    t_eReturnCode Ret_e = RC_OK;
    t_eFMKTIM_Timer timer_e;
    t_eFMKTIM_InterruptChnl chnl_e;

    if((f_InterruptLine_e >= FMKTIM_INTERRUPT_LINE_IO_NB)
    || (f_linePolarity_e >= FMKTIM_LINE_POLARITY_NB))
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
        ASSERT((t_uint16)Ret_e);
    }
    
    if(Ret_e == RC_OK)
    {
        timer_e = c_FmkTim_ITLineIOMapp_as[f_InterruptLine_e].timer_e;
        chnl_e = c_FmkTim_ITLineIOMapp_as[f_InterruptLine_e].channel_e;

        if(g_TimerInfo_as[timer_e].isConfigured_b == (t_bool)False)
        {
            Ret_e = s_FMKTIM_Set_BspTimerInit(&g_TimerInfo_as[timer_e],
                                                FMKTIM_HWTIM_CFG_PWM,
                                                f_pwmFreq_u32,
                                                (void *)NULL);
        }

        if(Ret_e == RC_OK)
        {
            Ret_e = s_FMKTIM_Set_PwmChannelCfg( (&g_TimerInfo_as[timer_e]),
                                                &g_TimChnlInfo_as[timer_e][chnl_e],
                                                f_pwmFreq_u32,
                                                f_linePolarity_e);
        }

        if(Ret_e == RC_OK)
        {
            g_TimChnlInfo_as[timer_e][chnl_e].chnl_cb = f_PwmPulseFinished_pcb;
        }
        else 
        {
            ASSERT((t_uint16)Ret_e);
        }
    }
    return Ret_e;
}

/*********************************
 * FMKTIM_Set_EcdrLineCfg
 *********************************/
t_eReturnCode FMKTIM_Set_EcdrLineCfg(t_eFMKTIM_InterruptLineIO f_InterruptLine_e,
                                            t_sFMKTIM_EcdrCfg f_EcdrCfg_s,
                                            t_uint32 f_ARRValue_u32)
{
    t_eReturnCode Ret_e = RC_OK;
    t_eFMKTIM_Timer timer_e;
    t_sFMKTIM_TimerInfo * timerInfo_ps;
    t_sFMKTIM_ChnlInfo * timChnlInfo_pas;
    t_eFMKTIM_InterruptChnl chnl_e;
    

    if(f_InterruptLine_e >= FMKTIM_INTERRUPT_LINE_IO_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
        ASSERT((t_uint16)Ret_e);
    }
    if(Ret_e == RC_OK)
    {
        timer_e = c_FmkTim_ITLineIOMapp_as[f_InterruptLine_e].timer_e;
        chnl_e = c_FmkTim_ITLineIOMapp_as[f_InterruptLine_e].channel_e;
        timerInfo_ps = (t_sFMKTIM_TimerInfo *)(&g_TimerInfo_as[timer_e]);
        timChnlInfo_pas = (t_sFMKTIM_ChnlInfo *)(&g_TimChnlInfo_as[timer_e]);

        //----- According Bsp, timer must the CHANNEL 1 and 2 use for Encoder -----//
        if((chnl_e != FMKTIM_CHANNEL_1)
        || (timChnlInfo_pas[FMKTIM_CHANNEL_1].IsChnlConfigure_b == (t_bool)True)
        || (timChnlInfo_pas[FMKTIM_CHANNEL_2].IsChnlConfigure_b == (t_bool)True))
        {
            Ret_e = RC_WARNING_WRONG_CONFIG;
        }
       
        if(Ret_e == RC_OK)
        {
            Ret_e = s_FMKTIM_Set_EcdrChannelCfg(timerInfo_ps,
                                                &f_EcdrCfg_s,
                                                f_ARRValue_u32);
        }
        if(Ret_e == RC_OK)
        {
            //------ Update Information ------//
            timChnlInfo_pas[FMKTIM_CHANNEL_1].IsChnlConfigure_b = (t_bool)True;
            timChnlInfo_pas[FMKTIM_CHANNEL_1].RunMode_e = FMKTIM_LINE_RUNMODE_POLLING;


            timChnlInfo_pas[FMKTIM_CHANNEL_2].IsChnlConfigure_b = (t_bool)True;
            timChnlInfo_pas[FMKTIM_CHANNEL_2].RunMode_e = FMKTIM_LINE_RUNMODE_POLLING;
        }    
        else 
        {
            ASSERT((t_uint16)Ret_e);
            timChnlInfo_pas[FMKTIM_CHANNEL_2].ErrState_e = FMKTIM_ERRSTATE_INIT_FAILED;
            timChnlInfo_pas[FMKTIM_CHANNEL_1].ErrState_e = FMKTIM_ERRSTATE_INIT_FAILED;
        }
        
    }
    
    return Ret_e;
}

/*********************************
 * FMKTIM_Get_EncoderValues
 *********************************/
t_eReturnCode FMKTIM_Get_EncoderValues(t_eFMKTIM_InterruptLineIO f_EcdrLine_e,
                                       t_uint32 *f_position_pu32,
                                       t_uint8 *f_direction_pu8)
{
    t_eReturnCode Ret_e = RC_OK;
    t_eFMKTIM_Timer timer_e;
    t_eFMKTIM_InterruptChnl chnl_e;

    t_sFMKTIM_TimerInfo timerInfo_s;
    t_sFMKTIM_ChnlInfo chnlInfo_s;

    //--- check pointers
    if ((f_direction_pu8 == NULL) || (f_position_pu32 == NULL))
    {
        Ret_e = RC_ERROR_PTR_NULL;
        ASSERT((t_uint16)Ret_e);
    }

    //--- check param
    if (f_EcdrLine_e >= FMKTIM_INTERRUPT_LINE_IO_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
        ASSERT((t_uint16)Ret_e);
    }

    if (Ret_e == RC_OK)
    {
        timer_e = c_FmkTim_ITLineIOMapp_as[f_EcdrLine_e].timer_e;
        chnl_e  = c_FmkTim_ITLineIOMapp_as[f_EcdrLine_e].channel_e;

        //--- safe read timer and channel info
        SMB_Read(&g_sfmb_TimerInfo_as[timer_e], &timerInfo_s, sizeof(t_sFMKTIM_TimerInfo));
        SMB_Read(&g_sfmb_TimChnlInfo_as[timer_e][chnl_e], &chnlInfo_s, sizeof(t_sFMKTIM_ChnlInfo));

        //--- check running validity
        if (chnlInfo_s.State_e != FMKTIM_CHNLST_ACTIVATED)
        {
            Ret_e = RC_WARNING_BUSY;
        }

        if (Ret_e == RC_OK)
        {
            *f_position_pu32  = (t_uint32)(timerInfo_s.bspTimer_ps->Instance->CNT);
            *f_direction_pu8  = (t_uint8)(timerInfo_s.bspTimer_ps->Instance->CR1);
        }
        else
        {
            ASSERT((t_uint16)Ret_e);
        }
    }

    return Ret_e;
}
/*********************************
 * FMKTIM_Set_ICLineCfg
 *********************************/
t_eReturnCode FMKTIM_Set_ICLineCfg(t_eFMKTIM_InterruptLineIO f_InterruptLine_e,
                                         t_eFMKTIM_ChnlMeasTrigger f_MeasTrigger_e,
                                         t_cbFMKTIM_InterruptLine  * f_ITChannel_cb)
{
    t_eReturnCode Ret_e = RC_OK;
    t_eFMKTIM_Timer timer_e;
    t_eFMKTIM_InterruptChnl chnl_e;

    if(f_ITChannel_cb == (t_cbFMKTIM_InterruptLine *)NULL_FUNCTION)
    {
        Ret_e = RC_ERROR_PTR_NULL;
        ASSERT((t_uint16)Ret_e);
    }
    if( (f_MeasTrigger_e >= FMKTIM_LINE_MEAS_NB)
    || (f_InterruptLine_e >= FMKTIM_INTERRUPT_LINE_IO_NB))
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
        ASSERT((t_uint16)Ret_e);
    }
    if(Ret_e == RC_OK)
    {
        //---------Get timer/channel information---------//
        timer_e = c_FmkTim_ITLineIOMapp_as[f_InterruptLine_e].timer_e;
        chnl_e = c_FmkTim_ITLineIOMapp_as[f_InterruptLine_e].channel_e;
        //---------Call Configuration function---------//
        Ret_e = s_FMKTIM_Set_ICChannelCfg(&g_TimerInfo_as[timer_e],
                                          &g_TimChnlInfo_as[timer_e][chnl_e],
                                          f_MeasTrigger_e,
                                          f_ITChannel_cb);
    }
    else 
    {
        ASSERT((t_uint16)Ret_e);
    }
    return Ret_e;
}

/*********************************
 * FMKTIM_Set_EvntTimerCfg
 *********************************/
t_eReturnCode FMKTIM_Set_EvntTimerCfg(   t_eFMKTIM_InterruptLineEvnt f_EvntITLine_e,
                                        t_uint32 f_periodms_u32,
                                        t_cbFMKTIM_InterruptLine f_ITChannel_cb)
{
    /********************************
     *   Some useful information for Evnt generation
     *   In FMKTIM_ConfigPrivate, 
     * 
     ********************************/
    t_eReturnCode Ret_e = RC_OK;
    t_eFMKTIM_Timer timer_e;
    t_eFMKTIM_InterruptChnl chnl_e;

    if (f_EvntITLine_e >= FMKTIM_INTERRUPT_LINE_EVNT_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
        ASSERT((t_uint16)Ret_e);
    }
    if (f_ITChannel_cb == (t_cbFMKTIM_InterruptLine *)NULL_FUNCTION)
    {
        Ret_e = RC_ERROR_PTR_NULL;
        ASSERT((t_uint16)Ret_e);
    }
    if(Ret_e == RC_OK)
    {
        //---------Get timer/channel information---------//
        timer_e = c_FmkTim_ITLineEvntMapp_as[f_EvntITLine_e].timer_e;
        chnl_e = c_FmkTim_ITLineEvntMapp_as[f_EvntITLine_e].channel_e;
        //---------Get Event Init Function---------//
        Ret_e = s_FMKTIM_Set_EvntChannelCfg(&g_TimerInfo_as[timer_e],
                                            &g_TimChnlInfo_as[timer_e][chnl_e],
                                            f_periodms_u32,
                                            f_ITChannel_cb);
        if(Ret_e < RC_OK)
        {
            ASSERT((t_uint16)Ret_e);
        }
    }
    

    return Ret_e;
}

/*********************************
 * FMKTIM_AddInterruptCallback
 *********************************/
t_eReturnCode FMKTIM_AddInterruptCallback(t_eFMKTIM_InterruptLineIO f_InterruptLine_e,
                                          t_cbFMKTIM_InterruptLine *f_ITChannel_cb)
{
    t_eReturnCode Ret_e = RC_OK;
    t_eFMKTIM_Timer timer_e;
    t_eFMKTIM_InterruptChnl chnl_e;

    t_sFMKTIM_TimerInfo timerInfo_s;
    t_sFMKTIM_ChnlInfo  chnlInfo_s;

    //--- Param check
    if (f_InterruptLine_e >= FMKTIM_INTERRUPT_LINE_IO_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
        ASSERT((t_uint16)Ret_e);
    }
    if (f_ITChannel_cb == (t_cbFMKTIM_InterruptLine *)NULL_FUNCTION)
    {
        Ret_e = RC_ERROR_PTR_NULL;
        ASSERT((t_uint16)Ret_e);
    }

    if (Ret_e == RC_OK)
    {
        //--- Get timer and channel
        timer_e = c_FmkTim_ITLineIOMapp_as[f_InterruptLine_e].timer_e;
        chnl_e  = c_FmkTim_ITLineIOMapp_as[f_InterruptLine_e].channel_e;

        //--- Safe read timer and channel info
        SMB_Read(&g_sfmb_TimerInfo_as[timer_e], &timerInfo_s, sizeof(t_sFMKTIM_TimerInfo));
        SMB_Read(&g_sfmb_TimChnlInfo_as[timer_e][chnl_e], &chnlInfo_s, sizeof(t_sFMKTIM_ChnlInfo));

        //--- Check configuration state
        if ((chnlInfo_s.IsChnlConfigure_b != (t_bool)True) ||
            (timerInfo_s.isConfigured_b   != (t_bool)True))
        {
            Ret_e = RC_ERROR_WRONG_STATE;
            ASSERT((t_uint16)Ret_e);
        }
        else
        {
            //--- Update local copy of channel info
            chnlInfo_s.RunMode_e = FMKTIM_LINE_RUNMODE_INTERRUPT;
            chnlInfo_s.chnl_cb   = f_ITChannel_cb;

            //--- Safe write back
            SMB_Write(&g_sfmb_TimChnlInfo_as[timer_e][chnl_e], &chnlInfo_s, sizeof(t_sFMKTIM_ChnlInfo));
        }
    }

    return Ret_e;
}

/*********************************
 * FMKTIM_Set_PwmLineValue
 *********************************/
t_eReturnCode FMKTIM_Set_PwmLineValue(t_eFMKTIM_InterruptLineIO f_Itline_e,
                                      t_sFMKTIM_PwmOpe f_PwmOpe_s,
                                      t_uint8 f_maskUpdate_u8)
{
    t_eReturnCode Ret_e = RC_OK;
    t_sFMKTIM_TimerInfo timerInfo_s;
    t_sFMKTIM_ChnlInfo  chnlInfo_s;
    t_eFMKTIM_Timer timer_e = FMKTIM_TIMER_NB;
    t_eFMKTIM_InterruptChnl chnl_e = FMKTIM_CHANNEL_NB;
    t_eFMKTIM_ChnlState chnlState_e = FMKTIM_CHNLST_DISACTIVATED;

    Ret_e = s_FMKTIM_RqstLineValidityOpe(FMKTIM_INTERRUPT_LINE_TYPE_IO,
                                         (t_uint8)f_Itline_e,
                                         (&timer_e),
                                         (&chnl_e),
                                         FMKTIM_HWTIM_CFG_PWM);
    if(Ret_e == RC_OK)
    {
        //--------- Lecture depuis la SRAM via SMB_Read ---------//
        Ret_e = SMB_Read(&g_sfmb_TimerInfo_as[timer_e], &timerInfo_s, sizeof(t_sFMKTIM_TimerInfo));
        if(Ret_e == RC_OK)
        {
            Ret_e = SMB_Read(&g_sfmb_TimChnlInfo_as[timer_e][chnl_e], &chnlInfo_s, sizeof(t_sFMKTIM_ChnlInfo));
        }
    }
    if(Ret_e == RC_OK)
    {
        // See if bit change frequency is SET
        if(GETBIT(f_maskUpdate_u8, FMKTIM_BIT_PWM_FREQUENCY) == BIT_IS_SET_8B)
        {
            if(f_PwmOpe_s.frequency_u32 == (t_uint32)0)
            {
                Ret_e = RC_ERROR_PARAM_INVALID;
                ASSERT((t_uint16)(f_maskUpdate_u8));
            }
            else 
            {
                //----- an event is going to be made -----//
                Ret_e = s_FMKTIM_UpdateTimerFrequency(&timerInfo_s, chnl_e, f_PwmOpe_s.frequency_u32);
                //----- don't change channel state ----//
                chnlState_e = chnlInfo_s.State_e;
            }
        }
        if(GETBIT(f_maskUpdate_u8, FMKTIM_BIT_PWM_DUTYCYCLE) == BIT_IS_SET_8B)
        {
            if(f_PwmOpe_s.dutyCycle_u16 > FMKTIM_PWM_MAX_DUTY_CYLCE)
            {
                f_PwmOpe_s.dutyCycle_u16 = FMKTIM_PWM_MAX_DUTY_CYLCE;
            }

            Ret_e = s_FMKTIM_UpdateDutyCycle(&timerInfo_s, chnl_e, (t_uint32)f_PwmOpe_s.dutyCycle_u16);
            chnlState_e = FMKTIM_CHNLST_ACTIVATED;
        }
        if(GETBIT(f_maskUpdate_u8, FMKTIM_BIT_PWM_NB_PULSES) == BIT_IS_SET_8B)
        {
            if(f_PwmOpe_s.nbPulses_u16 > CST_MAX_UINT_16BIT)
            {
                Ret_e = RC_WARNING_LIMIT_REACHED;
                f_PwmOpe_s.nbPulses_u16 = CST_MAX_UINT_16BIT;
            }

            if(chnlInfo_s.State_e == FMKTIM_CHNLST_DISACTIVATED)
            {
                timerInfo_s.bspTimer_ps->Instance->CNT = 0;
                timerInfo_s.bspTimer_ps->Instance->RCR = (t_uint16)(f_PwmOpe_s.nbPulses_u16 - (t_uint16)1);
            }
            else
            {
                Ret_e = s_FMKTIM_Set_HwChannelState(&timerInfo_s,
                                                    &chnlInfo_s,
                                                    FMKTIM_LINE_RUNMODE_INTERRUPT,
                                                    FMKTIM_HWTIM_CFG_EVNT,
                                                    FMKTIM_CHNLST_DISACTIVATED);

                __HAL_TIM_CLEAR_FLAG(timerInfo_s.bspTimer_ps, TIM_FLAG_UPDATE);
                if(Ret_e == RC_OK)
                {
                    Ret_e = s_FMKTIM_Set_HwChannelState(&timerInfo_s,
                                                        &chnlInfo_s,
                                                        chnlInfo_s.RunMode_e,
                                                        timerInfo_s.HwCfg_e,
                                                        FMKTIM_CHNLST_DISACTIVATED);
                }
                if(Ret_e == RC_OK)
                {
                    timerInfo_s.bspTimer_ps->Instance->CNT = 0;
                    timerInfo_s.bspTimer_ps->Instance->RCR = (t_uint16)(f_PwmOpe_s.nbPulses_u16 - (t_uint16)1);
                }
            }

            if(f_PwmOpe_s.nbPulses_u16 > (t_uint16)0)
            {
                chnlState_e = FMKTIM_CHNLST_ACTIVATED;

                __HAL_TIM_CLEAR_FLAG(timerInfo_s.bspTimer_ps, TIM_FLAG_UPDATE);
                timerInfo_s.ignoreInterrupt_b = (t_bool)True;
                Ret_e = s_FMKTIM_Set_HwChannelState(&timerInfo_s,
                                                    &chnlInfo_s,
                                                    FMKTIM_LINE_RUNMODE_INTERRUPT,
                                                    FMKTIM_HWTIM_CFG_EVNT,
                                                    FMKTIM_CHNLST_ACTIVATED);
            }
            else
            {
                chnlState_e = FMKTIM_CHNLST_DISACTIVATED;
            }
        }

        if(Ret_e == RC_OK)
        {
            if(chnlInfo_s.State_e != chnlState_e)
            {
                Ret_e = s_FMKTIM_Set_HwChannelState(&timerInfo_s,
                                                    &chnlInfo_s,
                                                    chnlInfo_s.RunMode_e,
                                                    timerInfo_s.HwCfg_e,
                                                    chnlState_e);
            }
        }
        else
        {
            ASSERT((t_uint16)Ret_e);
        }
        //--------- Écriture dans la SRAM via SMB_Write ---------//
        Ret_e = SMB_Write(&g_sfmb_TimerInfo_as[timer_e], &timerInfo_s, sizeof(t_sFMKTIM_TimerInfo));
        Ret_e |= SMB_Write(&g_sfmb_TimChnlInfo_as[timer_e][chnl_e], &chnlInfo_s, sizeof(t_sFMKTIM_ChnlInfo));
    }


    return Ret_e;
}

/*********************************
 * FMKTIM_Set_InterruptLineOpe
 *********************************/
t_eReturnCode FMKTIM_Set_ICLineValue(t_eFMKTIM_InterruptLineIO f_Itline_e,
                                    t_sFMKTIM_ICOpe f_ICOpe_s,
                                    t_uint8 f_mask_u8)
{
    t_eReturnCode Ret_e = RC_OK;
    t_eFMKTIM_InterruptChnl chnl_e = FMKTIM_CHANNEL_NB;
    t_eFMKTIM_Timer timer_e = FMKTIM_TIMER_NB; 
    t_sFMKTIM_TimerInfo timerInfo_s; 
    t_sFMKTIM_ChnlInfo chnlInfo_s;  
    t_eFMKTIM_ChnlState chnlState_e;
    t_uint32 bspARRVal_u32 = 0;
    t_uint32 bspPSCVal_u32 = 0;

    // Validation de la ligne d'interruption + récupération infos timer et channel
    Ret_e = s_FMKTIM_RqstLineValidityOpe(FMKTIM_INTERRUPT_LINE_TYPE_IO,
                                        (t_uint8)f_Itline_e,
                                        &timer_e,
                                        &chnl_e,
                                        FMKTIM_HWTIM_CFG_IC);
    if(Ret_e == RC_OK)
    {
        // Lecture protégée depuis la SRAM via SMB_Read
        Ret_e = SMB_Read(&g_sfmb_TimerInfo_as[timer_e], &timerInfo_s, sizeof(t_sFMKTIM_TimerInfo));
        if(Ret_e == RC_OK)
        {
            Ret_e = SMB_Read(&g_sfmb_TimChnlInfo_as[timer_e][chnl_e], &chnlInfo_s, sizeof(t_sFMKTIM_ChnlInfo));
        }
    }
    if(Ret_e == RC_OK)
    {
        // Gestion du bit IC_STATE
        if(GETBIT(f_mask_u8, FMKTIM_BIT_IC_STATE) == BIT_IS_SET_8B)
        {
            switch(f_ICOpe_s.IcState_e)
            {
                case FMKTIM_IC_STATE_ENABLE:
                    chnlState_e = FMKTIM_CHNLST_ACTIVATED;
                    break;
                case FMKTIM_IC_STATE_DISABLE:
                    chnlState_e = FMKTIM_CHNLST_DISACTIVATED;
                    break;
                case FMKTIM_IC_STATE_NB:
                default:
                    Ret_e = RC_ERROR_NOT_SUPPORTED;
                    break;
            }

            if(Ret_e == RC_OK)
            {
                Ret_e = s_FMKTIM_Set_HwChannelState(&timerInfo_s,
                                                    &chnlInfo_s,
                                                    chnlInfo_s.RunMode_e,
                                                    timerInfo_s.HwCfg_e,
                                                    chnlState_e);
            }
        }

        // Gestion du bit IC_FREQUENCY
        if((Ret_e == RC_OK) 
        && (GETBIT(f_mask_u8, FMKTIM_BIT_IC_FREQUENCY) == BIT_IS_SET_8B))
        {
            Ret_e = c_FMKTIM_TimerFunc_apf[FMKTIM_HWTIM_CFG_IC].
                    GetTimerInfoInit_pcb(timerInfo_s.c_clock_e,
                                        timerInfo_s.timerFreqMHz_u32,
                                        (t_float32)f_ICOpe_s.frequency_u32,
                                        &bspARRVal_u32,
                                        &bspPSCVal_u32);

            if(Ret_e == RC_OK)
            {
                timerInfo_s.bspTimer_ps->Instance->ARR = bspARRVal_u32;
                timerInfo_s.bspTimer_ps->Instance->PSC = bspPSCVal_u32;
            }
            else
            {
                ASSERT((t_uint16)Ret_e);
            }
        }
        // Écriture protégée dans la SRAM via SMB_Write
        if(Ret_e == RC_OK)
        {
            Ret_e = SMB_Write(&g_sfmb_TimerInfo_as[timer_e], &timerInfo_s, sizeof(t_sFMKTIM_TimerInfo));
            Ret_e |= SMB_Write(&g_sfmb_TimChnlInfo_as[timer_e][chnl_e], &chnlInfo_s, sizeof(t_sFMKTIM_ChnlInfo));
        }
    }

    

    return Ret_e;
}

/*********************************
 * FMKTIM_Set_EcdrLineState
 *********************************/
t_eReturnCode FMKTIM_Set_EcdrLineState(t_eFMKTIM_InterruptLineIO f_Itline_e,
                                      t_eFMKTIM_EcdrOpe f_EcdrOpe)
{
    t_eReturnCode Ret_e = RC_OK;
    t_eFMKTIM_InterruptChnl chnl_e = FMKTIM_CHANNEL_NB;
    t_eFMKTIM_Timer timer_e = FMKTIM_TIMER_NB;
    t_sFMKTIM_TimerInfo timerInfo_s;
    t_sFMKTIM_ChnlInfo  chnlInfo_s;
    t_eFMKTIM_ChnlState chnlState_e = FMKTIM_CHNLST_DISACTIVATED;

    // Validation de la ligne d'interruption et récupération des infos timer + channel
    Ret_e = s_FMKTIM_RqstLineValidityOpe(FMKTIM_INTERRUPT_LINE_TYPE_IO,
                                        (t_uint8)f_Itline_e,
                                        &timer_e,
                                        &chnl_e,
                                        FMKTIM_HWTIM_CFG_ECDR);
    if(Ret_e == RC_OK)
    {
        //--------- Lecture depuis la SRAM via SMB_Read ---------//
        Ret_e = SMB_Read(&g_sfmb_TimerInfo_as[timer_e], &timerInfo_s, sizeof(t_sFMKTIM_TimerInfo));
    }
    if(Ret_e == RC_OK)
    {
        // Détermination du channel et état selon l'opération
        switch (f_EcdrOpe)
        {
            case FMKTIM_ECDR_OPE_START_TI1:
                chnl_e = FMKTIM_CHANNEL_1;
                Ret_e = SMB_Read(   &g_sfmb_TimChnlInfo_as[timer_e][FMKTIM_CHANNEL_1], 
                                    &chnlInfo_s, 
                                    sizeof(t_sFMKTIM_ChnlInfo));
                chnlState_e = FMKTIM_CHNLST_ACTIVATED;
                break;

            case FMKTIM_ECDR_OPE_START_TI2:
                chnl_e = FMKTIM_CHANNEL_2;
                Ret_e = SMB_Read(   &g_sfmb_TimChnlInfo_as[timer_e][FMKTIM_CHANNEL_2], 
                                    &chnlInfo_s, 
                                    sizeof(t_sFMKTIM_ChnlInfo));
                chnlState_e = FMKTIM_CHNLST_ACTIVATED;
                break;

            case FMKTIM_ECDR_OPE_START_BOTH:
                chnl_e = FMKTIM_CHANNEL_ALL;
                //--- ass default take the first one ----//
                Ret_e = SMB_Read(   &g_sfmb_TimChnlInfo_as[timer_e][FMKTIM_CHANNEL_1], 
                                    &chnlInfo_s, 
                                    sizeof(t_sFMKTIM_ChnlInfo));
                chnlState_e = FMKTIM_CHNLST_ACTIVATED;
                break;

            case FMKTIM_ECDR_OPE_STOP_TI1:
                chnl_e = FMKTIM_CHANNEL_1;
                Ret_e = SMB_Read(   &g_sfmb_TimChnlInfo_as[timer_e][FMKTIM_CHANNEL_1], 
                                    &chnlInfo_s, 
                                    sizeof(t_sFMKTIM_ChnlInfo));
                chnlState_e = FMKTIM_CHNLST_DISACTIVATED;
                break;

            case FMKTIM_ECDR_OPE_STOP_TI2:
                chnl_e = FMKTIM_CHANNEL_2;
                Ret_e = SMB_Read(   &g_sfmb_TimChnlInfo_as[timer_e][FMKTIM_CHANNEL_2], 
                                    &chnlInfo_s, 
                                    sizeof(t_sFMKTIM_ChnlInfo));
                chnlState_e = FMKTIM_CHNLST_DISACTIVATED;
                break;

            case FMKTIM_ECDR_OPE_STOP_BOTH:
                chnl_e = FMKTIM_CHANNEL_ALL;
                //--- ass default take the first one ----//
                Ret_e = SMB_Read(   &g_sfmb_TimChnlInfo_as[timer_e][FMKTIM_CHANNEL_1], 
                                    &chnlInfo_s, 
                                    sizeof(t_sFMKTIM_ChnlInfo));
                chnlState_e = FMKTIM_CHNLST_DISACTIVATED;
                break;

            case FMKTIM_ECDR_OPE_NB:
            default:
                Ret_e = RC_ERROR_NOT_SUPPORTED;
                break;
        }
    }

    // Si OK, appliquer l'état matériel sur le channel
    if(Ret_e == RC_OK)
    {
        Ret_e = s_FMKTIM_Set_HwChannelState(&timerInfo_s,
                                            &chnlInfo_s,
                                            chnlInfo_s.RunMode_e,
                                            timerInfo_s.HwCfg_e,
                                            chnlState_e);
        if(Ret_e == RC_OK)
        {
            Ret_e = SMB_Write(&g_sfmb_TimerInfo_as[timer_e], &timerInfo_s, sizeof(t_sFMKTIM_TimerInfo));
            Ret_e |= SMB_Write(&g_sfmb_TimChnlInfo_as[timer_e][chnl_e], &chnlInfo_s, sizeof(t_sFMKTIM_ChnlInfo));
        }
    }

    if(Ret_e < RC_OK)
    {
        ASSERT((t_uint16)Ret_e);
    }

    return Ret_e;
}
/*********************************
 * FMKTIM_Set_InterruptLineOpe
 *********************************/
t_eReturnCode FMKTIM_Set_EvntLineState(  t_eFMKTIM_InterruptLineEvnt f_EvntITLine_e,
                                        t_eFMKTIM_EvntOpe f_EvntOpe)
{
    t_eReturnCode Ret_e = RC_OK;
    t_eFMKTIM_InterruptChnl chnl_e = FMKTIM_CHANNEL_NB;
    t_eFMKTIM_Timer timer_e = FMKTIM_TIMER_NB;
    t_sFMKTIM_TimerInfo timerInfo_s;
    t_sFMKTIM_ChnlInfo  chnlInfo_s;
    t_eFMKTIM_ChnlState chnlState_e = FMKTIM_CHNLST_DISACTIVATED;

    Ret_e = s_FMKTIM_RqstLineValidityOpe(   FMKTIM_INTERRUPT_LINE_TYPE_EVNT,
                                        (t_uint8)f_EvntITLine_e,
                                        &timer_e,
                                        &chnl_e,
                                        FMKTIM_HWTIM_CFG_EVNT);
    if(Ret_e == RC_OK)
    {
        //--------- Lecture depuis la SRAM via SMB_Read ---------//
        Ret_e = SMB_Read(&g_sfmb_TimerInfo_as[timer_e], &timerInfo_s, sizeof(t_sFMKTIM_TimerInfo));
        if(Ret_e == RC_OK)
        {
            Ret_e = SMB_Read(&g_sfmb_TimChnlInfo_as[timer_e][chnl_e], &chnlInfo_s, sizeof(t_sFMKTIM_ChnlInfo));
        }
    }
    if(Ret_e == RC_OK)
    {
        switch (f_EvntOpe)
        {
            case FMKTIM_EVNT_OPE_START_TIMER:
                chnlState_e = FMKTIM_CHNLST_ACTIVATED;
                break;
            case FMKTIM_EVNT_OPE_STOP_TIMER:
                chnlState_e = FMKTIM_CHNLST_DISACTIVATED;
                break;
            case FMKTIM_EVNT_OPE_NB:
            default:
                Ret_e = RC_ERROR_NOT_SUPPORTED;
                break;
        }
        if(Ret_e == RC_OK)
        {
            Ret_e = s_FMKTIM_Set_HwChannelState(&timerInfo_s,
                                                &chnlInfo_s,
                                                chnlInfo_s.RunMode_e,
                                                timerInfo_s.HwCfg_e,
                                                chnlState_e);
            if(Ret_e == RC_OK)
            {
                Ret_e = SMB_Write(&g_sfmb_TimerInfo_as[timer_e], &timerInfo_s, sizeof(t_sFMKTIM_TimerInfo));
                Ret_e |= SMB_Write(&g_sfmb_TimChnlInfo_as[timer_e][chnl_e], &chnlInfo_s, sizeof(t_sFMKTIM_ChnlInfo));
            }
        }
        else 
        {
            ASSERT((t_uint16)Ret_e);
        }
    }

    return Ret_e;
}

/*********************************
 * FMKTIM_Get_PwmLineValue
 *********************************/
t_eReturnCode FMKTIM_Get_PwmLineValue(  t_eFMKTIM_InterruptLineIO f_Itline_e,
                                        t_sFMKTIM_PwmValue * f_PwmValue_ps,
                                        t_uint8 f_mask_u8)
{
    t_eReturnCode Ret_e = RC_OK;
    t_eFMKTIM_InterruptChnl chnl_e = FMKTIM_CHANNEL_NB;
    t_eFMKTIM_Timer timer_e = FMKTIM_TIMER_NB;
    t_sFMKTIM_TimerInfo timerInfo_s;
    t_uint32 comparedValue_u32;

    Ret_e = s_FMKTIM_RqstLineValidityOpe(   FMKTIM_INTERRUPT_LINE_TYPE_IO,
                                            (t_uint8)f_Itline_e,
                                            (&timer_e),
                                            (&chnl_e),
                                            FMKTIM_HWTIM_CFG_PWM);
    if(Ret_e == RC_OK)
    {
        Ret_e = SMB_Read(&g_sfmb_TimerInfo_as[timer_e], &timerInfo_s, sizeof(t_sFMKTIM_TimerInfo));
    }
    if(Ret_e == RC_OK)
    {
        if(GETBIT(f_mask_u8, FMKTIM_BIT_PWM_DUTYCYCLE) == BIT_IS_SET_8B)
        {
            Ret_e = s_FMKTIM_Get_CCRxValue(&timerInfo_s, chnl_e, &comparedValue_u32);

            if(Ret_e == RC_OK)
            {
                f_PwmValue_ps->dutyCycle_u16 = (t_uint16)(((t_float32)comparedValue_u32 * 1000) /
                                                                    (t_float32)(timerInfo_s.bspTimer_ps->Instance->ARR + 1));
            }
        }
        if(GETBIT(f_mask_u8, FMKTIM_BIT_PWM_FREQUENCY) == BIT_IS_SET_8B)
        {
            //------ calculate frequency -----//
            f_PwmValue_ps->frequency_f32 = (t_float32)((t_float32)timerInfo_s.timerFreqMHz_u32 * CST_MHZ_TO_HZ) /
                                                            (t_float32)((timerInfo_s.bspTimer_ps->Instance->ARR + 1) *
                                                            (timerInfo_s.bspTimer_ps->Instance->PSC + 1));
        }
    }
    else 
    {
        ASSERT((t_uint16)Ret_e);
    }

    return Ret_e;
}

/*********************************
 * FMKTIM_Get_EcdrLineValue
 *********************************/
t_eReturnCode FMKTIM_Get_EcdrLineValue(     t_eFMKTIM_InterruptLineIO f_Itline_e,
                                            t_uint32 * f_position_u32,
                                            t_uint32 * f_direction_u32)
{
    t_eReturnCode Ret_e = RC_OK;
    t_eFMKTIM_InterruptChnl chnl_e = FMKTIM_CHANNEL_NB;
    t_eFMKTIM_Timer timer_e = FMKTIM_TIMER_NB;
    t_sFMKTIM_TimerInfo timerInfo_s;

    Ret_e = s_FMKTIM_RqstLineValidityOpe(   FMKTIM_INTERRUPT_LINE_TYPE_IO,
                                            (t_uint8)f_Itline_e,
                                            (&timer_e),
                                            (&chnl_e),
                                            FMKTIM_HWTIM_CFG_ECDR);
    if(Ret_e == RC_OK)
    {
        Ret_e = SMB_Read(&g_sfmb_TimerInfo_as[timer_e], &timerInfo_s, sizeof(t_sFMKTIM_TimerInfo));
    }
    if(Ret_e == RC_OK)
    {
        if(f_direction_u32 != (t_uint32 *)NULL)
        {
            *f_direction_u32 = (t_uint32)(timerInfo_s.bspTimer_ps->Instance->CR1);
        }
        if(f_position_u32 != (t_uint32 *)NULL)
        {
            *f_position_u32 = (t_uint32)(timerInfo_s.bspTimer_ps->Instance->CNT);
        }
    }

    return Ret_e;
}

/*********************************
 * FMKTIM_Get_ICLineValue
 *********************************/
t_eReturnCode FMKTIM_Get_ICLineValue(   t_eFMKTIM_InterruptLineIO f_Itline_e,
                                        t_sFMKTIM_ICValue * ICValue_ps,
                                        t_uint8 f_mask_u8)
{
    t_eReturnCode Ret_e = RC_OK;
    t_sFMKTIM_TimerInfo timerInfo_s;
    t_eFMKTIM_Timer timer_e = FMKTIM_TIMER_NB;
    t_eFMKTIM_InterruptChnl chnl_e = FMKTIM_CHANNEL_NB;
    t_uint32 comparedValue_u32;

    Ret_e = s_FMKTIM_RqstLineValidityOpe(   FMKTIM_INTERRUPT_LINE_TYPE_IO,
                                            (t_uint8)f_Itline_e,
                                            (&timer_e),
                                            (&chnl_e),
                                            FMKTIM_HWTIM_CFG_IC);
    if(Ret_e == RC_OK)
    {
        //--------- Lecture depuis la SRAM via SMB_Read ---------//
        Ret_e = SMB_Read(&g_sfmb_TimerInfo_as[timer_e], &timerInfo_s, sizeof(t_sFMKTIM_TimerInfo));
    }
    if(Ret_e == RC_OK)
    {
        if(GETBIT(f_mask_u8, FMKTIM_BIT_IC_FREQUENCY) == BIT_IS_SET_8B)
        {
            //------ calculate frequency -----//
            ICValue_ps->frequency_f32 = (t_float32)((t_float32)timerInfo_s.timerFreqMHz_u32 * CST_MHZ_TO_HZ) /
                                                            (t_float32)((timerInfo_s.bspTimer_ps->Instance->ARR + 1) *
                                                            (timerInfo_s.bspTimer_ps->Instance->PSC + 1));
        }
        if(GETBIT(f_mask_u8, FMKTIM_BIT_IC_ARR_REGISTER) == BIT_IS_SET_8B)
        {
            ICValue_ps->ARR_Register_u32 = (t_uint32)(timerInfo_s.bspTimer_ps->Instance->ARR);
        }
        if(GETBIT(f_mask_u8, FMKTIM_BIT_IC_CCRX_REGISTER) == BIT_IS_SET_8B)
        {   
            Ret_e = s_FMKTIM_Get_CCRxValue(&timerInfo_s, chnl_e, &comparedValue_u32);

            if(Ret_e == RC_OK)
            {
                ICValue_ps->CCRxRegister_u16 = (t_uint16)comparedValue_u32;
            }
        }
    }

    return Ret_e;
}

/*********************************
 * FMKTIM_Get_LineErrorStatus
 *********************************/
t_eReturnCode FMKTIM_Get_LineErrorStatus(    t_eFMKTIM_InterruptLineType f_ITLineType_e,
                                                t_uint32 f_IT_line_u8,
                                                t_uint16 *f_chnlErrInfo_pu16)
{
    t_eReturnCode Ret_e = RC_OK;
    t_eFMKTIM_Timer timer_e = FMKTIM_TIMER_NB;
    t_eFMKTIM_InterruptChnl chnl_e = FMKTIM_CHANNEL_NB;
    t_sFMKTIM_ChnlInfo  chnlInfo_s;

    if(f_ITLineType_e >= FMKTIM_INTERRUPT_LINE_TYPE_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
    }
    if(g_FmkTim_ModState_e != STATE_CYCLIC_OPE)
    {
        Ret_e = RC_WARNING_BUSY;
    }
    if(f_chnlErrInfo_pu16 == (t_uint16 *)NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }
    if(Ret_e == RC_OK)
    {
        Ret_e = s_FMKTIM_Get_TimChnlFromITLine(f_ITLineType_e,
                                               f_IT_line_u8,
                                               &timer_e,
                                               &chnl_e);
        if(Ret_e == RC_OK)
        {
            Ret_e = SMB_Read(&g_sfmb_TimChnlInfo_as[timer_e][chnl_e], &chnlInfo_s, sizeof(t_sFMKTIM_ChnlInfo));
        }
        if(Ret_e == RC_OK)
        {
            *f_chnlErrInfo_pu16 = chnlInfo_s.ErrState_e;
        }
        else 
        {
            ASSERT((t_uint16)Ret_e);
        }
        
    }

    return Ret_e;
}

/*********************************
 * FMKTIM_PRIVATE_GetHandleTypeDef
 *********************************/
TIM_HandleTypeDef * FMKTIM_PRIVATE_GetHandleTypeDef(t_uint8 f_timer_u8)
{
    if(g_TimerInfo_as[f_timer_u8].isConfigured_b == (t_bool)False)
    {
        ASSERT((t_uint16)0);
    }
    return (TIM_HandleTypeDef *)(g_TimerInfo_as[f_timer_u8].bspTimer_ps);
}
//********************************************************************************
//                      Local functions - Implementation
//********************************************************************************
/*********************************
 * s_FMKTIM_Operational
 *********************************/
static t_eReturnCode s_FMKTIM_Operational(void)
{
    t_eReturnCode Ret_e = RC_OK;
    t_uint8 idxTimer_u8;
    static t_uint32 SavedTime_u32 = 0;
    t_uint32 currentTime_u32 = 0;

    FMKCPU_GetTick(&currentTime_u32);

    for(idxTimer_u8 = (t_uint8)0 ; idxTimer_u8 < FMKTIM_TIMER_NB ; idxTimer_u8++)
    {
        if(g_TimerInfo_as[idxTimer_u8].isConfigured_b == (t_bool)TRUE)
        {
            if((g_TimerInfo_as[idxTimer_u8].errDetected_b == (t_bool)TRUE)
            || ((currentTime_u32 - SavedTime_u32) > (t_uint32)FMKTIM_TIME_BTWN_DIAG_MS))
            {
                SavedTime_u32 = currentTime_u32;
                Ret_e = s_FMKTIM_PerformDiagnostic(idxTimer_u8);

            }
        }
    }
    // else do other thing( or nothing for now)
    
    return Ret_e;
}

/*********************************
 * s_FMKTIM_PerformDiagnostic
 *********************************/
static t_eReturnCode s_FMKTIM_PerformDiagnostic(t_eFMKTIM_Timer f_timer_e)
{
    t_eReturnCode Ret_e = RC_OK;
    HAL_TIM_StateTypeDef bspTimerState_e;
    HAL_TIM_ChannelStateTypeDef bspChnlState_e = HAL_TIM_CHANNEL_STATE_READY;
    t_uint32 bspChannel_u32 = 0;
    t_sFMKTIM_TimerInfo * timerInfo_ps;
    t_sFMKTIM_ChnlInfo * chnlInfo_ps;
    t_uint8 CLLI_u8; /**< Channel loop */
    t_uint16 debugInfo1_u16 = (t_uint16)0;
    t_uint32 currentTime_u32 = (t_uint32)0;
    t_bool isDiagOn_b = (t_bool)FALSE;

    if(f_timer_e >= FMKTIM_TIMER_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
        ASSERT((t_uint16)0);
    }
    else
    {
        //-----------update information for loop-----------//
        timerInfo_ps = (t_sFMKTIM_TimerInfo *)&g_TimerInfo_as[f_timer_e];
        FMKCPU_GetTick(&currentTime_u32);
        //-----------diag for timer currently working/ Running-----------//
        if(timerInfo_ps->IsTimerRunning_b == (t_bool)True)
        {
            //-----------Timer diagnostic-----------//
            bspTimerState_e = c_FMKTIM_TimerFunc_apf[timerInfo_ps->HwCfg_e].GetTimerState_pcb(
                                    timerInfo_ps->bspTimer_ps);

            //----------- associate bsp err with channel FMKTIM err-----------//
            if((bspTimerState_e  == HAL_TIM_STATE_BUSY)
            || (bspTimerState_e  == HAL_TIM_STATE_READY))
            {
                timerInfo_ps->errorId_e = FMKTIM_ERRSTATE_OK; 
            }
            else if(bspTimerState_e  == HAL_TIM_STATE_RESET)
            {
                timerInfo_ps->errorId_e = FMKTIM_ERRSTATE_NOT_CONFIGURED; 
            }
            else if(bspTimerState_e  == HAL_TIM_STATE_TIMEOUT)
            {
                timerInfo_ps->errorId_e = FMKTIM_ERRSTATE_TIMEOUT;
            }
            else if(bspTimerState_e == HAL_TIM_STATE_ERROR)
            {
                timerInfo_ps->errorId_e =  HAL_TIM_STATE_ERROR;  
            }
            else 
            {
                //--- shouls never be here but never know 
                timerInfo_ps->errorId_e = FMKTIM_ERRSTATE_UNKNOWN;
            }
            
            //-----------diag for the channel currently used-----------//
            for(CLLI_u8 = (t_uint8)0 ; CLLI_u8 < FMKTIM_CHANNEL_NB ; CLLI_u8++)
            {
                chnlInfo_ps = (t_sFMKTIM_ChnlInfo *)(&g_TimChnlInfo_as[f_timer_e][CLLI_u8]);

                if(chnlInfo_ps->State_e == FMKTIM_CHNLST_ACTIVATED)
                {
                    Ret_e = s_FMKTIM_Get_BspChannel(CLLI_u8, &bspChannel_u32);

                    if(Ret_e == RC_OK)
                    {
                        bspChnlState_e = HAL_TIM_GetChannelState(timerInfo_ps->bspTimer_ps, bspChannel_u32);

                        if((bspChnlState_e != HAL_TIM_CHANNEL_STATE_BUSY)) // busy means ok
                        {
                            chnlInfo_ps->ErrState_e = FMKTIM_ERRSTATE_OFF_UNEXPECTED;
                        }
                        else 
                        {
                            chnlInfo_ps->ErrState_e = FMKTIM_ERRSTATE_OK;
                        }
                    }
                }
            }
            //---- now diagnostic active/deactive maangement ----//
            if(timerInfo_ps->errDetected_b == (t_bool)TRUE)
            {
                if(timerInfo_ps->errorId_e != FMKTIM_ERRSTATE_OK)
                {
                    isDiagOn_b = (t_bool)TRUE;
                    debugInfo1_u16 = (t_uint16)(f_timer_e << 8) | (t_uint8)0xFF; // means no channel
                    APPSDM_ReportDiagEvnt(  APPSDM_DIAG_ITEM_FMK_TIM_OPE_ERROR,
                                            APPSDM_DIAG_ITEM_REPORT_FAIL,
                                            (t_uint16)debugInfo1_u16,
                                            (t_uint16)timerInfo_ps->errorId_e);
                }
                
                for(CLLI_u8 = (t_uint8)0 ; CLLI_u8 < FMKTIM_CHANNEL_NB; CLLI_u8++)
                {
                    if((g_TimChnlInfo_as[f_timer_e][CLLI_u8].IsChnlConfigure_b == (t_bool)TRUE)
                    && (g_TimChnlInfo_as[f_timer_e][CLLI_u8].ErrState_e != FMKTIM_ERRSTATE_OK))
                    {
                        isDiagOn_b = (t_bool)TRUE;
                        debugInfo1_u16 = (t_uint16)(f_timer_e << (t_uint8)8 | CLLI_u8);
                        APPSDM_ReportDiagEvnt(  APPSDM_DIAG_ITEM_FMK_TIM_OPE_ERROR,
                                                APPSDM_DIAG_ITEM_REPORT_FAIL,
                                                (t_uint16)debugInfo1_u16,
                                                (t_uint16)g_TimChnlInfo_as[f_timer_e][CLLI_u8].ErrState_e);
                    }
                }
            }
        }
        if(isDiagOn_b == (t_bool)FALSE)
        {
            APPSDM_ReportDiagEvnt(  APPSDM_DIAG_ITEM_FMK_TIM_OPE_ERROR,
                                    APPSDM_DIAG_ITEM_REPORT_PASS,
                                    (t_uint16)0,
                                    (t_uint16)0);
            timerInfo_ps->errDetected_b = (t_bool)FALSE;
        }
    }

    return Ret_e;
}

/*********************************
 * s_FMKTIM_RqstLineValidityOpe
 *********************************/
static t_eReturnCode s_FMKTIM_RqstLineValidityOpe(  t_eFMKTIM_InterruptLineType f_ITLineType_e,
                                                    t_uint8 f_Itline_u8,
                                                    t_eFMKTIM_Timer * f_timer_pe,
                                                    t_eFMKTIM_InterruptChnl * f_chnl_pe,
                                                    t_eFMKTIM_HwTimerCfg f_HwTypeSuppose_e)
{
    t_eReturnCode Ret_e = RC_OK;
    t_eFMKTIM_Timer timer_e = FMKTIM_TIMER_NB;
    t_eFMKTIM_InterruptChnl chnl_e = FMKTIM_CHANNEL_NB;

    t_sFMKTIM_TimerInfo * timerInfo_ps;
    t_sFMKTIM_ChnlInfo * channel_pas;

    if(f_ITLineType_e >= FMKTIM_INTERRUPT_LINE_TYPE_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
        ASSERT((t_uint16)Ret_e);
    }
    else if((f_timer_pe == (t_eFMKTIM_Timer *)NULL)
    ||      (f_chnl_pe == (t_eFMKTIM_InterruptChnl *)NULL))
    {
        Ret_e = RC_ERROR_PTR_NULL;
        ASSERT((t_uint16)Ret_e);
    }
    else if(g_FmkTim_ModState_e != STATE_CYCLIC_OPE
    && g_FmkTim_ModState_e != STATE_CYCLIC_PREOPE)
    {
        Ret_e = RC_WARNING_BUSY;
        ASSERT((t_uint16)Ret_e);
    }
    else
    {
        Ret_e = s_FMKTIM_Get_TimChnlFromITLine(f_ITLineType_e,
                                               (t_uint32)f_Itline_u8,
                                               &timer_e,
                                               &chnl_e);

        //--- here we're allawed to get direct access of structure 
        //      cause it's just check flag that does not change ----//
        if((Ret_e == RC_OK)
        && (timer_e < FMKTIM_TIMER_NB))
        {
            timerInfo_ps = (t_sFMKTIM_TimerInfo *)(&g_TimerInfo_as[timer_e]);
            channel_pas = (t_sFMKTIM_ChnlInfo *)(g_TimChnlInfo_as[timer_e]);
            // min channel all
            if((chnl_e != FMKTIM_CHANNEL_ALL)
            && (chnl_e < FMKTIM_CHANNEL_NB))
            {
                if((timerInfo_ps->isConfigured_b == (t_bool)False)
                || (channel_pas[chnl_e].IsChnlConfigure_b == (t_bool)False))
                {
                    Ret_e = RC_ERROR_INSTANCE_NOT_INITIALIZED;
                    ASSERT((t_uint16)Ret_e);
                }
            }
            else if(chnl_e == FMKTIM_CHANNEL_ALL)
            {
                if((timerInfo_ps->isConfigured_b == (t_bool)False)
                || (channel_pas[FMKTIM_CHANNEL_1].IsChnlConfigure_b == (t_bool)False)
                || (channel_pas[FMKTIM_CHANNEL_2].IsChnlConfigure_b == (t_bool)False))
                {
                    Ret_e = RC_ERROR_INSTANCE_NOT_INITIALIZED;
                    ASSERT((t_uint16)Ret_e);
                }
            }
            else 
            {
                Ret_e = RC_ERROR_PARAM_INVALID;
                ASSERT((t_uint16)Ret_e);
            }
            if(timerInfo_ps->HwCfg_e != f_HwTypeSuppose_e)
            {
                Ret_e = RC_ERROR_WRONG_CONFIG;
                ASSERT((t_uint16)Ret_e);
            }
        }
        if(Ret_e == RC_OK)
        {
            *f_timer_pe = timer_e,
            *f_chnl_pe = chnl_e;
        }
    }

    return Ret_e;
}
/*********************************
 * s_FMKTIM_Set_PwmChannelCfg
 *********************************/
static t_eReturnCode s_FMKTIM_Set_PwmChannelCfg(t_sFMKTIM_TimerInfo * f_timerInfo_ps,
                                                t_sFMKTIM_ChnlInfo * f_chnlInfo_ps,
                                                t_uint32 f_pwmFreq_u32,
                                                t_eFMKTIM_LinePolarity f_linePolarity_e)
{
    /********************************
     *   Some useful information for PWM generation
     *   1 - ARR = Period in Init =  (((freq_timer)/ (freq_pwm * (PSC+1))) -1)
     *   For a dutycyle E [0 - 1000]
     *   2 - CCR1_2_3_4 depending on channel, set the Duty Cycle -> (DutyCyle/ 1000) * (ARR +1)
     ********************************/
    t_eReturnCode Ret_e = RC_OK;
    HAL_StatusTypeDef BspRet_e = HAL_OK;
    HAL_TIM_ChannelStateTypeDef bspChannelState_e = HAL_TIM_CHANNEL_STATE_BUSY;
    TIM_OC_InitTypeDef BspOcInit_s;
    TIM_MasterConfigTypeDef masterCfg_s;
    t_uint32 bspChannel_u32 = 0;
    
    if((f_timerInfo_ps == (t_sFMKTIM_TimerInfo *)NULL)
    || (f_chnlInfo_ps == (t_sFMKTIM_ChnlInfo *)NULL))
    {
        Ret_e = RC_ERROR_PTR_NULL;
        ASSERT((t_uint16)0);
    }
    //----------Check if timer is not configured yet------------------//
    else if ((f_timerInfo_ps->isConfigured_b == (t_bool)False))
    {
        ASSERT((t_uint16)Ret_e);
        Ret_e = RC_ERROR_INSTANCE_NOT_INITIALIZED;
    }
    //-------this timer has already been configured and cannot be used for another Type of Configuration------//
    else if (f_timerInfo_ps->HwCfg_e != FMKTIM_HWTIM_CFG_PWM)
    { 
        ASSERT((t_uint16)Ret_e);
        Ret_e = RC_ERROR_NOT_ALLOWED;
    }
    //-------Channel is already used------//
    else if (f_chnlInfo_ps->IsChnlConfigure_b == (t_bool)True)
    {
        ASSERT((t_uint16)Ret_e);
        Ret_e = RC_ERROR_ALREADY_CONFIGURED;
    }
    else
    {   
        //-----Function get here only if Timer is initialized and channel not initialized yet----//
        //----Now Configure the channel in PWM mode-----//
        Ret_e = s_FMKTIM_Get_BspChannel(f_chnlInfo_ps->selfId_e, &bspChannel_u32);

        if (Ret_e == RC_OK)
        {
            //-------see if channel state is ok-------//
            bspChannelState_e = HAL_TIM_GetChannelState(f_timerInfo_ps->bspTimer_ps, bspChannel_u32);

            if(bspChannelState_e == HAL_TIM_CHANNEL_STATE_READY)
            {
                if(f_linePolarity_e == FMKTIM_LINE_POLARITY_HIGH)
                {
                    BspOcInit_s.OCMode = TIM_OCMODE_PWM1;       // Mode PWM1
                }
                else 
                {
                    BspOcInit_s.OCMode = TIM_OCMODE_PWM2;
                }
                BspOcInit_s.Pulse = 0;                     // Initial Duty Cycle à 0%
                BspOcInit_s.OCFastMode = TIM_OCFAST_DISABLE;    // Mode rapide désactivé
                BspOcInit_s.OCPolarity = TIM_OCPOLARITY_HIGH;  // Polarité normale (actif haut)
#ifdef FMKCPU_STM32_ECU_FAMILY_G
                BspOcInit_s.OCNPolarity = TIM_OCNPOLARITY_HIGH; // Non utilisé si pas de signal complémentaire
                BspOcInit_s.OCIdleState = TIM_OCIDLESTATE_RESET; // État au repos à 0
                BspOcInit_s.OCNIdleState = TIM_OCNIDLESTATE_RESET; // Non utilisé si pas de signal complémentaire
#endif
                BspRet_e = HAL_TIM_PWM_ConfigChannel(   f_timerInfo_ps->bspTimer_ps,
                                                        &BspOcInit_s,
                                                        (uint32_t)bspChannel_u32);  
                if (BspRet_e != HAL_OK)
                {
                    Ret_e = RC_ERROR_WRONG_STATE;
                }
            }
            else
            {
                Ret_e = RC_WARNING_BUSY;
            }
        }
        if(Ret_e == RC_OK)
        {
            masterCfg_s.MasterOutputTrigger = TIM_TRGO_RESET;
            masterCfg_s.MasterOutputTrigger2 = TIM_TRGO2_RESET;
            masterCfg_s.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
            BspRet_e = HAL_TIMEx_MasterConfigSynchronization(   f_timerInfo_ps->bspTimer_ps,
                                                                &masterCfg_s);

            if(BspRet_e != HAL_OK)
            {
                ASSERT((t_uint16)Ret_e);
                Ret_e = RC_ERROR_WRONG_RESULT;
            }
        }
        if (Ret_e == RC_OK)
        { //----------Update INformation------------//
            f_chnlInfo_ps->IsChnlConfigure_b = (t_bool)True;
        }
        else
        {
            ASSERT((t_uint16)Ret_e);
            f_chnlInfo_ps->ErrState_e = FMKTIM_ERRSTATE_INIT_FAILED;
        }
        
    }
    
    return Ret_e;
}

/*********************************
 * s_FMKTIM_Set_EcdrChannelCfg
 *********************************/
static t_eReturnCode s_FMKTIM_Set_EcdrChannelCfg(   t_sFMKTIM_TimerInfo * f_timerInfo_ps,
                                                    t_sFMKTIM_EcdrCfg * f_EcdrCfg_ps,
                                                    t_uint32 f_ARRValue_u32)
{
    t_eReturnCode Ret_e = RC_OK;
    HAL_StatusTypeDef bspRet_e = HAL_OK;
    t_uint32 computeARRVal_u32;
    TIM_Encoder_InitTypeDef bspEcdrInit_s;
    TIM_MasterConfigTypeDef masterCfg_s;

    //----------Check if timer is not configured yet------------------//
    if ((f_timerInfo_ps->isConfigured_b == (t_bool)True))
    {
        ASSERT((t_uint16)Ret_e);
        Ret_e = RC_ERROR_ALREADY_CONFIGURED;
    }
    else
    {
        Ret_e = s_FMKTIM_FilledBspEcdrInit(f_EcdrCfg_ps, &bspEcdrInit_s);
        
        if(Ret_e == RC_OK)
        {
            Ret_e = s_FMKTIM_GetEcdrARRValue(   f_EcdrCfg_ps->HwMode_e,
                                                f_ARRValue_u32,
                                                &computeARRVal_u32);
            if(Ret_e == RC_OK)
            {
                Ret_e = s_FMKTIM_Set_BspTimerInit(  f_timerInfo_ps,
                                                    FMKTIM_HWTIM_CFG_ECDR,
                                                    computeARRVal_u32,
                                                    (void *)(&bspEcdrInit_s));
            }      
            if(Ret_e == RC_OK)
            {
                masterCfg_s.MasterOutputTrigger = TIM_TRGO_RESET;
                masterCfg_s.MasterOutputTrigger2 = TIM_TRGO2_RESET;
                masterCfg_s.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
                bspRet_e = HAL_TIMEx_MasterConfigSynchronization(   f_timerInfo_ps->bspTimer_ps,
                                                                    &masterCfg_s);

                if(bspRet_e != HAL_OK)
                {
                    Ret_e = RC_ERROR_WRONG_RESULT;
                }
            }                                            
        }
        if(Ret_e < RC_OK)
        {
            ASSERT((t_uint16)Ret_e);
        }
    }

    return Ret_e;
}

/*********************************
 * s_FMKTIM_Set_ICChannelCfg
 *********************************/
static t_eReturnCode s_FMKTIM_Set_ICChannelCfg( t_sFMKTIM_TimerInfo * f_timerInfo_ps,
                                                t_sFMKTIM_ChnlInfo * f_chnlInfo_ps,
                                                t_eFMKTIM_ChnlMeasTrigger f_MeasTrigger_e,
                                                t_cbFMKTIM_InterruptLine f_ITChannel_cb)
{
    t_eReturnCode Ret_e = RC_OK;
    HAL_StatusTypeDef BspRet_e = HAL_OK;
    HAL_TIM_ChannelStateTypeDef bspChannelState_e = HAL_TIM_CHANNEL_STATE_BUSY;
    TIM_IC_InitTypeDef BspICInit_s;
    t_uint32 bspChannel_u32 = 0;
    t_uint32 bspMeasPolarity_u32 = 0;
    
    if((f_timerInfo_ps == (t_sFMKTIM_TimerInfo *)NULL)
    || f_chnlInfo_ps == (t_sFMKTIM_ChnlInfo *)NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
        ASSERT((t_uint16)0);
    }
    //-------Channel is already used------//
    else if (f_chnlInfo_ps->IsChnlConfigure_b == (t_bool)True)
    {
        ASSERT((t_uint16)0);
        Ret_e = RC_ERROR_ALREADY_CONFIGURED;
    }
    else
    {
        //----------Check if timer is not configured yet------------------//
        if(f_timerInfo_ps->isConfigured_b == (t_bool)FALSE)
        {
        
            //----------Set Input Compare configuration for all channels------------------//
            Ret_e = s_FMKTIM_Set_BspTimerInit(  f_timerInfo_ps,
                                                FMKTIM_HWTIM_CFG_IC,
                                                (t_uint32)40,
                                                (void *)NULL);
        }
        if((Ret_e == RC_OK)
        && (f_timerInfo_ps->isConfigured_b == (t_bool)TRUE))
        {
            //-------this timer has already been configured and cannot be used for another Type of Configuration------//
            if (f_timerInfo_ps->HwCfg_e != FMKTIM_HWTIM_CFG_IC)
            {
                Ret_e = RC_ERROR_NOT_ALLOWED;
                ASSERT((t_uint16)0);
            }
        }

        if(Ret_e == RC_OK)
        {
            Ret_e = s_FMKTIM_Get_BspChannel(f_chnlInfo_ps->selfId_e, &bspChannel_u32);
            if (Ret_e == RC_OK)
            {
                Ret_e = s_FMKTIM_Get_BspICTrigger(f_MeasTrigger_e, &bspMeasPolarity_u32);
            }
            if(Ret_e == RC_OK)
            {
                bspChannelState_e = HAL_TIM_GetChannelState(f_timerInfo_ps->bspTimer_ps, bspChannel_u32);
                
                if(bspChannelState_e == HAL_TIM_CHANNEL_STATE_READY)
                {
                    //-------Configure bsp channel structure------//
                    BspICInit_s.ICPolarity = (t_uint32)bspMeasPolarity_u32;
                    BspICInit_s.ICSelection = (t_uint32)TIM_ICSELECTION_DIRECTTI;
                    BspICInit_s.ICPrescaler = (t_uint32)TIM_ICPSC_DIV1;
                    BspICInit_s.ICFilter = (t_uint32)0;
                    BspRet_e = HAL_TIM_IC_ConfigChannel(f_timerInfo_ps->bspTimer_ps,
                                                        &BspICInit_s,
                                                        bspChannel_u32);
                    if (BspRet_e != HAL_OK)
                    {
                        Ret_e = RC_ERROR_WRONG_STATE;
                    }
                }
                else
                {
                    Ret_e = RC_WARNING_BUSY;
                }
            }
            if (Ret_e == RC_OK)
            { 
                //-------update information-------//
                f_chnlInfo_ps->IsChnlConfigure_b = (t_bool)True;
                f_chnlInfo_ps->RunMode_e = FMKTIM_LINE_RUNMODE_INTERRUPT;
                f_chnlInfo_ps->chnl_cb = f_ITChannel_cb;
            }
            else
            {
                ASSERT((t_uint16)Ret_e);
                f_chnlInfo_ps->ErrState_e =  FMKTIM_ERRSTATE_INIT_FAILED;
            }
        }
    }
    
    return Ret_e;
}

/*********************************
 * s_FMKTIM_Set_EvntChannelCfg
 *********************************/
static t_eReturnCode s_FMKTIM_Set_EvntChannelCfg(   t_sFMKTIM_TimerInfo * f_timerInfo_ps,
                                                    t_sFMKTIM_ChnlInfo * f_chnlInfo_ps,
                                                    t_uint32 f_periodms_u32,
                                                    t_cbFMKTIM_InterruptLine f_ITChannel_cb)
{
    t_eReturnCode Ret_e = RC_OK;
    t_float32 freqEvnt_f32 = 0.0f;

    if((f_timerInfo_ps == (t_sFMKTIM_TimerInfo *)NULL)
    || f_chnlInfo_ps == (t_sFMKTIM_ChnlInfo *)NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
        ASSERT((t_uint16)0);
    }
    //-------------See if timer has already been configured-------------//
    else if(f_timerInfo_ps->isConfigured_b == (t_bool)True)
    {
        //-------------For Timer Event Configuration
        // As the timer itself generate the Interrutpion
        // You can use only on Aplication Channel-------------//
        Ret_e = RC_ERROR_ALREADY_CONFIGURED;
        ASSERT((t_uint16)Ret_e);
    }
    else
    {
        //----------Get the prescaler/ ARR for the timer ------------------//
        freqEvnt_f32 = (t_float32)(((t_float32)1.0f)/(f_periodms_u32/ (t_float32)CST_MSEC_TO_SEC));

        //-------------Call Init Function-------------//
        Ret_e = s_FMKTIM_Set_BspTimerInit(  f_timerInfo_ps,
                                            FMKTIM_HWTIM_CFG_EVNT,
                                            freqEvnt_f32,
                                            (void *)NULL);
        if (Ret_e == RC_OK)
        {
            //-------------Update Information-------------//
            f_timerInfo_ps->isConfigured_b = (t_bool)True;
            f_chnlInfo_ps->chnl_cb = f_ITChannel_cb;
            f_chnlInfo_ps->RunMode_e = FMKTIM_LINE_RUNMODE_INTERRUPT;
            f_chnlInfo_ps->IsChnlConfigure_b = (t_bool)True;
        }
        else
        {
            ASSERT((t_uint16)Ret_e);
            f_chnlInfo_ps->ErrState_e = FMKTIM_ERRSTATE_INIT_FAILED;
        }
    }

    return Ret_e;
}

/*********************************
 * s_FMKTIM_FilledBspEcdrInit
 *********************************/
static t_eReturnCode s_FMKTIM_FilledBspEcdrInit(t_sFMKTIM_EcdrCfg * f_EcdrCdg_ps,
                                                TIM_Encoder_InitTypeDef * f_BspEcdrInit_ps)
{
    t_eReturnCode Ret_e = RC_OK;
    t_uint32 bspEcdrMode_u32;
    t_uint32 bspIC1Selection_u32 = (t_uint32)0;
    t_uint32 bspIC2Selection_u32 = (t_uint32)0;
    t_uint32 bspIC1Polarity_u32 = (t_uint32)0;
    t_uint32 bspIC2Polarity_u32 = (t_uint32)0;

    if((f_EcdrCdg_ps == (t_sFMKTIM_EcdrCfg *)NULL)
    || (f_BspEcdrInit_ps == (TIM_Encoder_InitTypeDef *)NULL))
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }
    if(Ret_e == RC_OK)
    {
        //-------- Get Encoder Mode--------//
        Ret_e = s_FMKTIM_GetBspEcdrMode(f_EcdrCdg_ps->HwMode_e, &bspEcdrMode_u32);

        //-------- Get Encoder IC1 Selection--------//
        if(Ret_e == RC_OK)
        {
            Ret_e = s_FMKTIM_GetBspEcdrSelection(f_EcdrCdg_ps->IC1_s.Selection_e, &bspIC1Selection_u32);
        }
        //-------- Get Encoder IC1 Polarity--------//
        if(Ret_e == RC_OK)
        {
            s_FMKTIM_GetBspEcdrPolarity(f_EcdrCdg_ps->IC1_s.Polarity_e, &bspIC1Polarity_u32);
        }
        //-------- Get Encoder IC2 Selection--------//
        if(Ret_e == RC_OK)
        {
            Ret_e = s_FMKTIM_GetBspEcdrSelection(f_EcdrCdg_ps->IC1_s.Selection_e, &bspIC2Selection_u32);
        }
        //-------- Get Encoder IC2 Polarity--------//
        if(Ret_e == RC_OK)
        {
            s_FMKTIM_GetBspEcdrPolarity(f_EcdrCdg_ps->IC1_s.Polarity_e, &bspIC2Polarity_u32);
        }
        if(Ret_e == RC_OK)
        {
            //-------- Copy Data--------//
            f_BspEcdrInit_ps->EncoderMode = bspEcdrMode_u32;
            f_BspEcdrInit_ps->IC1Filter = 0x03;
            f_BspEcdrInit_ps->IC1Prescaler = TIM_ICPSC_DIV1;
            f_BspEcdrInit_ps->IC1Polarity = bspIC1Polarity_u32;
            f_BspEcdrInit_ps->IC1Selection = bspIC1Selection_u32;
            f_BspEcdrInit_ps->IC2Filter = 0x03;
            f_BspEcdrInit_ps->IC2Prescaler = TIM_ICPSC_DIV1;
            f_BspEcdrInit_ps->IC2Polarity = bspIC2Polarity_u32;
            f_BspEcdrInit_ps->IC2Selection = bspIC2Selection_u32;
        }
        else 
        {
            ASSERT((t_uint16)Ret_e);
        }
    }

    return Ret_e;
}

/*********************************
 * s_FMKTIM_Set_HwChannelState
 *********************************/
static t_eReturnCode s_FMKTIM_Set_HwChannelState( t_sFMKTIM_TimerInfo * f_timInfo_ps,
                                                  t_sFMKTIM_ChnlInfo * f_chnlInfo_ps,
                                                  t_eFMKTIM_ChnlRunMode f_runMode_e,
                                                  t_eFMKTIM_HwTimerCfg f_hwTimCfg_e,
                                                  t_eFMKTIM_ChnlState f_chnlState_e)
{
    t_eReturnCode Ret_e = RC_OK;
    t_uint32 bspChannel_u32 = 0;
    HAL_StatusTypeDef bspRet_e = HAL_OK;

    if((f_timInfo_ps == (t_sFMKTIM_TimerInfo *) NULL)
    || (f_chnlInfo_ps == (t_sFMKTIM_ChnlInfo *)NULL))
    {
        Ret_e = RC_ERROR_PTR_NULL;
        ASSERT((t_uint16)0);
    }
    if((f_runMode_e >= FMKTIM_LINE_RUNMODE_NB)
    || (f_hwTimCfg_e >= FMKTIM_HWTIM_CFG_NB))
    {
        ASSERT((t_uint16)0);
        Ret_e = RC_ERROR_PARAM_INVALID;
    }

    if (Ret_e == RC_OK)
    {
        Ret_e = s_FMKTIM_Get_BspChannel(f_chnlInfo_ps->selfId_e, &bspChannel_u32);
    }
    if (Ret_e == RC_OK)
    { 
            //--------Depending on wehter we need to activate or deactivate function--------//
        if(f_chnlState_e ==  FMKTIM_CHNLST_ACTIVATED)
        {
            //--------Update Mask Channel status--------//
            f_timInfo_ps->mskChnlState_u16 |= 1 << (t_uint16)(f_chnlInfo_ps->selfId_e);

            //-------- We have to start HALfunction in Basic or Interrupt depending on f_chnlRunMode_e--------//
            //--------call the right HAL function Polling or Interrupt mode--------//
            if ((f_runMode_e != FMKTIM_LINE_RUNMODE_POLLING)
            &&  (f_timInfo_ps->IsNVICTimerEnable_b == (t_bool)False))
            {
                Ret_e = FMKCPU_Set_NVICState(f_timInfo_ps->c_IRQNType_e, FMKCPU_NVIC_OPE_ENABLE);

                if(Ret_e == RC_OK)
                {
                    f_timInfo_ps->IsNVICTimerEnable_b = (t_bool)True;
                }
            }
            switch(f_runMode_e)
                {
                case FMKTIM_LINE_RUNMODE_POLLING:
                    bspRet_e = c_FMKTIM_TimerFunc_apf[f_hwTimCfg_e].
                                    StartFuncPoll_pcb(f_timInfo_ps->bspTimer_ps,
                                                        (t_uint32)bspChannel_u32);
                    break;
                case FMKTIM_LINE_RUNMODE_INTERRUPT:
                    bspRet_e = c_FMKTIM_TimerFunc_apf[f_hwTimCfg_e].
                                    StartFuncInterrupt_pcb(f_timInfo_ps->bspTimer_ps,
                                                            (t_uint32)bspChannel_u32);
                    break;
                case FMKTIM_LINE_RUNMODE_DMA:
                {
                    bspRet_e = c_FMKTIM_TimerFunc_apf[f_hwTimCfg_e].
                                    StartFuncDma_pcb(f_timInfo_ps->bspTimer_ps,
                                                    (t_uint32)bspChannel_u32,
                                                    f_chnlInfo_ps->DmaInfo_ps.BufferAdd1_pu32,
                                                    f_chnlInfo_ps->DmaInfo_ps.BufferAdd2_pu32,
                                                    f_chnlInfo_ps->DmaInfo_ps.bufferLen_u16);

                    break;
                }
                case FMKTIM_LINE_RUNMODE_NB:
                default:
                    Ret_e = RC_ERROR_NOT_SUPPORTED;
                    break;
                }
            
        }
        else // disactivated
        {     
            //--------Update Mask Channel status--------//
            f_timInfo_ps->mskChnlState_u16 &= ~(1 << (t_uint32)(f_chnlInfo_ps->selfId_e));
            if (f_timInfo_ps->IsNVICTimerEnable_b == (t_bool)True)
            {
                Ret_e = FMKCPU_Set_NVICState(f_timInfo_ps->c_IRQNType_e, FMKCPU_NVIC_OPE_DISABLE);

                if(Ret_e == RC_OK)
                {
                    f_timInfo_ps->IsNVICTimerEnable_b = (t_bool)False;
                }
            }
            if (Ret_e == RC_OK)
            {
                switch (f_runMode_e)
                {
                    case FMKTIM_LINE_RUNMODE_POLLING:
                        bspRet_e = c_FMKTIM_TimerFunc_apf[f_hwTimCfg_e].
                                        StopFuncPoll_pcb(f_timInfo_ps->bspTimer_ps,
                                        (t_uint32)bspChannel_u32);
                        break;

                    case FMKTIM_LINE_RUNMODE_INTERRUPT:
                        bspRet_e = c_FMKTIM_TimerFunc_apf[f_hwTimCfg_e].
                                        StopFuncInterrupt_pcb(f_timInfo_ps->bspTimer_ps,
                                                                (t_uint32)bspChannel_u32);
                        break;

                    case FMKTIM_LINE_RUNMODE_DMA:
                    {
                        bspRet_e = c_FMKTIM_TimerFunc_apf[f_hwTimCfg_e].
                                    StopFuncDma_pcb(f_timInfo_ps->bspTimer_ps,
                                                    (t_uint32)bspChannel_u32);
                        break;
                    }
                    case FMKTIM_LINE_RUNMODE_NB:
                    default:
                        Ret_e = RC_ERROR_NOT_SUPPORTED;
                        break;
                }
            }
        }
        if (bspRet_e != HAL_OK)
        {
            Ret_e = RC_ERROR_WRONG_RESULT;
        }
        else
        {
            //--------Update Information--------//
            if((f_runMode_e == f_chnlInfo_ps->RunMode_e) && (Ret_e == RC_OK))
            {
                f_timInfo_ps->IsTimerRunning_b =
                    (f_timInfo_ps->mskChnlState_u16 == (t_uint32)0)? False : True;
                    
                f_chnlInfo_ps->State_e = f_chnlState_e;
            }
        }
    }
    return Ret_e;
}

/*********************************
 * FMKCPU_Get_PWMChannelDuty
 *********************************/
static t_eReturnCode s_FMKTIM_Get_CCRxValue(t_sFMKTIM_TimerInfo * f_timInfo_ps, 
                                            t_eFMKTIM_InterruptChnl f_channel_e,
                                            t_uint32 * f_comparedValue_pu32)
{
    t_eReturnCode Ret_e = RC_OK;
    t_uint32 BspChannel_u32 = 0;

   if((f_timInfo_ps == (t_sFMKTIM_TimerInfo *)NULL)
   || (f_channel_e >= FMKTIM_CHANNEL_NB))
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
    }
    if (f_comparedValue_pu32 == (t_uint32 *)NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }
    if(Ret_e == RC_OK)
    {   
        //--------- get dutycycle only if channel is active---------//
        Ret_e = s_FMKTIM_Get_BspChannel(f_channel_e, &BspChannel_u32);

        if (Ret_e == RC_OK)
        {
            //--------- use BSP function to know dutycyle---------//
            *f_comparedValue_pu32 = (t_uint32)HAL_TIM_ReadCapturedValue(f_timInfo_ps->bspTimer_ps, BspChannel_u32);
        }
        else 
        {
            ASSERT((t_uint16)Ret_e);
            *f_comparedValue_pu32 = (t_uint32)0;
        }
    }

    return Ret_e;
}

/*********************************
 * s_FMKTIM_GetEcdrARRValue
 *********************************/
static t_eReturnCode s_FMKTIM_GetEcdrARRValue(  t_eFMKTIM_EcdrMode f_EcdrMode_e,
                                                t_uint32 f_rqstArrValue_u32,
                                                t_uint32 * f_ArrValue_pu32)
{
    t_eReturnCode Ret_e = RC_OK;

    if(f_EcdrMode_e >= FMKTIM_ECDR_MODE_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
    }
    if(f_ArrValue_pu32 == (t_uint32 *)NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }
    if(Ret_e == RC_OK)
    {
        switch(f_EcdrMode_e)
        {
            case FMKTIM_ECDR_MODE_TI1:
            case FMKTIM_ECDR_MODE_TI2:
            case FMKTIM_ECDR_MODE_CLOCKPLUS_DIRECTION_X1:
            case FMKTIM_ECDR_MODE_TI12:
            case FMKTIM_ECDR_MODE_DIRECTIONAL_CLK_X1_TI12:
            case FMKTIM_ECDR_MODE_X1_TI1:
            case FMKTIM_ECDR_MODE_X1_TI12:
            {
                *f_ArrValue_pu32 = (t_uint32)(f_rqstArrValue_u32);
                break;
            }
            case FMKTIM_ECDR_MODE_DIRECTIONAL_CLK_X2:
            case FMKTIM_ECDR_MODE_CLOCKPLUS_DIRECTION_X2:
            {
                *f_ArrValue_pu32 = (t_uint32)((t_uint32)2 * f_rqstArrValue_u32);
                break;
            }
            case FMKTIM_ECDR_MODE_NB:
            default:
            {
                Ret_e = RC_ERROR_NOT_SUPPORTED;
            }
        }
    }

    return Ret_e;
}
/*********************************
 * s_FMKTIM_Set_BspTimerInit
 *********************************/
static t_eReturnCode s_FMKTIM_Set_BspTimerInit( t_sFMKTIM_TimerInfo * f_timer_ps,
                                                t_eFMKTIM_HwTimerCfg f_hwTimCfg_e,
                                                t_uint32 f_InfoInit_u32,
                                                void * f_timerCfg_pv)

{
    t_eReturnCode Ret_e = RC_OK;
    HAL_StatusTypeDef bspRet_e = HAL_OK;
    t_uint32 bspPeriod_u32;
    t_uint32 bspPrescaler_u32;

    if (f_timer_ps == (t_sFMKTIM_TimerInfo *)NULL)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
    }
    if(Ret_e == RC_OK)
    {
        Ret_e = FMKCPU_Set_HwClock(f_timer_ps->c_clock_e, FMKCPU_CLOCKPORT_OPE_ENABLE);
    }
    if(Ret_e == RC_OK)
    {
        //----------Get the prescaler/ ARR for the timer ------------------//
        Ret_e = c_FMKTIM_TimerFunc_apf[f_hwTimCfg_e].
                    GetTimerInfoInit_pcb(f_timer_ps->c_clock_e,
                                         f_timer_ps->timerFreqMHz_u32,
                                        (t_float32)f_InfoInit_u32,
                                        &bspPeriod_u32,
                                        &bspPrescaler_u32);
    }
    if (Ret_e == RC_OK)
    {
        //---------- Set Bsp Structure ------------------//
        f_timer_ps->bspTimer_ps->Init.Prescaler         = bspPrescaler_u32;
        f_timer_ps->bspTimer_ps->Init.Period            = bspPeriod_u32;
        f_timer_ps->bspTimer_ps->Init.RepetitionCounter = (t_uint32)0;
        f_timer_ps->bspTimer_ps->Init.CounterMode       = TIM_COUNTERMODE_UP;
        f_timer_ps->bspTimer_ps->Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
        f_timer_ps->bspTimer_ps->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

        //------- Call the Right Bsp Timer Init -----//
        bspRet_e = c_FMKTIM_TimerFunc_apf[f_hwTimCfg_e].TimerInit_pcb(  f_timer_ps->bspTimer_ps,
                                                                        (void *)f_timerCfg_pv);
        if(bspRet_e != HAL_OK)
        {
            ASSERT((t_uint16)Ret_e);
            Ret_e = RC_ERROR_WRONG_RESULT;
        }
        else 
        {
            f_timer_ps->HwCfg_e = f_hwTimCfg_e;
            f_timer_ps->isConfigured_b = (t_bool)True;
        }               
    }

    return Ret_e;
}

/***********************************
 * FMKTIM_BspRqst_InterruptMngmt
 ***********************************/
static void FMKTIM_BspRqst_InterruptMngmt(TIM_HandleTypeDef *f_timerIstce_ps, t_eFMKTIM_BspEvent f_cbEvnt_e)
{
    t_eReturnCode Ret_e = RC_OK;
    t_eFMKTIM_Timer Calltimer_e = FMKTIM_TIMER_NB;
    HAL_TIM_ActiveChannel BspITChnl_e = HAL_TIM_ACTIVE_CHANNEL_CLEARED;
    t_eFMKTIM_InterruptChnl ITChnl_e = FMKTIM_CHANNEL_NB;
    t_sFMKTIM_TimerInfo timerInfo_s;
    t_sFMKTIM_ChnlInfo  chnlInfo_s;
    t_uint8 LLI_u8 = 0;

    // loop to know  which timer it is
    for (LLI_u8 = (t_uint8)0; LLI_u8 < (t_uint8)FMKTIM_TIMER_NB; LLI_u8++)
    {
        if (g_TimerInfo_as[LLI_u8].bspTimer_ps == (TIM_HandleTypeDef *)f_timerIstce_ps)
        {
            Calltimer_e = (t_eFMKTIM_Timer)LLI_u8;
            Ret_e = SMB_Read(&g_sfmb_TimerInfo_as[LLI_u8], &timerInfo_s, sizeof(t_sFMKTIM_TimerInfo));
            break;
        }
    }
    if ((Calltimer_e >= FMKTIM_TIMER_NB)
    ||  (Ret_e != RC_OK))
    {
        ASSERT((t_uint16)Calltimer_e);
        return; 
    }
    else
    {
        if(f_cbEvnt_e == FMKTIM_BSP_CB_ERROR)
        {
            timerInfo_s.errDetected_b = (t_bool)TRUE;
            FMKCPU_GetTick(&timerInfo_s.lastCbError_u32);
        }
        else 
        {
            switch(timerInfo_s.HwCfg_e)
            {  
                case FMKTIM_HWTIM_CFG_PWM:
                {
                    //----- Pulse finished -----//
                    if(f_cbEvnt_e == FMKTIM_BSP_CB_PERIOD_ELAPSED)
                    {
                        if(timerInfo_s.ignoreInterrupt_b == (t_bool)True)
                        {
                            //----- Update Flag -----//
                            timerInfo_s.ignoreInterrupt_b = (t_bool)False;
                            //__HAL_TIM_ENABLE_IT
                        }
                        else 
                        {
                            //----- Reset PWM ON & call user-----//
                            for(LLI_u8 = (t_uint8)0 ; (LLI_u8 < FMKTIM_CHANNEL_NB) && (Ret_e == RC_OK) ; LLI_u8++)
                            {
                                if(g_TimChnlInfo_as[Calltimer_e][LLI_u8].State_e == FMKTIM_CHNLST_ACTIVATED)
                                {
                                    Ret_e = SMB_Read(   &g_sfmb_TimChnlInfo_as[Calltimer_e][LLI_u8], 
                                                        &chnlInfo_s, 
                                                        sizeof(t_sFMKTIM_ChnlInfo));
                                    if(Ret_e == RC_OK)
                                    {
                                        Ret_e = s_FMKTIM_Set_HwChannelState(&timerInfo_s, 
                                                                            &chnlInfo_s,
                                                                            chnlInfo_s.RunMode_e,
                                                                            timerInfo_s.HwCfg_e,
                                                                            FMKTIM_CHNLST_DISACTIVATED);
                                    }

                                    if((Ret_e == RC_OK) 
                                    && (chnlInfo_s.chnl_cb != NULL_FUNCTION))
                                    {
                                        chnlInfo_s.chnl_cb( c_FmkTim_ChnlItLineMapp[Calltimer_e][LLI_u8].type_e,
                                                            c_FmkTim_ChnlItLineMapp[Calltimer_e][LLI_u8].ITLine_u8);
                                    }
                                    if(Ret_e == RC_OK)
                                    {
                                        Ret_e = SMB_Write(&g_sfmb_TimChnlInfo_as[Calltimer_e][LLI_u8], &chnlInfo_s, sizeof(t_sFMKTIM_ChnlInfo));
                                    }
                                }
                            }
                            //g_timerPeriodPwm_ab[Calltimer_e] = (t_bool)False;
                            Ret_e = s_FMKTIM_Set_HwChannelState(&timerInfo_s, 
                                                                &chnlInfo_s,
                                                                FMKTIM_LINE_RUNMODE_INTERRUPT,
                                                                FMKTIM_HWTIM_CFG_EVNT,
                                                                FMKTIM_CHNLST_DISACTIVATED);

                            if(Ret_e < RC_OK)
                            {
                                ASSERT((t_uint16)Ret_e);
                            }                        
                        }
                    }
                    break;
                }
                case FMKTIM_HWTIM_CFG_EVNT:
                {
                    //--- check directly the container info 
                    //      by pass the SBM API ----//
                    if(g_TimChnlInfo_as[Calltimer_e][FMKTIM_CHANNEL_1].chnl_cb != NULL_FUNCTION)
                    {
                        g_TimChnlInfo_as[Calltimer_e][FMKTIM_CHANNEL_1].
                            chnl_cb(c_FmkTim_ChnlItLineMapp[Calltimer_e][FMKTIM_CHANNEL_1].type_e,
                                    c_FmkTim_ChnlItLineMapp[Calltimer_e][FMKTIM_CHANNEL_1].ITLine_u8);
                    }
                    break;
                }
                case FMKTIM_HWTIM_CFG_IC:
                {
                    //------------Find Bsp Channel which triggered the interruption------------//
                    BspITChnl_e = HAL_TIM_GetActiveChannel(timerInfo_s.bspTimer_ps);
                    switch (BspITChnl_e)
                    {
                        case HAL_TIM_ACTIVE_CHANNEL_1:
                            ITChnl_e = FMKTIM_CHANNEL_1;
                            break;

                        case HAL_TIM_ACTIVE_CHANNEL_2:
                            ITChnl_e = FMKTIM_CHANNEL_2;
                            break;

                        case HAL_TIM_ACTIVE_CHANNEL_3:
                            ITChnl_e = FMKTIM_CHANNEL_3;
                            break;

                        case HAL_TIM_ACTIVE_CHANNEL_4:
                            ITChnl_e = FMKTIM_CHANNEL_4;
                            break;
    #ifdef FMKTIM_STM32_ECU_FAMILY_G
                        case HAL_TIM_ACTIVE_CHANNEL_5:
                            ITChnl_e = FMKTIM_CHANNEL_5;
                            break;

                        case HAL_TIM_ACTIVE_CHANNEL_6:
                            ITChnl_e = FMKTIM_CHANNEL_6;
                            break;
    #endif              
                        case HAL_TIM_ACTIVE_CHANNEL_CLEARED:
                        default:
                            Ret_e = RC_ERROR_NOT_SUPPORTED;
                            break;
                    }
                    //--- check directly the container info 
                    //      by pass the SBM API ----//
                    if( (Ret_e == RC_OK) 
                    && (g_TimChnlInfo_as[Calltimer_e][ITChnl_e].chnl_cb != NULL_FUNCTION))
                    {
                        g_TimChnlInfo_as[Calltimer_e][ITChnl_e].
                            chnl_cb(c_FmkTim_ChnlItLineMapp[Calltimer_e][ITChnl_e].type_e,
                                    c_FmkTim_ChnlItLineMapp[Calltimer_e][ITChnl_e].ITLine_u8);
                    }
                    break;
                }
                case FMKTIM_HWTIM_CFG_OC:
                case FMKTIM_HWTIM_CFG_OP:
                case FMKTIM_HWTIM_CFG_ECDR:
                case FMKTIM_HWTIM_CFG_DAC:
                case FMKTIM_HWTIM_CFG_NB:
                default:
                {
                    Ret_e = RC_WARNING_NO_OPERATION;
                }
            }
        }
        if(Ret_e != RC_OK)
        {
            ASSERT((t_uint16)Ret_e);
        }
        else 
        {
            Ret_e = SMB_Write(&g_sfmb_TimerInfo_as[Calltimer_e], &timerInfo_s, sizeof(t_sFMKTIM_TimerInfo));
        }
    }

    return;
}

/*********************************
 * s_FMKTIM_Get_BspChannel
 *********************************/
static t_eReturnCode s_FMKTIM_Get_BspChannel(t_eFMKTIM_InterruptChnl f_channel_e, t_uint32 *f_bspChnl_pu32)
{
    t_eReturnCode Ret_e = RC_OK;

    if (f_bspChnl_pu32 == (t_uint32 *)NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }
    if ((f_channel_e >= FMKTIM_CHANNEL_NB)
    &&  (f_channel_e != FMKTIM_CHANNEL_ALL))
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
    }
    if (Ret_e == RC_OK)
    {
        switch (f_channel_e)
        {
        case FMKTIM_CHANNEL_1:
            *f_bspChnl_pu32 = (t_uint32)TIM_CHANNEL_1;
            break;
        case FMKTIM_CHANNEL_2:
            *f_bspChnl_pu32 = (t_uint32)TIM_CHANNEL_2;
            break;
        case FMKTIM_CHANNEL_3:
            *f_bspChnl_pu32 = (t_uint32)TIM_CHANNEL_3;
            break;
        case FMKTIM_CHANNEL_4:
            *f_bspChnl_pu32 = (t_uint32)TIM_CHANNEL_4;
            break;
#ifdef FMKTIM_STM32_ECU_FAMILY_G
        case FMKTIM_CHANNEL_5:
            *f_bspChnl_pu32 = (t_uint32)TIM_CHANNEL_5;
            break;
        case FMKTIM_CHANNEL_6:
            *f_bspChnl_pu32 = (t_uint32)TIM_CHANNEL_6;
            break;
#endif
        case FMKTIM_CHANNEL_ALL:
            *f_bspChnl_pu32 = (t_uint32)TIM_CHANNEL_ALL;
            break;
        case FMKTIM_CHANNEL_NB:
        default:
            Ret_e = RC_ERROR_PARAM_NOT_SUPPORTED;
            break;
        }
    }
    return Ret_e;
}

/*********************************
 * s_FMKTIM_Get_BspTimer
 *********************************/
static t_eReturnCode s_FMKTIM_Get_BspICTrigger(t_eFMKTIM_ChnlMeasTrigger f_MeasTrigger_e, t_uint32 *f_bspMeasTrigger_u32)
{
    t_eReturnCode Ret_e = RC_OK;

    if (f_bspMeasTrigger_u32 == (t_uint32 *)NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }
    if (f_MeasTrigger_e >= FMKTIM_LINE_MEAS_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
    }
    if (Ret_e == RC_OK)
    {
        switch (f_MeasTrigger_e)
        {
        case FMKTIM_LINE_MEAS_FALLING_EDGE:
            *f_bspMeasTrigger_u32 = (t_uint32)TIM_ICPOLARITY_FALLING;
            break;
        case FMKTIM_LINE_MEAS_RISING_EDGE:
            *f_bspMeasTrigger_u32 = (t_uint32)TIM_ICPOLARITY_RISING;
            break;
        case FMKTIM_LINE_MEAS_BOTH_EDGE:
            *f_bspMeasTrigger_u32 = (t_uint32)TIM_ICPOLARITY_BOTHEDGE;
            break;
        case FMKTIM_LINE_MEAS_NB:
        default:
            Ret_e = RC_ERROR_PARAM_NOT_SUPPORTED;
            break;
        }
    }
    return Ret_e;
}

/*********************************
 * s_FMKTIM_Get_BspTimer
 *********************************/
static t_eReturnCode s_FMKTIM_GetBspEcdrPolarity(t_eFMKTIM_EcdrInPolarity f_ICEcdrPolarity_e, t_uint32 *f_bspICEcdrPolarity_pu32)
{
    t_eReturnCode Ret_e = RC_OK;

    if (f_bspICEcdrPolarity_pu32 == (t_uint32 *)NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }
    if (f_ICEcdrPolarity_e >= FMKTIM_ECDR_IN_POLARITY_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
    }
    if (Ret_e == RC_OK)
    {
        switch (f_ICEcdrPolarity_e)
        {
            case FMKTIM_ECDR_IN_POLARITY_RISING:
                *f_bspICEcdrPolarity_pu32 = (t_uint32)TIM_ENCODERINPUTPOLARITY_RISING;
                break;
            case FMKTIM_ECDR_IN_POLARITY_FALLING:
                *f_bspICEcdrPolarity_pu32 = (t_uint32)TIM_ENCODERINPUTPOLARITY_FALLING;
                break;
            case FMKTIM_ECDR_IN_POLARITY_NB:
            default:
                Ret_e = RC_ERROR_PARAM_NOT_SUPPORTED;
                break;
        }
    }
    return Ret_e;
}

/*********************************
 * s_FMKTIM_Get_BspTimer
 *********************************/
static t_eReturnCode s_FMKTIM_GetBspEcdrMode(t_eFMKTIM_EcdrMode f_EcdrMode_e, t_uint32 *f_bspEcdrMode_pu32)
{
    t_eReturnCode Ret_e = RC_OK;

    if (f_bspEcdrMode_pu32 == (t_uint32 *)NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }
    if (f_EcdrMode_e >= FMKTIM_ECDR_MODE_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
    }
    if (Ret_e == RC_OK)
    {
        switch (f_EcdrMode_e)
        {
            case FMKTIM_ECDR_MODE_TI1:
                *f_bspEcdrMode_pu32 = (t_uint32)TIM_ENCODERMODE_TI1;
                break;
            case FMKTIM_ECDR_MODE_TI2:
                *f_bspEcdrMode_pu32 = (t_uint32)TIM_ENCODERMODE_TI2;
                break;
            case FMKTIM_ECDR_MODE_TI12:
                *f_bspEcdrMode_pu32 = (t_uint32)TIM_ENCODERMODE_TI12;
                break;
            case FMKTIM_ECDR_MODE_CLOCKPLUS_DIRECTION_X2:
                *f_bspEcdrMode_pu32 = (t_uint32)TIM_ENCODERMODE_CLOCKPLUSDIRECTION_X2;
                break;
            case FMKTIM_ECDR_MODE_CLOCKPLUS_DIRECTION_X1:
                *f_bspEcdrMode_pu32 = (t_uint32)TIM_ENCODERMODE_CLOCKPLUSDIRECTION_X1;
                break;
            case FMKTIM_ECDR_MODE_DIRECTIONAL_CLK_X2:
                *f_bspEcdrMode_pu32 = (t_uint32)TIM_ENCODERMODE_DIRECTIONALCLOCK_X2;
                break;
            case FMKTIM_ECDR_MODE_DIRECTIONAL_CLK_X1_TI12:
                *f_bspEcdrMode_pu32 = (t_uint32)TIM_ENCODERMODE_DIRECTIONALCLOCK_X1_TI12;
                break;
            case FMKTIM_ECDR_MODE_X1_TI1:
                *f_bspEcdrMode_pu32 = (t_uint32)TIM_ENCODERMODE_X1_TI1;
                break;
            case FMKTIM_ECDR_MODE_X1_TI12:
                *f_bspEcdrMode_pu32 = (t_uint32)TIM_ENCODERMODE_X1_TI2;
                break;
            case FMKTIM_ECDR_MODE_NB:
            default:
                Ret_e = RC_ERROR_NOT_SUPPORTED;   
        }
    }
    return Ret_e;
}

/*********************************
 * s_FMKTIM_Get_BspTimer
 *********************************/
static t_eReturnCode s_FMKTIM_GetBspEcdrSelection(t_eFMKTIM_ICSelection f_ICEcdrSelect_e, t_uint32 *f_bspICEcdrSelect_pu32)
{
    t_eReturnCode Ret_e = RC_OK;

    if (f_bspICEcdrSelect_pu32 == (t_uint32 *)NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }
    if (f_ICEcdrSelect_e >= FMKTIM_ICSELECT_NB)
    {
        Ret_e = RC_ERROR_PARAM_INVALID;
    }
    if (Ret_e == RC_OK)
    {
        switch (f_ICEcdrSelect_e)
        {
            case FMKTIM_ICSELECT_INDIRECT_TI:
                *f_bspICEcdrSelect_pu32 = (t_uint32)TIM_ICSELECTION_DIRECTTI;
                break;
            case FMKTIM_ICSELECT_DIRECT_TI:
                *f_bspICEcdrSelect_pu32 = (t_uint32)TIM_ICSELECTION_INDIRECTTI;
                break;
            case FMKTIM_ICSELECT_TRC:
                *f_bspICEcdrSelect_pu32 = (t_uint32)TIM_ICSELECTION_TRC;
                break;
            case FMKTIM_ICSELECT_NB:
            default:
                Ret_e = RC_ERROR_PARAM_NOT_SUPPORTED;
                break;
        }
    }
    return Ret_e;
}

/*********************************
 * s_FMKTIM_UpdateTimerFrequency
 *********************************/
static t_eReturnCode s_FMKTIM_UpdateTimerFrequency( t_sFMKTIM_TimerInfo * f_timerInfo_ps, 
                                                    t_eFMKTIM_InterruptChnl f_chnl_e,
                                                    t_uint32 f_timFreq_u32)
{
    t_eReturnCode Ret_e = RC_OK;
    t_uint32 bspARRVal_u32 = (t_uint32)0;
    t_uint32 bspPSCVal_u32 = (t_uint32)0;
    t_uint32 bspChnl_u32   = (t_uint32)0;

    if(f_timerInfo_ps == (t_sFMKTIM_TimerInfo *)NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }
    if(Ret_e == RC_OK)
    { 
        Ret_e = c_FMKTIM_TimerFunc_apf[FMKTIM_HWTIM_CFG_PWM].
                        GetTimerInfoInit_pcb(   f_timerInfo_ps->c_clock_e,
                                                f_timerInfo_ps->timerFreqMHz_u32,
                                                (t_float32)(f_timFreq_u32),
                                                &bspARRVal_u32,
                                                &bspPSCVal_u32);

        if (Ret_e == RC_OK)
        {
            Ret_e = s_FMKTIM_Get_BspChannel(f_chnl_e, &bspChnl_u32);
        }
        if(Ret_e == RC_OK)
        {        
            //----- Stop timer to avoid glitches -----//
            //bspIsct_ps->CR1 &= ~TIM_CR1_CEN;

            //----- Activate Shadow Register for ARR & PSC -----//
            f_timerInfo_ps->bspTimer_ps->Instance->CR1 |= TIM_CR1_ARPE;

            //----- Update PWM channel settings if necessary -----//
            switch(f_chnl_e)
            {
                case FMKTIM_CHANNEL_1: 
                    f_timerInfo_ps->bspTimer_ps->Instance->CCMR1 |= TIM_CCMR1_OC1CE; 
                    break;
                case FMKTIM_CHANNEL_2: 
                    f_timerInfo_ps->bspTimer_ps->Instance->CCMR1 |= TIM_CCMR1_OC2CE; 
                    break;
                case FMKTIM_CHANNEL_3: 
                    f_timerInfo_ps->bspTimer_ps->Instance->CR1 |= TIM_CR1_ARPE;
                    f_timerInfo_ps->bspTimer_ps->Instance->CCMR2 |= TIM_CCMR2_OC3CE; 
                    break;
                case FMKTIM_CHANNEL_4: 
                    f_timerInfo_ps->bspTimer_ps->Instance->CCMR2 |= TIM_CCMR2_OC4CE; 
                    break;
                case FMKTIM_CHANNEL_NB:
                default:
                    Ret_e = RC_ERROR_NOT_ALLOWED;
                    break;
            }
            if(Ret_e == RC_OK)
            {
                //----- Update ARR and PSC for next cycle (shadow registers) -----//
                f_timerInfo_ps->bspTimer_ps->Instance->ARR = (t_uint32)bspARRVal_u32;
                f_timerInfo_ps->bspTimer_ps->Instance->PSC = (t_uint32)bspPSCVal_u32;
            }
        }
        if(Ret_e != RC_OK)
        {
            ASSERT((t_uint16)Ret_e);
        }
    }
    return Ret_e;
}

/*********************************
 * s_FMKTIM_Get_TimChnlFromITLine
 *********************************/
static t_eReturnCode s_FMKTIM_UpdateDutyCycle(  t_sFMKTIM_TimerInfo * f_timerInfo_ps, 
                                                t_eFMKTIM_InterruptChnl f_chnl_e,
                                                t_uint32 f_dutyCycle_u32)
{
    t_eReturnCode Ret_e = RC_OK;
    t_uint32 CCRxValue_u32 = (t_uint32)0;
    t_uint32 bspChannel_u32 = (t_uint32)0;

    if(f_timerInfo_ps == (t_sFMKTIM_TimerInfo *)NULL)
    {
        Ret_e = RC_ERROR_PTR_NULL;
    }
    if(Ret_e == RC_OK)
    {
        Ret_e = s_FMKTIM_Get_BspChannel(f_chnl_e, &bspChannel_u32);

        CCRxValue_u32 = (t_uint32)((t_float32)(f_dutyCycle_u32) / (t_float32)FMKTIM_PWM_MAX_DUTY_CYLCE *
                    (t_float32)(f_timerInfo_ps->bspTimer_ps->Instance->ARR + (t_uint32)1));

        __HAL_TIM_SET_COMPARE(f_timerInfo_ps->bspTimer_ps, bspChannel_u32, (t_uint32)CCRxValue_u32);
    }

    return Ret_e;
}

/*********************************
 * s_FMKTIM_Get_TimChnlFromITLine
 *********************************/
static t_eReturnCode s_FMKTIM_Get_TimChnlFromITLine(t_eFMKTIM_InterruptLineType f_ITLineType_e,
                                                    t_uint32                    f_ITLine_u32,    
                                                    t_eFMKTIM_Timer             *f_timer_pe,
                                                    t_eFMKTIM_InterruptChnl     *f_channel_pe)
{
    t_eReturnCode Ret_e = RC_OK;

    switch(f_ITLineType_e)
    {
        case FMKTIM_INTERRUPT_LINE_TYPE_IO:
        {
            if(f_ITLine_u32 >= FMKTIM_INTERRUPT_LINE_IO_NB)
            {
                Ret_e = RC_ERROR_PARAM_INVALID;
            }
            else 
            {
                *f_timer_pe = c_FmkTim_ITLineIOMapp_as[(t_eFMKTIM_InterruptLineIO)f_ITLine_u32].timer_e;
                if(g_TimerInfo_as[(*f_timer_pe)].HwCfg_e == FMKTIM_HWTIM_CFG_ECDR)
                {
                    *f_channel_pe = FMKTIM_CHANNEL_ALL;
                }
                else 
                {
                    *f_channel_pe = c_FmkTim_ITLineIOMapp_as[(t_eFMKTIM_InterruptLineIO)f_ITLine_u32].channel_e;
                }
            }
            break;
        }
        case FMKTIM_INTERRUPT_LINE_TYPE_EVNT:
        {
            if(f_ITLine_u32 >= FMKTIM_INTERRUPT_LINE_EVNT_NB)
            {
                Ret_e = RC_ERROR_PARAM_INVALID;
            }
            else 
            {
                *f_timer_pe = c_FmkTim_ITLineEvntMapp_as[(t_eFMKTIM_InterruptLineEvnt)f_ITLine_u32].timer_e;
                *f_channel_pe = c_FmkTim_ITLineEvntMapp_as[(t_eFMKTIM_InterruptLineEvnt)f_ITLine_u32].channel_e;
            }
            
            break;
        }
        case FMKTIM_INTERRUPT_LINE_TYPE_DAC:
        {
            if(f_ITLine_u32 >= FMKTIM_INTERRUPT_LINE_EVNT_NB)
            {
                Ret_e = RC_ERROR_PARAM_INVALID;
            }
            else 
            {
                *f_timer_pe = c_FmkTim_ITLineDacMapp_as[(t_eFMKTIM_InterruptLineDAC)f_ITLine_u32].timer_e;
                *f_channel_pe = c_FmkTim_ITLineDacMapp_as[(t_eFMKTIM_InterruptLineDAC)f_ITLine_u32].channel_e;
            }
            
            break;
        }
        case FMKTIM_INTERRUPT_LINE_TYPE_NB:
        default:
            Ret_e = RC_ERROR_NOT_SUPPORTED;
            break;
    }
    if(((*f_timer_pe) >= FMKTIM_TIMER_NB)
    || (((*f_channel_pe) >= FMKTIM_CHANNEL_NB)
    && (*f_channel_pe) != FMKTIM_CHANNEL_ALL))
    {
        Ret_e = RC_ERROR_LIMIT_REACHED;
        ASSERT((t_uint16)Ret_e);
    }
    
    return Ret_e;
}

/**
 *
 *	@brief      Every callback function is now centralized in one function
 *
 */
void HAL_TIM_ErrorCallback(TIM_HandleTypeDef *htim) { return FMKTIM_BspRqst_InterruptMngmt(htim, FMKTIM_BSP_CB_PERIOD_ELAPSED); }
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) { return FMKTIM_BspRqst_InterruptMngmt(htim, FMKTIM_BSP_CB_PERIOD_ELAPSED); }
//void HAL_TIM_PeriodElapsedHalfCpltCallback(TIM_HandleTypeDef *htim) { return FMKTIM_BspRqst_InterruptMngmt(htim); }
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) { return FMKTIM_BspRqst_InterruptMngmt(htim, FMKTIM_BSP_CB_IC_CAPTURE); }
//void HAL_TIM_IC_CaptureHalfCpltCallback(TIM_HandleTypeDef *htim) { return FMKTIM_BspRqst_InterruptMngmt(htim); }
//void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim) { return FMKTIM_BspRqst_InterruptMngmt(htim, FMKTIM_BSP_CB_OC_DELAY_ELAPSED); }
//void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) { return FMKTIM_BspRqst_InterruptMngmt(htim, FMKTIM_BSP_CB_PWM_PULSE_FINISHED); }
//void HAL_TIM_PWM_PulseFinishedHalfCpltCallback(TIM_HandleTypeDef *htim) { return FMKTIM_BspRqst_InterruptMngmt(htim); }
//void HAL_TIM_TriggerCallback(TIM_HandleTypeDef *htim) { return FMKTIM_BspRqst_InterruptMngmt(htim, FMKTIM_HWTIM_CFG_TRGR); }
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
