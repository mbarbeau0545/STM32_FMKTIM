CST_MHZ_TO_HZ = 1_000_000
FMKTIM_TIMER_PWM_ARR_TARGET_16_BIT = 65535
FMKTIM_TIMER_PWM_ARR_TARGET_32_BIT = 4294967295
FMKTIM_MAX_LOOP_DECREASING = 10
FMKTIM_ARR_DECREASING_VAL_16B = 100
FMKTIM_FREQ_COMPUTE_DELTA_ACCEPTANCE = 0.5 # Hz, à ajuster
CST_MAX_UINT_16BIT = 0xFFFF
CST_MAX_UINT_32BIT = 0xFFFFFFFF

# Fréquence timer en MHz
timerFreqMHz = 240  # Exemple : 72 MHz

MAX_ARR = 50000
# Utilitaires
def get_pwm_param(pwm_freq):
    def get_arr_register(timer_freq_mhz, prescaler, pwm_freq):
        timer_clk_hz = timer_freq_mhz * CST_MHZ_TO_HZ
        return (timer_clk_hz / (prescaler + 1)) / pwm_freq - 1

    def get_prescaler(timer_freq_mhz, arr, pwm_freq):
        timer_clk_hz = timer_freq_mhz * CST_MHZ_TO_HZ
        return (timer_clk_hz / ((arr + 1) * pwm_freq)) - 1

    def get_pwm_freq(timer_freq_mhz, prescaler, arr):
        timer_clk_hz = timer_freq_mhz * CST_MHZ_TO_HZ
        return timer_clk_hz / ((prescaler + 1) * (arr + 1))

    def decompose_float(value):
        integer_part = int(value)
        delta = value - integer_part
        return integer_part, delta
   
    ratio = (timerFreqMHz * CST_MHZ_TO_HZ) / float(pwm_freq)
    decreasing_value = FMKTIM_ARR_DECREASING_VAL_16B
    loop_cnt = 0
    target_arr = MAX_ARR
    psc_theo = (ratio / target_arr) - 1

    while psc_theo > CST_MAX_UINT_16BIT:
        loop_cnt += 1
        target_arr -= decreasing_value
        psc_theo = (ratio / target_arr) - 1


    real_psc, delta_psc = decompose_float(psc_theo)

    if real_psc != 0:
        delta_arr = round((target_arr * (delta_psc + 1) - real_psc) / real_psc)
        real_arr = int(target_arr + delta_arr)
        while real_arr >= CST_MAX_UINT_16BIT:
            target_arr -= 18000
            psc_theo = (ratio / target_arr) - 1
            real_psc, delta_psc = decompose_float(psc_theo)
            loop_cnt += 1
            delta_arr = round((target_arr * (delta_psc + 1) - real_psc) / real_psc)
            real_arr = int(target_arr + delta_arr)
            real_freq = get_pwm_freq(timerFreqMHz, (real_psc - 1), real_arr)
        real_freq = get_pwm_freq(timerFreqMHz, (real_psc - 1), real_arr)
   
    else:
        """delta_arr = target_arr * (delta_psc + 1)
        real_arr = int(target_arr + delta_arr)"""
        real_arr = ((timerFreqMHz * CST_MHZ_TO_HZ) / pwm_freq) - 1
        
        while real_arr >= CST_MAX_UINT_16BIT:
            loop_cnt += 1
            real_psc += 1
            real_arr = ((timerFreqMHz * CST_MHZ_TO_HZ) / (pwm_freq * (real_psc + 1))) - 1
        delta_arr = 0
        real_freq = get_pwm_freq(timerFreqMHz, real_psc, int(real_arr))

    delta_freq = abs(pwm_freq - real_freq)
    ratio_freq = delta_freq / pwm_freq

    if delta_freq > FMKTIM_FREQ_COMPUTE_DELTA_ACCEPTANCE or real_arr >= CST_MAX_UINT_16BIT or real_psc >= CST_MAX_UINT_16BIT:
        pass#print(f'for {pwm_freq}, compute {real_freq} with ARR {real_arr} PSC {real_psc} with delta {delta_psc}')
        #raise Exception('') 

    return real_psc, real_arr, loop_cnt, ratio_freq


print(get_pwm_param(8666.66))