
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "stm32l0xx_hal.h"

void Error_Handler(void);

#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA

long unsigned int millis(void);
void delay(uint32_t ms);
void setup(void);
void loop(void);

bool MY_Debug(void);
void MY_Sleep_DeInit(void);

#include "event.h"
#ifdef EVENT_TIMER
uint32_t MY_TimerMs(void);
#endif

#ifdef __cplusplus
}
#endif

#endif 
