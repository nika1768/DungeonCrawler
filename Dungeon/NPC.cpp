#include "NPC.h"
#include "ResourceManager.h"
#include "Tilemap.h"

NPC::NPC(int x, int y) {
	tilepos.x = x;
	tilepos.y = y;
	ResourceManager::GetTilemap()->gameobjects[std::make_pair(tilepos.x, tilepos.y)] = this;
	tex = ResourceManager::LoadTexture("Graphics/ghost.png");
}

GameObject* NPC::getPtr()
{
	return nullptr;
}

SDL_Point NPC::getPosition()
{
	return tilepos;
}
