#pragma once

#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyboardInputEvent.h"


class InputSystem : public System
{
private:
	std::vector<int> KeyboardStat;

public:
	InputSystem() = default;
	
	void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus)
	{
		eventBus->SubscribeToEvent<InputSystem, KeyboardInputEvent>(this, &InputSystem::onKeyBoardInput);
	}

	void onKeyBoardInput(KeyboardInputEvent& e)
	{
		switch (e.InputType)
		{
		case KeyInputType::PRESSED:
			if (std::find(KeyboardStat.begin(), KeyboardStat.end(), e.Key) == KeyboardStat.end())
			{
				// this key haven't been pressed.
				Logger::Log(std::to_string(e.Key) + " pressed! ");
				KeyboardStat.push_back(e.Key);
			}
			break;
		case KeyInputType::RELEASED:
			if (std::find(KeyboardStat.begin(), KeyboardStat.end(), e.Key) != KeyboardStat.end())
			{
				// this key has been pressed.
				Logger::Log(std::to_string(e.Key) + " released! ");
				KeyboardStat.erase(std::find(KeyboardStat.begin(), KeyboardStat.end(), e.Key));
			}
			break;

		default:
			break;
		}
	}

};