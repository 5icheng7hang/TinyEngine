#pragma once

#include "../ECS/ECS.h"
#include "../Components/ColliderComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"

class DamageSystem : public System
{
public:
	DamageSystem()
	{
		RequireComponent<ColliderComponent>();
	}

	void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus)
	{
		eventBus->SubscribeToEvent<DamageSystem, CollisionEvent>(this, &DamageSystem::onCollision);
	}

	void onCollision(CollisionEvent& event)
	{
		Logger::Log("fuck");
		for (auto it : event.CollideInfo.CollidingEntities)
		{
			it.Kill();
		}
	}

	void Update()
	{

	}
};