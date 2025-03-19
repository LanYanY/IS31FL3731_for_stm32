# STM32 IS31FL3731 LED驱动库

这是一个用于STM32微控制器的IS31FL3731 LED矩阵驱动芯片的C语言库。IS31FL3731是一个I2C控制的LED矩阵驱动芯片，能够控制最多144个独立LED，支持16×9矩阵布局。

## 功能特点

- 完整支持IS31FL3731的基本功能
- 支持8个显示帧
- 支持屏幕旋转
- 支持音频同步模式
- 简单易用的API接口
- 基于STM32 HAL库，易于移植

## 硬件要求

- STM32系列微控制器（已在STM32F4上测试）
- IS31FL3731 LED矩阵驱动芯片
- I2C通信接口

## 使用方法

### 初始化

```c
// 定义IS31FL3731句柄
IS31FL3731_HandleTypeDef is31fl3731;

// 初始化设备
IS31FL3731_Init(&is31fl3731, &hi2c1, ISSI_ADDR_DEFAULT);
```

### 绘制像素

```c
// 在指定位置绘制亮度为128的像素
IS31FL3731_DrawPixel(&is31fl3731, 5, 3, 128);
```

### 清除显示

```c
// 清除当前帧
IS31FL3731_Clear(&is31fl3731);
```

### 帧控制

```c
// 切换到帧2
IS31FL3731_SetFrame(&is31fl3731, 2);

// 在帧2上绘制
IS31FL3731_DrawPixel(&is31fl3731, 10, 5, 255);

// 显示帧2
IS31FL3731_DisplayFrame(&is31fl3731, 2);
```

## API参考

### 初始化函数

```c
HAL_StatusTypeDef IS31FL3731_Init(IS31FL3731_HandleTypeDef *handle, I2C_HandleTypeDef *hi2c, uint8_t addr);
```

### 显示控制函数

```c
HAL_StatusTypeDef IS31FL3731_SetRotation(IS31FL3731_HandleTypeDef *handle, uint8_t rotation);
HAL_StatusTypeDef IS31FL3731_Clear(IS31FL3731_HandleTypeDef *handle);
HAL_StatusTypeDef IS31FL3731_DisplayFrame(IS31FL3731_HandleTypeDef *handle, uint8_t frame);
HAL_StatusTypeDef IS31FL3731_SetFrame(IS31FL3731_HandleTypeDef *handle, uint8_t frame);
HAL_StatusTypeDef IS31FL3731_DrawPixel(IS31FL3731_HandleTypeDef *handle, uint8_t x, uint8_t y, uint8_t brightness);
HAL_StatusTypeDef IS31FL3731_SetLEDPWM(IS31FL3731_HandleTypeDef *handle, uint8_t lednum, uint8_t pwm, uint8_t bank);
HAL_StatusTypeDef IS31FL3731_AudioSync(IS31FL3731_HandleTypeDef *handle, uint8_t sync);
```

## 接线说明

IS31FL3731与STM32连接示例：

| IS31FL3731 | STM32 |
|------------|-------|
| VCC        | 3.3V/5V |
| GND        | GND   |
| SDA        | I2C1_SDA |
| SCL        | I2C1_SCL |
| ADDR       | GND (默认地址) |

## 移植说明

本库基于STM32 HAL库开发，如需移植到其他平台，需要修改以下部分：

1. 替换STM32 HAL相关头文件
2. 实现相应的I2C通信函数
3. 根据需要调整延时函数

## 示例代码

```c
// 初始化I2C和IS31FL3731
HAL_Init();
MX_I2C1_Init();
IS31FL3731_HandleTypeDef matrix;
IS31FL3731_Init(&matrix, &hi2c1, ISSI_ADDR_DEFAULT);

// 绘制一个简单的图案
for(uint8_t i = 0; i < 16; i++) {
    IS31FL3731_DrawPixel(&matrix, i, i/2, 255);
}

// 显示图案
IS31FL3731_DisplayFrame(&matrix, 0);
```

## 许可证

本库基于MIT许可证发布，详情请查看LICENSE文件。

## 致谢

- 本库移植自Arduino LED_11x7_Matrix_IS31FL3731库
- 感谢Adafruit提供的原始Arduino库代码 