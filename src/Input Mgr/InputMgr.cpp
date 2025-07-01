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
		, isWindowFocused(false)
	{}


	//! HandleEvent
	//! Handles the provided SDL event and updates relevant activity states
	//! 
	void InputMgr::HandleEvent(const SDL_Event& event) {
		//! Update clamps/wraps
		Util::Rotation& rotation = player->GetCamera()->GetRotation();
		rotation.roll = Util::Wrap(rotation.roll, 0, 2 * Util::PI);
		rotation.pitch = Util::Clamp(rotation.pitch, -Util::PI/2, Util::PI/2);
		rotation.yaw = Util::Wrap(rotation.yaw, 0, 2 * Util::PI);

		//! Handle non-key mappings
		switch (event.type) {
		case SDL_EVENT_WINDOW_FOCUS_GAINED:
			isWindowFocused = true;
			return;

		case SDL_EVENT_WINDOW_FOCUS_LOST:
			isWindowFocused = false;
			return;

		case SDL_EVENT_MOUSE_MOTION:
			if (!isWindowFocused) {
				return;
			}

			Util::Rotation& rotation = player->GetCamera()->GetRotation();
			rotation.yaw = Util::Wrap(rotation.yaw - event.motion.xrel * mouseSensitivity, 0, 2*Util::PI);
			rotation.pitch = Util::Clamp(rotation.pitch - event.motion.yrel * mouseSensitivity, -Util::PI/2, Util::PI/2);
			return;
		}

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
		/* ----------------------------------------------------------------
		* Movement
		* ---------------------------------------------------------------- */
		int dForward = activeActions.count(INPUT_ACTION::MOVE_FORWARD) - activeActions.count(INPUT_ACTION::MOVE_BACKWARD);
		int dRight = activeActions.count(INPUT_ACTION::MOVE_RIGHT) - activeActions.count(INPUT_ACTION::MOVE_LEFT);
		int dUp = activeActions.count(INPUT_ACTION::MOVE_UP) - activeActions.count(INPUT_ACTION::MOVE_DOWN);

		Util::Vector3<double>& position = player->GetCamera()->GetPosition();
		const Player::Camera::FRUVector fruVector = player->GetCamera()->GetFRUVector();

		double moveSpeed = player->GetMovementSpeed();
		position += (dForward * fruVector.forward * moveSpeed);
		position += (dRight * fruVector.right * moveSpeed);
		position += (dUp * fruVector.up * moveSpeed);

	
		/* ----------------------------------------------------------------
		* Camera Rotations
		* ---------------------------------------------------------------- */
		int dRoll = activeActions.count(INPUT_ACTION::ROLL_LEFT) - activeActions.count(INPUT_ACTION::ROLL_RIGHT);

		Util::Rotation& rotation = player->GetCamera()->GetRotation();
		double rotSpeed = .1;	// TODO: Configurable
		rotation.roll += dRoll * rotSpeed;
	}

}; // namespace InputMgr