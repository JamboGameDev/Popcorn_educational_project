﻿#include "Ball.h"

// ABall
const double ABall::Start_Ball_Y_Pos = 181.0;
const double ABall::Radius = 2.0;
//------------------------------------------------------------------------------------------------------------
ABall::ABall()
: Ball_State(EBS_Normal), Ball_Pen(0), Ball_Brush(0), Center_X_Pos(0.0), Center_Y_Pos(Start_Ball_Y_Pos), Ball_Speed(0.0), Rest_Distance(0.0), Ball_Direction(0), Ball_Rect{}, Prev_Ball_Rect{}
{
	Set_State(EBS_Normal, 0);
}
//------------------------------------------------------------------------------------------------------------
void ABall::Init()
{
	AsConfig::Create_Pen_Brush(255, 255, 255, Ball_Pen, Ball_Brush);
}
//------------------------------------------------------------------------------------------------------------
void ABall::Draw(HDC hdc, RECT &paint_area)
{
	RECT intersection_rect;

	// 1. Очищаем фон
	if (IntersectRect(&intersection_rect, &paint_area, &Prev_Ball_Rect) )
	{
		SelectObject(hdc, AsConfig::BG_Pen);
		SelectObject(hdc, AsConfig::BG_Brush);

		Ellipse(hdc, Prev_Ball_Rect.left, Prev_Ball_Rect.top, Prev_Ball_Rect.right - 1, Prev_Ball_Rect.bottom - 1);
	}

	// 2. Рисуем шарик
	if (IntersectRect(&intersection_rect, &paint_area, &Ball_Rect) )
	{
		SelectObject(hdc, Ball_Pen);
		SelectObject(hdc, Ball_Brush);

		Ellipse(hdc, Ball_Rect.left, Ball_Rect.top, Ball_Rect.right - 1, Ball_Rect.bottom - 1);
	}
}
//------------------------------------------------------------------------------------------------------------
void ABall::Move(AHit_Checker *level_hit_checker, AHit_Checker *border_hit_checker,AHit_Checker *paltform_hit_checker)
{
	bool got_hit;
	double next_x_pos, next_y_pos;
	int platform_y_pos = AsConfig::Platform_Y_Pos - AsConfig::Ball_Size;
	
	double step_size = 1.0 / AsConfig::Global_Scale;

	if (Ball_State != EBS_Normal)
		return;

	Prev_Ball_Rect = Ball_Rect;
	Rest_Distance += Ball_Speed;

	while (Rest_Distance >= step_size)
	{
		got_hit = false;

	next_x_pos = Center_X_Pos + step_size * cos(Ball_Direction);
	next_y_pos = Center_Y_Pos - step_size * sin(Ball_Direction);

	// Корректируем позицию при отражении:
	got_hit |= border_hit_checker->Check_Hit(next_x_pos, next_y_pos, this); //рамки
	got_hit |= level_hit_checker->Check_Hit(next_x_pos, next_y_pos, this); //кирпичей
	got_hit |= paltform_hit_checker->Check_Hit(next_x_pos, next_y_pos, this); //платформа
	

	

	// Мячик продлжит движения, если не взаимодействовал с другими объектами
	if (!got_hit)
	  {
		Rest_Distance -= step_size;

	  Center_X_Pos = next_x_pos;
	  Center_Y_Pos = next_y_pos;
	  }
	}
	Redraw_Ball();
}
//------------------------------------------------------------------------------------------------------------
EBall_State ABall::Get_State()
{
	return Ball_State;
}
//------------------------------------------------------------------------------------------------------------
void ABall::Set_State(EBall_State new_state, double x_pos)
{
	switch (new_state)
	{
	case EBS_Normal:
		Center_X_Pos = (double)x_pos;
		Center_Y_Pos = Start_Ball_Y_Pos;
		Ball_Speed = 3.0;
		Rest_Distance = 0.0;
		Ball_Direction = M_PI - M_PI_4;
		Redraw_Ball();
		break;


	case EBS_Lost:
		Ball_Speed = 0.0;
		break;


	case EBS_On_Platform:
		Center_X_Pos = (double)x_pos;
		Center_Y_Pos = Start_Ball_Y_Pos;
		Ball_Speed = 0.0;
		Rest_Distance = 0.0;
		Ball_Direction = M_PI - M_PI_4;
		Redraw_Ball();
		break;
	}

	Ball_State = new_state;
}
//------------------------------------------------------------------------------------------------------------
void ABall::Redraw_Ball()
{
	Ball_Rect.left = (int)((Center_X_Pos - Radius) * AsConfig::Global_Scale);
	Ball_Rect.top = (int)((Center_Y_Pos - Radius) * AsConfig::Global_Scale);
	Ball_Rect.right = (int)((Center_X_Pos + Radius) * AsConfig::Global_Scale);
	Ball_Rect.bottom = (int)((Center_Y_Pos + Radius) * AsConfig::Global_Scale);

	InvalidateRect(AsConfig::Hwnd, &Prev_Ball_Rect, FALSE);
	InvalidateRect(AsConfig::Hwnd, &Ball_Rect, FALSE);
}
//------------------------------------------------------------------------------------------------------------

