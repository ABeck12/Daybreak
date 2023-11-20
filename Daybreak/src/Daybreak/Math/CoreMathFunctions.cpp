#include "dbpch.h"

#include "Daybreak/Math/CoreMathFunctions.h"

#include <math.h>

namespace Daybreak::Math
{
	float Min(const float value1, const float value2) { return std::fminf(value1, value2); }
	float Max(const float value1, const float value2) { return std::fmaxf(value1, value2); }

	float Sign(const float value)
	{
		if (value == 0.0)
			return 0.0f;
		return (value > 0.0f) ? 1.0f : -1.0f;
	}
	
	float Abs(const float value) { return std::fabs(value); }

	float Sqrt(const float value) { return std::sqrtf(value); }

	float Radians(const float value) { return value * Pi / 180.0f; }
	float Degrees(const float value) { return value * 180.0f / Pi; }

	float Sin(const float value) { return std::sinf(value); }
	float Cos(const float value) { return std::cosf(value); }
	float Tan(const float value) { return std::tanf(value); }

	float Sinh(const float value) { return std::sinhf(value); }
	float Cosh(const float value) { return std::coshf(value); }
	float Tanh(const float value) { return std::tanhf(value); }

	float ASin(const float value) { return std::sinf(value); }
	float ACos(const float value) { return std::cosf(value); }
	float ATan(const float value) { return std::tanf(value); }

	float ASinh(const float value) { return std::asinhf(value); }
	float ACosh(const float value) { return std::acoshf(value); }
	float ATanh(const float value) { return std::atanhf(value); }

	//int Rand()
	//int RandInRange(int min, int max);

	//float Randf();
	//float RandfInRange(float min, float max);
}