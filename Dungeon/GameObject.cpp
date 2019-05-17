#include "GameObject.h"

void GameObject::setPosition(int y, int x) {
	
	auto* go = ResourceManager::GetTilemap();

	if (ResourceManager::GetTilemap()->gameobjects[tilepos.y][tilepos.x] == this) {
		ResourceManager::GetTilemap()->gameobjects[tilepos.y][tilepos.x] = nullptr;
	}
	tilepos.x = x;
	tilepos.y = y;
	ResourceManager::GetTilemap()->gameobjects[tilepos.y][tilepos.x] = this;
}

SDL_Point GameObject::getPosition() {
	return tilepos;
}
