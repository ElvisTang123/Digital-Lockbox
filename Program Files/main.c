
#include "main.h"
#include "keypad.h"
#include "LCD.h"
#include "LED.h"
#include <stdio.h>

void SystemClock_Config(void);
void LockScreen(void);
void UnlockScreen(void);
void ReSetPin(void);
uint8_t CheckPress(void);
uint8_t compareArrays(uint8_t a[], uint8_t b[], uint8_t n);
uint8_t Key_in(uint8_t key[],uint8_t button,uint8_t i);

int main(void)
{
	uint8_t key[4];
	uint8_t DFLT_key[4] = {'1','2','3','4'};
	uint8_t button;
	uint8_t i = 0;
	uint8_t j = 0;

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	SysTick_Init();

	/* Initialize keypad, LCD, LED */
	keypad_init();
	LCD_init();
	LED_init();

	LockScreen();	// Show "Lock" on the LCD
	LED_toggle();	// Turn LED on
	while (1)
	{
		button = CheckPress();	// Make sure only read button once and wait till button pressed

		if(button != NO_KEY && button < 10 && i<4){	//Set the range of the number and restricted the maximum pins to 4 pins
				Key_in(key,button,i);	// Print pressed button on the LCD
				i++;
		}
		else if(button == STAR){
			   if(compareArrays(key, DFLT_key, 4)){	//Compare key-in keys with the default keys return true if two keys are same
					LCD_clear();	// Clear the display
					UnlockScreen();	// Show "unlocked" on the LCD
					key[0] = 0;		// Make key-in keys different from default key
					LED_toggle();	// Turn LED off

				    button = CheckPress(); // Wait till button pressed

				    if(button == POUND){
						   LCD_clear();	// Clear the display
						   ReSetPin();	// Show "reset pin" on the LCD
						   while (1){

								button = CheckPress();	// Wait till button pressed

								if(button != NO_KEY && button < 10 && j<4){	//Set the range of the number and restricted the maximum pins to 4 pins
									Key_in(DFLT_key,button,j);	// Print pressed button on the LCD and set the new pins
									j++;
								}
								else if(button == POUND){	// Modify the new set pins
									j = 0;
									LCD_clear();	// Clear the display
									ReSetPin();		// Show "reset pin" on the LCD
								}
								else if(button == STAR){	//	Confirm the reset pins and set the system to lock state
									LCD_clear();	// Clear the display
									LockScreen();	// Show "Lock" on the LCD
									i = 0;	// Reactivate the key_in function
									j = 0;	// Reactivate the key_in function
									GPIOB->ODR |= (GPIO_ODR_OD8);	// Turn LED on
									break;
								}
						  }
				    }
			   }
			   else{	// Modify the key-in pins
				LCD_clear();	// Clear the display
				LockScreen();	// Show "Lock" on the LCD
				i = 0;
				GPIOB->ODR |= (GPIO_ODR_OD8);	// Turn LED on
			   }
		}
		while(keypad_read() != NO_KEY);	// wait for button release
		delay_us(50);
	}
}


void LockScreen(void){
	LCD_write('L');	LCD_write('O');	LCD_write('C');	LCD_write('K');	LCD_write('E');	LCD_write('D'); LCD_write(' '); // Print LOCKED
	LCD_command(0xc0);
	LCD_write('E');	LCD_write('N');	LCD_write('T');	LCD_write('E');	LCD_write('R');	LCD_write(' '); // Print ENTER
	LCD_write('K');	LCD_write('E');	LCD_write('Y');	LCD_write(':'); // Print KEY:
}

void UnlockScreen(void){
	LCD_write('U');	LCD_write('N'); LCD_write('L');	LCD_write('O');	LCD_write('C');	LCD_write('K');	LCD_write('E');	LCD_write('D'); LCD_write(' '); // Print UNLOCKED
	LCD_command(0xc0);
	LCD_write('P');	LCD_write('R');	LCD_write('E');	LCD_write('S');	LCD_write('S');	LCD_write(' '); // Print PRESS
	LCD_write('*'); LCD_write(' ');	// Print *
	LCD_write('T');	LCD_write('O');	LCD_write(' ');	// Print TO
	LCD_write('L');	LCD_write('O');	LCD_write('C');	LCD_write('K'); LCD_write(' ');// Print LOCK
}

void ReSetPin(void){
	LCD_write('R');	LCD_write('E'); LCD_write('S');	LCD_write('E');	LCD_write('T');	LCD_write(' ');	// Print RESET
	LCD_write('P');	LCD_write('I');	LCD_write('N'); LCD_write('S');	LCD_write(':');// Print PINS
}

uint8_t compareArrays(uint8_t a[], uint8_t b[], uint8_t n) {
  int i;
  for(i = 0; i < n; i++) {				// Compare through each element
    if (a[i] != b[i]) return 0;
  }
  return 1;
}
uint8_t CheckPress(void){
	uint8_t button;
	while(keypad_read() != NO_KEY);	    // wait for button release

	button = keypad_read();

	while(button == NO_KEY){			// read until button press
		button = keypad_read();
	}
	return button;
}

uint8_t Key_in(uint8_t key[],uint8_t button,uint8_t i){
	key[i] = button + '0';				// Key-in 4 pins
	switch(i){
	case 0:
	   LCD_write(key[0]);
	   break;
	case 1:
	   LCD_write(key[1]);
	   break;
	case 2:
	   LCD_write(key[2]);
	   break;
	case 3:
	   LCD_write(key[3]);
	   break;
	}
	return key[i];
}
/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
