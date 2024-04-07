#pragma once

#include "../ECS/ECS.h"

#include <SDL.h>

struct CollideInfoStruct
{
	bool IsCollided;
	std::vector<Entity> CollidingEntities;
};

struct ColliderComponent
{
	int width;
	int height;
	CollideInfoStruct CollideInfo;
	glm::vec2 offset;

	ColliderComponent(int width = 0, int height = 0, glm::vec2 offset = glm::vec2(0))
	{
		this->height = height;
		this->width = width;
		this->offset = offset;
	}
};