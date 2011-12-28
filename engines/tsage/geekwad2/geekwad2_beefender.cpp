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
#include "tsage/geekwad2/geekwad2_beefender.h"

namespace TsAGE {

namespace Geekwad2 {

/*--------------------------------------------------------------------------
 * Scene 30 - Beefender
 *
 *--------------------------------------------------------------------------*/

Scene30::Scene30(): Scene() {
	_field30A = 0;
	_field318 = 0;
	_field31A = 0;
	_field31C = 0;

	for (int i = 0; i < ARRAY_SIZE; ++i) {
		_field1090[i] = 0;
		_field1694[i] = 0;
		_field1996[i] = 0;
	}

	for (int i = 0; i < 3; ++i)
		_field1B6C[i] = 0;
}

void Scene30::synchronize(Serializer &s) {
	Scene::synchronize(s);

	s.syncAsSint16LE(_field30A);
	s.syncAsSint16LE(_field318);
	s.syncAsSint16LE(_field31A);
	s.syncAsSint16LE(_field31C);
	s.syncAsSint16LE(_field31E);
	_rect1.synchronize(s);
	_rect2.synchronize(s);

	for (int i = 0; i < ARRAY_SIZE; ++i) {
		s.syncAsSint16LE(_field1090[i]);
		s.syncAsSint16LE(_field1694[i]);
		s.syncAsSint16LE(_field1996[i]);
	}

	for (int i = 0; i < 3; ++i)
		s.syncAsSint16LE(_field1B6C[i]);
}

void Scene30::postInit(SceneObjectList *OwnerList) {
	Scene::postInit();
	loadScene(300);


}

} // End of namespace Geekwad2

} // End of namespace TsAGE
