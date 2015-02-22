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
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "xeen/interface.h"
#include "xeen/dialogs_automap.h"
#include "xeen/dialogs_char_info.h"
#include "xeen/dialogs_control_panel.h"
#include "xeen/dialogs_error.h"
#include "xeen/dialogs_fight_options.h"
#include "xeen/dialogs_info.h"
#include "xeen/dialogs_items.h"
#include "xeen/dialogs_query.h"
#include "xeen/dialogs_quests.h"
#include "xeen/dialogs_quick_ref.h"
#include "xeen/dialogs_spells.h"
#include "xeen/resources.h"
#include "xeen/xeen.h"

#include "xeen/dialogs_party.h"

namespace Xeen {

PartyDrawer::PartyDrawer(XeenEngine *vm): _vm(vm) {
	_restoreSprites.load("restorex.icn");
	_hpSprites.load("hpbars.icn");
	_dseFace.load("dse.fac");
	_hiliteChar = -1;
}

void PartyDrawer::drawParty(bool updateFlag) {
	Combat &combat = *_vm->_combat;
	Party &party = *_vm->_party;
	Resources &res = *_vm->_resources;
	Screen &screen = *_vm->_screen;
	bool inCombat = _vm->_mode == MODE_COMBAT;
	_restoreSprites.draw(screen, 0, Common::Point(8, 149));

	// Handle drawing the party faces
	uint partyCount = inCombat ? combat._combatParty.size() : party._activeParty.size();
	for (uint idx = 0; idx < partyCount; ++idx) {
		Character &ps = inCombat ? *combat._combatParty[idx] : party._activeParty[idx];
		Condition charCondition = ps.worstCondition();
		int charFrame = FACE_CONDITION_FRAMES[charCondition];
		
		SpriteResource *sprites = (charFrame > 4) ? &_dseFace : ps._faceSprites;
		if (charFrame > 4)
			charFrame -= 5;

		sprites->draw(screen, charFrame, Common::Point(CHAR_FACES_X[idx], 150));
	}

	for (uint idx = 0; idx < partyCount; ++idx) {
		Character &ps = inCombat ? *combat._combatParty[idx] : party._activeParty[idx];

		// Draw the Hp bar
		int maxHp = ps.getMaxHP();
		int frame;
		if (ps._currentHp < 1)
			frame = 4;
		else if (ps._currentHp > maxHp)
			frame = 3;
		else if (ps._currentHp == maxHp)
			frame = 0;
		else if (ps._currentHp < (maxHp / 4))
			frame = 2;
		else
			frame = 1;

		_hpSprites.draw(screen, frame, Common::Point(HP_BARS_X[idx], 182));
	}

	if (_hiliteChar != -1)
		res._globalSprites.draw(screen, 8, Common::Point(CHAR_FACES_X[_hiliteChar] - 1, 149));

	if (updateFlag)
		screen._windows[33].update();
}

void PartyDrawer::highlightChar(int charId) {
	Resources &res = *_vm->_resources;
	Screen &screen = *_vm->_screen;

	if (charId != _hiliteChar && _hiliteChar != HILIGHT_CHAR_DISABLED) {
		// Handle deselecting any previusly selected char
		if (_hiliteChar != -1) {
			res._globalSprites.draw(screen, 9 + _hiliteChar,
				Common::Point(CHAR_FACES_X[_hiliteChar] - 1, 149));
		}

		// Highlight new character
		res._globalSprites.draw(screen, 8, Common::Point(CHAR_FACES_X[charId] - 1, 149));
		_hiliteChar = charId;
		screen._windows[33].update();
	}
}

void PartyDrawer::unhighlightChar() {
	Resources &res = *_vm->_resources;
	Screen &screen = *_vm->_screen;

	if (_hiliteChar != -1) {
		res._globalSprites.draw(screen, _hiliteChar + 9,
			Common::Point(CHAR_FACES_X[_hiliteChar] - 1, 149));
		_hiliteChar = -1;
		screen._windows[33].update();
	}
}

void PartyDrawer::resetHighlight() {
	_hiliteChar = -1;
}
/*------------------------------------------------------------------------*/

Interface::Interface(XeenEngine *vm) : ButtonContainer(), InterfaceMap(vm), 
		PartyDrawer(vm), _vm(vm) {
	_buttonsLoaded = false;
	_intrIndex1 = 0;
	_steppingFX = 0;
	_falling = false;
	_blessedUIFrame = 0;
	_powerShieldUIFrame = 0;
	_holyBonusUIFrame = 0;
	_heroismUIFrame = 0;
	_flipUIFrame = 0;
	_face1UIFrame = 0;
	_face2UIFrame = 0;
	_batUIFrame = 0;
	_spotDoorsUIFrame = 0;
	_dangerSenseUIFrame = 0;
	_face1State = _face2State = 0;
	_upDoorText = false;
	_tillMove = 0;

	initDrawStructs();
}

void Interface::initDrawStructs() {
	_mainList[0] = DrawStruct(7, 232, 74);
	_mainList[1] = DrawStruct(0, 235, 75);
	_mainList[2] = DrawStruct(2, 260, 75);
	_mainList[3] = DrawStruct(4, 286, 75);
	_mainList[4] = DrawStruct(6, 235, 96);
	_mainList[5] = DrawStruct(8, 260, 96);
	_mainList[6] = DrawStruct(10, 286, 96);
	_mainList[7] = DrawStruct(12, 235, 117);
	_mainList[8] = DrawStruct(14, 260, 117);
	_mainList[9] = DrawStruct(16, 286, 117);
	_mainList[10] = DrawStruct(20, 235, 148);
	_mainList[11] = DrawStruct(22, 260, 148);
	_mainList[12] = DrawStruct(24, 286, 148);
	_mainList[13] = DrawStruct(26, 235, 169);
	_mainList[14] = DrawStruct(28, 260, 169);
	_mainList[15] = DrawStruct(30, 286, 169);
}

void Interface::setup() {
	_borderSprites.load("border.icn");
	_spellFxSprites.load("spellfx.icn");
	_fecpSprites.load("fecp.brd");
	_blessSprites.load("bless.icn");
	_charPowSprites.load("charpow.icn");
	_uiSprites.load("inn.icn");

	Party &party = *_vm->_party;
	party.loadActiveParty();
	party._newDay = party._minutes >= 300;
}

void Interface::startup() {
	Resources &res = *_vm->_resources;
	Screen &screen = *_vm->_screen;
	_iconSprites.load("main.icn");

	animate3d();
	if (_vm->_map->_isOutdoors) {
		setIndoorsMonsters();
		setIndoorsObjects();
	} else {
		setOutdoorsMonsters();
		setOutdoorsObjects();
	}
	draw3d(false);

	res._globalSprites.draw(screen._windows[1], 5, Common::Point(232, 9));
	drawParty(false);

	_mainList[0]._sprites = &res._globalSprites;
	for (int i = 1; i < 16; ++i)
		_mainList[i]._sprites = &_iconSprites;

	setMainButtons();

	_tillMove = false;
}

void Interface::mainIconsPrint() {
	Screen &screen = *_vm->_screen;
	screen._windows[38].close();
	screen._windows[12].close();
	screen._windows[0].drawList(_mainList, 16);
	screen._windows[34].update();
}

void Interface::setMainButtons(bool combatMode) {
	clearButtons();

	addButton(Common::Rect(235,  75, 259,  95),  Common::KEYCODE_s, &_iconSprites);
	addButton(Common::Rect(260,  75, 284,  95),  Common::KEYCODE_c, &_iconSprites);
	addButton(Common::Rect(286,  75, 310,  95),  Common::KEYCODE_r, &_iconSprites);
	addButton(Common::Rect(235,  96, 259, 116),  Common::KEYCODE_b, &_iconSprites);
	addButton(Common::Rect(260,  96, 284, 116),  Common::KEYCODE_d, &_iconSprites);
	addButton(Common::Rect(286,  96, 310, 116),  Common::KEYCODE_v, &_iconSprites);
	addButton(Common::Rect(235, 117, 259, 137),  Common::KEYCODE_m, &_iconSprites);
	addButton(Common::Rect(260, 117, 284, 137),  Common::KEYCODE_i, &_iconSprites);
	addButton(Common::Rect(286, 117, 310, 137),  Common::KEYCODE_q, &_iconSprites);
	addButton(Common::Rect(109, 137, 122, 147), Common::KEYCODE_TAB, &_iconSprites);
	addButton(Common::Rect(235, 148, 259, 168), Common::KEYCODE_LEFT, &_iconSprites);
	addButton(Common::Rect(260, 148, 284, 168), Common::KEYCODE_UP, &_iconSprites);
	addButton(Common::Rect(286, 148, 310, 168), Common::KEYCODE_RIGHT, &_iconSprites);
	addButton(Common::Rect(235, 169, 259, 189), (Common::KBD_CTRL << 16) |Common::KEYCODE_LEFT, &_iconSprites);
	addButton(Common::Rect(260, 169, 284, 189), Common::KEYCODE_DOWN, &_iconSprites);
	addButton(Common::Rect(286, 169, 310, 189), (Common::KBD_CTRL << 16) | Common::KEYCODE_RIGHT, &_iconSprites);
	addButton(Common::Rect(236,  11, 308,  69),  Common::KEYCODE_EQUALS);
	addButton(Common::Rect(239,  27, 312,  37),  Common::KEYCODE_1);
	addButton(Common::Rect(239, 37, 312, 47), Common::KEYCODE_2);
	addButton(Common::Rect(239, 47, 312, 57), Common::KEYCODE_3);
	addPartyButtons(_vm);

	if (combatMode) {
		_buttons[0]._value = Common::KEYCODE_f;
		_buttons[1]._value = Common::KEYCODE_c;
		_buttons[2]._value = Common::KEYCODE_a;
		_buttons[3]._value = Common::KEYCODE_u;
		_buttons[4]._value = Common::KEYCODE_r;
		_buttons[5]._value = Common::KEYCODE_b;
		_buttons[6]._value = Common::KEYCODE_o;
		_buttons[7]._value = Common::KEYCODE_i;
		_buttons[16]._value = 0;
	}
}

/**
 * Waits for a keypress or click, whilst still allowing the game scene to
 * be animated.
 */
void Interface::perform() {
	Combat &combat = *_vm->_combat;
	EventsManager &events = *_vm->_events;
	Map &map = *_vm->_map;
	Party &party = *_vm->_party;
	Scripts &scripts = *_vm->_scripts;
	Spells &spells = *_vm->_spells;
	const Common::Rect waitBounds(8, 8, 224, 140);

	events.updateGameCounter();
	draw3d(true);

	// Wait for a frame
	do {
		events.pollEventsAndWait();
		checkEvents(_vm);
	} while (!_buttonValue && events.timeElapsed() < 1 && !_vm->_party->_partyDead);

	if (!_buttonValue && !_vm->_party->_partyDead)
		return;

	if (_buttonValue == Common::KEYCODE_SPACE ||
			(events._leftButton && waitBounds.contains(events._mousePos))) {
		int lookupId = map.mazeLookup(party._mazePosition, 
			WALL_SHIFTS[party._mazeDirection][2]);

		bool eventsFlag = true;
		switch (lookupId) {
		case 1:
			if (!map._isOutdoors) {
				scripts.openGrate(13, 1);
				eventsFlag = _buttonValue != 0;
			}

		case 6:
			if (!map._isOutdoors) {
				scripts.openGrate(9, 0);
				eventsFlag = _buttonValue != 0;
			}
			break;
		case 9:
			if (!map._isOutdoors) {
				scripts.openGrate(6, 0);
				eventsFlag = _buttonValue != 0;
			}
			break;
		case 13:
			if (!map._isOutdoors) {
				scripts.openGrate(1, 1);
				eventsFlag = _buttonValue != 0;
			}
			break;
		default:
			break;
		}
		if (eventsFlag) {
			scripts.checkEvents();
			if (_vm->shouldQuit())
				return;
		}
	}

	switch (_buttonValue) {
	case Common::KEYCODE_TAB:
		// Stop mosters doing any movement
		_vm->_moveMonsters = false;
		if (ControlPanel::show(_vm) == -1) {
			_vm->_quitMode = 2;
		} else {
			_vm->_moveMonsters = 1;
		}
		break;

	case Common::KEYCODE_SPACE:
	case Common::KEYCODE_w:
		// Wait one turn
		chargeStep();
		combat.moveMonsters();
		_upDoorText = false;
		_flipDefaultGround = !_flipDefaultGround;
		_flipGround = !_flipGround;

		stepTime();
		break;

	case (Common::KBD_CTRL << 16) | Common::KEYCODE_LEFT:
	case Common::KEYCODE_KP4:
		if (checkMoveDirection((Common::KBD_CTRL << 16) | Common::KEYCODE_LEFT)) {
			switch (party._mazeDirection) {
			case DIR_NORTH:
				--party._mazePosition.x;
				break;
			case DIR_SOUTH:
				++party._mazePosition.x;
				break;
			case DIR_EAST:
				++party._mazePosition.y;
				break;
			case DIR_WEST:
				--party._mazePosition.y;
				break;
			default:
				break;
			}

			chargeStep();
			_isAnimReset = true;
			party._mazeDirection = (Direction)((int)party._mazeDirection & 3);
			_flipSky = !_flipSky;
			stepTime();
		}
		break;

	case (Common::KBD_CTRL << 16) | Common::KEYCODE_RIGHT:
	case Common::KEYCODE_KP6:
		if (checkMoveDirection((Common::KBD_CTRL << 16) | Common::KEYCODE_RIGHT)) {
			switch (party._mazeDirection) {
			case DIR_NORTH:
				++party._mazePosition.x;
				break;
			case DIR_SOUTH:
				--party._mazePosition.x;
				break;
			case DIR_EAST:
				--party._mazePosition.y;
				break;
			case DIR_WEST:
				++party._mazePosition.y;
				break;
			default:
				break;
			}

			chargeStep();
			_isAnimReset = true;
			party._mazeDirection = (Direction)((int)party._mazeDirection & 3);
			_flipSky = !_flipSky;
			stepTime();
		}
		break;

	case Common::KEYCODE_LEFT:
	case Common::KEYCODE_KP7:
		party._mazeDirection = (Direction)((int)party._mazeDirection - 1);
		_isAnimReset = true;
		party._mazeDirection = (Direction)((int)party._mazeDirection & 3);
		_flipSky = !_flipSky;
		stepTime();
		break;

	case Common::KEYCODE_RIGHT:
	case Common::KEYCODE_KP9:
		party._mazeDirection = (Direction)((int)party._mazeDirection + 1);
		_isAnimReset = true;
		party._mazeDirection = (Direction)((int)party._mazeDirection & 3);
		_flipSky = !_flipSky;
		stepTime();
		break;

	case Common::KEYCODE_UP:
	case Common::KEYCODE_KP8:
		if (checkMoveDirection(Common::KEYCODE_UP)) {
			switch (party._mazeDirection) {
			case DIR_NORTH:
				++party._mazePosition.y;
				break;
			case DIR_SOUTH:
				--party._mazePosition.y;
				break;
			case DIR_EAST:
				++party._mazePosition.x;
				break;
			case DIR_WEST:
				--party._mazePosition.x;
				break;
			default:
				break;
			}

			chargeStep();
			stepTime();
		}
		break;

	case Common::KEYCODE_DOWN:
	case Common::KEYCODE_KP2:
		if (checkMoveDirection(Common::KEYCODE_DOWN)) {
			switch (party._mazeDirection) {
			case DIR_NORTH:
				--party._mazePosition.y;
				break;
			case DIR_SOUTH:
				++party._mazePosition.y;
				break;
			case DIR_EAST:
				--party._mazePosition.x;
				break;
			case DIR_WEST:
				++party._mazePosition.x;
				break;
			default:
				break;
			}

			chargeStep();
			stepTime();
		}
		break;

	case Common::KEYCODE_F1:
	case Common::KEYCODE_F2:
	case Common::KEYCODE_F3:
	case Common::KEYCODE_F4:
	case Common::KEYCODE_F5:
	case Common::KEYCODE_F6:
		_buttonValue -= Common::KEYCODE_F1;
		if (_buttonValue < (int)party._activeParty.size()) {
			CharacterInfo::show(_vm, _buttonValue);
			if (party._stepped)
				combat.moveMonsters();
		}
		break;

	case Common::KEYCODE_EQUALS:
	case Common::KEYCODE_KP_EQUALS:
		// Toggle minimap
		_vm->_moveMonsters = false;
		party._automapOn = !party._automapOn;
		_vm->_moveMonsters = true;
		break;

	case Common::KEYCODE_b:
		chargeStep();
		
		if (map.getCell(2) < map.mazeData()._difficulties._wallNoPass
				&& !map._isOutdoors) {
			switch (party._mazeDirection) {
			case DIR_NORTH:
				++party._mazePosition.y;
				break;
			case DIR_EAST:
				++party._mazePosition.x;
				break;
			case DIR_SOUTH:
				--party._mazePosition.y;
				break;
			case DIR_WEST:
				--party._mazePosition.x;
				break;
			}
			chargeStep();
			stepTime();
		} else {
			bash(party._mazePosition, party._mazeDirection);
		}
		break;

	case Common::KEYCODE_c: {
		// Cast spell
		if (_tillMove) {
			combat.moveMonsters();
			draw3d(true);
		}

		int result = 0;
		Character *c = &party._activeParty[(spells._lastCaster < 0 ||
			spells._lastCaster >= (int)party._activeParty.size()) ?
			(int)party._activeParty.size() - 1 : spells._lastCaster];
		do {
			int spellId = CastSpell::show(_vm, c, _vm->_mode);
			if (spellId == -1 || c == nullptr)
				break;

			result = spells.castSpell(c, spellId);
		} while (result != -1);

		if (result == 1) {
			chargeStep();
			doStepCode();
		}
		break;
	}

	case Common::KEYCODE_i:
		// Show Info dialog
		_vm->_moveMonsters = false;
		InfoDialog::show(_vm);
		_vm->_moveMonsters = true;
		break;

	case Common::KEYCODE_m:
		// Show map dialog
		AutoMapDialog::show(_vm);
		break;

	case Common::KEYCODE_q:
		// Show the quick reference dialog
		QuickReferenceDialog::show(_vm);
		break;

	case Common::KEYCODE_r:
		// Rest
		rest();
		break;

	case Common::KEYCODE_v:
		// Show the quests dialog
		Quests::show(_vm);
		break;

	case Common::KEYCODE_x:
		// ****DEBUG***
		PartyDialog::show(_vm); //***DEBUG****
	default:
		break;
	}
}

void Interface::chargeStep() {
	if (_vm->_party->_partyDead) {
		_vm->_party->changeTime(_vm->_map->_isOutdoors ? 10 : 1);
		if (!_tillMove) {
			_vm->_combat->moveMonsters();
		}

		_tillMove = 3;
	}
}

/**
 * Handles incrementing game time
 */
void Interface::stepTime() {
	Party &party = *_vm->_party;
	SoundManager &sound = *_vm->_sound;
	doStepCode();
	
	if (++party._ctr24 == 24)
		party._ctr24 = 0;

	if (_buttonValue != Common::KEYCODE_SPACE && _buttonValue != Common::KEYCODE_w) {
		_steppingFX ^= 1;
		sound.playFX(_steppingFX + 7);
	}

	_upDoorText = false;
	_flipDefaultGround = !_flipDefaultGround;
	_flipGround = !_flipGround;
}

void Interface::doStepCode() {
	Combat &combat = *_vm->_combat;
	Map &map = *_vm->_map;
	Party &party = *_vm->_party;
	Scripts &scripts = *_vm->_scripts;
	int damage = 0;

	party._stepped = true;
	_upDoorText = false;

	map.getCell(2);
	int surfaceId = map.mazeData()._surfaceTypes[map._currentSurfaceId];

	switch (surfaceId) {
	case SURFTYPE_SPACE:
		// Wheeze.. can't breathe in space! Explosive decompression, here we come 
		party._partyDead = true;
		break;
	case SURFTYPE_LAVA:
		// It burns, it burns! 
		damage = 100;
		party._damageType = DT_FIRE;
		break;
	case SURFTYPE_SKY:
		// We can fly, we can.. oh wait, we can't!
		damage = 100;
		party._damageType = DT_PHYSICAL;
		_falling = true;
		break;
	case SURFTYPE_DESERT:
		// Without navigation skills, simulate getting lost by adding extra time
		if (map._isOutdoors && !party.checkSkill(NAVIGATOR))
			party.addTime(170);
		break;
	case SURFTYPE_CLOUD:
		if (!party._levitateActive) {
			party._damageType = DT_PHYSICAL;
			_falling = true;
			damage = 100;
		}
		break;
	default:
		break;
	}

	if (_vm->_files->_isDarkCc && party._gameFlags[374]) {
		_falling = false;
	} else {
		if (_falling)
			startFalling(false);

		if ((party._mazePosition.x & 16) || (party._mazePosition.y & 16)) {
			if (map._isOutdoors)
				map.getNewMaze();
		}

		if (damage) {
			_flipGround = !_flipGround;
			draw3d(true);

			int oldVal = scripts._v2;
			scripts._v2 = 0;
			combat.giveCharDamage(damage, combat._damageType, 0);

			scripts._v2 = oldVal;
			_flipGround = !_flipGround;
		} else if (party._partyDead) {
			draw3d(true);
		}
	}
}

/**
 * Start the party falling
 */
void Interface::startFalling(bool flag) {
	Combat &combat = *_vm->_combat;
	Map &map = *_vm->_map;
	Party &party = *_vm->_party;
	Scripts &scripts = *_vm->_scripts;
	bool isDarkCc = _vm->_files->_isDarkCc;

	if (isDarkCc && party._gameFlags[374]) {
		_falling = 0;
		return;
	}

	_falling = false;
	draw3d(true);
	_falling = 2;
	draw3d(false);

	if (flag) {
		if (!isDarkCc || party._fallMaze != 0) {
			party._mazeId = party._fallMaze;
			party._mazePosition = party._fallPosition;
		}
	}

	_falling = true;
	map.load(party._mazeId);
	if (flag) {
		if (((party._mazePosition.x & 16) || (party._mazePosition.y & 16)) &&
				map._isOutdoors) {
			map.getNewMaze();
		}
	} 
	
	if (isDarkCc) {
		switch (party._mazeId - 25) {
		case 0:
		case 26:
		case 27:
		case 28:
		case 29:
			party._mazeId = 24;
			party._mazePosition = Common::Point(11, 9);
			break;
		case 1:
		case 30:
		case 31:
		case 32:
		case 33:
			party._mazeId = 12;
			party._mazePosition = Common::Point(6, 15);
			break;
		case 2:
		case 34:
		case 35:
		case 36:
		case 37:
		case 51:
		case 52:
		case 53:
			party._mazeId = 15;
			party._mazePosition = Common::Point(4, 12);
			party._mazeDirection = DIR_SOUTH;
			break;
		case 40:
		case 41:
			party._mazeId = 14;
			party._mazePosition = Common::Point(8, 3);
			break;
		case 44:
		case 45:
			party._mazeId = 1;
			party._mazePosition = Common::Point(8, 7);
			party._mazeDirection = DIR_NORTH;
			break;
		case 49:
			party._mazeId = 12;
			party._mazePosition = Common::Point(11, 13);
			party._mazeDirection = DIR_SOUTH;
			break;
		case 57:
		case 58:
		case 59:
			party._mazeId = 5;
			party._mazePosition = Common::Point(12, 7);
			party._mazeDirection = DIR_NORTH;
			break;
		case 60:
			party._mazeId = 6;
			party._mazePosition = Common::Point(12, 3);
			party._mazeDirection = DIR_NORTH;
			break;
		default:
			party._mazeId = 23;
			party._mazePosition = Common::Point(12, 10);
			party._mazeDirection = DIR_NORTH;
			break;
		}
	} else {
		if (party._mazeId > 89 && party._mazeId < 113) {
			party._mazeId += 168;
		} else {
			switch (party._mazeId - 25) {
			case 0:
				party._mazeId = 89;
				party._mazePosition = Common::Point(2, 14);
				break;
			case 1:
				party._mazeId = 109;
				party._mazePosition = Common::Point(13, 14);
				break;
			case 2:
				party._mazeId = 112;
				party._mazePosition = Common::Point(13, 3);
				break;
			case 3:
				party._mazeId = 92;
				party._mazePosition = Common::Point(2, 3);
				break;
			case 12:
			case 13:
				party._mazeId = 14;
				party._mazePosition = Common::Point(10, 2);
				break;
			case 16:
			case 17:
			case 18:
				party._mazeId = 4;
				party._mazePosition = Common::Point(5, 14);
				break;
			case 20:
			case 21:
			case 22:
				party._mazeId = 21;
				party._mazePosition = Common::Point(9, 11);
				break;
			case 24:
			case 25:
			case 26:
				party._mazeId = 1;
				party._mazePosition = Common::Point(10, 4);
				break;
			case 28:
			case 29:
			case 30:
			case 31:
				party._mazeId = 26;
				party._mazePosition = Common::Point(12, 10);
				break;
			case 32:
			case 33:
			case 34:
			case 35:
				party._mazeId = 3;
				party._mazePosition = Common::Point(4, 9);
				break;
			case 36:
			case 37:
			case 38:
			case 39:
				party._mazeId = 16;
				party._mazePosition = Common::Point(2, 7);
				break;
			case 40:
			case 41:
			case 42:
			case 43:
				party._mazeId = 23;
				party._mazePosition = Common::Point(10, 9);
				break;
			case 44:
			case 45:
			case 46:
			case 47:
				party._mazeId = 13;
				party._mazePosition = Common::Point(2, 10);
				break;
			case 103:
			case 104:
				map._loadDarkSide = false;
				party._mazeId = 8;
				party._mazePosition = Common::Point(11, 15);
				party._mazeDirection = DIR_NORTH;
				break;
			case 105:
				party._mazeId = 24;
				party._mazePosition = Common::Point(11, 9);
				break;
			case 106:
				party._mazeId = 12;
				party._mazePosition = Common::Point(6, 15);
				break;
			case 107:
				party._mazeId = 15;
				party._mazePosition = Common::Point(4, 12);
				break;
			default:
				party._mazeId = 29;
				party._mazePosition = Common::Point(25, 21);
				party._mazeDirection = DIR_NORTH;
				break;
			}
		}
	}

	_flipGround ^= 1;
	draw3d(true);
	int tempVal = scripts._v2;
	scripts._v2 = 0;
	combat.giveCharDamage(party._fallDamage, DT_PHYSICAL, 0);
	scripts._v2 = tempVal;

	_flipGround ^= 1;
}

/**
 * Check movement in the given direction
 */
bool Interface::checkMoveDirection(int key) {
	Map &map = *_vm->_map;
	Party &party = *_vm->_party;
	SoundManager &sound = *_vm->_sound;
	Direction dir = party._mazeDirection;

	switch (key) {
	case (Common::KBD_CTRL << 16) | Common::KEYCODE_LEFT:
		party._mazeDirection = (party._mazeDirection == DIR_NORTH) ? DIR_WEST :
			(Direction)(party._mazeDirection - 1);
		break;
	case (Common::KBD_CTRL << 16) | Common::KEYCODE_RIGHT:
		party._mazeDirection = (party._mazeDirection == DIR_WEST) ? DIR_NORTH :
			(Direction)(party._mazeDirection + 1);
		break;
	case Common::KEYCODE_DOWN:
		party._mazeDirection = (party._mazeDirection == DIR_NORTH) ? DIR_SOUTH : DIR_NORTH;
		break;
	default:
		break;
	}
	
	map.getCell(7);
	int startSurfaceId = map._currentSurfaceId;
	int surfaceId;

	if (map._isOutdoors) {
		party._mazeDirection = dir;

		switch (map._currentWall) {
		case 5:
			if (_vm->_files->_isDarkCc)
				goto check;

			// Deliberate FAll-through
		case 0:
		case 2:
		case 4:
		case 8:
		case 11:
		case 13:
		case 14:
			surfaceId = map.mazeData()._surfaceTypes[map._currentSurfaceId];
			if (surfaceId == SURFTYPE_WATER) {
				if (party.checkSkill(SWIMMING) || party._walkOnWaterActive)
					return true;
			} else if (surfaceId == SURFTYPE_DWATER) {
				if (party._walkOnWaterActive)
					return true;
			} else if (surfaceId != SURFTYPE_SPACE) {
				return true;
			}

			sound.playFX(21);
			return false;

		case 1:
		case 7:
		case 9:
		case 10:
		case 12:
			check:
			if (party.checkSkill(MOUNTAINEER))
				return true;

			sound.playFX(21);
			return false;

		default:
			break;
		}
	} else {
		int surfaceId = map.getCell(2);
		if (surfaceId >= map.mazeData()._difficulties._wallNoPass) {
			party._mazeDirection = dir;
			sound.playFX(46);
			return false;
		} else {
			party._mazeDirection = dir;

			if (startSurfaceId == SURFTYPE_SWAMP || party.checkSkill(SWIMMING) ||
					party._walkOnWaterActive) {
				sound.playFX(46);
				return false;
			} else {
				if (_buttonValue == Common::KEYCODE_UP && _wo[107]) {
					_openDoor = true;
					sound.playFX(47);
					draw3d(true);
					_openDoor = false;
				}
				return true;
			}
		}
	}

	return true;
}

void Interface::rest() {
	EventsManager &events = *_vm->_events;
	Map &map = *_vm->_map;
	Party &party = *_vm->_party;
	Screen &screen = *_vm->_screen;
	SoundManager &sound = *_vm->_sound;

	map.cellFlagLookup(party._mazePosition);

	if ((map._currentCantRest || (map.mazeData()._mazeFlags & RESTRICTION_REST))
			&& _vm->_mode != MODE_12) {
		ErrorScroll::show(_vm, TOO_DANGEROUS_TO_REST, WT_NONFREEZED_WAIT);
	} else {
		// Check whether any character is in danger of dying
		bool dangerFlag = false;
		for (uint charIdx = 0; charIdx < party._activeParty.size(); ++charIdx) {
			for (int attrib = MIGHT; attrib <= LUCK; ++attrib) {
				if (party._activeParty[charIdx].getStat((Attribute)attrib) < 1)
					dangerFlag = true;
			}
		}

		if (dangerFlag) {
			if (!Confirm::show(_vm, SOME_CHARS_MAY_DIE))
				return;
		}

		// Mark all the players as being asleep
		for (uint charIdx = 0; charIdx < party._activeParty.size(); ++charIdx) {
			party._activeParty[charIdx]._conditions[ASLEEP] = 1;
		}
		drawParty(true);

		Mode oldMode = _vm->_mode;
		_vm->_mode = MODE_SLEEPING;

		if (oldMode == MODE_12) {
			party.changeTime(8 * 60);
		} else {
			for (int idx = 0; idx < 10; ++idx) {
				chargeStep();
				draw3d(true);
				
				if (_vm->_mode == MODE_1) {
					_vm->_mode = oldMode;
					return;
				}
			}

			party.changeTime(map._isOutdoors ? 380 : 470);
		}

		if (_vm->getRandomNumber(1, 20) == 1) {
			// Show dream
			screen.saveBackground();
			screen.fadeOut(4);
			events.hideCursor();

			screen.loadBackground("scene1.raw");
			screen._windows[0].update();
			screen.fadeIn(4);

			events.updateGameCounter();
			while (!_vm->shouldQuit() && events.timeElapsed() < 7)
				events.pollEventsAndWait();

			File f("dreams2.voc");
			sound.playSample(&f, 1);
			while (!_vm->shouldQuit() && sound.playSample(1, 0))
				events.pollEventsAndWait();
			f.close();

			f.openFile("laff1.voc");
			sound.playSample(&f, 1);
			while (!_vm->shouldQuit() && sound.playSample(1, 0))
				events.pollEventsAndWait();
			f.close();

			events.updateGameCounter();
			while (!_vm->shouldQuit() && events.timeElapsed() < 7)
				events.pollEventsAndWait();

			screen.fadeOut(4);
			events.setCursor(0);
			screen.restoreBackground();
			screen._windows[0].update();

			screen.fadeIn(4);
		}

		party.resetTemps();

		// Wake up the party
		bool starving = false;
		int foodConsumed = 0;
		for (uint charIdx = 0; charIdx < party._activeParty.size(); ++charIdx) {
			Character &c = party._activeParty[charIdx];
			c._conditions[ASLEEP] = 0;

			if (party._food == 0) {
				starving = true;
			} else {
				party._rested = true;
				Condition condition = c.worstCondition();

				if (condition < DEAD || condition > ERADICATED) {
					--party._food;
					++foodConsumed;
					party._heroism = 0;
					party._holyBonus = 0;
					party._powerShield = 0;
					party._blessed = 0;
					c._conditions[UNCONSCIOUS] = 0;
					c._currentHp = c.getMaxHP();
					c._currentSp = c.getMaxSP();
				}
			}
		}

		drawParty(true);
		_vm->_mode = oldMode;
		doStepCode();
		draw3d(true);

		ErrorScroll::show(_vm, Common::String::format(REST_COMPLETE,
			starving ? PARTY_IS_STARVING : HIT_SPELL_POINTS_RESTORED,
			foodConsumed));
		party.checkPartyDead();
	}
}

void Interface::bash(const Common::Point &pt, Direction direction) {
	EventsManager &events = *_vm->_events;
	Map &map = *_vm->_map;
	Party &party = *_vm->_party;
	Screen &screen = *_vm->_screen;
	SoundManager &sound = *_vm->_sound;

	if (map._isOutdoors)
		return;

	sound.playFX(31);

	uint charNum1 = 0, charNum2 = 0;
	for (uint charIdx = 0; charIdx < party._activeParty.size(); ++charIdx) {
		Character &c = party._activeParty[charIdx];
		Condition condition = c.worstCondition();

		if (!(condition == ASLEEP || (condition >= PARALYZED &&
				condition <= ERADICATED))) {
			if (charNum1) {
				charNum2 = charIdx + 1;
				break;
			} else {
				charNum1 = charIdx + 1;
			}
		}
	}

	party._activeParty[charNum1 - 1].subtractHitPoints(2);
	_charPowSprites.draw(screen._windows[0], 0, 
		Common::Point(CHAR_FACES_X[charNum1 - 1], 150));
	screen._windows[0].update();

	if (charNum2) {
		party._activeParty[charNum2 - 1].subtractHitPoints(2);
		_charPowSprites.draw(screen._windows[0], 0,
			Common::Point(CHAR_FACES_X[charNum2 - 1], 150));
		screen._windows[0].update();
	}

	int cell = map.mazeLookup(Common::Point(pt.x + SCREEN_POSITIONING_X[direction][7],
		pt.y + SCREEN_POSITIONING_Y[direction][7]), 0, 0xffff);
	if (cell != INVALID_CELL) {
		int v = map.getCell(2);

		if (v == 7) {
			++_wo[207];
			++_wo[267];
			++_wo[287];
		} else if (v == 14) {
			++_wo[267];
			++_wo[287];
		} else if (v == 15) {
			++_wo[287];
		} else {
			int might = party._activeParty[charNum1 - 1].getStat(MIGHT) +
				_vm->getRandomNumber(1, 30);
			if (charNum2)
				might += party._activeParty[charNum2 - 1].getStat(MIGHT);

			int bashThreshold = (v == 9) ? map.mazeData()._difficulties._bashGrate :
				map.mazeData()._difficulties._bashWall;
			if (might >= bashThreshold) {
				// Remove the wall on the current cell, and the reverse wall
				// on the cell we're bashing through to
				map.setWall(pt, direction, 3);
				switch (direction) {
				case DIR_NORTH:
					map.setWall(Common::Point(pt.x, pt.y + 1), DIR_SOUTH, 3);
					break;
				case DIR_EAST:
					map.setWall(Common::Point(pt.x + 1, pt.y), DIR_WEST, 3);
					break;
				case DIR_SOUTH:
					map.setWall(Common::Point(pt.x, pt.y - 1), DIR_NORTH, 3);
					break;
				case DIR_WEST:
					map.setWall(Common::Point(pt.x - 1, pt.y), DIR_EAST, 3);
					break;
				}
			}
		}
	}

	party.checkPartyDead();
	events.ipause(2);
	drawParty(true);
}

void Interface::draw3d(bool updateFlag) {
	Combat &combat = *_vm->_combat;
	EventsManager &events = *_vm->_events;
	Party &party = *_vm->_party;
	Screen &screen = *_vm->_screen;
	Scripts &scripts = *_vm->_scripts;
	
	if (screen._windows[11]._enabled)
		return;

	_flipUIFrame = (_flipUIFrame + 1) % 4;
	if (_flipUIFrame == 0)
		_flipWater = !_flipWater;
	if (_tillMove && (_vm->_mode == MODE_1 || _vm->_mode == MODE_COMBAT) &&
		!combat._monstersAttacking && _vm->_moveMonsters) {
		if (--_tillMove == 0)
			combat.moveMonsters();
	}

	// Draw the map
	drawMap();

	// Draw the minimap
	drawMiniMap();

	if (_falling == 1)
		handleFalling();

	if (_falling == 2) {
		screen.saveBackground(1);
	}

	assembleBorder();

	// Draw any on-screen text if flagged to do so
	if (_upDoorText && combat._attackMonsters[0] == -1) {
		screen._windows[3].writeString(_screenText);
	}

	if (updateFlag) {
		screen._windows[1].update();
		screen._windows[3].update();
	}

	if (combat._attackMonsters[0] != -1 || combat._attackMonsters[1] != -1
			|| combat._attackMonsters[2] != -1) {
		if ((_vm->_mode == MODE_1 || _vm->_mode == MODE_SLEEPING) && 
				!combat._monstersAttacking && !_charsShooting && _vm->_moveMonsters) {
			doCombat();
			if (scripts._eventSkipped)
				scripts.checkEvents();
		}
	}

	party._stepped = false;
	if (_vm->_mode == MODE_9) {
		// TODO: Save current scripts data?
	}

	// TODO: Check use of updateFlag here. Original doesn't have it, but I
	// wanted to ensure in places like the AutoMapDialog, that the draw3d
	// doesn't result in the screen updating until the dialog has had
	// a chance to full render itself
	if (updateFlag)
		events.wait(2);
}

/**
 * Handle doing the falling
 */
void Interface::handleFalling() {
	Party &party = *_vm->_party;
	Screen &screen = *_vm->_screen;
	SoundManager &sound = *_vm->_sound;
	Window &w = screen._windows[3];
	File voc1("scream.voc");
	File voc2("unnh.voc");
	saveFall();

	for (uint idx = 0; idx < party._activeParty.size(); ++idx) {
		party._activeParty[idx]._faceSprites->draw(screen._windows[0], 4,
			Common::Point(CHAR_FACES_X[idx], 150));
	}

	screen._windows[33].update();
	sound.playFX(11);
	sound.playSample(&voc1, 0);

	for (int idx = 0, incr = 2; idx < 133; ++incr, idx += incr) {
		fall(idx);
		assembleBorder();
		w.update();
	}

	fall(132);
	assembleBorder();
	w.update();

	sound.playSample(nullptr, 0);
	sound.playSample(&voc2, 0);
	sound.playFX(31);

	fall(127);
	assembleBorder();
	w.update();

	fall(132);
	assembleBorder();
	w.update();

	fall(129);
	assembleBorder();
	w.update();

	fall(132);
	assembleBorder();
	w.update();

	shake(10);
}

void Interface::saveFall() {
	// TODO
}

void Interface::fall(int v) {

}

/**
 * Shake the screen
 */
void Interface::shake(int time) {

}

/**
 * Draw the minimap
 */
void Interface::drawMiniMap() {
	Map &map = *_vm->_map;
	Party &party = *_vm->_party;
	Resources &res = *_vm->_resources;
	Screen &screen = *_vm->_screen;
	Window &window1 = screen._windows[1];

	if (screen._windows[2]._enabled || screen._windows[10]._enabled)
		return;
	if (!party._automapOn && !party._wizardEyeActive) {
		// Draw the Might & Magic logo
		res._globalSprites.draw(window1, 5, Common::Point(232, 9));
		return;
	}

	int v, frame;
	int frame2 = _overallFrame * 2;
	bool eyeActive = party._wizardEyeActive;
	if (party._automapOn)
		party._wizardEyeActive = false;

	if (map._isOutdoors) {
		res._globalSprites.draw(window1, 15, Common::Point(237, 12));

		for (int rowNum = 0, yp = 12, yDiff = 3; rowNum < MINIMAP_SIZE; ++rowNum, yp += 8, --yDiff) {
			for (int colNum = 0, xp = 237, xDiff = -3; colNum < MINIMAP_SIZE; ++colNum, xp += 10, ++xDiff) {
				v = map.mazeLookup(
					Common::Point(party._mazePosition.x + xDiff, party._mazePosition.y + yDiff),
					4);
				frame = map.mazeDataCurrent()._surfaceTypes[v];

				if (frame != -1 && (map._currentSteppedOn || party._wizardEyeActive)) {
					map._tileSprites.draw(window1, frame, Common::Point(xp, yp));
				}
			}
		}

		for (int rowNum = 0, yp = 12, yDiff = 3; rowNum < MINIMAP_SIZE; ++rowNum, yp += 8, --yDiff) {
			for (int colNum = 0, xp = 237, xDiff = -3; colNum < MINIMAP_SIZE; ++colNum, xp += 10, ++xDiff) {
				v = map.mazeLookup(
					Common::Point(party._mazePosition.x + xDiff, party._mazePosition.y + yDiff),
					4);
				frame = map.mazeData()._wallTypes[v];

				if (frame != -1 && (map._currentSteppedOn || party._wizardEyeActive)) {
					map._tileSprites.draw(window1, frame + 16, Common::Point(xp, yp));
				}
			}
		}

		for (int rowNum = 0, yp = 12, yDiff = 3; rowNum < MINIMAP_SIZE; ++rowNum, yp += 8, --yDiff) {
			for (int colNum = 0, xp = 237, xDiff = -3; colNum < MINIMAP_SIZE; ++colNum, xp += 10, ++xDiff) {
				v = map.mazeLookup(
					Common::Point(party._mazePosition.x + xDiff, party._mazePosition.y + yDiff),
					4);

				if (v != -1 && (map._currentSteppedOn || party._wizardEyeActive)) {
					map._tileSprites.draw(window1, v + 32, Common::Point(xp, yp));
				}
			}
		}

		// Draw the direction arrow
		res._globalSprites.draw(window1, party._mazeDirection + 1,
			Common::Point(267, 36));
	}
	else {
		frame2 = (frame2 + 2) % 8;

		// First draw the default surface bases for each cell to show
		for (int rowNum = 0, yp = 12, yDiff = 3; rowNum < MINIMAP_SIZE; ++rowNum, yp += 8, --yDiff) {
			for (int colNum = 0, xp = 237, xDiff = -3; colNum < MINIMAP_SIZE; ++colNum, xp += 10, ++xDiff) {
				v = map.mazeLookup(
					Common::Point(party._mazePosition.x + xDiff, party._mazePosition.y + yDiff),
					0, 0xffff);

				if (v != INVALID_CELL && (map._currentSteppedOn || party._wizardEyeActive)) {
					map._tileSprites.draw(window1, 0, Common::Point(xp, yp));
				}
			}
		}

		// Draw correct surface bases for revealed tiles
		for (int rowNum = 0, yp = 17, yDiff = 3; rowNum < MINIMAP_SIZE; ++rowNum, yp += 8, --yDiff) {
			for (int colNum = 0, xp = 242, xDiff = -3; colNum < MINIMAP_SIZE; ++colNum, xp += 10, ++xDiff) {
				v = map.mazeLookup(
					Common::Point(party._mazePosition.x + xDiff, party._mazePosition.y + yDiff),
					0, 0xffff);
				int surfaceId = map.mazeData()._surfaceTypes[map._currentSurfaceId];

				if (v != INVALID_CELL && map._currentSurfaceId &&
					(map._currentSteppedOn || party._wizardEyeActive)) {
					map._tileSprites.draw(window1, surfaceId + 36, Common::Point(xp, yp));
				}
			}
		}

		v = map.mazeLookup(Common::Point(party._mazePosition.x - 4, party._mazePosition.y + 4), 0xffff, 0);
		if (v != INVALID_CELL && map._currentSurfaceId &&
			(map._currentSteppedOn || party._wizardEyeActive)) {
			map._tileSprites.draw(window1,
				map.mazeData()._surfaceTypes[map._currentSurfaceId] + 36,
				Common::Point(232, 9));
		}

		// Handle drawing surface sprites partially clipped at the left edge
		for (int rowNum = 0, yp = 17, yDiff = 3; rowNum < MINIMAP_SIZE; ++rowNum, --yDiff, yp += 8) {
			v = map.mazeLookup(
				Common::Point(party._mazePosition.x - 4, party._mazePosition.y + yDiff),
				0, 0xffff);

			if (v != INVALID_CELL && map._currentSurfaceId &&
				(map._currentSteppedOn || party._wizardEyeActive)) {
				map._tileSprites.draw(window1,
					map.mazeData()._surfaceTypes[map._currentSurfaceId] + 36,
					Common::Point(232, yp));
			}
		}

		// Handle drawing surface sprites partially clipped at the top edge
		for (int colNum = 0, xp = 242, xDiff = -3; colNum < MINIMAP_SIZE; ++colNum, ++xDiff, xp += 8) {
			v = map.mazeLookup(
				Common::Point(party._mazePosition.x + xDiff, party._mazePosition.y + 4),
				0, 0xffff);

			if (v != INVALID_CELL && map._currentSurfaceId &&
				(map._currentSteppedOn || party._wizardEyeActive)) {
				map._tileSprites.draw(window1,
					map.mazeData()._surfaceTypes[map._currentSurfaceId] + 36,
					Common::Point(xp, 9));
			}
		}

		//
		for (int idx = 0, xp = 237, yp = 60, xDiff = -3; idx < MINIMAP_SIZE;
			++idx, ++xDiff, xp += 10, yp -= 8) {
			v = map.mazeLookup(
				Common::Point(party._mazePosition.x - 4, party._mazePosition.y - 3 + idx),
				12, 0xffff);

			switch (v) {
			case 1:
				frame = 18;
				break;
			case 3:
				frame = 22;
				break;
			case 4:
			case 13:
				frame = 16;
				break;
			case 5:
			case 8:
				frame = 2;
				break;
			case 6:
				frame = 30;
				break;
			case 7:
				frame = 32;
				break;
			case 9:
				frame = 24;
				break;
			case 10:
				frame = 28;
				break;
			case 11:
				frame = 14;
				break;
			case 12:
				frame = frame2 + 4;
				break;
			case 14:
				frame = 24;
				break;
			case 15:
				frame = 26;
				break;
			default:
				frame = -1;
				break;
			}

			if (frame != -1 && (map._currentSteppedOn || party._wizardEyeActive))
				map._tileSprites.draw(window1, frame, Common::Point(222, yp));

			v = map.mazeLookup(
				Common::Point(party._mazePosition.x - 3 + idx, party._mazePosition.y + 4),
				0);

			switch (v) {
			case 1:
				frame = 19;
				break;
			case 2:
				frame = 35;
				break;
			case 3:
				frame = 23;
				break;
			case 4:
			case 13:
				frame = 17;
				break;
			case 5:
			case 8:
				frame = 3;
				break;
			case 6:
				frame = 31;
				break;
			case 7:
				frame = 33;
				break;
			case 9:
				frame = 21;
				break;
			case 10:
				frame = 29;
				break;
			case 11:
				frame = 15;
				break;
			case 12:
				frame = frame2 + 5;
				break;
			case 14:
				frame = 25;
				break;
			case 15:
				frame = 27;
				break;
			default:
				frame = -1;
				break;
			}

			if (frame != -1 && (map._currentSteppedOn || party._wizardEyeActive))
				map._tileSprites.draw(window1, frame, Common::Point(xp, 4));
		}

		// Draw the front/back walls of cells in the minimap
		for (int rowNum = 0, yp = 12, yDiff = 3; rowNum < MINIMAP_SIZE;
			++rowNum, --yDiff, yp += 8) {
			for (int colNum = 0, xp = 237, xDiff = -3; colNum < MINIMAP_SIZE;
				++colNum, ++xDiff, xp += 10) {
				if (colNum == 4 && rowNum == 4) {
					// Center of the minimap. Draw the direction arrow
					res._globalSprites.draw(window1, party._mazeDirection + 1,
						Common::Point(272, 40));
				}

				v = map.mazeLookup(Common::Point(party._mazePosition.x + xDiff,
					party._mazePosition.y + yDiff), 12, 0xffff);
				switch (v) {
				case 1:
					frame = 18;
					break;
				case 3:
					frame = 22;
					break;
				case 4:
				case 13:
					frame = 16;
					break;
				case 5:
				case 8:
					frame = 2;
					break;
				case 6:
					frame = 30;
					break;
				case 7:
					frame = 32;
					break;
				case 9:
					frame = 20;
					break;
				case 10:
					frame = 28;
					break;
				case 11:
					frame = 14;
					break;
				case 12:
					frame = frame2 + 4;
					break;
				case 14:
					frame = 24;
					break;
				case 15:
					frame = 26;
					break;
				default:
					frame = -1;
					break;
				}

				if (frame != -1 && (map._currentSteppedOn || party._wizardEyeActive)) {
					map._tileSprites.draw(window1, frame, Common::Point(xp, yp));
				}

				v = map.mazeLookup(Common::Point(party._mazePosition.x + xDiff,
					party._mazePosition.y + yDiff), 12, 0xffff);
				switch (v) {
				case 1:
					frame = 19;
					break;
				case 2:
					frame = 35;
					break;
				case 3:
					frame = 23;
					break;
				case 4:
				case 13:
					frame = 17;
					break;
				case 5:
				case 8:
					frame = 3;
					break;
				case 6:
					frame = 31;
					break;
				case 7:
					frame = 33;
					break;
				case 9:
					frame = 21;
					break;
				case 10:
					frame = 29;
					break;
				case 11:
					frame = 15;
					break;
				case 12:
					frame = frame2 + 5;
					break;
				case 14:
					frame = 25;
					break;
				case 15:
					frame = 27;
					break;
				default:
					frame = -1;
					break;
				}

				if (v == -1 && (map._currentSteppedOn || party._wizardEyeActive)) {
					map._tileSprites.draw(window1, frame, Common::Point(xp, yp));
				}
			}
		}

		// Draw the top of blocked/wall cells on the map
		for (int rowNum = 0, yp = 12, yDiff = 3; rowNum < MINIMAP_SIZE; ++rowNum, yp += 8, --yDiff) {
			for (int colNum = 0, xp = 237, xDiff = -3; colNum < MINIMAP_SIZE; ++colNum, xp += 10, ++xDiff) {
				v = map.mazeLookup(
					Common::Point(party._mazePosition.x + xDiff, party._mazePosition.y + yDiff),
					0, 0xffff);

				if (v == INVALID_CELL || (!map._currentSteppedOn && !party._wizardEyeActive)) {
					map._tileSprites.draw(window1, 1, Common::Point(xp, yp));
				}
			}
		}
	}

	// Draw outer rectangle around the automap
	res._globalSprites.draw(window1, 6, Common::Point(223, 3));
	party._wizardEyeActive = eyeActive;
}

/**
 * Draw the display borders
 */
void Interface::assembleBorder() {
	Resources &res = *_vm->_resources;
	Screen &screen = *_vm->_screen;

	// Draw the outer frame
	res._globalSprites.draw(screen._windows[0], 0, Common::Point(8, 8));

	// Draw the animating bat character used to show when levitate is active
	_borderSprites.draw(screen._windows[0], _vm->_party->_levitateActive ? _batUIFrame + 16 : 16,
		Common::Point(0, 82));
	_batUIFrame = (_batUIFrame + 1) % 12;

	// Draw UI element to indicate whether can spot hidden doors
	_borderSprites.draw(screen,
		(_thinWall && _vm->_party->checkSkill(SPOT_DOORS)) ? _spotDoorsUIFrame + 28 : 28,
		Common::Point(194, 91));
	_spotDoorsUIFrame = (_spotDoorsUIFrame + 1) % 12;

	// Draw UI element to indicate whether can sense danger
	_borderSprites.draw(screen,
		(_vm->_dangerSenseAllowed && _vm->_party->checkSkill(DANGER_SENSE)) ? _spotDoorsUIFrame + 40 : 40,
		Common::Point(107, 9));
	_dangerSenseUIFrame = (_dangerSenseUIFrame + 1) % 12;

	// Handle the face UI elements for indicating clairvoyance status
	_face1UIFrame = (_face1UIFrame + 1) % 4;
	if (_face1State == 0)
		_face1UIFrame += 4;
	else if (_face1State == 2)
		_face1UIFrame = 0;

	_face2UIFrame = (_face2UIFrame + 1) % 4 + 12;
	if (_face2State == 0)
		_face2UIFrame += 252;
	else if (_face2State == 2)
		_face2UIFrame = 0;

	if (!_vm->_party->_clairvoyanceActive) {
		_face1UIFrame = 0;
		_face2UIFrame = 8;
	}

	_borderSprites.draw(screen, _face1UIFrame, Common::Point(0, 32));
	_borderSprites.draw(screen,
		screen._windows[10]._enabled || screen._windows[2]._enabled ?
		52 : _face2UIFrame,
		Common::Point(215, 32));

	// Draw resistence indicators
	if (!screen._windows[10]._enabled && !screen._windows[2]._enabled
		&& screen._windows[38]._enabled) {
		_fecpSprites.draw(screen, _vm->_party->_fireResistence ? 1 : 0,
			Common::Point(2, 2));
		_fecpSprites.draw(screen, _vm->_party->_electricityResistence ? 3 : 2,
			Common::Point(219, 2));
		_fecpSprites.draw(screen, _vm->_party->_coldResistence ? 5 : 4,
			Common::Point(2, 134));
		_fecpSprites.draw(screen, _vm->_party->_poisonResistence ? 7 : 6,
			Common::Point(219, 134));
	} else {
		_fecpSprites.draw(screen, _vm->_party->_fireResistence ? 9 : 8,
			Common::Point(8, 8));
		_fecpSprites.draw(screen, _vm->_party->_electricityResistence ? 10 : 11,
			Common::Point(219, 8));
		_fecpSprites.draw(screen, _vm->_party->_coldResistence ? 12 : 13,
			Common::Point(8, 134));
		_fecpSprites.draw(screen, _vm->_party->_poisonResistence ? 14 : 15,
			Common::Point(219, 134));
	}

	// Draw UI element for blessed
	_blessSprites.draw(screen, 16, Common::Point(33, 137));
	if (_vm->_party->_blessed) {
		_blessedUIFrame = (_blessedUIFrame + 1) % 4;
		_blessSprites.draw(screen, _blessedUIFrame, Common::Point(33, 137));
	}

	// Draw UI element for power shield
	if (_vm->_party->_powerShield) {
		_powerShieldUIFrame = (_powerShieldUIFrame + 1) % 4;
		_blessSprites.draw(screen, _powerShieldUIFrame + 4,
			Common::Point(55, 137));
	}

	// Draw UI element for holy bonus
	if (_vm->_party->_holyBonus) {
		_holyBonusUIFrame = (_holyBonusUIFrame + 1) % 4;
		_blessSprites.draw(screen, _holyBonusUIFrame + 8, Common::Point(160, 137));
	}

	// Draw UI element for heroism
	if (_vm->_party->_heroism) {
		_heroismUIFrame = (_heroismUIFrame + 1) % 4;
		_blessSprites.draw(screen, _heroismUIFrame + 12, Common::Point(182, 137));
	}

	// Draw direction character if direction sense is active
	if (_vm->_party->checkSkill(DIRECTION_SENSE) && !_vm->_noDirectionSense) {
		const char *dirText = DIRECTION_TEXT_UPPER[_vm->_party->_mazeDirection];
		Common::String msg = Common::String::format(
			"\002""08\003""c\013""139\011""116%c\014""d\001", *dirText);
		screen._windows[0].writeString(msg);
	}

	// Draw view frame
	if (screen._windows[12]._enabled)
		screen._windows[12].frame();
}

void Interface::doCombat() {
	Combat &combat = *_vm->_combat;
	EventsManager &events = *_vm->_events;
	Map &map = *_vm->_map;
	Party &party = *_vm->_party;
	Screen &screen = *_vm->_screen;
	Scripts &scripts = *_vm->_scripts;
	Spells &spells = *_vm->_spells;
	SoundManager &sound = *_vm->_sound;
	bool upDoorText = _upDoorText;
	bool reloadMap = false;
	
	_upDoorText = false;
	combat._combatMode = 2;
	_vm->_mode = MODE_COMBAT;

	_iconSprites.load("combat.icn");
	for (int idx = 1; idx < 16; ++idx)
		_mainList[idx]._sprites = nullptr;

	// Set the combat buttons
	setMainButtons(true);
	mainIconsPrint();

	combat._combatParty.clear();
	combat._charsGone.clear();
	combat._charsBlocked.clear();
	combat._charsArray1[0] = 0;
	combat._charsArray1[1] = 0;
	combat._charsArray1[2] = 0;
	combat._monstersAttacking = 0;
	combat._partyRan = false;

	// Set up the combat party
	combat.setupCombatParty();
	combat.setSpeedTable();
	combat._whosSpeed = -1;
	combat._whosTurn = -1;
	resetHighlight();

	nextChar();

	if (!party._dead) {
		combat.setSpeedTable();
		if (_tillMove) {
			combat.moveMonsters();
			draw3d(true);
		}

		Window &w = screen._windows[2];
		w.open();
		bool breakFlag = false;

		while (!_vm->shouldQuit() && !breakFlag) {
			highlightChar(combat._whosTurn);
			combat.setSpeedTable();

			// Write out the description of the monsters being battled
			w.writeString(combat.getMonsterDescriptions());
			_iconSprites.draw(screen, 32, Common::Point(233, combat._monsterIndex * 10 + 27),
				0x8010000);
			w.update();

			// Wait for keypress
			int index = 0;
			do {
				events.updateGameCounter();
				draw3d(true);

				if (++index == 5 && combat._attackMonsters[0] != -1) {
					MazeMonster &monster = map._mobData._monsters[combat._monster2Attack];
					MonsterStruct &monsterData = map._monsterData[monster._spriteId];
					sound.playFX(monsterData._fx);
				}

				do {
					events.pollEventsAndWait();
					checkEvents(_vm);
				} while (!_vm->shouldQuit() && events.timeElapsed() < 1 && !_buttonValue);
			} while (!_vm->shouldQuit() && !_buttonValue);
			if (_vm->shouldQuit())
				return;

			switch (_buttonValue) {
			case Common::KEYCODE_TAB:
				// Show the control panel
				if (ControlPanel::show(_vm) == 2) {
					reloadMap = true;
					breakFlag = true;
				} else {
					highlightChar(combat._whosTurn);
				}
				break;

			case Common::KEYCODE_1:
			case Common::KEYCODE_2:
			case Common::KEYCODE_3:
				_buttonValue -= Common::KEYCODE_1;
				if (combat._attackMonsters[_buttonValue] != -1) {
					combat._monster2Attack = combat._attackMonsters[_buttonValue];
					combat._monsterIndex = _buttonValue;
				}
				break;

			case Common::KEYCODE_a:
				// Attack
				combat.attack(*combat._combatParty[combat._whosTurn], RT_0);
				nextChar();
				break;

			case Common::KEYCODE_b:
				// Block
				combat.block();
				nextChar();
				break;
				
			case Common::KEYCODE_c: {
				// Cast spell
				int spellId = CastSpell::show(_vm, _vm->_mode);
				if (spellId != -1) {
					Character *c = combat._combatParty[combat._whosTurn];
					spells.castSpell(c, spellId);
				}
				break;
			}

			case Common::KEYCODE_f:
				// Quick Fight
				combat.quickFight();
				nextChar();
				break;

			case Common::KEYCODE_i:
				// Info dialog
				InfoDialog::show(_vm);
				highlightChar(combat._whosTurn);
				break;

			case Common::KEYCODE_o:
				// Fight Options
				FightOptions::show(_vm);
				highlightChar(combat._whosTurn);
				break;
			
			case Common::KEYCODE_q:
				// Quick Reference dialog
				QuickReferenceDialog::show(_vm);
				highlightChar(combat._whosTurn);
				break;

			case Common::KEYCODE_r:
				// Run from combat
				combat.run();
				nextChar();

				if (_vm->_mode == MODE_1) {
					warning("TODO: loss of treasure");
					party.moveToRunLocation();
					breakFlag = true;
				}
				break;

			case Common::KEYCODE_u: {
				int whosTurn = combat._whosTurn;
				ItemsDialog::show(_vm, combat._combatParty[combat._whosTurn], ITEMMODE_COMBAT);
				if (combat._whosTurn == whosTurn) {
					highlightChar(combat._whosTurn);
				} else {
					combat._whosTurn = whosTurn;
					nextChar();
				}
				break;
			}

			case Common::KEYCODE_F1:
			case Common::KEYCODE_F2:
			case Common::KEYCODE_F3:
			case Common::KEYCODE_F4:
			case Common::KEYCODE_F5:
			case Common::KEYCODE_F6:
				// Show character info
				_buttonValue -= Common::KEYCODE_F1;
				if (_buttonValue < (int)combat._combatParty.size()) {
					CharacterInfo::show(_vm, _buttonValue);
				}
				highlightChar(combat._whosTurn);
				break;

			case Common::KEYCODE_LEFT:
			case Common::KEYCODE_RIGHT:
				// Rotate party direction left or right
				if (_buttonValue == Common::KEYCODE_LEFT) {
					party._mazeDirection = (party._mazeDirection == DIR_NORTH) ? 
						DIR_WEST : (Direction)((int)party._mazeDirection - 1);
				} else {
					party._mazeDirection = (party._mazeDirection == DIR_WEST) ?
					DIR_NORTH : (Direction)((int)party._mazeDirection + 1);
				}

				_flipSky ^= 1;
				if (_tillMove)
					combat.moveMonsters();
				party._stepped = true;
				break;
			}

			// Handling for if the combat turn is complete
			if (combat.allHaveGone()) {
				Common::fill(&combat._charsGone[0], &combat._charsGone[combat._charsGone.size()], false);
				Common::fill(&combat._charsBlocked[0], &combat._charsBlocked[combat._charsBlocked.size()], false);
				combat.setSpeedTable();
				combat._whosTurn = -1;
				combat._whosSpeed = -1;
				nextChar();

				for (uint idx = 0; idx < map._mobData._monsters.size(); ++idx) {
					MazeMonster &monster = map._mobData._monsters[idx];
					if (monster._spriteId == 53) {
						warning("TODO: Monster 53's HP is altered here?!");
					}
				}

				combat.moveMonsters();
				setIndoorsMonsters();
				party.changeTime(1);
			}

			if (combat._attackMonsters[0] == -1 && combat._attackMonsters[1] == -1
					&& combat._attackMonsters[2] == -1) {
				party.changeTime(1);
				draw3d(true);

				if (combat._attackMonsters[0] == -1 && combat._attackMonsters[1] == -1
						&& combat._attackMonsters[2] == -1)
					break;
			}

			party.checkPartyDead();
			if (party._dead || _vm->_mode != MODE_COMBAT)
				break;
		}

		_vm->_mode = MODE_1;
		if (combat._partyRan && (combat._attackMonsters[0] != -1 ||
				combat._attackMonsters[1] != -1 || combat._attackMonsters[2] != -1)) {
			party.checkPartyDead();
			if (!party._dead) {
				party.moveToRunLocation();

				for (uint idx = 0; idx < combat._combatParty.size(); ++idx) {
					Character &c = *combat._combatParty[idx];
					if (c.isDisabled())
						c._conditions[DEAD] = 1;
				}
			}
		}

		w.close();
		events.clearEvents();

		_vm->_mode = MODE_COMBAT;
		draw3d(true);
		party.giveTreasure();
		_vm->_mode = MODE_1;
		party._stepped = true;
		unhighlightChar();

		combat.setupCombatParty();
		drawParty(true);
	}

	_iconSprites.load("main.icn");
	for (int idx = 1; idx < 16; ++idx)
		_mainList[idx]._sprites = &_iconSprites;

	setMainButtons();
	mainIconsPrint();
	combat._monster2Attack = -1;

	if (upDoorText) {
		map.cellFlagLookup(party._mazePosition);
		if (map._currentIsEvent)
			scripts.checkEvents();
	}

	if (reloadMap) {
		sound.playFX(51);
		map._loadDarkSide = _vm->getGameID() != GType_WorldOfXeen;
		map.load(_vm->getGameID() == GType_WorldOfXeen ? 28 : 29);
		party._mazeDirection = _vm->getGameID() == GType_WorldOfXeen ?
			DIR_EAST : DIR_SOUTH;
	}

	combat._combatMode = 1;
}

/**
 * Select next character or monster to be attacking
 */
void Interface::nextChar() {
	Combat &combat = *_vm->_combat;
	Party &party = *_vm->_party;

	if (combat.allHaveGone())
		return;
	if ((combat._attackMonsters[0] == -1 && combat._attackMonsters[1] == -1 &&
		combat._attackMonsters[2] == -1) || combat._combatParty.size() == 0) {
		_vm->_mode = MODE_1;
		return;
	}

	for (;;) {
		// Check if party is dead
		party.checkPartyDead();
		if (party._dead) {
			_vm->_mode = MODE_1;
			break;
		}

		if (combat._whosTurn < (int)combat._combatParty.size()) {
			if (!combat.allHaveGone())
				highlightChar(combat._whosTurn);
			break;
		} else {
			combat.attackMonster(0);
			if (!party._dead)
				party.checkPartyDead();

			if (party._dead)
				break;
		}

		// Check the combat participants
		bool resetFlag = false;
		for (uint idx = 0; idx < combat._speedTable.size(); ++idx) {
			// Mark the given character as haven taken their turn
			if (combat._whosTurn != -1) {
				combat._charsGone[combat._whosTurn] = true;
			}

			combat._whosSpeed %= combat._speedTable.size();
			combat._whosTurn = combat._speedTable[combat._whosSpeed];

			if (combat.allHaveGone()) {
				if (combat.charsCantAct()) {
					combat.setSpeedTable();
					combat._whosTurn = -1;
					combat._whosSpeed = -1;

					combat._charsGone.resize(combat._speedTable.size());
					Common::fill(&combat._charsGone[0], &combat._charsGone[combat._charsGone.size()], 0);
					resetFlag = true;
					break;
				}
				return;
			} else if (combat._whosTurn >= (int)combat._combatParty.size() ||
					!combat._combatParty[combat._whosTurn]->isDisabledOrDead()) {
				break;
			}
		}

		if (party._dead)
			break;
	}
}

} // End of namespace Xeen