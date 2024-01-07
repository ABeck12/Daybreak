#pragma once

namespace Daybreak
{
	using MouseCode = uint16_t;

	namespace Mouse
	{
		enum : MouseCode
		{
			Button1         = 0,
			Button2         = 1,
			Button3         = 2,
			Button4         = 3,
			Button5         = 4,
			Button6         = 5,
			Button7         = 6,
			Button8         = 7,
			ButtonLast      = Button8,
			ButtonLeft      = Button1,
			ButtonRight     = Button2,
			ButtonMiddle    = Button3
		};
	}
}
//// From glfw3.h
//#define DB_MOUSE_BUTTON_1         0
//#define DB_MOUSE_BUTTON_2         1
//#define DB_MOUSE_BUTTON_3         2
//#define DB_MOUSE_BUTTON_4         3
//#define DB_MOUSE_BUTTON_5         4
//#define DB_MOUSE_BUTTON_6         5
//#define DB_MOUSE_BUTTON_7         6
//#define DB_MOUSE_BUTTON_8         7
//#define DB_MOUSE_BUTTON_LAST      DB_MOUSE_BUTTON_8
//#define DB_MOUSE_BUTTON_LEFT      DB_MOUSE_BUTTON_1
//#define DB_MOUSE_BUTTON_RIGHT     DB_MOUSE_BUTTON_2
//#define DB_MOUSE_BUTTON_MIDDLE    DB_MOUSE_BUTTON_3