#include "temp.h"

void init_ADC() {
	/* ADC initialisation structure */
	ADC_InitTypeDef ADC_InitStruct;
	/* Needs to be configured to work with any ADC */
	ADC_CommonInitTypeDef ADC_CommonInitStruct;
	ADC_DeInit(); //set to default
	
	/** Enable clock **/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 
	
	ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div8;
	ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	
	ADC_CommonInit(&ADC_CommonInitStruct);
	
	ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStruct.ADC_NbrOfConversion = 1;
	
	ADC_Init(ADC1, &ADC_InitStruct);
	
	/* Connect ADC1 to temperature sensor */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_TempSensor, 1, 	ADC_SampleTime_144Cycles);
	
	/* Enable internal temperature senosor */
	ADC_TempSensorVrefintCmd(ENABLE);
	
	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);
}

/**
* Function gets current temperature value.
* @return current temperature value
*/
double get_temperature() {
	double tempValue;
	ADC_SoftwareStartConv(ADC1); //adc starts conversion
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET) {
		//waiting conversion to end
	}
	//these next calculations i found somewhere in stm32f4
	//manual
	tempValue = ADC_GetConversionValue(ADC1); 
	tempValue *= 3300; 
	tempValue /= 0xfff; //this gets mV
	tempValue /= 1000.0; //this gets V
	tempValue -= 0.760; // subtract referent voltage
	tempValue /= .0025; // divided by step
	tempValue -= 11.0; // some kind of magic number
	return tempValue;
}
