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

#ifndef TSAGE_GEEKWAD2_SCENES0_H
#define TSAGE_GEEKWAD2_SCENES0_H

#include "common/scummsys.h"
#include "tsage/converse.h"
#include "tsage/events.h"
#include "tsage/core.h"
#include "tsage/scenes.h"
#include "tsage/globals.h"
#include "tsage/sound.h"
#include "tsage/geekwad2/geekwad2_logic.h"
#include "tsage/geekwad2/geekwad2_speakers.h"

namespace TsAGE {

namespace Geekwad2 {

using namespace TsAGE;

class Scene20: public SceneExt {
	/* Actions */
	class Action1 : public Action {
	public:
		virtual void signal();
	};
	class Action2 : public Action {
	public:
		virtual void signal();
	};
	class Action3 : public Action {
	public:
		virtual void signal();
	};
	class Action4 : public Action {
	public:
		virtual void signal();
	};
	class Action5 : public Action {
	public:
		virtual void signal();
	};
	class Action6 : public Action {
	public:
		virtual void signal();
	};
	class Action7 : public Action {
	public:
		virtual void signal();
	};
	class Action8 : public Action {
	};
	class Action9 : public Action {
	};
	class Action10 : public Action {
	};

	/* Objects */
	class Object: public SceneObject {
	public:
		ASound _sound;

		int _field310;
		int _field312, _field314;
		int _field316, _field318;
		int _field31A, _field31C;
		int _field31E, _objIndex;
		int _field322, _field324;
		Common::Point _newPosition;
	};

private:
	void reset();
	void updateScore();

	static void timer();
public:
	int _field30A, _field30C, _field30E;
	Action1 _action1;
	SpeakerMurray20 _murraySpeaker;
	Action2 _action2;
	uint32 _frameNumber;
	int _field766, _field768;
	Rect _rect1;
	int _field776, _field778, _field77A, _field77C;
	int _field77E, _field780, _field782, _field784;
	ASound _sound1, _sound2, _sound3, _sound4;
	ASound _sound5, _sound6, _sound7;
	SceneObject _object1, _object2, _object3, _object4;
	Object _object5;
	SceneObject _object6;
	int _field1F04;
	Object _objList1[20];
	Object *_field5E4E[20];
	Object _object7;
	SceneObject *_objectP;
	Common::String _scoreBuffer;
	int _currentScore;
	BackgroundTextualObject _scoreValue, _scoreLabel, _bgObject3, _bgObject4;
	BackgroundTextualObject _bgObject5, _bgObject6, _bgObject7;
	int _field66D2;
	int _field66D4;
	int _field66D6;
	int _field66D8;
	int _field66DA;
	int _field66DC;
	int _field66DE;
	int _field66E0;
	Action3 _action3;
	Action4 _action4[20];
	Action5 _action5;
	Action6 _action6;
	Action7 _action7;
	Action8 _action8[20];
	Action9 _action9;
	Action10 _action10;
public:
	Scene20();

	virtual void postInit(SceneObjectList *OwnerList = NULL);
	virtual void remove();
	virtual void process(Event &event);
	virtual void dispatch();
};

} // End of namespace Geekwad2

} // End of namespace TsAGE

#endif
