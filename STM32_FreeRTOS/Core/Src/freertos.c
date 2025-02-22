/* USER CODE BEGIN Header */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "queue.h"
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
QueueHandle_t distanceQueue;  // Declare the queue handle
/* USER CODE END Variables */
/* Definitions for TaskMainApp */
osThreadId_t TaskMainAppHandle;
const osThreadAttr_t TaskMainApp_attributes = {
  .name = "TaskMainApp",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for TaskUltrasonic */
osThreadId_t TaskUltrasonicHandle;
const osThreadAttr_t TaskUltrasonic_attributes = {
  .name = "TaskUltrasonic",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for TaskOLED */
osThreadId_t TaskOLEDHandle;
const osThreadAttr_t TaskOLED_attributes = {
  .name = "TaskOLED",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for TaskLoRa */
osThreadId_t TaskLoRaHandle;
const osThreadAttr_t TaskLoRa_attributes = {
  .name = "TaskLoRa",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for TaskServoMotor */
osThreadId_t TaskServoMotorHandle;
const osThreadAttr_t TaskServoMotor_attributes = {
  .name = "TaskServoMotor",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
/* USER CODE END FunctionPrototypes */

void main_application(void *argument);
void Task_Ultrasonic_Handler(void *argument);
void Task_OLED_Handler(void *argument);
void Task_LoRa_Handler(void *argument);
void Task_ServoMotor_Handler(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  // Create the queue with space for 5 uint32_t values



  distanceQueue = xQueueCreate(10, sizeof(uint32_t));
  if (distanceQueue == NULL) {
      printf("\r Queue creation failed!\n");
  } else {
      printf("\r Queue created successfully.\n");
  }



  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of TaskMainApp */
  TaskMainAppHandle = osThreadNew(main_application, NULL, &TaskMainApp_attributes);

  /* creation of TaskUltrasonic */
  TaskUltrasonicHandle = osThreadNew(Task_Ultrasonic_Handler, NULL, &TaskUltrasonic_attributes);

  /* creation of TaskOLED */
  TaskOLEDHandle = osThreadNew(Task_OLED_Handler, NULL, &TaskOLED_attributes);

  /* creation of TaskLoRa */
  TaskLoRaHandle = osThreadNew(Task_LoRa_Handler, NULL, &TaskLoRa_attributes);

  /* creation of TaskServoMotor */
  TaskServoMotorHandle = osThreadNew(Task_ServoMotor_Handler, NULL, &TaskServoMotor_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_main_application */
/* USER CODE END Header_main_application */
__weak void main_application(void *argument)
{
  /* USER CODE BEGIN main_application */
  /* USER CODE END main_application */
}

/* USER CODE BEGIN Header_Task_Ultrasonic_Handler */
/* USER CODE END Header_Task_Ultrasonic_Handler */
__weak void Task_Ultrasonic_Handler(void *argument)
{
  /* USER CODE BEGIN Task_Ultrasonic_Handler */
  /* USER CODE END Task_Ultrasonic_Handler */
}

/* USER CODE BEGIN Header_Task_OLED_Handler */
/* USER CODE END Header_Task_OLED_Handler */
__weak void Task_OLED_Handler(void *argument)
{
  /* USER CODE BEGIN Task_OLED_Handler */
  /* USER CODE END Task_OLED_Handler */
}

/* USER CODE BEGIN Header_Task_LoRa_Handler */
/**
* @brief Function implementing the TaskLoRa thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Task_LoRa_Handler */
__weak void Task_LoRa_Handler(void *argument)
{
  /* USER CODE BEGIN Task_LoRa_Handler */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Task_LoRa_Handler */
}

/* USER CODE BEGIN Header_Task_ServoMotor_Handler */
/**
* @brief Function implementing the TaskServoMotor thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Task_ServoMotor_Handler */
__weak void Task_ServoMotor_Handler(void *argument)
{
  /* USER CODE BEGIN Task_ServoMotor_Handler */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Task_ServoMotor_Handler */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
/* USER CODE END Application */

