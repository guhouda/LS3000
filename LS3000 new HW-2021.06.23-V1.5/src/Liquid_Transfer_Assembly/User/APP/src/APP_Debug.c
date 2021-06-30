#include "bsp.h"

void Process_Debug(U8 *p)
{
	if(strstr((const char*)p,"list"))
	{
		printf("********************************************************\r\n");
		printf("******               调试菜单                    ******\r\n");
		printf("********************************************************\r\n");
		printf("请选择输入命令（十六进制发送）\r\n");
		printf("命令：AA040B010000000000A5，动作：开机初始化\r\n");
		printf("命令：AA04003000XXXXXXXX(XX表示步数：例如10000步请输入10270000)A5，动作：机械臂水平电机(MT1)运行到什么位置\r\n");
		printf("命令：AA04013000XXXXXXXX(XX表示步数：例如10000步请输入10270000)A5，动作：机械臂垂直电机(MT2)运行到什么位置\r\n");
		printf("命令：AA04023000XXXXXXXX(XX表示步数：例如10000步请输入10270000)A5，动作：反应盘(MT3)运行到什么位置\r\n");
		printf("命令：AA04033000XXXXXXXX(XX表示步数：例如10000步请输入10270000)A5，动作：精密泵电机1(MT4)运行到什么位置\r\n");
		printf("命令：AA0404300000000000(XX表示步数：例如10000步请输入10270000)A5，动作：稀释液传送电机(MT5)运行到什么位置\r\n");
		printf("命令：AA0405300000000000(XX表示步数：例如10000步请输入10270000)A5，动作：采光头扫描&拨卡电机(MT6)运行到什么位置\r\n");
		printf("命令：AA0400310000000000A5，动作：机械臂水平电机(MT1)复位\r\n");
		printf("命令：AA0401310000000000A5，动作：机械臂垂直电机(MT2)复位\r\n");
		printf("命令：AA0402310000000000A5，动作：反应盘(MT3)复位\r\n");
		printf("命令：AA0403310000000000A5，动作：精密泵电机1(MT4)复位\r\n");
		printf("命令：AA0404310000000000A5，动作：稀释液传送电机(MT5)复位\r\n");
		printf("命令：AA0405310000000000A5，动作：采光头扫描&拨卡电机(MT5)复位\r\n");
		printf("命令：AA0402DA01XXXXXXXX(XX表示步数：例如10000步请输入10270000)A5，动作：设置转盘复位偏移步数(MT3)\r\n");		
		printf("命令：AA0405DA01XXXXXXXX(XX表示步数：例如10000步请输入10270000)A5，动作：设置光路检测电机(MT6)到开始检测位置步数\r\n");
		printf("命令：AA0405DA02XXXXXXXX(XX表示步数：例如10000步请输入10270000)A5，动作：设置光路检测电机(MT6)到检测完成位置步数\r\n");
		printf("命令：AA0405DA03XXXXXXXX(XX表示步数：例如10000步请输入10270000)A5，动作：设置光路检测电机(MT6)到退卡位置步数\r\n");			
		printf("命令：AA0420DA01XXXXXXXX(XX表示结果系数)A5，动作：设置结果系数\r\n");
		printf("命令：AA0420DA02XXXXXXXX(XX表示偏移步数)A5，动作：设置偏移步数\r\n");
		printf("命令：AA0420DA03XXXXXXXX(XX表示LED PWM)A5，动作：设置LED PWM\r\n");			
		printf("命令：AA0410DB0000000000A5，动作：保存设置的系统参数\r\n");
		printf("命令：AA0410DC0000000000A5，动作：读取系统参数\r\n");
		printf("命令：AA0410400000000000A5，动作：获取下位机版本号\r\n");
		printf("命令：AA041026XX(0x01一联卡;0x02二联卡;0x03 三联卡)00000000A5，动作：启动工程测试\r\n");
		printf("命令：AA04103700XXXXXXXX(XX表示清洗时间（单位：100ms）)A5，动作：加样针清洗槽内壁清洗\r\n");
		printf("命令：AA04103800XXXXXXXX(XX表示清洗时间（单位：100ms）)A5，动作：加样针外壁清洗\r\n");
		printf("命令：AA04103900XXXXXXXX(XX表示清洗时间（单位：100ms）)A5，动作：加样针内壁清洗\r\n");
		printf("命令：AA041015XX(Bit1表示泵1，Bit2表示泵2...，1表示打开，0表示关闭)00000000A5，动作：操作泵\r\n");
		printf("命令：AA041016XX(Bit1表示阀1，Bit2表示阀2...，1表示打开，0表示关闭)00000000A5，动作：操作阀\r\n");
		printf("命令：AA0410170100000000A5，动作：清洗液桶满状态检测开关\r\n");
		printf("命令：AA0410170200000000A5，动作：清洗液桶空状态检测开关\r\n");
		printf("命令：AA0410170300000000A5，动作：废液桶满状态检测开关\r\n");
		printf("命令：AA0410170400000000A5，动作：废液桶空状态检测开关\r\n");
		printf("命令：AA0410180000000000A5，动作：检测转盘进卡处是否有卡条\r\n");
		printf("命令：AA0410240000000000A5，动作：将转盘上卡条拨到光路检测处\r\n");
		printf("命令：AA0410600000000000A5，动作：开始老化\r\n");		
		printf("命令：AA0410610000000000A5，动作：停止老化\r\n");
        printf("命令：AA0410460000000000A5，动作：打开外扫码\r\n");
        printf("命令：AA04103501XXXXXXXX(XX表示吸样量（单位：ul）)A5，动作：液面探测吸急诊位样本\r\n");
        printf("命令：AA04103502XXXXXXXX(XX表示吸样量（单位：ul）)A5，动作：液面探测吸低试管样本\r\n");
        printf("命令：AA04103503XXXXXXXX(XX表示吸样量（单位：ul）)A5，动作：液面探测吸高试管样本\r\n");
        printf("命令：qx，动作：清洗\r\n");
        printf("命令：qk，动作：清卡\r\n");
		printf("********************************************************\r\n");	
	}
	else if(strstr((const char*)p,"qx"))
	{   
        //加样针内外壁清洗    
        CleaningNeedle_Out(30);
        CleaningNeedle_In(40);
    }  
	else if(strstr((const char*)p,"qk"))
	{
		for(U16 i = 0;i<24;i++)
        {   
           TMC5160_MotorMove(MOTOR3, g_MachineParam.MotorMoveStep.M3_Move_Min_Pos*i);
           MoveCardOut();  
        }		
    }    
	else if(strstr((const char*)p,"zhuanpanceshi"))
	{
		TMC5160_MotorMove(MOTOR3,0);
		TMC5160_MotorMove(MOTOR3, g_MachineParam.MotorMoveStep.M3_Move_Min_Pos*12);
	}
    else if(strstr((const char*)p,"yeweitance"))
	{
        Can_Send_Msg(POS, NULL, 0);	
    }
    else if(strstr((const char*)p,"yeweishibai"))
	{
		W25QXX_Read((U8*)&oneLiqParam.ConValue, 12*1024*1024, sizeof(U16));
		printf("%d\r\n",oneLiqParam.ConValue);
	}
    else if(strstr((const char*)p,"gaoguantance"))
    {
        int err_cnt = 0;
        for(int i = 0;i<20;i++)
        {
            if(!Suction_Sample_Liq(HIGH_TUBE_POS,0))
                err_cnt++;
            printf("失败次数%d\r\n",err_cnt);
        }        
    }
    else if(strstr((const char*)p,"diguantance"))
    {
        Suction_Sample_Liq(LOW_TUBE_POS,100);
    }
}
