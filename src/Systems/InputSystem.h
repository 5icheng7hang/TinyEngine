#pragma once

#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyboardInputEvent.h"
#include "../Components/InputControlComponent.h"


class InputSystem : public System
{
private:
	std::vector<int> KeyboardStat;

public:
	InputSystem() {
		RequireComponent<InputControlComponent>();
		RequireComponent<SpriteComponent>();
		RequireComponent<RigidBodyComponent>();
	}
	
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

				for (auto entity : GetSystemEntities())
				{
					const auto InputControlComp = entity.GetComponent<InputControlComponent>();
					auto& SpriteComp = entity.GetComponent<SpriteComponent>();
					auto& RigidBodyComp = entity.GetComponent<RigidBodyComponent>();

					switch (e.Key)
					{
					case SDLK_UP:
						RigidBodyComp.velocity = InputControlComp.upVelocity;
						SpriteComp.srcRect.y = SpriteComp.height * 0;
						break;
					case SDLK_RIGHT:
						RigidBodyComp.velocity = InputControlComp.rightVelocity;
						SpriteComp.srcRect.y = SpriteComp.height * 1;
						break;
					case SDLK_DOWN:
						RigidBodyComp.velocity = InputControlComp.downVelocity;
						SpriteComp.srcRect.y = SpriteComp.height * 2;
						break;
					case SDLK_LEFT:
						RigidBodyComp.velocity = InputControlComp.leftVelocity;
						SpriteComp.srcRect.y = SpriteComp.height * 3;
						break;
					}
				}

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