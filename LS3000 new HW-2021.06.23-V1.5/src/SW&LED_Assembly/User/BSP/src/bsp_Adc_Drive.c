#include "bsp.h"

// 宏定义
#define LED_ONOFF(x)        x?  GPIO_SetBits(GPIOH, GPIO_Pin_15):GPIO_ResetBits(GPIOH, GPIO_Pin_15);
#define ADS1246_BUSY            (GPIO_ReadInputDataBit(ADS1246_BUSY_GPIO, ADS1246_BUSY_PIN) == 1)
#define ADS1246_CS(x)       x?  GPIO_ResetBits(ADS1246_CS_GPIO, ADS1246_CS_PIN):GPIO_SetBits(ADS1246_CS_GPIO, ADS1246_CS_PIN);
#define ADS1246_RST(x)      x?  GPIO_ResetBits(ADS1246_RST_GPIO, ADS1246_RST_PIN):GPIO_SetBits(ADS1246_RST_GPIO, ADS1246_RST_PIN);
#define ADS1246_START(x)    x?  GPIO_SetBits(ADS1246_START_GPIO, ADS1246_START_PIN):GPIO_ResetBits(ADS1246_START_GPIO, ADS1246_START_PIN);
#define LED_PWM(x)          x?  GPIO_SetBits(GPIOI, GPIO_Pin_0):GPIO_ResetBits(GPIOI, GPIO_Pin_0);

static void delay_nms(U32 cnt)
{
    for(U32 i = 0; i < cnt; i++)
        for(U32 k = 0; k < 1024; k++);
}

// 初始化ADC端口 AD1246
void InitADC_ADC(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    SPI_InitTypeDef   SPI_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOG | RCC_AHB1Periph_GPIOI, ENABLE);
    
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_PinAFConfig(GPIOG, GPIO_PinSource12, GPIO_AF_SPI6); // MISO
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_Init(GPIOG, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOG, GPIO_PinSource14, GPIO_AF_SPI6); // MOSI
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_Init(GPIOG, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOG, GPIO_PinSource13, GPIO_AF_SPI6); // CLK
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_Init(GPIOG, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;             // NSS
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Pin  = ADS1246_CS_PIN;
    GPIO_Init(ADS1246_CS_GPIO, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;             // AD_START
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Pin  = ADS1246_START_PIN;
    GPIO_Init(ADS1246_START_GPIO, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;             // AD_RST
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Pin  = ADS1246_RST_PIN;
    GPIO_Init(ADS1246_RST_GPIO, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;              // AD_BUSY
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Pin  = ADS1246_BUSY_PIN;
    GPIO_Init(ADS1246_BUSY_GPIO, &GPIO_InitStructure);
    
    // Init SPI6
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI6, ENABLE);		
    SPI_Cmd(SPI6, DISABLE);
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; 	
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master; 			
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; 	
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High; 		
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; 	
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; 		
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32; 	
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; 	
    SPI_InitStructure.SPI_CRCPolynomial = 7;	
    SPI_Init(SPI6, &SPI_InitStructure); 	
    SPI_Cmd(SPI6, ENABLE);
}

// SPI6原始读写
U8 SPI6ReadAndWrite(U8 dat)
{
    U8 ret;
    for(U32 i = 0; i < 0x80; i++); 
    while ((SPI6->SR & (1 << 1)) == 0); // 等待发送缓存区空	
    SPI6->DR = dat; 
    while ((SPI6->SR & (1 << 0)) == 0); 
    ret = SPI6->DR; 
    return ret;
}

// 读ADS1246寄存器
static U8 ReadADS1246Reg(U8 reg)
{
    U8 ret = 0;
    ADS1246_CS(1);    // 片选选中
    delay_nms(3);
    SPI6ReadAndWrite(ADS1246_CMD_RREG | reg);
    SPI6ReadAndWrite(0);
    ret = SPI6ReadAndWrite(ADS1246_CMD_NOP);
    delay_nms(1);
    ADS1246_CS(0);   // 取消片选
    return ret;
}

// 写ADS1246寄存器
static void WriteADS1246Reg(U8 reg, U8 data)
{
    ADS1246_CS(1);
    delay_nms(3);
    SPI6ReadAndWrite(ADS1246_CMD_WREG | reg);
    SPI6ReadAndWrite(0);
    SPI6ReadAndWrite(data);
    delay_nms(1);
    SPI6ReadAndWrite(ADS1246_CMD_NOP);
    delay_nms(1);
    ADS1246_CS(0);
}

// 等待繁忙结束
U8 ADS1246_WaitBusy(void)
{   
    U16 i = 0;
    ADS1246_CS(0); // 片选不选中
    while(ADS1246_BUSY)
    {  
        delay_nms(10);
        i++;
        if (i > 200)
        {
            return 1;
        }
    }
    ADS1246_CS(1);
    return 0;
}

// LED_PWM配置 PI0 TIM5->CH4
static void ConfigPWM(U8 pwm)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_OCInitTypeDef TIM_OCInit;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseinit;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI | RCC_AHB1Periph_GPIOH, ENABLE); 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;             // LED_PWM
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
    GPIO_Init(GPIOI, &GPIO_InitStructure);
    
    GPIO_PinAFConfig(GPIOI, GPIO_PinSource0, GPIO_AF_TIM5); 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_Init(GPIOI, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;             // LED_OE
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;
    GPIO_Init(GPIOH, &GPIO_InitStructure);
    
    TIM_TimeBaseStructInit(&TIM_TimeBaseinit);
    TIM_TimeBaseinit.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseinit.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseinit.TIM_Period = 400;
    TIM_TimeBaseinit.TIM_Prescaler = 590; //440-400HZ
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseinit);
    TIM_ARRPreloadConfig(TIM5, ENABLE);
    TIM_ClearFlag(TIM5, TIM_FLAG_Update);

    TIM_OCInit.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInit.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInit.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
    TIM_OCInit.TIM_OCNPolarity = TIM_OCNPolarity_High;
    TIM_OCInit.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInit.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInit.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInit.TIM_Pulse = pwm;

    TIM_OC4Init(TIM5, &TIM_OCInit);
    TIM_OC4PreloadConfig(TIM5, TIM_OCPreload_Enable);
    TIM_OC4PolarityConfig(TIM5, TIM_OCPolarity_High);
    TIM_SelectOCxM(TIM5, TIM_Channel_4, TIM_OCMode_PWM1);
    TIM_CCxCmd(TIM5, TIM_Channel_4, TIM_CCx_Enable);
    TIM_CtrlPWMOutputs(TIM5, ENABLE);

    TIM_Cmd(TIM5, ENABLE);
}

// ADD_INPUT_PWM配置 PI2 TIM8->4
static void ADD_INPUT_ConfigPWM(U8 pwm)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_OCInitTypeDef TIM_OCInit;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseinit;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, ENABLE); 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
    
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;             // ADD_INPUT_PWM
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;
    GPIO_Init(GPIOI, &GPIO_InitStructure);
    
    GPIO_PinAFConfig(GPIOI, GPIO_PinSource2, GPIO_AF_TIM8); 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(GPIOI, &GPIO_InitStructure);
	
    TIM_TimeBaseStructInit(&TIM_TimeBaseinit);
    TIM_TimeBaseinit.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseinit.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseinit.TIM_Period = 400;
    TIM_TimeBaseinit.TIM_Prescaler = 590; //440-400HZ
    TIM_TimeBaseInit(TIM8, &TIM_TimeBaseinit);
    TIM_ARRPreloadConfig(TIM8, ENABLE);
    TIM_ClearFlag(TIM8, TIM_FLAG_Update);

    TIM_OCInit.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInit.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInit.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
    TIM_OCInit.TIM_OCNPolarity = TIM_OCNPolarity_High;
    TIM_OCInit.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInit.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInit.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInit.TIM_Pulse = pwm;

    TIM_OC4Init(TIM8, &TIM_OCInit);
    TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);
    TIM_OC4PolarityConfig(TIM8, TIM_OCPolarity_Low);
    TIM_SelectOCxM(TIM8, TIM_Channel_4, TIM_OCMode_PWM1);
    TIM_CCxCmd(TIM8, TIM_Channel_4, TIM_CCx_Enable);
    TIM_CtrlPWMOutputs(TIM8, ENABLE); //主输出使能

    TIM_Cmd(TIM8, ENABLE);
}    

// 采集芯片初始化
BOOL ADS1246_Init(void)
{
    U8 id;
    
    // 初始化IO
    InitADC_ADC();
    
    // LED_PWM配置
    ConfigPWM(100);
    // ADD_INPUT_PWM配置
    ADD_INPUT_ConfigPWM(100);
    
    ADS1246_RST(1); 
    delay_nms(200);
    ADS1246_RST(0);
    ADS1246_CS(0);
    SPI6ReadAndWrite(ADS1246_CMD_NOP); 
    ADS1246_START(1);  
    delay_nms(600);
    SPI6ReadAndWrite(ADS1246_CMD_RST);  
    delay_nms(60);
    // 读取芯片ID
	delay_nms(500);
	WriteADS1246Reg(ADS1246_REG_ID,0x08);
	delay_nms(200);
    id = ReadADS1246Reg(ADS1246_REG_ID);
    id = id&(0x7<<4);
	printf("ADS1246_ID = %04x\r\n",id);
    if (id != 0x10) // 读取不正确
    {
        U16 i =0;
        while(1)
        {
            if (i >100)
                return FALSE;  
            id = ReadADS1246Reg(ADS1246_REG_BCS);
            delay_nms(10);
            i++;
        }
        ADS1246_START(0);
		// 打印初始化错误信息
        return FALSE; 
    }
    WriteADS1246Reg(ADS1246_REG_SYS0, ADC_GAIN_8 | ADC_SPS_160);            
    id = ReadADS1246Reg(ADS1246_REG_SYS0);
    if (id != (ADC_GAIN_8 | ADC_SPS_160))
    {
        ADS1246_START(0); 
        return FALSE;
    }
    else // 校准ADS1246
    {
        WriteADS1246Reg(ADS1246_REG_MUX1, 0x00); // 恢复正常模式
        SPI6ReadAndWrite(ADS1246_CMD_SELFOCAL);  // 系统偏移自校准
        ADS1246_WaitBusy();                      // 等待校准完成

        WriteADS1246Reg(ADS1246_REG_MUX1, 0x01); // 设置AINP + AINN = (AVDD + AVSS)/2
        SPI6ReadAndWrite(ADS1246_CMD_SYSOCAL);  
        ADS1246_WaitBusy();

        WriteADS1246Reg(ADS1246_REG_MUX1, 0x02); // 设置AINP=VREF+,AINN=VREF-; for gain calibration
        SPI6ReadAndWrite(ADS1246_CMD_SYSGCAL);   // 系统增益校准
        ADS1246_WaitBusy();
		
		WriteADS1246Reg(ADS1246_REG_MUX1, 0x00); 
    }
    
    ADS1246_START(0); 
    return TRUE;
}

// 采集数据
U32 ReadADS1246_ADC(void)
{
    U32 ret = 0;
    while(ADS1246_BUSY);
    ADS1246_CS(1);
    SPI6ReadAndWrite(ADS1246_CMD_RDATA);
    ret |= SPI6ReadAndWrite(ADS1246_CMD_NOP) << 16;
    ret |= SPI6ReadAndWrite(ADS1246_CMD_NOP) << 8;
    ret |= SPI6ReadAndWrite(ADS1246_CMD_NOP) << 0;
    ADS1246_CS(0);
	if (ret > 0x7FFFFF)				// 防止反向溢出
		ret = 0xFFFFFF - ret;
    return ret;
}

// 采集测试
void TestADS1246(void)
{
    U32 SUM;
	static U32 readData[400];
    U8 tempbuf[20];
    OpenCloseLed_ADC(TRUE);
    delay_nms(50);
    StartADC_ADC(TRUE); 
    delay_nms(1);
    for(U32 i = 0; i < 400; i++)
    {
        readData[i] = ReadADS1246_ADC();
    }
    StartADC_ADC(FALSE);
    delay_nms(50);
    OpenCloseLed_ADC(FALSE);
    for(U32 i = 0; i < 400; i++)
    {
        memset(tempbuf,0,sizeof(tempbuf));
        sprintf((char*)tempbuf,"%d\r\n",readData[i]);
        SendCmdData_UD(NO_UART1, 0, tempbuf, strlen((CHAR*)tempbuf));
        SUM+=readData[i];
        delay_nms(6);
    }
    SUM = SUM>>24;
    SUM *= 2500;
}

void OpenCloseLed_ADC(BOOL flag)
{
    if (flag == TRUE)
    {
        LED_ONOFF(1);
    }
    else
    {
        LED_ONOFF(0);
    }
}

// 采集开关
void StartADC_ADC(BOOL flag)
{
    if (flag == TRUE)
    {
        ADS1246_START(1);
    }
    else
    {
        ADS1246_START(0);
    }
}


