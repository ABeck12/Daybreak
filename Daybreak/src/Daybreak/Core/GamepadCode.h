#pragma onnce

#include <cstdint>

namespace Daybreak
{
	using GamepadCode = uint16_t;
	using GamepadGroupCode = uint16_t;

	namespace Gamepad
	{
		enum : GamepadCode
		{
			ButtonRight = 0,
			ButtonDown = 1,
			ButtonUp = 2,
			ButtonLeft = 3,

			Select = 6,
			Start = 7,

			LeftBumper = 4,
			RightBumper = 5,
			LeftStickIn = 9,
			RightStickIn = 10,

			DPadUp = 11,
			DPadRight = 12,
			DPadDown = 13,
			DPadLeft = 14,

			AxisLeftX,
			AxisLeftY,
			AxisRightX,
			AxisRightY,

			AxisLeftTrigger,
			AxisRightTrigger,
		};
	}

	namespace GamepadGroup
	{
		enum : GamepadGroupCode
		{
			Gamepad1 = 0,
			Gamepad2 = 1,
			Gamepad3 = 2,
			Gamepad4 = 3,
			Gamepad5 = 4,
			Gamepad6 = 5,
			Gamepad7 = 6,
			Gamepad8 = 7,
			Gamepad9 = 8,
			Gamepad10 = 9,
			Gamepad11 = 10,
			Gamepad12 = 11,
			Gamepad13 = 12,
			Gamepad14 = 13,
			Gamepad15 = 14,
			Gamepad16 = 15,
			GamepadLast = Gamepad16,
		};
	}
}