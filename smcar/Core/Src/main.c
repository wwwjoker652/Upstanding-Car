/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "IIC.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "mpu6050.h"
#include "tb6612.h"
#include "motorenc.h"
#include "oled.h"
#include "pid.h"
#include "esp.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define RXBUFFERSIZE 256
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
float p,y,r;
char message1[50] = "";
char message2[50] = "";
char message3[50] = "";
char message4[50] = "";
char message5[50] = "";
short gx,gy,gz;
int enc_left,enc_right;
volatile int total_left = 0;
volatile int total_left2 = 0;
volatile float total_right = 0;
volatile int total_right2 = 0;
volatile float total_rightm = 0;
char RxBuffer[RXBUFFERSIZE]; 
uint8_t aRxBuffer;
uint8_t Uart2_Rx_Cnt = 0;	
char my_order[15]={0};
char receive_flag=0;
uint8_t my_test_v = 0;
int pre_tvalue = 0;
uint8_t count = 0;
uint8_t state = 0;
uint8_t flag = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void read(void);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void mode1(void);
void mode2(void);
void mode3(void);
void mode4(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

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
  MX_USART3_UART_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM4_Init();
  MX_I2C1_Init();
  MX_TIM3_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
	OLED_Init();
	mpu_init();
	mpu_dmp_init();	
  OLED_Clear();
	HAL_UART_Receive_IT(&huart2, (uint8_t *)&aRxBuffer, 1);
	esp8266_start_trans();
	tb6612_init();
	enc_init();
	while(mpu_dmp_get_data(&p,&r,&y));
	OLED_ShowString(1,1,"OK",8,1);
	OLED_Refresh();
	motorcontrol(0,0);
	HAL_TIM_Base_Start_IT(&htim3);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
//		while(mpu_dmp_get_data(&p,&r,&y));
//		sprintf(message2,"pitch:%3f\n",p);
//    OLED_ShowString(1,1,(uint8_t*)message2,8,1);
//    sprintf((char*)message3,"roll:%3f\n",r);
//    OLED_ShowString(1,10,(uint8_t*)message3,8,1);
//    sprintf((char*)message4,"yaw:%3f\n\n",y);
//    OLED_ShowString(1,20,(uint8_t*)message4,8,1);
//		OLED_Refresh();
//		HAL_Delay(50);
//		OLED_Clear();
//		read();
//		sprintf(message2,"left:%d",enc_left);
//		sprintf((char*)message3,"right:%d",enc_right);
//		sprintf(message4,"right:%.2f",total_right);
//		sprintf((char*)message1,"right2:%d",total_right2);
//		sprintf((char*)message5,"rightm:%d",total_right2);
//		OLED_ShowString(1,1,(uint8_t*)message2,8,1);
//		OLED_ShowString(1,10,(uint8_t*)message3,8,1);
//		OLED_ShowString(1,20,(uint8_t*)message4,8,1);
//		OLED_ShowString(1,30,(uint8_t*)message1,8,1);
//		OLED_ShowString(1,40,(uint8_t*)message5,8,1);
//		OLED_Refresh();
//		HAL_Delay(50);
//		control();
		if(count >= 50){
			Target_Speed = 0;
		}
//		sprintf(message2,"left:%d",Enc_left);
//    OLED_ShowString(1,1,(uint8_t*)message2,8,1);
//    sprintf((char*)message3,"right:%d",Enc_right);
//    OLED_ShowString(1,10,(uint8_t*)message3,8,1);
//		OLED_Refresh();
//		HAL_Delay(10);
		if(receive_flag)
			{
				receive_flag=0;
				if(strstr((const char*)my_order,(const char*)"<speed 1>")) //??on,LED0?
				{
					Uart2_Rx_Cnt=0;
					memset(RxBuffer,0x00,sizeof(RxBuffer)); //????	
					memset(RxBuffer,0x00,sizeof(my_order)); //????	
					pre_tvalue = 6;
				}
				else if(strstr((const char*)my_order,(const char*)"<speed 2>"))
				{
					Uart2_Rx_Cnt=0;
					memset(RxBuffer,0x00,sizeof(RxBuffer)); //????	
					memset(RxBuffer,0x00,sizeof(my_order)); //????	
					pre_tvalue = 12;
				}
				else if(strstr((const char*)my_order,(const char*)"<speed 3>"))
				{
					Uart2_Rx_Cnt=0;
					memset(RxBuffer,0x00,sizeof(RxBuffer)); //????	
					memset(RxBuffer,0x00,sizeof(my_order)); //????	
					pre_tvalue = 18;
				}
				else if(strstr((const char*)my_order,(const char*)"<speed 4>"))
				{
					Uart2_Rx_Cnt=0;
					memset(RxBuffer,0x00,sizeof(RxBuffer)); //????	
					memset(RxBuffer,0x00,sizeof(my_order)); //????	
					pre_tvalue = 24;
				}
				else if(strstr((const char*)my_order,(const char*)"<speed 5>"))
				{
					Uart2_Rx_Cnt=0;
					memset(RxBuffer,0x00,sizeof(RxBuffer)); //????	
					memset(RxBuffer,0x00,sizeof(my_order)); //????	
					pre_tvalue = 30;
				}
				else if(strstr((const char*)my_order,(const char*)"<position +1>"))
				{
					Uart2_Rx_Cnt=0;
					memset(RxBuffer,0x00,sizeof(RxBuffer)); //????	
					memset(RxBuffer,0x00,sizeof(my_order)); //????	
					Target_Speed = pre_tvalue;
					count = 0;
				}
				else if(strstr((const char*)my_order,(const char*)"<position -1>"))
				{
					Uart2_Rx_Cnt=0;
					memset(RxBuffer,0x00,sizeof(RxBuffer)); //????	
					memset(RxBuffer,0x00,sizeof(my_order)); //????	
					Target_Speed = -pre_tvalue;
					count = 0;
				}
				else if(strstr((const char*)my_order,(const char*)"<turn +1>"))
				{
					Uart2_Rx_Cnt=0;
					memset(RxBuffer,0x00,sizeof(RxBuffer)); //????	
					memset(RxBuffer,0x00,sizeof(my_order)); //????	
					Target_turn += 20;
					if(Target_turn >= 50){
						Target_turn = 50;
					}
					count = 0;
					uint32_t tick1 = HAL_GetTick();
					uint32_t tick2 = HAL_GetTick();
					while(tick2 - tick1 <= 200){
						tick2 = HAL_GetTick();
					}
					Target_turn = 0;
				}
				else if(strstr((const char*)my_order,(const char*)"<turn -1>"))
				{
					Uart2_Rx_Cnt=0;
					memset(RxBuffer,0x00,sizeof(RxBuffer)); //????	
					memset(RxBuffer,0x00,sizeof(my_order)); //????	
					Target_turn -= 20;
					if(Target_turn <= -50){
						Target_turn = -50;
					}
					count = 0;
					uint32_t tick1 = HAL_GetTick();
					uint32_t tick2 = HAL_GetTick();
					while(tick2 - tick1 <= 200){
						tick2 = HAL_GetTick();
					}
					Target_turn = 0;
				}
				else if(strstr((const char*)my_order,(const char*)"<mode 1>"))
				{
					Uart2_Rx_Cnt=0;
					memset(RxBuffer,0x00,sizeof(RxBuffer)); //????	
					memset(RxBuffer,0x00,sizeof(my_order)); //????	
					mode1();
				}
				else if(strstr((const char*)my_order,(const char*)"<mode 2>"))
				{
					Uart2_Rx_Cnt=0;
					memset(RxBuffer,0x00,sizeof(RxBuffer)); //????	
					memset(RxBuffer,0x00,sizeof(my_order)); //????	
					mode2();
				}
				else if(strstr((const char*)my_order,(const char*)"<restart>"))
				{
					Uart2_Rx_Cnt=0;
					memset(RxBuffer,0x00,sizeof(RxBuffer)); //????	
					memset(RxBuffer,0x00,sizeof(my_order)); //????	
					NVIC_SystemReset();
				}
				else if(strstr((const char*)my_order,(const char*)"<state>"))
				{
					Uart2_Rx_Cnt=0;
					memset(RxBuffer,0x00,sizeof(RxBuffer)); //????	
					memset(RxBuffer,0x00,sizeof(my_order)); //????	
					esp8266_send_cmd("AT+CIPSEND=0,3","OK",50);
					esp8266_send_cmd("Yes","OK",50);
				}
				else if(strstr((const char*)my_order,(const char*)"<mode 3>"))
				{
					Uart2_Rx_Cnt=0;
					memset(RxBuffer,0x00,sizeof(RxBuffer)); //????	
					memset(RxBuffer,0x00,sizeof(my_order)); //????	
					mode3();
				}
				else if(strstr((const char*)my_order,(const char*)"<mode 4>"))
				{
					Uart2_Rx_Cnt=0;
					memset(RxBuffer,0x00,sizeof(RxBuffer)); //????	
					memset(RxBuffer,0x00,sizeof(my_order)); //????	
					mode4();
				}
				else{
					Uart2_Rx_Cnt=0;
					memset(RxBuffer,0x00,sizeof(RxBuffer)); //????	
					memset(RxBuffer,0x00,sizeof(my_order)); //????	
				}
					
			}
//			if(total_right2 >= 2400 || total_right2 <= -2400){
//				Target_Speed = 0;
//			}

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

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
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void read(void){
//	enc_left = read_enc(&htim2);
//	enc_right = -read_enc(&htim4);
	total_right2 += Enc_right;
	total_left2 += Enc_left;
//	total_left = total_left2 / 1320;
//	total_right = (float)total_right2 / 1320;
//	total_rightm = total_right * 0.22;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM3) {
			control();
			count++;
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(huart);
  /* NOTE: This function Should not be modified, when the callback is needed,
           the HAL_UART_TxCpltCallback could be implemented in the user file
   */
 
	if(Uart2_Rx_Cnt >= 255)  //????
	{
		Uart2_Rx_Cnt = 0;
		memset(RxBuffer,0x00,sizeof(RxBuffer));	        
	}
	else
	{
		static uint8_t Uart2_count=0;
		
		RxBuffer[Uart2_Rx_Cnt] = aRxBuffer;   //??????
		if(receive_flag==0)
		{
		if(RxBuffer[Uart2_Rx_Cnt-Uart2_count]=='<')
		{	
			Uart2_count++;
		if((RxBuffer[Uart2_Rx_Cnt]=='>')||Uart2_count>=14)
		{
			uint8_t My_i=0;
		for(int i=Uart2_Rx_Cnt-Uart2_count+1;i<Uart2_Rx_Cnt+1;i++)
			my_order[My_i++]=RxBuffer[i];
			receive_flag=1;
			Uart2_count=0;
		}
	}
	}
		Uart2_Rx_Cnt++;
	}
	
	HAL_UART_Receive_IT(&huart2, (uint8_t *)&aRxBuffer, 1);   //???????
}

void mode1(void){
	Target_Speed = 20;
	uint32_t tick1 = HAL_GetTick();
	uint32_t tick2 = HAL_GetTick();
	while(tick2 - tick1 <= 500){
		tick2 = HAL_GetTick();
	}
	Target_Speed = 40;
	while(1){
		read();
		sprintf((char*)message1,"right2:%d",total_right2);
		OLED_ShowString(1,30,(uint8_t*)message1,8,1);
		OLED_Refresh();
		if(total_right2 >= 4200 || total_right2 <= -4200){
			Target_Speed = 0;
			break;
		}
	}
}

void mode2(void){
	Target_Speed = 20;
	uint32_t tick1 = HAL_GetTick();
	uint32_t tick2 = HAL_GetTick();
	while(tick2 - tick1 <= 500){
		tick2 = HAL_GetTick();
	}
	Target_Speed = 40;
	tick1 = HAL_GetTick();
	tick2 = HAL_GetTick();
	while(tick2 - tick1 <= 500){
		tick2 = HAL_GetTick();
	}
	Target_Speed = 60;
	tick1 = HAL_GetTick();
	tick2 = HAL_GetTick();
	while(tick2 - tick1 <= 1000){
		tick2 = HAL_GetTick();
	}
	Target_Speed = 75;
	Target_turn = 28;
	while(1){
				if(receive_flag)
			{
				receive_flag=0;
				if(strstr((const char*)my_order,(const char*)"<exit>")) //??on,LED0?
				{
					Uart2_Rx_Cnt=0;
					memset(RxBuffer,0x00,sizeof(RxBuffer)); //????	
					memset(RxBuffer,0x00,sizeof(my_order)); //????	
					Target_Speed = 0;
					Target_turn = 0;
					break;
				}
			}
	}
}

//void mode3(void){
//	Target_Speed = 5;
//	OLED_ShowString(1,30,"MODE3",8,1);
//	OLED_Refresh();
//	uint32_t a = 0;
//	while(1){
//		a = HAL_GetTick();
//	}
//}

void mode3(void){
	OLED_ShowString(1,30,"MODE3",8,1);
	OLED_Refresh();
	while(1){
		uint8_t flag = 0;
		Target_Speed = 20;
		uint32_t tick1 = HAL_GetTick();
		uint32_t tick2 = HAL_GetTick();
		while(tick2 - tick1 <= 3000){
			tick2 = HAL_GetTick();
			if(receive_flag)
			{
				receive_flag=0;
				if(strstr((const char*)my_order,(const char*)"<exit>")) //??on,LED0?
				{
					Uart2_Rx_Cnt=0;
					memset(RxBuffer,0x00,sizeof(RxBuffer)); //????	
					memset(RxBuffer,0x00,sizeof(my_order)); //????	
					Target_Speed = 0;
					flag = 1;
					break;
				}
			}
		}
		Target_Speed = 0;
		tick1 = HAL_GetTick();
		tick2 = HAL_GetTick();
		while(tick2 - tick1 <= 3000){
			tick2 = HAL_GetTick();
			if(receive_flag)
			{
				receive_flag=0;
				if(strstr((const char*)my_order,(const char*)"<exit>")) //??on,LED0?
				{
					Uart2_Rx_Cnt=0;
					memset(RxBuffer,0x00,sizeof(RxBuffer)); //????	
					memset(RxBuffer,0x00,sizeof(my_order)); //????	
					Target_Speed = 0;
					flag = 1;
					break;
				}
			}
		}
		if(flag == 1){
			flag = 0;
			break;
		}
		Target_Speed = 25;
		tick1 = HAL_GetTick();
		tick2 = HAL_GetTick();
		while(tick2 - tick1 <= 3000){
			tick2 = HAL_GetTick();
			if(receive_flag)
			{
				receive_flag=0;
				if(strstr((const char*)my_order,(const char*)"<exit>")) //??on,LED0?
				{
					Uart2_Rx_Cnt=0;
					memset(RxBuffer,0x00,sizeof(RxBuffer)); //????	
					memset(RxBuffer,0x00,sizeof(my_order)); //????	
					Target_Speed = 0;
					flag = 1;
					break;
				}
			}
		}
		Target_Speed = 0;
		tick1 = HAL_GetTick();
		tick2 = HAL_GetTick();
		while(tick2 - tick1 <= 3000){
			tick2 = HAL_GetTick();
			if(receive_flag)
			{
				receive_flag=0;
				if(strstr((const char*)my_order,(const char*)"<exit>")) //??on,LED0?
				{
					Uart2_Rx_Cnt=0;
					memset(RxBuffer,0x00,sizeof(RxBuffer)); //????	
					memset(RxBuffer,0x00,sizeof(my_order)); //????	
					Target_Speed = 0;
					flag = 1;
					break;
				}
			}
		}
		if(flag == 1){
			flag = 0;
			break;
		}
		Target_Speed = 30;
		tick1 = HAL_GetTick();
		tick2 = HAL_GetTick();
		while(tick2 - tick1 <= 3000){
			tick2 = HAL_GetTick();
			if(receive_flag)
			{
				receive_flag=0;
				if(strstr((const char*)my_order,(const char*)"<exit>")) //??on,LED0?
				{
					Uart2_Rx_Cnt=0;
					memset(RxBuffer,0x00,sizeof(RxBuffer)); //????	
					memset(RxBuffer,0x00,sizeof(my_order)); //????	
					Target_Speed = 0;
					flag = 1;
					break;
				}
			}
		}
		Target_Speed = 0;
		tick1 = HAL_GetTick();
		tick2 = HAL_GetTick();
		while(tick2 - tick1 <= 3000){
			tick2 = HAL_GetTick();
			if(receive_flag)
			{
				receive_flag=0;
				if(strstr((const char*)my_order,(const char*)"<exit>")) //??on,LED0?
				{
					Uart2_Rx_Cnt=0;
					memset(RxBuffer,0x00,sizeof(RxBuffer)); //????	
					memset(RxBuffer,0x00,sizeof(my_order)); //????	
					Target_Speed = 0;
					flag = 1;
					break;
				}
			}
		}
		if(flag == 1){
			flag = 0;
			break;
		}
	}
}

void mode4(void){
	OLED_ShowString(1,30,"MODE4",8,1);
	OLED_Refresh();
	Target_turn = 30;
	while(1){
		uint8_t flag = 0;
		Target_Speed = 20;
		uint32_t tick1 = HAL_GetTick();
		uint32_t tick2 = HAL_GetTick();
		while(tick2 - tick1 <= 3000){
			tick2 = HAL_GetTick();
			if(receive_flag)
			{
				receive_flag=0;
				if(strstr((const char*)my_order,(const char*)"<exit>")) //??on,LED0?
				{
					Uart2_Rx_Cnt=0;
					memset(RxBuffer,0x00,sizeof(RxBuffer)); //????	
					memset(RxBuffer,0x00,sizeof(my_order)); //????	
					Target_Speed = 0;
					flag = 1;
					break;
				}
			}
		}
		if(flag == 1){
			flag = 0;
			break;
		}
		Target_Speed = 25;
		tick1 = HAL_GetTick();
		tick2 = HAL_GetTick();
		while(tick2 - tick1 <= 3000){
			tick2 = HAL_GetTick();
			if(receive_flag)
			{
				receive_flag=0;
				if(strstr((const char*)my_order,(const char*)"<exit>")) //??on,LED0?
				{
					Uart2_Rx_Cnt=0;
					memset(RxBuffer,0x00,sizeof(RxBuffer)); //????	
					memset(RxBuffer,0x00,sizeof(my_order)); //????	
					Target_Speed = 0;
					flag = 1;
					break;
				}
			}
		}
		if(flag == 1){
			flag = 0;
			break;
		}
		Target_Speed = 30;
		tick1 = HAL_GetTick();
		tick2 = HAL_GetTick();
		while(tick2 - tick1 <= 3000){
			tick2 = HAL_GetTick();
			if(receive_flag)
			{
				receive_flag=0;
				if(strstr((const char*)my_order,(const char*)"<exit>")) //??on,LED0?
				{
					Uart2_Rx_Cnt=0;
					memset(RxBuffer,0x00,sizeof(RxBuffer)); //????	
					memset(RxBuffer,0x00,sizeof(my_order)); //????	
					Target_Speed = 0;
					flag = 1;
					break;
				}
			}
		}
		if(flag == 1){
			flag = 0;
			break;
		}
	}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
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
