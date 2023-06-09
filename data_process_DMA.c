#include "data_process_DMA.h"

/* <功能码处理函数声明> */
static void LED_on(void);
static void LED_off(void);
/* <4位数据帧处理函数> */
static DataFrameError Four_Bit_Data_Frame(DataFrameHandle_t *data_frame);

/* <数据帧句柄> */
DataFrameHandle_t data_frame_handle;
/* <定义功能码与处理函数的映射关系数组> */
FrameProceHandle_t func_handle_map[] = {
	{0x01, LED_on},
	{0x02, LED_off},
};

/**
  * @brief   启动串口DMA接收与使能IDLE中断函数
  * @note    由用户在main函数调用
  * @param   *buff：串口DMA结束缓存区
  * @param   无
  * @param   无
  * @retval  无 
  */
void Enable_IDLE_Receive_DMA(DataFrameHandle_t *buff)
{
	HAL_UART_Receive_DMA(&UART_HANDLE, (uint8_t *)buff->rx_buff, sizeof(buff->rx_buff));  //开启串口DMA接收
	__HAL_UART_ENABLE_IT(&UART_HANDLE, UART_IT_IDLE);  //使能指定的串口中断(IDLE中断)
}

/**
  * @brief   数据帧的处理函数
  * @note    由用户在while循环调用
  * @param   
  * @param   
  * @param    
  * @retval   
  */
void Data_Frame_Process(DataFrameHandle_t *data_frame)
{	
	
	if(data_frame->rx_flag == 1){
		data_frame->rx_flag = 0;    //清除中断标志			
		
		switch(data_frame->rx_count)
		{
			case 4: Four_Bit_Data_Frame(data_frame); break;
			case 5: break;
		}		
	}	
}

/**
  * @brief   处理4位数据帧格函数（帧头：aa 设备码：00 功能码：01 帧尾：55）
  * @note    data_frame：接收到的数据帧
  * @param   无
  * @param   无
  * @param   无
  * @retval  返回帧错误码
  */
static DataFrameError Four_Bit_Data_Frame(DataFrameHandle_t *data_frame)
{
	uint8_t i = 0;
	
	if(data_frame->rx_buff[0] == FRAME_HEAD && data_frame->rx_buff[data_frame->rx_count-1] == FRAME_END){  //判断帧头帧尾			
		if(data_frame->rx_buff[1] == DEVICE_CODE){  //判断设备码	
			
			uint8_t func_code = data_frame->rx_buff[2];  //获取功能码		
			
			/* <查找处理函数> */
			for(i = 0; i<sizeof(func_handle_map)/sizeof(func_handle_map[0]); i++){
				if(func_handle_map[i].function_code == func_code){
					func_handle_map[i].func_process();  //调用处理函数
					break;
				}
			}
			if(i == sizeof(func_handle_map)/sizeof(func_handle_map[0])){  //功能码不匹配
				Debug(DEBUG_UART, "function error\r\n");
				return FUNC_ERR;
			}
		}
		else{Debug(DEBUG_UART, "device error\r\n");  return DEV_ERR;}  //设备码不匹配
	}
	else{Debug(DEBUG_UART, "head tail error\r\n"); return HT_ERR;}   //帧头帧尾不匹配
	
	data_frame->rx_count = 0;   //清除接收到的数据个数
	
	return DATA_SUCCESS;
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1){
		data_frame_handle.rx_flag = 1;  //判断是否进入中断
		data_frame_handle.rx_count = huart->RxXferSize - huart->hdmarx->Instance->CNDTR;  //计算串口接收数据的个数
		HAL_UART_Receive_DMA(&huart1, (uint8_t *)data_frame_handle.rx_buff, sizeof(data_frame_handle.rx_buff));  //重新启动串口DMA接收
	}
}

/**
  * @brief  点亮LED功能码处理函数
  * @note   用户自己编写，用于实现不同功能码对应的功能
  * @param  无  
  * @param  无
  * @param  无
  * @retval 无
  */
static void LED_on(void)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
}

/**
  * @brief  熄灭LED功能码处理函数
  * @note   用户自己编写，用于实现不同功能码对应的功能
  * @param  无  
  * @param  无
  * @param  无
  * @retval 无
  */
static void LED_off(void)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
}

