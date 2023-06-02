/******************************************************************************
statemachine.c
CSE/EE 5385/7385 Microprocessor Architecture and Interfacing
ARM MCBSTM32C Final Project
******************************************************************************/

#include "stdio.h"
//#include <stm32f10x_cl.h>
#include "_HAL_GPIO.h"
#include "stm32f10x.h"
#include "GLCD.h"
#include <math.h>
#include <stdlib.h>

#define LED_NUM     8                   /* Number of user LEDs                */
const long led_mask[] = { 1<<8, 1<<9, 1<<10, 1<<11, 1<<12, 1<<13, 1<<14, 1<<15 };
int i=0;

int but = 0, but_ = -1;
int state = 0;
int j;
int myTicks = 0;

void delaySeconds(int s){

	TIM4->CR1 |= TIM_CR1_CEN;
	
	myTicks = 0;
	//When ticks less than required time, keep loopping
	while(myTicks < (s * 2000000)){
		myTicks++;
	}
	
	TIM4->CR1 |= ~TIM_CR1_CEN;
}

void enable_gpio_interrupt(uint32_t pinNumber, IRQn_Type irqNumber);
void clear_gpio_interrupt(uint32_t pinNumber);

void EXTI3_IRQ_IRQHandler(){
	//GLCD_DisplayString (9, 0, 1, "   work   ");
	
	//First of all, clear interrupt
	clear_gpio_interrupt(3);
	
	//Change state from State0 to Gear1
	state = 1;
}

void EXTI4_IRQ_IRQHandler(){
	//First of all, clear interrupt
	clear_gpio_interrupt(4);
	
	if(but == 4){
		//Cycle through 8 LEDs
		for (j = 0; j < 8; j++) {
			GPIOE->BSRR = led_mask[j]; // Turn on LED for 2 seconds
			delaySeconds(2);
			GPIOE->BSRR = led_mask[j] << 16; // Turn off
		}
	}
	
	//Change state from Gear3 to Gear2 or Gear4
	state = but;
}

void TIM4_IRQHandler(void){
	
	if(TIM4->SR & TIM_SR_UIF){
		myTicks++;
	}
	
	TIM4->SR &= ~TIM_SR_UIF;
	
}

int main (void) {
	
	GPIO_TYPE smGPIO;
	
	smGPIO.port = PORTC;
	smGPIO.pin = 10;
	smGPIO.mode = OUTPUT_MODE;
	smGPIO.mode_type = OUTPUT_GEN_PURPOSE;
	smGPIO.speed = SPEED_50MHZ;
	
	gpio_init(smGPIO);
	
	//Configure interrupt
	configure_gpio_interrupt(PORTC, 4, RISING_EDGE);
	
  SystemInit();

  /* Configure the GPIO for Push Buttons                                      */
  RCC->APB2ENR |=  1 <<  2;             /* Enable GPIOA clock                 */
  RCC->APB2ENR |=  1 <<  3;             /* Enable GPIOB clock                 */
  RCC->APB2ENR |=  1 <<  4;             /* Enable GPIOC clock                 */
  GPIOA->CRL   &= 0xFFFFFFF0;
  GPIOA->CRL   |= 0x00000004;
  GPIOB->CRL   &= 0x0FFFFFFF;
  GPIOB->CRL   |= 0x40000000;
  GPIOC->CRH   &= 0xFF0FFFFF;
  GPIOC->CRH   |= 0x00400000;
  
	/*Configure pin3 on GPIOC*/
	//Might result in button failure
	//GPIOC->CRH |= ( (1<<20) | (1<<21) );
	//GPIOC->CRH &= ~( (1<<22) | (1<<23) );
	
  /* Setup GPIO for LEDs*/
  RCC->APB2ENR |=  1 <<  6;             /* Enable GPIOE clock 			*/
  GPIOE->CRH    = 0x33333333;           /* Configure the GPIO for LEDs 	*/

  GLCD_Init();                          /* Initialize graphical LCD display   */
  GLCD_Clear(White);                    /* Clear graphical LCD display        */
	
	
	//Enable timer clock
	RCC->APB2ENR |= RCC_APB1ENR_TIM4EN;
	
	//Configure timer
	TIM4->PSC = 0;
	TIM4->ARR = 72; //Define frequence as 1MHz
	TIM4->CR1 |= TIM_CR1_URS;//update interrupt will be generated only when counter over/underflow
	TIM4->DIER |= TIM_DIER_UIE;
	TIM4->EGR |= TIM_EGR_UG;
	
	NVIC_EnableIRQ(TIM4_IRQn);
	
  for (;;) {   
		
		/* Button inputs                                                          */
    but = 0;
    if (GPIOB->IDR & (1 <<  7)) but |= (1 << 0);  /* Button User (S1), but=1         */
		//if ((GPIOB->IDR & (1 <<  7)) && state == 3) cycle = 1;  /* Button User (S1)		      */
    if (GPIOC->IDR & (1 << 13)) but |= (1 << 1);  /* Button Tamper (S2), but=2       */
    if (GPIOA->IDR & (1 <<  0)) but |= (1 << 2);  /* Button Wakeup (S3), but=4       */
    but ^= 0x03;
		
    switch (state) {
      case 0:
			//case 1:
			/********* YOUR CODE GOES HERE **********/
		  /*Display state, check for push button, move to state if User pressed*/
			GLCD_SetBackColor(Red);
			GLCD_SetTextColor(White);
			/*Display "Task A started"  				 */
			GLCD_DisplayString (0, 0, 1, "  ARM RESET STATE 0  ");
			GLCD_SetBackColor(White);
			GLCD_SetTextColor(Blue);
			GLCD_DisplayString (3, 0, 1, "   Engine Idle ...   ");
			GLCD_DisplayString (5, 0, 1, "  Waiting to start   ");
			GLCD_DisplayString (7, 0, 1, "  Press Wakeup key    ");
			if (but == 4){//When Wakeup key is pressed
				//Configure interrupt
				configure_gpio_interrupt(PORTC, 3, RISING_EDGE);
				//Enable interrupt
				enable_gpio_interrupt(3, EXTI3_IRQn);
				gpio_toggle(PORTC,9);
				
				state = 1;
			}
			/********** END **********/
      break;
			
			//case 0:
      case 1:                          
      /********* YOUR CODE GOES HERE **********/
		  /*Display state, add delay, move to next state*/
			GLCD_Clear(White);
			GLCD_SetTextColor(Black);
			GLCD_DisplayString (0, 0, 1, "ARM RUNNING STATE 1  ");
			GLCD_DisplayString (5, 0, 1, "      Gear 1         ");
			
			//Delay 3 seconds
			delaySeconds(3);
			//Change state from Gear1 to Gear2
			state = 2;
		  /********** END **********/
			break;
			
			case 2:                           
      /********* YOUR CODE GOES HERE **********/
		  /*Display state, add delay, move to next state*/
			GLCD_Clear(White);
			GLCD_SetTextColor(Black);
			GLCD_DisplayString (0, 0, 1, "ARM RUNNING STATE 2  ");
			GLCD_DisplayString (5, 0, 1, "      Gear 2         ");
			
			//Delay 5 seconds
			delaySeconds(5);
			//Change state from Gear2 to Gear3
			state = 3;
		  /********** END **********/
      break;
			
			case 3:                           
			/********* YOUR CODE GOES HERE **********/
			/*Display state, check for push button, and move to corresponding state*/
			/*Flash all 8 LEDs sequentially*/
			//GLCD_Clear(White);
			GLCD_SetTextColor(Red);
			GLCD_DisplayString (0, 0, 1, "ARM RUNNING STATE 3  ");
			GLCD_DisplayString (5, 0, 1, "    Adjust Speed     ");
			GLCD_DisplayString (7, 0, 1, "Press Tamper/ Wakeup");
			if (but == 2){//When Tamper key is pressed, switch to Gear2
				//Configure interrupt
				configure_gpio_interrupt(PORTC, 4, RISING_EDGE);
				//Enable interrupt
				enable_gpio_interrupt(4, EXTI4_IRQn);
				gpio_toggle(PORTC,10);
				
				state = but;
			}else if (but == 4){//When Wakeup key is pressed
				//Configure interrupt
				configure_gpio_interrupt(PORTC, 4, RISING_EDGE);
				//Enable interrupt
				enable_gpio_interrupt(4, EXTI4_IRQn);
				gpio_toggle(PORTC,10);
				
				//Cycle through 8 LEDs
				for (j = 0; j < 8; j++) {
					GPIOE->BSRR = led_mask[j]; // Turn on
					delaySeconds(1);
					GPIOE->BSRR = led_mask[j] << 16; // Turn off
				}
				
				//Switch to state 4
				state = but;
			}
		  /********** END **********/
      break;
				
		 case 4:                            
		 /********* YOUR CODE GOES HERE **********/
		 /*Display state, add delay, move back to state zero*/
			GLCD_Clear(White);
			GLCD_SetTextColor(Black);
			GLCD_DisplayString (0, 0, 1, "ARM RUNNING STATE 4  ");
			GLCD_DisplayString (5, 0, 1, "      Gear 4         ");
		 //Delay 6 seconds
			delaySeconds(6);
			//Change state from Gear4 to Neutral
		  state = 0;
		 /********** END **********/
		 break;
    }
  }
}


