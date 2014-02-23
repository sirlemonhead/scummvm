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

#ifndef MADS_EVENTS_H
#define MADS_EVENTS_H

#include "common/scummsys.h"
#include "mads/msprite.h"
#include "mads/assets.h"

namespace MADS {

enum CursorType { CURSOR_NONE = 0, CURSOR_ARROW = 1, CURSOR_WAIT = 2, CURSOR_GO_DOWN = 3, 
	CURSOR_GO_UP = 4, CURSOR_GO_LEFT = 5, CURSOR_GO_RIGHT = 6 };

class MADSEngine;

class EventsManager {
private:
	MADSEngine *_vm;
	CursorType _cursorId;
	CursorType _newCursorId;
	uint32 _gameCounter;
	uint32 _priorFrameTime;
	Common::Point _mousePos;

	/**
	 * Updates the cursor image when the current cursor changes
	 */
	void changeCursor();

	/**
	 * Checks for whether the next game frame number has been reached.
	 */
	void checkForNextFrameCounter();
public:
	SpriteAsset *_cursorSprites;
	bool _mouseClicked;
	bool _keyPressed;
public:
	/**
	 * Constructor
	 */
	EventsManager(MADSEngine *vm);

	/**
	 * Destructor
	 */
	~EventsManager();

	/**
	 * Loads the sprite set containing the cursors
	 */
	void loadCursors(const Common::String &spritesName);

	/**
	 * Sets the cursor
	 */
	void setCursor(CursorType cursorId);

	/**
	 * Sets the cursor
	 */
	void setCursor2(CursorType cursorId);

	/**
	 * Show the mouse cursor
	 */
	void showCursor();

	/**
	 * Hide the mouse cursor
	 */
	void hideCursor();

	/**
	 * Poll any pending events
	 */
	void pollEvents();

	/**
	 * Return the current mouse position
	 */
	Common::Point mousePos() const { return _mousePos; }

	/**
	 * Delay for a given number of frames
	 */
	void delay(int amount);
};

} // End of namespace MADS

#endif /* MADS_EVENTS_H */