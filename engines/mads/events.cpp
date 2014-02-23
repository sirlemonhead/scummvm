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
#include "graphics/cursorman.h"
#include "common/events.h"
#include "engines/util.h"
#include "mads/mads.h"
#include "mads/events.h"

#define GAME_FRAME_RATE 50
#define GAME_FRAME_TIME (1000 / GAME_FRAME_RATE) 

namespace MADS {

EventsManager::EventsManager(MADSEngine *vm) {
	_vm = vm;
	_cursorSprites = nullptr;
	_gameCounter = 0;
	_priorFrameTime = 0;
	_keyPressed = false;
	_mouseClicked = false;
}

EventsManager::~EventsManager() {
	delete _cursorSprites;
}

void EventsManager::loadCursors(const Common::String &spritesName) {
	_cursorSprites = new SpriteAsset(_vm, spritesName, 0x4000);
}

void EventsManager::setCursor(CursorType cursorId) {
	_cursorId = cursorId;
	changeCursor();
}

void EventsManager::setCursor2(CursorType cursorId) {
	_cursorId = cursorId;
	_newCursorId = cursorId;
	changeCursor();
}

void EventsManager::showCursor() {
	CursorMan.showMouse(true);
}

void EventsManager::hideCursor() {
	CursorMan.showMouse(false);
}

void EventsManager::changeCursor() {

}

void EventsManager::pollEvents() {
	checkForNextFrameCounter();

	Common::Event event;
	while (g_system->getEventManager()->pollEvent(event)) {
		// Handle keypress
		switch (event.type) {
		case Common::EVENT_QUIT:
		case Common::EVENT_RTL:
			return;

		case Common::EVENT_KEYDOWN:
			// Check for debugger
			if (event.kbd.keycode == Common::KEYCODE_d && (event.kbd.flags & Common::KBD_CTRL)) {
				// Attach to the debugger
				_vm->_debugger->attach();
				_vm->_debugger->onFrame();
			} else {
				_keyPressed = true;
			}
			return;
		case Common::EVENT_KEYUP:
			_keyPressed = false;
			return;
		case Common::EVENT_LBUTTONDOWN:
		case Common::EVENT_RBUTTONDOWN:
			_mouseClicked = true;
			return;
		case Common::EVENT_LBUTTONUP:
		case Common::EVENT_RBUTTONUP:
			_mouseClicked = false;
			return;
		case Common::EVENT_MOUSEMOVE:
			_mousePos = event.mouse;
			break;
		default:
 			break;
		}
	}
}

void EventsManager::checkForNextFrameCounter() {
	// Check for next game frame
	uint32 milli = g_system->getMillis();
	if ((milli - _priorFrameTime) >= GAME_FRAME_TIME) {
		++_gameCounter;
		_priorFrameTime = milli;

		// Give time to the debugger
		_vm->_debugger->onFrame();

		// Display the frame
		_vm->_screen->updateScreen();

		// Signal the ScummVM debugger
		_vm->_debugger->onFrame();
	}
}

void EventsManager::delay(int cycles) {
	uint32 totalMilli = cycles * 1000 / GAME_FRAME_RATE;
	uint32 delayEnd = g_system->getMillis() + totalMilli;

	while (!_vm->shouldQuit() && g_system->getMillis() < delayEnd) {
		g_system->delayMillis(10);

		pollEvents();
	}
}

} // End of namespace MADS