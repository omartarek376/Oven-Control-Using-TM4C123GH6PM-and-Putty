#include "Dio.h"

void GPIO_init(){
//GPIO initialization A,D,E Ports
	SYSCTL_RCGCGPIO_R |= 0x00000019;	
  GPIO_PORTD_LOCK_R = 0x4C4F434B;
  GPIO_PORTD_CR_R = 0xFF;      
  GPIO_PORTD_DIR_R = 0xFF;      
  GPIO_PORTD_DEN_R = 0xFF;     
	GPIO_PORTE_LOCK_R = 0x4C4F434B;
  GPIO_PORTE_CR_R = 0x2F;       
  GPIO_PORTE_DIR_R = 0x2F;      
  GPIO_PORTE_DEN_R = 0x2F;      
	GPIO_PORTA_LOCK_R = 0x4C4F434B;
	//pins 0 & 1 alternative function 
	GPIO_PORTA_AFSEL_R |= (1<<0 | 1<<1);         
	//pin alternative function
	GPIO_PORTA_PCTL_R  |= (1<<(0*4) | 1<<(1*4));      
	GPIO_PORTA_DEN_R   |= (1<<0 | 1<<1);   
	
	// UART initialization
	SYSCTL_RCGCUART_R |= 1<<0 ;			
	delayUs(1);
	UART0_CTL_R &=~ 1<<0;     
	UART0_IBRD_R=8;
	UART0_FBRD_R=44;
	UART0_LCRH_R = (UART_LCRH_WLEN_8 | UART_LCRH_FEN);
	UART0_CC_R =0x05;
	UART0_CTL_R=0x0301;
		
	LCD_start();
}
