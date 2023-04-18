#pragma once

#include <map>
#include <string>
#include <SDL.h>

class AssetStore
{
private:
	std::map<std::string, SDL_Texture*> textures;
	// TODO: Fonts and Audios.

public:
	AssetStore();
	~AssetStore();

	void ClearAssets();
	void AddTexture(SDL_Renderer* renderer, const std::string& assetId, const std::string& filepath);
	SDL_Texture* GetTexture(const std::string& assetId);
};