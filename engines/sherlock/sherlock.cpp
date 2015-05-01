/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "sherlock/sherlock.h"
#include "sherlock/graphics.h"
#include "common/scummsys.h"
#include "common/config-manager.h"
#include "common/debug-channels.h"
#include "engines/util.h"

namespace Sherlock {

SherlockEngine::SherlockEngine(OSystem *syst, const SherlockGameDescription *gameDesc) :
		Engine(syst), _gameDescription(gameDesc), _randomSource("Sherlock") {
	_animation = nullptr;
	_debugger = nullptr;
	_events = nullptr;
	_inventory = nullptr;
	_journal = nullptr;
	_map = nullptr;
	_people = nullptr;
	_res = nullptr;
	_saves = nullptr;
	_scene = nullptr;
	_screen = nullptr;
	_sound = nullptr;
	_talk = nullptr;
	_ui = nullptr;
	_useEpilogue2 = false;
	_loadingSavedGame = false;
	_keyPadSpeed = 0;
	_loadGameSlot = -1;
	_canLoadSave = false;
}

SherlockEngine::~SherlockEngine() {
	delete _animation;
	delete _debugger;
	delete _events;
	delete _journal;
	delete _map;
	delete _people;
	delete _saves;
	delete _scene;
	delete _screen;
	delete _sound;
	delete _talk;
	delete _ui;
	delete _inventory;
	delete _res;
}

void SherlockEngine::initialize() {
	initGraphics(SHERLOCK_SCREEN_WIDTH, SHERLOCK_SCREEN_HEIGHT, false);

	DebugMan.addDebugChannel(kDebugScript, "scripts", "Script debug level");

	ImageFile::setVm(this);
	Object::setVm(this);
	Sprite::setVm(this);
	_res = new Resources();
	_animation = new Animation(this);
	_debugger = new Debugger(this);
	_events = new Events(this);
	_inventory = new Inventory(this);
	_map = new Map(this);
	_journal = new Journal(this);
	_people = new People(this);
	_saves = new SaveManager(this, _targetName);
	_scene = new Scene(this);
	_screen = new Screen(this);
	_sound = new Sound(this);
	_talk = new Talk(this);
	_ui = new UserInterface(this);
}

Common::Error SherlockEngine::run() {
	initialize();

	// If requested, load a savegame instead of showing the intro
	if (ConfMan.hasKey("save_slot")) {
		int saveSlot = ConfMan.getInt("save_slot");
		if (saveSlot >= 1 && saveSlot <= MAX_SAVEGAME_SLOTS)
			_loadGameSlot = saveSlot;
	}

	if (_loadGameSlot != -1) {
		_saves->loadGame(_loadGameSlot);
		_loadGameSlot = -1;
	} else {
		// Temporarily disabled for now
		//	showOpening();
	}

	while (!shouldQuit()) {
		// Prepare for scene, and handle any game-specific scenes. This allows 
		// for game specific cutscenes or mini-games that aren't standard scenes
		startScene();
		if (shouldQuit())
			break;

		// Reset UI flags
		_ui->reset();

		// Reset the active characters to initially just Sherlock
		_people->reset();

		// Initialize and load the scene. 
		_scene->selectScene();

		// Scene handling loop
		sceneLoop();
	}

	return Common::kNoError;
}

void SherlockEngine::sceneLoop() {
	while (!shouldQuit() && _scene->_goToScene == -1) {
		// See if a script needs to be completed from either a goto room code,
		// or a script that was interrupted by another script
		if (_talk->_scriptMoreFlag == 1 || _talk->_scriptMoreFlag == 3)
			_talk->talkTo(_talk->_scriptName);
		else
			_talk->_scriptMoreFlag = 0;

		// Handle any input from the keyboard or mouse
		handleInput();

		if (_people->_hSavedPos.x == -1) {
			_canLoadSave = true;
			_scene->doBgAnim();
			_canLoadSave = false;
		}
	}

	_scene->freeScene();
	_people->freeWalk();

}

/**
 * Handle all player input
 */
void SherlockEngine::handleInput() {
	_canLoadSave = true;
	_events->pollEventsAndWait();
	_canLoadSave = false;

	// See if a key or mouse button is pressed
	_events->setButtonState();

	_ui->handleInput();
}


/**
 * Read the state of a global flag
 */
bool SherlockEngine::readFlags(int flagNum) {
	bool value = _flags[ABS(flagNum)];
	if (flagNum < 0)
		value = !value;

	return value;
}

/**
 * Sets a global flag to either true or false depending on whether the specified
 * flag is positive or negative
 */
void SherlockEngine::setFlags(int flagNum) {
	_flags[ABS(flagNum)] = flagNum >= 0;

	_scene->checkSceneFlags(true);
}

void SherlockEngine::saveConfig() {
	// TODO
}

/**
 * Synchronize the data for a savegame
 */
void SherlockEngine::synchronize(Common::Serializer &s) {
	for (uint idx = 0; idx < _flags.size(); ++idx)
		s.syncAsByte(_flags[idx]);
}

/**
 * Returns true if a savegame can be loaded
 */
bool SherlockEngine::canLoadGameStateCurrently() {
	return _canLoadSave;
}

/**
 * Returns true if the game can be saved
 */
bool SherlockEngine::canSaveGameStateCurrently() {
	return _canLoadSave;
}

/**
 * Called by the GMM to load a savegame
 */
Common::Error SherlockEngine::loadGameState(int slot) {
	_saves->loadGame(slot);
	return Common::kNoError;
}

/**
 * Called by the GMM to save the game
 */
Common::Error SherlockEngine::saveGameState(int slot, const Common::String &desc) {
	_saves->saveGame(slot, desc);
	return Common::kNoError;
}

} // End of namespace Comet