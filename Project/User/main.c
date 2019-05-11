// ͷ�ļ�
#include <includes.h>

// ��ջ����
static OS_STK Task_Startup_STK[TASK_STARTUP_STK_SIZE];
static OS_STK Task_Angel_STK[TASK_ANGEL_STK_SIZE];
static OS_STK Task_PID_STK[TASK_PID_STK_SIZE];
static OS_STK Task_COM_STK[TASK_COM_STK_SIZE];

// ��������
static void OS_Systick_Init(void);
static void Task_Startup(void *p_arg);
static void Task_Angel(void *p_arg);
static void Task_PID(void *p_arg);
static void Task_COM(void *p_arg);

// ������
int main(void)
{
    // ����ϵͳ��ʼ��
    OSInit();
    // ������������
    OSTaskCreate(Task_Startup, (void *)0, &Task_Startup_STK[TASK_STARTUP_STK_SIZE - 1], TASK_STARTUP_PRIO);
    // ���в���ϵͳ
    OSStart();
    
    return 0;
}

// ����ϵͳSystick��ʼ��
static void OS_Systick_Init(void)
{
    RCC_ClocksTypeDef rcc_clocks;
    RCC_GetClocksFreq(&rcc_clocks);
    SysTick_Config(rcc_clocks.HCLK_Frequency / OS_TICKS_PER_SEC);
}

// ����������
static void Task_Startup(void *p_arg)
{
    // �弶������ʼ��
    BSP_Init();
    // ����ϵͳSystick��ʼ��
    OS_Systick_Init();

    // ���CPU��ǰ����
    #if (OS_TASK_STAT_EN > 0)
        OSStatInit();
    #endif

    // ������������
    OSTaskCreate(Task_Angel, (void *)0, &Task_Angel_STK[TASK_ANGEL_STK_SIZE - 1], TASK_ANGEL_PRIO);
    OSTaskCreate(Task_PID, (void *)0, &Task_PID_STK[TASK_PID_STK_SIZE - 1], TASK_PID_PRIO);
    OSTaskCreate(Task_COM, (void *)0, &Task_COM_STK[TASK_COM_STK_SIZE - 1], TASK_COM_PRIO);

    // ɾ������
    OSTaskDel(OS_PRIO_SELF);
}

// ��̬��������
static void Task_Angel(void *p_arg)
{
    while(1)
    {
        // ��ȡ����������
        Get_AHRS_Data();
        // ��̬����
        AHRS_Update(init_gx, init_gy, init_gz, init_ax, init_ay, init_az, init_mx, init_my, init_mz);
        // ��ʱ
        OSTimeDly(1);
    }
}

// PID��������
static void Task_PID(void *p_arg)
{
    while(1)
    {
        // ң��ֵ����
        Motor_Expectation_Calculate(PWMInCh1, PWMInCh2, PWMInCh3, PWMInCh4);
        // ���PID����
        Motor_Calculate();
        // ������Ƶ��
        PWM_Output(Motor_1, Motor_2, Motor_3, Motor_4);
        // ��ʱ
        OSTimeDly(5);
    }
}

// ����/����ͨ������
static void Task_COM(void *p_arg)
{
    while(1)
    {
        // ��ȡ��λ������
        Get_COM();
        // ��ʱ
        OSTimeDly(20);
    }
}
