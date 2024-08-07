/**
 ********************************************************************************
 * @file    API_lcd.c
 * @author  rami
 * @date    Jul 29, 2024
 * @brief   
 ********************************************************************************
 */

/************************************
 * INCLUDES
 ************************************/
#include "API_lcd.h"
#include "API_uart.h"
/************************************
 * EXTERN VARIABLES
 ************************************/

/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/
//Ctrl Bits
#define RS_BIT				(1<<0)
#define RW_BIT				(1<<1)
#define EN_BIT				(1<<2)
#define BL_BIT				(1<<3)
//CMD or Data Mode
#define COMMAND_MODE		(0<<0)
#define DATA_MODE			(1<<0)
//CMD Bits
#define CLEARDISPLAY		(1<<0)
#define RETURNHOME			(1<<1)
#define ENTRYMODESET		(1<<2)
#define DISPLAYCONTROL		(1<<3)
#define CURSORDISPLAYSHIFT	(1<<4)
#define FUNCTIONSET			(1<<5)
#define SETCGRAM			(1<<6)
#define SETDDRAM			(1<<7)
// CMD_Entry ctrl
#define SHIFTDECREMENT 		(0<<0)
#define SHIFTINCREMENT 		(1<<0)
#define ENTRYRIGHT          (0<<1)
#define ENTRYLEFT			(1<<1)
// CMD_Display ctrl
#define BLINKOFF            (0<<0)
#define BLINKON             (1<<0)
#define CURSOROFF           (0<<1)
#define CURSORON            (1<<1)
#define DISPLAYOFF			(0<<2)
#define DISPLAYON           (1<<2)
//CMD_Cursor ctrl
#define MOVELEFT			(0<<2)
#define MOVERIGHT			(1<<2)
#define CURSORMOVE          (0<<3)
#define DISPLAYMOVE			(1<<3)
//CMD_Function set ctrl
#define FONT_5x8DOTS        (0<<2)
#define FONT_5x10DOTS       (1<<2)
#define LINES_1LINE			(0<<3)
#define LINES_2LINE         (1<<3)
#define MODE_4BITMODE       (0<<4)
#define MODE_8BITMODE       (1<<4)
/************************************
 * PRIVATE TYPEDEFS
 ************************************/

/************************************
 * STATIC VARIABLES
 ************************************/

/************************************
 * GLOBAL VARIABLES
 ************************************/
I2C_HandleTypeDef hi2c1;
/************************************
 * STATIC FUNCTION PROTOTYPES
 ************************************/

/************************************
 * STATIC FUNCTIONS
 ************************************/
static void lcdSendCommand (uint8_t cmd)
{
	uint8_t highNibble, lowNibble;
	uint8_t txData[4];

	highNibble = (cmd & 0xF0) | BL_BIT;
	lowNibble = ((cmd<<4) & 0xF0) | BL_BIT;

	txData[0] = highNibble | EN_BIT;
	txData[1] = highNibble;
	txData[2] = lowNibble | EN_BIT;
	txData[3] = lowNibble;

	for (uint8_t i = 0; i<sizeof(txData); i++)
	{
		if ((HAL_I2C_Master_Transmit(&hi2c1, (LCD_ADDRESS<<1), (txData+i), 1, HAL_MAX_DELAY)) != HAL_OK)
		{
			uartSendString((uint8_t)"tx de lcd broken");
		}
	}
	HAL_Delay(5);
}

static void lcdSendData (uint8_t data)
{
	uint8_t highNibble, lowNibble;
	uint8_t txData[4];

	highNibble = (data & 0xF0) | BL_BIT | RS_BIT;
	lowNibble = ((data<<4) & 0xF0) | BL_BIT | RS_BIT;

	txData[0] = highNibble | EN_BIT;
	txData[1] = highNibble;
	txData[2] = lowNibble | EN_BIT;
	txData[3] = lowNibble;

	for (uint8_t i = 0; i<sizeof(txData); i++)
	{
		if ((HAL_I2C_Master_Transmit(&hi2c1, (LCD_ADDRESS<<1), (txData+i), 1, HAL_MAX_DELAY)) != HAL_OK)
		{
			uartSendString((uint8_t)"tx de lcd broken");
		}
	}
	HAL_Delay(5);
}

static void MX_I2C1_Init(void)
{
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 10000;
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

  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
}

/************************************
 * GLOBAL FUNCTIONS
 ************************************/
void lcdInit ()
{
	MX_I2C1_Init();
	HAL_Delay(100);

	HAL_I2C_Master_Transmit(&hi2c1, (LCD_ADDRESS<<1), (uint8_t[]){(0x30) | EN_BIT | BL_BIT}, 1, HAL_MAX_DELAY);
	HAL_Delay(1);
	HAL_I2C_Master_Transmit(&hi2c1, (LCD_ADDRESS<<1), (uint8_t[]){0x30}, 1, HAL_MAX_DELAY);
	HAL_Delay(20);

	HAL_I2C_Master_Transmit(&hi2c1, (LCD_ADDRESS<<1), (uint8_t[]){(0x30) | EN_BIT | BL_BIT}, 1, HAL_MAX_DELAY);
	HAL_Delay(1);
	HAL_I2C_Master_Transmit(&hi2c1, (LCD_ADDRESS<<1), (uint8_t[]){0x30}, 1, HAL_MAX_DELAY);
	HAL_Delay(10);

	HAL_I2C_Master_Transmit(&hi2c1, (LCD_ADDRESS<<1), (uint8_t[]){(0x30) | EN_BIT | BL_BIT}, 1, HAL_MAX_DELAY);
	HAL_Delay(1);
	HAL_I2C_Master_Transmit(&hi2c1, (LCD_ADDRESS<<1), (uint8_t[]){0x30}, 1, HAL_MAX_DELAY);
	HAL_Delay(1);

	HAL_I2C_Master_Transmit(&hi2c1, (LCD_ADDRESS<<1), (uint8_t[]){(0x20) | EN_BIT | BL_BIT}, 1, HAL_MAX_DELAY);
	HAL_Delay(1);
	HAL_I2C_Master_Transmit(&hi2c1, (LCD_ADDRESS<<1), (uint8_t[]){0x20}, 1, HAL_MAX_DELAY);
	HAL_Delay(1);

	//End of hardcoded 8bit to 4Bit initialization, now ill start wrapping commands
	lcdSendCommand(FUNCTIONSET | MODE_4BITMODE| LINES_2LINE | FONT_5x8DOTS);
	HAL_Delay(1);
	lcdSendCommand(DISPLAYCONTROL);
	HAL_Delay(1);

	lcdSendCommand(RETURNHOME);
	lcdSendCommand(ENTRYMODESET | ENTRYLEFT | SHIFTDECREMENT);
	lcdSendCommand(DISPLAYCONTROL | DISPLAYON | CURSOROFF | BLINKOFF);
	lcdClear();
	lcdHome();
}
void lcdClear ()
{
	lcdSendCommand(CLEARDISPLAY);
	HAL_Delay(5);
}
void lcdHome ()
{
	lcdSendCommand(RETURNHOME);
	HAL_Delay(5);
}
void lcdSetCursor (uint8_t row, uint8_t col)
{
	uint8_t rowOffsets[] = {0x00, 0x40, 0x14, 0x54};
	row = row % ROWS;
	lcdSendCommand(SETDDRAM | (col + rowOffsets[row]));
	HAL_Delay(5);
}
void lcdDisplayOn ()
{
	lcdSendCommand(DISPLAYCONTROL | DISPLAYON);
	HAL_Delay(5);
}
void lcdDisplayOff ()
{
	lcdSendCommand(DISPLAYCONTROL | DISPLAYOFF);
	HAL_Delay(5);
}
void lcdCursorOn ()
{
	lcdSendCommand(DISPLAYCONTROL | CURSORON);
	HAL_Delay(5);
}
void lcdCursorOff ()
{
	lcdSendCommand(DISPLAYCONTROL | CURSOROFF);
	HAL_Delay(5);
}
void lcdBlinkOn ()
{
	lcdSendCommand(DISPLAYCONTROL | BLINKON);
	HAL_Delay(5);
}
void lcdBlinkOff ()
{
	lcdSendCommand(DISPLAYCONTROL | BLINKOFF);
	HAL_Delay(5);
}
void lcdPrintInt(int16_t num)
{
    uint8_t str[3];  // Buffer for the converted string
    uint8_t i = 0;
    bool_t isNegative = false;

    // Handle negative numbers
    if (num < 0) {
        isNegative = true;
        num = -num;
    }

    // Convert to string (in reverse order)
    do {
        str[i++] = (num % 10) + '0'; // Get digit and convert to character
        num /= 10;
    } while (num > 0);

    if (isNegative) {
        str[i++] = '-';
    }

    str[i] = '\0'; // Add null terminator

    // Reverse the string
    for (uint8_t j = 0, k = i - 1; j < k; j++, k--) {
        char temp = str[j];
        str[j] = str[k];
        str[k] = temp;
    }

    lcdPrintString((uint8_t*)str); // Write directly to the current position
}
void lcdPrintChar (uint8_t c)
{
	lcdSendData(c);
}
void lcdPrintString (uint8_t* str)
{
	while (*str != '\0')
	{
		lcdSendData(*str++);
	}
}
void lcdPrintf(const uint8_t* format, uint8_t c, int16_t num, const uint8_t* str)
{
    while (*format != '\0')
    {
        if (*format == '%')
        {
            format++;
            switch (*format)
            {
                case 'c':
                    lcdPrintChar(c);
                    break;
                case 'd':
                    lcdPrintInt(num);
                    break;
                case 's':
                    lcdPrintString((uint8_t*)str);
                    break;
            }
        }
        else
        {
            lcdPrintChar(*format);
        }
        format++;
    }
}
void lcdPrintfCenteredString(uint8_t* str, uint8_t c, int16_t num, const uint8_t* string)
{
    if (str == NULL) {
        uartSendString((uint8_t*)"error printeo lcd\n\r");
        return;
    }

    uint8_t strLen = 0;
    while (str[strLen] != '\0' && strLen < COLS) {
        strLen++;
    }

    uint8_t paddingBefore = (COLS - strLen) / 2; // Integer division for even padding

    // Write padding (spaces) before the string
    for (uint8_t i = 0; i < paddingBefore; i++) {
        lcdPrintChar(' ');
    }

    lcdPrintf(str, c, num, string);

    // Write padding (spaces) after the string
    uint8_t paddingAfter = COLS - strLen - paddingBefore;
    for (uint8_t i = 0; i < paddingAfter; i++) {
        lcdPrintChar(' ');
    }
}

void lcdPrintfLine (uint8_t* str, uint8_t c, int16_t num, const uint8_t* string)
{

}
