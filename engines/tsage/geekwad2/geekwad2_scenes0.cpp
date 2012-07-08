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
#include "tsage/staticres.h"
#include "tsage/geekwad2/geekwad2_scenes0.h"

namespace TsAGE {

namespace Geekwad2 {

/*--------------------------------------------------------------------------
 * Scene 10 - Spaced Solicitors
 *
 *--------------------------------------------------------------------------*/

void Scene10::Action1::signal() {
	Scene10 *scene = (Scene10 *)GW2_GLOBALS._sceneManager._scene;

	if (scene->_fieldEB2 == 1) {
		scene->_object3.remove();
		scene->_fieldEB2 = 0;

		switch (_actionIndex++) {
		case 0:
			GW2_GLOBALS._v4708C = 1;
			scene->_object2.hide();
			scene->_objectP->setAction(NULL);
			scene->_objectP->addMover(NULL);
			scene->_field31E = 1;

			switch (scene->_fieldEC0) {
			case 0:
				scene->_sound2.play(101, NULL, 100);
				GW2_GLOBALS._soundManager.setMasterVol(100);
				scene->_stripManager.start(100, this, scene->_stripCallback);
				break;
			case 1:
				scene->_sound2.play(110, NULL, 100);
				GW2_GLOBALS._soundManager.setMasterVol(100);
				scene->_stripManager.start(130, this, scene->_stripCallback);
				break;
			case 2:
				scene->_sound2.play(103, NULL, 90);
				GW2_GLOBALS._soundManager.setMasterVol(100);
				scene->_stripManager.start(150, this, scene->_stripCallback);
				break;
			case 3:
				scene->_sound2.play(104, NULL, 100);
				GW2_GLOBALS._soundManager.setMasterVol(100);
				scene->_stripManager.start(170, this, scene->_stripCallback);
				break;
			case 4:
				scene->_sound2.play(105, NULL, 100);
				GW2_GLOBALS._soundManager.setMasterVol(100);
				scene->_stripManager.start(190, this, scene->_stripCallback);
				break;
			case 5:
				scene->_sound2.play(106, NULL, 100);
				GW2_GLOBALS._soundManager.setMasterVol(100);
				scene->_stripManager.start(210, this, scene->_stripCallback);
				break;
			default:
				scene->_sound2.play(107, NULL, 100);
				GW2_GLOBALS._soundManager.setMasterVol(100);
				scene->_stripManager.start(230, this, scene->_stripCallback);
				break;
			}		

		case 1:
			scene->_field31E = 0;
			scene->_object2.show();
			setAction(&scene->_action5);
			break;

		default:
			break;
		}
	}
}

void Scene10::Action2::signal() {
	Scene10 *scene = (Scene10 *)GW2_GLOBALS._sceneManager._scene;

	scene->_fieldEBC = 0;
	if (scene->_fieldEB2 == 1) {
		scene->_object3.remove();
		scene->_fieldEB2 = 0;
	}

	switch (_actionIndex) {
	case 0:
		++_actionIndex;
		GW2_GLOBALS._v4708C = 1;
		scene->_field31E = 1;
		scene->_sound2.stop();
		//TODO

	case 1:
		++_actionIndex;
		setDelay(1);
		break;

	// TODO:

	default:
		break;
	}
}

void Scene10::Action3::signal() {
	Scene10 *scene = (Scene10 *)GW2_GLOBALS._sceneManager._scene;

	switch (_actionIndex++) {
	case 0:
		scene->_sound4.play(112, NULL);
		scene->_object2._strip = 7;
		scene->_object2._frame = 1;
		scene->_object2._numFrames = 5;
		scene->_object2.animate(ANIM_MODE_5, this);
		break;
	
	case 1:
		scene->_object2._numFrames = 10;
		scene->_object2._strip = 3;
		scene->_object2._frame = 1;
		remove();
		break;

	default:
		break;
	}
}

void Scene10::Action4::signal() {
	Scene10 *scene = (Scene10 *)GW2_GLOBALS._sceneManager._scene;

	switch (_actionIndex++) {
	case 0: {
		BackgroundTextualObject *obj = (BackgroundTextualObject *)_owner;
		obj->setVisage(150);
		obj->_frame = 1;
		obj->animate(ANIM_MODE_5, this);
		break;
	}

	case 1: {
		BackgroundTextualObject *obj = (BackgroundTextualObject *)_owner;
		obj->remove();
		scene->_field127A[obj->_field92];
		break;
	}

	default:
		break;
	}
}

void Scene10::Action5::signal() {
	Scene10 *scene = (Scene10 *)GW2_GLOBALS._sceneManager._scene;
	int strip, stripNum;

	switch (_actionIndex++) {
	case 0:
		GW2_GLOBALS._v4708C = 0;
		if (scene->_fieldEB2 == 1) {
			scene->_object3.remove();
			scene->_fieldEB2 = 0;
		}
		
		scene->_fieldECC = 1;
		if (scene->_fieldEC0 >= 6)
			scene->_fieldEC4 = 1;

		scene->_field319A -= 2;
		if (scene->_field319A < 0)
			scene->_field319A = 0;

		scene->_fieldEB0 = 100;
		if ((scene->_fieldEC2 + 5) >= 105) {
			scene->_fieldEC2 = 105;
		} else {
			scene->_fieldEC2 += 5;
		}

		if ((scene->_fieldEC8 + 25) >= 600) {
			scene->_fieldEC8 = 600;
		} else {
			scene->_fieldEC8 = 25;
		}

		if ((scene->_fieldECA - 1) > 0)
			--scene->_fieldECA;
		
		for (int idx = 0; idx < 5; ++idx) {
			if (scene->_field127A[idx]) {
				scene->_field127A[idx]->remove();
				scene->_field127A[idx] = NULL;
			}
		}

		scene->_objectP->setVisage(110);
		GW2_GLOBALS._soundManager.setMasterVol(100);

		strip = 0;
		stripNum = scene->_fieldEC0 / 2;
		if (stripNum > 5) {
			strip = 7;
			scene->_objectP->setStrip(7);
		} else {
			switch (stripNum) {
			case 0:
				strip = 1;
				break;
			case 1:
				strip = 4;
				break;
			case 2:
				strip = 2;
				break;
			case 3:
				strip = 5;
				break;
			case 4:
				strip = 3;
				break;
			case 5:
				strip = 6;
				break;
			default:
				break;
			}

			scene->_objectP->setStrip(stripNum + 1);
		}

		scene->_obj2.remove();
		scene->_obj2.setup(115, strip, 4, 250, 1, 130, 1);
		scene->_obj2.draw();
		
		scene->_objectP->setFrame2(1);
		scene->_objectP->setPosition(Common::Point(0, 23));
		scene->_objectP->_field8E = 0;
		scene->_objectP->show();
		scene->_objectP->_moveDiff.x = 20;
		ADD_MOVER((*scene->_objectP), 160, 23);
		break;

	case 1:
		scene->_objectP->setFrame2(2);
		setDelay(30);
		break;

	case 2:
		scene->_objectP->setFrame2(1);
		ADD_MOVER((*scene->_objectP), 350, 23);

		for (int idx = scene->_fieldEB4; idx <= scene->_fieldEB6; ++idx) {
			for (int idx2 = 0; idx2 < 4; ++idx2) {
				Object *obj = scene->_field1C2E[idx2][idx];
				obj->postInit();
				scene->_field128E[idx2][idx] = 1;
				obj->setVisage(140);
				obj->animate(ANIM_MODE_2, NULL);

				stripNum = scene->_fieldEC0 / 2;
				strip = (stripNum <= 5) ? stripNum : GW2_GLOBALS._randomSource.getRandomNumber(5);
				obj->setStrip(strip);
				obj->setFrame(1);
				obj->changeZoom(-1);
				obj->fixPriority(100);
				obj->setPosition(Common::Point(160, 23));
				obj->_moveDiff = Common::Point(30, 30);
				obj->_field90 = 0;
				obj->setAction(NULL);
			}
		}
		
		ADD_MOVER_NULL((*scene->_field1C2E[0][0]), 60, 120);
		ADD_MOVER_NULL((*scene->_field1C2E[0][1]), 60, 90);
		ADD_MOVER_NULL((*scene->_field1C2E[0][2]), 60, 60);
		ADD_MOVER_NULL((*scene->_field1C2E[0][3]), 60, 30);
		ADD_MOVER_NULL((*scene->_field1C2E[1][0]), 130, 120);
		ADD_MOVER_NULL((*scene->_field1C2E[1][1]), 130, 90);
		ADD_MOVER_NULL((*scene->_field1C2E[1][2]), 130, 60);
		ADD_MOVER_NULL((*scene->_field1C2E[1][3]), 130, 30);
		ADD_MOVER_NULL((*scene->_field1C2E[2][0]), 190, 120);
		ADD_MOVER_NULL((*scene->_field1C2E[2][1]), 190, 90);
		ADD_MOVER_NULL((*scene->_field1C2E[2][2]), 190, 60);
		ADD_MOVER_NULL((*scene->_field1C2E[2][3]), 190, 30);
		ADD_MOVER_NULL((*scene->_field1C2E[3][0]), 260, 120);
		ADD_MOVER_NULL((*scene->_field1C2E[3][1]), 260, 90);
		ADD_MOVER_NULL((*scene->_field1C2E[3][2]), 260, 60);
		ADD_MOVER_NULL((*scene->_field1C2E[3][3]), 260, 30);
		
		scene->_field319C = 16;
		break;

	case 3:
		scene->_objectP->hide();
		scene->_objectP->_field8E = 1;
		scene->_field31E = 0;
		scene->_fieldEBC = 1;
		scene->_field772 = 0;
		scene->_objectP->setAction(&scene->_action6);

		remove();
		break;
	}
}

void Scene10::Action6::signal() {
	Scene10 *scene = (Scene10 *)GW2_GLOBALS._sceneManager._scene;

	switch (_actionIndex) {
	case 0:
		_actionIndex = 1;
		setDelay(GW2_GLOBALS._randomSource.getRandomNumber(30 * scene->_fieldECA));
		break;

	case 1:
		if (scene->_fieldEBC == 1) {
			scene->_objectP->setVisage(110);
			int v = scene->_fieldEC0 / 2;
			scene->_objectP->setStrip((v <= 5) ? v + 1 : 7);
			scene->_objectP->_field8E = 0;
			scene->_objectP->_moveDiff.x = 4;
			scene->_objectP->show();

			_actionIndex = 2;
			if (GW2_GLOBALS._randomSource.getRandomNumber(1) == 1) {
				scene->_objectP->setPosition(Common::Point(330, 23));
				scene->_objectP->setFrame2(3);
			} else {
				scene->_objectP->setPosition(Common::Point(0, 23));
				scene->_objectP->setFrame2(1);
			}
			ADD_MOVER((*scene->_objectP), 160, 23);
		} else {
			setDelay(1);
		}
		break;

	case 2:
		if ((scene->_field319C > 0) && (scene->_fieldEBC == 1) && (scene->_objectP->_field8E == 0) &&
				(scene->_fieldEC4 == 1)) {
			bool flag = false;
			int idx;
			for (idx = 0; idx < 4; ++idx) {
				if (!scene->_field128E[0][0] && !scene->_field128E[1][0] &&
					!scene->_field128E[2][0] && !scene->_field128E[3][0]) {
					flag = true;
					break;
				}
			}

			if (flag && !scene->_field1C2E[0][idx] && !scene->_field1C2E[1][idx] 
					&& !scene->_field1C2E[2][idx] && !scene->_field1C2E[3][idx]) {
				// Increment frame					
				scene->_objectP->setFrame2(scene->_objectP->_frame + 1);

				// Loop
				for (int idx = scene->_fieldEB4; idx <= scene->_fieldEB6; ++idx) {
					for (int idx2 = 0; idx2 < 4; ++idx) {
						Object *obj = scene->_field1C2E[idx][idx2];

						if (scene->_field128E[idx][idx2] && obj->_position.y < 170) {
							scene->_field128E[idx][idx2] = 1;

							obj->postInit();
							obj->setVisage(140);
							obj->animate(ANIM_MODE_2, NULL);
							
							int strip = scene->_fieldEC0 / 2;
							obj->setStrip((strip <= 5) ? strip : GW2_GLOBALS._randomSource.getRandomNumber(5));

							obj->setFrame(1);
							obj->changeZoom(-1);
							obj->fixPriority(100);
							obj->setPosition(Common::Point(obj->_position.x, 23));
							obj->_moveDiff = Common::Point(30, 30);
							obj->_field90 = 0;
							obj->setAction(NULL);
							obj->reposition();

							int v = 0; // TODO: Decode floating point logic here
							if (v) {
								GW2_GLOBALS._soundManager.setMasterVol(GW2_GLOBALS._soundManager.getMasterVol() - 20);
							}

							break;
						}
					}
				}
			}	

			setDelay(1);
			_actionIndex = 3;
		}
		break;

	case 3:
		if (scene->_fieldEBC == 1) {
			switch (scene->_objectP->_frame) {
			case 1:
			case 2:
				scene->_objectP->setFrame2(1);
				ADD_MOVER((*scene->_objectP), 340, 23);
				break;
			case 3:
			case 4:
				scene->_objectP->setFrame2(3);
				ADD_MOVER((*scene->_objectP), 0, 23);
				break;
			default:
				break;
			}		
		} else {
			setDelay(1);
		}
		break;

	case 4:
		if (scene->_fieldEBC == 1) {
			scene->_objectP->hide();
			scene->_objectP->_field8E = 1;
		}

		_actionIndex = 0;
		setDelay(1);
		break;

	default:
		break;
	}
}

void Scene10::Action7::signal() {
	Scene10 *scene = (Scene10 *)GW2_GLOBALS._sceneManager._scene;

	switch (_actionIndex) {
	case 0:
		setDelay(GW2_GLOBALS._randomSource.getRandomNumber(240));
		break;

	case 1: {
		SceneObject *obj = static_cast<SceneObject *>(_owner);
		obj->setVisage(130);
		obj->_frame = 1;
		obj->animate(ANIM_MODE_NONE, NULL);
		
		_actionIndex = 2;
		setDelay(60);
		break;
	}

	case 2: {
		for (int idx = 0; idx < 4; ++idx) {
			if (!scene->_field127A[idx]	&& GW2_GLOBALS._sceneObjects->contains(scene->_field127A[idx])) {
				SceneObject *owner = static_cast<SceneObject *>(_owner);
				Object *obj = scene->_field127A[idx] = &scene->_objList1[idx];
				obj->postInit();
				obj->setVisage(120);
				obj->setStrip(owner->_strip);
				obj->setFrame(1);
				obj->fixPriority(250);
				obj->changeZoom(10);

				obj->_field92 = idx;
				obj->setAction(NULL);
				obj->setPosition(Common::Point(owner->_position.x + 5, owner->_position.y - 10));
				obj->animate(ANIM_MODE_2, NULL);
				
				owner->_frame = 2;
				break;
			}
		}

		_actionIndex = 3;
		setDelay(60);
		break;
	}

	case 3: {
		SceneObject *owner = static_cast<SceneObject *>(_owner);
		owner->setVisage(140);
		owner->_frame = 1;
		owner->animate(ANIM_MODE_2, NULL);
		
		_actionIndex = 0;
		setDelay(1);
		break;
	}

	default:
		break;
	}
}

/*--------------------------------------------------------------------------*/

Scene10::Scene10(): Scene() {
	_field30A = 0;
	_stripCallback = NULL;
	_field30C = 0;
	_field772 = 0;
	_fieldEB2 = 0;
	_fieldEB4 = 3;
	_fieldEB6  = 0;
 
	for (int idx = _fieldEB6; idx < _fieldEB4; ++idx) {
		for (int idx2 = 0; idx2 < 4; ++idx) {
			_field1C2E[idx][idx2] = &_objList2[idx][idx2];
			_field128E[idx][idx2] = 1;
		}
	}
	
	for (int idx = 0; idx < 4; ++idx)
		_field127A[idx] = 0;

	_rect1.set(50, 200, 270, 200);

	GW2_GLOBALS._events.addTimer(&timer, 70);
}

Scene10::~Scene10() {
	GW2_GLOBALS._events.removeTimer(&timer);
}

void Scene10::postInit(SceneObjectList *OwnerList) {
	Scene::postInit();
	GW2_GLOBALS._events.setCursor((CursorType)1);

	setZoomPercents(20, 1, 30, 100);
	loadScene(10);

	_objectP = &_object5;
	_objectP->postInit();
	_objectP->setVisage(110);
	_objectP->setStrip(1);
	_objectP->setFrame(1);
	_objectP->setPriority(90);
	_objectP->hide();

	_object2.postInit();
	_object2.setVisage(100);
	_object2.setStrip(3);
	_object2.setFrame(1);
	_object2.fixPriority(250);
	_object2.setPosition(Common::Point(50, 160));
	_object2.hide();

	_object4.postInit();
	_object4.setVisage(160);
	_object4.setStrip(1);
	_object4.setFrame(_object4.getFrameCount());
	_object4.fixPriority(90);
	_object4.setPosition(Common::Point(1, 1));
	
	_stripManager.addSpeaker(&_granSpeaker);
	_stripManager.addSpeaker(&GW2_GLOBALS._gameTextSpeaker);

	reset();
}

void Scene10::reset() {
	_field31E = 1;
	_fieldEC0 = -1;
	_fieldEBC = 0;
	_fieldECC = 1;
	_field319A = 20;
	_fieldEC2 = 10;
	_fieldEC4 = 0;
	_fieldEC6 = 3;

	_field2022 = 0;
	_fieldEC8 = 100;
	_fieldECA = 20;
	_field319C = 0;
	_score = 0;
	
	setupScore();

	_obj3.remove();
	_obj3.setup(100, 6, 4, 40, 13, 130, 1);
	_obj3.draw();

	_obj4.remove();
	_obj4.setup(100, 6, 4, 50, 13, 130, 1);
	_obj4.draw();

	_obj5.remove();
	_obj6.remove();
	_obj7.remove();
	_obj2.remove();

	setupAction();
}

void Scene10::setupScore() {
	if (_score) {
		_numberBuffer = Common::String::format("%ld", _score);
	} else {
		_numberBuffer = "0";
	}

	if (_bgSceneObjects.contains(&_obj1))
		_bgSceneObjects.remove(&_obj1);

	_obj1._maxWidth = 180;
	_obj1._fontNumber = 11;
	_obj1._fontFgColour = GW2_GLOBALS._scenePalette._colors.background;
	_obj1._fontBgColour = GW2_GLOBALS._scenePalette._colors.background;
	_obj1._fontFgColour2 = 0;
	_obj1._textMode = ALIGN_CENTER;
	_obj1._message = _numberBuffer;
	_obj1.setup(0, 0, 0, 157, 15, 156, 1);
	_obj1.draw();
}

void Scene10::setupAction() {
	++_fieldEC0;
	if (_fieldEB2 == 1) {
		_object3.remove();
		_fieldEB2 = 0;
	}

	if ((_fieldEC0 % 2) == 0)
		setAction(&_action1);
	else
		setAction(&_action5);	
}

void Scene10::timer() {

}

/*--------------------------------------------------------------------------
 * Scene 150 - Tsunami Title Screen
 *
 *--------------------------------------------------------------------------*/

void Scene150::Action1::signal() {
	Scene150 *scene = (Scene150 *)GW2_GLOBALS._sceneManager._scene;
	static byte black[3] = { 0, 0, 0 };

	switch (_actionIndex++) {
	case 0:
		setDelay(2);
		break;
	case 1:
		GW2_GLOBALS._sound1.play(1);
		GW2_GLOBALS._scenePalette.addRotation(64, 127, -1, 1, this);
		break;
	case 2:
		scene->_object1.setVisage(822);
		scene->_object1._strip = 1;
		scene->_object1._frame = 1;
		scene->_object1.changeZoom(100);

		scene->_object2.setVisage(822);
		scene->_object2._strip = 2;
		scene->_object2._frame = 1;
		scene->_object2.changeZoom(100);

		scene->_object3.setVisage(822);
		scene->_object3._strip = 3;
		scene->_object3._frame = 1;
		scene->_object3.changeZoom(100);

		scene->_object4.setVisage(822);
		scene->_object4._strip = 4;
		scene->_object4._frame = 1;
		scene->_object4.changeZoom(100);

		scene->_object5.setVisage(822);
		scene->_object5._strip = 5;
		scene->_object5._frame = 1;
		scene->_object5.changeZoom(100);

		scene->_object6.setVisage(822);
		scene->_object6._strip = 6;
		scene->_object6._frame = 1;
		scene->_object6.changeZoom(100);

		scene->_object7.setVisage(822);
		scene->_object7._strip = 7;
		scene->_object7._frame = 1;
		scene->_object7.changeZoom(100);

		scene->_object8.setVisage(822);
		scene->_object8._strip = 8;
		scene->_object8._frame = 1;
		scene->_object8.changeZoom(100);

		setDelay(1);
		break;
	case 3:
		GW2_GLOBALS._scenePalette.addFader(scene->_scenePalette._palette, 256, 8, this);
		break;
	case 4:
		setDelay(60);
		break;
	case 5:
		scene->_object2.animate(ANIM_MODE_5, NULL);
		scene->_object3.animate(ANIM_MODE_5, NULL);
		scene->_object4.animate(ANIM_MODE_5, NULL);
		scene->_object5.animate(ANIM_MODE_5, NULL);
		scene->_object6.animate(ANIM_MODE_5, NULL);
		scene->_object7.animate(ANIM_MODE_5, this);
		break;
	case 6:
		setDelay(120);
		break;
	case 7:
		GW2_GLOBALS._scenePalette.addFader(black, 1, 5, this);
		break;
	case 8:
		GW2_GLOBALS._sceneManager.changeScene(200);
		remove();
		break;
	}
}

/*--------------------------------------------------------------------------*/

void Scene150::postInit(SceneObjectList *OwnerList) {
	loadScene(820);
	Scene::postInit();
	setZoomPercents(60, 85, 200, 100);

	_scenePalette.loadPalette(822);

	_object1.postInit();
	_object1.setVisage(821);
	_object1._strip = 1;
	_object1._frame = 1;
	_object1.animate(ANIM_MODE_NONE, NULL);
	_object1.setPosition(Common::Point(62, 85));

	_object2.postInit();
	_object2.setVisage(821);
	_object2._strip = 2;
	_object2._frame = 1;
	_object2.animate(ANIM_MODE_NONE, NULL);
	_object2.setPosition(Common::Point(27, 94));

	_object3.postInit();
	_object3.setVisage(821);
	_object3._strip = 2;
	_object3._frame = 2;
	_object3.animate(ANIM_MODE_NONE, NULL);
	_object3.setPosition(Common::Point(68, 94));

	_object4.postInit();
	_object4.setVisage(821);
	_object4._strip = 2;
	_object4._frame = 3;
	_object4.animate(ANIM_MODE_NONE, NULL);
	_object4.setPosition(Common::Point(110, 94));

	_object5.postInit();
	_object5.setVisage(821);
	_object5._strip = 2;
	_object5._frame = 4;
	_object5.animate(ANIM_MODE_NONE, NULL);
	_object5.setPosition(Common::Point(154, 94));

	_object6.postInit();
	_object6.setVisage(821);
	_object6._strip = 2;
	_object6._frame = 5;
	_object6.animate(ANIM_MODE_NONE, NULL);
	_object6.setPosition(Common::Point(199, 94));

	_object7.postInit();
	_object7.setVisage(821);
	_object7._strip = 2;
	_object7._frame = 6;
	_object7.animate(ANIM_MODE_NONE, NULL);
	_object7.setPosition(Common::Point(244, 94));

	_object8.postInit();
	_object8.setVisage(821);
	_object8._strip = 2;
	_object8._frame = 7;
	_object8.animate(ANIM_MODE_NONE, NULL);
	_object8.setPosition(Common::Point(286, 94));

	setAction(&_action1);
}

/*--------------------------------------------------------------------------
 * Scene 200 - Credits screen
 *
 *--------------------------------------------------------------------------*/

struct CreditEntry {
	int visage;
	int strip;
	int frame;
};

const CreditEntry credits[] = {
	{5500, 1, 1}, {5500, 1, 2}, {5500, 2, 1}, {5500, 2, 2}, {5500, 8, 2}, {5500, 3, 1}, 
	{5500, 3, 2}, {5500, 4, 1}, {5500, 4, 2}, {5500, 5, 1}, {5500, 5, 2}, {5500, 8, 2}, 
	{5500, 6, 1}, {5500, 6, 2}, {5500, 7, 1}, {5500, 7, 2}, {5500, 8, 1}
};
#define TOTAL_CREDITS 17

void Scene200::Action1::signal() {
	Scene200 *scene = (Scene200 *)GW2_GLOBALS._sceneManager._scene;

	switch (_actionIndex) {
	case 0: {
		++scene->_fieldAB2;
		
		if ((scene->_creditIndex < TOTAL_CREDITS) && (!scene->_creditIndex || (*scene->_list2.begin())->_position.y < 192)) {
			SceneObject2 *obj = new SceneObject2();
			obj->postInit();
			obj->setVisage(credits[scene->_creditIndex].visage);
			obj->setStrip(credits[scene->_creditIndex].strip);
			obj->setFrame(credits[scene->_creditIndex].frame);
			obj->changeZoom(106);
			obj->fixPriority(100);
			obj->setPosition(Common::Point(160, 210));
			obj->_flags |= OBJFLAG_CLONED;
			obj->_v1 = obj->_v2 = 0;

			scene->_list2.push_back(obj);
			++scene->_creditIndex;
		}

		SceneObject2 *item;
		SynchronizedList<SceneObject2 *>::iterator i;
		for (i = scene->_list2.begin(); i != scene->_list2.end(); ++i) {
			item = *i;
			item->setPosition(Common::Point(item->_position.x, item->_position.y - 1));
			item->changeZoom(item->_percent - ((210 - item->_position.y + 100) / 100));
		}

		item = *scene->_list2.begin();
		if (item->_percent < 20) {
			item->remove();
			scene->_list2.remove(item);

			if (scene->_list2.empty()) {
				++_actionIndex;
				scene->_sound1.fadeOut(this);
			}
		}

		setDelay(6);
		break;
	}
	case 1:
		setDelay(1);
		++_actionIndex;
		break;
	case 2:
		scene->_sound1.play(3);
		scene->_sound1.setVol(127);

		scene->_object1.postInit();
		scene->_object1.setVisage(5510);
		scene->_object1.setStrip(1);
		scene->_object1.setFrame(1);
		scene->_object1.changeZoom(10);
		scene->_object1.fixPriority(100);
		scene->_object1.setPosition(Common::Point(160, 190));
	
		setDelay(3);
		++_actionIndex;
		break;
	case 3:
		scene->_object1.changeZoom(scene->_object1._percent + 5);
		if (scene->_object1._percent >= 100)
			++_actionIndex;

		setDelay(3);
		break;
	case 4:
		scene->_object2.postInit();
		scene->_object2.setVisage(5000);
		scene->_object2.setStrip(1);
		scene->_object2.setFrame(1);
		scene->_object2.fixPriority(110);
		scene->_object2.setPosition(Common::Point(-30, 100));
		scene->_object2.animate(ANIM_MODE_2);

		scene->_object3.postInit();
		scene->_object3.setVisage(5505);
		scene->_object3.setStrip(1);
		scene->_object3.setFrame(1);
		scene->_object3.fixPriority(100);
		scene->_object3.setPosition(Common::Point(412, 117));

		scene->_object4.postInit();
		scene->_object4.setVisage(5505);
		scene->_object4.setStrip(2);
		scene->_object4.setFrame(1);
		scene->_object4.fixPriority(101);
		scene->_object4.setPosition(Common::Point(scene->_object3._position.x + 35, 
			scene->_object3._position.y - 19));

		++_actionIndex;
		setDelay(6);
		break;
	case 5:
		scene->_creditIndex = 0;
		scene->_object2.setPosition(Common::Point(scene->_object2._position.x +12, scene->_object2._position.y));

		if (scene->_object2._position.x > 240)
			++_actionIndex;
		break;
	case 6:
		++scene->_creditIndex;
		if ((scene->_creditIndex % 2) != 0) {
			scene->_object2.setPosition(Common::Point(scene->_object2._position.x - 12, scene->_object2._position.y));
			scene->_sound2.play(611);
		} else {
			scene->_object2.setPosition(Common::Point(scene->_object2._position.x + 12, scene->_object2._position.y));
		}

		if (scene->_creditIndex >= 6)
			++_actionIndex;
		break;
	case 7:
		scene->loadBackground(8, 0);
		scene->_object2.setPosition(Common::Point(scene->_object2._position.x, scene->_object2._position.y + 2));
		scene->_object1.setPosition(Common::Point(scene->_object1._position.x - 12, scene->_object1._position.y));

		if (scene->_sceneBounds.left >= 160) {
			scene->_object1.remove();
			++_actionIndex;
		}
		break;
	case 8:
		scene->_object2.setPosition(Common::Point(scene->_object2._position.x + 12, scene->_object2._position.y));

		if (scene->_object2._position.x > 321)
			++_actionIndex;
		break;
	case 9:
		scene->_sound2.play(611);
		scene->_object2.setPosition(Common::Point(scene->_object2._position.x - 12, scene->_object2._position.y));
		scene->_object3.setPosition(Common::Point(scene->_object3._position.x + 12, scene->_object3._position.y));
		scene->_object4.setPosition(Common::Point(scene->_object4._position.x + 12, scene->_object2._position.y));
		scene->_object4.setFrame(scene->_object4.getFrameCount());
		
		setDelay(6);
		++_actionIndex;
		break;
	case 10:
		scene->_object3.setPosition(Common::Point(scene->_object3._position.x - 12, scene->_object3._position.y));
		scene->_object4.setPosition(Common::Point(scene->_object4._position.x - 12, scene->_object2._position.y));
		scene->_object4.setAction(&scene->_action2);
		scene->_object2.setPosition(Common::Point(scene->_object2._position.x - 12, scene->_object2._position.y));
		scene->_object2.fixPriority(80);
		
		setDelay(6);
		++_actionIndex;
		break;
	case 11:
		scene->_object2.changeZoom(scene->_object2._percent - 5);
		if (scene->_object2._percent < 20)
			++_actionIndex;
		break;
	case 12:
		scene->_object2.setPosition(Common::Point(scene->_object2._position.x + 2,
			scene->_object2._position.y - ((scene->_object2._position.y <= 80) ? 0 : 2)));
		if (scene->_object2._position.x > 390)
			++_actionIndex;
		break;
	case 13:
		scene->_field846 = GW2_GLOBALS._events.getFrameNumber() - scene->_field846;
		scene->_field848 = (scene->_field846 > 3600) ? 2 : 1;
		scene->_creditIndex = 0;
		++_actionIndex;
		// Deliberate fall-through
	case 14: {
		setDelay(2);

		if ((scene->_creditIndex != 0) && ((scene->_creditIndex == -1) || ((*scene->_list1.begin())->_position.y >= 142))) {
			Common::String msg = g_resourceManager->getMessage(200, scene->_creditIndex);
			
			if (msg.hasPrefix("~")) {
				scene->_creditIndex = -1;
			} else {
				if (msg.empty())
					msg = " ";
			
				SceneText *sceneText = new SceneText();
				sceneText->_fontNumber = 71;

				if (msg.hasPrefix("@")) {
					sceneText->_color1 = 43;
					sceneText->_color2 = 46;
					sceneText->_color3 = 80;
					msg.deleteChar(0);
				} else {
					sceneText->_color1 = 100;
					sceneText->_color2 = 103;
					sceneText->_color3 = 16;
				}

				sceneText->_width = 400;
				sceneText->setup(msg);
				sceneText->_flags |= OBJFLAG_CLONED;
				sceneText->fixPriority(199);

				int fontNumber = GW2_GLOBALS.gfxManager()._font._fontNumber;
				GW2_GLOBALS.gfxManager()._font.setFontNumber(sceneText->_fontNumber);
				int strWidth = GW2_GLOBALS.gfxManager().getStringWidth(msg.c_str());

				sceneText->setPosition(Common::Point(270 - (strWidth / 2), 150));
				GW2_GLOBALS.gfxManager()._font.setFontNumber(fontNumber);
				
				scene->_list1.push_back(sceneText);
			}
		}

		SynchronizedList<SceneText *>::iterator i;
		for (i = scene->_list1.begin(); i != scene->_list1.end(); ++i) {
			SceneText *item = *i;
			item->setPosition(Common::Point(item->_position.x, item->_position.y - scene->_field848));
		}

		SceneText *topItem = *scene->_list1.begin();
		if (topItem->_position.y < 50) {
			topItem->remove();
			scene->_list1.remove(topItem);

			if (scene->_list1.empty()) {
				setDelay(10);
				++_actionIndex;
			}
		}
		break;
	}
	case 15:
		++_actionIndex;
		scene->_sound1.fade(0, 5, 4, true, this);
		break;
	case 16:
		GW2_GLOBALS._sceneManager.changeScene(500);
		remove();
		break;
	}
}

void Scene200::Action2::signal() {
	switch (_actionIndex++) {
	case 0:
		++_actionIndex;
		setDelay(GW2_GLOBALS._randomSource.getRandomNumber(50) + 50);
		break;
	case 1:
		++_actionIndex;
		static_cast<SceneObject *>(_owner)->animate(ANIM_MODE_8, 1, this);
		break;
	case 2:
		_actionIndex = 0;
		static_cast<SceneObject *>(_owner)->setFrame(1);
		setDelay(1);
		break;
	}
}

/*--------------------------------------------------------------------------*/

Scene200::Scene200(): Scene() {
	_creditIndex = _fieldAB2 = 0;
	_field846 = GW2_GLOBALS._events.getFrameNumber();
}

void Scene200::postInit(SceneObjectList *OwnerList) {
	Scene::postInit();
	loadScene(5500);
	_sound1.play(2);

	setAction(&_action1);
}

void Scene200::process(Event &event) {
	switch (event.eventType) {
	case EVENT_BUTTON_DOWN:
		GW2_GLOBALS._sceneManager.changeScene(500);
		break;
	case EVENT_KEYPRESS:
		if ((event.kbd.keycode == Common::KEYCODE_ESCAPE) || (event.kbd.keycode == Common::KEYCODE_RETURN))
			GW2_GLOBALS._sceneManager.changeScene(500);
		break;
	default:
		break;
	}

	if (!event.handled)
		Scene::process(event);
}

} // End of namespace Geekwad2

} // End of namespace TsAGE
