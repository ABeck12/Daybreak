#include "glm/glm.hpp"
#include <string>

//char* Vec2ToString(const glm::vec2& vec)
//{
//	std::string out, x, y;
//	x = std::to_string(vec.x);
//	y = std::to_string(vec.y);
//	out = "(" + x + "," + y + ")";
//	return out.c_str();
//	//str = "(" +   vec.x << "," << vec.y << "," << vec.z << "," << vec.w << ")";
//}

#define GUILOGVEC2(vec) "%.3f, %.3f", vec.x, vec.y
#define GUILOGVEC3(vec) "%.3f, %.3f, %.3f", vec.x, vec.y, vec.z
#define GUILOGVEC4(vec) "%.3f, %.3f, %.3f, $0.3f", vec.x, vec.y, vec.z, vec.w




inline std::ostream& operator<<(std::ostream& os, const glm::vec4& vec)
{
	return os << "(" << vec.x << "," << vec.y << "," << vec.z << "," << vec.w << ")";
}

inline std::ostream& operator<<(std::ostream& os, const glm::vec3& vec)
{
	return os << "(" << vec.x << "," << vec.y << "," << vec.z << ")";
}

inline std::ostream& operator<<(std::ostream& os, const glm::vec2& vec)
{
	return os << "(" << vec.x << "," << vec.y << ")";
}