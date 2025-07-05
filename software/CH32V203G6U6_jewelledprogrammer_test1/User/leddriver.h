#include "i2cdriver.h"
#ifndef LEDDRIVER_H
#define LEDDRIVER_H

#define NUMLEDS 18

// Register definitions
#define LEDDRIVER_SSD 0x00
typedef enum {
    LEDDRIVER_SSD_ENABLE = 0x00,
    LEDDRIVER_SSD_DISABLED = 0x01
} LedDriverSSD_Typedef;

#define LEDDRIVER_PWM_OFFSET 0x01
#define LEDDRIVER_PWM_LENGTH 0x12

#define LEDDRIVER_CURR_OFFSET 0x14
#define LEDDRIVER_CURR_LENGTH 0x12
typedef enum {
    LEDDRIVER_CURR_12_12_IMAX = 0x10,
    LEDDRIVER_CURR_11_12_IMAX = 0x11,
    LEDDRIVER_CURR_9_12_IMAX = 0x12,
    LEDDRIVER_CURR_7_12_IMAX = 0x13,
    LEDDRIVER_CURR_0_12_IMAX = 0x00
} LedDriverCurrent_Typedef;

#define LEDDRIVER_GC 0x26
typedef enum {
    LEDDRIVER_GC_NORMAL = 0x00,
    LEDDRIVER_GC_ALL_OFF = 0x01
} LedDriverGC_Typedef;

#define LEDDRIVER_FREQ 0x27
typedef enum {
    LEDDRIVER_FREQ_23 = 0x00,
    LEDDRIVER_FREQ_3_45 = 0x01
} LedDriverFreq_Typedef;

#define LEDDRIVER_RESET 0x2F

typedef enum {
    LEDDRIVER_NO_GAMME_CORRECTION = 0x00,
    LEDDRIVER_GAMME_CORRECTION = 0x01
} LedDriverGammaTable_Typedef;

// Struct to represent the LED driver
typedef struct {
    I2CDriver* i2cdrv;
    u8 led_buffer[NUMLEDS];
    u8 useGammaTable;
//    void (*init)(int pin);    // Function pointer for initialization
//    void (*turnOn)(int pin);  // Function pointer to turn on the LED
//    void (*turnOff)(int pin); // Function pointer to turn off the LED
} LedDriver;

// Function declarations
void LedDriver_Init(LedDriver* driver);
void LedDriver_WriteData(LedDriver* driver, u8* data, u8 length);
void LedDriver_WriteGlobalData(LedDriver* driver, u8 data);

void LedDriver_Update(LedDriver* driver);
void LedDriver_SetSoftwareShutdown(LedDriver* driver, LedDriverSSD_Typedef valSSD);
void LedDriver_SetCurrent(LedDriver* driver, LedDriverCurrent_Typedef* data, u8 length);
void LedDriver_SetGlobalCurrent(LedDriver* driver, LedDriverCurrent_Typedef valCurr);
void LedDriver_SetGlobalControl(LedDriver* driver, LedDriverGC_Typedef valGC);
void LedDriver_SetFrequency(LedDriver* driver, LedDriverFreq_Typedef valFreq);
void LedDriver_SetGammeTable(LedDriver* driver, LedDriverGammaTable_Typedef valGamma);
void LedDriver_Reset(LedDriver* driver);


#endif // LEDDRIVER_H
