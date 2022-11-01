#include "LCD_config.h"
#include "ADC.h"
#include "Dio.h"
#include "main.h"

// Queues needed
static QueueHandle_t xUARTQueue;
static QueueHandle_t xLCDQueue;
static QueueHandle_t xAlarmQueue;

// Messages initialization
typedef struct Message{
	char Txt1[4]; 
	char Txt2[4];
} AMessage;

// main_task function

void main_task(void *pvParameters){

	unsigned char setpoint=30; // Temperature setpoint
	volatile int Temperature;  // Temprature variable
	volatile int mV;          // mV variable for potentiometer mapping
	const int on = 1;
	const int off = 0;
	AMessage msg;
	adc_init() ;

 while(1){
	// Communication initialization and checking 
	ADC0_PSSI_R |= (1UL<<3);
	while((ADC0_RIS_R & 8) == 0) {}
	//reads potentiometer values
	unsigned int val = ADC0_SSFIFO3_R; 					
	ADC0_ISC_R = 8; 								//Read ADC
  //Convert potentiometer input to readable temp in C
	mV = ((val * 3300)	/ 4096);				
	Temperature = ((mV)/25)+10;			
	// Receive data
	xQueueReceive(xUARTQueue,	&setpoint, 0);		
		
	// Temperature conditions 
	if(Temperature < setpoint){										
			GPIO_PORTE_DATA_R |= 0x02;	//Green LED ON
	}else																		
	{			
		GPIO_PORTE_DATA_R &=~ 0x02;	//Green Led OFF
	}
	
	//Measured and setpoint string assigned to variables
	toString(Temperature, msg.Txt1);	
	toString(setpoint, msg.Txt2);	
	
	//Send messages to LCD through QUEUE 
	xQueueSendToBack(xLCDQueue, &msg,0);

	//Temperature conditions
	if(Temperature > setpoint+20){ 								
		xQueueSend(xAlarmQueue, &on, 0); 				//RED LED ON
	}
	else{
	xQueueSend(xAlarmQueue,&off,0);			//RED LED ON
	}
	vTaskDelay(10/portTICK_RATE_MS);

		}
}

// uart controller
void UART_task(void* pvParameters){
	// Setpoint input by the user through PUTTY
		int S;
		int Total;
		while (1)
		{
			S = 0;
			Total = 0;
			print("\nEnter Temperature Set Point: ");
			while(1){
				while ((UART0_FR_R & (1<<4) )!= 0);
				S = UART0_DR_R;
				printchar(S);
				if (S == '\r')
					break;
				S = S - '0';
				Total = 10 * Total + S;
		}
		print("\nSet Point Updated");
		xQueueSendToBack(xUARTQueue,&Total, portMAX_DELAY);
		
	}
}

// lcd controller
void LCD_task(void *pvParameters){
	AMessage msg;
	LCD_clear ();
	while(1){
		xQueueReceive(xLCDQueue,&msg,portMAX_DELAY);
		//Printing both setpoint and measured values
		LCD_line(1); 
		LCD_display("Measured: ");   
		LCD_display(msg.Txt1);
		LCD_display("   ");	
		LCD_line(2);
		LCD_display("Setpoint: "); 		
		LCD_display(msg.Txt2);
		LCD_display("   ");
	}
}

// alarm controller
void ALARM_task(void *pvParameters){
int state=0; // State indicates wether temprature exceeded a specific value
    while(1){
			xQueueReceive(xAlarmQueue,&state,portMAX_DELAY);
			if(state==1){
				GPIO_PORTE_DATA_R |= 0x05;			// RED LED IS ON 
			}else{
				GPIO_PORTE_DATA_R &=~ 0x05;			// RED LED IS OFF
			}
    }
}

int main(void)
{
	// Queues creation for UART, LCD, ALARM
	xUARTQueue = xQueueCreate(1,sizeof(int));    // UART Queue
	xLCDQueue = xQueueCreate(1,sizeof(AMessage));  // LCD Queue
	xAlarmQueue = xQueueCreate(1,sizeof(int));    // Alarm Queue
  GPIO_init();
	// Tasks creation for ALARM, UART, LCD, CONTROLLER 
	xTaskCreate(ALARM_task,"Alarm_Controller",50,NULL, 2,NULL);
	xTaskCreate(UART_task,"UART_Controller",100,NULL, 1,NULL);
	xTaskCreate(LCD_task,"LCD_Controller",50,NULL, 4,NULL);
	xTaskCreate(main_task,"MAIN_Controller",50,NULL, 3,NULL);
	vTaskStartScheduler();
}

