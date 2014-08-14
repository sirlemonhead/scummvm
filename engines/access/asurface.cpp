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

#include "common/algorithm.h"
#include "common/endian.h"
#include "common/memstream.h"
#include "access/access.h"
#include "access/asurface.h"

namespace Access {

SpriteResource::SpriteResource(AccessEngine *vm, const byte *data, uint32 size,
		DisposeAfterUse::Flag disposeMemory) {
	Common::MemoryReadStream stream(data, size);
	Common::Array<uint32> offsets;
	int count = stream.readUint16LE();

	for (int i = 0; i < count; i++)
		offsets.push_back(stream.readUint32LE());
	offsets.push_back(size);	// For easier calculations of Noctropolis sizes

	// Build up the frames
	for (int i = 0; i < count; ++i) {
		stream.seek(offsets[i]);
		int frameSize = offsets[i + 1] - offsets[i];

		SpriteFrame *frame = new SpriteFrame(vm, stream, frameSize);
		_frames.push_back(frame);
	}

	if (disposeMemory == DisposeAfterUse::YES)
		delete[] data;
}

SpriteResource::~SpriteResource() {
	for (uint i = 0; i < _frames.size(); ++i)
		delete _frames[i];
}

SpriteFrame::SpriteFrame(AccessEngine *vm, Common::MemoryReadStream &stream, int frameSize) {
	int w = stream.readUint16LE();
	int h = stream.readUint16LE();
	create(w, h, Graphics::PixelFormat::createFormatCLUT8());
	
	// Empty surface
	byte *data = (byte *)getPixels();
	Common::fill(data, data + w * h, 0);
	
	// Decode the data
	for (int y = 0; y < h; ++y) {
		int offset = stream.readByte();
		int len = stream.readByte();
		assert((offset + len) <= w);

		byte *destP = (byte *)getBasePtr(offset, y);
		stream.read(destP, len);
	}
}

SpriteFrame::~SpriteFrame() {
	free();
}

/*------------------------------------------------------------------------*/

int ASurface::_leftSkip;
int ASurface::_rightSkip;
int ASurface::_topSkip;
int ASurface::_bottomSkip;
int ASurface::_clipWidth;
int ASurface::_clipHeight;
int ASurface::_lastBoundsX;
int ASurface::_lastBoundsY;
int ASurface::_lastBoundsW;
int ASurface::_lastBoundsH;
int ASurface::_scrollX;
int ASurface::_scrollY;

void ASurface::init() {
	_leftSkip = _rightSkip = 0;
	_topSkip = _bottomSkip = 0;
	_clipWidth = _clipHeight = 0;
	_lastBoundsX = _lastBoundsY = 0;
	_lastBoundsW = _lastBoundsH = 0;
	_scrollX = _scrollY = 0;
}

void ASurface::clearBuffer() {
	byte *pSrc = (byte *)getPixels();
	Common::fill(pSrc, pSrc + w * h, 0);
}

bool ASurface::clip(Common::Rect &r) {
	int skip;
	_leftSkip = _rightSkip = 0;
	_topSkip = _bottomSkip = 0;

	if (r.left > _clipWidth) {
		if (r.left >= 0)
			return true;

		skip = -r.left;
		r.setWidth(r.width() - skip);
		_leftSkip = skip;
		r.moveTo(0, r.top);
	}

	int right = r.right - 1;
	if (right < 0)
		return true;
	else if (right > _clipWidth) {
		skip = right - _clipWidth;
		r.setWidth(r.width() - skip);
		_rightSkip = skip;
	}

	if (r.top > _clipHeight) {
		if (r.top >= 0)
			return true;

		skip = -r.top;
		r.setHeight(r.height() - skip);
		_topSkip = skip;
		r.moveTo(r.left, 0);
	}

	int bottom = r.bottom - 1;
	if (bottom < 0)
		return true;
	else if (bottom > _clipHeight) {
		skip = bottom - _clipHeight;
		_bottomSkip = skip;
		r.setHeight(r.height() - skip);
	}

	return false;
}

void ASurface::plotImage(SpriteResource *sprite, int frameNum, const Common::Point &pt) {
	SpriteFrame *frame = sprite->getFrame(frameNum);
	Common::Rect r(pt.x, pt.y, pt.x + frame->w, pt.y + frame->h);

	if (!clip(r)) {
		_lastBoundsX = r.left;
		_lastBoundsY = r.top;
		_lastBoundsW = r.width();
		_lastBoundsH = r.height();

		plotFrame(frame, pt);
	}
}

void ASurface::plotFrame(SpriteFrame *frame, const Common::Point &pt) {
	frame->copyTo(this, pt);
//	g_system->copyRectToScreen((byte *)getPixels(), 320, 0, 0, 320, 200);
//	g_system->updateScreen();


/*
	byte *destP = (byte *)getBasePtr(pt.x, _scrollY + pt.y);
	byte *srcP = frame->_data;
	
	int8 leftVal1 = 18;
	int8 leftVal2 = -8;
	if (_leftSkip) {
		++leftVal2;
		leftVal1 = -12;
	}
	int8 rightVal = (_rightSkip) ? -7 : -8;

	// Skip over any lines of the frame
	for (int yp = 0; yp < _topSkip; ++yp) {
		srcP += *(srcP + 1) + 2;
	}
	
	byte *srcLineP = srcP;
	byte *destLineP = destP;
	for (int yp = 0; yp < frame->h; ++yp, srcP = srcLineP, destP = destLineP) {
		// Get length of line
		int v = *srcP++;
		int len = *srcP++;
		srcLineP = srcP + len;
		destLineP = destP + this->pitch;

		// Draw the line of the frame
		if (v != 0 || len != 0) {
			warning("TODO: Line draw");
		}
	}
	*/
}

void ASurface::copyTo(ASurface *dest, const Common::Point &destPos) {
	for (int yp = 0; yp < h; ++yp) {
		byte *srcP = (byte *)getBasePtr(0, yp);
		byte *destP = (byte *)dest->getBasePtr(destPos.x, destPos.y + yp);

		for (int xp = 0; xp < this->w; ++xp, ++srcP, ++destP) {
			if (*srcP != 0)
				*destP = *srcP;
		}
	}
}

void ASurface::sPlotB(SpriteFrame *frame, const Common::Point &pt) {
	error("TODO");
}

void ASurface::sPlotF(SpriteFrame *frame, const Common::Point &pt) {
	error("TODO");
}

void ASurface::plotB(SpriteFrame *frame, const Common::Point &pt) {
	error("TODO");
}

void ASurface::copyBlock(ASurface *src, const Common::Rect &bounds) {
	byte *srcP = (byte *)getBasePtr(bounds.left, bounds.top + _scrollY);
	byte *destP = (byte *)getBasePtr(bounds.left, bounds.top); /* + _windowYAdd */

	for (int y = 0; y < bounds.height(); ++y) {
		Common::copy(srcP, srcP + bounds.width(), destP);
		srcP += src->pitch;
		destP += this->pitch;
	}
}

} // End of namespace Access