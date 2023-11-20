#pragma once

#include "Daybreak/Math/CoreMathFunctions.h"


namespace Daybreak
{
#pragma region Vec2
	struct Vec2
	{
		Vec2(const float _x, const float _y) : x(_x), y(_y) {}
		Vec2(const float value) : x(value), y(value) {}
		Vec2() : x(0.0f), y(0.0f) {}

		union { float x, r, s, u; };
		union { float y, g, t, v; };


		static inline Vec2 One() { return Vec2(1.0f, 1.0f); }
		static inline Vec2 Zero() { return Vec2(0.0f, 0.0f); }
		static inline Vec2 Up() { return Vec2(0.0f, 1.0f); }
		static inline Vec2 Left() { return Vec2(-1.0f, 0.0f); }
		static inline Vec2 Right() { return Vec2(1.0f, 0.0f); }
		static inline Vec2 Down() { return Vec2(0.0f, -1.0f); }
		inline float LengthSqr() { return x * x + y * y; }
		inline float Length() { return Math::Sqrt(LengthSqr()); }
		inline void Normalize()
		{
			const float length = Length();
			x /= length;
			y /= length;
		}



		inline Vec2 operator+(const Vec2& other) const
		{
			return Vec2(x + other.x, y + other.y);
		}

		inline Vec2 operator-(const Vec2& other) const
		{
			return Vec2(x - other.x, y - other.y);
		}

		inline Vec2 operator-() const
		{
			return Vec2(-x, -y);
		}

		inline Vec2 operator*(const float& other) const
		{
			return Vec2(x * other, y * other);
		}

		inline Vec2 operator/(const float& other) const
		{
			return Vec2(x / other, y / other);
		}

		inline float operator[](const int& i)
		{
			//DB_CORE_ASSERT(i <= 1 && i >=0, "Vec2 index out of range!");
			switch (i)
			{
			default:
			case 0:
				return x;
			case 1:
				return y;
			}
		}
	};
#pragma endregion






#pragma region Vec3
	struct Vec3
	{
		Vec3(const float _x, const float _y, const float _z) : x(_x), y(_y), z(_z) {}
		Vec3(const float value) : x(value), y(value), z(value) {}
		Vec3() : x(0.0f), y(0.0f), z(0.0f) {}

		union { float x, r, s, u; };
		union { float y, g, t, v; };
		union { float z, b, p, w; };


		static inline Vec3 One() { return Vec3(1.0f, 1.0f, 1.0f); }
		static inline Vec3 Zero() { return Vec3(0.0f, 0.0f, 0.0f); }
		static inline Vec3 Up() { return Vec3(0.0f, 1.0f, 0.0f); }
		static inline Vec3 Left() { return Vec3(-1.0f, 0.0f, 0.0f); }
		static inline Vec3 Right() { return Vec3(1.0f, 0.0f, 0.0f); }
		static inline Vec3 Down() { return Vec3(0.0f, -1.0f, 0.0f); }
		static inline Vec3 Forward() { return Vec3(0.0f, 0.0f, 1.0f); }
		static inline Vec3 Backward() { return Vec3(0.0f, 0.0f, -1.0f); }

		inline float LengthSqr() { return x * x + y * y+ z * z; }
		inline float Length() { return Math::Sqrt(LengthSqr()); }
		inline void Normalize()
		{
			float length = Length();
			x /= length;
			y /= length;
			z /= length;
		}
		



		inline Vec3 operator+(const Vec3& other) const
		{
			return Vec3(x + other.x, y + other.y, z + other.z);
		}

		inline Vec3 operator-(const Vec3& other) const
		{
			return Vec3(x - other.x, y - other.y, z - other.z);
		}

		inline Vec3 operator-() const
		{
			return Vec3(-x, -y, -z);
		}

		inline Vec3 operator*(const float& other) const
		{
			return Vec3(x * other, y * other, z * other);
		}





		inline Vec3 operator/(const float& other) const
		{
			return Vec3(x / other, y / other, z / other);
		}

		inline float operator[](const int& i)
		{
			//DB_CORE_ASSERT(i <= 2 && i >=0, "Vec2 index out of range!");
			switch (i)
			{
			default:
			case 0:
				return x;
			case 1:
				return y;
			case 2:
				return z;
			}
		}

	};
#pragma endregion


	inline Vec3 operator*(const float& other, const Vec3& vec)
	{
		return Vec3(vec.x * other, vec.y * other, vec.z * other);
	}

	inline Vec3 operator*(const Vec3& vec, const float& other)
	{
		return Vec3(vec.x * other, vec.y * other, vec.z * other);
	}




#pragma region Vec4
	struct Vec4
	{
		Vec4(const float _x, const float _y, const float _z, const float _w) : x(_x), y(_y), z(_z), w(_w) {}
		Vec4(const float value) : x(value), y(value), z(value), w(value) {}
		Vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}

		union { float x, r, s; };
		union { float y, g, t; };
		union { float z, b, p; };
		union { float w, a, q; };


		static inline Vec4 One() { return Vec4(1.0f, 1.0f, 1.0f, 1.0f); }
		static inline Vec4 Zero() { return Vec4(0.0f, 0.0f, 0.0f, 0.0f); }
		inline float LengthSqr() { return x * x + y * y + z * z + w * w; }
		inline float Length() { return Math::Sqrt(LengthSqr()); }

		inline void Normalize()
		{
			const float length = Length();
			x /= length;
			y /= length;
			z /= length;
			w /= length;
		}



		inline Vec4 operator+(const Vec4& other) const
		{
			return Vec4(x + other.x, y + other.y, z + other.z, w + other.w);
		}

		inline Vec4 operator-(const Vec4& other) const
		{
			return Vec4(x - other.x, y - other.y, z - other.z, w - other.w);
		}

		inline Vec4 operator-() const
		{
			return Vec4(-x, -y, -z, -w);
		}

		inline Vec4 operator*(const float& other) const
		{
			return Vec4(x * other, y * other, z * other, z * other);
		}

		inline float operator[](const int& i)
		{
			switch (i)
			{
			default:
			case 0:
				return x;
			case 1:
				return y;
			case 2:
				return z;
			case 3:
				return w;
			}
		}


	};
#pragma endregion


	static inline float Dot(const Vec2& v1, const Vec2& v2) { return v1.x * v2.x + v1.y * v2.y; }
	static inline float Dot(const Vec3& v1, const Vec3& v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; }
	static inline float Dot(const Vec4& v1, const Vec4& v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w; }

	static inline Vec3 Cross(const Vec3& v1, const Vec3& v2)
	{
		return Vec3(
			v1.y * v2.z - v1.z * v2.y,
			v1.z * v2.x - v1.x * v2.z,
			v1.x * v2.y - v1.y * v2.x
		);
	}

	//static inline Vec3 Distance(const Vec2& v1, const Vec2& v2); //Finds the distance between two vectors to a point




#pragma region ostream Overload
	inline std::ostream& operator<<(std::ostream& os, const Vec2& vec)
	{
		return os << "(" << vec.x << "," << vec.y << ")";
	}
	inline std::ostream& operator<<(std::ostream& os, const Vec3& vec)
	{
		return os << "(" << vec.x << "," << vec.y << "," << vec.z << ")";
	}
	inline std::ostream& operator<<(std::ostream& os, const Vec4& vec)
	{
		return os << "(" << vec.x << "," << vec.y << "," << vec.z << "," << vec.w << ")";
	}
#pragma endregion
}