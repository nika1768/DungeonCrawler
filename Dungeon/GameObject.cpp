#include "GameObject.h"

void GameObject::setPosition(int x, int y) {
	
	if (ResourceManager::GetTilemap()->gameobjects[std::make_pair(tilepos.x, tilepos.y)] == this) {
		ResourceManager::GetTilemap()->gameobjects[std::make_pair(tilepos.x, tilepos.y)] = nullptr;
	}
	tilepos.x = x;
	tilepos.y = y;
	ResourceManager::GetTilemap()->gameobjects[std::make_pair(tilepos.x, tilepos.y)] = this;
}

SDL_Point GameObject::getPosition() {
	return tilepos;
}
