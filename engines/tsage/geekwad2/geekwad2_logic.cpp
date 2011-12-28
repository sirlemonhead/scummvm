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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "common/config-manager.h"
#include "tsage/scenes.h"
#include "tsage/tsage.h"
#include "tsage/geekwad2/geekwad2_logic.h"
#include "tsage/geekwad2/geekwad2_scenes0.h"

namespace TsAGE {

namespace Geekwad2 {

void Geekwad2Game::start() {
	// Start the game
	g_globals->_sceneManager.changeScene(150);
}

Scene *Geekwad2Game::createScene(int sceneNumber) {
	switch (sceneNumber) {
	/* Scene group #0 */
	// Tsunami title screen
	case 150: return new Scene150();

	// Credits screen
	case 200: return new Scene200();

	default:
		error("Unknown scene number - %d", sceneNumber);
		break;
	}
}

void Geekwad2Game::rightClick() {
}

/**
 * Returns true if it is currently okay to restore a game
 */
bool Geekwad2Game::canLoadGameStateCurrently() {
	return true;
}

/**
 * Returns true if it is currently okay to save the game
 */
bool Geekwad2Game::canSaveGameStateCurrently() {
	return true;
}

void Geekwad2Game::processEvent(Event &event) {
}

/*--------------------------------------------------------------------------*/

SceneExt::SceneExt(): Scene() {
}

/*--------------------------------------------------------------------------*/

SceneObjectB4::SceneObjectB4(): SceneObject() {
	clear();
}

void SceneObjectB4::clear() {
	_field8E = _field90 = _field92 = 0;
	_message = Common::String();
	_resNum = -1;
	_lineNum = 0;
	_textMode = ALIGN_LEFT;
}

void SceneObjectB4::synchronize(Serializer &s) {
	SceneObject::synchronize(s);

	s.syncAsSint16LE(_field8E);
	s.syncAsSint16LE(_field90);
	s.syncAsSint16LE(_field92);
	s.syncAsSint16LE(_resNum);
	s.syncAsSint16LE(_lineNum);
	s.syncAsSint16LE(_fontNumber);
	s.syncAsSint16LE(_fontFgColor);
	s.syncAsSint16LE(_fontBgColor);
	s.syncAsSint16LE(_fontFgColor2);
	s.syncAsSint16LE(_maxWidth);
	s.syncAsSint16LE(_textMode);
	_textBounds.synchronize(s);
}

void SceneObjectB4::setup(int visage, int strip, int frame, int xp, int yp, int priority, int v) {
	if (GW2_GLOBALS._sceneManager._scene->_bgSceneObjects.contains(this))
		GW2_GLOBALS._sceneManager._scene->_bgSceneObjects.remove(this);

	SceneObject::setup(visage, strip, frame, xp, yp, priority);
	_field8E = v;
}

void SceneObjectB4::draw() {
	if (_message.empty() && (_resNum == -1)) {
		if (!_field90) {
			reposition();
			_textBounds = _bounds;
		}
	} else {
		// Get the string bounds
		GfxFont f;
		f.setFontNumber(_fontNumber);

		Rect msgBounds;
		if (_resNum == -1) {
			f.getStringBounds(_message.c_str(), msgBounds, _maxWidth);
		} else {
			Common::String msg = g_resourceManager->getMessage(_resNum, _lineNum);
			f.getStringBounds(msg.c_str(), msgBounds, _maxWidth);
		}

		if (_textMode == ALIGN_CENTER) {
			_textBounds.center(_position.x, _position.y);
		} else {
			_textBounds.moveTo(_position.x, _position.y);
		}
	}

	_textBounds.expandPanes();

	if (_field8E) {
		// TODO: Stuff relating to scene offsets here. Need to check if it's necessary
	}

	if (!_message.empty() || (_resNum != -1) || (_field90 && _field92)) {
		if (_field90) {
			reposition();
		} else {
			GfxFont f;
			f.setFontNumber(_fontNumber);
			f._colors.foreground = _fontFgColor;
			f._colors2.background = _fontBgColor;
			f._colors2.foreground = _fontFgColor2;

			if (_resNum == -1) {
				f.writeLines(_message.c_str(), _textBounds, _textMode);
			} else {
				Common::String msg = g_resourceManager->getMessage(_resNum, _lineNum);
				f.writeLines(msg.c_str(), _textBounds, _textMode);
			}
		}
	} else if (!_field90 || _field92) {
		SceneObject::draw();
	}
}

} // End of namespace Geekwad2

} // End of namespace TsAGE
