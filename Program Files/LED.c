/*
 * LED.c
 *
 *  Created on: Oct 14, 2021
 *      Author: Prestige
 */

#include "main.h"
#include "LED.h"
#include <stdio.h>
#include <stdbool.h>

void LED_init(void){

	  RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOBEN);        // enable GPIOB clock on bus

	  GPIOB->MODER &= ~(GPIO_MODER_MODE8);			    // clear MODE5 bits
	  GPIOB->MODER |= (1 << GPIO_MODER_MODE8_Pos);	// set PB8 as GPIO output
	  GPIOB->OTYPER &= ~(GPIO_OTYPER_OT8);			    // PB8 set to push-pull
	  GPIOB->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED8);		// PB8 slow speed
	  GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD8);			    // PB8 no pullup/pulldown resistor
}

void LED_toggle(void){
	GPIOB->ODR ^= (GPIO_ODR_OD8); // toggle the LED
}
