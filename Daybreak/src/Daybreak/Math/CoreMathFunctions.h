#pragma once

//#include "dbpch.h"

namespace Daybreak::Math
{
	inline const float Pi = 3.14159265358979323846f;
	inline const float Epsilon = 1.192092896e-07f;
	inline const float Sqrt2 = 1.41421356237309504880f;
	inline const float Sqrt3 = 1.73205080756887729352f;
	inline const float OneOverSqrt2 = 0.70710678118654752440f;
	inline const float OneOverSqrt3 = 0.57735026918962576450f;



	float Min(const float value1, const float value2);
	float Max(const float value1, const float value2);
	
	float Sign(const float value);
	
	float Abs(const float value);
	
	float Sqrt(const float value);

	float Radians(const float value);
	float Degrees(const float value);
	
	float Sin(const float value);
	float Cos(const float value);
	float Tan(const float value);
	 
	float Sinh(const float value);
	float Cosh(const float value);
	float Tanh(const float value);
	
	float ASin(const float value);
	float ACos(const float value);
	float ATan(const float value);
	
	float ASinh(const float value);
	float ACosh(const float value);
	float ATanh(const float value);

	int Rand();
	int RandInRange(int min, int max);

	float Randf();
	float RandfInRange(float min, float max);
}