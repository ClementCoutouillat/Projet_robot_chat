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
extern uint16_t Encoder_Overflow_Count2;
// function to start the encoder
void startEncoder(void);
// get speed of the encoder
// uint32_t getEncoderValue(void);
int32_t getEncoderValue(void);
int32_t getEncoderValue2(void);

#endif /* GETENCODER_H_ */
