/*
 * getEncoder.h
 *
 *  Created on: Oct 25, 2023
 *      Author: jkx
 */

#ifndef GETENCODER_H_
#define GETENCODER_H_


#include "stdio.h"
#include "stdint.h"
#include "main.h"
#include "dcMotor.h"


extern uint16_t Encoder_Overflow_Count;
extern int Encoder_Overflow_Count2;
// function to start the encoder
void startEncoder(void);
// get speed of the encoder
//uint32_t getEncoderValue(void);
int getEncoderValue(void);
int getEncoderValue2(void);
// get direction of the encoder
uint8_t getMotorDirectionfromEncoder(void);



#endif /* GETENCODER_H_ */
