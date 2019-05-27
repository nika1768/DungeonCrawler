#pragma once
#include "libs.h"
#include "ResourceManager.h"

class Menu {

public:

	Menu(std::string message) {
		title_rect = ResourceManager::GetRectangle(ScreenWidth / 2 - 200, 200, 400, 100);
		message_rect = ResourceManager::GetRectangle(ScreenWidth / 2 - 300, 330, 600, 50);
		newgame_rect = ResourceManager::GetRectangle(ScreenWidth / 2 - 100, ScreenHeight / 2 - 10, 200, 100);
		quitgame_rect = ResourceManager::GetRectangle(ScreenWidth / 2 - 100, ScreenHeight / 2 + 100, 200, 100);
		button_tex = ResourceManager::LoadTexture("Graphics/button.png");
		button_hover_tex = ResourceManager::LoadTexture("Graphics/button_hover.png");
		buttons_click_tex = ResourceManager::LoadTexture("Graphics/button_click.png");
		newgame_tex = ResourceManager::LoadTextTexture("New Game", Color::Black);
		quitgame_tex = ResourceManager::LoadTextTexture("Quit Game", Color::Black);
		title_tex = ResourceManager::LoadTextTexture("Dungeon", Color::Black);
		message_tex = ResourceManager::LoadTextTexture(message, Color::Black);
	}
	~Menu() = default;

	void ResolveInput(SDL_Event& e) {

		// checks whether mouse is inside button rectangle
		if (IsMouseIn(newgame_rect)) {
			mouse_on_element = 0;
		}
		else if (IsMouseIn(quitgame_rect)) {
			mouse_on_element = 1;
		}
		else {
			mouse_on_element = -1;
			is_mouse_pressed = false;
		}

		if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT && mouse_on_element >= 0) {
			is_mouse_pressed = true;
		}
		else if (e.type == SDL_MOUSEBUTTONUP) {  
			// clicked
			if (is_mouse_pressed && mouse_on_element == 1) {
				SDL_Event ev;
				ev.type = SDL_QUIT;
				SDL_PushEvent(&ev);
			}
			else if (is_mouse_pressed && mouse_on_element == 0) {
				SDL_Event ev;
				ev.type = SDL_USEREVENT;
				ev.user.code = USER_NEW_GAME;
				SDL_PushEvent(&ev);
			}

			is_mouse_pressed = false;
		}
	}

	void OnRender() {
		
		// render the title of the game and a message underneath
		SDL_RenderCopy(ResourceManager::ren, title_tex, nullptr, &title_rect);
		SDL_RenderCopy(ResourceManager::ren, message_tex, nullptr, &message_rect);

		// if mouse is on the new game button, check whether LMB is pressed, render button text accordingly
		if (mouse_on_element == 0) {
			SDL_RenderCopy(ResourceManager::ren, (is_mouse_pressed ? buttons_click_tex : button_hover_tex ), nullptr, &newgame_rect);
			SDL_RenderCopy(ResourceManager::ren, button_tex, nullptr, &quitgame_rect);
		}
		// same, quit game button
		else if (mouse_on_element == 1) {
			SDL_RenderCopy(ResourceManager::ren, (is_mouse_pressed ? buttons_click_tex : button_hover_tex), nullptr, &quitgame_rect);
			SDL_RenderCopy(ResourceManager::ren, button_tex, nullptr, &newgame_rect);
		}
		else {
			SDL_RenderCopy(ResourceManager::ren, button_tex, nullptr, &newgame_rect);
			SDL_RenderCopy(ResourceManager::ren, button_tex, nullptr, &quitgame_rect);
		}

		// render text
		SDL_RenderCopy(ResourceManager::ren, newgame_tex, nullptr, &newgame_rect);
		SDL_RenderCopy(ResourceManager::ren, quitgame_tex, nullptr, &quitgame_rect);

	}

	bool IsMouseIn(SDL_Rect& rect) {

		int x, y;
		SDL_GetMouseState(&x, &y);
		if (rect.x <= x && rect.y <= y && x <= rect.x + rect.w && y <= rect.y + rect.h)
			return true;
		return false;
	}

private:

	int mouse_on_element = -1;
	bool is_mouse_pressed = false;

	SDL_Rect newgame_rect;
	SDL_Rect quitgame_rect;
	SDL_Rect title_rect;
	SDL_Rect message_rect;

	SDL_Texture* newgame_tex;
	SDL_Texture* quitgame_tex;
	SDL_Texture* button_tex;
	SDL_Texture* button_hover_tex;
	SDL_Texture* buttons_click_tex;
	SDL_Texture* title_tex;
	SDL_Texture* message_tex;
}