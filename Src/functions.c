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
	lightSensor1 =  (uint32_t)(HAL_ADC_GetValue(&hadc1)/((float)0x0fff) * max_voltage*1000);
	lightSensor2 =  (uint32_t)(HAL_ADC_GetValue(&hadc2)/((float)0x0fff) * max_voltage*1000);
	size = sprintf(buffer, "Value1: %lu[V]\r\nValue2: %lu[V]\r\n", lightSensor1,lightSensor2);
	HAL_UART_Transmit_IT(&huart3, (uint8_t*)buffer, size);
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
