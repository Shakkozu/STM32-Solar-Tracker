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
extern TIM_HandleTypeDef htim9;
extern int messageSize;
extern char buffer[];





/**
 * ReadSensors function.
 *
 * @brief	This function reads values from ADC1 and ADC2.
 * @desc	Values are converted to uint_32t type, to avoid operating on float numbers in future.
 * 			Function also sends ADC readings via UART.
 * @param 	pointer to a TIM Handler
 * @param 	lightSensor1 During function call this parameter should be passed by reference to get this value! It stores reading from sensor1 (uint32_t)
 * @param 	lightSensor2 During function call this parameter should be passed by reference to get this value! It stores reading from sensor2 (uint32_t)
 * @retval 	none but lighSensor1 and lightSensor2 should be passed by reference to store those values
 */
void ReadSensors(TIM_HandleTypeDef *htim,uint32_t *lightSensor1,uint32_t *lightSensor2);


/**
 * @brief	BlinkDiodes function Turns diodes one by one LD1,LD2,LD3 and switches them off.
 * @param 	delay Describes delay between actions (miliseconds)
 * @retval 	None
 */
void BlinkDiodes(int delay);


/* @brief	Set Output devices
 * @desc	SetDevice function properly sets output devices according to message received
 * 			via Uart. Received message is decoded in HAL_UART_RxCpltCallback() function.
 * @param 	deviceType appropriate string describing device ('LED || SRV')
 * @param 	deviceNumb
 * @param 	val
 * @retval 	None
 */
void SetDevice(char deviceType[],int deviceNumb,int val);


/*
 * @brief	MoveServo function.
 * @desc	This function sets servomechanism at given position in degrees (not yet)
 * @param 	degree given angle in degrees (0-180)
 * @retval 	None
 *  */
void MoveServo(int deg);


/*
 * @brief	SendACK function sends affirmative acknowledgment.
 * @param deviceType appropriate string describing device ('LED || SRV')
 * @param deviceNumb tells, which device was targeted
 * @param val stores value send to device
 */
void SendACK(char deviceType[],int deviceNumb,int val);









