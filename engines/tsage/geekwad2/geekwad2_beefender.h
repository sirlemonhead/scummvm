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

#ifndef TSAGE_GEEKWAD2_BEEFENDER_H
#define TSAGE_GEEKWAD2_BEEFENDER_H

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

#define ARRAY_SIZE 5

class Scene30: public Scene {
	/* Actions */
	class Action1: public Action {
	public:
		virtual void signal();
	};
	class Action2: public Action {
	public:
		virtual void signal();
	};
	class Action3: public Action {
	public:
		virtual void signal();
	};
	class Action4: public Action {
	public:
		virtual void signal();
	};
	class Action5: public Action {
	public:
		virtual void signal();
	};
	class Action6: public Action {
	public:
		virtual void signal();
	};
	class Action7: public Action {
	public:
		virtual void signal();
	};

	/* Objects */
	class Object2: public SceneObject {
	public:
		SceneObject _object;
	};
	class Object3: public SceneObject {
	public:
		SceneObject _object;
	};
	class Object4: public SceneObject {
	public:
		SceneObject _object;
	};
public:
	int _field30A, _field318, _field31A, _field31C, _field31E;
	int _field1090[ARRAY_SIZE];
	int _field1694[ARRAY_SIZE];
	int _field1B6C[3];
	int _field1996[ARRAY_SIZE];
	Rect _rect1, _rect2;
	Action1 _action1;
	SpeakerSaddam _saddamSpeaker;
	Action2 _action2;
	Action3 _action3;
	SceneObject _object1, _object2;
	SceneObject _object3, _object4, _object5;
	Object2 _objectList1[ARRAY_SIZE];
	Object3 _objectList2[ARRAY_SIZE];
	Object4 _objectList3[ARRAY_SIZE], _objectList4[ARRAY_SIZE];
	Action2 _actionList1[ARRAY_SIZE];
	SceneObject _objectList5[ARRAY_SIZE];
	Action3 _actionList2[ARRAY_SIZE];
	SceneObjectB4 _object6, _object7, _object8, _object9;
	SceneObjectB4 _object10, _object11, _object12, _object13;
	SceneObjectB4 _object14, _object15, _object16;
	ASound _sound1, _sound2, _sound3, _sound4, _sound5, _sound6;
	ASound _sound7, _sound8, _sound9, _sound10, _sound11, _sound12;
	ASound _sound13, _sound14, _sound15, _sound16;
	ASound _soundList1[ARRAY_SIZE];
	Action6 _action6;
	Action7 _action7;
public:
	Scene30();

	virtual void postInit(SceneObjectList *OwnerList = NULL);
	virtual void synchronize(Serializer &s);
};

} // End of namespace Geekwad2

} // End of namespace TsAGE

#endif
