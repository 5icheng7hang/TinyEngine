#include "Assetstore.h"
#include "../Logger/Logger.h"
#include <SDL_image.h>

AssetStore::AssetStore()
{
	Logger::Log("AssetStore Ctor called!!");
}

AssetStore::~AssetStore()
{
	ClearAssets();
	Logger::Log("AssetStore Dtor called!!");
}

void AssetStore::ClearAssets()
{
	for (auto texture: textures)
	{
		SDL_DestroyTexture(texture.second);
	}
	textures.clear();
}

void AssetStore::AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& filepath)
{
	auto surface = IMG_Load(filepath.c_str());

	if (!surface)
	{
		Logger::Err("couldn't load Texture id: " + assetId);
	}

	auto texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	// Add texture to map.
	textures.emplace(assetId, texture);

	Logger::Log("New texture added with id " + assetId);
}

SDL_Texture* AssetStore::GetTexture(const std::string& assetId)
{
	return textures[assetId];
}

