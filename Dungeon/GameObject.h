#pragma once
#include "libs.h"
#include "ResourceManager.h"
#include "Tilemap.h"

class GameObject {
public:

	~GameObject() = default;
	
	virtual void setPosition(int x, int y);
	SDL_Point getPosition();
	
	virtual void OnTurn() {};
	virtual SDL_Texture* getTexture() { return tex; };
	virtual bool CanAttack() { return false; };

	virtual std::string toString() { return "GameObject"; }
	
	SDL_Texture* tex;
	SDL_Point tilepos;
};