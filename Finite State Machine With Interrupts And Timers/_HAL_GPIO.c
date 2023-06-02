#include "_HAL_GPIO.h"
#include <stdint.h>

uint32_t PINPOS[16] = {
	(0x00), //pin0 position starts at 0.
	(0x04), //pin1 position starts at 4.
	(0x08),
	(0x0C),
	(0x10),
	(0x14),
	(0x18),
	(0x1C),
	
	(0x00), //pin8 position starts at 0.
	(0x04), //pin9 position starts at 4.
	(0x08),
	(0x0C),
	(0x10),
	(0x14),
	(0x18),
	(0x1C)
};

static void config_pin (GPIO_TypeDef *port, uint32_t pinNumber, uint32_t mode_type){
	
	if (pinNumber >= 8){ //Control high register
	
		switch(mode_type){
			
			// I/O Modes
			case OUTPUT_GEN_PURPOSE | INPUT_ANALOG:
				port -> CRH &= ~( (1<<CNF_POS_BIT1) | (1<<CNF_POS_BIT2)); 
			break;

			case OUTPUT_OD | INPUT_FLOATING:
				port -> CRH &= ~(1<<CNF_POS_BIT2); 
				port -> CRH |= (1<<CNF_POS_BIT1); 
			break;

			case OUTPUT_ALT_FUNCTION | INPUT_PU_PD:
				port -> CRH |= OUTPUT_ALT_FUNCTION << (CNF_POS_BIT1); 
			break;

			case OUTPUT_ALT_FUNCTION_OD:
				port -> CRH |= OUTPUT_ALT_FUNCTION_OD<<(CNF_POS_BIT1); 
			break;
			
		}
		
		
	}else{ //Control low register
		
		switch(mode_type){
			
			// I/O Modes
			case OUTPUT_GEN_PURPOSE | INPUT_ANALOG:
				port -> CRL &= ~( (1<<CNF_POS_BIT1) | (1<<CNF_POS_BIT2)); 
			break;

			case OUTPUT_OD | INPUT_FLOATING:
				port -> CRL &= ~(1<<CNF_POS_BIT2); 
				port -> CRL |= (1<<CNF_POS_BIT1); 
			break;

			case OUTPUT_ALT_FUNCTION | INPUT_PU_PD:
				port -> CRL |= OUTPUT_ALT_FUNCTION << (CNF_POS_BIT1); 
			break;

			case OUTPUT_ALT_FUNCTION_OD:
				port -> CRL |= OUTPUT_ALT_FUNCTION_OD<<(CNF_POS_BIT1); 
			break;
			
		}
		
	}
	
}

static void config_pin_speed (GPIO_TypeDef *port, uint32_t pinNumber, uint32_t pinSpeed, uint32_t mode){

	if(pinNumber>-8){
		
		if(mode == INPUT_MODE) //set Control HiGh Reg to inpot mode
			port -> CRH &= ~(1<<(PINPOS[pinNumber]) | 1<<(PINPOS[pinNumber]+1));
		else
			port -> CRH |= (pinSpeed << (PINPOS[pinNumber])); //set Control HiGh to output Mode at given speed
	
	} else {
		
		if(mode == INPUT_MODE)
			port -> CRL &= ~(1<<(PINPOS[pinNumber]) | 1<<(PINPOS[pinNumber]+1));
		else
			port -> CRL |= (pinSpeed << (pinNumber));
	
	}
}

void goio_write(GPIO_TypeDef *port, uint32_t pinNumber, uint8_t state){
	
	if(state){
	
		port -> BSRR = (1 << pinNumber);
		
	} else {
	
		port -> BSRR = (1 << (pinNumber + 16));
		
	}
	
}

void gpio_toggle(GPIO_TypeDef *port, uint32_t pinNumber){

	port -> ODR ^= (1 << pinNumber);

}

void gpio_init(GPIO_TYPE gpio_type){

	if(gpio_type.port == PORTA)
		GPIO_CLOCK_ENABLE_PORTA;
	
	if(gpio_type.port == PORTB)
		GPIO_CLOCK_ENABLE_PORTB;
	
	if(gpio_type.port == PORTC)
		GPIO_CLOCK_ENABLE_PORTC;
	
	if(gpio_type.port == PORTD)
		GPIO_CLOCK_ENABLE_PORTD;
	

	config_pin(gpio_type.port, gpio_type.pin, gpio_type.mode_type);

	config_pin_speed(gpio_type.port, gpio_type.pin, gpio_type.speed, gpio_type.mode);

}

//Interrupt Functions
//Configure interrupt
void configure_gpio_interrupt(GPIO_TypeDef *port, uint32_t pinNumber, edge_select edge){
	
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	
	//PORTA IF
	if(port == PORTA){
		switch(pinNumber){
			case 0:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI0_PA;
			break;
			case 1:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI1_PA;
			break;
			case 2:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI2_PA;
			break;
			case 3:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI3_PA;
			break;
			case 4:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI4_PA;
			break;
			case 5:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI5_PA;
			break;
			case 6:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI6_PA;
			break;
			case 7:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI7_PA;
			break;
			case 8:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI8_PA;
			break;
			case 9:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI9_PA;
			break;
			case 10:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI10_PA;
			break;
			case 11:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI11_PA;
			break;
			case 12:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI12_PA;
			break;
			case 13:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI13_PA;
			break;
			case 14:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI14_PA;
			break;
			case 15:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI15_PA;
			break;	
		}
	}
	
	//PORTB IF
	if(port == PORTB){
		switch(pinNumber){
			case 0:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI0_PB;
			break;
			case 1:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI1_PB;
			break;
			case 2:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI2_PB;
			break;
			case 3:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI3_PB;
			break;
			case 4:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI4_PB;
			break;
			case 5:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI5_PB;
			break;
			case 6:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI6_PB;
			break;
			case 7:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI7_PB;
			break;
			case 8:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI8_PB;
			break;
			case 9:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI9_PB;
			break;
			case 10:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI10_PB;
			break;
			case 11:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI11_PB;
			break;
			case 12:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI12_PB;
			break;
			case 13:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI13_PB;
			break;
			case 14:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI14_PB;
			break;
			case 15:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI15_PB;
			break;	
		}
	}
	
	//PORTC IF
	if(port == PORTC){
		switch(pinNumber){
			case 0:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI0_PC;
			break;
			case 1:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI1_PC;
			break;
			case 2:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI2_PC;
			break;
			case 3:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI3_PC;
			break;
			case 4:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI4_PC;
			break;
			case 5:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI5_PC;
			break;
			case 6:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI6_PC;
			break;
			case 7:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI7_PC;
			break;
			case 8:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI8_PC;
			break;
			case 9:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI9_PC;
			break;
			case 10:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI10_PC;
			break;
			case 11:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI11_PC;
			break;
			case 12:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI12_PC;
			break;
			case 13:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI13_PC;
			break;
			case 14:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI14_PC;
			break;
			case 15:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI15_PC;
			break;	
		}
	}
	
	//PORTD IF
	if(port == PORTD){
		switch(pinNumber){
			case 0:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI0_PD;
			break;
			case 1:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI1_PD;
			break;
			case 2:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI2_PD;
			break;
			case 3:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI3_PD;
			break;
			case 4:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI4_PD;
			break;
			case 5:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI5_PD;
			break;
			case 6:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI6_PD;
			break;
			case 7:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI7_PD;
			break;
			case 8:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI8_PD;
			break;
			case 9:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI9_PD;
			break;
			case 10:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI10_PD;
			break;
			case 11:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI11_PD;
			break;
			case 12:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI12_PD;
			break;
			case 13:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI13_PD;
			break;
			case 14:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI14_PD;
			break;
			case 15:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI15_PD;
			break;	
		}
	}
	
	//PORTE IF
	if(port == PORTE){
		switch(pinNumber){
			case 0:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI0_PE;
			break;
			case 1:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI1_PE;
			break;
			case 2:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI2_PE;
			break;
			case 3:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI3_PE;
			break;
			case 4:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI4_PE;
			break;
			case 5:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI5_PE;
			break;
			case 6:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI6_PE;
			break;
			case 7:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI7_PE;
			break;
			case 8:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI8_PE;
			break;
			case 9:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI9_PE;
			break;
			case 10:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI10_PE;
			break;
			case 11:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI11_PE;
			break;
			case 12:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI12_PE;
			break;
			case 13:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI13_PE;
			break;
			case 14:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI14_PE;
			break;
			case 15:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI15_PE;
			break;	
		}
	}
	
	//PORTF IF
	if(port == PORTF){
		switch(pinNumber){
			case 0:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI0_PF;
			break;
			case 1:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI1_PF;
			break;
			case 2:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI2_PF;
			break;
			case 3:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI3_PF;
			break;
			case 4:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI4_PF;
			break;
			case 5:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI5_PF;
			break;
			case 6:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI6_PF;
			break;
			case 7:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI7_PF;
			break;
			case 8:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI8_PF;
			break;
			case 9:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI9_PF;
			break;
			case 10:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI10_PF;
			break;
			case 11:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI11_PF;
			break;
			case 12:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI12_PF;
			break;
			case 13:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI13_PF;
			break;
			case 14:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI14_PF;
			break;
			case 15:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI15_PF;
			break;	
		}
	}
	
	//PORTG IF
	if(port == PORTG){
		switch(pinNumber){
			case 0:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI0_PG;
			break;
			case 1:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI1_PG;
			break;
			case 2:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI2_PG;
			break;
			case 3:
				AFIO->EXTICR[0] = AFIO_EXTICR1_EXTI3_PG;
			break;
			case 4:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI4_PG;
			break;
			case 5:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI5_PG;
			break;
			case 6:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI6_PG;
			break;
			case 7:
				AFIO->EXTICR[1] = AFIO_EXTICR2_EXTI7_PG;
			break;
			case 8:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI8_PG;
			break;
			case 9:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI9_PG;
			break;
			case 10:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI10_PG;
			break;
			case 11:
				AFIO->EXTICR[2] = AFIO_EXTICR3_EXTI11_PG;
			break;
			case 12:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI12_PG;
			break;
			case 13:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI13_PG;
			break;
			case 14:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI14_PG;
			break;
			case 15:
				AFIO->EXTICR[3] = AFIO_EXTICR4_EXTI15_PG;
			break;	
		}
	}
	
	if(edge == RISING_EDGE){
		EXTI ->RTSR |= 1<<pinNumber;
	}
	if(edge == FALLING_EDGE){
		EXTI ->FTSR |= 1<<pinNumber;
	}
	if(edge == RISING_FALLING_EDGE){
		EXTI ->RTSR |= 1<<pinNumber;
		EXTI ->FTSR |= 1<<pinNumber;
	}
}

//Enable interrupt
void enable_gpio_interrupt(uint32_t pinNumber, IRQn_Type irqNumber){
	EXTI ->IMR |= 1<<pinNumber;
	
	//GLCD_DisplayString (8, 0, 1, "   work   ");
	
	NVIC_EnableIRQ(irqNumber);
}

//Clear interrupt
void clear_gpio_interrupt(uint32_t pinNumber){

	EXTI ->PR |= 1<<pinNumber;
	
	//NVIC_EnableIRQ(irqNumber);
	
}
