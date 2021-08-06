#include "system_tm4c1294.h" // CMSIS-Core
#include "driverleds.h" // device drivers
#include "cmsis_os2.h" // CMSIS-RTOS
#include "driverbuttons.h"

#define TROCA_LED 0x01
#define INCREMENTA_PERIODO 0x02
#define PWM_PERIODO 10U

osThreadId_t controladora_id, aciona_led[4];

osMutexId_t mutex_led_id;

typedef struct{
  uint8_t led;
  uint8_t periodo;
  osMessageQueueId_t pwm_queue_id;
}led_pwm_t;

led_pwm_t leds[] = {
  {.led = LED1,.periodo = PWM_PERIODO},
  {.led = LED2,.periodo = PWM_PERIODO},
  {.led = LED3,.periodo = PWM_PERIODO},
  {.led = LED4,.periodo = PWM_PERIODO},
}; 

void GPIOJ_Handler(void){
  if ((~ButtonRead(USW1) & USW1) == USW1)
  {
    ButtonIntClear(USW1);
    osThreadFlagsSet(controladora_id, TROCA_LED);
  }
  if ((~ButtonRead(USW2) & USW2) == USW2)
  {
    ButtonIntClear(USW2);
    osThreadFlagsSet(controladora_id, INCREMENTA_PERIODO);
  }
} 

void SwitchOn(uint8_t led) {
  osMutexAcquire(mutex_led_id, osWaitForever);
  LEDOn(led);
  osMutexRelease(mutex_led_id);
}
void SwitchOff(uint8_t led) {
  osMutexAcquire(mutex_led_id, osWaitForever);
  LEDOff(led);
  osMutexRelease(mutex_led_id);
}
  
void controladora(void *arg)
{
  uint8_t ret = 0, count = 0, periodo[4]={10U,10U,10U,10U};
  osMessageQueueId_t msg_id = leds[0].pwm_queue_id;
  
  while(1)
  {
    ret = (uint8_t)osThreadFlagsWait(TROCA_LED | INCREMENTA_PERIODO, osFlagsWaitAny, osWaitForever);
    if ((ret & TROCA_LED) == TROCA_LED)
    {
      count++;
      if(count >= 4) count=0;
      
      switch (count)
      {
        case 0:
          msg_id = leds[0].pwm_queue_id;
          // Faz o Led piscar so para saber qual LED estamos lidando
          SwitchOn(leds[0].led);
          osDelay(100);
          SwitchOff(leds[0].led);
        break;
        case 1:
          msg_id = leds[1].pwm_queue_id;
          // Faz o Led piscar so para saber qual LED estamos lidando
          SwitchOn(leds[1].led);
          osDelay(100);
          SwitchOff(leds[1].led);
        break;
        case 2:
          msg_id = leds[2].pwm_queue_id;
          // Faz o Led piscar so para saber qual LED estamos lidando
          SwitchOn(leds[2].led);
          osDelay(100);
          SwitchOff(leds[2].led);
        break;
        case 3:
          msg_id = leds[3].pwm_queue_id;
          // Faz o Led piscar so para saber qual LED estamos lidando
          SwitchOn(leds[3].led);
          osDelay(100);
          SwitchOff(leds[3].led);
        break;
        default:
        break;
      }      
    }
    if ((ret & INCREMENTA_PERIODO) == INCREMENTA_PERIODO)
    {
      periodo[count] += 10U;
      if (periodo[count] > 100U)  periodo[count] = 10U;
      osMessageQueuePut(msg_id, &periodo[count], 0, osWaitForever);
    }
  }
}

void acionadora(void *arg)
{
  led_pwm_t *led_pwm = (led_pwm_t *)arg;
  uint8_t state = 0;
  uint32_t tick = 0;
  
  while(1){
    osMutexAcquire(mutex_led_id, osWaitForever);
    tick = osKernelGetTickCount(); 
    state ^= led_pwm->led;
    LEDWrite(led_pwm->led, state);
    osMutexRelease(mutex_led_id);
    osDelayUntil(tick + (led_pwm->periodo * PWM_PERIODO)/100);
    
    osMutexAcquire(mutex_led_id, osWaitForever);
    tick = osKernelGetTickCount(); 
    state ^= led_pwm->led;
    LEDWrite(led_pwm->led, state);
    osMutexRelease(mutex_led_id);
    osDelayUntil(tick + ((100 - led_pwm->periodo) * PWM_PERIODO)/100); 
    
    osMessageQueueGet(led_pwm->pwm_queue_id, &led_pwm->periodo, NULL, 0); 
  } // while   
}

void main(void){
  SystemInit();
  
  LEDInit(LED1 | LED2 | LED3 | LED4);
  ButtonInit(USW1 | USW2);
  ButtonIntEnable(USW1 | USW2);
  
  osKernelInitialize();
  
  mutex_led_id = osMutexNew(NULL);
  
  controladora_id = osThreadNew(controladora, NULL, NULL);
  
  for(uint8_t i=0;i<4;i++){
   leds[i].pwm_queue_id = osMessageQueueNew(10, sizeof(uint8_t), NULL);
   aciona_led[i] = osThreadNew(acionadora, &leds[i], NULL);
  }
    
  if(osKernelGetState() == osKernelReady)
    osKernelStart();

  while(1);
} 
                 