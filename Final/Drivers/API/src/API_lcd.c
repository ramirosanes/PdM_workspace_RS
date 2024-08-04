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
#define SHIFT_NIBBLE_UP(x) (uint8_t) (x<<4)
#define HIGH_NIBBLE 0xf0
#define LOW_NIBBLE  0x0f

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
static void lcdExpanderWrite(uint8_t data)
{
	uint8_t txData = (data) | BL_BIT;
	if (HAL_I2C_Master_Transmit(&hi2c1, (LCD_ADDRESS<<1), &txData, sizeof(txData), HAL_MAX_DELAY)!= HAL_OK){
		uartSendString((uint8_t*)"tx de lcd broken");
	}
}
static void lcdEnToggle (uint8_t data)
{
	lcdExpanderWrite( (data) | EN_BIT);
	HAL_Delay(1);

	lcdExpanderWrite(data);
	HAL_Delay(1);
}
static void lcdWrite4Bits (uint8_t data)
{
	lcdExpanderWrite(data);
	lcdEnToggle(data);
}
static void lcdSend (uint8_t data, uint8_t mode)
{
	uint8_t highNibble = data & HIGH_NIBBLE;
	uint8_t lowNibble = data & LOW_NIBBLE;
	lcdWrite4Bits((highNibble) | (mode));
	lcdWrite4Bits((SHIFT_NIBBLE_UP(lowNibble)) | mode);
}
static void lcdCmd (uint8_t cmd)
{
	lcdSend(cmd, COMMAND_MODE);
}
static void lcdData (uint8_t data)
{
	lcdSend(data, DATA_MODE);
}
//HAL
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

	lcdCmd(0x30);
	HAL_Delay(5);

	lcdCmd(0x30);
	HAL_Delay(1);

	lcdCmd(0x30);
	HAL_Delay(1);

	lcdCmd(0x20);
	HAL_Delay(1);

	lcdCmd(FUNCTIONSET | MODE_4BITMODE| LINES_2LINE | FONT_5x8DOTS);
	HAL_Delay(1);

	lcdCmd(DISPLAYCONTROL | DISPLAYOFF | CURSOROFF | BLINKOFF);
	HAL_Delay(1);

	lcdCmd(0x01);
	HAL_Delay(1);

	lcdCmd(ENTRYMODESET | ENTRYLEFT | SHIFTINCREMENT);
	HAL_Delay(1);

	lcdHome();
	lcdPrintString("la concha de tu madre");

}
void lcdClear ()
{
	lcdCmd(CLEARDISPLAY);
	HAL_Delay(2);
}
void lcdHome ()
{
	lcdCmd(RETURNHOME);
	HAL_Delay(2);
}
void lcdSetCursor (uint8_t row, uint8_t col)
{
	uint8_t rowOffsets[] = {0x00, 0x40, 0x14, 0x54};
	row = row % ROWS;
	lcdCmd(SETDDRAM | (col + rowOffsets[row]));
}
void lcdDisplayOn ()
{
	lcdCmd(DISPLAYCONTROL | DISPLAYON);
}
void lcdDisplayOff ()
{
	lcdCmd(DISPLAYCONTROL | DISPLAYOFF);
}
void lcdCursorOn ()
{
	lcdCmd(DISPLAYCONTROL | CURSORON);
}
void lcdCursorOff ()
{
	lcdCmd(DISPLAYCONTROL | CURSOROFF);
}
void lcdBlinkOn ()
{
	lcdCmd(DISPLAYCONTROL | BLINKON);
}
void lcdBlinkOff ()
{
	lcdCmd(DISPLAYCONTROL | BLINKOFF);
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
	lcdData(c);
}
void lcdPrintString (uint8_t* str)
{
	while (*str != '\0')
	{
		lcdData(*str++);
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
