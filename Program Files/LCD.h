/*
 * LCD.h
 *
 *  Created on: Oct 11, 2021
 *      Author: Prestige
 */

#ifndef SRC_LCD_H_
#define SRC_LCD_H_

#define Rs GPIOA->ODR &= ~GPIO_MODER_MODE12
#define RW GPIOA->ODR &= ~GPIO_MODER_MODE11
#define E GPIOA->ODR &= ~GPIO_MODER_MODE10
#define data_bus GPIOA->ODR &= ~(GPIO_MODER_MODE6 | GPIO_MODER_MODE7 | GPIO_MODER_MODE8 | GPIO_MODER_MODE9)

void LCD_init(void);
void SysTick_Init(void);
void delay_us(const uint16_t time_us);
void LCD_nibble_write(uint8_t data_nibble, uint8_t rs);
void LCD_command(uint8_t command);
void LCD_write(uint8_t data);
void LCD_write_char(uint8_t letter);
void lcd_send_string (char *str);
void lcd_put_cur(int row, int col);
void LCD_clear(void);

#endif /* SRC_LCD_H_ */

