#ifndef _BSP_ADC_DRIVE_H
#define _BSP_ADC_DRIVE_H

#define MAX_COLLECT_DATA_NUM        1600
// 采集芯片初始化
_Bool ADS1246_Init(void);

// 采集测试
void TestADS1246(void);

// 采集开关
void StartADC_ADC(_Bool flag);

void OpenCloseLed_ADC(_Bool flag);

// 采集数据
U32 ReadADS1246_ADC(void);

#define ADS1246_CMD_WAKEUP          0x00
#define ADS1246_CMD_SLEEP           0x02
#define ADS1246_CMD_SYNC            0x04
#define ADS1246_CMD_RST             0x06
#define ADS1246_CMD_NOP             0xFF
#define ADS1246_CMD_RDATA           0x12
#define ADS1246_CMD_RDATAC          0x14
#define ADS1246_CMD_SDATAC          0x16
#define ADS1246_CMD_RREG            0x20
#define ADS1246_CMD_WREG            0x40
#define ADS1246_CMD_SYSOCAL         0x60
#define ADS1246_CMD_SYSGCAL         0x61
#define ADS1246_CMD_SELFOCAL        0x62
#define ADS1246_CMD_RESTRICTED      0xF1

// 使用引脚
#define ADS1246_CS_PIN              GPIO_Pin_4
#define ADS1246_CS_GPIO             GPIOB
#define ADS1246_RST_PIN             GPIO_Pin_11
#define ADS1246_RST_GPIO            GPIOG
#define ADS1246_BUSY_PIN            GPIO_Pin_3
#define ADS1246_BUSY_GPIO           GPIOB
#define ADS1246_START_PIN           GPIO_Pin_5   
#define ADS1246_START_GPIO          GPIOB

// ADS1246支持的增益列表
#define ADC_GAIN_1          0x00
#define ADC_GAIN_2          0x10
#define ADC_GAIN_4          0x20
#define ADC_GAIN_8          0x30
#define ADC_GAIN_16         0x40
#define ADC_GAIN_32         0x50
#define ADC_GAIN_64         0x60
#define ADC_GAIN_128        0x70

// ADS1246支持的转换速率列表
#define ADC_SPS_5           0x00
#define ADC_SPS_10          0x01
#define ADC_SPS_20          0x02
#define ADC_SPS_40          0x03
#define ADC_SPS_80          0x04
#define ADC_SPS_160         0x05
#define ADC_SPS_320         0x06
#define ADC_SPS_640         0x07
#define ADC_SPS_1000        0x08
#define ADC_SPS_2000        0x09

typedef enum
{
#define ADS1246_REG_BCS     BCS
    BCS = 0,
#define ADS1246_REG_VBIAS   VBIAS
    VBIAS,
#define ADS1246_REG_MUX1    MUX1
    MUX1,
#define ADS1246_REG_SYS0    SYS0
    SYS0,
#define ADS1246_REG_OFC0    OFC0
    OFC0,
#define ADS1246_REG_OFC1    OFC1
    OFC1,
#define ADS1246_REG_OFC2    OFC2
    OFC2,
#define ADS1246_REG_FSC0    FSC0
    FSC0,
#define ADS1246_REG_FSC1    FSC1
    FSC1,
#define ADS1246_REG_FSC2    FSC2
    FSC2,
#define ADS1246_REG_ID      ID
    ID,
    REG_INVALID
}ADS1246_REGS_em;




#endif


