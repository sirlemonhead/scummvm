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

#ifndef MADS_GRAPHICS_H
#define MADS_GRAPHICS_H

#include "common/scummsys.h"
#include "common/array.h"
#include "mads/msurface.h"

namespace MADS {

#define MADS_SCREEN_WIDTH 320
#define MADS_SCREEN_HEIGHT 200

class ScreenSurface : public MSurface {
public:
	Common::Point _offset;
	byte *_pixels;
	int _pitch;
public:
	/**
	 * Constructor
	 */
	ScreenSurface() {}

	/**
	 * Initialise the surface
	 */
	void init();

	void setPointer(MSurface *s);

	/**
	 * Updates the screen with the contents of the surface
	 */
	void updateScreen();

	void transition(bool transitionFlag, bool surfaceFlag);
};

} // End of namespace MADS

#endif /* MADS_GRAPHICS_H */