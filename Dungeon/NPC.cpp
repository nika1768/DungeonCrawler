#include "NPC.h"
#include "ResourceManager.h"
#include "Tilemap.h"

NPC::NPC() {
	
	tex = ResourceManager::LoadTexture("Graphics/ghost.png");
}

GameObject* NPC::getPtr()
{
	return nullptr;
}

void NPC::OnTurn()
{
	SDL_Point dst = tilepos;
	int rnd = std::rand() % 4;

	if (rnd == 0) {
		dst.x++;
		if (ResourceManager::GetTilemap()->CanMove(this, tilepos, dst)) {
			tilepos = dst;
			return;
		}
		dst.x--;
	}
	else if (rnd == 1) {
		dst.x--;
		if (ResourceManager::GetTilemap()->CanMove(this, tilepos, dst)) {
			tilepos = dst;
			return;
		}
		dst.x++;
	}
	else if (rnd == 3) {
		dst.y++;
		if (ResourceManager::GetTilemap()->CanMove(this, tilepos, dst)) {
			tilepos = dst;
			return;
		}
		dst.y--;
	}
	else {
		dst.y--;
		if (ResourceManager::GetTilemap()->CanMove(this, tilepos, dst)) {
			tilepos = dst;
			return;
		}
		dst.y++;
	}
}