# STM32-DMA-Serial-Data-Frame-
该仓库仅上传个人代码模块

# 使用教程
1. main.c
```
#include "stdio.h"
#include "Debug.h"
#include "data_process_DMA.h"

int main(void)
{
  Debug(huart1, "begin\r\n");
  Enable_IDLE_Receive_DMA(&data_frame_handle);	
  while (1)
  {
    Data_Frame_Process(&data_frame_handle);
    HAL_Delay(1); 
  }
}
```
