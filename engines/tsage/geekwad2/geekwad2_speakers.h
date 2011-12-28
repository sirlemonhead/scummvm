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

#ifndef TSAGE_GEEKWAD2_SPEAKERS_H
#define TSAGE_GEEKWAD2_SPEAKERS_H

#include "common/scummsys.h"
#include "tsage/converse.h"
#include "tsage/events.h"
#include "tsage/core.h"
#include "tsage/scenes.h"
#include "tsage/globals.h"
#include "tsage/geekwad2/geekwad2_logic.h"

namespace TsAGE {

namespace Geekwad2 {

using namespace TsAGE;

class VisualSpeaker: public Speaker {
	class Object1: public SceneObject {
	public:
		int _v;

		Object1();
		virtual void synchronize(Serializer &s);
	};
	class Action1: public Action {
	public:
		virtual void signal();
	};
	class Action2: public Action {
	public:
		virtual void signal();
	};
public:
	Object1 _object1;
	SceneObject _object2, _object3, _object4, _object5;
	bool _removeObject1, _removeObject2, _removeObject3;
	bool _removeObject4, _removeObject5;
	Action _action1, _action2;
	Action _action3;
	int _xp;
	int _numFrames;
	Common::Point _offsetPos;
public:
	VisualSpeaker();

	virtual Common::String getClassName() { return "VisualSpeaker"; }
	virtual void synchronize(Serializer &s);
	virtual void remove();
	virtual void proc12(Action *action);
	virtual void setText(const Common::String &msg);
	virtual void proc15();
};

class SpeakerGameText: public VisualSpeaker {
public:
	SpeakerGameText();

	virtual Common::String getClassName() { return "SpeakerGameText"; }
};


class SpeakerSaddam: public VisualSpeaker {
public:
	SpeakerSaddam();

	virtual void proc12(Action *action);
	virtual Common::String getClassName() { return "SpeakerSaddam"; }
	virtual void setText(const Common::String &msg);
};

class SpeakerCyb60: public VisualSpeaker {
public:
	SpeakerCyb60();

	virtual void proc12(Action *action);
	virtual Common::String getClassName() { return "SpeakerCyb60"; }
	virtual void setText(const Common::String &msg);
};

} // End of namespace Geekwad2

} // End of namespace TsAGE

#endif
