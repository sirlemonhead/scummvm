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
	_bgObject2.remove();
	_bgObject1.remove();

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
	_field61E0 = 0;

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

} // End of namespace Geekwad2

} // End of namespace TsAGE
