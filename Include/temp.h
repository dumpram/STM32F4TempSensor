#ifndef _TEMP_H_
#define _TEMP_H_

#include "stm32f4xx_adc.h"

#define TEMP_INTERVAL 8000000

void init_ADC(void);
double get_temperature(void);


#endif