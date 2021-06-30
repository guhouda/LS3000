#include "bsp.h"

void Process_Debug(U8 *p)
{
	if(strstr((const char*)p,"list"))
	{
		printf("********************************************************\r\n");
		printf("******               调试菜单                    ******\r\n");
		printf("********************************************************\r\n");
		printf("请选择输入命令（十六进制发送）\r\n");
		printf("命令：AA030B010000000000A5，动作：开机初始化\r\n");
		printf("命令：AA03101100XX(XX表示取几号卡)000000A5，动作：取XX号卡\r\n"); 
		printf("命令：AA0310120000000000A5，动作：查询是否准备好进卡\r\n");
		printf("命令：AA0310130000000000A5，动作：拨卡进反应盘\r\n");
		printf("命令：AA03003000XXXXXXXX(XX表示步数：例如10000步请输入10270000)A5，动作：拨卡横移驱动电机(MT1)运行到什么位置\r\n");
		printf("命令：AA03013000XXXXXXXX(XX表示步数：例如10000步请输入10270000)A5，动作：卡条出盒驱动电机(MT2)运行到什么位置\r\n");
		printf("命令：AA03023000XXXXXXXX(XX表示步数：例如10000步请输入10270000)A5，动作：卡条进入换向机构驱动电机(MT3)运行到什么位置\r\n");
		printf("命令：AA03033000XXXXXXXX(XX表示步数：例如10000步请输入10270000)A5，动作：卡条换向驱动电机(MT4)运行到什么位置\r\n");
		printf("命令：AA0304300000000000(XX表示步数：例如10000步请输入10270000)A5，动作：卡条送入检测盘驱动电机(MT5)运行到什么位置\r\n");
		printf("命令：AA0300310000000000A5，动作：卡盒横移电机(MT1)复位\r\n");
		printf("命令：AA0301310000000000A5，动作：卡条出盒驱动电机(MT2)复位\r\n");
		printf("命令：AA0302310000000000A5，动作：卡条进入换向机构驱动电机(MT3)复位\r\n");
		printf("命令：AA0303310000000000A5，动作：卡条换向驱动电机(MT4)复位\r\n");
		printf("命令：AA0304310000000000A5，动作：卡条送入检测盘驱动电机(MT5)复位\r\n");
		printf("命令：AA0300DA01XXXXXXXX(XX表示步数：例如10000步请输入10270000)A5，动作：设置拨卡横移驱动电机(MT1)到卡盒1位置步数\r\n");
		printf("命令：AA0300DA02XXXXXXXX(XX表示步数：例如10000步请输入10270000)A5，动作：设置拨卡横移驱动电机(MT1)到卡盒2位置步数\r\n");
		printf("命令：AA0300DA03XXXXXXXX(XX表示步数：例如10000步请输入10270000)A5，动作：设置拨卡横移驱动电机(MT1)到卡盒3位置步数\r\n");
		printf("命令：AA0300DA04XXXXXXXX(XX表示步数：例如10000步请输入10270000)A5，动作：设置拨卡横移驱动电机(MT1)到卡盒4位置步数\r\n");
		printf("命令：AA0300DA05XXXXXXXX(XX表示步数：例如10000步请输入10270000)A5，动作：设置拨卡横移驱动电机(MT1)到卡盒5位置步数\r\n");		
		printf("命令：AA0301DA01XXXXXXXX(XX表示步数：例如10000步请输入10270000)A5，动作：设置卡条出盒驱动电机(MT2)出盒步数\r\n");
		printf("命令：AA0302DA01XXXXXXXX(XX表示步数：例如10000步请输入10270000)A5，动作：设置卡条进入换向机构(MT3)步数\r\n");		
		printf("命令：AA0303DA01XXXXXXXX(XX表示步数：例如10000步请输入10270000)A5，动作：设置卡条换向驱动电机(MT4)转向步数\r\n");
		printf("命令：AA0304DA01XXXXXXXX(XX表示步数：例如10000步请输入10270000)A5，动作：设置卡条送入检测盘驱动电机(MT5)开始钩卡步数\r\n");
		printf("命令：AA0304DA02XXXXXXXX(XX表示步数：例如10000步请输入10270000)A5，动作：设置卡条送入检测盘驱动电机(MT5)钩卡结束步数\r\n");
		printf("命令：AA0310DB0000000000A5，动作：保存设置的系统参数\r\n");
		printf("命令：AA0310DC0000000000A5，动作：读取系统参数\r\n");
		printf("命令：AA0310400000000000A5，动作：获取下位机版本号\r\n");
		printf("命令：AA0310490000000000A5，动作：检查卡盒微动开关状态\r\n");
		printf("命令：AA0310600000000000A5，动作：开始老化\r\n");		
		printf("命令：AA0310610000000000A5，动作：停止老化\r\n");
		printf("********************************************************\r\n");	
	}
	else if(strstr((const char*)p,"openscan"))
	{   
        WaitScanData_BC(UART_CARD_SCAN); 
    } 
else if(strstr((const char*)p,"boka"))
	{   
    U8 SendData[4] = {0};    
		for(int i=0;i<4;i++)
		{
			PushCardToReversePos(0x01);
			PushCardToRoller();
			SendMsg_ToSlove(0x04,SYN_CMD,0xAB,0, SendData,sizeof(SendData));
			PushCardToReversePos(0x02);
			PushCardToRoller();
			SendMsg_ToSlove(0x04,SYN_CMD,0xAB,0, SendData,sizeof(SendData));
			PushCardToReversePos(0x03);
			PushCardToRoller();
			SendMsg_ToSlove(0x04,SYN_CMD,0xAB,0, SendData,sizeof(SendData));
			PushCardToReversePos(0x04);
			PushCardToRoller();
			SendMsg_ToSlove(0x04,SYN_CMD,0xAB,0, SendData,sizeof(SendData));
			PushCardToReversePos(0x05);
			PushCardToRoller();
			SendMsg_ToSlove(0x04,SYN_CMD,0xAB,0, SendData,sizeof(SendData));
		}
			PushCardToReversePos(0x01);
			PushCardToRoller();
			SendMsg_ToSlove(0x04,SYN_CMD,0xAB,0, SendData,sizeof(SendData));
			PushCardToReversePos(0x02);
			PushCardToRoller();
			SendMsg_ToSlove(0x04,SYN_CMD,0xAB,0, SendData,sizeof(SendData));
			PushCardToReversePos(0x03);
			PushCardToRoller();
			SendMsg_ToSlove(0x04,SYN_CMD,0xAB,0, SendData,sizeof(SendData));
			PushCardToReversePos(0x04);
			PushCardToRoller();
			SendMsg_ToSlove(0x04,SYN_CMD,0xAB,0, SendData,sizeof(SendData));
    }	
}
