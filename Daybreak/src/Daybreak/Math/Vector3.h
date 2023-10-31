#pragma once

namespace Daybreak
{
	struct Vec3
	{
	public:
		Vec3(float x, float y, float z);
		~Vec3();

		//float Dot(Vec3& other) const;
		//Vec3 Cross(Vec3& other) const;
		//void Set(const float _x, const float _y, const float _z);
		//float Magnitude() const;
		//float Normalize();

		static inline Vec3 One() { return Vec3(1.0f, 1.0f, 1.0f); }
		static inline Vec3 Zero() { return Vec3(0.0f, 0.0f, 0.0f); }

		float x, y, z;

	public:
		inline Vec3 operator+(const Vec3& other) const
		{
			return Vec3(x + other.x, y + other.y, z + other.z);
		}

		inline Vec3 operator-(const Vec3& other) const
		{
			return Vec3(x - other.x, y - other.y, z - other.z);
		}

		inline Vec3 operator*(const float& other) const
		{
			return Vec3(x * other, y * other, z * other);
		}

		inline Vec3 operator/(const float& other) const
		{
			return Vec3(x / other, y / other, z / other);
		}

		inline bool operator==(const Vec3& other) const
		{
			return (x == other.x && y == other.y && z == other.z) ? true : false;
		}
	};

	inline std::ostream& operator<<(std::ostream& os, const Vec3& vec)
	{
		return os << "(" << vec.x << "," << vec.y << "," << vec.z << ")";
	}

}