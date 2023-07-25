#pragma once

#include "../ECS/ECS.h"
#include "../Components/AnimationComponent.h"
#include "../Components/SpriteComponent.h"

#include <SDL.h>

class AnimationSystem : public System
{
public:
	AnimationSystem()
	{
		RequireComponent<SpriteComponent>();
		RequireComponent<AnimationComponent>();
	}


	void Update()
	{
		for (auto entity : GetSystemEntities())
		{
			auto& animation = entity.GetComponent<AnimationComponent>();
			auto& sprite = entity.GetComponent<SpriteComponent>();

			// TODO: change frame, change source rect of sprite
			animation.currentFrame = ((SDL_GetTicks() - animation.startTime) * animation.frameSpeedRate / 1000) % animation.numFrames;
			sprite.srcRect.x = animation.currentFrame * sprite.width;
		}
	}




};