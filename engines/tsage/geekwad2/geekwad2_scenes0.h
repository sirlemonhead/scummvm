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

class Scene10: public Scene {
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
	public:
		virtual void signal();
	};

	/* Objects */
	class Object: public SceneObject {
	};

public:
	int _field30A, _field30C;
	Rect _bounds;
	StripCallback *_stripCallback;
	int _field31E;
	SpeakerGran _granSpeaker;
	Action1 _action1;
	Action2 _action2;
	int _field772;
	ASound _sound1, _sound2;
	SceneObject _object1, _object2, _object3, _object4;
	int _fieldEB0, _fieldEB2, _fieldEB4, _fieldEB6, _fieldEB8;
	int _fieldEBA, _fieldEBC, _fieldEBE, _fieldEC0;
	int _fieldEC2, _fieldEC4, _fieldEC6, _fieldEC8;
	int _fieldECA, _fieldECC;
	Rect _rect1, _rect2;
	SceneObject _object5;
	int _fieldF74;

	SceneObject *_objectP;
	Object _objList1[5];
	int _field127A[20];
	int _field128E[16];
	Object _objList2[16];
	int _field1C2E[64];

	Action6 _action6;
	Action7 _actionList1[16];
	Action8 _actionList2[16];
	int _field200A[26];

	BackgroundTextualObject _obj1, _obj2, _obj3, _obj4;
	BackgroundTextualObject _obj5, _obj6, _obj7;
	ASound _sound3, _sound4, _sound5;
	ASound _sound6, _sound7;
	int _field319A, _field319C;
	Action3 _action3;
	Action4 _action4[5];
	Action5 _action5;
};

class Scene150: public Scene {
	/* Actions */
	class Action1 : public Action {
	public:
		virtual void signal();
	};
public:
	Action1 _action1;
	ScenePalette _scenePalette;
	SceneObject _object1, _object2, _object3, _object4;
	SceneObject _object5, _object6, _object7, _object8;

	virtual void postInit(SceneObjectList *OwnerList = NULL);
};

class Scene200: public Scene {
	/* Actions */
	class Action1: public Action {
	public:
		virtual void signal();
	};
	class Action2: public Action {
	public:
		virtual void signal();
	};
public:
	Action1 _action1;
	Action2 _action2;
	ASound _sound1, _sound2;
	SceneObject _object1, _object2, _object3, _object4;
	int _field846, _field848;
	int _fieldAB2, _fieldAB4, _creditIndex;
	SynchronizedList<SceneText *> _list1;
	SynchronizedList<SceneObject2 *> _list2;

	Scene200();
	virtual void postInit(SceneObjectList *OwnerList = NULL);
	virtual void process(Event &event);
};

} // End of namespace Geekwad2

} // End of namespace TsAGE

#endif
