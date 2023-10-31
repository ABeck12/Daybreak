#pragma once

//#include "dbpch.h"

namespace Daybreak::Math
{
	float Min(const float value1, const float value2);
	float Max(const float value1, const float value2);
	
	float Sign(const float value);
	
	float Abs(const float value);
	
	float Sqrt(const float value);
	
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