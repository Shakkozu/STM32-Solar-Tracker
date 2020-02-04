/*
 * functions.h
 *
 *  Created on: Jan 20, 2020
 *      Author: Adam Kolodziejczyk & Maciej Sadowski
 */


#include "main.h"
#include "stdio.h"

/* Servo parameters acquired due to tests */
#define SERVO_PARAMETER 65
#define SERVO_CONST 2700

extern uint32_t lightSensor1;
extern uint32_t lightSensor2;
extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern UART_HandleTypeDef huart3;
extern TIM_HandleTypeDef htim7;
extern TIM_HandleTypeDef htim9;
extern int messageSize;
extern char buffer[];

/* Function prototypes****************************************************************************/


/**
 *
 * @brief	This function reads values from ADC1 and ADC2.
 * @desc	Values are converted to uint_32t type, to avoid operating on float numbers in future.
 *
 * @param 	pointer to a TIM Handler
 * @param 	lightSensor1 handler for reading from sensor 1
 * @param 	lightSensor2 handler for reading from sensor 2
 * @param	mode this parameter is designed to let user choose between sending information
 * 			retrieved from sensors via UART, or not sending them
 * @retval 	Function returns levelized sensors value
 *
 */
uint32_t ReadSensors(TIM_HandleTypeDef *htim,uint32_t *lightSensor1,uint32_t *lightSensor2, int mode);


/*
 * @brief 	SendSensorsInfo sends ADC readings via UART.
 * @param 	lightSensor1 handler for reading from sensor 1
 * @param 	lightSensor2 handler for reading from sensor 2
 * */
void SendSensorsInfo(uint32_t lightsensor1, uint32_t lightsensor2);


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
 * @retval 	Function returns 1 if parameters were in supposed range or 0 otherwise
 */
int SetDevice(char deviceType[],int deviceNumb,int val);


/*
 * @brief	MoveServo function.
 * @desc	This function sets servomechanism at given position in degrees
 * @param 	degree given angle in degrees (0-180)
 * @retval 	None
 *  */
void MoveServo(int deg);


/*
 * @brief	SendACK function sends affirmative acknowledgment.
 * @param 	deviceType appropriate string describing device ('LED || SRV')
 * @param 	deviceNumb tells, which device was targeted
 * @param 	val stores value send to device
 * @retval	None
 */
void SendACK(char deviceType[],int deviceNumb,int val);

/*
 * @brief 	ClearBuffer clears buffer that is storing received messages
 * @param	buff for incoming messages
 * @param	size of buffer
 */
void ClearBuffer(char buff[], int size);

/*
 * @brief 	LossOfData function Clears buffer and sends information, that
 * 			packet of data was lost
 * @param	buffer buffer for incoming messages
 * @param 	buffer_size size of buffer
 * @param 	huart pointer to UART handle
 * */
void LossOfData(char buffer[], int buffer_size, UART_HandleTypeDef *huart);

/*
 * @brief	ScanArea function Sets servomechanism at degrees <0:180>
 * @desc	After first scan, this function should Start TIM7, to do
 * 			that effectively, parameter 'mode' is used.
 * @param	maxResult variable stores maximum value received during scan
 * @param 	mode If mode equals 0, function starts TIM7 after scan
 * */
int ScanArea(uint32_t *maxResult, int mode);





