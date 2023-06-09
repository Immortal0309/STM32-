#ifndef _DATA_PROCESS_DMA_
#define _DATA_PROCESS_DMA_

#include "main.h"
#include "usart.h"
#include "Debug.h"

#define DEBUG_UART             huart1  /*<Debug���ھ��>*/
#define UART_HANDLE            huart1  /*<���ڲ������Ӿ��>*/

#define LENGTH                 100     /*<���ڽ��ջ�������С>*/

#define FRAME_HEAD             0xaa    /*<֡ͷ>*/
#define FRAME_END              0x55    /*<֡β>*/
#define FUNC_LED_OPEN_CODE     0x01    /*<����֡>*/
#define FUNC_LED_CLOSE_CODE    0x02    /*<����֡>*/
#define FUNC_LED_TOGGLE_CODE   0x03    /*<����֡>*/
#define DEVICE_CODE            0x00    /*<�豸֡>*/

/* <�����뺯��ָ��> */
typedef void(*HandleFunc)();

/* <���幦�����봦������ӳ���ϵ> */
typedef struct
{
	uint8_t function_code;    /* <������> */
	HandleFunc func_process;  /* <�����봦����> */
}FrameProceHandle_t;

typedef struct
{
	uint8_t rx_flag : 1;      /*<������ɱ�־��0Ϊδ��ɣ�1Ϊ���>*/
	uint8_t rx_count;         /*<�������ݵĸ���>*/
	uint8_t rx_buff[LENGTH];  /*<���ڽ��ջ�����>*/
}DataFrameHandle_t;

extern DataFrameHandle_t data_frame_handle;

typedef enum
{
	HT_ERR        = 0,      /*<��ʾ֡ͷ֡β����>*/
	DEV_ERR       = 1,      /*<��ʾ�豸����>*/
	FUNC_ERR      = 2,      /*<��ʾ���ܴ���>*/
	DATA_SUCCESS  = 3       /*<��ʾ�ɹ�>*/
}DataFrameError;

void Enable_IDLE_Receive_DMA(DataFrameHandle_t *buff);
void Data_Frame_Process(DataFrameHandle_t *data_frame_handle);

#endif
