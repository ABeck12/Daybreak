#pragma once

namespace Daybreak
{
	struct Vec2
	{
	public:
		Vec2(float x, float y);
		~Vec2();

		//float Dot(Vec2& other) const;
		//void Set(const float _x, const float _y);
		//float Magnitude() const;
		//float Normalize();

		static inline Vec2 One() { return Vec2(1.0f, 1.0f); }
		static inline Vec2 Zero() { return Vec2(0.0f, 0.0f); }

		float x, y;



	public:
		inline Vec2 operator+(const Vec2& other) const
		{
			return Vec2(x + other.x, y + other.y);
		}

		inline Vec2 operator-(const Vec2& other) const
		{
			return Vec2(x - other.x, y - other.y);
		}

		inline Vec2 operator*(const float& other) const
		{
			return Vec2(x * other, y * other);
		}

		inline Vec2 operator/(const float& other) const
		{
			return Vec2(x / other, y / other);
		}

		inline bool operator==(const Vec2& other) const
		{
			return (x == other.x && y == other.y) ? true : false;
		}

	};

	inline std::ostream& operator<<(std::ostream& os, const Vec2& vec)
	{
		return os << "(" << vec.x << "," << vec.y << ")";
	}

}