//!
//! InputMgr.cpp
//! Manages user input events
//! 
#include "InputMgr.h"



namespace InputMgr {

	//! Constructor
	//! 
	InputMgr::InputMgr(Player::Player* player, World::World* world)
		: player(player)
		, world(world)
	{}


	//! HandleEvent
	//! Handles the provided SDL event
	//! 
	void InputMgr::HandleEvent(const SDL_Event& event) {
		// TODO
		if (event.type != SDL_EVENT_KEY_DOWN) {
			return;
		}

		//! Get action type
		auto iter = actionMappings.find(event.key.key);
		if (iter == actionMappings.end()) {
			//! Action does not exist
			return;
		}

		const InputMgr::INPUT_ACTION& action = iter->second;

		//! Handle action
		switch (action) {
		case InputMgr::INPUT_ACTION::MOVE_FORWARD:
			player->GetCamera()->GetPosition().z += player->GetMovementSpeed();
			break;

		case InputMgr::INPUT_ACTION::MOVE_BACKWARD:
			player->GetCamera()->GetPosition().z -= player->GetMovementSpeed();
			break;

		case InputMgr::INPUT_ACTION::MOVE_RIGHT:
			player->GetCamera()->GetPosition().x += player->GetMovementSpeed();
			break;

		case InputMgr::INPUT_ACTION::MOVE_LEFT:
			player->GetCamera()->GetPosition().x -= player->GetMovementSpeed();
			break;

		case InputMgr::INPUT_ACTION::MOVE_UP:
			player->GetCamera()->GetPosition().y += player->GetMovementSpeed();
			break;

		case InputMgr::INPUT_ACTION::MOVE_DOWN:
			player->GetCamera()->GetPosition().y -= player->GetMovementSpeed();
			break;

		default:
			LOG_ERROR("InputMgr: Unhandled action found in mapped action list");
			break;
		}
	}

}; // namespace InputMgr