/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "oled.h"
#include "bh1750.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
u_char rec1;//usart1接受数据
u_char rec2;//usart2接受数据

uint16_t light;//光强

float adc_value;//ADC转换值存放
float vol;//电压值
float hum;//湿度百分比的分子部分

uint8_t Hum;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void) {
    /* USER CODE BEGIN 1 */
    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_USART1_UART_Init();
    MX_USART2_UART_Init();
    MX_I2C1_Init();
    MX_I2C2_Init();
    MX_ADC1_Init();
    /* USER CODE BEGIN 2 */
    OLED_Init();
    OLED_Clear();
    HAL_UART_Receive_IT(&huart1, &rec1, 1);
    HAL_UART_Receive_IT(&huart2, &rec2, 1);
    HAL_ADC_Start_IT(&hadc1);


    start();
    connectIP();
    OLED_Clear();
    OLED_ShowString(1, 1, "** Start_up **", 14);
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1) {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
        printf("hello!");


//        GY30();
//        dataTran();
    }
    /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }
    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                  | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
        Error_Handler();
    }
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */
//启动函数+连WiFi
void start() {
    HAL_Delay(500);
    OLED_ShowString(2, 2, "System startup\n", 14);
    OLED_ShowString(2, 3, "...\n", 3);
    HAL_Delay(3000);
    OLED_Clear();
    OLED_ShowString(0, 0, "Start connecting to the network\n", 31);
    HAL_Delay(3000);
    OLED_Clear();
}

//usart2发送指令联网——AT指令（实验室内网）
void connectIP() {
    HAL_UART_Transmit(&huart2, "AT+CIPSTART=\"TCP\",\"192.168.2.212\",8080\r\n",
                      sizeof("AT+CIPSTART=\"TCP\",\"192.168.2.212\",8080\r\n"), 1000);
    HAL_Delay(500);
    HAL_UART_Transmit(&huart2, "AT+CIPMODE=1\r\n", sizeof("AT+CIPMODE=1\r\n"), 1000);
    HAL_Delay(500);
    HAL_UART_Transmit(&huart2, "AT+CIPSEND\r\n", sizeof("AT+CIPSEND\r\n"), 1000);
    HAL_Delay(500);
    HAL_UART_Transmit(&huart2, "Connection successful!\r\n", sizeof("Connection successful!\r\n"), 1000);
    OLED_ShowString(0, 0, "Connection successful!\r\n", 22);
    HAL_Delay(1000);
}

//usart中断方式接受数据
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
        if (rec1 == '1') {
            HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
            HAL_UART_Transmit(&huart2, "led on!\r\n", sizeof("led on!\r\n"), 10000);
            HAL_UART_Transmit(&huart1, "led on!\r\n", sizeof("led on!\r\n"), 10000);
            OLED_Clear();
            OLED_ShowString(0, 0, "led on!\r\n", 7);
        }
        if (rec1 == '0') {
            HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
            HAL_UART_Transmit(&huart2, "led off!\r\n", sizeof("led off!\r\n"), 10000);
            HAL_UART_Transmit(&huart1, "led off!\r\n", sizeof("led off!\r\n"), 10000);
            OLED_Clear();
            OLED_ShowString(0, 0, "led off!\r\n", 8);
        }
        HAL_UART_Receive_IT(&huart1, &rec1, 1);
    } else if (huart->Instance == USART2) {//从服务器端发送数据到esp8266，然后串口接收到主控板stm32
        if (rec2 == '1') {
            HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
            HAL_UART_Transmit(&huart2, "led on!\r\n", sizeof("led on!\r\n"), 10000);
            HAL_UART_Transmit(&huart1, "led on!\r\n", sizeof("led on!\r\n"), 10000);
            OLED_Clear();
            OLED_ShowString(0, 0, "led on!\r\n", 7);
        } else if (rec2 == '0') {
            HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
            HAL_UART_Transmit(&huart2, "led off!\r\n", sizeof("led off!\r\n"), 10000);
            HAL_UART_Transmit(&huart1, "led off!\r\n", sizeof("led off!\r\n"), 10000);
            OLED_Clear();
            OLED_ShowString(0, 0, "led off!\r\n", 8);
        } else if (rec2 == '2') {
            dataTran();
        }
        HAL_UART_Receive_IT(&huart2, &rec2, 1);
    }
}

//GY30测试(光照测试)
void GY30() {
    uint8_t dat[2] = {0};
    HAL_Delay(200);
    if (HAL_OK == BH1750_Send_Cmd(ONCE_H_MODE)) {
//        printf("send ok\n");
    } else {
//        printf("send fail\n");
    }
    HAL_Delay(200);
    if (HAL_OK == BH1750_Read_Dat(dat)) {
        light = BH1750_Dat_To_Lux(dat);
    } else {
        //printf("recv fail");
    }
    HAL_Delay(1000);
}

//这是中断回调函数
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
    //获取ad value
    adc_value = HAL_ADC_GetValue(&hadc1);
    //2的12次方，所以3V = 4096
    vol = (float) adc_value * 3.3 / 4096;
    //在实际测试中，ad_value 随着水分的增加，而减少
    //在干燥的空气中，ad_value 在4020左右 -> 0
    //在纯水的中的值为：ad_value 在1500左右 -> 100%
    //土壤湿度的范围为 0~100 %
    hum = -0.03968253968253968 * adc_value + 159.52380952380952;
    //限制一下上下限
    if (hum < 0) hum = 0;
    if (hum > 100) hum = 100;
    //再次启动ADC转换
    HAL_ADC_Start_IT(hadc);
}

//数据传输给网络服务器（网络调试助手）
void dataTran() {
    u_char str1[8] = "light:";
    u_char str2[6] = "\nHum:";
    printf("光照强度： %d\n", light);
    uint8_t h, l;//用来存取light的高八位和低八位
    h = light % 256;
    l = light / 256;
    printf("土壤湿度： %.2f\n", hum);
    Hum = hum;//转换为整数
    HAL_UART_Transmit(&huart2, str1, sizeof(str1), 100);
    HAL_UART_Transmit(&huart2, &l, sizeof(l), 100);
    HAL_UART_Transmit(&huart2, &h, sizeof(h), 100);
    HAL_UART_Transmit(&huart2, str2, sizeof(str2), 100);
    HAL_UART_Transmit(&huart2, &Hum, sizeof(Hum), 100);
}
/* USER CODE END 0 */


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void) {
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1) {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
      /* User can add his own implementation to report the file name and line number,
         ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

