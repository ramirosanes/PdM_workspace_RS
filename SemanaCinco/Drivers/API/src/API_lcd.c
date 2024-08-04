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
/************************************
 * EXTERN VARIABLES
 ************************************/

/************************************
 * PRIVATE MACROS AND DEFINES
 ************************************/
//MISC
#define TX_TIMEOUT 100
#define LCD_CHARS 80
#define LINEMAXCHARS 20

#define SHIFT_NIBBLE_UP(x) (uint8_t) (x<<4)
#define HIGH_NIBBLE 0b11110000
#define LOW_NIBBLE  0b00001111
//I2C CONFIG PARAMS
#define CLOCKSPEED 100000
#define OWN_ADDRESS 0

/*-----------------------[COMMAND MACROS]-----------------------*/
//Cmd Bits
#define CLEARDISPLAY	(1<<0)
#define RETURNHOME		(1<<1)
#define ENTRYMODESET	(1<<2)
#define DISPLAYCONTROL	(1<<3)
#define CURSORDISPLAYSHIFT	(1<<4)
#define FUNCTIONSET			(1<<5)
#define SETCGRAM			(1<<6)
#define SETDDRAM			(1<<7)
//Ctrl Bits
#define RS_BIT				(1<<0)
#define RW_BIT				(1<<1)
#define EN_BIT				(1<<2)
#define BL_BIT				(1<<3)

#define LCD_COMMAND_MODE		0x00
#define LCD_DATA_MODE			0x01

#define LCD_CLEARDISPLAY        0x01
#define LCD_RETURNHOME          0x02
#define LCD_ENTRYMODESET        0x04
#define LCD_DISPLAYCONTROL      0x08
#define LCD_CURSORSHIFT         0x10
#define LCD_FUNCTIONSET         0x20
#define LCD_SETCGRAMADDR        0x40
#define LCD_SETDDRAMADDR        0x80
// DISPLAY ENTRY
#define LCD_ENTRYRIGHT          0x00
#define LCD_ENTRYLEFT           0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00
// DISPLAY CONTROL
#define LCD_DISPLAYON           0x04
#define LCD_DISPLAYOFF          0x00
#define LCD_CURSORON            0x02
#define LCD_CURSOROFF           0x00
#define LCD_BLINKON             0x01
#define LCD_BLINKOFF            0x00
// CURSOR MOTION
#define LCD_DISPLAYMOVE         0x08
#define LCD_CURSORMOVE          0x00
#define LCD_MOVERIGHT           0x04
#define LCD_MOVELEFT            0x00
// FUNCTION SET
#define LCD_8BITMODE            0x10
#define LCD_4BITMODE            0x00
#define LCD_2LINE               0x08
#define LCD_1LINE               0x00
#define LCD_5x10DOTS            0x04
#define LCD_5x8DOTS             0x00
// BACKLIGHT CONTROL
#define LCD_BACKLIGHT           0x08
#define LCD_NOBACKLIGHT         0x00
#define EN                      0b00000100  // Enable bit
#define RW                      0b00000010  // Read/Write bit
#define RS                      0b00000001  // Register select bit
/*-----------------------[END COMMAND MACROS]-----------------------*/

/************************************
 * PRIVATE TYPEDEFS
 ************************************/

/************************************
 * STATIC VARIABLES
 ************************************/
static I2C_HandleTypeDef hi2c1;

static const lcd_t lcdUser =
{
		.lcdHandle = &hi2c1,
		.lcdAddress = LCD_ADDRESS,
		.lcdCols = COLS,
		.lcdRows = ROWS,
};

uint8_t displayBuffer[LCD_CHARS] = {0x69};
/************************************
 * GLOBAL VARIABLES
 ************************************/

/************************************
 * STATIC FUNCTION PROTOTYPES
 ************************************/

/************************************
 * STATIC FUNCTIONS
 ************************************/
//lcd private functions
static void lcdExpanderWrite(uint8_t data)
{
	HAL_StatusTypeDef status = HAL_OK;
	uint8_t txData = (data) | LCD_BACKLIGHT;
	status = HAL_I2C_Master_Transmit(lcdUser.lcdHandle, (lcdUser.lcdAddress<<1), &txData, sizeof(txData), TX_TIMEOUT);
	if (status != HAL_OK)
	{
		Error_Handler();
	}
}
static void lcdEnToggle (uint8_t data)
{
	lcdExpanderWrite(data | EN);
	HAL_Delay(1);

	lcdExpanderWrite(data); //Se manda un byte mas "al pedo" pero necesario
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
	lcdWrite4Bits(highNibble | mode);
	lcdWrite4Bits(SHIFT_NIBBLE_UP(lowNibble) | mode);
}
static void lcdCmd (uint8_t cmd)
{
	lcdSend(cmd, LCD_COMMAND_MODE);
}
static void lcdData (uint8_t data)
{
	lcdSend(data, LCD_DATA_MODE);
}

//lcd i2c initialization private functions
static void MX_I2C1_Init(void)
{
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = CLOCKSPEED;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = OWN_ADDRESS;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = OWN_ADDRESS;
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
typedef enum
{
	DISPLAY,
	CONFIG,
}
display_t;

display_t displayState = DISPLAY;

void displayInit ()
{
	for (uint8_t i = 0; i < LCD_CHARS; i++)
	{
		displayBuffer[i] = ' ';
	}
}

static void writeLineDisplay (uint8_t* string, uint8_t row, uint8_t offset)
{
	uint8_t len = 0;
	uint8_t displayIndex;

	if ((row > 3) | (string == NULL) | (offset > LINEMAXCHARS))
	{
		Error_Handler();
	}

	displayIndex = row*LINEMAXCHARS + offset;
	while (*string != '\0' && (len+offset)<LINEMAXCHARS)
	{
		displayBuffer[displayIndex++] = *string;
		len++;
		string++;
	}
}

void lcdShowHighFlanks ()
{
	writeLineDisplay((uint8_t*)"FA: 5", 0, 0);
}
void lcdShowLowFlanks ()
{
	writeLineDisplay((uint8_t*)"FB: 3", 1, 0);
}

void lcdShowDate()
{
	writeLineDisplay((uint8_t*)"69", 2, 0);
}

void lcdShowTime()
{
	writeLineDisplay((uint8_t*)"mcdonal", 3, 0);
}

void displayFSM()
{
	switch (displayState)
	{
	case DISPLAY:
		lcdShowHighFlanks();
		lcdShowLowFlanks();
		lcdShowDate();
		lcdShowTime();
		lcdSetCursor(0, 0);
		lcdPrintString(displayBuffer);
		break;
	case CONFIG:
		break;
	default:
		break;
	}
}

void lcdInit ()
{
	MX_I2C1_Init();
	displayInit();
	lcdCmd(0x30);
	HAL_Delay(5);
	lcdCmd(0x30);
	HAL_Delay(5);
	lcdCmd(0x30);
	HAL_Delay(150);

	lcdCmd(0x02);
	lcdCmd(LCD_FUNCTIONSET | LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS);
	lcdCmd(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF);
	lcdCmd(LCD_ENTRYMODESET | LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT);

	lcdClear();
}
void lcdClear ()
{
	lcdCmd(LCD_CLEARDISPLAY);
	DELAY_MS(2);
}
void lcdHome ()
{
	lcdCmd(LCD_RETURNHOME);
	DELAY_MS(2);
}
void lcdSetCursor (uint8_t row, uint8_t col)
{
	uint8_t rowOffsets[] = {0x00, 0x40, 0x14, 0x54};
	if (row > lcdUser.lcdRows)
	{
		row = lcdUser.lcdRows -1;
	}
	lcdCmd(LCD_SETDDRAMADDR | (col + rowOffsets[row]));
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
void lcdDisplayOn ()
{
	lcdCmd(LCD_DISPLAYCONTROL | LCD_DISPLAYON);
}
void lcdDisplayOff ()
{
	lcdCmd(LCD_DISPLAYCONTROL | LCD_DISPLAYOFF);
}
void lcdCursorOn ()
{
	lcdCmd(LCD_DISPLAYCONTROL | LCD_CURSORON);
}
void lcdCursorOff ()
{
	lcdCmd(LCD_DISPLAYCONTROL | LCD_CURSOROFF);
}
void lcdBlinkOn ()
{
	lcdCmd(LCD_DISPLAYCONTROL | LCD_BLINKON);
}
void lcdBlinkOff ()
{
	lcdCmd(LCD_DISPLAYCONTROL | LCD_BLINKOFF);
}
