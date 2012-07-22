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

#ifndef TSAGE_GEEKWAD2_LOGIC_H
#define TSAGE_GEEKWAD2_LOGIC_H

#include "common/scummsys.h"
#include "tsage/events.h"
#include "tsage/core.h"
#include "tsage/scenes.h"
#include "tsage/globals.h"

namespace TsAGE {

namespace Geekwad2 {

using namespace TsAGE;

class SceneFactory {
public:
	static Scene *createScene(int sceneNumber);
};

class SceneExt: public Scene {
private:
	bool _keyPressed[322];
public:
	SceneExt();

	virtual Common::String getClassName() { return "SceneExt"; }
	virtual void process(Event &event);

	bool isKeyPressed(Common::KeyCode keyCode);
};

class Geekwad2Game: public Game {
public:
	virtual void start();

	virtual Scene *createScene(int sceneNumber);
	virtual void processEvent(Event &event);
	virtual void rightClick();
	virtual bool canSaveGameStateCurrently();
	virtual bool canLoadGameStateCurrently();

	int showPauseDialog();
};

class SceneObject2: public SceneObject {
public:
	int _v1, _v2;

	SceneObject2() { _v1 = _v2 = 0; }
	virtual Common::String getClassName() { return "SceneObject2"; }
	virtual void synchronize(Serializer &s) {
		SceneObject::synchronize(s);
		s.syncAsSint16LE(_v1);
		s.syncAsSint16LE(_v2);
	}
};

class BackgroundTextualObject: public BackgroundSceneObject {
public:
	Common::String _message;
	Rect _textBounds;
	int _field8E, _field90, _field92;
	int _resNum, _lineNum;
	int _fontNumber;
	int _fontFgColour;
	int _fontBgColour;
	int _fontFgColour2;
	int _maxWidth;
	TextAlign _textMode;
public:
	BackgroundTextualObject();

	virtual void remove();
	virtual void removeObject();
	virtual void reposition();
	virtual void draw();
	virtual void updateScreen();

	void setup(int visage, int strip, int frame, int xp, int yp, int priority, int v8e);
	void clear();
};

} // End of namespace Geekwad2

} // End of namespace TsAGE

#endif
