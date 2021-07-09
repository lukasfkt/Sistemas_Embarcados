#include <stdint.h>
#include <stdbool.h>
// includes da biblioteca driverlib
#include "driverlib/systick.h"
#include "driverleds.h" // Projects/drivers

// MEF com apenas 2 estados e 1 evento temporal que alterna entre eles
// Seleção por evento

typedef enum {Estado_000 =0,
Estado_001 = LED3,
Estado_011 = LED2 | LED3,
Estado_010 = LED2,
Estado_110 = LED1 | LED2,
Estado_111 = LED1 | LED2 | LED3,
Estado_101 = LED1 | LED3,
Estado_100 = LED1,
 } state_t;

volatile uint8_t Evento = 0;

void SysTick_Handler(void){
  Evento = 1;
} // SysTick_Handler

void main(void){
  static state_t Estado = Estado_000; // estado inicial da MEF
  
  LEDInit(LED1 | LED2 | LED3);
  SysTickPeriodSet(12000000); // f = 1Hz para clock = 24MHz
  SysTickIntEnable();
  SysTickEnable();

  while(1){
    if(Evento){
      Evento = 0;
      switch(Estado){
        case Estado_000:
          LEDOn(Estado_000);
          LEDOff(~Estado_000);
          Estado = Estado_001;
          break;
        case Estado_001:
          LEDOn(Estado_001);
          LEDOff(~Estado_001);
          Estado = Estado_011;
          break;
        case Estado_011:
          LEDOn(Estado_011);
          LEDOff(~Estado_011);
          Estado = Estado_010;
          break;
        case Estado_010:
          LEDOn(Estado_010);
          LEDOff(~Estado_010);
          Estado = Estado_110;
          break;
        case Estado_110:
          LEDOn(Estado_110);
          LEDOff(~Estado_110);
          Estado = Estado_111;
          break;
        case Estado_111:
          LEDOn(Estado_111);
          LEDOff(~Estado_111);
          Estado = Estado_101;
          break;
        case Estado_101:
          LEDOn(Estado_101);
          LEDOff(~Estado_101);
          Estado = Estado_100;
          break;
        case Estado_100:
          LEDOn(Estado_100);
          LEDOff(~Estado_100);
          Estado = Estado_000;
          break;
      }
    } // if
  } // while
} // main
