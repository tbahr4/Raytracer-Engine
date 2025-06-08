//!
//! InputMgr.h
//! Manages user input events
//! 
#pragma once

#include <SDL3/SDL.h>
#include <unordered_map>
#include <unordered_set>
#include "Util.h"
#include "Player.h"
#include "World.h"



namespace InputMgr {

	class InputMgr {
	private:
		//! Actions
		enum class INPUT_ACTION {
			MOVE_FORWARD,
			MOVE_BACKWARD,
			MOVE_RIGHT,
			MOVE_LEFT,
			MOVE_UP,
			MOVE_DOWN
		};

		//! Keybinds
		const std::unordered_map<SDL_Keycode, INPUT_ACTION> actionMappings = {
			{SDLK_W, INPUT_ACTION::MOVE_FORWARD},
			{SDLK_S, INPUT_ACTION::MOVE_BACKWARD},
			{SDLK_D, INPUT_ACTION::MOVE_RIGHT},
			{SDLK_A, INPUT_ACTION::MOVE_LEFT},
			{SDLK_SPACE, INPUT_ACTION::MOVE_UP},
			{SDLK_LSHIFT, INPUT_ACTION::MOVE_DOWN}
		};

	private:
		Player::Player* player;
		World::World* world;

		std::unordered_set<INPUT_ACTION> activeActions;	// Active keypresses/actions

	public:
		InputMgr(Player::Player* player, World::World* world);

		void HandleEvent(const SDL_Event& event);
		void ProcessActivityState();
	};

}; // namespace InputMgr