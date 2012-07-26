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
#include "tsage/geekwad2/geekwad2_space_golf.h"

namespace TsAGE {

namespace Geekwad2 {

void Geekwad2Game::start() {
	// Start the game
//	g_globals->_sceneManager.changeScene(150);
	g_globals->_sceneManager.changeScene(20);
}

Scene *Geekwad2Game::createScene(int sceneNumber) {
	switch (sceneNumber) {
	/* Scene group #0 */
	// Spaced Solicitors
	case 10: return new Scene10();

	// Space Golf
	case 20: return new Scene20();

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

int Geekwad2Game::showPauseDialog() {
	return MessageDialog::show("Stop game in progress?     ", "Resume game", " Stop game ");
}

void Geekwad2Game::minigameDone(int minigameNumber, uint32 score) {
	warning("TODO");
}

int Geekwad2Game::getRandomEmptyLockIndex() {
	warning("TODO");
	return -1;
}

void Geekwad2Game::saveHistory() {
	warning("TODO");
}

void Geekwad2Game::showLockDigit(char digit) {
	warning("TODO");
}


/*--------------------------------------------------------------------------*/

SceneExt::SceneExt(): Scene() {
	Common::fill(&_keyPressed[0], &_keyPressed[322], false);
}

void SceneExt::process(Event &event) {
	if (event.eventType == Common::EVENT_KEYDOWN)
		_keyPressed[event.kbd.keycode] = true;
	else if (event.eventType == Common::EVENT_KEYUP)
		_keyPressed[event.kbd.keycode] = true;

	Scene::process(event);
}

bool SceneExt::isKeyPressed(Common::KeyCode keyCode) {
	return _keyPressed[keyCode];
}

/*--------------------------------------------------------------------------*/

BackgroundTextualObject::BackgroundTextualObject() {
	clear();
}

void BackgroundTextualObject::remove() {
	updateScreen();
	removeObject();
}

void BackgroundTextualObject::removeObject() {
	GW2_GLOBALS._sceneItems.remove(this);

	if (GW2_GLOBALS._sceneManager._scene->_bgSceneObjects.contains(this))
		GW2_GLOBALS._sceneManager._scene->_bgSceneObjects.remove(this);

	clear();

	if (_flags & OBJFLAG_CLONED) 
		destroy();
}

void BackgroundTextualObject::reposition() {
	if (!_message.empty() || _resNum != -1 || _field90) {
		_bounds = _textBounds;
	} else {
		SceneObject::reposition();
	}

	_xs = _bounds.left;
	_xe = _bounds.right;
}

void BackgroundTextualObject::draw() {
	if (!_message.empty() || (_resNum != -1)) {
		GfxFontBackup font;
		GfxManager &gfxManager = g_globals->gfxManager();
		gfxManager._font.setFontNumber(this->_fontNumber);

		if (_resNum == -1) {
			gfxManager.getStringBounds(_message.c_str(), _textBounds, _maxWidth);
		} else {
			Common::String msg = g_resourceManager->getMessage(_resNum, _lineNum);
			gfxManager.getStringBounds(msg.c_str(), _textBounds, _maxWidth);
		}

		if (_textMode == ALIGN_CENTER) {
			_textBounds.center(_position.x, _position.y);
		} else {
			_textBounds.moveTo(_position.x, _position.y);
		}
	} else if (!_field90) {
		reposition();
		_textBounds = _bounds;
	}

	_textBounds.expandPanes();

	if (_field8E) {
		_textBounds.translate(-GW2_GLOBALS._sceneOffset.x, -GW2_GLOBALS._sceneOffset.y);
		SceneObject::draw();
		_textBounds.translate(GW2_GLOBALS._sceneOffset.x, GW2_GLOBALS._sceneOffset.y);
	}

	if (!_message.empty() || _resNum != -1 || (_field90 && _field92)) {
		_textBounds.translate(-GW2_GLOBALS._sceneOffset.x, -GW2_GLOBALS._sceneOffset.y);
		
		if (!_field90) {
			GfxFontBackup font;
			GfxManager &gfxManager = g_globals->gfxManager();
			gfxManager._font.setFontNumber(this->_fontNumber);

			gfxManager.setFillFlag(false);
			gfxManager._font._colors.foreground = _fontFgColour;
			gfxManager._font._colors2.background = _fontBgColour;
			gfxManager._font._colors2.foreground = _fontFgColour2;

			if (_resNum == -1) {
				gfxManager._font.writeLines(_message.c_str(), _textBounds, _textMode);
			} else {
				Common::String msg = g_resourceManager->getMessage(_resNum, _lineNum);
				gfxManager._font.writeLines(msg.c_str(), _textBounds, _textMode);
			}

			if (_textMode == ALIGN_CENTER) {
				_textBounds.center(_position.x, _position.y);
			} else {
				_textBounds.moveTo(_position.x, _position.y);
			}
		} else {
			reposition();
		}

		_textBounds.translate(GW2_GLOBALS._sceneOffset.x, GW2_GLOBALS._sceneOffset.y);
		reposition();
	} else if (!_field90 || _field92) {
		BackgroundSceneObject::draw();
	}
}

void BackgroundTextualObject::updateScreen() {
	_textBounds.translate(-GW2_GLOBALS._sceneOffset.x, -GW2_GLOBALS._sceneOffset.y);
	SceneObject::updateScreen();
	_textBounds.translate(GW2_GLOBALS._sceneOffset.x, GW2_GLOBALS._sceneOffset.y);

	if (GW2_GLOBALS._sceneManager._scene->_bgSceneObjects.contains(this))
		GW2_GLOBALS._sceneManager._scene->_bgSceneObjects.remove(this);

	clear();
}

void BackgroundTextualObject::setup(int visage, int strip, int frame, int xp, int yp, int priority, int v8e) {
	if (GW2_GLOBALS._sceneManager._scene->_bgSceneObjects.contains(this))
		GW2_GLOBALS._sceneManager._scene->_bgSceneObjects.remove(this);

	SceneObject::setup(visage, strip, frame, xp, yp, priority);
}

void BackgroundTextualObject::clear() {
	_field8E = _field90 = 0;
	_field92 = 1;
	_message.clear();
	_resNum = -1;
	_lineNum = 0;
	_textMode = ALIGN_LEFT;
}

} // End of namespace Geekwad2

} // End of namespace TsAGE
