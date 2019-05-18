#include "GameObject.h"

void GameObject::setPosition(int y, int x) {
	
	auto* go = ResourceManager::GetTilemap();

	if (ResourceManager::GetTilemap()->object_map[tilepos.y][tilepos.x] == this) {
		ResourceManager::GetTilemap()->object_map[tilepos.y][tilepos.x] = nullptr;
	}
	tilepos.x = x;
	tilepos.y = y;
	ResourceManager::GetTilemap()->object_map[tilepos.y][tilepos.x] = this;
}

SDL_Point GameObject::getPosition() {
	return tilepos;
}
