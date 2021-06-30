#include "bsp.h"

void Process_Debug(U8 *p)
{
	if(strstr((const char*)p,"list"))
	{
		printf("********************************************************\r\n");
		printf("******               调试菜单                    ******\r\n");
		printf("********************************************************\r\n");
		printf("请选择输入命令（十六进制发送）\r\n");
		printf("命令：AA020B010000000000A5，动作：开机初始化\r\n");
		printf("命令：AA0210020000000000A5，动作：批量开始检测\r\n"); 
		printf("命令：AA0210040000000000A5，动作：吸样完成，移动到下一个样品（和上个动作联动，单独操作无反应）\r\n");
		printf("命令：AA0210060000000000A5，动作：试管架已清空，继续检测（和上个动作联动，单独操作无反应）\r\n");
		printf("命令：AA02003000XXXXXXXX(XX表示步数：例如10000步请输入10270000)A5，动作：样本架入轨电机(MT5)移动XX步数\r\n");
		printf("命令：AA02013000XXXXXXXX(XX表示步数：例如10000步请输入10270000)A5，动作：电机4移动XX步数\r\n");
		printf("命令：AA02023000XXXXXXXX(XX表示步数：例如10000步请输入10270000)A5，动作：电机1移动XX步数\r\n");
		//printf("命令：AA02033000XXXXXXXX(XX表示步数：例如10000步请输入10270000)A5，动作：电机3移动XX步数\r\n");
		printf("命令：AA0204300000000000(XX表示步数：例如10000步请输入10270000)A5，动作：电机6移动XX步数\r\n");
		printf("命令：AA02053000XXXXXXXX(XX表示步数：例如10000步请输入10270000)A5，动作：电机2移动XX步数\r\n");
		printf("命令：AA0200310000000000A5，动作：样本架入轨电机(MT5)复位\r\n");
		printf("命令：AA0201310000000000A5，动作：电机4复位\r\n");
		printf("命令：AA0202310000000000A5，动作：电机1复位\r\n");
		//printf("命令：AA0203310000000000A5，动作：电机3复位\r\n");
		printf("命令：AA0204310000000000A5，动作：电机6复位\r\n");
		printf("命令：AA0205310000000000A5，动作：电机2复位\r\n");
		printf("命令：AA0200DA01XXXXXXXX(XX表示步数：例如10000步请输入10270000)A5，动作：设置样本架入轨电机(MT5)进样步数\r\n");
		printf("命令：AA0201DA01XXXXXXXX(XX表示步数：例如10000步请输入10270000)A5，动作：设置样本架入轨辅助电机(MT4)钩回试管架步数\r\n");
		printf("命令：AA0202DA01XXXXXXXX(XX表示步数：例如10000步请输入10270000)A5，动作：设置样本架传送电机(MT1)到工作位置步数\r\n");		
		printf("命令：AA0202DA02XXXXXXXX(XX表示步数：例如10000步请输入10270000)A5，动作：设置样本架传送电机(MT1)到扫码位置步数\r\n");
		printf("命令：AA0202DA03XXXXXXXX(XX表示步数：例如10000步请输入10270000)A5，动作：设置样本架传送电机(MT1)单次传送步数\r\n");
		printf("命令：AA0202DA04XXXXXXXX(XX表示步数：例如10000步请输入10270000)A5，动作：设置样本架传送电机(MT1)运动到出轨位置步数\r\n");
		printf("命令：AA0204DA01XXXXXXXX(XX表示步数：例如10000步请输入10270000)A5，动作：设置穿刺限位电机(MT6)到75mm试管位置步数\r\n");
		printf("命令：AA0204DA02XXXXXXXX(XX表示步数：例如10000步请输入10270000)A5，动作：设置穿刺限位电机(MT6)到100mm试管位置步数\r\n");
		printf("命令：AA0205DA01XXXXXXXX(XX表示步数：例如10000步请输入10270000)A5，动作：设置样本架出轨驱动电机(MT2)出轨步数\r\n");
		printf("命令：AA0210DB0000000000A5，动作：保存设置的系统参数\r\n");
		printf("命令：AA0210DC0000000000A5，动作：读取系统参数\r\n");
		printf("命令：AA0210400000000000A5，动作：获取下位机版本号\r\n");
		printf("命令：AA0210460000000000A5，动作：扫码添加项目\r\n");
		printf("命令：AA0210600000000000A5，动作：开始老化\r\n");		
		printf("命令：AA0210610000000000A5，动作：停止老化\r\n");
		printf("********************************************************\r\n");	
	}
	else if(strstr((const char*)p,"openscan"))
	{   
        ContinueScan(NO_UART8,1);
        while(Scan_OK == 0)
        {
            MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Rack_To_Scan_Rack_Pos-700,10);
                MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Rack_To_Scan_Rack_Pos+700,10);
        }
    }
	else if(strstr((const char*)p,"closescan"))
	{   
        ContinueScan(NO_UART8,0);
    }   
	else if(strstr((const char*)p,"cc1"))
	{   
		//电机1移动到轨道工作点
		MotorMove_TD(MOTOR1,g_MachineParam.MotorMoveStep.M1_Rack_Work_Pos,10);	
		//推试管架进入轨道	
		MotorMove_TD(MOTOR5, g_MachineParam.MotorMoveStep.M5_Rack_In_Pos,10);
        MotorMove_TD(MOTOR5, 0,10);
        //运动到一号试管穿刺位
        MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Rack_To_Scan_Pos+\
                            g_MachineParam.MotorMoveStep.M1_Rack_Min_Pos*(0+7),10);	        
    } 
	else if(strstr((const char*)p,"cc2"))
	{   
		//电机1移动到轨道工作点
		MotorMove_TD(MOTOR1,g_MachineParam.MotorMoveStep.M1_Rack_Work_Pos,10);	
		//推试管架进入轨道	
		MotorMove_TD(MOTOR5, g_MachineParam.MotorMoveStep.M5_Rack_In_Pos,10);
        MotorMove_TD(MOTOR5, 0,10);
        //运动到一号试管穿刺位
        MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Rack_To_Scan_Pos+\
                            g_MachineParam.MotorMoveStep.M1_Rack_Min_Pos*(1+7),10);	        
    }  
	else if(strstr((const char*)p,"cc3"))
	{   
		//电机1移动到轨道工作点
		MotorMove_TD(MOTOR1,g_MachineParam.MotorMoveStep.M1_Rack_Work_Pos,10);	
		//推试管架进入轨道	
		MotorMove_TD(MOTOR5, g_MachineParam.MotorMoveStep.M5_Rack_In_Pos,10);
        MotorMove_TD(MOTOR5, 0,10);
        //运动到一号试管穿刺位
        MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Rack_To_Scan_Pos+\
                            g_MachineParam.MotorMoveStep.M1_Rack_Min_Pos*(2+7),10);	        
    }     
	else if(strstr((const char*)p,"cc4"))
	{   
		//电机1移动到轨道工作点
		MotorMove_TD(MOTOR1,g_MachineParam.MotorMoveStep.M1_Rack_Work_Pos,10);	
		//推试管架进入轨道	
		MotorMove_TD(MOTOR5, g_MachineParam.MotorMoveStep.M5_Rack_In_Pos,10);
        MotorMove_TD(MOTOR5, 0,10);
        //运动到一号试管穿刺位
        MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Rack_To_Scan_Pos+\
                            g_MachineParam.MotorMoveStep.M1_Rack_Min_Pos*(3+7),10);	        
    } 
	else if(strstr((const char*)p,"cc5"))
	{   
		//电机1移动到轨道工作点
		MotorMove_TD(MOTOR1,g_MachineParam.MotorMoveStep.M1_Rack_Work_Pos,10);	
		//推试管架进入轨道	
		MotorMove_TD(MOTOR5, g_MachineParam.MotorMoveStep.M5_Rack_In_Pos,10);
        MotorMove_TD(MOTOR5, 0,10);
        //运动到一号试管穿刺位
        MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Rack_To_Scan_Pos+\
                            g_MachineParam.MotorMoveStep.M1_Rack_Min_Pos*(4+7),10);	        
    }   
	else if(strstr((const char*)p,"cc6"))
	{   
		//电机1移动到轨道工作点
		MotorMove_TD(MOTOR1,g_MachineParam.MotorMoveStep.M1_Rack_Work_Pos,10);	
		//推试管架进入轨道	
		MotorMove_TD(MOTOR5, g_MachineParam.MotorMoveStep.M5_Rack_In_Pos,10);
        MotorMove_TD(MOTOR5, 0,10);
        //运动到一号试管穿刺位
        MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Rack_To_Scan_Pos+\
                            g_MachineParam.MotorMoveStep.M1_Rack_Min_Pos*(5+7),10);	        
    }      
	else if(strstr((const char*)p,"cc7"))
	{   
		//电机1移动到轨道工作点
		MotorMove_TD(MOTOR1,g_MachineParam.MotorMoveStep.M1_Rack_Work_Pos,10);	
		//推试管架进入轨道	
		MotorMove_TD(MOTOR5, g_MachineParam.MotorMoveStep.M5_Rack_In_Pos,10);
        MotorMove_TD(MOTOR5, 0,10);
        //运动到一号试管穿刺位
        MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Rack_To_Scan_Pos+\
                            g_MachineParam.MotorMoveStep.M1_Rack_Min_Pos*(6+7),10);	        
    }    
	else if(strstr((const char*)p,"cc8"))
	{   
		//电机1移动到轨道工作点
		MotorMove_TD(MOTOR1,g_MachineParam.MotorMoveStep.M1_Rack_Work_Pos,10);	
		//推试管架进入轨道	
		MotorMove_TD(MOTOR5, g_MachineParam.MotorMoveStep.M5_Rack_In_Pos,10);
        MotorMove_TD(MOTOR5, 0,10);
        //运动到一号试管穿刺位
        MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Rack_To_Scan_Pos+\
                            g_MachineParam.MotorMoveStep.M1_Rack_Min_Pos*(7+7),10);	        
    } 
	else if(strstr((const char*)p,"cc9"))
	{   
		//电机1移动到轨道工作点
		MotorMove_TD(MOTOR1,g_MachineParam.MotorMoveStep.M1_Rack_Work_Pos,10);	
		//推试管架进入轨道	
		MotorMove_TD(MOTOR5, g_MachineParam.MotorMoveStep.M5_Rack_In_Pos,10);
        MotorMove_TD(MOTOR5, 0,10);
        //运动到一号试管穿刺位
        MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Rack_To_Scan_Pos+\
                            g_MachineParam.MotorMoveStep.M1_Rack_Min_Pos*(8+7),10);	        
    }  
	else if(strstr((const char*)p,"cc10"))
	{   
		//电机1移动到轨道工作点
		MotorMove_TD(MOTOR1,g_MachineParam.MotorMoveStep.M1_Rack_Work_Pos,10);	
		//推试管架进入轨道	
		MotorMove_TD(MOTOR5, g_MachineParam.MotorMoveStep.M5_Rack_In_Pos,10);
        MotorMove_TD(MOTOR5, 0,10);
        //运动到一号试管穿刺位
        MotorMove_TD(MOTOR1, g_MachineParam.MotorMoveStep.M1_Rack_To_Scan_Pos+\
                            g_MachineParam.MotorMoveStep.M1_Rack_Min_Pos*(9+7),10);	        
    }     
	else if(strstr((const char*)p,"mt5g"))
	{ 
         MotorMove_TD(MOTOR6,g_MachineParam.MotorMoveStep.M6_High_Tube_Pos,10);   
    }
	else if(strstr((const char*)p,"mt5l"))
	{ 
         MotorMove_TD(MOTOR6,g_MachineParam.MotorMoveStep.M6_Low_Tube_Pos,10);   
    }  
	else if(strstr((const char*)p,"s1"))
	{
		SetMotorRunParam_MD(MOTOR3, 1,Speed_Steps_to_Par(70));
	}
	else if(strstr((const char*)p,"yaoyun"))
	{
		Tube_Shaking(1);
	}
}
