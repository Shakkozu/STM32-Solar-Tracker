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

	/* Wait until readings are done */
	while((HAL_ADC_PollForConversion(&hadc1, 10) != HAL_OK) &&
			(HAL_ADC_PollForConversion(&hadc2,10) != HAL_OK));

	lightSensor1 =  (uint32_t)(HAL_ADC_GetValue(&hadc1)/((float)0x0fff) * MAX_VOLTAGE*1000);
	lightSensor2 =  (uint32_t)(HAL_ADC_GetValue(&hadc2)/((float)0x0fff) * MAX_VOLTAGE*1000);
	uint32_t retval1 = (uint32_t)lightSensor1/33;
	uint32_t retval2 = (uint32_t)lightSensor2/33;
	messageSize = sprintf(buffer, "SEN1 =%lu\r\nSEN2 =%lu", retval1,retval2);
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
		SendACK(deviceType, deviceNumb, val);
	}
	else if(!strcmp(deviceType,"SRV"))
	{
		MoveServo(val);
		SendACK(deviceType, deviceNumb, val);
	}

}

void SendACK(char deviceType[],int deviceNumb,int val)
{
	if(!strcmp(deviceType,"LED"))
		messageSize = sprintf(buffer,"%s %d=00%d",deviceType,deviceNumb,val);
	else if(!strcmp(deviceType,"SRV"))
	{
		if(val<10) messageSize = sprintf(buffer,"%s %d=00%d",deviceType,deviceNumb,val);
		else if(val<100) messageSize = sprintf(buffer,"%s %d=0%d",deviceType,deviceNumb,val);
		else messageSize = sprintf(buffer,"%s %d=%d",deviceType,deviceNumb,val);
	}

	HAL_UART_Transmit_IT(&huart3,(uint8_t*)buffer,messageSize);
}

void MoveServo(int deg)
{
	uint16_t SET_COMPARE = deg * SERVO_PARAMETER + SERVO_CONST;
	__HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_1,SET_COMPARE);
}
