 /*
 * LCD.c
 *
 *  Created on: Oct 11, 2021
 *      Author: Prestige
 */

#include "main.h"
#include "LCD.h"
#include <stdio.h>
#include <stdbool.h>

void LCD_init(void){

	RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN); //Enables GPIO BUS for port A and C

	//outputs: Rs, R/W, E, B7~B4; port A (PA6, PA7,, PA8, PA9, PA10, PA11, PA12) will be used for outputs
	GPIOA->MODER &= ~(GPIO_MODER_MODE6 | GPIO_MODER_MODE7 | GPIO_MODER_MODE8 | GPIO_MODER_MODE9| GPIO_MODER_MODE10 | GPIO_MODER_MODE11 | GPIO_MODER_MODE12); //clear mode bits for the input pins
	GPIOA->MODER |= (1 << GPIO_MODER_MODE6_Pos | 1 << GPIO_MODER_MODE7_Pos | 1 << GPIO_MODER_MODE8_Pos | 1 << GPIO_MODER_MODE9_Pos| 1 << GPIO_MODER_MODE10_Pos | 1 << GPIO_MODER_MODE11_Pos | 1 << GPIO_MODER_MODE12_Pos);
	GPIOA->OTYPER &= ~(GPIO_OTYPER_OT6 | GPIO_OTYPER_OT7 | GPIO_OTYPER_OT8 | GPIO_OTYPER_OT9 | GPIO_OTYPER_OT10 | GPIO_OTYPER_OT11 | GPIO_OTYPER_OT12 ); //set output to push-pull
	GPIOA->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED6 | GPIO_OSPEEDR_OSPEED7 | GPIO_OSPEEDR_OSPEED8 | GPIO_OSPEEDR_OSPEED9 | GPIO_OSPEEDR_OSPEED10 | GPIO_OSPEEDR_OSPEED11 | GPIO_OSPEEDR_OSPEED12); //clear speed register
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD6 | GPIO_PUPDR_PUPD7 | GPIO_PUPDR_PUPD8 | GPIO_PUPDR_PUPD9 | GPIO_PUPDR_PUPD10 | GPIO_PUPDR_PUPD11 | GPIO_PUPDR_PUPD12); //no pu/pd resistors

	for(uint8_t i =0;i<40;i++){
		delay_us(1000);       //delay 4000us
	}


	LCD_nibble_write(0x03,0); // entry mode set
	delay_us(37);

	LCD_command(0x28); // 4 bits mode, 2 lines display, 5 x 8 dots format display mode
	delay_us(37);

	LCD_command(0x28); // 4 bits mode, 2 lines display, 5 x 8 dots format display mode
	delay_us(37);

	LCD_command(0x0f); // display on
	delay_us(37);

	LCD_command(0x01); // clear display
	delay_us(500);
	delay_us(500);
	delay_us(500);

	LCD_command(0x06); // entry mode set
	delay_us(37);

}
/* Configure SysTick Timer for use with delay_us function. This will break
 * break compatibility with HAL_delay() by disabling interrupts to allow for
 * shorter delay timing.
 */
void SysTick_Init(void){
    SysTick->CTRL |= (SysTick_CTRL_ENABLE_Msk |	       // enable SysTick Timer
                      SysTick_CTRL_CLKSOURCE_Msk);     // select CPU clock
    SysTick->CTRL &= ~(SysTick_CTRL_TICKINT_Msk);      // disable interrupt,
                                                       // breaks HAL delay function
}

/* Delay function using the SysTick timer to count CPU clock cycles for more
 * precise delay timing. Passing a time of 0 will cause an error and result
 * in the maximum delay. Short delays are limited by the clock speed and will
 * often result in longer delay times than specified. @ 4MHz, a delay of 1us111
 * will result in a delay of 10-15 us.
 */
void delay_us(const uint16_t time_us) {
    // set the counts for the specified delay
    SysTick->LOAD = (uint32_t)((time_us * SystemCoreClock / 1000000) - 1);
    SysTick->VAL = 0;                                      // clear the timer count
    SysTick->CTRL &= ~(SysTick_CTRL_COUNTFLAG_Msk);        // clear the count flag
    while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)); // wait for the flag
}

void LCD_nibble_write(uint8_t data_nibble, uint8_t rs){

	GPIOA->ODR &= 0x0;     // set all the bit to 0 (Rs, R/W, En, B7~B4)

	if(rs == 0 && ((GPIOA->ODR &= 0x1000)!=0)){
		GPIOA->ODR &= (0<<12);   // Rs 0ff -> command mode
	}
	else if(rs == 1 && ((GPIOA->ODR &= 0x1000)!=1)){
		GPIOA->ODR |= (1 << 12); // Rs 0n -> write mode
	}



	GPIOA->ODR |= (data_nibble << 6); //shift data_nibble to DB7~DB4 in the ODR

	GPIOA->ODR |= (1<<10); //En 0n
	delay_us(1);

	GPIOA->ODR &= ~(1<<10); //En off

	return;

}

void LCD_command(uint8_t command){

	uint8_t hiNib = (command & 0xf0) >> 4; // separate the command into high nibbles and low nibbles
	uint8_t lowNib = (command & 0x0f);

	LCD_nibble_write(hiNib,0);
	delay_us(2);

	LCD_nibble_write(lowNib,0);

	return;

}

void LCD_write(uint8_t data){

	uint8_t hiNib = (data & 0xf0) >> 4;	// separate the data into high nibbles and low nibbles
	uint8_t lowNib = data & 0x0f;

	LCD_nibble_write(hiNib,1);
	delay_us(2);

	LCD_nibble_write(lowNib,1);

	return;

}

void LCD_clear(void)
{
	LCD_command(0x01); // clear the display
	delay_us(500); //delay 1520us
	delay_us(500);
	delay_us(500);
}
