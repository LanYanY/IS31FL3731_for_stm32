/**
  ******************************************************************************
  * @file           : IS31FL3731.h
  * @brief          : 用于STM32的IS31FL3731 LED驱动芯片库
  * @details        : 移植自Arduino LED_11x7_Matrix_IS31FL3731库
  ******************************************************************************
  */

#ifndef __IS31FL3731_H
#define __IS31FL3731_H

#ifdef __cplusplus
extern "C" {
#endif

/* 包含 ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* 宏定义 ------------------------------------------------------------------*/
#define ISSI_ADDR_DEFAULT (0x75 << 1)  // 7位地址左移一位，适应STM32 HAL I2C库

#define ISSI_REG_CONFIG           0x00
#define ISSI_REG_CONFIG_PICTUREMODE     0x00
#define ISSI_REG_CONFIG_AUTOPLAYMODE    0x08
#define ISSI_REG_CONFIG_AUDIOPLAYMODE   0x18

#define ISSI_CONF_PICTUREMODE      0x00
#define ISSI_CONF_AUTOFRAMEMODE    0x04
#define ISSI_CONF_AUDIOMODE        0x08

#define ISSI_REG_PICTUREFRAME      0x01

#define ISSI_REG_SHUTDOWN          0x0A
#define ISSI_REG_AUDIOSYNC         0x06

#define ISSI_COMMANDREGISTER       0xFD
#define ISSI_BANK_FUNCTIONREG      0x0B // 函数寄存器页

#define IS31_WIDTH                 16
#define IS31_HEIGHT                9

/* 结构体定义 ------------------------------------------------------------------*/
typedef struct {
  I2C_HandleTypeDef *hi2c;      // I2C句柄
  uint8_t addr;                 // 设备地址
  uint8_t frame;                // 当前帧（0-7）
  uint8_t rotate;               // 旋转方向 (0-3)
} IS31FL3731_HandleTypeDef;

/* 函数声明 ------------------------------------------------------------------*/
// 初始化函数
HAL_StatusTypeDef IS31FL3731_Init(IS31FL3731_HandleTypeDef *handle, I2C_HandleTypeDef *hi2c, uint8_t addr);

// 显示控制函数
HAL_StatusTypeDef IS31FL3731_SetRotation(IS31FL3731_HandleTypeDef *handle, uint8_t rotation);
HAL_StatusTypeDef IS31FL3731_Clear(IS31FL3731_HandleTypeDef *handle);
HAL_StatusTypeDef IS31FL3731_DisplayFrame(IS31FL3731_HandleTypeDef *handle, uint8_t frame);
HAL_StatusTypeDef IS31FL3731_SetFrame(IS31FL3731_HandleTypeDef *handle, uint8_t frame);
HAL_StatusTypeDef IS31FL3731_DrawPixel(IS31FL3731_HandleTypeDef *handle, uint8_t x, uint8_t y, uint8_t brightness);
HAL_StatusTypeDef IS31FL3731_SetLEDPWM(IS31FL3731_HandleTypeDef *handle, uint8_t lednum, uint8_t pwm, uint8_t bank);
HAL_StatusTypeDef IS31FL3731_AudioSync(IS31FL3731_HandleTypeDef *handle, uint8_t sync);

// 低级I2C通信函数
HAL_StatusTypeDef IS31FL3731_SelectBank(IS31FL3731_HandleTypeDef *handle, uint8_t bank);
HAL_StatusTypeDef IS31FL3731_WriteRegister8(IS31FL3731_HandleTypeDef *handle, uint8_t bank, uint8_t reg, uint8_t data);
HAL_StatusTypeDef IS31FL3731_ReadRegister8(IS31FL3731_HandleTypeDef *handle, uint8_t bank, uint8_t reg, uint8_t *data);

#ifdef __cplusplus
}
#endif

#endif /* __IS31FL3731_H */ 