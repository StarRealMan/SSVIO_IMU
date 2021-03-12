#include "usart.h"

__IO UCHAR8 UA3RxDMAbuf[USART3_RXDMA_LEN] = {0};
     UCHAR8 UA3RxMailbox[USART3_RXMB_LEN] = {0};
USART_RX_TypeDef USART3_Rcr = {USART3,USART3_RX_STREAM,UA3RxMailbox,UA3RxDMAbuf,USART3_RXMB_LEN,USART3_RXDMA_LEN,0,0,0};

__IO UCHAR8 UA3TxDMAbuf[USART3_TXDMA_LEN] = {0};
     UCHAR8 UA3TxMailbox[USART3_TXMB_LEN] = {0};
USART_TX_TypeDef USART3_Tcr = {USART3,USART3_TX_STREAM,UA3TxMailbox,UA3TxDMAbuf,USART3_TXMB_LEN,USART3_TXDMA_LEN};	


void USART3_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3); //TX	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3); //RX
	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
      
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_DeInit(USART3);
	USART_InitStructure.USART_BaudRate = 460800;   
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;  
	USART_Init(USART3,&USART_InitStructure);
    
	USART_Cmd(USART3,ENABLE);
	
	USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);
	USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE);
	USART_ITConfig(USART3,USART_IT_IDLE,ENABLE); //���������ж�
	
	//RX
	DMA_DeInit(USART3_RX_STREAM);
	DMA_InitStructure.DMA_Channel            = DMA_Channel_4;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(USART3->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)UA3RxDMAbuf;
	DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize         = USART3_RXDMA_LEN;
	DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;	
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority           = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_FIFOMode           = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold      = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst        = DMA_Mode_Normal;
	DMA_InitStructure.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;
  	DMA_Init(USART3_RX_STREAM,&DMA_InitStructure);

  	DMA_Cmd(USART3_RX_STREAM,ENABLE);
	
	//Tx
	DMA_DeInit(USART3_TX_STREAM);
	DMA_InitStructure.DMA_Channel            = DMA_Channel_4;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(USART3->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr    = (uint32_t)UA3TxDMAbuf;
	DMA_InitStructure.DMA_DIR                = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_BufferSize         = Main_Buf_Length;
	DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority           = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_FIFOMode           = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold      = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst        = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;
	DMA_Init(USART3_TX_STREAM,&DMA_InitStructure);
	DMA_ITConfig(DMA1_Stream3, DMA_IT_TC, ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	DMA_Cmd(USART3_TX_STREAM,ENABLE);
	
	MainSendData.stMainData.Head1 = 0x55;
	MainSendData.stMainData.Head2 = 0x00;
	MainSendData.stMainData.Tail1 = 0x00;
	MainSendData.stMainData.Tail2 = 0xAA;
}

