#include "can.h"

void CAN1_init()
{
	CAN_InitTypeDef           can;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,  ENABLE);
		
	NVIC_config(CAN1_RX0_IRQn, CAN1_RX0_IRQn_pre, CAN1_RX0_IRQn_sub);
	
	CAN_DeInit(CAN1);
	CAN_StructInit(&can);
	
	can.CAN_TTCM = DISABLE;		//
	can.CAN_ABOM = DISABLE;		//
	can.CAN_AWUM = DISABLE;		//
	can.CAN_NART = ENABLE;		//gai
	can.CAN_RFLM = DISABLE;		//
	can.CAN_TXFP = ENABLE;		//
	can.CAN_Mode = CAN_Mode_Normal;

	can.CAN_SJW       = CAN_SJW_1tq;
	can.CAN_BS1       = CAN_BS1_9tq;
	can.CAN_BS2       = CAN_BS2_4tq;
	can.CAN_Prescaler = 3;   //CAN BaudRate 42/(1+9+4)/3=1Mbps
	CAN_Init(CAN1, &can);

	CAN_FilterInitTypeDef     can_filter;

	can_filter.CAN_FilterNumber         = 0;
	can_filter.CAN_FilterMode           = CAN_FilterMode_IdMask;
	can_filter.CAN_FilterScale          = CAN_FilterScale_32bit;
	can_filter.CAN_FilterIdHigh         = 0x0000;
	can_filter.CAN_FilterIdLow          = 0x0000;
	can_filter.CAN_FilterMaskIdHigh     = 0x0000;
	can_filter.CAN_FilterMaskIdLow      = 0x0000;
	can_filter.CAN_FilterFIFOAssignment = 0;//the message which pass the filter save in fifo0
	can_filter.CAN_FilterActivation     = ENABLE;
	CAN_FilterInit(&can_filter);
	
	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);
}

void CAN2_init(void)
{
    CAN_InitTypeDef        can;
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);  //CAN2若要单独使用必须先使能CAN1时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);

	  NVIC_config(CAN2_RX0_IRQn, CAN2_RX0_IRQn_pre, CAN2_RX0_IRQn_sub);

    CAN_DeInit(CAN2);
    CAN_StructInit(&can);

    can.CAN_TTCM = DISABLE;
    can.CAN_ABOM = ENABLE;//软件离线自动管理    
    can.CAN_AWUM = DISABLE;    
    can.CAN_NART = ENABLE;//禁止报文自动传送 
    can.CAN_RFLM = DISABLE;    
    can.CAN_TXFP = ENABLE;     
    can.CAN_Mode = CAN_Mode_Normal; 
		
    can.CAN_SJW       = CAN_SJW_1tq;
    can.CAN_BS1       = CAN_BS1_9tq;
    can.CAN_BS2       = CAN_BS2_4tq;
    can.CAN_Prescaler = 3;   //CAN BaudRate 42/(1+9+4)/3=1Mbps
    CAN_Init(CAN2, &can);
    
		CAN_FilterInitTypeDef  can_filter;
		
    can_filter.CAN_FilterNumber         = 14;
    can_filter.CAN_FilterMode           = CAN_FilterMode_IdMask;
    can_filter.CAN_FilterScale          = CAN_FilterScale_32bit;
    can_filter.CAN_FilterIdHigh         = 0x0000;
    can_filter.CAN_FilterIdLow          = 0x0000;
    can_filter.CAN_FilterMaskIdHigh     = 0x0000;
    can_filter.CAN_FilterMaskIdLow      = 0x0000;
    can_filter.CAN_FilterFIFOAssignment = 0;//the message which pass the filter save in fifo0 CAN_Filter_FIFO0
    can_filter.CAN_FilterActivation     = ENABLE;
    CAN_FilterInit(&can_filter);
    
    CAN_ITConfig(CAN2,CAN_IT_FMP0, ENABLE);    ////FIFO0消息挂号中断允许
		CAN_ITConfig(CAN2,CAN_IT_TME, ENABLE);
} 

void CAN_init(void)
{
	CAN1_init();
	CAN2_init();
}
int CAN1_motor_check[7];//检查CAN1电机 首元素用来计算结果
int CAN2_motor_check[5];//检查CAN2电机 首元素用来计算结果

CanRxMsg CAN1_RxMessage;    //发送缓冲区
CanTxMsg CAN1_TxMessage;    //接受缓冲区

CanRxMsg CAN2_RxMessage;    //发送缓冲区
CanTxMsg CAN2_TxMessage;    //接受缓冲区

/*CAN1发送电流值*/
void CAN1_send(uint32_t Equipment_ID,int16_t Data0,int16_t Data1,int16_t Data2,int16_t Data3)		//底盘电机
{		
	
	CAN1_TxMessage.StdId = Equipment_ID;			 //使用的扩展ID，电调820R标识符0X200
	CAN1_TxMessage.IDE   = CAN_ID_STD;				 //标准模式
	CAN1_TxMessage.RTR   = CAN_RTR_DATA;			 //数据帧RTR=0，远程帧RTR=1
	CAN1_TxMessage.DLC   = 8;							 	   //数据长度为8字节

	CAN1_TxMessage.Data[0] = Data0>>8; 
	CAN1_TxMessage.Data[1] = Data0;
	CAN1_TxMessage.Data[2] = Data1>>8; 
	CAN1_TxMessage.Data[3] = Data1;
	CAN1_TxMessage.Data[4] = Data2>>8; 
	CAN1_TxMessage.Data[5] = Data2;
	CAN1_TxMessage.Data[6] = Data3>>8; 
	CAN1_TxMessage.Data[7] = Data3;

	CAN_Transmit(CAN1, &CAN1_TxMessage);	//发送数据
}
/*CAN1发送电流值*/
void CAN2_send(uint32_t Equipment_ID, int16_t Data0, int16_t Data1, int16_t Data2, int16_t Data3)
{				
  CAN2_TxMessage.StdId = Equipment_ID;		 //使用的标准ID
  CAN2_TxMessage.IDE   = CAN_ID_STD;				 //标准模式
  CAN2_TxMessage.RTR   = CAN_RTR_DATA;			 //发送的是数据
  CAN2_TxMessage.DLC   = 8;							 //数据长度为8字节
	
  CAN2_TxMessage.Data[0] = Data0>>8;
  CAN2_TxMessage.Data[1] = Data0;
  CAN2_TxMessage.Data[2] = Data1>>8;
  CAN2_TxMessage.Data[3] = Data1;
	CAN2_TxMessage.Data[4] = Data2>>8;      
	CAN2_TxMessage.Data[5] = Data2;
  CAN2_TxMessage.Data[6] = Data3>>8;
  CAN2_TxMessage.Data[7] = Data3;
	
	CAN_Transmit(CAN2, &CAN2_TxMessage);
}

/*CAN1中断函数*/
void CAN1_RX0_IRQHandler()
{
	if(CAN_GetITStatus(CAN1,CAN_IT_FMP0)!=RESET)
	{
		CAN_ClearITPendingBit(CAN1,CAN_IT_FMP0);		//清楚中断挂起
		CAN_Receive(CAN1, CAN_FIFO0, &CAN1_RxMessage);		//接收can数据
		
		for(u8 k = 1; k < 7; k++)
		{
      CAN1_motor_check[k] ++;
		}
		
		switch (CAN1_RxMessage.StdId)
		{
			case 0x201:
			{		
				CAN1_motor_check[1] = 0;
				chassis.wheel_angle_fdb[M1] = (int16_t)(CAN1_RxMessage.Data[0]<<8 | CAN1_RxMessage.Data[1]);		//机械角度	0~8191(0x1FFF)
				chassis.wheel_speed_fdb[M1] = (int16_t)(CAN1_RxMessage.Data[2]<<8 | CAN1_RxMessage.Data[3]);		//电机转速
			
			}break;
		
			case 0x202:
			{	
				CAN1_motor_check[2] = 0;
				chassis.wheel_angle_fdb[M2] = (int16_t)(CAN1_RxMessage.Data[0]<<8 | CAN1_RxMessage.Data[1]);		//机械角度	0~8191(0x1FFF)
				chassis.wheel_speed_fdb[M2] = (int16_t)(CAN1_RxMessage.Data[2]<<8 | CAN1_RxMessage.Data[3]);		//电机转速	
				
			}break;
		
			case 0x203:
			{	
				CAN1_motor_check[3] = 0;
				chassis.wheel_angle_fdb[M3] = (int16_t)(CAN1_RxMessage.Data[0]<<8 | CAN1_RxMessage.Data[1]);		//机械角度	0~8191(0x1FFF)
				chassis.wheel_speed_fdb[M3] = (int16_t)(CAN1_RxMessage.Data[2]<<8 | CAN1_RxMessage.Data[3]);		//电机转速
			
			}break;
		
			case 0x204:
			{	
				CAN1_motor_check[4] = 0;
				chassis.wheel_angle_fdb[M4] = (int16_t)(CAN1_RxMessage.Data[0]<<8 | CAN1_RxMessage.Data[1]);		//机械角度	0~8191(0x1FFF)
				chassis.wheel_speed_fdb[M4] = (int16_t)(CAN1_RxMessage.Data[2]<<8 | CAN1_RxMessage.Data[3]);		//电机转速

			}break;

			case 0x205:
			{	
				CAN1_motor_check[5] = 0;
				lifter.angle_fdb[L] = (int16_t)(CAN1_RxMessage.Data[0]<<8 | CAN1_RxMessage.Data[1]);	  //机械角度	0~8191(0x1FFF) 实际角度作为PID计算电机的feedback
				lifter.speed_fdb[L] = (int16_t)(CAN1_RxMessage.Data[2]<<8 | CAN1_RxMessage.Data[3]);		//电机转速	
				lifter_data_handler(L);
			}break;
		
			case 0x206:
			{	
				CAN1_motor_check[6] = 0;
				lifter.angle_fdb[R] = (int16_t)(CAN1_RxMessage.Data[0]<<8 | CAN1_RxMessage.Data[1]);   //机械角度	0~8191(0x1FFF) 实际角度作为PID计算电机的feedback
				lifter.speed_fdb[R] = (int16_t)(CAN1_RxMessage.Data[2]<<8 | CAN1_RxMessage.Data[3]);		//电机转速		
				lifter_data_handler(R);
			}break;

			default:break;
		}		
  }
}

/*CAN2中断函数*/
void CAN2_RX0_IRQHandler(void)
{
	if(CAN_GetITStatus(CAN2,CAN_IT_FMP0)!= RESET) 
  {				 
	  CAN_ClearITPendingBit(CAN2, CAN_IT_FMP0);
		CAN_Receive(CAN2, CAN_FIFO0, &CAN2_RxMessage);

		for(u8 k = 1; k < 5; k++)
		{
      CAN2_motor_check[k] ++;
		}
		
		switch (CAN2_RxMessage.StdId)
		{
//			case 0x201:
//			{	
//				assist_wheel.wheel_angle_fdb = (int16_t)(CAN2_RxMessage.Data[0]<<8 | CAN2_RxMessage.Data[1]);   //机械角度	0~8191(0x1FFF) 实际角度作为PID计算电机的feedback
//				assist_wheel.wheel_speed_fdb = (int16_t)(CAN2_RxMessage.Data[2]<<8 | CAN2_RxMessage.Data[3]);		//电机转速	
//				assist_wheel_data_handler();         
//			}break;	
			
			case 0x201:
			{	
				CAN2_motor_check[1] = 0;
				bomb_claw.angle_fdb[L] = (int16_t)(CAN2_RxMessage.Data[0]<<8 | CAN2_RxMessage.Data[1]);	  //机械角度	0~8191(0x1FFF) 实际角度作为PID计算电机的feedback
				bomb_claw.speed_fdb[L] = (int16_t)(CAN2_RxMessage.Data[2]<<8 | CAN2_RxMessage.Data[3]);		//电机转速	
				
				if(bomb_claw.LR_reset)
				{
			  	bomb_claw_data_handler(L);
				}		
				else Overflow_check[0] = 0;				
			}break;
			
			case 0x202:
			{	
				CAN2_motor_check[2] = 0;
				bomb_claw.angle_fdb[R] = (int16_t)(CAN2_RxMessage.Data[0]<<8 | CAN2_RxMessage.Data[1]);	  //机械角度	0~8191(0x1FFF) 实际角度作为PID计算电机的feedback
				bomb_claw.speed_fdb[R] = (int16_t)(CAN2_RxMessage.Data[2]<<8 | CAN2_RxMessage.Data[3]);		//电机转速	
				
				if(bomb_claw.LR_reset)
				{
					bomb_claw_data_handler(R);
				}
				else Overflow_check[0] = 0;
			}break;
			
			case 0x203:
			{	
				CAN2_motor_check[3] = 0;
				bomb_claw.angle_fdb[Y] = (int16_t)(CAN2_RxMessage.Data[0]<<8 | CAN2_RxMessage.Data[1]);	  //机械角度	0~8191(0x1FFF) 实际角度作为PID计算电机的feedback
				bomb_claw.speed_fdb[Y] = (int16_t)(CAN2_RxMessage.Data[2]<<8 | CAN2_RxMessage.Data[3]);		//电机转速	
				
				if(bomb_claw.Y_reset)
				{
					bomb_claw_data_handler(Y);
				}
			}break;
			
			case 0x204:
			{	
				CAN2_motor_check[4] = 0;
				trailer.angle_fdb = (int16_t)(CAN2_RxMessage.Data[0]<<8 | CAN2_RxMessage.Data[1]);   //机械角度	0~8191(0x1FFF) 实际角度作为PID计算电机的feedback
				trailer.speed_fdb = (int16_t)(CAN2_RxMessage.Data[2]<<8 | CAN2_RxMessage.Data[3]);		//电机转速	
				trailer_data_handler();         
			}break;		
			
//			case 0x206:
//			{	
//				bomb_claw.angle_fdb[X] = (int16_t)(CAN2_RxMessage.Data[0]<<8 | CAN2_RxMessage.Data[1]);	  //机械角度	0~8191(0x1FFF) 实际角度作为PID计算电机的feedback
//				bomb_claw.speed_fdb[X] = (int16_t)(CAN2_RxMessage.Data[2]<<8 | CAN2_RxMessage.Data[3]);		//电机转速	
//				bomb_claw_data_handler(X);
//			}break;

			default:break;
		}
	}
}

bool CAN1_motor_offline_check(void)
{
	CAN1_motor_check[0] = 0;
	
	for(u8 k = 1; k < 7; k++)
	{
		CAN1_motor_check[0] += CAN1_motor_check[k];
	}
	if(CAN1_motor_check[0] > 1000)
	{
	  return YES;
	}
	else return NO;
}

bool CAN2_motor_offline_check(void)
{
	CAN2_motor_check[0] = 0;
	
	for(u8 k = 1; k < 5; k++)
	{
		CAN2_motor_check[0] += CAN2_motor_check[k];
	}
	if(CAN2_motor_check[0] > 10000)
	{
	  return YES;
	}
	else return NO;
}



