//!
//! InputMgr.cpp
//! Manages user input events
//! 
#include "InputMgr.h"



namespace InputMgr {

	//! Constructor
	//! 
	InputMgr::InputMgr(std::shared_ptr<Player::Player> player, std::shared_ptr<World::World> world)
		: player(player)
		, world(world)
	{}


	//! HandleEvent
	//! Handles the provided SDL event and updates relevant activity states
	//! 
	void InputMgr::HandleEvent(const SDL_Event& event) {
		//! Find action mapping
		auto iterAction = actionMappings.find(event.key.key);
		if (iterAction == actionMappings.end()) {
			//! Action does not exist
			return;
		}
		InputMgr::INPUT_ACTION action = iterAction->second;

		//! Set key activity
		switch (event.type) {
		case SDL_EVENT_KEY_DOWN:
			activeActions.insert(action);
			break;

		case SDL_EVENT_KEY_UP:
			activeActions.erase(action);
			break;
		}
	}

	//! ProcessActivityState
	//! Processes all current activity states
	//! 
	void InputMgr::ProcessActivityState() {
		//! Process each action
		for (InputMgr::INPUT_ACTION action : activeActions) {
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
				Util::Log::Error("InputMgr: Unhandled action found in mapped action list");
				break;
			}
		}		
	}

}; // namespace InputMgr