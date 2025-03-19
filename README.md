# STM32 IS31FL3731 LED������

����һ������STM32΢��������IS31FL3731 LED��������оƬ��C���Կ⡣IS31FL3731��һ��I2C���Ƶ�LED��������оƬ���ܹ��������144������LED��֧��16��9���󲼾֡�

## �����ص�

- ����֧��IS31FL3731�Ļ�������
- ֧��8����ʾ֡
- ֧����Ļ��ת
- ֧����Ƶͬ��ģʽ
- �����õ�API�ӿ�
- ����STM32 HAL�⣬������ֲ

## Ӳ��Ҫ��

- STM32ϵ��΢������������STM32F4�ϲ��ԣ�
- IS31FL3731 LED��������оƬ
- I2Cͨ�Žӿ�

## ʹ�÷���

### ��ʼ��

```c
// ����IS31FL3731���
IS31FL3731_HandleTypeDef is31fl3731;

// ��ʼ���豸
IS31FL3731_Init(&is31fl3731, &hi2c1, ISSI_ADDR_DEFAULT);
```

### ��������

```c
// ��ָ��λ�û�������Ϊ128������
IS31FL3731_DrawPixel(&is31fl3731, 5, 3, 128);
```

### �����ʾ

```c
// �����ǰ֡
IS31FL3731_Clear(&is31fl3731);
```

### ֡����

```c
// �л���֡2
IS31FL3731_SetFrame(&is31fl3731, 2);

// ��֡2�ϻ���
IS31FL3731_DrawPixel(&is31fl3731, 10, 5, 255);

// ��ʾ֡2
IS31FL3731_DisplayFrame(&is31fl3731, 2);
```

## API�ο�

### ��ʼ������

```c
HAL_StatusTypeDef IS31FL3731_Init(IS31FL3731_HandleTypeDef *handle, I2C_HandleTypeDef *hi2c, uint8_t addr);
```

### ��ʾ���ƺ���

```c
HAL_StatusTypeDef IS31FL3731_SetRotation(IS31FL3731_HandleTypeDef *handle, uint8_t rotation);
HAL_StatusTypeDef IS31FL3731_Clear(IS31FL3731_HandleTypeDef *handle);
HAL_StatusTypeDef IS31FL3731_DisplayFrame(IS31FL3731_HandleTypeDef *handle, uint8_t frame);
HAL_StatusTypeDef IS31FL3731_SetFrame(IS31FL3731_HandleTypeDef *handle, uint8_t frame);
HAL_StatusTypeDef IS31FL3731_DrawPixel(IS31FL3731_HandleTypeDef *handle, uint8_t x, uint8_t y, uint8_t brightness);
HAL_StatusTypeDef IS31FL3731_SetLEDPWM(IS31FL3731_HandleTypeDef *handle, uint8_t lednum, uint8_t pwm, uint8_t bank);
HAL_StatusTypeDef IS31FL3731_AudioSync(IS31FL3731_HandleTypeDef *handle, uint8_t sync);
```

## ����˵��

IS31FL3731��STM32����ʾ����

| IS31FL3731 | STM32 |
|------------|-------|
| VCC        | 3.3V/5V |
| GND        | GND   |
| SDA        | I2C1_SDA |
| SCL        | I2C1_SCL |
| ADDR       | GND (Ĭ�ϵ�ַ) |

## ��ֲ˵��

�������STM32 HAL�⿪����������ֲ������ƽ̨����Ҫ�޸����²��֣�

1. �滻STM32 HAL���ͷ�ļ�
2. ʵ����Ӧ��I2Cͨ�ź���
3. ������Ҫ������ʱ����

## ʾ������

```c
// ��ʼ��I2C��IS31FL3731
HAL_Init();
MX_I2C1_Init();
IS31FL3731_HandleTypeDef matrix;
IS31FL3731_Init(&matrix, &hi2c1, ISSI_ADDR_DEFAULT);

// ����һ���򵥵�ͼ��
for(uint8_t i = 0; i < 16; i++) {
    IS31FL3731_DrawPixel(&matrix, i, i/2, 255);
}

// ��ʾͼ��
IS31FL3731_DisplayFrame(&matrix, 0);
```

## ���֤

�������MIT���֤������������鿴LICENSE�ļ���

## ��л

- ������ֲ��Arduino LED_11x7_Matrix_IS31FL3731��
- ��лAdafruit�ṩ��ԭʼArduino����� 