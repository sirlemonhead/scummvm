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

#include "engines/util.h"
#include "mads/compression.h"
#include "mads/screen.h"
#include "mads/mads.h"
#include "mads/msurface.h"
#include "mads/resources.h"
#include "mads/sprites.h"

namespace MADS {

MADSEngine *MSurface::_vm = nullptr;

MSurface::MSurface() {
	pixels = nullptr;
	_freeFlag = false;
}

MSurface::MSurface(int width, int height) { 
	pixels = nullptr;
	_freeFlag = false;
	setSize(width, height); 
}

MSurface::~MSurface() {
	if (_freeFlag)
		Graphics::Surface::free();
}

void MSurface::setSize(int width, int height) {
	if (_freeFlag)
		Graphics::Surface::free();
	Graphics::Surface::create(width, height, Graphics::PixelFormat::createFormatCLUT8());
	_freeFlag = true;
}

void MSurface::setPixels(byte *pData, int horizSize, int vertSize) {
	_freeFlag = false;
	pixels = pData;
	w = pitch = horizSize;
	h = vertSize;
	format.bytesPerPixel = 1;
}

int MSurface::scaleValue(int value, int scale, int err) {
	int scaled = 0;
	while (value--) {
		err -= scale;
		while (err < 0) {
			scaled++;
			err += 100;
		}
	}
	return scaled;
}

void MSurface::drawSprite(const Common::Point &pt, SpriteInfo &info, const Common::Rect &clipRect) {

	enum {
		kStatusSkip,
		kStatusScale,
		kStatusDraw
	};

	// NOTE: The current clipping code assumes that the top left corner of the clip
	// rectangle is always 0, 0
	assert(clipRect.top == 0 && clipRect.left == 0);

	// TODO: Put err* and scaled* into SpriteInfo
	int errX = info.hotX * info.scaleX % 100;
	int errY = info.hotY * info.scaleY % 100;
	int scaledWidth = scaleValue(info.width, info.scaleX, errX);
	int scaledHeight = scaleValue(info.height, info.scaleY, errY);

	int x = pt.x, y = pt.y;
	int clipX = 0, clipY = 0;
	// Clip the sprite's width and height according to the clip rectangle's dimensions
	// This clips the sprite to the bottom and right
	if (x >= 0) {
		scaledWidth = MIN<int>(x + scaledWidth, clipRect.right) - x;
	} else {
		clipX = x;
		scaledWidth = x + scaledWidth;
	}
	if (y >= 0) {
		scaledHeight = MIN<int>(y + scaledHeight, clipRect.bottom) - y;
	} else {
		clipY = y;
		scaledHeight = y + scaledHeight;
	}

	// Check if sprite is inside the screen. If it's not, there's no need to draw it
	if (scaledWidth + x <= 0 || scaledHeight + y <= 0)	// check left and top (in case x,y are negative)
		return;
	if (scaledWidth <= 0 || scaledHeight <= 0)			// check right and bottom
		return;
	int heightAmt = scaledHeight;

	byte *src = info.sprite->getData();
	byte *dst = getBasePtr(x - info.hotX - clipX, y - info.hotY - clipY);

	int status = kStatusSkip;
	byte *scaledLineBuf = new byte[scaledWidth];

	while (heightAmt > 0) {

		if (status == kStatusSkip) {
			// Skip line
			errY -= info.scaleY;
			if (errY < 0)
				status = kStatusScale;
			else
				src += info.width;
		} else {

			if (status == kStatusScale) {
				// Scale current line
				byte *lineDst = scaledLineBuf;
				int curErrX = errX;
				int width = scaledWidth;
				byte *tempSrc = src;
				int startX = clipX;
				while (width > 0) {
					byte pixel = *tempSrc++;
					curErrX -= info.scaleX;
					while (curErrX < 0) {
						if (startX == 0) {
							*lineDst++ = pixel;
							width--;
						} else {
							startX++;
						}
						curErrX += 100;
					}
				}
				src += info.width;
				status = kStatusDraw;
			}

			if (status == kStatusDraw && clipY == 0) {
				// Draw previously scaled line
				// TODO Implement different drawing types (depth, shadow etc.)
				byte *tempDst = dst;
				for (int lineX = 0; lineX < scaledWidth; lineX++) {
					byte pixel = scaledLineBuf[lineX];

					if (info.encoding & 0x80) {

						if (pixel == 0x80) {
							pixel = 0;
						} else {
							byte destPixel = *tempDst;
							byte r, g, b;
							r = CLIP((info.palette[destPixel * 3] * pixel) >> 10, 0, 31);
							g = CLIP((info.palette[destPixel * 3 + 1] * pixel) >> 10, 0, 31);
							b = CLIP((info.palette[destPixel * 3 + 2] * pixel) >> 10, 0, 31);
							pixel = info.inverseColorTable[(b << 10) | (g << 5) | r];
						}
					}

					if (pixel)
						*tempDst = pixel;

					tempDst++;
				}
				dst += pitch;
				heightAmt--;
				// TODO depth etc.
				//depthAddress += Destination -> Width;

				errY += 100;
				if (errY >= 0)
					status = kStatusSkip;
			} else if (status == kStatusDraw && clipY < 0) {
				clipY++;

				errY += 100;
				if (errY >= 0)
					status = kStatusSkip;
			}

		}

	}
	
	delete[] scaledLineBuf;

}

void MSurface::empty() {
	Common::fill(getBasePtr(0, 0), getBasePtr(0, h), 0);
}

void MSurface::copyFrom(MSurface *src, const Common::Rect &srcBounds, 
		const Common::Point &destPos, int transparentColor) {
	// Validation of the rectangle and position	
	int destX = destPos.x, destY = destPos.y;		
	if ((destX >= w) || (destY >= h))
		return;

	Common::Rect copyRect = srcBounds;
	if (destX < 0) {
		copyRect.left += -destX;
		destX = 0;
	} else if (destX + copyRect.width() > w) {
		copyRect.right -= destX + copyRect.width() - w;
	}
	if (destY < 0) {
		copyRect.top += -destY;
		destY = 0;
	} else if (destY + copyRect.height() > h) {
		copyRect.bottom -= destY + copyRect.height() - h;
	}

	if (!copyRect.isValidRect())
		return;

	// Copy the specified area

	byte *data = src->getData();
	byte *srcPtr = data + (src->getWidth() * copyRect.top + copyRect.left);
	byte *destPtr = (byte *)pixels + (destY * getWidth()) + destX;

	for (int rowCtr = 0; rowCtr < copyRect.height(); ++rowCtr) {
		if (transparentColor == -1) {
			// No transparency, so copy line over
			Common::copy(srcPtr, srcPtr + copyRect.width(), destPtr);
		} else {
			// Copy each byte one at a time checking for the transparency color
			for (int xCtr = 0; xCtr < copyRect.width(); ++xCtr)
				if (srcPtr[xCtr] != transparentColor) destPtr[xCtr] = srcPtr[xCtr];
		}

		srcPtr += src->getWidth();
		destPtr += getWidth();
	}
}

void MSurface::copyFrom(MSurface *src, const Common::Point &destPos, int depth,
	MSurface *depthsSurface, int scale, int transparentColor) {

	int destX = destPos.x, destY = destPos.y;
	if (scale == 100) {
		// Copy the specified area
		Common::Rect copyRect(0, 0, src->getWidth(), src->getHeight());

		if (destX < 0) {
			copyRect.left += -destX;
			destX = 0;
		}
		else if (destX + copyRect.width() > w) {
			copyRect.right -= destX + copyRect.width() - w;
		}
		if (destY < 0) {
			copyRect.top += -destY;
			destY = 0;
		}
		else if (destY + copyRect.height() > h) {
			copyRect.bottom -= destY + copyRect.height() - h;
		}

		if (!copyRect.isValidRect())
			return;

		byte *data = src->getData();
		byte *srcPtr = data + (src->getWidth() * copyRect.top + copyRect.left);
		byte *depthsData = depthsSurface->getData();
		byte *depthsPtr = depthsData + (depthsSurface->pitch * destY) + destX;
		byte *destPtr = (byte *)pixels + (destY * pitch) + destX;

		// 100% scaling variation
		for (int rowCtr = 0; rowCtr < copyRect.height(); ++rowCtr) {
			// Copy each byte one at a time checking against the depth
			for (int xCtr = 0; xCtr < copyRect.width(); ++xCtr) {
				if ((depth <= (depthsPtr[xCtr] & 0x7f)) && (srcPtr[xCtr] != transparentColor))
					destPtr[xCtr] = srcPtr[xCtr];
			}

			srcPtr += src->getWidth();
			depthsPtr += depthsSurface->getWidth();
			destPtr += getWidth();
		}

		return;
	}

	// Start of draw logic for scaled sprites
	const byte *srcPixelsP = src->getData();

	int destRight = this->getWidth() - 1;
	int destBottom = this->getHeight() - 1;
	bool normalFrame = true;	// TODO: false for negative frame numbers
	int frameWidth = src->getWidth();
	int frameHeight = src->getHeight();

	int highestDim = MAX(frameWidth, frameHeight);
	bool lineDist[MADS_SCREEN_WIDTH];
	int distIndex = 0;
	int distXCount = 0, distYCount = 0;

	int distCtr = 0;
	do {
		distCtr += scale;
		if (distCtr < 100) {
			lineDist[distIndex] = false;
		} else {
			lineDist[distIndex] = true;
			distCtr -= 100;

			if (distIndex < frameWidth)
				++distXCount;

			if (distIndex < frameHeight)
				++distYCount;
		}
	} while (++distIndex < highestDim);

	destX -= distXCount / 2;
	destY -= distYCount - 1;

	// Check x bounding area
	int spriteLeft = 0;
	int spriteWidth = distXCount;
	int widthAmount = destX + distXCount - 1;

	if (destX < 0) {
		spriteWidth += destX;
		spriteLeft -= destX;
	}
	widthAmount -= destRight;
	if (widthAmount > 0)
		spriteWidth -= widthAmount;

	int spriteRight = spriteLeft + spriteWidth;
	if (spriteWidth <= 0)
		return;
	if (!normalFrame) {
		destX += distXCount - 1;
		spriteLeft = -(distXCount - spriteRight);
		spriteRight = (-spriteLeft + spriteWidth);
	}

	// Check y bounding area
	int spriteTop = 0;
	int spriteHeight = distYCount;
	int heightAmount = destY + distYCount - 1;

	if (destY < 0) {
		spriteHeight += destY;
		spriteTop -= destY;
	}
	heightAmount -= destBottom;
	if (heightAmount > 0)
		spriteHeight -= heightAmount;
	int spriteBottom = spriteTop + spriteHeight;

	if (spriteHeight <= 0)
		return;

	byte *destPixelsP = this->getBasePtr(destX + spriteLeft, destY + spriteTop);
	const byte *depthPixelsP = depthsSurface->getBasePtr(destX + spriteLeft, destY + spriteTop);

	spriteLeft = (spriteLeft * (normalFrame ? 1 : -1));

	// Loop through the lines of the sprite
	for (int yp = 0, sprY = -1; yp < frameHeight; ++yp, srcPixelsP += src->pitch) {
		if (!lineDist[yp])
			// Not a display line, so skip it
			continue;
		// Check whether the sprite line is in the display range
		++sprY;
		if ((sprY >= spriteBottom) || (sprY < spriteTop))
			continue;

		// Found a line to display. Loop through the pixels
		const byte *srcP = srcPixelsP;
		const byte *depthP = depthPixelsP;
		byte *destP = destPixelsP;
		for (int xp = 0, sprX = 0; xp < frameWidth; ++xp, ++srcP) {
			if (xp < spriteLeft)
				// Not yet reached start of display area
				continue;
			if (!lineDist[sprX++])
				// Not a display pixel
				continue;

			if ((*srcP != transparentColor) && (depth <= (*depthP & 0x7f)))
				*destP = *srcP;

			++destP;
			++depthP;
		}

		// Move to the next destination line
		destPixelsP += this->pitch;
		depthPixelsP += depthsSurface->pitch;
	}
}

void MSurface::scrollX(int xAmount) {
	if (xAmount == 0)
		return;

	byte buffer[80];
	int direction = (xAmount > 0) ? -1 : 1;
	int xSize = ABS(xAmount);
	assert(xSize <= 80);

	byte *srcP = getBasePtr(0, 0);

	for (int y = 0; y < this->h; ++y, srcP += pitch) {
		if (direction < 0) {
			// Copy area to be overwritten
			Common::copy(srcP, srcP + xSize, &buffer[0]);
			// Shift the remainder of the line over the given area
			Common::copy(srcP + xSize, srcP + this->w, srcP);
			// Move buffered area to the end of the line
			Common::copy(&buffer[0], &buffer[xSize], srcP + this->w - xSize);
		} else {
			// Copy area to be overwritten
			Common::copy_backward(srcP + this->w - xSize, srcP + this->w, &buffer[80]);
			// Shift the remainder of the line over the given area
			Common::copy_backward(srcP, srcP + this->w - xSize, srcP + this->w);
			// Move buffered area to the start of the line
			Common::copy_backward(&buffer[80 - xSize], &buffer[80], srcP + xSize);
		}
	}
}

void MSurface::scrollY(int yAmount) {
	if (yAmount == 0)
		return;

	int direction = (yAmount > 0) ? 1 : -1;
	int ySize = ABS(yAmount);
	assert(ySize < (this->h / 2));
	assert(this->w == pitch);

	int blockSize = ySize * this->w;
	byte *tempData = new byte[blockSize];
	byte *pixelsP = getBasePtr(0, 0);

	if (direction > 0) {
		// Buffer the lines to be overwritten
		byte *srcP = (byte *)getBasePtr(0, this->h - ySize);
		Common::copy(srcP, srcP + (pitch * ySize), tempData);
		// Vertically shift all the lines
		Common::copy_backward(pixelsP, pixelsP + (pitch * (this->h - ySize)),
			pixelsP + (pitch * this->h));
		// Transfer the buffered lines top the top of the screen
		Common::copy(tempData, tempData + blockSize, pixelsP);
	} else {
		// Buffer the lines to be overwritten
		Common::copy(pixelsP, pixelsP + (pitch * ySize), tempData);
		// Vertically shift all the lines
		Common::copy(pixelsP + (pitch * ySize), pixelsP + (pitch * this->h), pixelsP);
		// Transfer the buffered lines to the bottom of the screen
		Common::copy(tempData, tempData + blockSize, pixelsP + (pitch * (this->h - ySize)));
	}

	delete[] tempData;
}


void MSurface::translate(Common::Array<RGB6> &palette) {
	for (int y = 0; y < this->h; ++y) {
		byte *pDest = getBasePtr(0, y);

		for (int x = 0; x < this->w; ++x, ++pDest) {
			if (*pDest < 255)	// scene 752 has some palette indices of 255
				*pDest = palette[*pDest]._palIndex;
		}
	}
}

MSurface *MSurface::flipHorizontal() const {
	MSurface *dest = new MSurface(this->w, this->h);

	for (int y = 0; y < this->h; ++y) {
		const byte *srcP = getBasePtr(this->w - 1, y);
		byte *destP = dest->getBasePtr(0, y);

		for (int x = 0; x < this->w; ++x)
			*destP++ = *srcP--;
	}

	return dest;
}

/*------------------------------------------------------------------------*/

int DepthSurface::getDepth(const Common::Point &pt) {
	if (_vm->_game->_scene._sceneInfo->_depthStyle == 2) {
		int bits = (3 - (pt.x % 4)) * 2;
		byte v = *getBasePtr(pt.x >> 2, pt.y);
		return v >> bits;
	} else {
		if (pt.x < 0 || pt.y < 0 || pt.x >= this->w || pt.y >= this->h)
			return 0;

		return *getBasePtr(pt.x, pt.y) & 0xF;
	}
}

int DepthSurface::getDepthHighBit(const Common::Point &pt) {
	if (_vm->_game->_scene._sceneInfo->_depthStyle == 2) {
		int bits = (3 - (pt.x % 4)) * 2;
		byte v = *getBasePtr(pt.x >> 2, pt.y);
		return (v >> bits) & 2;
	} else {
		if (pt.x < 0 || pt.y < 0 || pt.x >= this->w || pt.y >= this->h)
			return 0;

		return *getBasePtr(pt.x, pt.y) & 0x80;
	}
}


} // End of namespace MADS