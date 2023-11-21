#pragma once

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/ColliderComponent.h"

#include <SDL.h>

class CollisionDebugSystem: public System 
{
public:
	CollisionDebugSystem()
	{
		RequireComponent<TransformComponent>();
		RequireComponent<ColliderComponent>();
	}

	void Update(SDL_Renderer* renderer)
	{
		for (auto entity: GetSystemEntities())
		{
			const auto transfrom = entity.GetComponent<TransformComponent>();
			const auto collider = entity.GetComponent<ColliderComponent>();

			SDL_Rect colliderRect = {
				static_cast<int>(transfrom.position.x + collider.offset.x),
				static_cast<int>(transfrom.position.y + collider.offset.y),
				static_cast<int>(collider.width),
				static_cast<int>(collider.height),
			};
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			SDL_RenderDrawRect(renderer, &colliderRect);
		}
	}
};