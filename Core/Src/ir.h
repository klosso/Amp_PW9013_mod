#pragma once

#include "stm32f1xx.h"

#define IR_CHANEL_IN TIM_CHANNEL_2

/*
// Stałe opisujące klawisze
#define IR_CODE_ONOFF      0x45
#define IR_CODE_MENU       0x47
#define IR_CODE_TEST       0x44
#define IR_CODE_PLUS       0x40
#define IR_CODE_BACK       0x43
#define IR_CODE_REWIND     0x07
#define IR_CODE_PLAY       0x15
#define IR_CODE_FORWARD    0x09
#define IR_CODE_0          0x16
#define IR_CODE_MINUS      0x19
#define IR_CODE_CANCEL     0x0D
#define IR_CODE_1          0x0C
#define IR_CODE_2          0x18
#define IR_CODE_3          0x5E
#define IR_CODE_4          0x08
#define IR_CODE_5          0x1C
#define IR_CODE_6          0x5A
#define IR_CODE_7          0x42
#define IR_CODE_8          0x52
#define IR_CODE_9          0x4A
*/

//YAMACHA IR codes
#define IR_CODE_VOL_UP      0x1A
#define IR_CODE_VOL_DW      0x1B
#define IR_CODE_MUTE        0x1C
#define IR_CODE_CD          0x15
#define IR_CODE_MD          0xC9
#define IR_CODE_TUNER       0x16
#define IR_CODE_MULTI       0x87
#define IR_CODE_DVD         0xC1
#define IR_CODE_DTV         0x54
#define IR_CODE_DVR         0x13
#define IR_CODE_AUX         0x55

#define IR_CODE_UP          0x9D
#define IR_CODE_DOWN        0x9C
#define IR_CODE_LEFT        0x9F
#define IR_CODE_RIGHT       0x9E
#define IR_CODE_ENTER       0xDE
#define IR_CODE_POWER       0x15
#define IR_CODE_STBY        0x15
#define IR_CODE_1           0x00
#define IR_CODE_2           0x03
#define IR_CODE_3           0x06
#define IR_CODE_4           0x09


#ifdef __cplusplus
extern "C" {
#endif


// Procedura obsługi przerwania
void ir_tim_interrupt(TIM_HandleTypeDef *htim);

// Inicjalizacja modułu
void ir_init(TIM_HandleTypeDef *htim);

// Funkcja odczytująca dane
// return - kod klawisza lub -1
int ir_read(void);

#ifdef __cplusplus
}
#endif

