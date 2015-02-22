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
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef XEEN_MAP_H
#define XEEN_MAP_H

#include "common/stream.h"
#include "common/array.h"
#include "common/rect.h"
#include "xeen/combat.h"
#include "xeen/party.h"
#include "xeen/scripts.h"
#include "xeen/sprites.h"

namespace Xeen {

#define MAP_WIDTH 16
#define MAP_HEIGHT 16
#define TOTAL_SURFACES 16
#define INVALID_CELL 0x8888

class XeenEngine;

enum MonsterType {
	MONSTER_0 = 0, MONSTER_ANIMAL = 1, MONSTER_INSECT = 2,
	MONSTER_HUMANOID = 3, MONSTER_UNDEAD = 4, MONSTER_GOLEM = 5,
	MONSTER_DRAGON = 6
};

class MonsterStruct {
public:
	Common::String _name;
	int _experience;
	int _hp;
	int _accuracy;
	int _speed;
	int _numberOfAttacks;
	int _hatesClass;
	int _strikes;
	int _dmgPerStrike;
	DamageType _attackType;
	SpecialAttack _specialAttack;
	int _hitChance;
	int _rangeAttack;
	MonsterType _monsterType;
	int _fireResistence;
	int _electricityResistence;
	int _coldResistence;
	int _poisonResistence;
	int _energyResistence;
	int _magicResistence;
	int _phsyicalResistence;
	int _field29;
	int _gold;
	int _gems;
	int _itemDrop;
	bool _flying;
	int _imageNumber;
	int _loopAnimation;
	int _animationEffect;
	int _fx;
	Common::String _attackVoc;
public:
	MonsterStruct();
	MonsterStruct(Common::String name, int experience, int hp, int accuracy,
		int speed, int numberOfAttacks, CharacterClass hatesClass, int strikes,
		int dmgPerStrike, DamageType attackType, SpecialAttack specialAttack,
		int hitChance, int rangeAttack, MonsterType monsterType,
		int fireResistence, int electricityResistence, int coldResistence,
		int poisonResistence, int energyResistence, int magicResistence,
		int phsyicalResistence, int field29, int gold, int gems, int itemDrop,
		bool flying, int imageNumber, int loopAnimation, int animationEffect,
		int field32, Common::String attackVoc);

	void synchronize(Common::SeekableReadStream &s);
};

class MonsterData : public Common::Array<MonsterStruct> {
private:
	void synchronize(Common::SeekableReadStream &s);
public:
	MonsterData();

	void load(const Common::String &name);
};

class SurroundingMazes {
public:
	int _north;
	int _east;
	int _south;
	int _west;
public:
	SurroundingMazes();

	void clear();

	void synchronize(Common::SeekableReadStream &s);

	int &operator[](int idx);
};

class MazeDifficulties {
public:
	int _wallNoPass;
	int _surfaceNoPass;
	int _unlockDoor;
	int _unlockBox;
	int _bashDoor;
	int _bashGrate;
	int _bashWall;
	int _chance2Run;
public:
	MazeDifficulties();

	void synchronize(Common::SeekableReadStream &s);
};

enum MazeFlags {
	OUTFLAG_GRATE = 0x80, OUTFLAG_DRAIN = 0x20, OUTFLAG_OBJECT_EXISTS = 0x08,
	INFLAG_INSIDE = 0x08, FLAG_AUTOEXECUTE_EVENT = 0x10,
	RESTRICTION_ETHERIALIZE = 0x40, RESTRICTION_80 = 0x80, 
	RESTRICTION_TOWN_PORTAL = 0x100, RESTRICTION_SUPER_SHELTER = 0x200,
	RESTRICTION_TIME_DISTORTION = 0x400, RESTRICTION_LLOYDS_BEACON = 0x800,
	RESTRICTION_TELPORT = 0x1000, RESTRICTION_2000 = 0x2000,
	RESTRICTION_REST = 0x4000, RESTRICTION_SAVE = 0x8000,

	FLAG_GROUND_BITS = 7
};

enum MazeFlags2 { FLAG_IS_OUTDOORS = 0x8000, FLAG_IS_DARK = 0x4000 };

enum SurfaceType {
	SURFTYPE_DEFAULT = 0, 
	SURFTYPE_WATER = 0, SURFTYPE_DIRT = 1, SURFTYPE_GRASS = 2,
	SURFTYPE_SNOW = 3, SURFTYPE_SWAMP = 4, SURFTYPE_LAVA = 5,
	SURFTYPE_DESERT = 6, SURFTYPE_ROAD = 7, SURFTYPE_DWATER = 8,
	SURFTYPE_TFLR = 9, SURFTYPE_SKY = 10, SURFTYPE_CROAD = 11,
	SURFTYPE_SEWER = 12, SURFTYPE_CLOUD = 13, SURFTYPE_SCORCH = 14,
	SURFTYPE_SPACE = 15
};

union MazeWallLayers {
	struct MazeWallIndoors {
		int _wallNorth : 4;
		int _wallEast : 4;
		int _wallSouth : 4;
		int _wallWest : 4;
	} _indoors;
	struct MazeWallOutdoors {
		SurfaceType _surfaceId : 4;
		int _iMiddle : 4;
		int _iTop : 4;
		int _iOverlay : 4;
	} _outdoors;
	uint16 _data;
};

struct MazeCell {
	int _flags;
	int _surfaceId;
	MazeCell() : _flags(0), _surfaceId(0) {}
};

class MazeData {
public:
	// Resource fields
	MazeWallLayers _wallData[MAP_HEIGHT][MAP_WIDTH];
	MazeCell _cells[MAP_HEIGHT][MAP_WIDTH];
	int _mazeNumber;
	SurroundingMazes _surroundingMazes;
	int _mazeFlags;
	int _mazeFlags2;
	int _wallTypes[16];
	int _surfaceTypes[16];
	int _floorType;
	Common::Point _runPosition;
	MazeDifficulties _difficulties;
	int _trapDamage;
	int _wallKind;
	int _tavernTips;
	bool _seenTiles[MAP_HEIGHT][MAP_WIDTH];
	bool _steppedOnTiles[MAP_HEIGHT][MAP_WIDTH];

	// Misc fields
	int _mazeId;
public:
	MazeData();

	void clear();

	void synchronize(Common::SeekableReadStream &s);

	void setAllTilesStepped();

	void clearCellSurfaces();
};

class MobStruct {
public:
	Common::Point _pos;
	int _id;
	Direction _direction;
public:
	MobStruct();

	bool synchronize(XeenSerializer &s);
};

struct MazeObject {
public:
	Common::Point _position;
	int _id;
	int _frame;
	int _spriteId;
	Direction _direction;
	bool _flipped;
	SpriteResource *_sprites;

	MazeObject();
};

struct MazeMonster {
	Common::Point _position;
	int _frame;
	int _id;
	int _spriteId;
	bool _isAttacking;
	int _damageType;
	int _field9;
	int _fieldA;
	int _hp;
	int _effect1, _effect2;
	int _effect3;
	SpriteResource *_sprites;
	SpriteResource *_attackSprites;

	MazeMonster();
};

class MazeWallItem {
public:
	Common::Point _position;
	int _id;
	int _frame;
	int _spriteId;
	Direction _direction;
	SpriteResource *_sprites;
public:
	MazeWallItem();
};

struct WallSprites {
	SpriteResource _surfaces[TOTAL_SURFACES];
	SpriteResource _fwl1;
	SpriteResource _fwl2;
	SpriteResource _fwl3;
	SpriteResource _fwl4;
	SpriteResource _swl;
};

class Map;

class MonsterObjectData {
	friend class Map;
public:
	struct SpriteResourceEntry {
		int _spriteId;
		SpriteResource _sprites;
		SpriteResource _attackSprites;

		SpriteResourceEntry() { _spriteId = -1; }
		SpriteResourceEntry(int spriteId): _spriteId(spriteId) { }
	};
private:
	XeenEngine *_vm;
	Common::Array<SpriteResourceEntry> _objectSprites;
	Common::Array<SpriteResourceEntry> _monsterSprites;
	Common::Array<SpriteResourceEntry> _monsterAttackSprites;
	Common::Array<SpriteResourceEntry> _wallItemSprites;
public:
	Common::Array<MazeObject> _objects;
	Common::Array<MazeMonster> _monsters;
	Common::Array<MazeWallItem> _wallItems;
public:
	MonsterObjectData(XeenEngine *vm);

	void synchronize(XeenSerializer &s, MonsterData monsterData);
};

class HeadData {
public:
	struct HeadEntry {
		int _left;
		int _right;
	};
	HeadEntry _data[MAP_HEIGHT][MAP_WIDTH];
public:
	HeadData();

	void synchronize(Common::SeekableReadStream &s);

	HeadEntry *operator[](int y) { return &_data[y][0]; }
};

struct AnimationFrame { int _front, _left, _back, _right; };
struct AnimationFlipped { bool _front, _left, _back, _right; };
struct AnimationEntry {
	union {
		AnimationFrame _positions;
		int _frames[4];
	} _frame1;
	union {
		AnimationFlipped _positions;
		bool _flags[4];
	} _flipped;
	union {
		AnimationFrame _positions;
		int _frames[4];
	} _frame2;

	void synchronize(Common::SeekableReadStream &s);
};

class AnimationInfo : public Common::Array<AnimationEntry> {
public:
	void synchronize(Common::SeekableReadStream &s);

	void load(const Common::String &name);
};

class Map {
private:
	XeenEngine *_vm;
	MazeData _mazeData[9];
	SpriteResource _wallPicSprites;
	int _sideTownPortal;
	int _sidePictures;
	int _sideObjects;
	int _sideMonsters;
	int _mazeDataIndex;

	void loadEvents(int mapId);
public:
	Common::String _mazeName;
	bool _isOutdoors;
	MonsterObjectData _mobData;
	MonsterData _monsterData;
	MazeEvents _events;
	HeadData _headData;
	AnimationInfo _animationInfo;
	SpriteResource _skySprites[2];
	SpriteResource _groundSprites;
	SpriteResource _tileSprites;
	SpriteResource _surfaceSprites[TOTAL_SURFACES];
	WallSprites _wallSprites;
	bool _currentIsGrate;
	bool _currentCantRest;
	bool _currentIsDrain;
	bool _currentIsEvent;
	int _currentSky;
	int _currentMonsterFlags;
	int _currentWall;
	int _currentTile;
	int _currentSurfaceId;
	bool _currentSteppedOn;
	bool _loadDarkSide;
public:
	Map(XeenEngine *vm);

	void load(int mapId);

	int mazeLookup(const Common::Point &pt, int layerShift, int wallMask = 0xf);

	void cellFlagLookup(const Common::Point &pt);

	void setCellSurfaceFlags(const Common::Point &pt, int bits);

	void setWall(const Common::Point &pt, Direction dir, int v);

	void saveMaze();

	int getCell(int idx);

	MazeData &mazeData() { return _mazeData[0]; }

	MazeData &mazeDataCurrent() { return _mazeData[_mazeDataIndex]; }

	void loadSky();

	void getNewMaze();
};

} // End of namespace Xeen

#endif	/* XEEN_MAP_H */