#include "MPU6050.h"
#include "stdio.h"
#include "stm32f4xx.h"
#include "I2C.h"
#include "USART.h"
#include "math.h"

// ������ת�Ƕ�������
#define ARC_TO_DEG 57.295780f

// ���ᴫ������ֵ
float init_ax, init_ay, init_az,
			init_gx, init_gy, init_gz,
			init_mx, init_my, init_mz;

// ���ٶ���Ưƫ����
float Gyro_Xout_Offset, Gyro_Yout_Offset, Gyro_Zout_Offset;
// ���ٶ���Ưƫ����
float Accel_Xout_Offset, Accel_Yout_Offset, Accel_Zout_Offset;

// ��ȡ���ٶ���Ưƫ����
void Get_Gyro_Bias(void)
{
	uint16_t i;
	int16_t gyro[3];
	int32_t gyro_x = 0, gyro_y = 0, gyro_z = 0;
	static int16_t count = 0;
	uint8_t data_write[6];

	// ȡ2000������
	for(i = 0; i < 2000; i++)
	{
		if(!i2cread(MPU6050_Addr2, GYRO_XOUT_H, 6, data_write))
		{
			// ��ȡ��������
			gyro[0] = ((((int16_t)data_write[0])<<8) | data_write[1]);
			gyro[1] = ((((int16_t)data_write[2])<<8) | data_write[3]);
			gyro[2] = ((((int16_t)data_write[4])<<8) | data_write[5]);
			gyro_x += gyro[0];
			gyro_y += gyro[1];
			gyro_z += gyro[2];
			// ��¼��Ч����
			count++;
		}
	}
	
	// ��ƽ��ֵ�õ���Ưƫ����
	Gyro_Xout_Offset = (float)gyro_x / count;
	Gyro_Yout_Offset = (float)gyro_y / count;
	Gyro_Zout_Offset = (float)gyro_z / count;
}

// ��ȡ���ٶ���Ưƫ����
void Get_Accel_Bias(void)
{
	uint32_t i;
	int16_t accel[3]; 
	uint8_t data_write[6];
	float accel_x = 0, accel_y = 0, accel_z = 0;
	static int16_t count2 = 0;
	
	// ȡ2000������
	for(i = 0; i < 2000; i++)
	{
		if(!i2cread(MPU6050_Addr2, ACCEL_XOUT_H, 14, data_write))
		{
			// ��ȡ��������
			accel[0] = (((int16_t)data_write[0])<<8) | data_write[1];
			accel[1] = (((int16_t)data_write[2])<<8) | data_write[3];
			accel[2] = (((int16_t)data_write[4])<<8) | data_write[5];
			accel_x += accel[0];
			accel_y += accel[1];
			accel_z += accel[2];
			// ��¼��Ч����
			count2++;
		}
	}
	
	// ��ƽ��ֵ�õ���Ưƫ����
	Accel_Xout_Offset = (float)accel_x / count2;
	Accel_Yout_Offset = (float)accel_y / count2;
	Accel_Zout_Offset = (float)accel_z / count2;
}

// ���ٶȼ��������ǳ�ʼ��
void MPU6050_Init(void)
{
	// �������״̬
	I2C_WriteByte(MPU6050_Addr, PWR_MGMT_1, 0x01);
	
	// ���ò���Ƶ��Ϊ1KHZ
	I2C_WriteByte(MPU6050_Addr, SMPLRT_DIV, 0x00);
	// ���õ�ͨ�˲��Ĵ���Ϊ5HZ
	I2C_WriteByte(MPU6050_Addr, CONFIG, 0x06);
	
	// ������·I2C
	I2C_WriteByte(MPU6050_Addr, INT_PIN_CFG, 0x42);
	// ��FIFO����
	I2C_WriteByte(MPU6050_Addr, USER_CTRL, 0x40);
	
	// ���������ǲɼ���ΧΪ+-500��/s
	I2C_WriteByte(MPU6050_Addr, GYRO_CONFIG, 0x0B);
	// ���ü��ٶȼƲɼ���ΧΪ+-4g
	I2C_WriteByte(MPU6050_Addr, ACCEL_CONFIG, 0x08);

	// ��ȡ��Ưƫ����
	Get_Gyro_Bias();
	Get_Accel_Bias();
}

// �����Ƴ�ʼ��
void HMC5883L_Init(void)
{
	// ���ñ�׼�����������Ϊ75HZ
	I2C_WriteByte(HMC5883L_Addr, HMC5883L_ConfigurationRegisterA, 0x18);
	// ���ò���Ƶ��Ϊ+-1.3Ga
	I2C_WriteByte(HMC5883L_Addr, HMC5883L_ConfigurationRegisterB, 0x20);
	// ������������ģʽ
	I2C_WriteByte(HMC5883L_Addr, HMC5883L_ModeRegister, 0x00);
}

// �ɼ�����������
void Get_AHRS_Data(void)
{
	int16_t gyro[3], accel[3]; 
	uint8_t data_write[14];

	// ��ȡ���ٶȺͽ��ٶ�
	if(!i2cread(MPU6050_Addr2, ACCEL_XOUT_H, 14, data_write))
	{
		accel[0] = (((int16_t)data_write[0])<<8) | data_write[1];
		accel[1] = (((int16_t)data_write[2])<<8) | data_write[3];
		accel[2] = (((int16_t)data_write[4])<<8) | data_write[5];
		gyro[0]  = (((int16_t)data_write[8])<<8) | data_write[9];
		gyro[1]  = (((int16_t)data_write[10])<<8) | data_write[11];
		gyro[2]  = (((int16_t)data_write[12])<<8) | data_write[13];
		
		// ��Ư������λת��
		init_ax = (float)(accel[0] - Accel_Xout_Offset) / Accel_4_Scale_Factor;
		init_ay = (float)(accel[1] - Accel_Yout_Offset) / Accel_4_Scale_Factor;
		init_az = (float)(accel[2] + (Accel_4_Scale_Factor - Accel_Zout_Offset)) / Accel_4_Scale_Factor;
		init_gx = ((float)gyro[0] - Gyro_Xout_Offset) / Gyro_500_Scale_Factor / ARC_TO_DEG;
		init_gy = ((float)gyro[1] - Gyro_Yout_Offset) / Gyro_500_Scale_Factor / ARC_TO_DEG;
		init_gz = ((float)gyro[2] - Gyro_Zout_Offset) / Gyro_500_Scale_Factor / ARC_TO_DEG;
	}
	// ��ȡ��ͨ��
	if(!i2cread(HMC5883L_Addr2, HMC5883L_XOUT_MSB, 6, data_write))
	{	
		init_mx = (data_write[0] << 8) | data_write[1];
		init_my = (data_write[4] << 8) | data_write[5];
		init_mz = (data_write[2] << 8) | data_write[3];

		// ���봦����λת��
		if(init_mx > 0x7fff) init_mx-=0xffff;
		if(init_my > 0x7fff) init_my-=0xffff;
		if(init_mz > 0x7fff) init_mz-=0xffff;
		init_mx /= 1090.0f;
		init_my /= 1090.0f;
		init_mz /= 1090.0f;
	}
}
