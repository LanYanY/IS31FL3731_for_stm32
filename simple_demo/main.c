/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : IS31FL3731库使用示例
  ******************************************************************************
  */

/* 包含 ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "gpio.h"
#include "IS31FL3731.h"

/* 私有变量 --------------------------------------------------------------*/
IS31FL3731_HandleTypeDef matrix;

/* 私有函数原型 ----------------------------------------------------------*/
void SystemClock_Config(void);
void DrawPattern(void);
void DrawScrollingText(void);

/**
  * @brief  主函数
  * @retval int
  */
int main(void)
{
  /* MCU配置 */
  HAL_Init();
  
  /* 配置系统时钟 */
  SystemClock_Config();
  
  /* 初始化所有配置的外设 */
  MX_GPIO_Init();
  MX_I2C1_Init();
  
  /* 初始化IS31FL3731 */
  if (IS31FL3731_Init(&matrix, &hi2c1, ISSI_ADDR_DEFAULT) != HAL_OK) {
    Error_Handler();
  }
  
  /* 无限循环 */
  while (1)
  {
    /* 演示1: 绘制图案 */
    DrawPattern();
    HAL_Delay(2000);
    IS31FL3731_Clear(&matrix);
    
    /* 演示2: 模拟滚动文本 */
    DrawScrollingText();
    HAL_Delay(2000);
    IS31FL3731_Clear(&matrix);
  }
}

/**
  * @brief  绘制图案演示
  * @retval None
  */
void DrawPattern(void)
{
  /* 清除当前帧 */
  IS31FL3731_Clear(&matrix);
  
  /* 绘制边框 */
  for (uint8_t i = 0; i < IS31_WIDTH; i++) {
    IS31FL3731_DrawPixel(&matrix, i, 0, 128);           // 顶部边框
    IS31FL3731_DrawPixel(&matrix, i, IS31_HEIGHT-1, 128); // 底部边框
  }
  
  for (uint8_t i = 0; i < IS31_HEIGHT; i++) {
    IS31FL3731_DrawPixel(&matrix, 0, i, 128);           // 左侧边框
    IS31FL3731_DrawPixel(&matrix, IS31_WIDTH-1, i, 128); // 右侧边框
  }
  
  /* 绘制对角线 */
  for (uint8_t i = 0; i < IS31_WIDTH && i < IS31_HEIGHT; i++) {
    IS31FL3731_DrawPixel(&matrix, i, i, 255);
    IS31FL3731_DrawPixel(&matrix, IS31_WIDTH-i-1, i, 255);
  }
}

/**
  * @brief  绘制滚动文本演示（简化模拟）
  * @retval None
  */
void DrawScrollingText(void)
{
  /* 清除当前帧 */
  IS31FL3731_Clear(&matrix);
  
  /* 简单模拟文字滚动效果（这里只是绘制一些点阵） */
  for (uint8_t frame = 0; frame < 8; frame++) {
    /* 切换到新帧 */
    IS31FL3731_SetFrame(&matrix, frame);
    IS31FL3731_Clear(&matrix);
    
    /* 绘制一个简单图案（不同帧图案有偏移） */
    for (uint8_t x = 0; x < 8; x++) {
      uint8_t pos_x = (x + frame) % IS31_WIDTH;
      IS31FL3731_DrawPixel(&matrix, pos_x, 2, 255);
      IS31FL3731_DrawPixel(&matrix, pos_x, 4, 255);
      IS31FL3731_DrawPixel(&matrix, pos_x, 6, 255);
    }
    
    /* 显示当前帧 */
    IS31FL3731_DisplayFrame(&matrix, frame);
    HAL_Delay(200);
  }
}

/**
  * @brief  系统时钟配置
  * @retval None
  */
void SystemClock_Config(void)
{
  /* 此处应包含系统时钟配置代码 */
  /* 在实际项目中应由STM32CubeMX生成 */
}

/**
  * @brief  错误处理函数
  * @retval None
  */
void Error_Handler(void)
{
  /* 用户可以添加自己的实现来报告应用程序中的错误
     例如：闪烁LED或在调试串口上发送消息 */
  __disable_irq();
  while (1)
  {
  }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  断言失败处理函数
  * @param  file: 源文件名
  * @param  line: 源代码行号
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* 用户可以添加自己的实现来报告文件名和行号,
     例如：printf("错误: 文件 %s 的第 %d 行\r\n", file, line) */
}
#endif /* USE_FULL_ASSERT */ 