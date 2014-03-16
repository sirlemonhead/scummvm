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

#ifndef MADS_GLOBALS_NEBULAR_H
#define MADS_GLOBALS_NEBULAR_H

#include "common/scummsys.h"
#include "common/array.h"
#include "mads/game.h"

namespace MADS {

namespace Nebular {

class Globals {
private:
	Common::Array<int16> _flags;
public:
	Common::Array<int> _spriteIndexes;

	int _v0;
	uint32 _frameTime;
	int _v2;
	int _v3;
	int _v4;
	int _v5;
	int _v6;
	int _v7;
	int _v8;
public:
	/**
	 * Constructor
	 */
	Globals();

	/**
	 * Square brackets operator for accessing flags
	 */
	int16 &operator[](int idx) { return _flags[idx]; }
};

} // End of namespace Nebular

} // End of namespace MADS

#endif /* MADS_GLOBALS_NEBULAR_H */