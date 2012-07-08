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

#include "tsage/geekwad2/geekwad2_speakers.h"
#include "tsage/geekwad2/geekwad2_scenes0.h"
#include "tsage/scenes.h"
#include "tsage/tsage.h"
#include "tsage/graphics.h"
#include "tsage/staticres.h"

namespace TsAGE {

namespace Geekwad2 {

VisualSpeaker::VisualSpeaker(): Speaker() {
	_textWidth = 312;
	_color1 = GW2_GLOBALS._scenePalette._colors.background;
	_hideObjects = false;
	_removeObject1 = false;
	_removeObject2 = false;
	_removeObject3 = false;
	_removeObject4 = false;
	_removeObject5 = false;
	_fontNumber = GW2_GLOBALS._gfxFontNumber;

	_color2 = 0;
	_numFrames = 0;
}

void VisualSpeaker::remove() {
	if (GW2_GLOBALS._v492C6 && _removeObject1) {
		_object1.setFrame(1);
		GW2_GLOBALS._sceneObjects->draw();
	}

	if (_removeObject1) {
		_object1.remove();
		_removeObject1 = false;
	}
	if (_removeObject2) {
		_object2.remove();
		_removeObject2 = false;
	}
	if (_removeObject4) {
		_object4.remove();
		_removeObject4 = false;
	}
	if (_removeObject3) {
		_object3.remove();
		_removeObject3 = false;
	}
	if (_removeObject5) {
		_object5.remove();
		_removeObject5 = false;
	}

	Speaker::remove();
}

void VisualSpeaker::synchronize(Serializer &s) {
	Speaker::synchronize(s);

	s.syncAsByte(_removeObject1);
	s.syncAsByte(_removeObject2);
	s.syncAsByte(_removeObject3);
	s.syncAsByte(_removeObject4);
	s.syncAsByte(_removeObject5);
	s.syncAsSint16LE(_numFrames);
	s.syncAsSint16LE(_offsetPos.x);
	s.syncAsSint16LE(_offsetPos.y);
}

void VisualSpeaker::proc12(Action *action) {
	Speaker::proc12(action);

	_textPos = Common::Point(_offsetPos.x + BF_GLOBALS._sceneManager._scene->_sceneBounds.left,
		_offsetPos.y + BF_GLOBALS._sceneManager._scene->_sceneBounds.top);
	_numFrames = 0;
}

void VisualSpeaker::setText(const Common::String &msg) {
	// TODO: EventManager::waitEvent
	_objectList.draw();
	BF_GLOBALS._sceneObjects->draw();

	_sceneText._color1 = _color1;
	_sceneText._color2 = _color2;
	_sceneText._color3 = _color3;
	_sceneText._width = _textWidth;
	_sceneText._fontNumber = _fontNumber;
	_sceneText._textMode = _textMode;
	_sceneText.setup(msg);

	// Ensure the text is in the foreground
	_sceneText.fixPriority(256);

	// Set the number of frames
	int len = msg.size();
	_numFrames = MAX(1, len / 2);
	
	// Set on-screen delay
	if (_action)
		_action->setDelay(MIN(_numFrames * 20, 120));
}

/*--------------------------------------------------------------------------*/

void VisualSpeaker::Action1::signal() {
	switch (_actionIndex) {
	case 0:
		++_actionIndex;
		setDelay(GW2_GLOBALS._randomSource.getRandomNumber(70) + 30);
		static_cast<SceneObject *>(_owner)->animate(ANIM_MODE_NONE);
		break;

	case 1:
		++_actionIndex;
		static_cast<SceneObject *>(_owner)->animate(ANIM_MODE_5, this);
		break;

	case 2:
		_actionIndex = 0;
		setDelay(1);
		break;

	default:
		break;
	}
}

/*--------------------------------------------------------------------------*/

void VisualSpeaker::Action2::signal() {
	Object1 *obj = (Object1 *)_owner;

	switch (_actionIndex++) {
	case 0: {
		obj->animate(ANIM_MODE_7, obj->_v, this);
		break;
	}

	case 1:
		obj->setFrame(1);
		remove();
		break;

	default:
		break;
	}
}

/*--------------------------------------------------------------------------*/

void VisualSpeaker::Object1::synchronize(Serializer &s) {
	SceneObject::synchronize(s);

	s.syncAsSint16LE(_v);
}

/*--------------------------------------------------------------------------*/

SpeakerGameText::SpeakerGameText(): VisualSpeaker() {
	_speakerName = "GAMETEXT";
	_color1 = 8;
	_color2 = 19;
}

/*--------------------------------------------------------------------------*/

SpeakerGran::SpeakerGran(): VisualSpeaker() {
	_fontNumber = 31;
	_color1 = 23;
	_color2 = 15;
	_offsetPos = Common::Point(10, 40);
	_speakerName = "GRAN";
}

void SpeakerGran::proc12(Action *action) {
	VisualSpeaker::proc12(action);

	GW2_GLOBALS._scenePalette.loadPalette(8010);
	if (GW2_GLOBALS._sceneManager._hasPalette)
		GW2_GLOBALS._scenePalette.refresh();

	_removeObject1 = true;
	_object1.postInit();
	_object1.setVisage(8010);
	_object1.setStrip2(2);
	_object1.setFrame(1);
	_object1.fixPriority(205);
	_object1.setPosition(Common::Point(GW2_GLOBALS._sceneManager._scene->_sceneBounds.left + 226,
			GW2_GLOBALS._sceneManager._scene->_sceneBounds.top + 122));

	_removeObject2 = true;
	_object2.postInit();
	_object2.setVisage(8010);
	_object2.setStrip2(1);
	_object2.setFrame(1);
	_object2.fixPriority(199);
	_object2.setPosition(Common::Point(GW2_GLOBALS._sceneManager._scene->_sceneBounds.left + 237,
			GW2_GLOBALS._sceneManager._scene->_sceneBounds.top + 189));

	_removeObject4 = true;
	_object4.postInit();
	_object4.setVisage(8010);
	_object4.setStrip2(3);
	_object4.setFrame(_object4.getFrameCount());
	_object4.fixPriority(200);
	_object4.setPosition(Common::Point(GW2_GLOBALS._sceneManager._scene->_sceneBounds.left + 224,
			GW2_GLOBALS._sceneManager._scene->_sceneBounds.top + 96));
	_object4.setAction(&_action1);
	
	_removeObject3 = true;
	_object3.postInit();
	_object3.setVisage(8010);
	_object3.setStrip2(4);
	_object3.setFrame(3);
	_object3.fixPriority(205);
	_object3.setPosition(Common::Point(GW2_GLOBALS._sceneManager._scene->_sceneBounds.left + 225,
			GW2_GLOBALS._sceneManager._scene->_sceneBounds.top + 92));
}

void SpeakerGran::setText(const Common::String &msg) {
	int v = 0;
	int frameNumber;
	warning("TODO: object offset 2e8h");

	switch (v) {
	case 122:
	case 124:
	case 140:
	case 162:
	case 182:
	case 202:
	case 222:
	case 242:
		frameNumber = 1;
		break;

	case 104:
	case 132:
	case 152:
	case 174:
	case 194:
	case 214:
	case 232:
		frameNumber = 2;
		break;

	default:
		frameNumber = 3;
		break;
	}

	_object3.setFrame(frameNumber);
	VisualSpeaker::setText(msg);

	_object1._v = _numFrames;
	_object1.setAction(&_action3);
}

} // End of namespace Geekwad2

} // End of namespace TsAGE
