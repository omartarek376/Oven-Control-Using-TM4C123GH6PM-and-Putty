#include <string.h>
#include <stdint.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "semphr.h"
#include <stdint.h>
#include <stdbool.h>
#include "main.h"
#include "tm4c123gh6pm.h"


#define LCD_DATA_DATA GPIO_PORTB_DATA_R
#define LCD_CTRL_DATA GPIO_PORTA_DATA_R
#define RS (1U<<6) 
#define EN (1U<<7) 

void LCD_command(unsigned char command);
void LCD_start(void);
void LCD_data(unsigned char data);
void LCD_clear (void);
void LCD_line(uint8_t line);
void LCD_display(char* name);
void toString (int tim, char text []);
void printchar(char x);
void print(char* string);
void delayMs(int n);
void delayUs(int n);

