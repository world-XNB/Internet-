//
// Created by 西~南~北 on 2022/4/22.
//

#ifndef RS485_RS485_H
#define RS485_RS485_H

#include <stdbool.h>
#include "main.h"
#include "usart.h"
#include <string.h>
#include "stdio.h"

#define SENSOR_485_DATA_BUFFER_MAX_LENGTH  60
typedef struct {
    uint16_t front;
    uint16_t rear;
    uint8_t *buffer;
    uint32_t maxSize;
} Buffer_t;

void askN();

void askP();

void askK();

void queueInit();

void uart2WriteByte(uint8_t);

void Buffer_Puts(Buffer_t *, uint8_t, uint16_t);

uint16_t CRC_16(uint8_t *vptr, uint8_t len);

double Modbus_Work(void);

#endif //RS485_RS485_H
