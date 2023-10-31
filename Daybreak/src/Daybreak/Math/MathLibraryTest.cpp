#include "dbpch.h"

#include <iostream>

#include "2nd try/CommonFunc.h"
#include "2nd try/Vector.h"
#include "2nd try/Matrix.h"
#include "2nd try/Quaternion.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#define LOG(x) std::cout << x << std::endl

using namespace Daybreak;

int main()
{

	//glm::vec3
	//Vec3 right = Vec3::Right();
	//Vec3 up = Vec3::Up();
	//Vec3 z_axis(right - up);
	//Quat quat(1.0f, 1.0f, 1.0f, 1.0f);
	//Vec4 test(1.0f, 1.0f, 1.0f, 1.0f);
	//Quat::Test();

	glm::vec4 glmvec4;
	Vec4 myVec4;
	//Daybreak::Vec3 test;
	//test.x = 5;
	LOG(myVec4);
	//LOG(glm::to_string(glmvec4));




	//Daybreak::Mat4 three(3.0f);
	//Daybreak::Mat4 two(2.0f);

	//LOG(glm::to_string(glm::ortho(0.0f, 800.f, 0.0f, 600.0f, -1.0f, 1.0f)));
	//LOG(Daybreak::Mat4::Orthographic(0.0f, 800.f,0.0f, 600.0f, -1.0f, 1.0f));



	//LOG(glm::to_string(glm::perspective(glm::radians(90.0f), 800.0f / 600.0f, .01f, 100.0f)));
	//LOG(Daybreak::Mat4::Perspective(glm::radians(90.0f), 800.0f / 600.0f, .01f, 100.0f));
	//LOG(test);

	//Daybreak::Mat4 mat(test, test, Daybreak::Vec4::Zero(), test);

	//LOG(Daybreak::Mat4(1.0f));
	//LOG(Daybreak::Vec2::One());
	//LOG(Daybreak::Vec2::Zero());
	//LOG(Daybreak::Vec3::One());
	//LOG(Daybreak::Vec3::Zero());
	//LOG(Daybreak::Vec4::One());
	//LOG(Daybreak::Vec4::Zero());
	return 0;
}