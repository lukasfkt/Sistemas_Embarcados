#include <stdint.h>
#include <stdbool.h>
// includes da biblioteca driverlib
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"

extern uint32_t SystemCoreClock;

//uint8_t LED_D1 = 0;

//void SysTick_Handler(void){
  //LED_D1 ^= GPIO_PIN_1; // Troca estado do LED D1
  //GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_1, LED_D1); // Acende ou apaga LED D1
//} // SysTick_Handler

void main(void){
  uint32_t frequencia = SystemCoreClock;
  uint8_t Status = 0;

  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); // Habilita GPIO F (LED D3 = PF4, LED D4 = PF0)
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)); // Aguarda final da habilitação
    
  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4); // LEDs D3 e D4 como saída
  GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4, 0); // LEDs D3 e D4 apagados
  GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4, GPIO_STRENGTH_12MA, GPIO_PIN_TYPE_STD);

  //SysTickIntEnable();
  //SysTickEnable();

  while(1){
    Status ^= GPIO_PIN_0; // Troca estado do LED 4
      GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_0, Status); // Acende ou desliga o LED D4
      for(uint32_t i=0;i<frequencia/4;i++); //Isso da um delay de 1s.
  } 
} 