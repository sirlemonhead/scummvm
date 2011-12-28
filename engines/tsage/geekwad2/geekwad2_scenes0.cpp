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
 * Scene 60 - King's Container
 *
 *--------------------------------------------------------------------------*/

void Scene60::Action1::signal() {
	Scene60 *scene = (Scene60 *)GW2_GLOBALS._sceneManager._scene;

	switch (_actionIndex++) {
	case 0:
		scene->setCursor(1, 0);
		
		// Reset the lock display
		for (int idx = 0; idx < 5; ++idx) {
			if (!GW2_GLOBALS._lockDigits[idx])
				GW2_GLOBALS._lockDisplay[idx] = 'A';
		}

		scene->_field30A = 1;
		scene->_bgSceneObjects.clear();

		scene->_object6.remove();
		scene->_object4.remove();
		scene->_object5.remove();

		scene->loadScene(8025);
		scene->_stripManager.start(9080, this);
		break;

	case 1:
		GW2_GLOBALS._sceneManager.changeScene(510);
		break;
	default:
		break;
	}
}

void Scene60::Action2::signal() {
	Scene60 *scene = (Scene60 *)GW2_GLOBALS._sceneManager._scene;

	switch (_actionIndex++) {
	case 0:
		setDelay(3);
		break;
	case 1: {
		if (GW2_GLOBALS._lockDigits[0] == 1) {
			scene->_list2[0].setup(6002, 4, 1, 146, 102, 130, 0);
			scene->_list2[0].draw();
			scene->_list3[0]._fontFgColor2 = 50;
		} else {
			scene->_list3[0]._fontFgColor2 = 0;
		}

		scene->_list3[0]._maxWidth = 180;
		scene->_list3[0]._fontNumber = 71;
		scene->_list3[0]._fontFgColor = 61;
		scene->_list3[0]._fontBgColor = 59;
		scene->_list3[0]._textMode = ALIGN_CENTER;

		scene->_lockCombo[0] = GW2_GLOBALS._lockDisplay[0];



		break;
	}
	default:
		break;
	}
}

/*--------------------------------------------------------------------------*/

Scene60::Scene60(): SceneExt() {
	_field30A = _field742 = _field744 = _field746 = 0;
	Common::set_to(&_lockCombo[0], &_lockCombo[5], 0);

	_list1[0]._pt = Common::Point(17, 18);
	_list1[1]._pt = Common::Point(142, 99);
	_list1[2]._pt = Common::Point(153, 99);
	_list1[3]._pt = Common::Point(164, 99);
	_list1[4]._pt = Common::Point(175, 99);
	_list1[5]._pt = Common::Point(186, 99);
	_list1[6]._pt = Common::Point(165, 65);

	if (GW2_GLOBALS._v470BC == 1) {
		_list1[1]._v = 0;
		_list1[2]._v = 0;
		_list1[3]._v = 0;
		_list1[4]._v = 0;
		_list1[5]._v = 0;
		_list1[6]._v = 1;
	} else {
		_list1[6]._v = 0;
	}

	int max = 32000;
	for (int idx = 0; idx < 7; ++idx) {
		if (_list1[idx]._v != 0) {
			int diff = ABS(GW2_GLOBALS._events._mousePos.x - _list1[idx]._pt.x) +
					ABS(GW2_GLOBALS._events._mousePos.y - _list1[idx]._pt.y);
			if (diff < max) {
				max = diff;
				_field746 = idx;
			}
		}
	}
}

void Scene60::postInit(SceneObjectList *OwnerList) {
	SceneExt::postInit();
	_stripManager.addSpeaker(&_cybSpeaker);

	setCursor(2, 1);
	resetCursor();
	loadScene(6002);

	_object4.postInit();
	_object4.setVisage(6002);
	_object4.setStrip(2);
	_object4.setFrame(1);
	_object4.fixPriority(250);
	_object4.setPosition(Common::Point(193, 158));

	_object5.postInit();
	_object5.setVisage(6002);
	_object5.setStrip(3);
	_object5.setFrame(1);
	_object5._numFrames = 5;
	_object5.fixPriority(250);
	_object5.setPosition(Common::Point(174, 163));

	setAction(&_action2);
}

void Scene60::remove() {
	restoreCursor();
	saveHistory();
	remove();
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
