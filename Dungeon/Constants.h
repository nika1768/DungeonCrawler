#pragma once
#include "libs.h"

const int TileSize = 32;
static int ratio = 2;
const int DefaultRenderTileSize = 64;
static int RenderTileSize = 64;
const int ScreenWidth = 1600;
const int ScreenHeight = 800;
const int HeroLevel = 1;
const int font_size = 13;
const std::string font_path = "Resources/calibri.ttf";
enum Color {Black};

static SDL_Rect GetTileRect(int code) {
	SDL_Rect rect;
	rect.h = 16;
	rect.w = 16;
	rect.x = 120;
	rect.y = 64;

	if (code == 1) {
		//rect.x = 16 + 16 * std::rand() % 4;
		rect.x = 16;
		rect.y = 0;
	}
	else if (code == 0 || code == 3) {
		rect.x = 96;
		rect.y = 0;
		//rect.x = 96 + 16 * std::rand() % 4;
		//rect.y = 0 + 16 * std::rand() % 3;
	}
	else if (code == 2) {
		rect.x = 112;
		rect.y = 48;
	}
	/*/
	else if (code == 3) {
		rect.x = 128;
		rect.y = 64;
	}
	*/
	return rect;
}
