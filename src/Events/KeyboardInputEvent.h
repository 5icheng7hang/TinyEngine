#pragma once

#include "../ECS/ECS.h"
#include "../EventBus/Event.h"

enum class KeyInputType
{
	PRESSED, // begin press
	// DOWN,	// during press
	RELEASED // end press
	// CLICKED // release in very quick 
};


class KeyboardInputEvent : public Event
{
public:
	int Key;
	KeyInputType InputType;
	KeyboardInputEvent(int key, KeyInputType type) : Key(key), InputType(type) {}
};