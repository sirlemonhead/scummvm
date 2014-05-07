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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef MADS_DRAGONSPHERE_SCENES_H
#define MADS_DRAGONSPHERE_SCENES_H

#include "common/scummsys.h"
#include "mads/game.h"
#include "mads/scene.h"
#include "mads/dragonsphere/game_dragonsphere.h"
//#include "mads/dragonsphere/globals_dragonsphere.h"


namespace MADS {

namespace Dragonsphere {

enum Noun {
	NOUN_LOOK_AT		= 0x0D1
	// TODO
};

class SceneFactory {
public:
	static SceneLogic *createScene(MADSEngine *vm);
};

/**
 * Specialized base class for Dragonsphere game scenes
 */
class DragonsphereScene : public SceneLogic {
protected:
	DragonsphereGlobals &_globals;
	GameDragonsphere &_game;
	MADSAction &_action;

	/**
	 * Forms an animation resource name
	 */
	Common::String formAnimName(char sepChar, int suffixNum);

	/**
	 * Plays appropriate sound for entering varous rooms
	 */
	void lowRoomsEntrySound();
public:
	/**
	 * Constructor
	 */
	DragonsphereScene(MADSEngine *vm);

	void sub7178C();
};

class SceneInfoDragonsphere : public SceneInfo {
	friend class SceneInfo;
protected:
	virtual void loadCodes(MSurface &depthSurface, int variant);

	virtual void loadCodes(MSurface &depthSurface, Common::SeekableReadStream *stream);

	/**
	* Constructor
	*/
	SceneInfoDragonsphere(MADSEngine *vm) : SceneInfo(vm) {}
};

// TODO: Temporary, remove once implemented properly
class Scene1xx : public DragonsphereScene {
protected:
	/**
	 * Plays an appropriate sound when entering a scene
	 */
	void sceneEntrySound() {}

	/**
	 *Sets the AA file to use for the scene
	 */
	void setAAName() {}

	/**
	 * Updates the prefix used for getting player sprites for the scene
	 */
	void setPlayerSpritesPrefix() {}
public:
	Scene1xx(MADSEngine *vm) : DragonsphereScene(vm) {}
};

// TODO: Temporary, remove once implemented properly
class DummyScene: public DragonsphereScene {
public:
	DummyScene(MADSEngine *vm) : DragonsphereScene(vm) {
		warning("Unimplemented scene");
	}

	virtual void setup() {}
	virtual void enter() {}
	virtual void step() {}
	virtual void preActions() {}
	virtual void actions() {}
	virtual void synchronize(Common::Serializer &s) {}
};

} // End of namespace Dragonsphere

} // End of namespace MADS

#endif /* MADS_DRAGONSPHERE_SCENES_H */