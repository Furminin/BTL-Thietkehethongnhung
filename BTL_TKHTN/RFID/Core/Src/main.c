/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "KEYPAD.h"
#include "I2C_LCD.h"
#include "string.h"
#include "SERVO.h"
#include "rc522.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MyI2C_LCD I2C_LCD_1
#define SERVO_Motor1    0
#define KEY_1       0
#define KEY_2       1
#define KEY_3       2
#define KEY_F1      3
#define KEY_4       4
#define KEY_5       5
#define KEY_6       6
#define KEY_F2      7
#define KEY_7       8
#define KEY_8       9
#define KEY_9       10
#define KEY_F3      11
#define KEY_A       12
#define KEY_0       13
#define KEY_H       14
#define KEY_F4      15
#define MAX_SLAVE_CARDS 5 // Maximum number of slave cards
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

SPI_HandleTypeDef hspi2;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */
uint32_t OctaveFour[7] = {262, 294, 330, 349, 392, 440, 493}; // Ocatve_4 Frequencies (C4->B4)
uint8_t NoteIndex = 0;
uint8_t gu8_KeyStatesArr[KEYS] = {0};
uint8_t SysTicks = 0;
int RightPassword[6]= {2, 4, 0, 8, 2, 0};
int Password[6]={};
int c = 2;
int i = 0;
int k = 0;
int a = 0;
int d = 0;
int b = 0;
// RFID variables
static uchar masterCard[4] = {0xE9, 0x11, 0x0B, 0x01}; // Replace with your master card UID
static uchar slaveCards[MAX_SLAVE_CARDS][4] = {
    {0x62, 0x99, 0x18, 0x02}, // Replace with your slave card UIDs
    {0xE9, 0x11, 0x0B, 0x01}
};
static uint8_t numSlaveCards = 2; // Current number of slave cards
static uint8_t addCardMode = 0; // Flag for add card mode
static uint8_t masterAuthenticated = 0; // Flag for master card authentication
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
static void MX_SPI2_Init(void);
/* USER CODE BEGIN PFP */
static void Tone(uint32_t Frequency, uint32_t Duration);
static void noTone();
static void Password_Check(void);
static void Notifications(void);
static void Set_Password(void);
static void Display_Handler(void);
static uchar CompareUID(uchar *scannedUID, uchar *storedUID);
static uchar IsSlaveCard(uchar *scannedUID);
static void AddSlaveCard(uchar *newUID);
static void RFID_ProcessCard(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void Password_Check(void)
{
    if(i == 6){
        for(i = 0; i<6; i++)
        {
            if(Password[i] != RightPassword[i])
            {
                c = 0;
                break;
            } else {
                c = 1;
            }
        }
    }
}

void Notifications(void)
{
    if(c == 1)
    {
        if( b == 0){
            SERVO_MoveTo(SERVO_Motor1, 0);
            SERVO_MoveTo(SERVO_Motor1, 90);
        }
        I2C_LCD_Clear(MyI2C_LCD);
        I2C_LCD_WriteString(MyI2C_LCD,"Welcome");
        Tone(659, 167); // D5
        Tone(587, 167); // E5
        Tone(370, 333); // F#5
        Tone(415, 333); // G5
        Tone(554, 167);
        Tone(494, 167);
        Tone(294, 333);
        Tone(330, 333);
        Tone(494, 167);
        Tone(440, 167);
        Tone(277, 333);
        Tone(330, 333);
        Tone(440, 667);
        noTone();
        if(b==0){
            SERVO_MoveTo(SERVO_Motor1, 0);
        }
        HAL_Delay(1500);
        NoteIndex = 0;
        i=0;
        c=2;
        k=0;
        memset(Password, 0, sizeof(Password));
        if (b == 1){
            d = 1;
        }
        else if (b == 0){}
        main();
    }
    else if (c == 0)
    {
        I2C_LCD_Clear(MyI2C_LCD);
        I2C_LCD_WriteString(MyI2C_LCD, "Sai Mat Khau");
        Tone(OctaveFour[NoteIndex++], 250);
        if(NoteIndex == 5){
            NoteIndex = 0;
        }
        noTone();
        HAL_Delay(1500);
        i=0;
        c=2;
        memset(Password, 0, sizeof(Password));
        k++;
        b = 0;
        if(k == 5)
        {
            I2C_LCD_Clear(MyI2C_LCD);
            I2C_LCD_WriteString(MyI2C_LCD, "Thu Lai Sau 30s");
            k = 0;
            HAL_Delay(10000);
            HAL_Delay(10000);
            HAL_Delay(10000);
            main();
        }
        main();
    }
}

void Set_Password(void)
{
    if(gu8_KeyStatesArr[KEY_1] == KEY_PRESSED)
    {
        I2C_LCD_WriteChar(MyI2C_LCD,'1');
        HAL_Delay(10);
        RightPassword[a] = 1;
        a++;
    }
    if(gu8_KeyStatesArr[KEY_2] == KEY_PRESSED)
    {
        I2C_LCD_WriteChar(MyI2C_LCD,'2');
        HAL_Delay(10);
        RightPassword[a] = 2;
        a++;
    }
    if(gu8_KeyStatesArr[KEY_3] == KEY_PRESSED)
    {
        I2C_LCD_WriteChar(MyI2C_LCD,'3');
        RightPassword[a] = 3;
        a++;
    }
    if(gu8_KeyStatesArr[KEY_4] == KEY_PRESSED)
    {
        I2C_LCD_WriteChar(MyI2C_LCD,'4');
        RightPassword[a] = 4;
        a++;
    }
    if(gu8_KeyStatesArr[KEY_5] == KEY_PRESSED)
    {
        I2C_LCD_WriteChar(MyI2C_LCD,'5');
        RightPassword[a] = 5;
        a++;
    }
    if(gu8_KeyStatesArr[KEY_6] == KEY_PRESSED)
    {
        I2C_LCD_WriteChar(MyI2C_LCD,'6');
        RightPassword[a] = 6;
        a++;
    }
    if(gu8_KeyStatesArr[KEY_7] == KEY_PRESSED)
    {
        I2C_LCD_WriteChar(MyI2C_LCD,'7');
        RightPassword[a] =7;
        a++;
    }
    if(gu8_KeyStatesArr[KEY_8] == KEY_PRESSED)
    {
        I2C_LCD_WriteChar(MyI2C_LCD,'8');
        RightPassword[a] = 8;
        a++;
    }
    if(gu8_KeyStatesArr[KEY_9] == KEY_PRESSED)
    {
        I2C_LCD_WriteChar(MyI2C_LCD,'9');
        RightPassword[a] = 9;
        a++;
    }
    if(gu8_KeyStatesArr[KEY_0] == KEY_PRESSED)
    {
        I2C_LCD_WriteChar(MyI2C_LCD,'0');
        RightPassword[a] = 0;
        a++;
    }
    if(a == 6)
    {
        a = 0;
        d = 0;
        b = 0;
        I2C_LCD_Clear(MyI2C_LCD);
        I2C_LCD_WriteString(MyI2C_LCD,"Doi thanh cong");
        HAL_Delay(1000);
        main();
    }
}

void Display_Handler(void)
{
    if(gu8_KeyStatesArr[KEY_1] == KEY_PRESSED)
    {
        HAL_Delay(100);
        I2C_LCD_WriteChar(MyI2C_LCD,'1');
        Password[i] = 1;
        i++;
    }
    if(gu8_KeyStatesArr[KEY_2] == KEY_PRESSED)
    {
        HAL_Delay(100);
        I2C_LCD_WriteChar(MyI2C_LCD,'2');
        Password[i] = 2;
        i++;
    }
    if(gu8_KeyStatesArr[KEY_3] == KEY_PRESSED)
    {
        HAL_Delay(100);
        I2C_LCD_WriteChar(MyI2C_LCD,'3');
        Password[i] = 3;
        i++;
    }
    if(gu8_KeyStatesArr[KEY_4] == KEY_PRESSED)
    {
        HAL_Delay(100);
        I2C_LCD_WriteChar(MyI2C_LCD,'4');
        Password[i] = 4;
        i++;
    }
    if(gu8_KeyStatesArr[KEY_5] == KEY_PRESSED)
    {
        HAL_Delay(100);
        I2C_LCD_WriteChar(MyI2C_LCD,'5');
        Password [i] = 5;
        i++;
    }
    if(gu8_KeyStatesArr[KEY_6] == KEY_PRESSED)
    {
        HAL_Delay(100);
        I2C_LCD_WriteChar(MyI2C_LCD,'6');
        Password [i] = 6;
        i++;
    }
    if(gu8_KeyStatesArr[KEY_7] == KEY_PRESSED)
    {
        HAL_Delay(100);
        I2C_LCD_WriteChar(MyI2C_LCD,'7');
        Password [i] =7;
        i++;
    }
    if(gu8_KeyStatesArr[KEY_8] == KEY_PRESSED)
    {
        HAL_Delay(100);
        I2C_LCD_WriteChar(MyI2C_LCD,'8');
        Password [i] = 8;
        i++;
    }
    if(gu8_KeyStatesArr[KEY_9] == KEY_PRESSED)
    {
        HAL_Delay(100);
        I2C_LCD_WriteChar(MyI2C_LCD,'9');
        Password [i] = 9;
        i++;
    }
    if(gu8_KeyStatesArr[KEY_0] == KEY_PRESSED)
    {
        HAL_Delay(100);
        I2C_LCD_WriteChar(MyI2C_LCD,'0');
        Password [i] = 0;
        i++;
    }
    if(gu8_KeyStatesArr[KEY_F4] == KEY_PRESSED)
    {
        I2C_LCD_Clear(MyI2C_LCD);
        memset(Password, 0, i*sizeof(Password));
        i = 0;
        b = 0;
        HAL_Delay(100);
        main();
    }
}

static uchar CompareUID(uchar *scannedUID, uchar *storedUID)
{
    for (uchar i = 0; i < 4; i++)
    {
        if (scannedUID[i] != storedUID[i])
        {
            return 0;
        }
    }
    return 1;
}

static uchar IsSlaveCard(uchar *scannedUID)
{
    for (uint8_t i = 0; i < numSlaveCards; i++)
    {
        if (CompareUID(scannedUID, slaveCards[i]))
        {
            return 1;
        }
    }
    return 0;
}

static void AddSlaveCard(uchar *newUID)
{
    if (numSlaveCards < MAX_SLAVE_CARDS)
    {
        for (uint8_t i = 0; i < 4; i++)
        {
            slaveCards[numSlaveCards][i] = newUID[i];
        }
        numSlaveCards++;
        I2C_LCD_Clear(MyI2C_LCD);
        I2C_LCD_WriteString(MyI2C_LCD, "Them Thanh Cong");
        Tone(659, 200); // E5
        Tone(784, 200); // G5
        noTone();
        HAL_Delay(1000);
    }
    else
    {
        I2C_LCD_Clear(MyI2C_LCD);
        I2C_LCD_WriteString(MyI2C_LCD, "Danh Sach Day");
        Tone(392, 500); // G4
        noTone();
        HAL_Delay(1000);
    }
}

static void RFID_ProcessCard(void)
{
    uchar status;
    uchar tagType[2];
    uchar serNum[5];

    if (addCardMode)
    {
        // Step 1: Prompt for master card
        if (!masterAuthenticated)
        {
            I2C_LCD_Clear(MyI2C_LCD);
            I2C_LCD_WriteString(MyI2C_LCD, "Quet Master Card");
            status = MFRC522_Request(PICC_REQIDL, tagType);
            if (status == MI_OK)
            {
                status = MFRC522_Anticoll(serNum);
                if (status == MI_OK)
                {
                    if (CompareUID(serNum, masterCard))
                    {
                        masterAuthenticated = 1;
                        I2C_LCD_Clear(MyI2C_LCD);
                        I2C_LCD_WriteString(MyI2C_LCD, "Quet The Moi");
                        Tone(523, 200); // C5
                        noTone();
                        HAL_Delay(200);
                    }
                    else
                    {
                        I2C_LCD_Clear(MyI2C_LCD);
                        I2C_LCD_WriteString(MyI2C_LCD, "Sai Master Card");
                        Tone(392, 500); // G4
                        noTone();
                        HAL_Delay(1000);
                        I2C_LCD_Clear(MyI2C_LCD);
                        I2C_LCD_WriteString(MyI2C_LCD, "Quet Master Card");
                    }
                    MFRC522_Halt();
                }
            }
        }
        // Step 2: Add new slave card
        else
        {
            status = MFRC522_Request(PICC_REQIDL, tagType);
            if (status == MI_OK)
            {
                status = MFRC522_Anticoll(serNum);
                if (status == MI_OK)
                {
                    // Check if card is not already a slave card
                    if (!IsSlaveCard(serNum) && !CompareUID(serNum, masterCard))
                    {
                        AddSlaveCard(serNum);
                    }
                    else
                    {
                        I2C_LCD_Clear(MyI2C_LCD);
                        I2C_LCD_WriteString(MyI2C_LCD, "The Da Ton Tai");
                        Tone(392, 500); // G4
                        noTone();
                        HAL_Delay(1000);
                    }
                    // Exit add card mode
                    addCardMode = 0;
                    masterAuthenticated = 0;
                    I2C_LCD_Clear(MyI2C_LCD);
                    I2C_LCD_WriteString(MyI2C_LCD, "Nhap mat khau:");
                    I2C_LCD_SetCursor(MyI2C_LCD, 1, 1);
                    MFRC522_Halt();
                }
            }
        }
    }
    else
    {
        // Normal mode: Check for slave cards
        status = MFRC522_Request(PICC_REQIDL, tagType);
        if (status == MI_OK)
        {
            status = MFRC522_Anticoll(serNum);
            if (status == MI_OK)
            {
                if (IsSlaveCard(serNum))
                {
                    I2C_LCD_Clear(MyI2C_LCD);
                    I2C_LCD_WriteString(MyI2C_LCD, "Welcome");
                    SERVO_MoveTo(SERVO_Motor1, 90);
                    Tone(659, 200); // E5
                    Tone(784, 200); // G5
                    noTone();
                    HAL_Delay(1500);
                    SERVO_MoveTo(SERVO_Motor1, 0);
                    I2C_LCD_Clear(MyI2C_LCD);
                    I2C_LCD_WriteString(MyI2C_LCD, "Nhap mat khau:");
                    I2C_LCD_SetCursor(MyI2C_LCD, 1, 1);
                }
                else
                {
                    I2C_LCD_Clear(MyI2C_LCD);
                    I2C_LCD_WriteString(MyI2C_LCD, "Sai The");
                    Tone(392, 500); // G4
                    noTone();
                    HAL_Delay(1000);
                    I2C_LCD_Clear(MyI2C_LCD);
                    I2C_LCD_WriteString(MyI2C_LCD, "Nhap mat khau:");
                    I2C_LCD_SetCursor(MyI2C_LCD, 1, 1);
                }
                MFRC522_Halt();
            }
        }
    }
}

static void Tone(uint32_t Frequency, uint32_t Duration)
{
    TIM2->ARR = (1000000UL / Frequency) - 1; // Set The PWM Frequency
    TIM2->CCR3 = (TIM2->ARR >> 1); // Set Duty Cycle 50%
    HAL_Delay(Duration); // Wait For The Tone Duration
}

static void noTone()
{
    TIM2->CCR3 = 0; // Set Duty Cycle 0%
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_SPI2_Init();
  /* USER CODE BEGIN 2 */
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    SERVO_Init(SERVO_Motor1);
    I2C_LCD_Init(MyI2C_LCD);
    I2C_LCD_Clear(MyI2C_LCD);
    I2C_LCD_SetCursor(MyI2C_LCD, 0, 0);
    if (d == 0){
        I2C_LCD_Backlight(MyI2C_LCD);
        I2C_LCD_Display(MyI2C_LCD);
        I2C_LCD_WriteString(MyI2C_LCD, "Nhap mat khau:");
    }
    if (d == 1){
        I2C_LCD_WriteString(MyI2C_LCD, "Mat khau moi:");
    }
    I2C_LCD_SetCursor(MyI2C_LCD, 1, 1);
    KEYPAD_Init(0, gu8_KeyStatesArr);
    MFRC522_Init(); // Initialize RFID module
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while (1)
    {
        if(gu8_KeyStatesArr[KEY_F1] == KEY_PRESSED)
        {
            if(gu8_KeyStatesArr[KEY_F2] == KEY_PRESSED){
                b = 1;
                memset(Password, 0, i*sizeof(Password));
                I2C_LCD_Clear(MyI2C_LCD);
                I2C_LCD_WriteString(MyI2C_LCD, "Mat khau cu:");
                I2C_LCD_SetCursor(MyI2C_LCD, 1, 1);
                HAL_Delay(200); // Debounce
            }
        }
        if(gu8_KeyStatesArr[KEY_F3] == KEY_PRESSED)
        {
            addCardMode = 1;
            masterAuthenticated = 0;
            I2C_LCD_Clear(MyI2C_LCD);
            I2C_LCD_WriteString(MyI2C_LCD, "Quet Master Card");
            HAL_Delay(200); // Debounce

        }

        KEYPAD_Scan(0);
        HAL_Delay(10);
        if (d == 0 && !addCardMode){
            Display_Handler();
            HAL_Delay(10);
            Password_Check();
            Notifications();
        }
        if (d == 1)
        {
            Set_Password();
            HAL_Delay(100);
        }
        RFID_ProcessCard(); // Process RFID cards
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_SET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 71;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 65535;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */
  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA0 PA1 PA2 PA3 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA4 PA5 PA6 PA7 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PB12 */
  GPIO_InitStruct.Pin = GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  /* USER CODE END MX_GPIO_Init_2 */
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
