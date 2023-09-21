#pragma once

#include "../ECS/ECS.h"
#include "../Components/ColliderComponent.h"
#include "../Components/TransformComponent.h"
#include <SDL.h>

class CollisionSystem : public System
{
public:
	CollisionSystem()
	{
		RequireComponent<ColliderComponent>();
		RequireComponent<TransformComponent>();
	}

	void Update() 
	{
		auto entities = GetSystemEntities();
		for (auto a = entities.begin(); a != entities.end(); a++)
		{
			for (auto b = a; b != entities.end(); b++)
			{
				if (*a == *b)
				{
					continue;
				}

				bool Collided = CheckAABBCollide(
					a->GetComponent<TransformComponent>().position + a->GetComponent<ColliderComponent>().offset,
					glm::vec2(a->GetComponent<ColliderComponent>().width, a->GetComponent<ColliderComponent>().height),
					b->GetComponent<TransformComponent>().position + b->GetComponent<ColliderComponent>().offset,
					glm::vec2(b->GetComponent<ColliderComponent>().width, b->GetComponent<ColliderComponent>().height)
				);

				if (Collided)
				{
					Logger::Log("Entity: " + std::to_string(a->GetId()) + " collided with Entity: " + std::to_string(b->GetId()));
				}
			}
		}
	}

	bool CheckAABBCollide(glm::vec2 aPos, glm::vec2 aExt, glm::vec2 bPos, glm::vec2 bExt)
	{
		return (
			aPos.x				<	 bPos.x + bExt.x &&
			aPos.x + aExt.x		>	 bPos.x &&
			aPos.y				<	 bPos.y + bExt.y &&
			aPos.y + aExt.y		>	 bPos.y 
			);
	}
};