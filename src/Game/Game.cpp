#include "Game.h"
#include "../Logger/Logger.h"
#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Systems/MovementSystem.h"
#include "../Systems/RenderSystem.h"
#include "../AssetStore/Assetstore.h"

#include <SDL.h>
#include <SDL_image.h>
#include <glm/glm.hpp>
#include <iostream>

Game::Game() 
{
    isRunning = false;
    registry = std::make_unique<Registry>();
    assetStore = std::make_unique<AssetStore>();

    Logger::Log("Game constructor called!");
}

Game::~Game() 
{
    Logger::Log("Game destructor called!");   
}

void Game::Initialize() 
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) 
    {
        Logger::Err("Error initializing SDL.");
        return;
    }
    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);
    windowWidth = displayMode.w;
    windowHeight = displayMode.h;
    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowWidth,
        windowHeight,
        SDL_WINDOW_BORDERLESS
    );
    if (!window) 
    {
        Logger::Err("Error creating SDL window.");
        return;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) 
    {
        Logger::Err("Error creating SDL renderer.");
        return;
    }
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    isRunning = true;
}

void Game::ProcessInput() 
{
    SDL_Event sdlEvent;
    while (SDL_PollEvent(&sdlEvent)) 
    {
        switch (sdlEvent.type) {
            case SDL_QUIT:
                isRunning = false;
                break;
            case SDL_KEYDOWN:
                if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) 
                {
                    isRunning = false;
                }
                break;
        }
    }
}

void Game::Setup() 
{
    // Add system.
    registry->AddSystem<MovementSystem>();
    registry->AddSystem<RenderSystem>();

    // Add assets to asset store
	assetStore->AddTexture(renderer, "tank-image", "./assets/images/tank-panther-right.png");
	assetStore->AddTexture(renderer, "truck-image", "./assets/images/truck-ford-right.png");

    // Create some entities
	Entity tank = registry->CreateEntity();
    Entity truck = registry->CreateEntity();

    // Add some components
    tank.AddComponent<TransformComponent>(glm::vec2(10.f, 30.f), glm::vec2(10.f, 10.f), 45.f);
    tank.AddComponent<RigidBodyComponent>(glm::vec2(50.f, 0.f));
    tank.AddComponent<SpriteComponent>("tank-image", 32, 32);


	truck.AddComponent<TransformComponent>(glm::vec2(30.f, 10.f), glm::vec2(10.f, 10.f), 90.f);
	truck.AddComponent<RigidBodyComponent>(glm::vec2(0.f, 30.f));
	truck.AddComponent<SpriteComponent>("truck-image", 32, 32);
}

void Game::Update() 
{
    // If we are too fast, waste some time until we reach the MILLISECS_PER_FRAME
    int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - millisecsPreviousFrame);
    if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME) 
    {
        SDL_Delay(timeToWait);
    }

    // The difference in ticks since the last frame, converted to seconds
    double deltaTime = (SDL_GetTicks() - millisecsPreviousFrame) / 1000.0;

    // Store the "previous" frame time
    millisecsPreviousFrame = SDL_GetTicks();

    // update registry to process entities waiting to be created or deleted
    registry->Update();


	// TODO: Update all system.
	registry->GetSystem<MovementSystem>().Update(deltaTime);
}

void Game::Render() 
{
    SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
    SDL_RenderClear(renderer);

    // TODO: Render game objects...
	registry->GetSystem<RenderSystem>().Update(renderer, assetStore);

    SDL_RenderPresent(renderer);
}

void Game::Run() 
{
    Setup();
    while (isRunning) {
        ProcessInput();
        Update();
        Render();
    }
}

void Game::Destroy() 
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
