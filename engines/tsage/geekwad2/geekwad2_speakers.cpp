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

#include "tsage/scenes.h"
#include "tsage/tsage.h"
#include "tsage/graphics.h"
#include "tsage/staticres.h"
#include "tsage/geekwad2/geekwad2_speakers.h"

namespace TsAGE {

namespace Geekwad2 {

VisualSpeaker::VisualSpeaker(): Speaker() {
	_textWidth = 312;
	_color1 = 19;
	_hideObjects = false;
	_removeObject1 = false;
	_removeObject2 = false;
	_xp = 160;
	_fontNumber = 4;
	_color2 = 82;
	_offsetPos = Common::Point(4, 170);
	_numFrames = 0;
}

void VisualSpeaker::remove() {
	if (_removeObject1) {
		_object1.setFrame(1);
		GW2_GLOBALS._sceneObjects->draw();

		_object1.remove();
		_removeObject1 = false;
	}

	if (_removeObject2) {
		_object2.remove();
		_removeObject2 = 0;
	}
	if (_removeObject4) {
		_object4.remove();
		_removeObject4 = 0;
	}
	if (_removeObject3) {
		_object3.remove();
		_removeObject3 = 0;
	}
	if (_removeObject5) {
		_object5.remove();
		_removeObject5 = 0;
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
	s.syncAsSint16LE(_xp);
	s.syncAsSint16LE(_numFrames);
	s.syncAsSint16LE(_offsetPos.x);
	s.syncAsSint16LE(_offsetPos.y);
}

void VisualSpeaker::proc12(Action *action) {
	Speaker::proc12(action);
	_textPos = Common::Point(_offsetPos.x + GW2_GLOBALS._sceneManager._scene->_sceneBounds.left,
		_offsetPos.y + GW2_GLOBALS._sceneManager._scene->_sceneBounds.top);
	_numFrames = 0;
}

void VisualSpeaker::setText(const Common::String &msg) {
	_objectList.draw();
	GW2_GLOBALS._sceneObjects->draw();

	_sceneText._color1 = _color1;
	_sceneText._color2 = _color2;
	_sceneText._color3 = _color3;
	_sceneText._width = _textWidth;
	_sceneText._fontNumber = _fontNumber;
	_sceneText._textMode = _textMode;
	_sceneText.setup(msg);

	// Get the string bounds
	GfxFont f;
	f.setFontNumber(_fontNumber);
	Rect bounds;
	f.getStringBounds(msg.c_str(), bounds, _textWidth);

	// Set the position for the text
	switch (_textMode) {
	case ALIGN_LEFT:
	case ALIGN_JUSTIFIED:
		_sceneText.setPosition(_textPos);
		break;
	case ALIGN_CENTER:
		_sceneText.setPosition(Common::Point(_textPos.x + (_textWidth - bounds.width()) / 2, _textPos.y));
		break;
	case ALIGN_RIGHT:
		_sceneText.setPosition(Common::Point(_textPos.x + _textWidth - bounds.width(), _textPos.y));
		break;
	default:
		break;
	}

	// Ensure the text is in the foreground
	_sceneText.fixPriority(256);

	// Count the number of words (by spaces) in the string
	const char *s = msg.c_str();
	int spaceCount = 0;
	while (*s) {
		if (*s++ == ' ')
			++spaceCount;
	}

	_numFrames = spaceCount * 3 + 2;
}

void VisualSpeaker::proc15() {
	if (_removeObject1) {
		_object1.setAction(NULL);
		_object1.animate(ANIM_MODE_NONE);
		_object1.setFrame(1);
	}

	if (_removeObject4) {
		_object4.setAction(&_action1);
		_object4.setFrame(_object4.getFrameCount());
	}
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
	switch (_actionIndex++) {
	case 0:
		warning("TODO: Verify owner points to an Object1 instance");
		static_cast<Object1 *>(_owner)->animate(ANIM_MODE_7, static_cast<Object1 *>(_owner)->_v, this);
		break;
	case 1:
		static_cast<SceneObject *>(_owner)->setFrame(1);
		remove();
		break;
	default:
		break;
	}
}

/*--------------------------------------------------------------------------*/

VisualSpeaker::Object1::Object1() {
	_v = 0;
}

void VisualSpeaker::Object1::synchronize(Serializer &s) {
	SceneObject::synchronize(s);
	s.syncAsSint16LE(_v);
}

/*--------------------------------------------------------------------------*/

SpeakerGameText::SpeakerGameText(): VisualSpeaker() {
	_speakerName = "GAMTEXT";
	_color1 = 8;
	_color2 = 19;
}

/*--------------------------------------------------------------------------*/

SpeakerSaddam::SpeakerSaddam(): VisualSpeaker() {
	_fontNumber = 31;
	_color1 = 23;
	_color2 = 15;
	_offsetPos = Common::Point(10, 40);
	_textWidth = 180;
	_speakerName = "SADDAM";
}
	
void SpeakerSaddam::proc12(Action *action) {
	VisualSpeaker::proc12(action);
	GW2_GLOBALS._scenePalette.loadPalette(8060);
	GW2_GLOBALS._scenePalette.refresh();

	_removeObject1 = true;
	_object1.postInit();
	_object1.setVisage(8060);
	_object1.setStrip2(2);
	_object1.fixPriority(205);
	_object1.setPosition(Common::Point(GW2_GLOBALS._sceneManager._scene->_sceneBounds.left + 196,
		_offsetPos.y + GW2_GLOBALS._sceneManager._scene->_sceneBounds.top + 134));

	_removeObject2 = true;
	_object2.postInit();
	_object2.setVisage(8060);
	_object2.setStrip2(1);
	_object2.fixPriority(199);
	_object1.setPosition(Common::Point(GW2_GLOBALS._sceneManager._scene->_sceneBounds.left + 237,
		_offsetPos.y + GW2_GLOBALS._sceneManager._scene->_sceneBounds.top + 157));

	_removeObject4 = true;
	_object4.postInit();
	_object4.setVisage(8060);
	_object4.setStrip2(3);
	_object4.setFrame(_object4.getFrameCount());
	_object4.fixPriority(200);
	_object1.setPosition(Common::Point(GW2_GLOBALS._sceneManager._scene->_sceneBounds.left + 210,
		_offsetPos.y + GW2_GLOBALS._sceneManager._scene->_sceneBounds.top + 97));

	_removeObject3 = true;
	_object3.postInit();
	_object3.setVisage(8060);
	_object3.setStrip2(4);
	_object3.fixPriority(205);
	_object1.setPosition(Common::Point(GW2_GLOBALS._sceneManager._scene->_sceneBounds.left + 212,
		_offsetPos.y + GW2_GLOBALS._sceneManager._scene->_sceneBounds.top + 95));
}

void SpeakerSaddam::setText(const Common::String &msg) {
	warning("TODO: Check where ax comes from");
	int v = 0;

	switch (v) {
	case 122:
	case 124:
	case 140:
	case 162:
	case 182:
	case 202:
	case 222:
	case 242:
		_object3.setFrame(1);
		break;
	case 104:
	case 132:
	case 152:
	case 174:
	case 194:
	case 214:
	case 232:
		_object3.setFrame(2);
		break;
	default:
		_object3.setFrame(3);
		break;
	}

	VisualSpeaker::setText(msg);
	_object1._v = _numFrames;
	_object1.setAction(&_action3);
}

/*--------------------------------------------------------------------------*/

SpeakerCyb60::SpeakerCyb60(): VisualSpeaker() {
	_fontNumber = 31;
	_color1 = 23;
	_color2 = 15;
	_offsetPos = Common::Point(130, 30);
	_textWidth = 180;
	_speakerName = "CYB";
}

void SpeakerCyb60::proc12(Action *action) {
	VisualSpeaker::proc12(action);

	_removeObject1 = true;
	_object1.postInit();
	_object1.setVisage(8025);
	_object1.setStrip2(1);
	_object1.fixPriority(205);
	_object1.setPosition(Common::Point(GLOBALS._sceneManager._scene->_sceneBounds.left + 81,
			GLOBALS._sceneManager._scene->_sceneBounds.top + 154));

	_removeObject4 = true;
	_object4.postInit();
	_object4.setVisage(8025);
	_object4.setStrip2(2);
	_object4.setFrame(_object4.getFrameCount());
	_object4.fixPriority(200);
	_object4.setPosition(Common::Point(GLOBALS._sceneManager._scene->_sceneBounds.left + 93,
			GLOBALS._sceneManager._scene->_sceneBounds.top + 98));
	_object4.setAction(&_action1);

	_removeObject3 = true;
	_object3.postInit();
	_object3.setVisage(8025);
	_object3.setStrip2(3);
	_object3.fixPriority(205);
	_object3.setFrame(_object3.getFrameCount());
	_object3.fixPriority(205);
	_object3.setPosition(Common::Point(GLOBALS._sceneManager._scene->_sceneBounds.left + 84,
			GLOBALS._sceneManager._scene->_sceneBounds.top + 89));
}

void SpeakerCyb60::setText(const Common::String &msg) {
	VisualSpeaker::setText(msg);
	_object1._v = _numFrames;
	_object1.setAction(&_action3);
}

} // End of namespace BlueForce

} // End of namespace TsAGE
