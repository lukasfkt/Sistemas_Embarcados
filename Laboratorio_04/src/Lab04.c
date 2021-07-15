#include "system_tm4c1294.h" // CMSIS-Core
#include "driverleds.h" // device drivers
#include "cmsis_os2.h" // CMSIS-RTOS

 osThreadId_t thread1_id,thread2_id,thread3_id,thread4_id;

typedef struct {
  uint8_t led; 
  uint32_t tempo_acionamento;
} pisca_led;
  
/*void thread1(void *arg){
  uint8_t state = 0;
  
  while(1){
    state ^= LED1;
    LEDWrite(LED1, state);
    osDelay(100); 
  } // while
} // thread1
*/

void thread2(void *arg){
  uint8_t state = 0;
  uint32_t tick;
  pisca_led *pisca = (pisca_led *)arg;  

  while(1){
    tick = osKernelGetTickCount();
    
    state ^= pisca->led;
    LEDWrite(pisca->led, state);
    
    osDelayUntil(tick + pisca->tempo_acionamento); 
  } // while
} // thread2

void main(void){
  LEDInit(LED1 | LED2 | LED3 | LED4);

  osKernelInitialize();

  pisca_led led1 = {.led = LED1, .tempo_acionamento = 200};
  pisca_led led2 = {.led = LED2, .tempo_acionamento = 300};
  pisca_led led3 = {.led = LED3, .tempo_acionamento = 500};
  pisca_led led4 = {.led = LED4, .tempo_acionamento = 700};

  thread1_id = osThreadNew(thread2, &led1, NULL);
  thread2_id = osThreadNew(thread2, &led2, NULL);
  thread3_id = osThreadNew(thread2, &led3, NULL);
  thread4_id = osThreadNew(thread2, &led4, NULL);

  if(osKernelGetState() == osKernelReady)
    osKernelStart();

  while(1);
} // main

