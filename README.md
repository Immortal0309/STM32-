# STM32 串口不定长接收数据帧处理代码模块
# 使用教程
1. main.c代码
```
#include "data_process_DMA.h"

int main(void)
{
  Enable_IDLE_Receive_DMA(&data_frame_handle);
  
  while (1)
  {
		Data_Frame_Process(&data_frame_handle);
		HAL_Delay(1);
  }
}
```


