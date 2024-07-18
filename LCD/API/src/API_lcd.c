/*
 * File: API_LCD.c
 * Driver Name: [[ I2C_LCD Display ]]
 * SW Layer:   API
 * Created on: Jan 28, 2024
 * Author:     Khaled Magdy
 * -------------------------------------------
 * For More Information, Tutorials, etc.
 * Visit Website: www.DeepBlueMbedded.com
 *
 */

#include "API_lcd.h"
#include "stm32f4xx_hal.h"
#include "utils.h"

#include <stdlib.h>
#include <string.h>



/*-----------------------[INTERNAL VARIABLES]-----------------------*/

extern I2C_HandleTypeDef hi2c1;

//-----[ Chanchadas, entrevero, desasosiego ]-----

typedef struct I2C_LCD_InfoParam_s
{
    uint8_t DisplayCtrl;
    uint8_t BacklightVal;
}
I2C_LCD_InfoParam_t;

typedef struct
{
	// I2C LCD Module Instance Index
	uint8_t I2C_LCD_Instance;

	// I2C Hardware Peripheral Handle
	I2C_HandleTypeDef* I2C_Handle;

	// I2C LCD Hardware Device Address
	uint8_t I2C_LCD_Address;

	// I2C LCD Columns Count
	uint8_t I2C_LCD_nCol;

	// I2C LCD Rows Count
	uint8_t I2C_LCD_nRow;

}
I2C_LCD_CfgType;


// VARIABLES DE CONFIGURACION DE USUARIO QUE NO DEBERIAN ESTAR ACA, PERO SOLO ASI FUNCIONA :D

I2C_LCD_InfoParam_t I2C_LCD_InfoParam_g[I2C_LCD_MAX];

const I2C_LCD_CfgType I2C_LCD_CfgParam[I2C_LCD_MAX] =
{
	{   /*  Configuration Parameter For I2C_LCD Instance #1   */
		I2C_LCD_1,	/* Index of I2C_LCD Instance #1           */
		&hi2c1,		/* Hardware I2C Module's Handle           */
		0x27,		/* Hardware I2C_LCD Device Address        */
		16,			/* LCD Columns Count                      */
		2			/* LCD Rows Count                         */
	}
};

/*---------------------[STATIC INTERNAL FUNCTIONS]-----------------------*/

static void I2C_LCD_ExpanderWrite(uint8_t I2C_LCD_InstanceIndex, uint8_t DATA)
{
    uint8_t TxData = (DATA) | I2C_LCD_InfoParam_g[I2C_LCD_InstanceIndex].BacklightVal;
    HAL_I2C_Master_Transmit(I2C_LCD_CfgParam[I2C_LCD_InstanceIndex].I2C_Handle, (I2C_LCD_CfgParam[I2C_LCD_InstanceIndex].I2C_LCD_Address<<1), &TxData, sizeof(TxData), 100);
}

static void I2C_LCD_EnPulse(uint8_t I2C_LCD_InstanceIndex, uint8_t DATA)
{
	I2C_LCD_ExpanderWrite(I2C_LCD_InstanceIndex, (DATA | EN)); // En high
	DELAY_US(2); // enable pulse must be >450ns

    I2C_LCD_ExpanderWrite(I2C_LCD_InstanceIndex, (DATA & ~EN)); // En low
    DELAY_US(50); // commands need > 37us to settle
}

static void I2C_LCD_Write4Bits(uint8_t I2C_LCD_InstanceIndex, uint8_t Val)
{
	I2C_LCD_ExpanderWrite(I2C_LCD_InstanceIndex, Val);
	I2C_LCD_EnPulse(I2C_LCD_InstanceIndex, Val);
}

static void I2C_LCD_Send(uint8_t I2C_LCD_InstanceIndex, uint8_t Val, uint8_t Mode)
{
    uint8_t HighNib = Val & 0xF0;
    uint8_t LowNib = (Val << 4) & 0xF0;
    I2C_LCD_Write4Bits(I2C_LCD_InstanceIndex, (HighNib) | Mode);
    I2C_LCD_Write4Bits(I2C_LCD_InstanceIndex, (LowNib) | Mode);
}

static void I2C_LCD_Cmd(uint8_t I2C_LCD_InstanceIndex, uint8_t CMD)
{
	I2C_LCD_Send(I2C_LCD_InstanceIndex, CMD, 0);
}

static void I2C_LCD_Data(uint8_t I2C_LCD_InstanceIndex, uint8_t DATA)
{
	I2C_LCD_Send(I2C_LCD_InstanceIndex, DATA, 1);
}

/*-----------------------------------------------------------------------*/

//=========================================================================================================================

/*-----------------------[USER EXTERNAL FUNCTIONS]-----------------------*/

void I2C_LCD_Init(uint8_t I2C_LCD_InstanceIndex)
{
	// According To Datasheet, We Must Wait At Least 40ms After Power Up Before Interacting With The LCD Module
	while(HAL_GetTick() < 50);
    I2C_LCD_Cmd(I2C_LCD_InstanceIndex, 0x30);
    DELAY_MS(5);  // Delay > 4.1ms
    I2C_LCD_Cmd(I2C_LCD_InstanceIndex, 0x30);
    DELAY_MS(5);  // Delay > 4.1ms
    I2C_LCD_Cmd(I2C_LCD_InstanceIndex, 0x30);
    DELAY_US(150);  // Delay > 100μs
    I2C_LCD_Cmd(I2C_LCD_InstanceIndex, 0x02);
    // Configure the LCD
    I2C_LCD_Cmd(I2C_LCD_InstanceIndex, LCD_FUNCTIONSET | LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS);
    I2C_LCD_Cmd(I2C_LCD_InstanceIndex, LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF);
    I2C_LCD_Cmd(I2C_LCD_InstanceIndex, LCD_ENTRYMODESET | LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT);
    I2C_LCD_InfoParam_g[I2C_LCD_InstanceIndex].DisplayCtrl = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    I2C_LCD_InfoParam_g[I2C_LCD_InstanceIndex].BacklightVal = LCD_BACKLIGHT;
    // Clear the LCD
    I2C_LCD_Clear(I2C_LCD_InstanceIndex);
}

void I2C_LCD_Clear(uint8_t I2C_LCD_InstanceIndex)
{
    I2C_LCD_Cmd(I2C_LCD_InstanceIndex, LCD_CLEARDISPLAY);
    DELAY_MS(2);
}

void I2C_LCD_Home(uint8_t I2C_LCD_InstanceIndex)
{
    I2C_LCD_Cmd(I2C_LCD_InstanceIndex, LCD_RETURNHOME);
    DELAY_MS(2);
}

void I2C_LCD_SetCursor(uint8_t I2C_LCD_InstanceIndex, uint8_t Col, uint8_t Row)
{
    int Row_Offsets[] = {0x00, 0x40, 0x14, 0x54};
    if (Row > I2C_LCD_CfgParam[I2C_LCD_InstanceIndex].I2C_LCD_nRow)
    {
    	Row = I2C_LCD_CfgParam[I2C_LCD_InstanceIndex].I2C_LCD_nRow - 1;
    }
    I2C_LCD_Cmd(I2C_LCD_InstanceIndex, LCD_SETDDRAMADDR | (Col + Row_Offsets[Row]));
}

void I2C_LCD_WriteChar(uint8_t I2C_LCD_InstanceIndex, char Ch)
{
    I2C_LCD_Data(I2C_LCD_InstanceIndex, Ch);
}

void I2C_LCD_WriteString(uint8_t I2C_LCD_InstanceIndex, char *Str)
{
    while (*Str)
    {
        I2C_LCD_Data(I2C_LCD_InstanceIndex, *Str++);
    }
}

void I2C_LCD_ShiftLeft(uint8_t I2C_LCD_InstanceIndex)
{
    I2C_LCD_Cmd(I2C_LCD_InstanceIndex, LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

void I2C_LCD_ShiftRight(uint8_t I2C_LCD_InstanceIndex)
{
    I2C_LCD_Cmd(I2C_LCD_InstanceIndex, LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

void I2C_LCD_Backlight(uint8_t I2C_LCD_InstanceIndex)
{
	I2C_LCD_InfoParam_g[I2C_LCD_InstanceIndex].BacklightVal = LCD_BACKLIGHT;
    I2C_LCD_ExpanderWrite(I2C_LCD_InstanceIndex, 0);
}

void I2C_LCD_NoBacklight(uint8_t I2C_LCD_InstanceIndex)
{
	I2C_LCD_InfoParam_g[I2C_LCD_InstanceIndex].BacklightVal = LCD_NOBACKLIGHT;
    I2C_LCD_ExpanderWrite(I2C_LCD_InstanceIndex, 0);
}

void I2C_LCD_Display(uint8_t I2C_LCD_InstanceIndex)
{
	I2C_LCD_InfoParam_g[I2C_LCD_InstanceIndex].DisplayCtrl |= LCD_DISPLAYON;
	I2C_LCD_Cmd(I2C_LCD_InstanceIndex, LCD_DISPLAYCONTROL | I2C_LCD_InfoParam_g[I2C_LCD_InstanceIndex].DisplayCtrl);
}

void I2C_LCD_NoDisplay(uint8_t I2C_LCD_InstanceIndex)
{
	I2C_LCD_InfoParam_g[I2C_LCD_InstanceIndex].DisplayCtrl &= ~LCD_DISPLAYON;
	I2C_LCD_Cmd(I2C_LCD_InstanceIndex, LCD_DISPLAYCONTROL | I2C_LCD_InfoParam_g[I2C_LCD_InstanceIndex].DisplayCtrl);
}

void I2C_LCD_Cursor(uint8_t I2C_LCD_InstanceIndex)
{
	I2C_LCD_InfoParam_g[I2C_LCD_InstanceIndex].DisplayCtrl |= LCD_CURSORON;
	I2C_LCD_Cmd(I2C_LCD_InstanceIndex, LCD_DISPLAYCONTROL | I2C_LCD_InfoParam_g[I2C_LCD_InstanceIndex].DisplayCtrl);
}

void I2C_LCD_NoCursor(uint8_t I2C_LCD_InstanceIndex)
{
	I2C_LCD_InfoParam_g[I2C_LCD_InstanceIndex].DisplayCtrl &= ~LCD_CURSORON;
	I2C_LCD_Cmd(I2C_LCD_InstanceIndex, LCD_DISPLAYCONTROL | I2C_LCD_InfoParam_g[I2C_LCD_InstanceIndex].DisplayCtrl);
}

void I2C_LCD_Blink(uint8_t I2C_LCD_InstanceIndex)
{
	I2C_LCD_InfoParam_g[I2C_LCD_InstanceIndex].DisplayCtrl |= LCD_BLINKON;
	I2C_LCD_Cmd(I2C_LCD_InstanceIndex, LCD_DISPLAYCONTROL | I2C_LCD_InfoParam_g[I2C_LCD_InstanceIndex].DisplayCtrl);
}

void I2C_LCD_NoBlink(uint8_t I2C_LCD_InstanceIndex)
{
	I2C_LCD_InfoParam_g[I2C_LCD_InstanceIndex].DisplayCtrl &= ~LCD_BLINKON;
	I2C_LCD_Cmd(I2C_LCD_InstanceIndex, LCD_DISPLAYCONTROL | I2C_LCD_InfoParam_g[I2C_LCD_InstanceIndex].DisplayCtrl);
}

void I2C_LCD_CreateCustomChar(uint8_t I2C_LCD_InstanceIndex, uint8_t CharIndex, const uint8_t* CharMap)
{
    CharIndex &= 0x07;
    I2C_LCD_Cmd(I2C_LCD_InstanceIndex, LCD_SETCGRAMADDR | (CharIndex << 3));
    for (int i = 0; i < 8; i++)
    {
    	I2C_LCD_Send(I2C_LCD_InstanceIndex, CharMap[i], RS);
    }
}

void I2C_LCD_PrintCustomChar(uint8_t I2C_LCD_InstanceIndex, uint8_t CharIndex)
{
	I2C_LCD_Send(I2C_LCD_InstanceIndex, CharIndex, RS);
}
