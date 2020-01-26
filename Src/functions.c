/*
 * functions.c
 *
 *  Created on: Jan 20, 2020
 *      Author: Adam Kolodziejczyk & Maciej Sadowski
 */

#include "functions.h"

void ReadSensors(TIM_HandleTypeDef *htim,uint32_t *lightSensor1,uint32_t *lightSensor2)
{

	HAL_ADC_Start(&hadc1);
	HAL_ADC_Start(&hadc2);
	while((HAL_ADC_PollForConversion(&hadc1, 10) != HAL_OK) && (HAL_ADC_PollForConversion(&hadc2,10) != HAL_OK));
	lightSensor1 =  (uint32_t)(HAL_ADC_GetValue(&hadc1)/((float)0x0fff) * MAX_VOLTAGE*1000) / 33;
	lightSensor2 =  (uint32_t)(HAL_ADC_GetValue(&hadc2)/((float)0x0fff) * MAX_VOLTAGE*1000) / 33;
	messageSize = sprintf(buffer, "SEN1 =%lu\r\nSEN2 =%lu", lightSensor1,lightSensor2);
	HAL_UART_Transmit_IT(&huart3, (uint8_t*)buffer, messageSize);
}


void BlinkDiodes(int delay)
{
	HAL_Delay(delay);
	HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, 1);
	HAL_Delay(delay);
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 1);
	HAL_Delay(delay);
	HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 1);
	HAL_Delay(delay);
	HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, 0);
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 0);
	HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, 0);
}

void SetDevice(char deviceType[],int deviceNumb,int val)
{
	if(!strcmp(deviceType,"LED"))
	{
		switch(deviceNumb)
		{
			case 1:HAL_GPIO_WritePin(USER_LD1_GPIO_Port,USER_LD1_Pin, val) ;break;
			case 2:HAL_GPIO_WritePin(USER_LD2_GPIO_Port,USER_LD2_Pin, val) ;break;
			case 3:HAL_GPIO_WritePin(USER_LD3_GPIO_Port,USER_LD3_Pin, val) ;break;
			case 4:HAL_GPIO_WritePin(USER_LD4_GPIO_Port,USER_LD4_Pin, val) ;break;
			case 5:HAL_GPIO_WritePin(USER_LD5_GPIO_Port,USER_LD5_Pin, val); break;
			default:;
		}
		SendACK(deviceType, deviceNumb, val,0);
	}
	else if(!strcmp(deviceType,"SRV"))
	{
		//Function to add!!
		//MoveServo(val);
		SendACK(deviceType, deviceNumb, val,1);
	}

}

void SendACK(char deviceType[],int deviceNumb,int val, int device)
{
	if(device == 0)
		messageSize = sprintf(buffer,"%s %d=00%d",deviceType,deviceNumb,val);
	else if(device == 1)
		messageSize = sprintf(buffer,"%s %d=%d",deviceType,deviceNumb,val);

	HAL_UART_Transmit_IT(&huart3,(uint8_t*)buffer,messageSize);


}

void MoveServo(int val)
{
	//__HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_1,val*500);

}
