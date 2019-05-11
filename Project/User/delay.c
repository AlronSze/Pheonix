#include "stm32f4xx.h"
#include "delay.h"
#include "ucos_ii.h"

static u8  fac_us = 0;
static u16 fac_ms = 0;

// ��ʱ���ã�SYSCLKΪ��ǰʱ��Ƶ�ʣ�MHZ��
void Delay_Config(u8 SYSCLK)
{
// ��������ʹ��ucosII
#ifdef OS_CRITICAL_METHOD
    u32 reload;
#endif

    // ϵͳ��ʱ�����ã��˷�Ƶ
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
    // ��ȡusֵ
    fac_us = SYSCLK / 8;

// ��������ʹ��ucosII
#ifdef OS_CRITICAL_METHOD
    // ÿ�����������K��
    reload = SYSCLK / 8;
    // �趨���ʱ��
    reload *= 1000000 / OS_TICKS_PER_SEC;
    // ucosII����ʱ����С��λ
    fac_ms = 1000 / OS_TICKS_PER_SEC;
    // ����Systick�ж�
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
    // ÿ(1 / OS_TICKS_PER_SEC)���ж�һ��
    SysTick->LOAD  = reload;
    // ����Systick
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
#else
    // ��ȡ��ucosII�µ�msֵ
    fac_ms = (u16)fac_us * 1000;
#endif
}

// ��������ʹ��ucosII
#ifdef OS_CRITICAL_METHOD
// ��ʱus��nusΪҪ��ʱ��us��
void delay_us(u32 nus)
{
    u32 ticks;
    u32 told, tnow, tcnt;
    
    // װ��ֵ
    u32 reload = SysTick->LOAD;
    // ������
    ticks = nus * fac_us;
    tcnt = 0;
    // ����ϵͳ��������
    OSSchedLock();
    // ��ǰSysTickֵ
    told = SysTick->VAL;
    
    while(1)
    {
        // ��ǰSysTickֵ
        tnow = SysTick->VAL;
        if(tnow != told)
        {
            // �ݼ�����
            if(tnow < told)
            {
                tcnt += told - tnow;
            }
            else
            {
                // ��ʱ���ǹ�һ�֣�����reload
                tcnt += reload - tnow + told;
            }
            told = tnow;
            // ��ʱ
            if(tcnt >= ticks)
            {
                break;
            }
        }
    };

    // ����ϵͳ���Ƚ���
    OSSchedUnlock();
}

// ��ʱms��nmsΪҪ��ʱ��ms��
void delay_ms(u16 nms)
{
    // ������ϵͳ��������
    if(OSRunning == OS_TRUE && OSLockNesting == 0)
    {
        // ��ʱ��ʱ����ڲ���ϵͳ��С��λ
        if(nms >= fac_ms)
        {
            // ����ϵͳ��ʱ
            OSTimeDly(nms / fac_ms);
        }
        // ��ʱ��ʱ����ڲ���ϵͳ��С��λ��������ͨ������ʱ
        nms %= fac_ms;
    }
    // ��ͨ������ʱ
    delay_us((u32)(nms * 1000));
}

// ��ʹ��ucosII
#else
// ��ʱus��nusΪҪ��ʱ��us��
void delay_us(u32 nus)
{
    u32 temp;
    // ʱ�����
    SysTick->LOAD  = nus * fac_us;
    // ��ռ�ʱ��
    SysTick->VAL   = 0x00;
    // ����Systick
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

    // ��ʼ��ʱֱ��ʱ�䵽��
    do
    {
        temp = SysTick->CTRL;
    }
    while((temp & 0x01) && !(temp & (1<<16)));

    // �ر�Systick
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    // ��ռ�ʱ��
    SysTick->VAL   = 0X00; 
}

// ��ʱms��nmsΪҪ��ʱ��ms��
void delay_ms(u16 nms)
{
    u32 temp;

    // ʱ�����
    SysTick->LOAD  = (u32)nms * fac_ms;
    // ��ռ�����
    SysTick->VAL   = 0x00;
    // ����Systick
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

    // ��ʼ��ʱֱ��ʱ�䵽��
    do
    {
        temp = SysTick->CTRL;
    }
    while((temp & 0x01) && !(temp & (1<<16)));

    // �ر�Systick
    SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;
    // ��ռ�ʱ��
    SysTick->VAL =0X00;
}
#endif
