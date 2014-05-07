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

#include "common/scummsys.h"
#include "common/config-manager.h"
#include "mads/mads.h"
#include "mads/game.h"
#include "mads/screen.h"
#include "mads/msurface.h"
#include "mads/phantom/game_phantom.h"
//#include "mads/nebular/dialogs_nebular.h"
//#include "mads/nebular/globals_nebular.h"
#include "mads/phantom/phantom_scenes.h"

namespace MADS {

namespace Phantom {

GamePhantom::GamePhantom(MADSEngine *vm): Game(vm) {
	_surface = new MSurface(MADS_SCREEN_WIDTH, MADS_SCENE_HEIGHT);
	_storyMode = STORYMODE_NAUGHTY;
}

ProtectionResult GamePhantom::checkCopyProtection() {
	/*
	// DEBUG: Flag copy protection failure
	_globals[5] = -1;
	
	if (!ConfMan.getBool("copy_protection"))
		return true;

	* DEBUG: Disabled for now
	CopyProtectionDialog *dlg = new CopyProtectionDialog(_vm, false);
	dlg->show();
	delete dlg;
	*/

	// DEBUG: Return that copy protection failed
	return PROTECTION_SUCCEED;
}

void GamePhantom::initialiseGlobals() {
	//int count, count2;
	//int bad;

	_globals.reset();
	//_globals[kTalkInanimateCount] = 8;

	/* Section #1 variables */
	// TODO

	/* Section #2 variables */
	// TODO

	/* Section #3 variables */
	// TODO

	/* Section #4 variables */
	// TODO

	/* Section #5 variables */
	// TODO

	/* Section #9 variables */
	// TODO

	_player._facing = FACING_NORTH;
	_player._turnToFacing = FACING_NORTH;

	//Player::preloadSequences("RXM", 1);
	//Player::preloadSequences("ROX", 1);
}

void GamePhantom::setSectionHandler() {
	delete _sectionHandler;

	switch (_sectionNumber) {
	case 1:
		_sectionHandler = new Section1Handler(_vm);
		break;
	case 2:
		_sectionHandler = new Section2Handler(_vm);
		break;
	case 3:
		_sectionHandler = new Section3Handler(_vm);
		break;
	case 4:
		_sectionHandler = new Section4Handler(_vm);
		break;
	case 5:
		_sectionHandler = new Section5Handler(_vm);
		break;
	default:
		break;
	}
}

void GamePhantom::checkShowDialog() {
	// TODO: Copied from Nebular
	if (_vm->_dialogs->_pendingDialog && _player._stepEnabled && !_globals[5]) {
		_player.releasePlayerSprites();
		_vm->_dialogs->showDialog();
		_vm->_dialogs->_pendingDialog = DIALOG_NONE;
	}
}

void GamePhantom::doObjectAction() {
	// TODO: Copied from Nebular
	//Scene &scene = _scene;
	MADSAction &action = _scene._action;
	//Dialogs &dialogs = *_vm->_dialogs;
	//int id;

	action._inProgress = false;
}

void GamePhantom::unhandledAction() {
	// TODO
}

void GamePhantom::step() {
	if (_player._visible && _player._stepEnabled && !_player._moving &&
		(_player._facing == _player._turnToFacing)) {

		// TODO
	}

}

void GamePhantom::synchronize(Common::Serializer &s, bool phase1) {
	Game::synchronize(s, phase1);

	// TODO: Copied from Nebular
	if (!phase1) {
		_globals.synchronize(s);
	}
}

} // End of namespace Phantom

} // End of namespace MADS