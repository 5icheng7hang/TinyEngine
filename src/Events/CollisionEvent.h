#pragma once

#include "../ECS/ECS.h"
#include "../EventBus/Event.h"

class CollisionEvent : public Event
{
public:
	CollideInfoStruct CollideInfo;
	CollisionEvent(CollideInfoStruct info) : CollideInfo(info) {}
};