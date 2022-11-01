#include "LCD_config.h"
 // LCD initializations
void LCD_command(unsigned char command)
{
 // Registet Select = 0 
// Read/Write = 0 
    LCD_CTRL_DATA = 0;         
    LCD_DATA_DATA = command;
	// ENable PW
    LCD_CTRL_DATA = EN; 
    delayUs(1);		
    LCD_CTRL_DATA = 0;
    if (command < 4)
        delayMs(2);         	
    else
        delayUs(40);
}

void LCD_start(void){
//GPIO initialization
    SYSCTL_RCGCGPIO_R |= 0x00000003;
    GPIO_PORTB_DIR_R = 0xFF;
    GPIO_PORTA_DIR_R = 0xFF;
    GPIO_PORTA_DEN_R = 0xFF;
    GPIO_PORTB_DEN_R = 0xFF;
	
   // LCD initialization sequence 
    delayMs(20);           // to ensure LCD power is ON
    LCD_command(0x38);		// To make LCD ready
    delayMs(5);				
    LCD_command(0x38);		
    delayUs(1);			
    LCD_command(0x38);		
    delayUs(1);			
  // LCD functionalities 
    LCD_command(0x38);      // set 8-bit data, 2-line, 5x7 font 
    LCD_command(0x06);      // move cursor right 
    LCD_command(0x01);      // clear screen, move cursor to home 
    LCD_command(0x0F);      // turn on display, cursor blinking 
}

// LCD connections
void LCD_data(unsigned char data){       
    LCD_CTRL_DATA = RS;    
    LCD_DATA_DATA = data;
    LCD_CTRL_DATA = EN | RS;   
    delayUs(1);
    LCD_CTRL_DATA = 0;
    delayMs(40);
}

void LCD_clear (void){
    LCD_command(0x01);
}

void LCD_line(uint8_t line){
    if (line == 1)
        LCD_command(0x80);
    else if (line == 2)
        LCD_command(0xC0);
}


void LCD_display(char* name){
    for(unsigned int i=0; i<strlen(name);i++){
        LCD_data(name[i]);
    }
}


// delay in Ms
void delayMs(int n)
{  
	volatile int i,j;  
	for(i=0;i<n;i++)
	//delay for 2 msec
		for(j=0;j<3180*2;j++)       
		{}
}

// delay in US
void delayUs(int n)             
{
	volatile int i,j;	
	for(i=0;i<n;i++)
	//delay for 100 micro second 
		for(j=0;j<200;j++)       
		{}
}

// convert to string
void toString (int tim, char text []){
	int number=tim;
	int digits=0;
	while (number) {
        number /= 10;
        digits++;
    }
	for (int j =0; j<digits; j++){
		text[j]='0';
	}
	
	//Char array
  int i = digits;
	while (tim != 0){
		i--;		
		int rem=tim%10;
		text[i]=rem + '0';
		tim=(tim/10)|0;
	}
	text[digits]='\0';
}

// Print output through UART
void printchar(char x)
{
	while ((UART0_FR_R & (1<<5)) != 0);
	UART0_DR_R = x ;
}


void print(char* string)
{
	while (*string)
	{
		printchar(*(string++));
	}
}


