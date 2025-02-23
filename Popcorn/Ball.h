﻿#pragma once

#include "Config.h"

//------------------------------------------------------------------------------------------------------------
enum EBall_State
{
	EBS_Normal,
	EBS_Lost,
	EBS_On_Platform
};
//------------------------------------------------------------------------------------------------------------
class ABall;
class AHit_Checker
{
public:
	virtual bool Check_Hit(double next_x_pos, double next_y_pos, ABall *ball) = 0;
};
//------------------------------------------------------------------------------------------------------------
class ABall
{
public:
	ABall();

	void Init();

	void Draw(HDC hdc, RECT &paint_area);
	void Move(AHit_Checker *level_hit_checker, AHit_Checker *border_hit_checker, AHit_Checker *paltform_hit_checker);
	EBall_State Get_State();
	void Set_State(EBall_State new_state, double x_pos);

	double Ball_Direction;
	double Rest_Distance;

	static const double Radius;

private:
	void Redraw_Ball();

	EBall_State Ball_State;
	HPEN Ball_Pen;
	HBRUSH Ball_Brush;
	
	double Ball_Speed;

	double Center_X_Pos, Center_Y_Pos;

	RECT Ball_Rect, Prev_Ball_Rect;

	static const double Start_Ball_Y_Pos;

	
};
//------------------------------------------------------------------------------------------------------------
