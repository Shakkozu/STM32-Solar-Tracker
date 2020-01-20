/*
 * functions.h
 *
 *  Created on: Jan 20, 2020
 *      Author: Adam Kolodziejczyk & Maciej Sadowski
 */


#include "main.h"
#include "stdio.h"

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern UART_HandleTypeDef huart3;
extern char buffer[];
extern uint8_t size;
extern float max_voltage;

/**
 * ReadSensors function
 *
 * This function reads values from ADC1 and ADC2.
 * Values are converted to uint_32t type, to avoid operating on float numbers in future.
 * Function also sends ADC readings via UART.
 *
 * @param htim HandleTypeDef for appropriate timer
 * @param lightSensor1 During function call this parameter should be passed by reference to get this value! It stores reading from sensor1 (uint32_t)
 * @param lightSensor2 During function call this parameter should be passed by reference to get this value! It stores reading from sensor2 (uint32_t)
 *
 * @return lighSensor1 and lightSensor2  should be passed by reference to
 */
void ReadSensors(TIM_HandleTypeDef *htim,uint32_t *lightSensor1,uint32_t *lightSensor2);

/**
 * BlinkDiodes function Turns diodes one by one LD1,LD2,LD3 and switches them off
 *
 * @param delay Describes delay between actions (miliseconds)
 */
void BlinkDiodes(int delay);
