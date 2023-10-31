#pragma once

namespace Daybreak
{
	struct Vec4
	{
	public:
		Vec4() : x(0), y(0), z(0), w(0) {}
		Vec4(float x, float y, float z, float w);
		~Vec4();

		//float Dot(Vec4& other) const;
		//Vec4 Cross(Vec4& other) const;
		//void Set(const float _x, const float _y, const float _z, const float _w);
		//float Magnitude() const;
		//float Normalize();

		static inline Vec4 One() { return Vec4(1.0f, 1.0f, 1.0f, 1.0f); }
		static inline Vec4 Zer() { return Vec4(0.0f, 0.0f, 0.0f, 0.0f); }


		float x, y, z, w;

	public:
		inline Vec4 operator+(const Vec4& other) const
		{
			return Vec4(x + other.x, y + other.y, z + other.z, w + other.w);
		}

		inline Vec4 operator-(const Vec4& other) const
		{
			return Vec4(x - other.x, y - other.y, z - other.z, w - other.w);
		}

		inline Vec4 operator*(const float& other) const
		{
			return Vec4(x * other, y * other, z * other, z * other);
		}

		inline Vec4 operator/(const float& other) const
		{
			return Vec4(x / other, y / other, z / other, z / other);
		}

		inline bool operator==(const Vec4& other) const
		{
			return (x == other.x && y == other.y && z == other.z && w == other.w) ? true : false;
		}
	};

	inline std::ostream& operator<<(std::ostream& os, const Vec4& vec)
	{
		return os << "(" << vec.x << "," << vec.y << "," << vec.z << "," << vec.w << ")";
	}
}