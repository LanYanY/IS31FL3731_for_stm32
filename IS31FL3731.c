/**
  ******************************************************************************
  * @file           : IS31FL3731.c
  * @brief          : 用于STM32的IS31FL3731 LED驱动芯片库
  * @details        : 移植自Arduino LED_11x7_Matrix_IS31FL3731库
  ******************************************************************************
  */

#include "IS31FL3731.h"
#include <string.h>

/**
  * @brief  初始化IS31FL3731芯片
  * @param  handle: IS31FL3731句柄指针
  * @param  hi2c: I2C句柄指针
  * @param  addr: 设备地址
  * @retval HAL状态
  */
HAL_StatusTypeDef IS31FL3731_Init(IS31FL3731_HandleTypeDef *handle, I2C_HandleTypeDef *hi2c, uint8_t addr) {
  HAL_StatusTypeDef status;
  
  // 初始化句柄
  handle->hi2c = hi2c;
  handle->addr = addr;
  handle->frame = 0;
  handle->rotate = 0;
  
  // 关闭系统
  status = IS31FL3731_WriteRegister8(handle, ISSI_BANK_FUNCTIONREG, ISSI_REG_SHUTDOWN, 0x00);
  if (status != HAL_OK) return status;
  
  // 短延时
  HAL_Delay(10);
  
  // 启动系统
  status = IS31FL3731_WriteRegister8(handle, ISSI_BANK_FUNCTIONREG, ISSI_REG_SHUTDOWN, 0x01);
  if (status != HAL_OK) return status;
  
  // 设置为图像模式
  status = IS31FL3731_WriteRegister8(handle, ISSI_BANK_FUNCTIONREG, ISSI_REG_CONFIG, ISSI_REG_CONFIG_PICTUREMODE);
  if (status != HAL_OK) return status;
  
  // 显示第0帧
  status = IS31FL3731_DisplayFrame(handle, handle->frame);
  if (status != HAL_OK) return status;
  
  // 清除当前帧
  status = IS31FL3731_Clear(handle);
  if (status != HAL_OK) return status;
  
  // 激活所有LED（但PWM设为0）
  for (uint8_t f = 0; f < 8; f++) {
    for (uint8_t i = 0; i <= 0x11; i++) {
      status = IS31FL3731_WriteRegister8(handle, f, i, 0xff);
      if (status != HAL_OK) return status;
    }
  }
  
  // 关闭音频同步
  status = IS31FL3731_AudioSync(handle, 0);
  if (status != HAL_OK) return status;
  
  return HAL_OK;
}

/**
  * @brief  设置旋转方向
  * @param  handle: IS31FL3731句柄指针
  * @param  rotation: 旋转方向(0-3)
  * @retval HAL状态
  */
HAL_StatusTypeDef IS31FL3731_SetRotation(IS31FL3731_HandleTypeDef *handle, uint8_t rotation) {
  if (rotation > 3) rotation = 0;
  handle->rotate = rotation;
  return HAL_OK;
}

/**
  * @brief  清除当前帧的所有LED
  * @param  handle: IS31FL3731句柄指针
  * @retval HAL状态
  */
HAL_StatusTypeDef IS31FL3731_Clear(IS31FL3731_HandleTypeDef *handle) {
  HAL_StatusTypeDef status;
  
  // 选择当前帧
  status = IS31FL3731_SelectBank(handle, handle->frame);
  if (status != HAL_OK) return status;
  
  // 逐个设置所有LED的PWM值为0
  for (uint8_t i = 0; i < 144; i++) {
    status = IS31FL3731_WriteRegister8(handle, handle->frame, 0x24 + i, 0);
    if (status != HAL_OK) return status;
  }
  
  return HAL_OK;
}

/**
  * @brief  设置LED的PWM值
  * @param  handle: IS31FL3731句柄指针
  * @param  lednum: LED编号(0-143)
  * @param  pwm: PWM值(0-255)
  * @param  bank: 帧编号(0-7)
  * @retval HAL状态
  */
HAL_StatusTypeDef IS31FL3731_SetLEDPWM(IS31FL3731_HandleTypeDef *handle, uint8_t lednum, uint8_t pwm, uint8_t bank) {
  if (lednum >= 144) return HAL_ERROR;
  return IS31FL3731_WriteRegister8(handle, bank, 0x24 + lednum, pwm);
}

/**
  * @brief  绘制像素
  * @param  handle: IS31FL3731句柄指针
  * @param  x: X坐标
  * @param  y: Y坐标
  * @param  brightness: 亮度(0-255)
  * @retval HAL状态
  */
HAL_StatusTypeDef IS31FL3731_DrawPixel(IS31FL3731_HandleTypeDef *hdev, uint8_t x, uint8_t y, uint8_t brightness)
{
    // 检查坐标是否有效
    if (x >= 16 || y >= 9) {
        return HAL_ERROR;
    }
    
    // 计算LED索引
    uint8_t ledIndex = y * 16 + x;
    
    // 限制亮度范围
    if (brightness > 255) brightness = 255;
    
    // 直接调用SetLEDPWM函数设置PWM值
    return IS31FL3731_SetLEDPWM(hdev, ledIndex, brightness, hdev->frame);
}

/**
  * @brief  设置当前帧
  * @param  handle: IS31FL3731句柄指针
  * @param  frame: 帧编号(0-7)
  * @retval HAL状态
  */
HAL_StatusTypeDef IS31FL3731_SetFrame(IS31FL3731_HandleTypeDef *handle, uint8_t frame) {
  if (frame > 7) frame = 0;
  handle->frame = frame;
  return HAL_OK;
}

/**
  * @brief  显示指定帧
  * @param  handle: IS31FL3731句柄指针
  * @param  frame: 帧编号(0-7)
  * @retval HAL状态
  */
HAL_StatusTypeDef IS31FL3731_DisplayFrame(IS31FL3731_HandleTypeDef *handle, uint8_t frame) {
  if (frame > 7) frame = 0;
  return IS31FL3731_WriteRegister8(handle, ISSI_BANK_FUNCTIONREG, ISSI_REG_PICTUREFRAME, frame);
}

/**
  * @brief  设置音频同步
  * @param  handle: IS31FL3731句柄指针
  * @param  sync: 同步状态(0-1)
  * @retval HAL状态
  */
HAL_StatusTypeDef IS31FL3731_AudioSync(IS31FL3731_HandleTypeDef *handle, uint8_t sync) {
  return IS31FL3731_WriteRegister8(handle, ISSI_BANK_FUNCTIONREG, ISSI_REG_AUDIOSYNC, sync ? 0x1 : 0x0);
}

/**
  * @brief  选择存储库
  * @param  handle: IS31FL3731句柄指针
  * @param  bank: 库编号
  * @retval HAL状态
  */
HAL_StatusTypeDef IS31FL3731_SelectBank(IS31FL3731_HandleTypeDef *handle, uint8_t bank) {
  uint8_t cmd[2] = {ISSI_COMMANDREGISTER, bank};
  return HAL_I2C_Master_Transmit(handle->hi2c, handle->addr, cmd, 2, HAL_MAX_DELAY);
}

/**
  * @brief  写寄存器
  * @param  handle: IS31FL3731句柄指针
  * @param  bank: 库编号
  * @param  reg: 寄存器地址
  * @param  data: 数据
  * @retval HAL状态
  */
HAL_StatusTypeDef IS31FL3731_WriteRegister8(IS31FL3731_HandleTypeDef *handle, uint8_t bank, uint8_t reg, uint8_t data) {
  HAL_StatusTypeDef status;
  
  // 选择库
  status = IS31FL3731_SelectBank(handle, bank);
  if (status != HAL_OK) return status;
  
  // 写数据
  uint8_t cmd[2] = {reg, data};
  return HAL_I2C_Master_Transmit(handle->hi2c, handle->addr, cmd, 2, HAL_MAX_DELAY);
}

/**
  * @brief  读寄存器
  * @param  handle: IS31FL3731句柄指针
  * @param  bank: 库编号
  * @param  reg: 寄存器地址
  * @param  data: 数据指针
  * @retval HAL状态
  */
HAL_StatusTypeDef IS31FL3731_ReadRegister8(IS31FL3731_HandleTypeDef *handle, uint8_t bank, uint8_t reg, uint8_t *data) {
  HAL_StatusTypeDef status;
  
  // 选择库
  status = IS31FL3731_SelectBank(handle, bank);
  if (status != HAL_OK) return status;
  
  // 写入寄存器地址，然后读取数据
  status = HAL_I2C_Master_Transmit(handle->hi2c, handle->addr, &reg, 1, HAL_MAX_DELAY);
  if (status != HAL_OK) return status;
  
  return HAL_I2C_Master_Receive(handle->hi2c, handle->addr, data, 1, HAL_MAX_DELAY);
} 