#pragma once
#include "libs.h"

const int TileSize = 32;
static int ratio = 2;
const int DefaultRenderTileSize = 64;
static int RenderTileSize = 64;
const int ScreenWidth = 1600;
const int ScreenHeight = 800;
const int HeroLevel = 1;
const int AwarenessDistance = 4;
const int font_size = 13;
const int ENEMY_AWARENESS = 6;
const int ENEMY_INITIATIVE = 2;
const int PLAYER_SIGHT = 5;
const int ENEMY_CHANCE = 20;
const int POTION_CHANCE = 30;

const std::string font_path = "Resources/calibri.ttf";

enum Color {Black};

// MAP CONSTS
const int TILE_FLOOR = 0;
const int TILE_WALL = 1;
const int TILE_DOOR_CLOSED_LR = 2;
const int TILE_DOOR_OPENED_LR = 3;
const int TILE_DOOR_CLOSED_TB = 4;
const int TILE_DOOR_OPENED_TB = 5;
const int TILE_SMALL_POTION = 6;
const int TILE_BIG_POTION = 7;
const int TILE_FLOOR_A = 50;
const int TILE_FLOOR_B = 51;
const int TILE_FLOOR_C = 52;
const int TILE_FLOOR_D = 53;

static SDL_Rect GetTileRect(int code) {
	SDL_Rect rect;
	rect.h = 16;
	rect.w = 16;
	rect.x = 120;
	rect.y = 64;

	if (code == TILE_WALL) {
		//rect.x = 16 + 16 * std::rand() % 4;
		rect.x = 16;
		rect.y = 0;
	}
	else if (code == TILE_FLOOR) {
		rect.x = 96;
		rect.y = 0;
		//rect.x = 96 + 16 * std::rand() % 4;
		//rect.y = 0 + 16 * std::rand() % 3;
	}
	else if (code == TILE_DOOR_CLOSED_LR || code == TILE_DOOR_OPENED_TB) {
		// 7 5
		rect.x = 112;
		rect.y = 64;
	}
	else if (code == TILE_DOOR_CLOSED_TB || code == TILE_DOOR_OPENED_LR) {
		rect.x = 112;
		rect.y = 48;
	}
	else if (code == TILE_BIG_POTION) {
		rect.x = 144;
		rect.y = 128;
	}
	else if (code == TILE_SMALL_POTION) {
		rect.x = 128;
		rect.y = 144;
	}
	return rect;
}
