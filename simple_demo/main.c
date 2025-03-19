/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : IS31FL3731��ʹ��ʾ��
  ******************************************************************************
  */

/* ���� ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "gpio.h"
#include "IS31FL3731.h"

/* ˽�б��� --------------------------------------------------------------*/
IS31FL3731_HandleTypeDef matrix;

/* ˽�к���ԭ�� ----------------------------------------------------------*/
void SystemClock_Config(void);
void DrawPattern(void);
void DrawScrollingText(void);

/**
  * @brief  ������
  * @retval int
  */
int main(void)
{
  /* MCU���� */
  HAL_Init();
  
  /* ����ϵͳʱ�� */
  SystemClock_Config();
  
  /* ��ʼ���������õ����� */
  MX_GPIO_Init();
  MX_I2C1_Init();
  
  /* ��ʼ��IS31FL3731 */
  if (IS31FL3731_Init(&matrix, &hi2c1, ISSI_ADDR_DEFAULT) != HAL_OK) {
    Error_Handler();
  }
  
  /* ����ѭ�� */
  while (1)
  {
    /* ��ʾ1: ����ͼ�� */
    DrawPattern();
    HAL_Delay(2000);
    IS31FL3731_Clear(&matrix);
    
    /* ��ʾ2: ģ������ı� */
    DrawScrollingText();
    HAL_Delay(2000);
    IS31FL3731_Clear(&matrix);
  }
}

/**
  * @brief  ����ͼ����ʾ
  * @retval None
  */
void DrawPattern(void)
{
  /* �����ǰ֡ */
  IS31FL3731_Clear(&matrix);
  
  /* ���Ʊ߿� */
  for (uint8_t i = 0; i < IS31_WIDTH; i++) {
    IS31FL3731_DrawPixel(&matrix, i, 0, 128);           // �����߿�
    IS31FL3731_DrawPixel(&matrix, i, IS31_HEIGHT-1, 128); // �ײ��߿�
  }
  
  for (uint8_t i = 0; i < IS31_HEIGHT; i++) {
    IS31FL3731_DrawPixel(&matrix, 0, i, 128);           // ���߿�
    IS31FL3731_DrawPixel(&matrix, IS31_WIDTH-1, i, 128); // �Ҳ�߿�
  }
  
  /* ���ƶԽ��� */
  for (uint8_t i = 0; i < IS31_WIDTH && i < IS31_HEIGHT; i++) {
    IS31FL3731_DrawPixel(&matrix, i, i, 255);
    IS31FL3731_DrawPixel(&matrix, IS31_WIDTH-i-1, i, 255);
  }
}

/**
  * @brief  ���ƹ����ı���ʾ����ģ�⣩
  * @retval None
  */
void DrawScrollingText(void)
{
  /* �����ǰ֡ */
  IS31FL3731_Clear(&matrix);
  
  /* ��ģ�����ֹ���Ч��������ֻ�ǻ���һЩ���� */
  for (uint8_t frame = 0; frame < 8; frame++) {
    /* �л�����֡ */
    IS31FL3731_SetFrame(&matrix, frame);
    IS31FL3731_Clear(&matrix);
    
    /* ����һ����ͼ������ͬ֡ͼ����ƫ�ƣ� */
    for (uint8_t x = 0; x < 8; x++) {
      uint8_t pos_x = (x + frame) % IS31_WIDTH;
      IS31FL3731_DrawPixel(&matrix, pos_x, 2, 255);
      IS31FL3731_DrawPixel(&matrix, pos_x, 4, 255);
      IS31FL3731_DrawPixel(&matrix, pos_x, 6, 255);
    }
    
    /* ��ʾ��ǰ֡ */
    IS31FL3731_DisplayFrame(&matrix, frame);
    HAL_Delay(200);
  }
}

/**
  * @brief  ϵͳʱ������
  * @retval None
  */
void SystemClock_Config(void)
{
  /* �˴�Ӧ����ϵͳʱ�����ô��� */
  /* ��ʵ����Ŀ��Ӧ��STM32CubeMX���� */
}

/**
  * @brief  ��������
  * @retval None
  */
void Error_Handler(void)
{
  /* �û���������Լ���ʵ��������Ӧ�ó����еĴ���
     ���磺��˸LED���ڵ��Դ����Ϸ�����Ϣ */
  __disable_irq();
  while (1)
  {
  }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  ����ʧ�ܴ�����
  * @param  file: Դ�ļ���
  * @param  line: Դ�����к�
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* �û���������Լ���ʵ���������ļ������к�,
     ���磺printf("����: �ļ� %s �ĵ� %d ��\r\n", file, line) */
}
#endif /* USE_FULL_ASSERT */ 