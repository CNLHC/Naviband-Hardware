#include "gpio.h"
#include "cmsis_os.h"

#ifndef NB_TASK_VIBRATOR_ACTUATOR_H
#define NB_TASK_VIBRATOR_ACTUATOR_H

void NBVibratorActuator(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin,osMessageQId qCMD);
#endif
