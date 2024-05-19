#pragma once

#include <cstdint>

namespace Daybreak
{
	// Tested on xbox360 controller
	// Does not work with nintendo switch controllers yet
	enum class GamepadButton : uint16_t
	{
		ButtonRight = 1,
		ButtonDown = 0,
		ButtonUp = 3,
		ButtonLeft = 2,

		Select = 6,
		Start = 7,
		// Menu = 14,

		LeftBumper = 4,
		RightBumper = 5,
		LeftStickIn = 8,
		RightStickIn = 9,

		DPadUp = 10,
		DPadRight = 11,
		DPadDown = 12,
		DPadLeft = 13,

		// LeftTrigger,  // This will be for switch trigger press
		// RightTrigger, // This will be for switch trigger press
	};

	enum class GamepadAxis : uint16_t
	{
		LeftX = 0,
		LeftY = 1,
		RightX = 2,
		RightY = 3,
		LeftTrigger = 4,
		RightTrigger = 5,
	};

	enum class Gamepad : uint16_t
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