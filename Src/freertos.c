/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */     
#include "NBConfig.h"
#include "NBTask/NBTaskBTHDaemon.h"

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
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId BlinkSysLEDHandle;
uint32_t BlinkSysLEDBuffer[ 128 ];
osStaticThreadDef_t BlinkSysLEDControlBlock;
osThreadId BTHDaemonHandle;
uint32_t BTHDaemonBuffer[ 512 ];
osStaticThreadDef_t BTHDaemonControlBlock;
osMessageQId qVibratorLCMDHandle;
uint8_t qVibratorLCMDBuffer[ 16 * sizeof( uint32_t ) ];
osStaticMessageQDef_t qVibratorLCMDControlBlock;
osMessageQId qVibratorRCMDHandle;
uint8_t qVibratorRCMDBuffer[ 16 * sizeof( uint32_t ) ];
osStaticMessageQDef_t qVibratorRCMDControlBlock;
osMessageQId qBTHSerialReadHandle;
uint8_t qBTHSerialReadBuffer[ 32 * sizeof( uint8_t ) ];
osStaticMessageQDef_t qBTHSerialReadControlBlock;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
   
/* USER CODE END FunctionPrototypes */

void osthBlinkSysLED(void const * argument);
void osthBTHDaemon(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* GetTimerTaskMemory prototype (linked to static allocation support) */
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];
  
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}                   
/* USER CODE END GET_IDLE_TASK_MEMORY */

/* USER CODE BEGIN GET_TIMER_TASK_MEMORY */
static StaticTask_t xTimerTaskTCBBuffer;
static StackType_t xTimerStack[configTIMER_TASK_STACK_DEPTH];
  
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize )  
{
  *ppxTimerTaskTCBBuffer = &xTimerTaskTCBBuffer;
  *ppxTimerTaskStackBuffer = &xTimerStack[0];
  *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
  /* place for user code */
}                   
/* USER CODE END GET_TIMER_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
       
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* definition and creation of qVibratorLCMD */
  osMessageQStaticDef(qVibratorLCMD, 16, uint32_t, qVibratorLCMDBuffer, &qVibratorLCMDControlBlock);
  qVibratorLCMDHandle = osMessageCreate(osMessageQ(qVibratorLCMD), NULL);

  /* definition and creation of qVibratorRCMD */
  osMessageQStaticDef(qVibratorRCMD, 16, uint32_t, qVibratorRCMDBuffer, &qVibratorRCMDControlBlock);
  qVibratorRCMDHandle = osMessageCreate(osMessageQ(qVibratorRCMD), NULL);

  /* definition and creation of qBTHSerialRead */
  osMessageQStaticDef(qBTHSerialRead, 32, uint8_t, qBTHSerialReadBuffer, &qBTHSerialReadControlBlock);
  qBTHSerialReadHandle = osMessageCreate(osMessageQ(qBTHSerialRead), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of BlinkSysLED */
  osThreadStaticDef(BlinkSysLED, osthBlinkSysLED, osPriorityNormal, 0, 128, BlinkSysLEDBuffer, &BlinkSysLEDControlBlock);
  BlinkSysLEDHandle = osThreadCreate(osThread(BlinkSysLED), (void*) &SYSLED_INTERVAL);

  /* definition and creation of BTHDaemon */
  osThreadStaticDef(BTHDaemon, osthBTHDaemon, osPriorityBelowNormal, 0, 512, BTHDaemonBuffer, &BTHDaemonControlBlock);
  BTHDaemonHandle = osThreadCreate(osThread(BTHDaemon), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_osthBlinkSysLED */
/**
  * @brief  Function implementing the BlinkSysLED thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_osthBlinkSysLED */
void osthBlinkSysLED(void const * argument)
{

  /* USER CODE BEGIN osthBlinkSysLED */
  uint32_t  Interval = *(uint32_t *)argument;

  /* Infinite loop */
  for(;;)
  {
      HAL_GPIO_WritePin(SysLED_GPIO_Port,SysLED_Pin,GPIO_PIN_SET);
      osDelay(Interval);
      HAL_GPIO_WritePin(SysLED_GPIO_Port,SysLED_Pin,GPIO_PIN_RESET);
      osDelay(Interval);
  }
  /* USER CODE END osthBlinkSysLED */
}

/* USER CODE BEGIN Header_osthBTHDaemon */
/**
* @brief Function implementing the BTHDaemon thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_osthBTHDaemon */
void osthBTHDaemon(void const * argument)
{
  /* USER CODE BEGIN osthBTHDaemon */
  /* Infinite loop */
  //to integra with CUBEMX, implement infinite loop in NBTask
  NBBTHDaemonEntry(argument);
  /* USER CODE END osthBTHDaemon */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
     
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
