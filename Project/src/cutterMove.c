/* 
* @Author: jeasinema
* @Date:   2015-12-06 22:24:59
* @Last Modified by:   jeasinema
* @Last Modified time: 2015-12-11 23:17:59
*/

#include "cutterMove.h"
#include "button_rocker.h"
#include "motor.h"
#include "math.h"


extern uint8_t Cutter_Init_Flag;
extern uint8_t Cutter_Control_Flag;
static unsigned char drillPosition = 2 ;//1----down 2-----up
/*
Stepper1
X
|
|
|
|
|
|
|-----------------------------Y  Stepper2
*/
void Cutter_Move_Manually(float speed , float angle , float DCSpeed)   //传入速度角度和大小控制运动
{
	
	DCMotor_Speed(DCSpeed);
	Stepper_Speed(stepper1 , speed);
	Stepper_Speed(stepper2 , speed*cos(angle));   
}

void Cutter_Move_Automatically(CMD *cmd)   //传入速度角度和大小控制运动
{
	float deltaX = cmd->x_target - cmd->x_current;
	float deltaY = cmd->y_target - cmd->y_current;
	float angle = atan((float)deltaY/(float)deltaX);
	Stepper_Speed(stepper1, cos(angle) * 60);
	Stepper_Speed(stepper2, sin(angle) * 60);
	float distance = sqrt(deltaX*deltaX + deltaY*deltaY);
	float delayTime = distance/60*0xFFFFFF;
	Delay(delayTime);
	cutterStop();
}

void Cutter_Move_Drill(unsigned char flag)       		//1----down 2-----up
{
	if(flag==1)
	{
		if(drillPosition==2){
			Stepper_Speed(stepper3 , 30);
			Delay(0xFFFF); 				//TODO：确定准确的数值
			Stepper_Speed(stepper3 , 0);
		}
		drillPosition=1;
	}
	else 
	{
		if(drillPosition==1){
			Stepper_Speed(stepper3 , -30);
			Delay(0xFFFFFF);
			Stepper_Speed(stepper3 , 0);
		}
		drillPosition=2;
	}
}

void Cutter_Init(void)						//回原点
{
	Stepper_Speed(stepper1 , -60);
	Stepper_Speed(stepper2 , -60);
	while(!(Cutter_Init_Flag>=2));
	Cutter_Control_Flag=0;//手动模式使能
}

int whetherReachDestination(){
	return 0;
}

void cutterStop(void){
	Stepper_Speed(stepper1 , 0);
	Stepper_Speed(stepper2 , 0);
}

void EXTI0_IRQHandler(void)   //到达x最小
{
	if(EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		Cutter_Init_Flag+=1;
		//Cutter_Control_Flag=0;   //手动模式使能
		Stepper_Speed(stepper1 , 0);
	}
	EXTI_ClearITPendingBit(EXTI_Line0);
}


void EXTI1_IRQHandler(void)   //到达x最大
{
	if(EXTI_GetITStatus(EXTI_Line1) != RESET)
	{
		Cutter_Control_Flag=0;
		Stepper_Speed(stepper1 , -60);  //X方向电机反向
	}
	EXTI_ClearITPendingBit(EXTI_Line1);
}

void EXTI2_IRQHandler(void)  //到达y最小
{
	if(EXTI_GetITStatus(EXTI_Line2) != RESET)
	{
		Cutter_Init_Flag+=1;
		//Cutter_Control_Flag=0;
		Stepper_Speed(stepper2 , 0);
	}
	EXTI_ClearITPendingBit(EXTI_Line2);
}

void EXTI3_IRQHandler(void)  //到达y最大
{
	if(EXTI_GetITStatus(EXTI_Line3) != RESET)
	{
		Cutter_Control_Flag=0;
		Stepper_Speed(stepper2 , -60);
	}
	EXTI_ClearITPendingBit(EXTI_Line3);
}



