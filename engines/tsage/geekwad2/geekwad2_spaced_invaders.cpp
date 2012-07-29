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
#include "tsage/geekwad2/geekwad2_spaced_invaders.h"

namespace TsAGE {

namespace Geekwad2 {

/*--------------------------------------------------------------------------
 * Scene 10 - Spaced Solicitors
 *
 *--------------------------------------------------------------------------*/

void Scene10::Action1::signal() {
	Scene10 *scene = (Scene10 *)GW2_GLOBALS._sceneManager._scene;
	int stripNumber;

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
				stripNumber = 100;
				break;
			case 1:
				scene->_sound2.play(110, NULL, 100);
				GW2_GLOBALS._soundManager.setMasterVol(100);
				stripNumber = 130;
				break;
			case 2:
				scene->_sound2.play(103, NULL, 90);
				GW2_GLOBALS._soundManager.setMasterVol(100);
				stripNumber = 150;
				break;
			case 3:
				scene->_sound2.play(104, NULL, 100);
				GW2_GLOBALS._soundManager.setMasterVol(100);
				stripNumber = 170;
				break;
			case 4:
				scene->_sound2.play(105, NULL, 100);
				GW2_GLOBALS._soundManager.setMasterVol(100);
				stripNumber = 190;
				break;
			case 5:
				scene->_sound2.play(106, NULL, 100);
				GW2_GLOBALS._soundManager.setMasterVol(100);
				stripNumber = 210;
				break;
			default:
				scene->_sound2.play(107, NULL, 100);
				GW2_GLOBALS._soundManager.setMasterVol(100);
				stripNumber = 230;
				break;
			}

			scene->_frameNumber = GW2_GLOBALS._events.getFrameNumber() + 120;
			scene->_stripManager.start(stripNumber, this);
			break;

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

Scene10::Scene10(): SceneExt() {
	_field30A = 0;
	_frameNumber = 0;
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
	_stripManager.addSpeaker(&_gameTextSpeaker);

	reset();
	GW2_GLOBALS._events.addTimer(&timer, 70);
}

void Scene10::remove() {
	GW2_GLOBALS._soundManager.setMasterVol(100);
	GW2_GLOBALS._v4708C = 1;

	GW2_GLOBALS._events.removeTimer(&timer);
	Scene::remove();
}

void Scene10::process(Event &event) {
	if (!_field31E) {
		switch (event.eventType) {
		case EVENT_BUTTON_DOWN:
			event.eventType = EVENT_NONE;
			event.handled = true;
			if ((_fieldEBC == 1) && !_object2._action && (_fieldEB2 != 1) &&
					!GW2_GLOBALS._sceneObjects->contains(&_object3)) {
				_sound7.play(110);
				_object2.animate(ANIM_MODE_5, NULL);
				
				_object3.postInit();
				_object3.setVisage(100);
				_object3.setStrip(2);
				_object3.setFrame(1);
				_object3.fixPriority(253);
				_object3.setPosition(Common::Point(_object2._position.x, _object2._position.y - 30));
				_object3.animate(ANIM_MODE_2, NULL);
				_fieldEB2 = 1;
			}
			GW2_GLOBALS._events.waitForPress(EVENT_BUTTON_UP);
			break;

		case EVENT_KEYPRESS:
			switch (event.kbd.ascii) {
			case Common::KEYCODE_ESCAPE:
				event.handled = true;
				if (((Geekwad2Game *)GW2_GLOBALS._game)->showPauseDialog() != 0) {
					// Stop game
					_field30C = 1;
					setAction(&_action2);
				}
				break;

			case 'A':
			case 'a':
				resetGame();
				event.handled = true;
				break;

			default:
				break;
			}

		default:
			break;
		}
	}

	if (!_frameNumber || (_frameNumber >= GW2_GLOBALS._events.getFrameNumber())) {
		_frameNumber = 0;
		if (!event.handled) {
			Scene::process(event);
		}
	}
}

void Scene10::dispatch() {
//	bool var6 = true;
	bool found = false;

	if (!_field31E) {
		_object2.setPosition(GW2_GLOBALS._events._mousePos);
		_obj4._flags |= OBJFLAG_PANES;
		_objectP->_flags |= OBJFLAG_PANES;

		if (_fieldEBC == 1) {
			if (!_field319C && _objectP->_field8E == 1) {
				for (int idx = 0; idx < 4; ++idx) {
					if (_field127A[idx])
						found = true;
				}

				if (!found) {
					if (_fieldEB2 == 1) {
						_object3.remove();
						_fieldEB2 = 0;
					}

					_fieldEBC = 0;
					setupAction();
					_field772 = 0;
				}
			}

			if (_fieldEB2 == 1) {
				_object3.setPosition(Common::Point(_object3._position.x, _object3._position.y - 10));
				_object3.setZoom(MAX(_object3._percent - 5, 5));
				proc1();
			}

			for (int idx = 0; idx < 4; ++idx) {
				if (_field127A[idx]) {
					if (_field127A[idx]->_action) {
						_field127A[idx]->_flags |= OBJFLAG_PANES;
					} else {
						_field127A[idx]->setPosition(Common::Point(
							_field127A[idx]->_position.x + GW2_GLOBALS._randomSource.getRandomNumber(4) - 2,
							_field127A[idx]->_position.y + _fieldEC8 / 100
						));

						if ((_field127A[idx]->_percent + 5) < 100)
							_field127A[idx]->changeZoom(_field127A[idx]->_percent + 5);

						if (_field127A[idx]->_bounds.intersects(_object2._bounds)) {
							_field127A[idx]->setAction(&_action4[idx]);
							_object2.setAction(&_action3);
						} else if (_field127A[idx]->_position.y >= 200) {
							_field127A[idx]->remove();
							_field127A[idx] = NULL;
						}
					}
				}
			}

			for (int idx = _fieldEB4; idx <= _fieldEB6; ++idx) {
				for (int idx2 = 0; idx < 4; ++idx2) {
					if (_field128E[idx2] && _field1C2E[idx][idx2]->_position.y >= 200) {
						_field1C2E[idx][idx2]->remove();
						_field128E[idx][idx2] = 0;

						// TODO: 
						double v1 = _fieldEB0 / 100;
						_fieldEB0 += _fieldEC2;
						double v2 = _fieldEB0 / 100;

						if (v2 < v1)
							GW2_GLOBALS._soundManager.setMasterVol(GW2_GLOBALS._soundManager.getMasterVol() + 20);

						proc2(_field319C);
						_object2._flags |= OBJFLAG_PANES;

						proc3();
					}
				}
			}
		}

		// Update turret position
		if (isKeyPressed(Common::KEYCODE_LEFT)) {
			if (_object2._position.x <= 265)
				_object2._position.x += 10;
		} else if (isKeyPressed(Common::KEYCODE_RIGHT)) {
			if (_object2._position.x >= 55)
				_object2._position.x -= 10;
		}
	}

	Scene::dispatch();
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

void Scene10::resetGame() {
	_field31E = 1;
	
	for (int idx = _fieldEB6; idx < _fieldEB4; ++idx) {
		for (int idx2 = 0; idx2 < 4; ++idx) {
			if (_field1C2E[idx][idx2]) {
				_field1C2E[idx][idx2]->remove();
				_field128E[idx][idx2] = 0;
			}
		}
	}

	_objectP->hide();
	_objectP->_field8E = 1;
	_objectP->setAction(NULL);
	_objectP->addMover(NULL);

	if (!_fieldEB2) {
		_object3.remove();
		_fieldEB2 = 0;
	}

	for (int idx = 0; idx < 4; ++idx) {
		if (_field127A[idx]) {
			_field127A[idx]->remove();
			_field127A[idx] = NULL;
		}
	}

	_fieldEC0 = 11;
	_fieldEBC = 0;
	_fieldECC = 1;
	_field319A = 0;
	_fieldEC2 = 70;
	_fieldEC4 = 0;
	_fieldEC6 = 3;
	_field2022 = 0;
	_fieldEC8 = 400;
	_fieldECA = 8;
	_field319C = 0;

	_score = 20000;
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

void Scene10::timer() {

}

void Scene10::proc1() {

}

void Scene10::proc2(int v) {
	_sound6.play(113);
/*
	switch (_fieldEC0) {
	default:
		// TODO
		break;
	}
*/
}

void Scene10::proc3() {

}

} // End of namespace Geekwad2

} // End of namespace TsAGE
