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
#include "tsage/geekwad2/geekwad2_spaced_solicitors.h"

namespace TsAGE {

namespace Geekwad2 {

/*--------------------------------------------------------------------------
 * Scene 10 - Spaced Solicitors
 *
 *--------------------------------------------------------------------------*/

void Scene10::Action1::signal() {
	Scene10 *scene = (Scene10 *)GW2_GLOBALS._sceneManager._scene;
	int stripNumber;

	if (scene->_coinActive) {
		scene->_coin.remove();
		scene->_coinActive = false;
	}

	switch (_actionIndex++) {
	case 0:
		GW2_GLOBALS._v4708C = 1;
		scene->_hand.hide();
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
		scene->_hand.show();
		setAction(&scene->_action5);
		break;

	default:
		break;
	}
}

void Scene10::Action2::signal() {
	Scene10 *scene = (Scene10 *)GW2_GLOBALS._sceneManager._scene;

	scene->_fieldEBC = 0;
	if (scene->_coinActive) {
		scene->_coin.remove();
		scene->_coinActive = false;
	}

	switch (_actionIndex) {
	case 0:
		++_actionIndex;
		GW2_GLOBALS._v4708C = 1;
		scene->_field31E = 1;
		scene->_sound2.stop();
		
		for (int idx = scene->_fieldEB4; idx <= scene->_fieldEB6; ++idx) {
			for (int idx2 = 0; idx2 < 4; ++idx) {
				if (scene->_girlScouts[idx][idx2]) {
					scene->_girlScouts[idx][idx2]->remove();
					scene->_girlScoutPresent[idx][idx2] = false;
				}
			}
		}

		scene->_objectP->hide();
		scene->_objectP->_field8E = 1;
		scene->_objectP->setAction(NULL);
		scene->_objectP->addMover(NULL);

		if (scene->_coinActive) {
			scene->_coin.remove();
			scene->_coinActive = false;
		}

		for (int idx = 0; idx < 5; ++idx) {
			if (scene->_field127A[idx]) {
				scene->_field127A[idx]->remove();
				scene->_field127A[idx] = NULL;
			}
		}

		scene->_hand.hide();

		if (!scene->_field30C) {
			int stripNum;
			switch (scene->_fieldEC0) {
			case 0:
				stripNum = 120;
				break;
			case 1:
				stripNum = 140;
				break;
			case 2:
				stripNum = 160;
				break;
			case 3:
				stripNum = 180;
				break;
			case 4:
				stripNum = 200;
				break;
			case 5:
				stripNum = 220;
				break;
			default:
				stripNum = 240;
				break;
			}

			scene->_frameNumber = GW2_GLOBALS._events.getFrameNumber() + 120;
			scene->_stripManager.start(stripNum, this);
		} else {
			_actionIndex = 3;
			scene->_field30C = 0;
			scene->signal();
		}
		break;

	case 1:
		++_actionIndex;
		setDelay(1);
		break;

	case 2:
		++_actionIndex;
		GW2_GAME.minigameDone(0, scene->_currentScore);

		if (!GW2_GLOBALS._minigameDigitObtained[0] && GW2_GLOBALS._highestScores[0] > GW2_GLOBALS._scoresToBeat[0]
				&& !GW2_GLOBALS._showComboDigits) {
			//  Required score has been obtained

			int idx = GW2_GAME.getRandomEmptyLockIndex();
			char lockDigit = GW2_GLOBALS._lockCombo[idx];
			GW2_GLOBALS._lockDisplay[idx] = lockDigit;
			GW2_GLOBALS._lockDigits[idx] = true;
			GW2_GLOBALS._minigameDigitObtained[1] = true;

			scene->saveHistory();
			GW2_GAME.showLockDigit(lockDigit);
		}

		setDelay(1);
		break;

	case 3:
		GW2_GLOBALS._soundManager.loadSound(91, false);
		GW2_GLOBALS._soundManager.loadSound(601, false);
		scene->_stripManager.start(280, this, scene);
		break;

	case 4:
		switch (scene->_stripManager._field2E8) {
		case 405:
			scene->reset();
			break;
		case 410:
			GW2_GLOBALS._sceneManager.changeScene(500);
			break;
		case 415:
			GW2_GAME.quitGame();
			--_actionIndex;
			setDelay(1);
			break;
		default:
			break;
		}
		break;

	default:
		break;
	}
}

void Scene10::Action3::signal() {
	Scene10 *scene = (Scene10 *)GW2_GLOBALS._sceneManager._scene;

	switch (_actionIndex++) {
	case 0:
		scene->_sound4.play(112, NULL);
		scene->_hand._strip = 7;
		scene->_hand._frame = 1;
		scene->_hand._numFrames = 5;
		scene->_hand.animate(ANIM_MODE_5, this);
		break;
	
	case 1:
		scene->_hand._numFrames = 10;
		scene->_hand._strip = 3;
		scene->_hand._frame = 1;
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
		// Start initial bus moving across the top of the screen
		GW2_GLOBALS._v4708C = 0;
		if (scene->_coinActive) {
			scene->_coin.remove();
			scene->_coinActive = false;
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
		// Bus door opening
		scene->_objectP->setFrame2(2);
		setDelay(30);
		break;

	case 2:
		// Girl scouts coming out
		scene->_objectP->setFrame2(1);
		ADD_MOVER((*scene->_objectP), 350, 23);

		for (int idx = scene->_fieldEB4; idx >= scene->_fieldEB6; --idx) {
			for (int idx2 = 0; idx2 < 4; ++idx2) {
				Object *obj = scene->_girlScouts[idx2][idx];
				obj->postInit();
				scene->_girlScoutPresent[idx2][idx] = true;
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
		
		// Set the positions to move the girl scouts to
		ADD_MOVER_NULL((*scene->_girlScouts[0][0]), 60, 120);
		ADD_MOVER_NULL((*scene->_girlScouts[0][1]), 60, 90);
		ADD_MOVER_NULL((*scene->_girlScouts[0][2]), 60, 60);
		ADD_MOVER_NULL((*scene->_girlScouts[0][3]), 60, 30);
		ADD_MOVER_NULL((*scene->_girlScouts[1][0]), 130, 120);
		ADD_MOVER_NULL((*scene->_girlScouts[1][1]), 130, 90);
		ADD_MOVER_NULL((*scene->_girlScouts[1][2]), 130, 60);
		ADD_MOVER_NULL((*scene->_girlScouts[1][3]), 130, 30);
		ADD_MOVER_NULL((*scene->_girlScouts[2][0]), 190, 120);
		ADD_MOVER_NULL((*scene->_girlScouts[2][1]), 190, 90);
		ADD_MOVER_NULL((*scene->_girlScouts[2][2]), 190, 60);
		ADD_MOVER_NULL((*scene->_girlScouts[2][3]), 190, 30);
		ADD_MOVER_NULL((*scene->_girlScouts[3][0]), 260, 120);
		ADD_MOVER_NULL((*scene->_girlScouts[3][1]), 260, 90);
		ADD_MOVER_NULL((*scene->_girlScouts[3][2]), 260, 60);
		ADD_MOVER_NULL((*scene->_girlScouts[3][3]), 260, 30);
		
		scene->_totalGirls = 16;
		break;

	case 3:
		// Bus has left the top of the screen, so time to start the girl scout movement action
		scene->_objectP->hide();
		scene->_objectP->_field8E = 1;
		scene->_field31E = 0;
		scene->_fieldEBC = 1;
		scene->_spacePressed = false;
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
		if ((scene->_totalGirls > 0) && (scene->_fieldEBC == 1) && (scene->_objectP->_field8E == 0) &&
				(scene->_fieldEC4 == 1)) {
			bool flag = false;
			int idx;
			for (idx = 0; idx < 4; ++idx) {
				if (!scene->_girlScoutPresent[0][0] && !scene->_girlScoutPresent[1][0] &&
					!scene->_girlScoutPresent[2][0] && !scene->_girlScoutPresent[3][0]) {
					flag = true;
					break;
				}
			}

			if (flag && !scene->_girlScouts[0][idx] && !scene->_girlScouts[1][idx] 
					&& !scene->_girlScouts[2][idx] && !scene->_girlScouts[3][idx]) {
				// Increment frame					
				scene->_objectP->setFrame2(scene->_objectP->_frame + 1);

				// Loop
				for (int idx = scene->_fieldEB4; idx <= scene->_fieldEB6; ++idx) {
					for (int idx2 = 0; idx2 < 4; ++idx) {
						Object *obj = scene->_girlScouts[idx][idx2];

						if (scene->_girlScoutPresent[idx][idx2] && obj->_position.y < 170) {
							scene->_girlScoutPresent[idx][idx2] = true;

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
		}

		setDelay(1);
		_actionIndex = 3;
		break;

	case 3:
		if (scene->_fieldEBC == 1) {
			switch (scene->_objectP->_frame) {
			case 1:
				ADD_MOVER((*scene->_objectP), 340, 23);
				break;

			case 2:
				scene->_objectP->setFrame2(1);
				ADD_MOVER((*scene->_objectP), 340, 23);
				break;

			case 3:
				ADD_MOVER((*scene->_objectP), 0, 23);
				break;

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

		_actionIndex = 4;
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
		for (int idx = 0; idx < 5; ++idx) {
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
	_spacePressed = false;
	_coinActive = false;
	_fieldEB4 = 3;
	_fieldEB6  = 0;
 
	for (int idx = _fieldEB6; idx <= _fieldEB4; ++idx) {
		for (int idx2 = 0; idx2 < 4; ++idx2) {
			_girlScouts[idx][idx2] = &_objList2[idx][idx2];
			_girlScoutPresent[idx][idx2] = true;
		}
	}
	
	for (int idx = 0; idx < 5; ++idx)
		_field127A[idx] = 0;

	_rect1.set(50, 200, 270, 200);
}

void Scene10::postInit(SceneObjectList *OwnerList) {
	Scene::postInit();
	GW2_GLOBALS._events.setCursor((CursorType)1);

	setZoomPercents(20, 1, 30, 100);
	loadScene(10);

	_objectP = &_bus;
	_objectP->postInit();
	_objectP->setVisage(110);
	_objectP->setStrip(1);
	_objectP->setFrame(1);
	_objectP->setPriority(90);
	_objectP->hide();

	_hand.postInit();
	_hand.setVisage(100);
	_hand.setStrip(3);
	_hand.setFrame(1);
	_hand.fixPriority(250);
	_hand.setPosition(Common::Point(160, 199));
	_hand.hide();

	_explosion.postInit();
	_explosion.setVisage(160);
	_explosion.setStrip(1);
	_explosion.setFrame(_explosion.getFrameCount());
	_explosion.fixPriority(90);
	_explosion.setPosition(Common::Point(1, 1));
	
	_stripManager.addSpeaker(&_granSpeaker);
	_stripManager.addSpeaker(&_gameTextSpeaker);

	reset();
}

void Scene10::remove() {
	GW2_GLOBALS._soundManager.setMasterVol(100);
	GW2_GLOBALS._v4708C = 1;

	Scene::remove();
}

void Scene10::process(Event &event) {
	if (!_field31E) {
		switch (event.eventType) {
		case EVENT_MOUSE_MOVE:
			_hand.setPosition(Common::Point(CLIP(GW2_GLOBALS._events._mousePos.x, (int16)45, (int16)274), 199));
			break;

		case EVENT_BUTTON_DOWN:
			event.eventType = EVENT_NONE;
			event.handled = true;
			if ((_fieldEBC == 1) && !_hand._action && !_coinActive && !GW2_GLOBALS._sceneObjects->contains(&_coin)) {
				// Start throwing a coin
				_sound7.play(110);
				_hand.animate(ANIM_MODE_5, NULL);
				
				_coin.postInit();
				_coin.setVisage(100);
				_coin.setStrip(2);
				_coin.setFrame(1);
				_coin.fixPriority(253);
				_coin.setPosition(Common::Point(_hand._position.x, _hand._position.y - 30));
				_coin.animate(ANIM_MODE_2, NULL);
				_coinActive = true;
			}
			GW2_GLOBALS._events.waitForPress(EVENT_BUTTON_UP);
			break;

		case EVENT_KEYPRESS:
			switch (event.kbd.keycode) {
			case Common::KEYCODE_ESCAPE:
				event.handled = true;
				if (((Geekwad2Game *)GW2_GLOBALS._game)->showPauseDialog() != 0) {
					// Stop game
					_field30C = 1;
					setAction(&_action2);
				}
				break;

			case Common::KEYCODE_SPACE:
				_spacePressed = true;
				break;

			case Common::KEYCODE_LEFT:
			case Common::KEYCODE_KP4:
				if (_hand._position.x >= 55)
					_hand._position.x -= 10;
				break;

			case Common::KEYCODE_RIGHT:
			case Common::KEYCODE_KP6:
				if (_hand._position.x <= 265)
					_hand._position.x += 10;
				break;

			case Common::KEYCODE_a:
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
		_obj4._flags |= OBJFLAG_PANES;
		_objectP->_flags |= OBJFLAG_PANES;

		if (_fieldEBC == 1) {
			if (_totalGirls == 0 && _objectP->_field8E == 1) {
				for (int idx = 0; idx < 5; ++idx) {
					if (_field127A[idx])
						found = true;
				}

				if (!found) {
					if (_coinActive) {
						_coin.remove();
						_coinActive = false;
					}

					_fieldEBC = 0;
					setupAction();
					_spacePressed = false;
				}
			}

			if (_coinActive) {
				_coin.setPosition(Common::Point(_coin._position.x, _coin._position.y - 10));
				_coin.setZoom(MAX(_coin._percent - 5, 5));
				checkCoinCollision();
			}

			for (int idx = 0; idx < 5; ++idx) {
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

						if (_field127A[idx]->_bounds.intersects(_hand._bounds)) {
							_field127A[idx]->setAction(&_action4[idx]);
							_hand.setAction(&_action3);
						} else if (_field127A[idx]->_position.y >= 200) {
							_field127A[idx]->remove();
							_field127A[idx] = NULL;
						}
					}
				}
			}

			for (int idx = _fieldEB4; idx <= _fieldEB6; ++idx) {
				for (int idx2 = 0; idx < 4; ++idx2) {
					if (_girlScoutPresent[idx2] && _girlScouts[idx][idx2]->_position.y >= 200) {
						_girlScouts[idx][idx2]->remove();
						_girlScoutPresent[idx][idx2] = false;

						// TODO: Figure out floating point logic
						double v1 = _fieldEB0 / 100;
						_fieldEB0 += _fieldEC2;
						double v2 = _fieldEB0 / 100;

						if (v2 < v1)
							GW2_GLOBALS._soundManager.setMasterVol(GW2_GLOBALS._soundManager.getMasterVol() + 20);

						proc2(_totalGirls);
						_hand._flags |= OBJFLAG_PANES;

						proc3();
					}
				}
			}

			// Handle moving the girl scouts left and right
			if (_fieldECC) {
				for (int idx = _fieldEB4; idx >= _fieldEB6; --idx) {
					for (int idx2 = 0; idx2 < 4; ++idx2) {
						if (_girlScoutPresent[idx][idx2]) {
							if (_girlScouts[idx][idx2]->_position.y >= 170) {
								_girlScouts[idx][idx2]->setPosition(Common::Point(_girlScouts[idx][idx2]->_position.x,
									_girlScouts[idx][idx2]->_position.y + 3));
							} else {
								if (_girlScouts[idx][idx2]->_position.x >= 300)
									_fieldECC = 0;

								_girlScouts[idx][idx2]->setPosition(Common::Point(
									_girlScouts[idx][idx2]->_position.x + _fieldEB0 / 100,
									_girlScouts[idx][idx2]->_position.y));
							}
						}
					}
				}

				if (!_fieldECC) {
					int foundIndex = -1;
					for (int idx2 = 0; idx2 < 4; ++idx2) {
						for (int idx = _fieldEB4; idx >= _fieldEB6; --idx) {
							if (_girlScoutPresent[idx][idx2]) {
								foundIndex = idx2;
								break;
							}
						}

						if (foundIndex != -1)
							break;
					}

					for (int idx = _fieldEB4; idx >= _fieldEB6; --idx) {
						for (int idx2 = 0; idx2 < 4; ++idx2) {
							if (_girlScoutPresent[idx][idx2]) {
								_girlScouts[idx][idx2]->setPosition(Common::Point(_girlScouts[idx][idx2]->_position.x,
									_girlScouts[idx][idx2]->_position.y + 5));

								if ((idx2 == foundIndex) && !_girlScouts[idx][idx2]->_action) {
									_girlScouts[idx][idx2]->setAction(&_actionList1[idx][idx2]);
								}
							}
						}
					}
				}
			} else {
				for (int idx = _fieldEB6; idx <= _fieldEB4; ++idx) {
					for (int idx2 = 0; idx2 < 4; ++idx2) {
						if (_girlScoutPresent[idx][idx2]) {
							if (_girlScouts[idx][idx2]->_position.y >= 170) {
								_girlScouts[idx][idx2]->setPosition(Common::Point(_girlScouts[idx][idx2]->_position.x,
									_girlScouts[idx][idx2]->_position.y + 3));
							} else {
								if (_girlScouts[idx][idx2]->_position.x <= 20)
									_fieldECC = 1;

								_girlScouts[idx][idx2]->setPosition(Common::Point(
									_girlScouts[idx][idx2]->_position.x - _fieldEB0 / 100,
									_girlScouts[idx][idx2]->_position.y));
							}
						}
					}
				}

				if (_fieldECC) {
					int foundIndex = -1;
					for (int idx2 = 0; idx2 < 4; ++idx2) {
						for (int idx = _fieldEB4; idx >= _fieldEB6; --idx) {
							if (_girlScoutPresent[idx][idx2]) {
								foundIndex = idx2;
								break;
							}
						}

						if (foundIndex != -1)
							break;
					}

					for (int idx = _fieldEB4; idx >= _fieldEB6; --idx) {
						for (int idx2 = 0; idx2 < 4; ++idx2) {
							if (_girlScoutPresent[idx][idx2]) {
								_girlScouts[idx][idx2]->setPosition(Common::Point(_girlScouts[idx][idx2]->_position.x,
									_girlScouts[idx][idx2]->_position.y + 5));

								if ((idx2 == foundIndex) && !_girlScouts[idx][idx2]->_action) {
									_girlScouts[idx][idx2]->setAction(&_actionList1[idx][idx2]);
								}
							}
						}
					}
				}				
			}

			// Check for firing a new coin
			if (_spacePressed && (_totalGirls > 0 || !_objectP->_field8E) && _fieldEBC == 1 && !_hand._action 
						&& !_coinActive && !GLOBALS._sceneObjects->contains(&_coin)) {
				// Start throwing a coin
				_sound7.play(110);
				_hand.animate(ANIM_MODE_5, NULL);

				_coin.postInit();
				_coin.setVisage(100);
				_coin.setStrip(2);
				_coin.setFrame(1);
				_coin.fixPriority(253);
				_coin.changeZoom(100);
				_coin.setPosition(Common::Point(_hand._position.x, _hand._position.y - 30));
				_coin.reposition();
				_coin.animate(ANIM_MODE_2, NULL);

				_coinActive = true;
			}

			_spacePressed = false;
		}
	}

	Scene::dispatch();
}

void Scene10::stripCallback(int v) {
	if (v == 302)
		GW2_GLOBALS._sound2.play(601);
	else if (v == 30)
		GW2_GLOBALS._sound2.play(91);
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
	_totalGirls = 0;
	_currentScore = 0;
	
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
	if (_currentScore) {
		_numberBuffer = Common::String::format("%ld", _currentScore);
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
	if (_coinActive) {
		_coin.remove();
		_coinActive = false;
	}

	if ((_fieldEC0 % 2) == 0)
		setAction(&_action1);
	else
		setAction(&_action5);	
}

void Scene10::resetGame() {
	_field31E = 1;
	
	for (int idx = _fieldEB4; idx >= _fieldEB6; --idx) {
		for (int idx2 = 0; idx2 < 4; ++idx2) {
			if (_girlScouts[idx][idx2]) {
				_girlScouts[idx][idx2]->remove();
				_girlScoutPresent[idx][idx2] = false;
			}
		}
	}

	_objectP->hide();
	_objectP->_field8E = 1;
	_objectP->setAction(NULL);
	_objectP->addMover(NULL);

	if (_coinActive) {
		_coin.remove();
		_coinActive = false;
	}

	for (int idx = 0; idx < 5; ++idx) {
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
	_totalGirls = 0;

	_currentScore = 20000;
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

void Scene10::checkCoinCollision() {
	bool breakFlag = false;

	for (int idx = _fieldEB4; idx >= _fieldEB6; --idx) {
		for (int idx2 = 0; idx2 < 4; ++idx2) {
			if (_girlScoutPresent[idx][idx2] && _girlScouts[idx][idx2]->_bounds.intersects(_coin._bounds)) {
				breakFlag = true;
				_explosion.setPosition(_girlScouts[idx][idx2]->_position);
				_explosion._strip = 2;
				_explosion._frame = 1;
				_explosion.animate(ANIM_MODE_5, NULL);
				_girlScouts[idx][idx2]->remove();
				_girlScoutPresent[idx][idx2] = false;

				_coin.remove();

				_coinActive = false;
				_fieldEBA -= 5;
				_hand.setFrame(1);

				int v = 0; // TODO: Decode floating point logic here
				if (v) {
					GW2_GLOBALS._soundManager.setMasterVol(GW2_GLOBALS._soundManager.getMasterVol() - 20);
				}

				_sound5.play(111);
				_currentScore += 93;
				setupScore();

				_field2022 += 93;
				if (_field2022 >= 10000) {
					_field2022 -= 10000;
					++_fieldECA;
					
					switch (_fieldECA) {
					case 2:
						_obj3.setup(100, 6, 4, 40, 13, 130, 1);
						_obj3.draw();
						break;
					case 3:
						_obj4.setup(100, 6, 4, 50, 13, 130, 1);
						_obj4.draw();
						break;
					case 4:
						_obj5.setup(100, 6, 4, 60, 13, 130, 1);
						_obj5.draw();
						break;
					case 5:
						_obj6.setup(100, 6, 4, 70, 13, 130, 1);
						_obj6.draw();
						break;
					case 6:
						_obj7.setup(100, 6, 4, 80, 13, 130, 1);
						_obj7.draw();
						break;
					default:
						break;
					}
				}

				--_totalGirls;
				bool flag = false;
				for (int idx3 = _fieldEB4; idx3 >= _fieldEB6 && !flag; --idx3) {
					if (_girlScoutPresent[idx3][idx2])
						flag = true;
				}

				if (!flag) {
					// Rotate the object list
					Object *objList[4];
					for (int idx3 = _fieldEB4; idx3 >= _fieldEB6 && !flag; --idx3) {
						objList[idx3] = _girlScouts[idx3][idx2];
					}

					for (int idx3 = idx2; idx3 <= 2; ++idx3) {
						for (int idx4 = _fieldEB4; idx4 >= _fieldEB6; --idx4) {
							_girlScoutPresent[idx4][idx3] = _girlScoutPresent[idx4][idx3 + 1];
							_girlScouts[idx4][idx3] = _girlScouts[idx4][idx3 + 1];
						}
					}

					for (int idx3 = _fieldEB4; idx3 >= _fieldEB6 && !flag; --idx3) {
						_girlScouts[idx3][3] = objList[idx3];
						_girlScoutPresent[idx3][3] = false;
					}
				}

				break;
			}
		}

		if (breakFlag)
			break;
	}

	if (_coinActive) {
		if (!_objectP->_field8E && _coin._bounds.intersects(_objectP->_bounds)) {
			_explosion.setPosition(_objectP->_position);
			_explosion._strip = 1;
			_explosion._frame = 1;
			_explosion.animate(ANIM_MODE_5, NULL);

			_sound5.play(114);
			_objectP->hide();
			_objectP->_field8E = 1;

			_currentScore += 218;
			setupScore();

			_field2022 += 218;
			if (_field2022 >= 10000) {
				_field2022 -= 10000;

				switch (++_fieldEC6) {
				case 2:
					_obj3.setup(100, 6, 4, 40, 13, 130, 1);
					_obj3.draw();
					break;
				case 3:
					_obj4.setup(100, 6, 4, 50, 13, 130, 1);
					_obj4.draw();
					break;
				case 4:
					_obj5.setup(100, 6, 4, 60, 13, 130, 1);
					_obj5.draw();
					break;
				case 5:
					_obj6.setup(100, 6, 4, 70, 13, 130, 1);
					_obj6.draw();
					break;
				case 6:
					_obj7.setup(100, 6, 4, 80, 13, 130, 1);
					_obj7.draw();
					break;
				}
			}
		}

		if (_coin._position.y < 10) {
			_coin.remove();
			_coinActive = false;
			_hand.setFrame(1);
		}
	}
}

void Scene10::proc2(int v) {
	_sound6.play(113);
	int lineNum = MAX(_fieldEC0 / 2, 6);
	TextualObject *obj = TextualObject::init(10, lineNum, Common::Point(160, 180), false, 300, true, 31, 44, 44, 0);
	
	GW2_GLOBALS._sceneObjects->draw();
	GW2_GLOBALS._events.waitForPress();

	obj->remove();
}

void Scene10::proc3() {

}

} // End of namespace Geekwad2

} // End of namespace TsAGE
