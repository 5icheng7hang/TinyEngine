#include "Game.h"
#include "../Logger/Logger.h"
#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/AnimationComponent.h"
#include "../Components/ColliderComponent.h"
#include "../Systems/MovementSystem.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/DamageSystem.h"
#include "../Systems/CollisionDebugSystem.h"
#include "../Systems/InputSystem.h"
#include "../AssetStore/Assetstore.h"

#include <SDL.h>
#include <SDL_image.h>
#include <glm/glm.hpp>
#include <iostream>
#include <fstream>

Game::Game() 
{
    isRunning = false;
    
    registry = std::make_unique<Registry>();
    assetStore = std::make_unique<AssetStore>();
    eventBus = std::make_unique<EventBus>();

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
    windowWidth = 1024;  //displayMode.w;
    windowHeight = 768; // displayMode.h;
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
    // SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
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
				if (sdlEvent.key.keysym.sym == SDLK_d)
				{
				    isDebug = !isDebug;
				}
                eventBus->EmitEvent<KeyboardInputEvent>(static_cast<int>(sdlEvent.key.keysym.sym), KeyInputType::PRESSED);
                break;

            case SDL_KEYUP:
                eventBus->EmitEvent<KeyboardInputEvent>(static_cast<int>(sdlEvent.key.keysym.sym), KeyInputType::RELEASED);
                break;
        }
    }
}

void Game::LoadLevel(int LevelId)
{
	// Add system.
	registry->AddSystem<MovementSystem>();
	registry->AddSystem<RenderSystem>();
    registry->AddSystem<AnimationSystem>();
    registry->AddSystem<CollisionSystem>();
    registry->AddSystem<CollisionDebugSystem>();
    registry->AddSystem<DamageSystem>();
    registry->AddSystem<InputSystem>();

	// Add assets to asset store
	assetStore->AddTexture(renderer, "tank-image", "./assets/images/tank-panther-right.png");
	assetStore->AddTexture(renderer, "truck-image", "./assets/images/truck-ford-right.png");
    assetStore->AddTexture(renderer, "chopper-image", "./assets/images/chopper.png");
	assetStore->AddTexture(renderer, "radar-image", "./assets/images/radar.png");


    // TODO: Load tilemap from tilemap png file
    //       and a .map csv file.
    // read in .map file, iterate/parse the file and generate
    // a entity with spriteComponent with src and dst rect
    // maybe store those entities in a vector to maintain?

    ///////////////////////////////////////////////////////////
    assetStore->AddTexture(renderer, "tilemap-image", "./assets/tilemaps/jungle.png");
    int tileSize = 32;
    double tileScale = 1.0;
    int mapNumCols = 25;
    int mapNumRows = 20;

    std::fstream mapFile;
    mapFile.open("./assets/tilemaps/jungle.map");
    for (int y = 0; y < mapNumRows; y++)
    {
        for (int x = 0; x < mapNumCols; x++)
        {
            char ch;
            mapFile.get(ch);
            int srcRectY = std::atoi(&ch) * tileSize;
            mapFile.get(ch);
            int srcRectX = std::atoi(&ch) * tileSize;
            mapFile.ignore();

            Entity tile = registry->CreateEntity();
            tile.AddComponent<TransformComponent>(glm::vec2(x * (tileScale * tileSize), y * (tileScale * tileSize)), glm::vec2(tileScale, tileScale), 0.0);
            tile.AddComponent<SpriteComponent>("tilemap-image", tileSize, tileSize, 2, srcRectX, srcRectY);
            
            //Logger::Err("current x = " + std::to_string(x) + ", y = " + std::to_string(y) + ", srcRextx = " + std::to_string(srcRectX) + ", srcRecty = " + std::to_string(srcRectY));
        }
    }
    mapFile.close();

	///////////////////////////////////////////////////////////




    Entity chopper = registry->CreateEntity();
    Entity chopper2 = registry->CreateEntity();
    // Entity CollisionArea = registry->CreateEntity();
    Entity radar = registry->CreateEntity();

    // init components.
    chopper.AddComponent<TransformComponent>(glm::vec2(10.f, 30.f), glm::vec2(3.f, 3.f), 0.f);
    chopper.AddComponent<RigidBodyComponent>(glm::vec2(20.f, 0.f));
    chopper.AddComponent<SpriteComponent>("chopper-image", 32, 32, 5);
    chopper.AddComponent<AnimationComponent>(2, 5,  true);
    chopper.AddComponent<ColliderComponent>(10, 10);

    chopper2.AddComponent<TransformComponent>(glm::vec2(500.f, 30.f), glm::vec2(3.f, 3.f), 0.f);
    chopper2.AddComponent<RigidBodyComponent>(glm::vec2(-20.f, 0.f));
    chopper2.AddComponent<SpriteComponent>("chopper-image", 32, 32, 5);
    chopper2.AddComponent<AnimationComponent>(2, 5, true);
    chopper2.AddComponent<ColliderComponent>(10, 10);

    //CollisionArea.AddComponent<TransformComponent>(glm::vec2(200.f, 30.f), glm::vec2(20.f, 20.f), 0.f);
    //CollisionArea.AddComponent<ColliderComponent>(200, 20);

	radar.AddComponent<TransformComponent>(glm::vec2(500, 300.f), glm::vec2(1.f, 1.f));
	radar.AddComponent<RigidBodyComponent>(glm::vec2(0.f, 0.f));
	radar.AddComponent<SpriteComponent>("radar-image", 64, 64, 5);
	radar.AddComponent<AnimationComponent>(8, 5, true);


    // chopper.Kill();
}


void Game::Setup() 
{
    LoadLevel(1);
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



    // subscription
    eventBus->Reset();

    registry->GetSystem<DamageSystem>().SubscribeToEvents(eventBus);
    registry->GetSystem<InputSystem>().SubscribeToEvents(eventBus);

    // update registry to process entities waiting to be created or deleted
    registry->Update();


	// TODO: Update all system.
	registry->GetSystem<MovementSystem>().Update(deltaTime);
    registry->GetSystem<AnimationSystem>().Update();
    registry->GetSystem<CollisionSystem>().Update(eventBus);
}

void Game::Render() 
{
    SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
    SDL_RenderClear(renderer);

    // TODO: Render game objects...
	registry->GetSystem<RenderSystem>().Update(renderer, assetStore);
    if (isDebug)
    {
        registry->GetSystem<CollisionDebugSystem>().Update(renderer);
    }
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
