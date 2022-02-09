#include "STM32F407xx.h"
#include "Board_LED.h"

void SysTick_Handler (void);


void initialiseADC() {
	// enables clock to the ADC.
	RCC->APB2ENR = (RCC->APB2ENR & ~RCC_APB2ENR_ADC1EN_Msk) | (0x1 << RCC_APB2ENR_ADC1EN_Pos);
	// enables clock to the GPIO Port C. CONFIRM CODE IS CORRECT
	RCC->APB2ENR = (RCC->APB2ENR & ~RCC_AHB1ENR_GPIOCEN_Msk) | (0x1 << RCC_AHB1ENR_GPIOCEN_Pos);
	// Configure GPIO to analogue input. 
	GPIOC->MODER = (GPIOC->MODER & ~GPIO_MODER_MODER14_Msk) | (0x3 << GPIO_MODER_MODER14_Pos);
	// Sets to read input 14 first. ??? CHECK WITH DAVE.
	ADC1->SQR3 = (ADC1->SQR3 & ~ADC_SQR3_SQ1_Msk) | (0xE << ADC_SQR3_SQ1_Pos);
	// Sets it to only perform a single conversion before raising EOC flag I think? 
	ADC1->SQR1 = (ADC1->SQR1 & ~ADC_SQR1_L_Msk) | (0x0000 << ADC_SQR1_L_Pos);
	// Turns ADC on. 
	ADC1->CR2 = (ADC1->CR2 & ~ADC_CR2_ADON_Msk) | (0x1 << ADC_CR2_ADON_Pos);
	// Sets discontinious mode. ???
	ADC1->CR1 = (ADC1->CR1 & ~ADC_CR1_DISCEN_Msk) | (0x1 << ADC_CR1_DISCEN_Pos);
	// Sets end of conversion flag mode?
	ADC1->CR2 = (ADC1->CR2 & ~ADC_CR2_EOCS_Msk) | (0x1 << ADC_CR2_EOCS_Pos);
}

int convertADCValue(uint16_t value) {
	value = (value / 4096) * 3;
	return value;
}

int main() {
	volatile uint32_t ADCconv = 0;
	
	// Setting up LEDs for testing. 
	LED_Initialize();
	//LED_On(3);
	
	
	initialiseADC();
	//SystemCoreClockUpdate();
	//SysTick_Config(SystemCoreClock/2);
	
	// Need to read correct bit of ADC1_SR to check if conversion has finished. 
	// BITWISE OPERATION?? 
	while(1) { 
		volatile uint32_t y = 0;
		// starts conversion of 'standard' channels? 
		ADC1->CR2 = (ADC1->CR2 & ~ADC_CR2_SWSTART_Msk) | (0x1 << ADC_CR2_SWSTART_Pos);
		y = ADC1->SR;
		
		// Checks if end of conversion flag has been set. 
		if ((y & ADC_SR_EOC_Msk) == ADC_SR_EOC_Msk) 
		{
			// retrieves converted value. 
			ADCconv = ADC1->DR;
			if (ADCconv > 500) 
			{				
			LED_On(3);
			}	//printf("The read analogue value is %d", ADCconv);
		}	
	}
	
}