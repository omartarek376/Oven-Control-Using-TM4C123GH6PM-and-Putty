#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "semphr.h"
#include <stdint.h>
#include <stdbool.h>
#include "main.h"
#include "tm4c123gh6pm.h"
#include "LCD_config.h"
#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

// analog to digital conversion to potentiometer input
void adc_init(void); 
// Reading adc of potentiometer input
unsigned int adc_read (void);
 
