#pragma once

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../AssetStore/Assetstore.h"

#include <algorithm>
#include <SDL.h>

class RenderSystem : public System 
{
public:
	RenderSystem() 
	{
		RequireComponent<TransformComponent>();
		RequireComponent<SpriteComponent>();
	}

	void Update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore) 
	{
		// TODO: sort by z index.
		auto entitiesToBeRendered = GetSystemEntities();
		std::sort(entitiesToBeRendered.begin(),
			entitiesToBeRendered.end(),
			[](const Entity& a, const Entity& b) {
				return a.GetComponent<SpriteComponent>().zIndex < b.GetComponent<SpriteComponent>().zIndex; });
		// Loop all entities that the system is interested in
		for (auto entity : entitiesToBeRendered) 
		{
			// TODO: Update entity position based on its velocity
			const auto transform = entity.GetComponent<TransformComponent>();
			const auto sprite = entity.GetComponent<SpriteComponent>();

			// Set source and destnation rect
			SDL_Rect srcRect = sprite.srcRect;
			SDL_Rect dstRect = {
				static_cast<int>(transform.position.x),
				static_cast<int>(transform.position.y),
				static_cast<int>(sprite.width * transform.scale.x),
				static_cast<int>(sprite.height * transform.scale.y)
			};

			SDL_RenderCopyEx(
				renderer,
				assetStore->GetTexture(sprite.assetId),
				&srcRect,
				&dstRect,
				transform.rotation,
				NULL,
				/* not do flip here. */
				SDL_FLIP_NONE);
		}
	}
};
