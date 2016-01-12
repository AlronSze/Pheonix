// MPU6050�豸��ַ
#define	MPU6050_Addr		0xD0
#define	MPU6050_Addr2   0x68
// MPU6050�ڼĴ�����ַ
#define	SMPLRT_DIV		  0x19
#define	CONFIG			    0x1A
#define	GYRO_CONFIG		  0x1B
#define	ACCEL_CONFIG	  0x1C
#define INT_PIN_CFG     0x37
#define USER_CTRL       0x6A
#define	ACCEL_XOUT_H	  0x3B
#define	ACCEL_XOUT_L	  0x3C
#define	ACCEL_YOUT_H	  0x3D
#define	ACCEL_YOUT_L	  0x3E
#define	ACCEL_ZOUT_H	  0x3F
#define	ACCEL_ZOUT_L	  0x40
#define	TEMP_OUT_H		  0x41
#define	TEMP_OUT_L		  0x42
#define	GYRO_XOUT_H	  	0x43
#define	GYRO_XOUT_L		  0x44
#define	GYRO_YOUT_H	  	0x45
#define	GYRO_YOUT_L		  0x46
#define	GYRO_ZOUT_H	  	0x47
#define	GYRO_ZOUT_L	  	0x48
#define	PWR_MGMT_1	  	0x6B
#define	WHO_AM_I		    0x75

// HMC5883L�豸��ַ
#define	HMC5883L_Addr						          0x3C
#define	HMC5883L_Addr2					          0x1E
// HMC5883L�ڼĴ�����ַ
#define HMC5883L_ConfigurationRegisterA		0x00
#define HMC5883L_ConfigurationRegisterB		0x01
#define HMC5883L_ModeRegister				      0x02
#define HMC5883L_XOUT_MSB					        0x03
#define HMC5883L_XOUT_LSB					        0x04
#define HMC5883L_ZOUT_MSB					        0x05
#define HMC5883L_ZOUT_LSB					        0x06
#define HMC5883L_YOUT_MSB					        0x07
#define HMC5883L_YOUT_LSB					        0x08
#define HMC5883L_StatusRegister			    	0x09
#define HMC5883L_ID_A							        0x0A
#define HMC5883L_ID_B							        0x0B
#define HMC5883L_ID_C							        0x0C

// ���ٶȵ�λת������
#define Gyro_250_Scale_Factor   131.0f
#define Gyro_500_Scale_Factor   65.5f
#define Gyro_1000_Scale_Factor  32.8f
#define Gyro_2000_Scale_Factor  16.4f
// ���ٶȵ�λת������
#define Accel_2_Scale_Factor    16384.0f
#define Accel_4_Scale_Factor    8192.0f
#define Accel_8_Scale_Factor    4096.0f
#define Accel_16_Scale_Factor   2048.0f

// ��������
extern float init_ax, init_ay, init_az,
						init_gx, init_gy, init_gz,
						init_mx, init_my, init_mz;
extern float Accel_Xout_Offset, Accel_Yout_Offset, Accel_Zout_Offset;

// ��������
void MPU6050_Init(void);
void HMC5883L_Init(void);
void Get_AHRS_Data(void);
