#include "stm32f4xx_it.h"
#include "COM.h"
#include "PID.h"
#include "USART.h"
#include "delay.h"

// ��λ������������
uint8_t ComRxBuffer[4] = {0};

// ��ȡ��λ������
void Get_COM(void)
{
	// ����֡��ʽ����
	if (ComRxBuffer[0] == 0x8A && ComRxBuffer[1] == 0xFE && ComRxBuffer[3] == 0xFC)
	{
		// ����ʶ��ʾ�����������PID���ͨ�Ź����ݲ�ʹ�ã��ȴ�δ������
		if (ComRxBuffer[2] == 0x01)
		{
			// �˴����빦��
		}
		else if (ComRxBuffer[2] == 0x02)
		{
			// �˴����빦��
		}
		
		// �������
		ComRxBuffer[0] = 0;
		ComRxBuffer[1] = 0;
		ComRxBuffer[2] = 0;
		ComRxBuffer[3] = 0;
	}
}
