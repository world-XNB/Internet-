//
// Created by 西~南~北 on 2022/4/22.
//

#include "RS485.h"

static Buffer_t sensor485Buffer;
uint8_t sensor485DataBuffer[SENSOR_485_DATA_BUFFER_MAX_LENGTH];//缓存数组

void queueInit() {
    sensor485Buffer.rear = 0;
    sensor485Buffer.front = 0;
    sensor485Buffer.maxSize = SENSOR_485_DATA_BUFFER_MAX_LENGTH;
    sensor485Buffer.buffer = sensor485DataBuffer;
}

void askN() {
    uint8_t ask1[8] = {0x01, 0X03, 0X00, 0X1E, 0X00, 0X01, 0XE4, 0X0C};//问询帧  氮
    queueInit();
    HAL_UART_Transmit(&huart1, ask1, sizeof(ask1), 0xffff);
    HAL_Delay(200);
}

void askP() {
    queueInit();
    uint8_t ask2[8] = {0x01, 0X03, 0X00, 0X1F, 0X00, 0X01, 0XB5, 0XCC};//问询帧  磷
    HAL_UART_Transmit(&huart1, ask2, 8, 0xffff);
    HAL_Delay(100);
}

void askK() {
    queueInit();
    uint8_t ask3[8] = {0x01, 0X03, 0X00, 0X20, 0X00, 0X01, 0X85, 0XC0};//问询帧  钾
    HAL_UART_Transmit(&huart1, ask3, 8, 0xffff);
    HAL_Delay(100);
}

void uart2WriteByte(uint8_t data) {
    Buffer_Puts(&sensor485Buffer, data, 1);//入队
}

void Buffer_Puts(Buffer_t *buffer, uint8_t data, uint16_t length) {
    buffer->rear = (buffer->rear + 1) % buffer->maxSize;
    buffer->buffer[buffer->rear] = data;//进队
}

//uint16_t CRC_16(uint8_t *vptr, uint8_t len) {
//    uint16_t MODBUSCRC = 0xffff;
//    uint16_t POLYNOMIAL = 0xa001;
//    uint8_t i, j;
//
//    for (i = 0; i < len; i++) {
//        MODBUSCRC ^= vptr[i];
//        for (j = 0; j < 8; j++) {
//            if ((MODBUSCRC & 0x0001) != 0) {
//                MODBUSCRC >>= 1;
//                MODBUSCRC ^= POLYNOMIAL;
//            } else {
//                MODBUSCRC >>= 1;
//            }
//        }
//    }
//    return MODBUSCRC;
//}

double Modbus_Work(void) {
    uint16_t content;
    uint8_t content_h;
    uint8_t content_l;
//    printf("%d\n", sensor485DataBuffer[1]);
    if (sensor485DataBuffer[1] == 0x01)//从机地址正确则进行换算
    {
        content_h = sensor485DataBuffer[4];
        content_l = sensor485DataBuffer[5];
        HAL_UART_Transmit(&huart2, &content_h, sizeof(content_h), 100);
        HAL_UART_Transmit(&huart2, &content_l, sizeof(content_l), 100);
        content = (uint16_t) ((content_h << 8) & content_l);
    }
    memset(sensor485DataBuffer, 0, SENSOR_485_DATA_BUFFER_MAX_LENGTH);
    return content;
}
