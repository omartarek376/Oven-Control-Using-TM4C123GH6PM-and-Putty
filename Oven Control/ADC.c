#include "LCD_config.h"
#include "ADC.h"

void adc_init (){
	

	SYSCTL_RCGCGPIO_R|=(1<<4);  // port E clock initialization 
	SYSCTL_RCGCADC_R |=  1; /* Enabling AD0*/ 
	
	// enable alternate function, disable digital function, enable analog function
	GPIO_PORTE_AFSEL_R |= (1UL<<3);  
  GPIO_PORTE_DEN_R &= ~(1UL<<3);    
  GPIO_PORTE_AMSEL_R |= (1UL<<3);  
	
	//  SS3 OFF
	ADC0_ACTSS_R &= ~(1UL<<3);
	// software trigger conversion
  ADC0_EMUX_R &= ~0xF000;    
	// input channel 
	ADC0_SSMUX3_R = 0;         
	// set flag at 1st sample
	ADC0_SSCTL3_R |= (1<<1)|(1<<2);        
	// enable ADC0 (3)
	ADC0_ACTSS_R |= (1<<3);         
		
}

