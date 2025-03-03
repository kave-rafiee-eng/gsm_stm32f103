#include "main.h"
#include "Clock.h"

void SystemClock_INI(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL8;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}


// Function to initialize the Watchdog
void Watchdog_Init(void)
{
    // Enable access to the IWDG_PR and IWDG_RLR registers by writing 0x5555 to the KR register
    IWDG->KR = 0x5555;
    
    // Set the prescaler: divide by 64 (value 4 in the PR register)
    IWDG->PR = 4;
    
    // Set the reload value: 1249 (for a timeout of approximately 2 seconds)
    IWDG->RLR = 4000;
    
    // Start the Watchdog by writing 0xCCCC to the KR register
    IWDG->KR = 0xCCCC;
}

// Function to refresh (reset) the Watchdog
void Watchdog_Refresh(void)
{
    // Write 0xAAAA to the KR register to refresh the Watchdog
    IWDG->KR = 0xAAAA;
}
