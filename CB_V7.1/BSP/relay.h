#ifndef __RELAY_H__
#define __RELAY_H__

#include "system.h"

/*抱杆爪子控制*/
#define TUBE_CLAW_CATCH             \
{                                   \
  GPIO_SetBits(GPIOD,GPIO_Pin_14);  \
	flag_tube_claw_catch = 1;         \
}                                   \

#define TUBE_CLAW_LOOSE             \
{                                   \
	GPIO_ResetBits(GPIOD,GPIO_Pin_14);\
	flag_tube_claw_catch = 0;         \
}                                   \

///*刹车控制*/
//#define TUBE_BRAKE_PULL  GPIO_SetBits(GPIOB,GPIO_Pin_1) //刹车抽回 
//#define TUBE_BRAKE_PUSH  GPIO_ResetBits(GPIOB,GPIO_Pin_1) //刹车推出

/*取弹爪子抓取控制*/
#define BOMB_CLAW_CATCH             \
{                                   \
  GPIO_ResetBits(GPIOE,GPIO_Pin_3); \
	flag_bomb_claw_catch = 1;         \
}                                   \

#define BOMB_CLAW_LOOSE             \
{                                   \
	GPIO_SetBits(GPIOE,GPIO_Pin_3);   \
	flag_bomb_claw_catch = 0;         \
}                                   \

/*弹射控制*/
#define BOMB_EJECT_PUSH  GPIO_SetBits(GPIOE,GPIO_Pin_4)   //弹射推出
#define BOMB_EJECT_PULL  GPIO_ResetBits(GPIOE,GPIO_Pin_4) //弹射抽回 

/*取弹爪子推拉控制*/
#define BOMB_CLAW_PUSH              \
{                                   \
  GPIO_SetBits(GPIOD,GPIO_Pin_11);  \
	flag_bomb_claw_push = 1;          \
}                                   \

#define BOMB_CLAW_PULL              \
{                                   \
	GPIO_ResetBits(GPIOD,GPIO_Pin_11);\
	flag_bomb_claw_push = 0;          \
}                                   \

/*拖车钩子上下控制*/
#define TRAILER_HOOK_PUSH           \
{                                   \
  GPIO_SetBits(GPIOD,GPIO_Pin_10);  \
	flag_trailer_hook = 1;            \
}                                   \

#define TRAILER_HOOK_PULL           \
{                                   \
	GPIO_ResetBits(GPIOD,GPIO_Pin_10);\
	flag_trailer_hook = 0;            \
}                                   \

///*支撑腿控制*/
//#define SUPPORTING_WHEEL_PUSH       \
//{                                   \
//	GPIO_SetBits(GPIOC,GPIO_Pin_8);   \
//	flag_supporting_wheel = 1;        \
//}                                   \

//#define SUPPORTING_WHEEL_PULL       \
//{                                   \
//	GPIO_ResetBits(GPIOC,GPIO_Pin_8); \
//	flag_supporting_wheel = 0;        \
//}                                   \


/*视野切换控制*/
#define CCTV_FEED_RESCUE              \
{                                   \
	GPIO_SetBits(GPIOE,GPIO_Pin_5);   \
	GPIO_ResetBits(GPIOE,GPIO_Pin_6); \
}

#define CCTV_GET_BOMB               \
{                                   \
	GPIO_SetBits(GPIOE,GPIO_Pin_6);   \
	GPIO_ResetBits(GPIOE,GPIO_Pin_5); \
}

#define CCTV_CLIMB_TUBE             \
{                                   \
  GPIO_ResetBits(GPIOE,GPIO_Pin_6); \
	GPIO_ResetBits(GPIOE,GPIO_Pin_5); \
}  


void relay_init(void);
void CCTV_switch_task(void);

#endif 
