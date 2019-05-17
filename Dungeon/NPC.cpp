#include "NPC.h"
#include "ResourceManager.h"
#include "Tilemap.h"
#include "Hero.h"

NPC::NPC() {
	
	tex = ResourceManager::LoadTexture("Graphics/ghost.png");

}

GameObject* NPC::getPtr()
{
	return nullptr;
}

int NPC::getMaxLife() {
	return max_life;
}

int NPC::getCurLife() {
	return cur_life;
}

void NPC::setLife(int value) {
	setMaxLife(value);
	setCurLife(value);
}

void NPC::setMaxLife(int value) {
	max_life = value;
}

void NPC::setCurLife(int value) {
	cur_life = value;
}

int NPC::getAttack() {
	return attack;
}

void NPC::setAttack(int value) {
	attack = value;
}

void NPC::Attack() {

	ResourceManager::GetHero()->Defend(attack);
}

void NPC::Defend(int amount) {

	std::cout << "Defending" << std::endl;
	cur_life = std::max(0, cur_life - amount);
	if (cur_life == 0) {
		std::cout << "Dying" << std::endl;
		// TODO
		ResourceManager::GetTilemap()->gameobjects[tilepos.y][tilepos.x] = nullptr;
		ResourceManager::DestroyGameObject(this);
	}

}

void NPC::OnTurn()
{
	SDL_Point dst = tilepos;
	int rnd = std::rand() % 4;

	if (rnd == 0) {
		dst.x++;
	}
	else if (rnd == 1) {
		dst.x--;
	}
	else if (rnd == 2) {
		dst.y++;
	}
	else {
		dst.y--;
	}

	if (tilepos.x != dst.x && tilepos.y != dst.y) {
		std::cout << "MOVE" << std::endl;
	}

	if (ResourceManager::GetTilemap()->CanMove(this, tilepos, dst)) {
		tilepos = dst;
	}
}

bool NPC::CanAttack() { return true; };