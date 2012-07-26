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
#include "tsage/geekwad2/geekwad2_space_golf.h"

namespace TsAGE {

namespace Geekwad2 {

/*--------------------------------------------------------------------------
 * Scene 20 - Space Golf
 *
 *--------------------------------------------------------------------------*/

void Scene20::Action1::signal() {
	Scene20 *scene = (Scene20 *)GW2_GLOBALS._sceneManager._scene;

	switch (_actionIndex++) {
	case 0:
		scene->_object3.animate(ANIM_MODE_5, this);
		break;
	case 1:
		setAction(&scene->_action3);
		break;
	default:
		break;
	}
}

void Scene20::Action2::signal() {
	Scene20 *scene = (Scene20 *)GW2_GLOBALS._sceneManager._scene;
	const uint32 v1 = 0, v2 = 0, v3 = 0;

	switch (_actionIndex) {
	case 0:
		++_actionIndex;
		scene->_field766 = 1;
		scene->_object1.hide();
		scene->_object1.setAction(NULL);
		scene->_object1.animate(ANIM_MODE_NONE);
		
		scene->_object2.remove();

		for (int idx = 0; idx < scene->_field66DA; ++idx) {
			if (scene->_field5E4E[idx]) {
				scene->_field5E4E[idx]->remove();
				scene->_field5E4E[idx] = NULL;
			}
		}

		if (scene->_objectP) {
			scene->_objectP->remove();
			scene->_objectP = NULL;
		}

		scene->_object5.setAction(NULL);
		scene->_object5.hide();
		scene->_object5._field31E = 1;
		
		scene->_object4.hide();
		scene->_object6.hide();

		if (scene->_field768) {
			_actionIndex = 3;
			scene->_field768 = 0;
			setDelay(1);
		} else {
			scene->_frameNumber = GW2_GLOBALS._events.getFrameNumber() + 120;
			GW2_GLOBALS._v4708C = 1;

			int stripNum = 0;
			switch (scene->_field30E) {
			case 0:
				stripNum = 320;
				break;
			case 1:
				stripNum = 325;
				break;
			case 2:
				stripNum = 335;
				break;
			case 3:
				stripNum = 346;
				break;
			default:
				break;
			}

			scene->_stripManager.start(stripNum, this);
		}
		break;

	case 1:
		++_actionIndex;
		setDelay(1);
		break;

	case 2:
		++_actionIndex;
		GW2_GAME.minigameDone(1, scene->_currentScore);

		if (!v3 && v1 > v2 && !GW2_GLOBALS._showComboDigits) {
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
		++_actionIndex;
		GW2_GLOBALS._soundManager.loadSound(91, false);
		GW2_GLOBALS._soundManager.loadSound(601, false);
		scene->_stripManager.start(310, this, scene);
		break;

	case 4:
		switch (scene->_stripManager._field2E8) {
		case 426:
			scene->reset();
			break;
		case 427:
			GW2_GLOBALS._sceneManager.changeScene(500);
			break;
		case 428:
			// Query if the user wants to quit the game
			GW2_GAME.quitGame();

			--_actionIndex;
			setDelay(1);
			break;
		default:
			break;
		}
		break;
	}
}

void Scene20::Action3::signal() {
	Scene20 *scene = (Scene20 *)GW2_GLOBALS._sceneManager._scene;
	int stripNum = 0;

	switch (_actionIndex) {
	case 0:
		scene->_field776 = 0;
		scene->_field778 = 0;

		switch (scene->_field66D6 / 24) {
		case 0:
			if (scene->_sound6.getSoundNum() != 200)
				scene->_sound6.play(200);

			scene->loadScene(205);
			scene->_object1.setPosition(Common::Point(160, 187));

			if (scene->_field66D6 < 24)
				stripNum = 300;
			break;

		case 3:
			if (scene->_sound6.getSoundNum() != 201)
				scene->_sound6.play(201);

			scene->loadScene(200);
			scene->_object1.setPosition(Common::Point(160, 187));

			if (scene->_field66D6 < 24) {
				stripNum = 350;
				scene->_frameNumber = GW2_GLOBALS._events.getFrameNumber() + 120;
			}
			break;

		case 6:
			if (scene->_sound6.getSoundNum() != 202)
				scene->_sound6.play(202);

			scene->loadScene(210);
			scene->_object1.setPosition(Common::Point(160, 187));

			if (scene->_field66D6 < 24) {
				stripNum = 360;
				scene->_frameNumber = GW2_GLOBALS._events.getFrameNumber() + 120;
			}
			break;

		case 9:
			if (scene->_sound6.getSoundNum() != 203)
				scene->_sound6.play(203);

			scene->loadScene(215);
			scene->_object1.setPosition(Common::Point(160, 187));

			if (scene->_field66D6 < 24) {
				stripNum = 365;
				scene->_frameNumber = GW2_GLOBALS._events.getFrameNumber() + 120;
			}
			break;

		case 12:
			if (scene->_sound6.getSoundNum() != 200)
				scene->_sound6.play(200);

			scene->loadScene(230);
			GW2_GLOBALS._sceneRegions.load(205);
			scene->_object1.setPosition(Common::Point(160, 187));

			if (scene->_field66D6 < 24) {
				stripNum = 370;
				scene->_frameNumber = GW2_GLOBALS._events.getFrameNumber() + 120;
			}
			break;

		case 15:
			if (scene->_sound6.getSoundNum() != 201)
				scene->_sound6.play(201);

			scene->loadScene(230);
			GW2_GLOBALS._sceneRegions.load(200);
			scene->_object1.setPosition(Common::Point(160, 187));
			break;

		case 18:
			if (scene->_sound6.getSoundNum() != 202)
				scene->_sound6.play(202);

			scene->loadScene(230);
			GW2_GLOBALS._sceneRegions.load(210);
			scene->_object1.setPosition(Common::Point(160, 187));
			break;

		case 21:
			if (scene->_sound6.getSoundNum() != 203)
				scene->_sound6.play(203);

			scene->loadScene(230);
			GW2_GLOBALS._sceneRegions.load(215);
			scene->_object1.setPosition(Common::Point(160, 187));
			break;

		default:
			break;
		}

		scene->_scoreLabel._message = "SCORE";
		scene->_scoreLabel._maxWidth = 180;
		scene->_scoreLabel._fontNumber = 21;
		scene->_scoreLabel._fontFgColour = 106;
		scene->_scoreLabel._fontBgColour = 110;
		scene->_scoreLabel._fontFgColour2 = 51;
		scene->_scoreLabel._textMode = ALIGN_CENTER;
		scene->_scoreLabel.setup(0, 0, 0, 161, 5, 156, 0);
		scene->_scoreLabel.draw();

		scene->updateScore();

		if (_actionIndex == 0) {
			setDelay(240);
		} else {
			GW2_GLOBALS._v4708C = 1;
			scene->_field766 = 1;
			
			scene->_object5.hide();
			scene->_object5._field31E = 1;
			scene->_object5.setAction(NULL);
			scene->_object4.hide();
			scene->_object6.hide();
			scene->_object1.setFrame(1);
			scene->_object1.setAction(NULL);
			scene->_object1.animate(ANIM_MODE_NONE);

			if (stripNum == 300 && scene->_field30C) {
				setDelay(1);
			} else {
				scene->_sound2.stop();
				scene->_field30C = 1;
				scene->_stripManager.start(stripNum, this);
			}
		}
		break;

	case 1:
		scene->loadResources();
		scene->_field776 = 0;
		GW2_GLOBALS._v4708C = 0;
		scene->_field766 = 0;

		if (--scene->_field66D4 < 0)
			scene->_field66D4 = 0;
		scene->_field66DC -= 5;
		if (scene->_field66DC < 0)
			scene->_field66DC = 0;
		scene->_field66DE -= 5;
		if (scene->_field66DE < 0)
			scene->_field66DE = 0;

		if (scene->_field66D6 == 0) {
			scene->_field66DA = 3;
		} else if (scene->_field66DA < 19) {
			++scene->_field66DA;
		}
		scene->_field66D8 = 0;

		for (int idx = 0; idx <= scene->_field66DA; ++idx) {
			if (!scene->_field5E4E[idx] && !GW2_GLOBALS._sceneObjects->contains(scene->_field5E4E[idx])) {
				Scene20::Object *obj = scene->_field5E4E[idx] = &scene->_objList1[idx];

				obj->postInit();
				obj->setVisage(220);
				obj->setStrip(1);
				obj->fixPriority(100);
				obj->_objIndex = idx;
				obj->_field31A = 0;
				obj->_field312 = 0;
				obj->_field310 = 0;
				obj->_field314 = 1;
				obj->_field31C = 1;
				obj->_newPosition = Common::Point(-10, -10);
				obj->setAction(NULL);
				obj->hide();
				obj->setPosition(Common::Point(GW2_GLOBALS._randomSource.getRandomNumber(299) + 10, 25));
				obj->_field316 = (GW2_GLOBALS._randomSource.getRandomNumber(1) == 1) ? 10 : -10;
				obj->_field318 = 0;

				++scene->_field66D8;
			}
		}

		scene->_field77A = 0;
		++_actionIndex;
		setDelay(1);
		break;

	case 2:
		if (!scene->_object5._action)
			scene->_object5.setAction(&scene->_action9);

		if (scene->_objectP->_bounds.intersects(scene->_object1._bounds))
			scene->_object1._position.x += 10;

		scene->_objectP->setPosition(Common::Point(scene->_objectP->_position.x - 3, scene->_objectP->_position.y));

		if (scene->_objectP->_position.x <= -20) {
			scene->_objectP->remove();
			scene->_objectP = NULL;
			remove();
		} else {
			for (int idx = 0; idx < scene->_field66DA; ++idx) {
				if (scene->_field5E4E[idx] && scene->_field5E4E[idx]->_field31C == 1 &&
						(scene->_objectP->_position.x - 18) < scene->_field5E4E[idx]->_position.x) {
					scene->_field5E4E[idx]->_field31C = 0;
					scene->_field5E4E[idx]->show();
					scene->_field5E4E[idx]->_sound.play(210);
				}
			}

			setDelay(1);
		}
		break;

	default:
		break;
	}
}

void Scene20::Action4::signal() {
	Scene20 *scene = (Scene20 *)GW2_GLOBALS._sceneManager._scene;
	Scene20::Object *obj = static_cast<Scene20::Object *>(_owner);

	switch (_actionIndex++) {
	case 0:
		setDelay(GW2_GLOBALS._randomSource.getRandomNumber(scene->_field66DE - 1) * 60);
		break;

	case 1:
		obj->_strip = 2;
		obj->animate(ANIM_MODE_2, NULL);
		setDelay((scene->_field66DC - 1) * 60);
		break;
		
	case 2:
		obj->_frame = 1;
		obj->_strip = 3;
		obj->_sound.play(220);
		obj->animate(ANIM_MODE_5, this);
		break;

	case 3:
		obj->_frame = 1;
		obj->_strip = 4;
		obj->_field314 = 0;
		obj->animate(ANIM_MODE_2, NULL);
		remove();
		break;

	default:
		break;
	}
}

void Scene20::Action5::signal() {
	Scene20 *scene = (Scene20 *)GW2_GLOBALS._sceneManager._scene;

	switch (_actionIndex) {
	case 0:
		++_actionIndex;
		setDelay(GW2_GLOBALS._randomSource.getRandomNumber(119) * 60);
		scene->_object5.animate(ANIM_MODE_NONE);
		break;

	case 1:
	case 2:
		++_actionIndex;
		scene->_object5.animate(ANIM_MODE_4, scene->_object5._frame, 1, this);
		break;

	case 3:
		_actionIndex = 0;
		scene->_object5.animate(ANIM_MODE_4, scene->_object5._frame, 1, this);
		break;

	default:
		break;
	}
}

void Scene20::Action6::signal() {
	Scene20 *scene = (Scene20 *)GW2_GLOBALS._sceneManager._scene;

	switch (_actionIndex++) {
	case 0:
		setDelay(1);
		break;

	case 1:
		scene->_object1.setFrame(1);
		if (scene->_object1._strip == 3) {
			scene->_object1.setStrip(1);
		} else if (scene->_object1._strip == 4) {
			scene->_object1.setStrip(2);
		}

		scene->_object1.animate(ANIM_MODE_4, 3, 1, this);
		break;

	case 2: {
		scene->_sound1.play(231);
		int xOffset = (scene->_object1._strip == 1) ? 8 : -8;

		for (int idx = 0; idx < 20; ++idx) {
			if (scene->_field5E4E[idx] && !scene->_field5E4E[idx]->_field31A && 
					ABS(scene->_field5E4E[idx]->_position.x - (scene->_object1._position.x + xOffset)) < 9 &&
					ABS(scene->_field5E4E[idx]->_position.y - scene->_object1._position.y) < 10) {
				--scene->_field66D8;
				scene->_object3.setPosition(scene->_field5E4E[idx]->_position);
				scene->_object3._frame = 1;

				if (scene->_field66D8 > 0) {
					scene->_object3.animate(ANIM_MODE_5, NULL);
				} else {
					scene->setAction(&scene->_action1);
				}

				scene->_sound3.play(232);
				scene->_field5E4E[idx]->remove();
				scene->_field5E4E[idx] = NULL;

				if (scene->_field66D8 <= 0) {
					++scene->_field66D6;
					if (!scene->_objectP) {
						Scene20::Object *obj = scene->_objectP = &scene->_object7;
						obj->postInit();
						obj->setVisage(230);
						obj->setStrip(5);
						obj->setFrame(1);
						obj->fixPriority(110);
						obj->setAction(NULL);
						obj->setPosition(Common::Point(350, 18));
					}
				}
			}

			++scene->_currentScore += 123;
			scene->updateScore();
		}

		scene->_object1.animate(ANIM_MODE_4, 1, 1, NULL);
		remove();
		break;
	}

	default:
		break;
	}
}

void Scene20::Action7::signal() {
	Scene20 *scene = (Scene20 *)GW2_GLOBALS._sceneManager._scene;
	int regionIndex;

	switch (_actionIndex++) {
	case 0:
		scene->_field766 = 1;
		GW2_GLOBALS._v4708C = 1;
		setDelay(1);
		break;

	case 1:
		scene->_object1.hide();
		scene->_object6.hide();
		scene->_sound2.stop();

		scene->_object2.postInit();
		scene->_object2.setVisage(250);
		scene->_object2.fixPriority(101);
		scene->_object2.setFrame(1);
		scene->_object2.setPosition(scene->_object1._position);

		regionIndex = GW2_GLOBALS._sceneRegions.indexOf(scene->_object1._position);
		if (regionIndex == 7 || regionIndex == 39) {
			for (int idx = 0; idx < 20; ++idx) {
				if (scene->_field5E4E[idx] && scene->_field5E4E[idx]->_field31A == 1) {
					scene->_field5E4E[idx]->remove();
					scene->_field5E4E[idx] = NULL;
				}
			}

			switch (scene->_field30A) {
			case 0:
				scene->_sound5.play(294);
				scene->_field30E = 1;
				scene->_object2.setStrip(2);
				scene->_object2.animate(ANIM_MODE_4, 9, 1, this);
				break;
			case 1:
				scene->_field30E = 2;
				scene->_object2.setStrip(3);
				scene->_object2.animate(ANIM_MODE_4, 7, 1, this);
				break;
			case 2:
				scene->_sound5.play(291);
				scene->_field30E = 3;
				scene->_object2.setStrip(4);
				scene->_object2.animate(ANIM_MODE_4, 6, 1, this);
				break;
			case 3:
				scene->_field30E = 3;
				scene->_object2.setStrip(5);
				scene->_object2.animate(ANIM_MODE_4, 10, 1, this);
				break;
			default:
				break;
			}
		} else {
			scene->_field30E = 0;
			scene->_object2.setStrip(1);
			scene->_object2.animate(ANIM_MODE_4, 4, 1, this);
		}
		break;

	case 2:
		switch (scene->_object2._strip) {
		case 1:
			scene->_sound5.play(290);
			scene->_object2.animate(ANIM_MODE_5, this);
			break;
		case 2:
			scene->_sound5.play(232);
			scene->_object2.animate(ANIM_MODE_5, this);
			break;
		case 3:
			scene->_sound5.play(293);
			scene->_object2.animate(ANIM_MODE_4, 9, 1, this);
			break;
		case 4:
			scene->_sound5.play(295);
			scene->_object2.animate(ANIM_MODE_5, this);
			break;
		case 5:
			scene->_sound5.play(232);
			scene->_object2.animate(ANIM_MODE_5, this);
			break;
		default:
			break;
		}
		break;

	case 3:
		if (scene->_object2._strip == 3) {
			scene->_sound5.play(293);
			scene->_object2.animate(ANIM_MODE_4, 11, 1, this);
		} else {
			setDelay(1);
		}
		break;

	case 4:
		if (scene->_object2._strip == 3) {
			scene->_sound5.play(293);
			scene->_object2.animate(ANIM_MODE_5, this);
		} else {
			setDelay(1);
		}
		break;

	case 5:
		scene->setAction(&scene->_action2);
		break;

	default:
		break;
	}
}

void Scene20::Action8::signal() {
	Scene20 *scene = (Scene20 *)GW2_GLOBALS._sceneManager._scene;
	Scene20::Object *obj = static_cast<Scene20::Object *>(_owner);

	if (obj->_field312 == 0) {
		if (obj->_field31A == 1) {
			obj->setPosition(Common::Point(scene->_object1._position.x + obj->_field324,
				scene->_object1._position.y + obj->_field322));
		} else {
			int regionIndex = GW2_GLOBALS._sceneRegions.indexOf(obj->_position);

			switch (regionIndex) {
			case 0:
			case 2:
			case 13:
				break;

			default:
				scene->_field780 = ABS(obj->_position.x - scene->_object1._position.x);
				scene->_field782 = 319 - obj->_position.x + scene->_object1._position.x;
				scene->_field784 = 319 - scene->_object1._position.x + obj->_position.x;

				if (scene->_field780 > scene->_field782 || scene->_field780 > scene->_field784) {
					if (scene->_field782 >= scene->_field784) {
						obj->_field316 = -scene->_field66E0;
						obj->setStrip(4);
					} else {
						obj->_field316 = scene->_field66E0;
						obj->setStrip(5);
					}
				} else if (obj->_position.x <= scene->_object1._position.x) {
					obj->_field316 = scene->_field66E0;
					obj->setStrip(5);
				} else {
					obj->_field316 = -scene->_field66E0;
					obj->setStrip(4);
				}
				break;
			}
		}

		setDelay(1);
	} else {
		if (obj->_field310 == 0) {
			obj->_field310 = 1;
			setDelay(180);
		} else {
			obj->_field310 = 0;
			obj->_field312 = 0;
			setDelay(1);
		}
	}
}

void Scene20::Action9::signal() {
	Scene20 *scene = (Scene20 *)GW2_GLOBALS._sceneManager._scene;

	switch (_actionIndex) {
	case 0:
		switch (GW2_GLOBALS._randomSource.getRandomNumber(2)) {
		case 0:
			scene->_object4.setPosition(Common::Point(30, 190));
			scene->_object5.setPosition(Common::Point(30, 170));
			break;
		case 1:
			scene->_object4.setPosition(Common::Point(161, 190));
			scene->_object5.setPosition(Common::Point(161, 170));
			break;
		case 2:
			scene->_object4.setPosition(Common::Point(290, 190));
			scene->_object5.setPosition(Common::Point(290, 170));
			break;
		default:
			break;
		}

		scene->_object4.show();
		scene->_object5.show();
		scene->_object5._field31E = 0;

		_actionIndex = 1;
		setDelay(1);
		break;

	case 1:
		scene->_object5._field318 = -160;
		scene->_object4.setFrame(1);
		scene->_object4.animate(ANIM_MODE_5, NULL);
		scene->_sound4.play(240);

		if (scene->_object5._position.y > -10) {
			_actionIndex = 1;
			setDelay(240);
		} else {
			scene->_object4.hide();
			scene->_object5.hide();
			scene->_object5._field31E = 1;

			_actionIndex = 0;
			setDelay(1);
		}
		break;

	default:
		break;
	}
}


/*--------------------------------------------------------------------------*/

Scene20::Scene20(): SceneExt() {
	_field30C = 0;
	_field768 = 0;
	GW2_GLOBALS._v4708C = 0;
	_field766 = 0;
	_frameNumber = 0;
	_field30A = 2;
	_field778 = 0;
	_field776 = 0;
}

void Scene20::postInit(SceneObjectList *OwnerList) {
	Scene::postInit();
	GW2_GLOBALS._events.addTimer(&timer, 70);

	_stripManager.addSpeaker(&_murraySpeaker);
	_stripManager.addSpeaker(&GW2_GLOBALS._gameTextSpeaker);

	reset();
}

void Scene20::remove() {
	GW2_GLOBALS._v4708C = 1;
	GW2_GLOBALS._events.removeTimer(&timer);
	SceneExt::remove();
}

void Scene20::process(Event &event) {
	if (!_field766) {
		if (event.eventType == EVENT_KEYPRESS && event.kbd.keycode == Common::KEYCODE_ESCAPE) {
			_field768 = 1;
			setAction(&_action2);
		}
	}

	if (!_frameNumber || GW2_GLOBALS._events.getFrameNumber() >= _frameNumber) {
		_frameNumber = 0;
		if (!event.handled)
			SceneExt::process(event);
	}
}

void Scene20::dispatch() {
	bool flag = false;

	if (!_field766) {
		_object4._flags |= OBJFLAG_PANES;
		int regionIndex = GW2_GLOBALS._sceneRegions.indexOf(_object1._position);
		if ((regionIndex == 7) || (regionIndex == 39)) {
			_field77E = 0;
			_field77C = -7;
		} else if ((regionIndex == 15) || (regionIndex == 13) || (regionIndex == 2)) {
			_sound7.play(211);

			if (regionIndex == 2) {
				_field77C = -_field77C;
				if (_field77E <= 0) {
					_object1._position.x += 8;
				} else {
					_object1._position.x -= 8;
				}
			} else {
				_object1._position.y += 5;
				_field77C = ABS(_field77C);
			}

			_field77E = -_field77E;
		}

		if (isKeyPressed(Common::KEYCODE_LEFT) || isKeyPressed(Common::KEYCODE_KP6) ||
				isKeyPressed(Common::KEYCODE_RIGHT) || isKeyPressed(Common::KEYCODE_KP6) ||
				isKeyPressed(Common::KEYCODE_UP) || isKeyPressed(Common::KEYCODE_KP8)) {
			flag = true;
			if (_object1._strip != 1 && _object1._strip != 2) {
				if (_object1._strip == 3) {
					_object1.setStrip(1);
					_object1.setFrame(1);
					_object6.setStrip(6);
				} else {
					_object1.setStrip(2);
					_object1.setFrame(1);
					_object6.setStrip(7);
				}
			}

			if (_field77C > -40)
				_field77C -= 6;
		}
	
		if (isKeyPressed((Common::KeyCode)73) || isKeyPressed((Common::KeyCode)77)) {
			if (_object1._action) {
				_object1.setAction(NULL);
				_object1.setFrame(1);
				_object1.animate(ANIM_MODE_NONE);
			}

			if (regionIndex == 0) {
				flag = true;
				if (_object1._strip != 1) {
					_object1.setStrip(1);
					_object1.setFrame(1);
					_object6.setStrip(6);
				}

				if (_field77E < 40)
					_field77E += 6;
			} else if (regionIndex == 7 || regionIndex == 39) {
				if (_object1._strip != 3) {
					_object1.setStrip(3);
					_object6.setStrip(6);
				}

				_object1.setFrame(_object1._frame == _object1.getFrameCount() ? 1 : _object1._frame + 1);
				_field77E = 20;
			}
		}

		if (isKeyPressed((Common::KeyCode)71) || isKeyPressed((Common::KeyCode)75)) {
			if (_object1._action) {
				_object1.setAction(NULL);
				_object1.setFrame(1);
				_object1.animate(ANIM_MODE_NONE);
			}

			if (regionIndex == 0) {
				flag = true;
				if (_object1._strip != 2) {
					_object1.setStrip(2);
					_object1.setFrame(1);
					_object6.setStrip(7);
				}

				if (_field77E > -40)
					_field77E -= 6;
			} else if (regionIndex == 7 || regionIndex == 39) {
				if (_object1._strip != 4) {
					_object1.setStrip(4);
					_object6.setStrip(7);
				}

				_object1.setFrame(_object1._frame == _object1.getFrameCount() ? 1 : _object1._frame + 1);
				_field77E = -20;
			}
		}

		if (_field778 > 0)
			--_field778;

		if (_field776 == 1 && _field778 > 0 && !_action) {
			_object1.setAction(&_action6);
			_field778 = 6;
		}

		_field776 = 0;
		if (_field77C < 60)
			_field77C += 3;
		if (_object1._position.y <= 10) {
			_field77C = ABS(_field77C);
			_object1._position.y += 5;
		}

		_object1.setPosition(Common::Point(_object1._position.x + _field77E / 10, _object1._position.y + _field77C / 10));
		if (_object1._position.x > 319)
			_object1.setPosition(Common::Point(0, _object1._position.y));
		if (_object1._position.x < 0) 
			_object1.setPosition(Common::Point(319, _object1._position.y));

		if (flag) {
			if (_object1._strip == 1) {
				_object6.setPosition(Common::Point(_object1._position.x - 8, _object1._position.y - 11));
			} else {
				_object6.setPosition(Common::Point(_object1._position.x + 8, _object1._position.y - 11));
			}
			_object6.show();

			if (!_sound2.isPlaying()) {
				_sound2.play(230);
				_sound2.holdAt(1);
			}
		} else {
			_object6.hide();
			if (_sound2.isPlaying())
				_sound2.stop();
		}

		if (_object5._field318 < 60)
			_object5._field318 += 3;
		if (_object5._field318 <= 0) {
			if (_object5._strip == 3) {
				_object5.setFrame(1);
				_object5.setStrip(2);
				_object5.animate(ANIM_MODE_5, NULL);
			}
		} else {
			if (_object5._strip == 2) {
				_object5.setFrame(1);
				_object5.setStrip(3);
				_object5.animate(ANIM_MODE_2, NULL);
			}
		}

		_object5.setPosition(Common::Point(_object5._position.x, _object5._position.y + _object5._field318 / 45));
		if (!_object5._field31E && _object5._bounds.intersects(_object1._bounds)) {
			for (int idx = 0; idx < 20; ++idx) {
				if (_field5E4E[idx] && _field5E4E[idx]->_field31A == 1) {
					_field5E4E[idx]->_sound.play(241);
					_field5E4E[idx]->_field312 = 1;
					_field5E4E[idx]->_field31A = 0;
					_field5E4E[idx]->fixPriority(100);
					--_field77A;
				}
			}
		}

		for (int idx = 0; idx < 20; ++idx) {
			if (_field5E4E[idx] && !_field5E4E[idx]->_field31C && _field5E4E[idx]->_field31A != 1) {
				if (_field5E4E[idx]->_bounds.intersects(_object1._bounds)) {
					if (_object1._strip == 1 || _object1._strip == 3) {
						if ((_object1._position.x + 2) <= _field5E4E[idx]->_position.x ||
								ABS(_field5E4E[idx]->_position.y - _object1._position.y) < 13) {
							if (!_field5E4E[idx]->_field312) {
								_field5E4E[idx]->_sound.play(223);
								_field5E4E[idx]->_field31A = 1;
								_field5E4E[idx]->fixPriority(105);
								_field5E4E[idx]->_field314 = 0;

								++_field77A;
								if (_field77A == 3 && !GW2_GLOBALS._v472A1) {
									setAction(&_action7);
								}

								if (_field5E4E[idx]->_position.y <= (_field5E4E[idx]->_position.y - 7)) {
									_field5E4E[idx]->_field322 = _field5E4E[idx]->_position.y - _object1._position.y 
										+ GW2_GLOBALS._randomSource.getRandomNumber(4) + 7;
								} else {
									_field5E4E[idx]->_field322 = _field5E4E[idx]->_position.y - _object1._position.y 
										- GW2_GLOBALS._randomSource.getRandomNumber(4) - 7;
								}

								_field5E4E[idx]->_field324 = GW2_GLOBALS._randomSource.getRandomNumber(10) - 5;
								_field5E4E[idx]->setStrip(6);
								_field5E4E[idx]->animate(ANIM_MODE_2, NULL);
								_field5E4E[idx]->setAction(&_action8[idx]);
							}
						}
					} else {
						if ((_object1._position.x - 2) >= _field5E4E[idx]->_position.x ||
								ABS(_field5E4E[idx]->_position.y - _object1._position.y) < 13) {
							if (!_field5E4E[idx]->_field312) {
								_field5E4E[idx]->_field31A = 1;
								_field5E4E[idx]->fixPriority(105);
								_field5E4E[idx]->_field314 = 0;

								++_field77A;
								if (_field77A == 3 && !GW2_GLOBALS._v472A1) {
									setAction(&_action7);
								}

								if (_field5E4E[idx]->_position.y <= (_field5E4E[idx]->_position.y - 7)) {
									_field5E4E[idx]->_field322 = _field5E4E[idx]->_position.y - _object1._position.y 
										+ GW2_GLOBALS._randomSource.getRandomNumber(4) + 7;
								} else {
									_field5E4E[idx]->_field322 = _field5E4E[idx]->_position.y - _object1._position.y 
										- GW2_GLOBALS._randomSource.getRandomNumber(4) - 7;
								}

								_field5E4E[idx]->_field324 = GW2_GLOBALS._randomSource.getRandomNumber(10) - 5;
								_field5E4E[idx]->setStrip(6);
								_field5E4E[idx]->animate(ANIM_MODE_2, NULL);
								_field5E4E[idx]->setAction(&_action8[idx]);
							}
						}
					}
				}

				if (_field5E4E[idx]->_field31A != 1) {
					int regionIndex = GW2_GLOBALS._sceneRegions.indexOf(_field5E4E[idx]->_position);
					if (regionIndex == 7 || regionIndex == 39) {
						_field5E4E[idx]->_field312 = 0;
						if (ABS(_field5E4E[idx]->_field316) > 15) {
							if (_field5E4E[idx]->_field316 > 0) 
								_field5E4E[idx]->_field316 = _field66E0;
							else
								_field5E4E[idx]->_field316 = -_field66E0;
						}

						if (_field5E4E[idx]->_field314) {
							if (_field5E4E[idx]->_field318 > 0 && _field5E4E[idx]->_field318 < 30) {
								_field5E4E[idx]->_sound.play(211, NULL, _field5E4E[idx]->_field318 * 2);
								_field5E4E[idx]->_field318 = 0;
								_field5E4E[idx]->_field316 = 0;
							} else if (_field5E4E[idx]->_field318 > 0) {
								_field5E4E[idx]->_sound.play(211, NULL, _field5E4E[idx]->_field318 * 2);
								_field5E4E[idx]->_field318 = -_field5E4E[idx]->_field318 + 15;
							}

							if (!_field5E4E[idx]->_action)
								_field5E4E[idx]->setAction(&_action4[idx]);
						} else {
							_field5E4E[idx]->_field318 = 0;
							if (!_field5E4E[idx]->_action)
								_field5E4E[idx]->setAction(&_action8[idx]);

							if (regionIndex == 39) {
								if (ABS(_field5E4E[idx]->_position.y - _object1._position.y) < 10) {
									int v = GW2_GLOBALS._randomSource.getRandomNumber(9);
									if (v < 6) {
										_field5E4E[idx]->setStrip((_field5E4E[idx]->_strip == 5) ? 4 : 5);
										_field5E4E[idx]->_field316 = -_field5E4E[idx]->_field316;
									} else if (v < 9) {
										_field5E4E[idx]->_field318 = -80;
										_field5E4E[idx]->_field316 = (_field5E4E[idx]->_field316 > 0) ? 20 : -20;
										_field5E4E[idx]->_sound.play(221 + GW2_GLOBALS._randomSource.getRandomNumber(1));
									}
								} else if (_field5E4E[idx]->_position.y > _object1._position.y) {
									int v = GW2_GLOBALS._randomSource.getRandomNumber(9);
									if (v < 4) {
										_field5E4E[idx]->setStrip((_field5E4E[idx]->_strip == 5) ? 4 : 5);
										_field5E4E[idx]->_field316 = -_field5E4E[idx]->_field316;
									} else if (v < 8) {
										_field5E4E[idx]->_field318 = -80;
										_field5E4E[idx]->_field316 = (_field5E4E[idx]->_field316 > 0) ? 20 : -20;
										_field5E4E[idx]->_sound.play(221 + GW2_GLOBALS._randomSource.getRandomNumber(1));
									}
								} else {
									int v = GW2_GLOBALS._randomSource.getRandomNumber(9);
									if (v < 6) {
										_field5E4E[idx]->setStrip((_field5E4E[idx]->_strip == 5) ? 4 : 5);
										_field5E4E[idx]->_field316 = -_field5E4E[idx]->_field316;
									} else if (v < 8) {
										_field5E4E[idx]->_field318 = -80;
										_field5E4E[idx]->_field316 = (_field5E4E[idx]->_field316 > 0) ? 20 : -20;
										_field5E4E[idx]->_sound.play(221 + GW2_GLOBALS._randomSource.getRandomNumber(1));
									}
								}
							}
						}
					} else if (regionIndex == 13 || regionIndex == 2) {
						_field5E4E[idx]->_sound.play(211);
						_field5E4E[idx]->_position = _field5E4E[idx]->_newPosition;

						if (_field5E4E[idx]->_field316 <= 0) {
							if (!_field5E4E[idx]->_field314)
								_field5E4E[idx]->setStrip(5);
						} else {
							if (!_field5E4E[idx]->_field314)
								_field5E4E[idx]->setStrip(4);
						}
					} else {
						if (_field5E4E[idx]->_field318 < 60)
							_field5E4E[idx]->_field318 += 3;
					}
				}
			}
		}
	}

	Scene::dispatch();
}

void Scene20::timer() {

}

void Scene20::reset() {
	_scoreLabel.remove();
	_scoreValue.remove();

	_field766 = 0;
	_field77E = 5;
	_field77C = 5;

	for (int idx = 0; idx < 20; ++idx) 
		_field5E4E[idx] = 0;
	
	_field66D2 = 0;
	_field66D4 = 5;
	_field66D6 = 0;
	_field66DA = 0;
	_field66DC = 30;
	_field66DE = 30;
	_field66E0 = 10;
	_objectP = NULL;
	_field30A = GW2_GLOBALS._randomSource.getRandomNumber(3);
	
	_object1.postInit();
	_object1.setVisage(210);
	_object1.setStrip(1);
	_object1.fixPriority(101);
	_object1.setFrame(1);
	_object1.setPosition(Common::Point(160, 187));
	_object1.hide();

	_object6.postInit();
	_object6.setVisage(210);
	_object6.setStrip(6);
	_object6.setFrame(1);
	_object6.fixPriority(50);
	_object6.animate(ANIM_MODE_2, NULL);
	_object6.setPosition(_object1._position);
	_object6.hide();
	_currentScore = 0;

	_object3.postInit();
	_object3.setVisage(210);
	_object3.setStrip(5);
	_object3.setFrame(_object3.getFrameCount());
	_object3.fixPriority(40);
	_object3.setPosition(Common::Point(1, 1));

	_object4.postInit();
	_object4.setVisage(230);
	_object4.setStrip(1);
	_object4.setFrame(1);
	_object4.fixPriority(170);
	_object4.setPosition(Common::Point(150, 190));
	_object4.hide();

	_object5.postInit();
	_object5.setVisage(230);
	_object5.setStrip(2);
	_object5.setFrame(1);
	_object5.fixPriority(160);
	_object5.setPosition(Common::Point(150, 190));
	_object5._field318 = 5;
	_object5._numFrames = 3;
	_object5.animate(ANIM_MODE_2, NULL);
	_object5.hide();
	_object5._field31E = 1;
	_object5.setAction(NULL);

	if (!_objectP) {
		_objectP = &_object7;
		_objectP->postInit();
		_objectP->setVisage(230);
		_objectP->setStrip(5);
		_objectP->setFrame(1);
		_objectP->fixPriority(110);
		_objectP->setAction(NULL);
		_objectP->setPosition(Common::Point(350, 18));
	}

	setAction(&_action3);
}

void Scene20::updateScore() {
	if (_currentScore == 0) {
		_scoreBuffer = "0";
	} else {
		_scoreBuffer = Common::String::format("%d", _currentScore);
	}

	//TODO
}

void Scene20::stripCallback(int v) {
	if (v == 302)
		GW2_GLOBALS._sound2.play(601);
	else if (v == 303)
		GW2_GLOBALS._sound2.play(91);
}

} // End of namespace Geekwad2

} // End of namespace TsAGE
