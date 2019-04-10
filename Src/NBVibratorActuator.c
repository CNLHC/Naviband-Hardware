
#include "NBConfig.h"
#include "NBTask/NBVibratorActuator.h"

void NBVibratorActuator(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin,osMessageQId qCMD){
    while(1){
       osEvent evt = osMessageGet(qCMD,osWaitForever);
       if(evt.status==osEventMessage){
           HAL_GPIO_WritePin(GPIOx,GPIO_Pin,GPIO_PIN_SET);
           osDelay(1000);
           HAL_GPIO_WritePin(GPIOx,GPIO_Pin,GPIO_PIN_RESET);
       }
    }
}
