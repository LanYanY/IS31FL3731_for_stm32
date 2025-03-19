/**
  ******************************************************************************
  * @file           : IS31FL3731.c
  * @brief          : ����STM32��IS31FL3731 LED����оƬ��
  * @details        : ��ֲ��Arduino LED_11x7_Matrix_IS31FL3731��
  ******************************************************************************
  */

#include "IS31FL3731.h"
#include <string.h>

/**
  * @brief  ��ʼ��IS31FL3731оƬ
  * @param  handle: IS31FL3731���ָ��
  * @param  hi2c: I2C���ָ��
  * @param  addr: �豸��ַ
  * @retval HAL״̬
  */
HAL_StatusTypeDef IS31FL3731_Init(IS31FL3731_HandleTypeDef *handle, I2C_HandleTypeDef *hi2c, uint8_t addr) {
  HAL_StatusTypeDef status;
  
  // ��ʼ�����
  handle->hi2c = hi2c;
  handle->addr = addr;
  handle->frame = 0;
  handle->rotate = 0;
  
  // �ر�ϵͳ
  status = IS31FL3731_WriteRegister8(handle, ISSI_BANK_FUNCTIONREG, ISSI_REG_SHUTDOWN, 0x00);
  if (status != HAL_OK) return status;
  
  // ����ʱ
  HAL_Delay(10);
  
  // ����ϵͳ
  status = IS31FL3731_WriteRegister8(handle, ISSI_BANK_FUNCTIONREG, ISSI_REG_SHUTDOWN, 0x01);
  if (status != HAL_OK) return status;
  
  // ����Ϊͼ��ģʽ
  status = IS31FL3731_WriteRegister8(handle, ISSI_BANK_FUNCTIONREG, ISSI_REG_CONFIG, ISSI_REG_CONFIG_PICTUREMODE);
  if (status != HAL_OK) return status;
  
  // ��ʾ��0֡
  status = IS31FL3731_DisplayFrame(handle, handle->frame);
  if (status != HAL_OK) return status;
  
  // �����ǰ֡
  status = IS31FL3731_Clear(handle);
  if (status != HAL_OK) return status;
  
  // ��������LED����PWM��Ϊ0��
  for (uint8_t f = 0; f < 8; f++) {
    for (uint8_t i = 0; i <= 0x11; i++) {
      status = IS31FL3731_WriteRegister8(handle, f, i, 0xff);
      if (status != HAL_OK) return status;
    }
  }
  
  // �ر���Ƶͬ��
  status = IS31FL3731_AudioSync(handle, 0);
  if (status != HAL_OK) return status;
  
  return HAL_OK;
}

/**
  * @brief  ������ת����
  * @param  handle: IS31FL3731���ָ��
  * @param  rotation: ��ת����(0-3)
  * @retval HAL״̬
  */
HAL_StatusTypeDef IS31FL3731_SetRotation(IS31FL3731_HandleTypeDef *handle, uint8_t rotation) {
  if (rotation > 3) rotation = 0;
  handle->rotate = rotation;
  return HAL_OK;
}

/**
  * @brief  �����ǰ֡������LED
  * @param  handle: IS31FL3731���ָ��
  * @retval HAL״̬
  */
HAL_StatusTypeDef IS31FL3731_Clear(IS31FL3731_HandleTypeDef *handle) {
  HAL_StatusTypeDef status;
  
  // ѡ��ǰ֡
  status = IS31FL3731_SelectBank(handle, handle->frame);
  if (status != HAL_OK) return status;
  
  // �����������LED��PWMֵΪ0
  for (uint8_t i = 0; i < 144; i++) {
    status = IS31FL3731_WriteRegister8(handle, handle->frame, 0x24 + i, 0);
    if (status != HAL_OK) return status;
  }
  
  return HAL_OK;
}

/**
  * @brief  ����LED��PWMֵ
  * @param  handle: IS31FL3731���ָ��
  * @param  lednum: LED���(0-143)
  * @param  pwm: PWMֵ(0-255)
  * @param  bank: ֡���(0-7)
  * @retval HAL״̬
  */
HAL_StatusTypeDef IS31FL3731_SetLEDPWM(IS31FL3731_HandleTypeDef *handle, uint8_t lednum, uint8_t pwm, uint8_t bank) {
  if (lednum >= 144) return HAL_ERROR;
  return IS31FL3731_WriteRegister8(handle, bank, 0x24 + lednum, pwm);
}

/**
  * @brief  ��������
  * @param  handle: IS31FL3731���ָ��
  * @param  x: X����
  * @param  y: Y����
  * @param  brightness: ����(0-255)
  * @retval HAL״̬
  */
HAL_StatusTypeDef IS31FL3731_DrawPixel(IS31FL3731_HandleTypeDef *hdev, uint8_t x, uint8_t y, uint8_t brightness)
{
    // ��������Ƿ���Ч
    if (x >= 16 || y >= 9) {
        return HAL_ERROR;
    }
    
    // ����LED����
    uint8_t ledIndex = y * 16 + x;
    
    // �������ȷ�Χ
    if (brightness > 255) brightness = 255;
    
    // ֱ�ӵ���SetLEDPWM��������PWMֵ
    return IS31FL3731_SetLEDPWM(hdev, ledIndex, brightness, hdev->frame);
}

/**
  * @brief  ���õ�ǰ֡
  * @param  handle: IS31FL3731���ָ��
  * @param  frame: ֡���(0-7)
  * @retval HAL״̬
  */
HAL_StatusTypeDef IS31FL3731_SetFrame(IS31FL3731_HandleTypeDef *handle, uint8_t frame) {
  if (frame > 7) frame = 0;
  handle->frame = frame;
  return HAL_OK;
}

/**
  * @brief  ��ʾָ��֡
  * @param  handle: IS31FL3731���ָ��
  * @param  frame: ֡���(0-7)
  * @retval HAL״̬
  */
HAL_StatusTypeDef IS31FL3731_DisplayFrame(IS31FL3731_HandleTypeDef *handle, uint8_t frame) {
  if (frame > 7) frame = 0;
  return IS31FL3731_WriteRegister8(handle, ISSI_BANK_FUNCTIONREG, ISSI_REG_PICTUREFRAME, frame);
}

/**
  * @brief  ������Ƶͬ��
  * @param  handle: IS31FL3731���ָ��
  * @param  sync: ͬ��״̬(0-1)
  * @retval HAL״̬
  */
HAL_StatusTypeDef IS31FL3731_AudioSync(IS31FL3731_HandleTypeDef *handle, uint8_t sync) {
  return IS31FL3731_WriteRegister8(handle, ISSI_BANK_FUNCTIONREG, ISSI_REG_AUDIOSYNC, sync ? 0x1 : 0x0);
}

/**
  * @brief  ѡ��洢��
  * @param  handle: IS31FL3731���ָ��
  * @param  bank: ����
  * @retval HAL״̬
  */
HAL_StatusTypeDef IS31FL3731_SelectBank(IS31FL3731_HandleTypeDef *handle, uint8_t bank) {
  uint8_t cmd[2] = {ISSI_COMMANDREGISTER, bank};
  return HAL_I2C_Master_Transmit(handle->hi2c, handle->addr, cmd, 2, HAL_MAX_DELAY);
}

/**
  * @brief  д�Ĵ���
  * @param  handle: IS31FL3731���ָ��
  * @param  bank: ����
  * @param  reg: �Ĵ�����ַ
  * @param  data: ����
  * @retval HAL״̬
  */
HAL_StatusTypeDef IS31FL3731_WriteRegister8(IS31FL3731_HandleTypeDef *handle, uint8_t bank, uint8_t reg, uint8_t data) {
  HAL_StatusTypeDef status;
  
  // ѡ���
  status = IS31FL3731_SelectBank(handle, bank);
  if (status != HAL_OK) return status;
  
  // д����
  uint8_t cmd[2] = {reg, data};
  return HAL_I2C_Master_Transmit(handle->hi2c, handle->addr, cmd, 2, HAL_MAX_DELAY);
}

/**
  * @brief  ���Ĵ���
  * @param  handle: IS31FL3731���ָ��
  * @param  bank: ����
  * @param  reg: �Ĵ�����ַ
  * @param  data: ����ָ��
  * @retval HAL״̬
  */
HAL_StatusTypeDef IS31FL3731_ReadRegister8(IS31FL3731_HandleTypeDef *handle, uint8_t bank, uint8_t reg, uint8_t *data) {
  HAL_StatusTypeDef status;
  
  // ѡ���
  status = IS31FL3731_SelectBank(handle, bank);
  if (status != HAL_OK) return status;
  
  // д��Ĵ�����ַ��Ȼ���ȡ����
  status = HAL_I2C_Master_Transmit(handle->hi2c, handle->addr, &reg, 1, HAL_MAX_DELAY);
  if (status != HAL_OK) return status;
  
  return HAL_I2C_Master_Receive(handle->hi2c, handle->addr, data, 1, HAL_MAX_DELAY);
} 