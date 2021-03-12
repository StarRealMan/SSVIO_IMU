#include "main.h"

SINT32 main(void)
{
	BSP_Configuration();
	while(1)
	{
		if(Flag_200Hz)
		{
			USART_DMA_Send();
		}

		if(Flag_1000hz)
		{
			IMU_Update_Mahony(&imu_data,1e-3f);
		}
	}

	return 0;		
}


