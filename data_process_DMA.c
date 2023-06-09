#include "data_process_DMA.h"

/* <�����봦��������> */
static void LED_on(void);
static void LED_off(void);
/* <4λ����֡������> */
static DataFrameError Four_Bit_Data_Frame(DataFrameHandle_t *data_frame);

/* <����֡���> */
DataFrameHandle_t data_frame_handle;
/* <���幦�����봦������ӳ���ϵ����> */
FrameProceHandle_t func_handle_map[] = {
	{0x01, LED_on},
	{0x02, LED_off},
};

/**
  * @brief   ��������DMA������ʹ��IDLE�жϺ���
  * @note    ���û���main��������
  * @param   *buff������DMA����������
  * @param   ��
  * @param   ��
  * @retval  �� 
  */
void Enable_IDLE_Receive_DMA(DataFrameHandle_t *buff)
{
	HAL_UART_Receive_DMA(&UART_HANDLE, (uint8_t *)buff->rx_buff, sizeof(buff->rx_buff));  //��������DMA����
	__HAL_UART_ENABLE_IT(&UART_HANDLE, UART_IT_IDLE);  //ʹ��ָ���Ĵ����ж�(IDLE�ж�)
}

/**
  * @brief   ����֡�Ĵ�����
  * @note    ���û���whileѭ������
  * @param   
  * @param   
  * @param    
  * @retval   
  */
void Data_Frame_Process(DataFrameHandle_t *data_frame)
{	
	
	if(data_frame->rx_flag == 1){
		data_frame->rx_flag = 0;    //����жϱ�־			
		
		switch(data_frame->rx_count)
		{
			case 4: Four_Bit_Data_Frame(data_frame); break;
			case 5: break;
		}		
	}	
}

/**
  * @brief   ����4λ����֡������֡ͷ��aa �豸�룺00 �����룺01 ֡β��55��
  * @note    data_frame�����յ�������֡
  * @param   ��
  * @param   ��
  * @param   ��
  * @retval  ����֡������
  */
static DataFrameError Four_Bit_Data_Frame(DataFrameHandle_t *data_frame)
{
	uint8_t i = 0;
	
	if(data_frame->rx_buff[0] == FRAME_HEAD && data_frame->rx_buff[data_frame->rx_count-1] == FRAME_END){  //�ж�֡ͷ֡β			
		if(data_frame->rx_buff[1] == DEVICE_CODE){  //�ж��豸��	
			
			uint8_t func_code = data_frame->rx_buff[2];  //��ȡ������		
			
			/* <���Ҵ�����> */
			for(i = 0; i<sizeof(func_handle_map)/sizeof(func_handle_map[0]); i++){
				if(func_handle_map[i].function_code == func_code){
					func_handle_map[i].func_process();  //���ô�����
					break;
				}
			}
			if(i == sizeof(func_handle_map)/sizeof(func_handle_map[0])){  //�����벻ƥ��
				Debug(DEBUG_UART, "function error\r\n");
				return FUNC_ERR;
			}
		}
		else{Debug(DEBUG_UART, "device error\r\n");  return DEV_ERR;}  //�豸�벻ƥ��
	}
	else{Debug(DEBUG_UART, "head tail error\r\n"); return HT_ERR;}   //֡ͷ֡β��ƥ��
	
	data_frame->rx_count = 0;   //������յ������ݸ���
	
	return DATA_SUCCESS;
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1){
		data_frame_handle.rx_flag = 1;  //�ж��Ƿ�����ж�
		data_frame_handle.rx_count = huart->RxXferSize - huart->hdmarx->Instance->CNDTR;  //���㴮�ڽ������ݵĸ���
		HAL_UART_Receive_DMA(&huart1, (uint8_t *)data_frame_handle.rx_buff, sizeof(data_frame_handle.rx_buff));  //������������DMA����
	}
}

/**
  * @brief  ����LED�����봦����
  * @note   �û��Լ���д������ʵ�ֲ�ͬ�������Ӧ�Ĺ���
  * @param  ��  
  * @param  ��
  * @param  ��
  * @retval ��
  */
static void LED_on(void)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
}

/**
  * @brief  Ϩ��LED�����봦����
  * @note   �û��Լ���д������ʵ�ֲ�ͬ�������Ӧ�Ĺ���
  * @param  ��  
  * @param  ��
  * @param  ��
  * @retval ��
  */
static void LED_off(void)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
}

