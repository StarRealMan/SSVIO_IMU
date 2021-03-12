#include "usart_protocol.h"

extern float quaternion_0;
extern float quaternion_1;
extern float quaternion_2;
extern float quaternion_3;

extern float Acc_X_Real;
extern float Acc_Y_Real;
extern float Acc_Z_Real;


void USART_DMA_Send(void)
{
	MainSendData.stMainData.quaternion_0 = quaternion_0;
	MainSendData.stMainData.quaternion_1 = quaternion_1;
	MainSendData.stMainData.quaternion_2 = quaternion_2;
	MainSendData.stMainData.quaternion_3 = quaternion_3;
	MainSendData.stMainData.acc_0 = Acc_X_Real;
	MainSendData.stMainData.acc_1 = Acc_Y_Real;
	MainSendData.stMainData.acc_2 = Acc_Z_Real;
	
	DMA_ClearITPendingBit(DMA1_Stream3, DMA_IT_TCIF3);
	DMA_Cmd(DMA1_Stream3, DISABLE);
	DMA1_Stream3->M0AR = (uint32_t)&MainSendData;
	DMA1_Stream3->NDTR = (uint32_t)Main_Buf_Length;
	DMA_Cmd(DMA1_Stream3, ENABLE);

}

