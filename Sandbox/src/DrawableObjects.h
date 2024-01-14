#pragma once

#include <Daybreak.h>

struct DrawableObject
{
	Daybreak::Ref<Daybreak::Texture2D> texture;
	glm::vec3 position;
	glm::vec2 size;
	std::string name;

	bool operator<(const DrawableObject& c2)
	{
		return position.z < c2.position.z;
	}
	//bool operator() (DrawableObject o1, DrawableObject o2) { return (o1.position.z < o2.position.z); }
};