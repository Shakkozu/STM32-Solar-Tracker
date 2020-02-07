/*
 * functions.c
 *
 *  Created on: Jan 20, 2020
 *      Author: Adam Kolodziejczyk & Maciej Sadowski
 */

#include "functions.h"

uint32_t ReadSensors(TIM_HandleTypeDef *htim,uint32_t *lightSensor11,uint32_t *lightSensor21, int mode)
{

	HAL_ADC_Start(&hadc1);
	HAL_ADC_Start(&hadc2);

	/* Wait until readings are done */
	while((HAL_ADC_PollForConversion(&hadc1, 10) != HAL_OK) &&
			(HAL_ADC_PollForConversion(&hadc2,10) != HAL_OK));

	lightSensor1 =  (uint32_t)(HAL_ADC_GetValue(&hadc1)/((float)0x0fff) * MAX_VOLTAGE*1000);
	lightSensor2 =  (uint32_t)(HAL_ADC_GetValue(&hadc2)/((float)0x0fff) * MAX_VOLTAGE*1000);
	uint32_t result = ((lightSensor1 + lightSensor2))/2;
	uint32_t retval1 = (uint32_t)lightSensor1/33;
	uint32_t retval2 = (uint32_t)lightSensor2/33;
	//messageSize = sprintf(buffer, "SEN1 =%lu\rSEN2 =%lu\r", retval1,retval2);
	//HAL_UART_Transmit_IT(&huart3, (uint8_t*)buffer, messageSize);
	//if(mode == 1) SendSensorsInfo(&lightSensor1, &lightSensor2);

	return result;
}

void SendSensorsInfo(uint32_t lightsensor1, uint32_t lightsensor2)
{
	uint32_t retval1 = (uint32_t)lightSensor1/33;
	uint32_t retval2 = (uint32_t)lightSensor2/33;
	messageSize = sprintf(buffer, "SEN 1=%lu\rSEN 2=%lu\r", retval1,retval2);
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

int SetDevice(char deviceType[],int deviceNumb,int val)
{
	int result =0;
	if(!strcmp(deviceType,"LED"))
	{
		result=1;
		switch(deviceNumb)
		{
			case 1:HAL_GPIO_WritePin(USER_LD1_GPIO_Port,USER_LD1_Pin, val) ;break;
			case 2:HAL_GPIO_WritePin(USER_LD2_GPIO_Port,USER_LD2_Pin, val) ;break;
			case 3:HAL_GPIO_WritePin(USER_LD3_GPIO_Port,USER_LD3_Pin, val) ;break;
			case 4:HAL_GPIO_WritePin(USER_LD4_GPIO_Port,USER_LD4_Pin, val) ;break;
			case 5:HAL_GPIO_WritePin(USER_LD5_GPIO_Port,USER_LD5_Pin, val); break;
			default:result=0;
		}
	}
	else if(!strcmp(deviceType,"SRV"))
	{
		if(val<=180 && val >=0)
		{
		MoveServo(val);
		result=1;
		}
	}
	else if(!strcmp(deviceType,"AUT"))
	{
		if(val == 1)
			{
			autoMode = 1;
		//	actualAngle = ScanArea(actualValue);
			result=1;
			}


		else if(val ==0) autoMode = 0;
		result =1;
	}
	else result =0;
	return result;


}

void SendACK(char deviceType[],int deviceNumb,int val)
{
	int success = 0;
	uint32_t retval1 = (uint32_t)lightSensor1/33;
	uint32_t retval2 = (uint32_t)lightSensor2/33;
	//messageSize = sprintf(buffer, "SEN1 =%lu\rSEN2 =%lu\r", retval1,retval2);
	if(!strcmp(deviceType,"LED"))
	{
messageSize = sprintf(buffer,"%s %d=00%d\rSEN 1=%lu\rSEN 2=%lu\r",deviceType,deviceNumb,val,retval1,retval2);
	success = 1;
	}
	else if(!strcmp(deviceType,"SRV"))
	{
		if(val<10) messageSize = sprintf(buffer,"%s %d=00%d\rSEN 1=%lu\rSEN 2=%lu\r",deviceType,deviceNumb,val,retval1,retval2);
		else if(val<100) messageSize = sprintf(buffer,"%s %d=0%d\rSEN 1=%lu\rSEN 2=%lu\r",deviceType,deviceNumb,val,retval1,retval2);
		else messageSize = sprintf(buffer,"%s %d=%d\rSEN 1=%lu\rSEN 2=%lu\r",deviceType,deviceNumb,val,retval1,retval2);
		success =1;
	}
	if(success) HAL_UART_Transmit_IT(&huart3,(uint8_t*)buffer,messageSize);
}

void MoveServo(int deg)
{
	uint16_t SET_COMPARE = deg * SERVO_PARAMETER + SERVO_CONST;
	__HAL_TIM_SET_COMPARE(&htim9,TIM_CHANNEL_1,SET_COMPARE);
	//actualAngle = deg;


}

void ClearBuffer(char buff[], int size)
{
	while(size != 0)
	{
		buff[size]=0;
		size--;
	}
}

void LossOfData(char buffer[], int buffer_size, UART_HandleTypeDef *huart)
{
	char message[] = "Loss Of Data\r";
	ClearBuffer(buffer, buffer_size);
	HAL_UART_Transmit_IT(huart, (uint8_t*)message, (sizeof(message)/sizeof(char))-1);
}

int ScanArea(uint32_t *maxResult)
{
	scanFlag=1;
	uint32_t dataBuffer[180] = {};

	int resultAngle=0; //wyni kąta
	uint32_t max_result=0; //?
	for(int angle=0; angle<180;angle++)
	{
		MoveServo(angle);
		HAL_Delay(100);
		dataBuffer[angle] = ReadSensors(&htim7, &lightSensor1, &lightSensor2,1);
		if(dataBuffer[angle] > max_result)
		{
			max_result = dataBuffer[angle];
			resultAngle = angle;
		}
		//Send information about actual position
		SendACK("SRV", 1,angle);
	}
	///SendACK("SRV", 1, resultAngle);
	MoveServo(resultAngle);
	SendACK("SRV", 1,resultAngle);

	//return maxResult via reference
	//if(mode == 0) HAL_TIM_Base_Start_IT(&htim7);
	actualValue = max_result;
	scanFlag = 0;
	return resultAngle;
}

int RecalibratePosition()
{

	if(lightSensor1 > 500 || lightSensor2 >500)
	{
		if(lightSensor2 > lightSensor1)
		{
			if(actualAngle > 0)
			{
				actualAngle--;
			}
		}
		else
		{
			if(actualAngle < 180)
			{
			actualAngle++;
			}

		}
	}
	//actualValue = ReadSensors(htim, lightSensor11, lightSensor21, mode)
	MoveServo(actualAngle);
	SendACK("SRV", 1,actualAngle);
	return actualAngle;
}

