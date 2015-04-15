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

#include "sherlock/objects.h"
#include "sherlock/sherlock.h"
#include "sherlock/people.h"
#include "sherlock/scene.h"
#include "common/util.h"

namespace Sherlock {

#define START_FRAME 0

#define UPPER_LIMIT 0
#define LOWER_LIMIT CONTROLS_Y
#define LEFT_LIMIT 0
#define RIGHT_LIMIT SHERLOCK_SCREEN_WIDTH

// Distance to walk around WALK_AROUND boxes
#define CLEAR_DIST_X 5
#define CLEAR_DIST_Y 0

SherlockEngine *Sprite::_vm;

/**
 * Reset the data for the sprite
 */
void Sprite::clear() {
	_name = "";
	_description = "";
	_examine.clear();
	_pickUp = "";
	_sequences = nullptr;
	_images = nullptr;
	_imageFrame = nullptr;
	_walkCount = 0;
	_allow = 0;
	_frameNumber = _sequenceNumber = 0;
	_position.x = _position.y = 0;
	_delta.x = _delta.y = 0;
	_oldPosition.x = _oldPosition.y = 0;
	_oldSize.x = _oldSize.y = 0;
	_goto.x = _goto.y = 0;
	_type = INVALID;
	_pickUp.clear();
	_noShapeSize.x = _noShapeSize.y = 0;
	_status = 0;
	_misc = 0;
	_numFrames = 0;
}

/**
 * Updates the image frame poiner for the sprite
 */
void Sprite::setImageFrame() {
	int imageNumber = (*_sequences)[_sequenceNumber][_frameNumber] + 
		(*_sequences)[_sequenceNumber][0] - 2;
	_imageFrame = &(*_images)[imageNumber];
}

/**
 * This adjusts the sprites position, as well as it's animation sequence:
 */
void Sprite::adjustSprite() {
	People &people = *_vm->_people;
	Scene &scene = *_vm->_scene;
	Talk &talk = *_vm->_talk;

	if (_type == INVALID || (_type == CHARACTER && scene._animating))
		return;

	if (!talk._talkCounter && _type == CHARACTER && _walkCount) {
		// Handle active movement for the sprite
		_position += _delta;
		--_walkCount;

		if (!_walkCount) {
			// If there any points left for the character to walk to along the
			// route to a destination, then move to the next point
			if (!people._walkTo.empty()) {
				people._walkDest = people._walkTo.pop();
				people.setWalking();
			} else {
				people.gotoStand(*this);
			}
		}
	}

	if (_type == CHARACTER && !_vm->_onChessboard) {
		if ((_position.y / 100) > LOWER_LIMIT) {
			_position.y = LOWER_LIMIT * 100;
			people.gotoStand(*this);
		}

		if ((_position.y / 100) < UPPER_LIMIT) {
			_position.y = UPPER_LIMIT * 100;
			people.gotoStand(*this);
		}

		if ((_position.x / 100) < LEFT_LIMIT) {
			_position.x = LEFT_LIMIT * 100;
			people.gotoStand(*this);
		}
	} else if (!_vm->_onChessboard) {
		_position.y = CLIP((int)_position.y, UPPER_LIMIT, LOWER_LIMIT);
		_position.x = CLIP((int)_position.x, LEFT_LIMIT, RIGHT_LIMIT);
	}

	if (!_vm->_onChessboard || (_vm->_slowChess = !_vm->_slowChess))
		++_frameNumber;

	if ((*_sequences)[_sequenceNumber][_frameNumber] == 0) {
		switch (_sequenceNumber) {
		case STOP_UP:
		case STOP_DOWN:
		case STOP_LEFT:
		case STOP_RIGHT:
		case STOP_UPRIGHT:
		case STOP_UPLEFT:
		case STOP_DOWNRIGHT:
		case STOP_DOWNLEFT:
			// We're in a stop sequence, so reset back to the last frame, so
			// the character is shown as standing still
			--_frameNumber;
			break;

		default:
			// Move 1 past the first frame - we need to compensate, since we
			// already passed the frame increment
			_frameNumber = 1;
			break;
		}
	}

	// Update the _imageFrame to point to the new frame's image
	setImageFrame();

	// Check to see if character has entered an exit zone
	if (!_walkCount && scene._walkedInScene && scene._goToScene == -1) {
		Common::Rect charRect(_position.x / 100 - 5, _position.y / 100 - 2,
			_position.x / 100 + 5, _position.y / 100 + 2);
		Exit *exit = scene.checkForExit(charRect);

		if (exit) {
			scene._hsavedPos = exit->_people;
			scene._hsavedFs = exit->_peopleDir;

			if (scene._hsavedFs > 100 && scene._hsavedPos.x < 1)
				scene._hsavedPos.x = 100;
		}		
	}
}

/**
 * Checks the sprite's position to see if it's collided with any special objects
 */
void Sprite::checkSprite() {
	Events &events = *_vm->_events;
	People &people = *_vm->_people;
	Scene &scene = *_vm->_scene;
	Screen &screen = *_vm->_screen;
	Talk &talk = *_vm->_talk;
	Common::Point pt;
	Common::Rect objBounds;
	Common::Point spritePt(_position.x / 100, _position.y / 100);

	if (!talk._talkCounter && _type == CHARACTER) {
		pt = _walkCount ? _position + _delta : _position;
		pt.x /= 100;
		pt.y /= 100;

		for (uint idx = 0; idx < scene._bgShapes.size() && !talk._talkToAbort; ++idx) {
			Object &obj = scene._bgShapes[idx];

			if (obj._aType > PERSON && obj._type != INVALID && obj._type != HIDDEN) {
				if (obj._type == NO_SHAPE) {
					objBounds = Common::Rect(obj._position.x, obj._position.y,
						obj._position.x + obj._noShapeSize.x, obj._position.y + obj._noShapeSize.y);
				} else {
					int xp = obj._position.x + obj._imageFrame->_offset.x;
					int yp = obj._position.y + obj._imageFrame->_offset.y;
					objBounds = Common::Rect(xp, yp,
						xp + obj._imageFrame->_frame.w, yp + obj._imageFrame->_frame.h);
				}
			}

			if (objBounds.contains(pt)) {
				if (objBounds.contains(spritePt)) {
					// Current point is already inside the the bounds, so impact occurred
					// on a previous call. So simply do nothing until we're clear of the box
					switch (obj._aType) {
					case TALK_MOVE:
						if (_walkCount) {
							// Holmes is moving
							obj._type = HIDDEN;
							obj.setFlagsAndToggles();
							talk.talkTo(obj._use[0]._target);
						}
						break;

					case PAL_CHANGE:
					case PAL_CHANGE2:
						if (_walkCount) {
							int palStart = atoi(obj._use[0]._names[0].c_str()) * 3;
							int palLength = atoi(obj._use[0]._names[1].c_str()) * 3;
							int templ = atoi(obj._use[0]._names[2].c_str()) * 3;
							if (templ == 0)
								templ = 100;

							// Ensure only valid palette change data found
							if (palLength > 0) {
								// Figure out how far into the shape Holmes is so that we
								// can figure out what percentage of the original palette
								// to set the current palette to
								int palPercent = (pt.x - objBounds.left) * 100 / objBounds.width();
								palPercent = palPercent * templ / 100;
								if (obj._aType == PAL_CHANGE)
									// Invert percentage
									palPercent = 100 - palPercent;
								
								for (int idx = palStart; idx < (palStart + palLength); ++idx)
									screen._sMap[idx] = screen._cMap[idx] * palPercent / 100;

								events.pollEvents();
								screen.setPalette(screen._sMap);
							}
						}
						break;

					case TALK:
					case TALK_EVERY:
						obj._type = HIDDEN;
						obj.setFlagsAndToggles();
						talk.talkTo(obj._use[0]._target);
						break;

					default:
						break;
					}
				} else {
					// New impact just occurred
					switch (obj._aType) {
					case BLANK_ZONE:
						// A blank zone masks out all other remaining zones underneath it.
						// If this zone is hit, exit the outer loop so we do not check anymore
						return;

					case SOLID:
					case TALK:
						// Stop walking
						if (obj._aType == TALK) {
							obj.setFlagsAndToggles();
							talk.talkTo(obj._use[0]._target);
						} else {
							people.gotoStand(*this);
						}
						break;

					case TALK_EVERY:
						if (obj._aType == TALK_EVERY) {
							obj._type = HIDDEN;
							obj.setFlagsAndToggles();
							talk.talkTo(obj._use[0]._target);
						} else {
							people.gotoStand(*this);
						}
						break;

					case FLAG_SET:
						obj.setFlagsAndToggles();
						obj._type = HIDDEN;
						break;

					case WALK_AROUND:
						if (objBounds.contains(people._walkTo.top())) {
							// Reached zone
							people.gotoStand(*this);
						} else {
							// Destination not within box, walk to best corner
							Common::Point walkPos;

							if (spritePt.x >= objBounds.left && spritePt.x < objBounds.right) {
								// Impact occurred due to vertical movement. Determine whether to
								// travel to the left or right side
								if (_delta.x > 0)
									// Go to right side
									walkPos.x = objBounds.right + CLEAR_DIST_X;
								else if (_delta.x < 0)
									// Go to left side
									walkPos.x = objBounds.left - CLEAR_DIST_X;
								else {
									// Going straight up or down. So choose best side
									if (spritePt.x >= (objBounds.left + objBounds.width() / 2))
										walkPos.x = objBounds.right + CLEAR_DIST_X;
									else
										walkPos.x = objBounds.left - CLEAR_DIST_X;
								}

								walkPos.y = (_delta.y >= 0) ? objBounds.top - CLEAR_DIST_Y : 
									objBounds.bottom + CLEAR_DIST_Y;
							} else {
								// Impact occurred due to horizontal movement
								if (_delta.y > 0)
									// Go to bottom of box
									walkPos.y = objBounds.bottom + CLEAR_DIST_Y;
								else if (_delta.y < 0)
									// Go to top of box
									walkPos.y = objBounds.top - CLEAR_DIST_Y;
								else {
									// Going straight horizontal, so choose best side
									if (spritePt.y >= (objBounds.top + objBounds.height() / 2))
										walkPos.y = objBounds.bottom + CLEAR_DIST_Y;
									else
										walkPos.y = objBounds.top - CLEAR_DIST_Y;
								}

								walkPos.x = (_delta.x >= 0) ? objBounds.left - CLEAR_DIST_X :
									objBounds.right + CLEAR_DIST_X;
							}

							walkPos.x += people[AL]._imageFrame->_frame.w / 2;
							people._walkDest = walkPos;
							people._walkTo.push(walkPos);
							people.setWalking();
						}
						break;

					case DELTA:
						_position.x += 200;
						break;
					}
				}
			}
		}
	}
}

/*----------------------------------------------------------------*/

void ActionType::synchronize(Common::SeekableReadStream &s) {
	char buffer[12];
	
	_cAnimNum = s.readByte();
	_cAnimSpeed = s.readByte();

	for (int idx = 0; idx < 4; ++idx) {
		s.read(buffer, 12);
		_names[idx] = Common::String(buffer);
	}
}

/*----------------------------------------------------------------*/

void UseType::synchronize(Common::SeekableReadStream &s) {
	char buffer[12];

	_cAnimNum = s.readByte();
	_cAnimSpeed = s.readByte();

	for (int idx = 0; idx < 4; ++idx) {
		s.read(buffer, 12);
		_names[idx] = Common::String(buffer);
	}

	_useFlag = s.readSint16LE();
	_dFlag[0] = s.readSint16LE();
	_lFlag[0] = s.readSint16LE();
	_lFlag[1] = s.readSint16LE();

	s.read(buffer, 12);
	_target = Common::String(buffer);
}

/*----------------------------------------------------------------*/

SherlockEngine *Object::_vm;
bool Object::_countCAnimFrames;

void Object::setVm(SherlockEngine *vm) {
	_vm = vm;
	_countCAnimFrames = false;
}

Object::Object() {
	_sequenceOffset = 0;
	_sequences = nullptr;
	_images = nullptr;
	_imageFrame = nullptr;
	_walkCount = 0;
	_allow = 0;
	_frameNumber = 0;
	_sequenceNumber = 0;
	_type = INVALID;
	_pickup = 0;
	_defaultCommand = 0;
	_lookFlag = 0;
	_pickupFlag = 0;
	_requiredFlag = 0;
	_status = 0;
	_misc = 0;
	_maxFrames = 0;
	_flags = 0;
	_lookFrames = 0;
	_seqCounter = 0;
	_lookFacing = 0;
	_lookcAnim = 0;
	_seqStack = 0;
	_seqTo = 0;
	_descOffset = 0;
	_seqCounter2 = 0;
	_seqSize = 0;
}

/**
 * Load the object data from the passed stream
 */
void Object::synchronize(Common::SeekableReadStream &s) {
	char buffer[41];
	s.read(buffer, 12);
	_name = Common::String(buffer);
	s.read(buffer, 41);
	_description = Common::String(buffer);

	_examine.clear();
	_sequences = nullptr;
	_images = nullptr;
	_imageFrame = nullptr;

	s.skip(4);
	_sequenceOffset = s.readUint16LE();
	s.seek(10, SEEK_CUR);

	_walkCount = s.readByte();
	_allow = s.readByte();
	_frameNumber = s.readSint16LE();
	_sequenceNumber = s.readSint16LE();
	_position.x = s.readSint16LE();
	_position.y = s.readSint16LE();
	_delta.x = s.readSint16LE();
	_delta.y = s.readSint16LE();
	_type = (SpriteType)s.readUint16LE();
	_oldPosition.x = s.readSint16LE();
	_oldPosition.y = s.readSint16LE();
	_oldSize.x = s.readUint16LE();
	_oldSize.y = s.readUint16LE();
	_goto.x = s.readSint16LE();
	_goto.y = s.readSint16LE();
	
	_pickup = s.readByte();
	_defaultCommand = s.readByte();
	_lookFlag = s.readUint16LE();
	_pickupFlag = s.readUint16LE();
	_requiredFlag = s.readUint16LE();
	_noShapeSize.x = s.readUint16LE();
	_noShapeSize.y = s.readUint16LE();
	_status = s.readUint16LE();
	_misc = s.readByte();
	_maxFrames = s.readUint16LE();
	_flags = s.readByte();	
	_aOpen.synchronize(s);
	_aType = (AType)s.readByte();
	_lookFrames = s.readByte();
	_seqCounter = s.readByte();
	_lookPosition.x = s.readUint16LE();
	_lookPosition.y = s.readByte();
	_lookFacing = s.readByte();
	_lookcAnim = s.readByte();
	_aClose.synchronize(s);
	_seqStack = s.readByte();
	_seqTo = s.readByte();
	_descOffset = s.readUint16LE();
	_seqCounter2 = s.readByte();
	_seqSize = s.readUint16LE();
	s.skip(1);
	_aMove.synchronize(s);
	s.skip(8);
	
	for (int idx = 0; idx < 4; ++idx)
		_use[idx].synchronize(s);
}

/**
 * Toggle the type of an object between hidden and active
 */
void Object::toggleHidden() {
	if (_type != HIDDEN && _type != HIDE_SHAPE && _type != INVALID) {
		if (_seqTo != 0)
			_sequences[_frameNumber] = _seqTo + SEQ_TO_CODE + 128;
		_seqTo = 0;

		if (_images == nullptr || _images->size() == 0)
			// No shape to erase, so flag as hidden
			_type = HIDDEN;
		else
			// Otherwise, flag it to be hidden after it gets erased
			_type = HIDE_SHAPE;
	} else if (_type != INVALID) {
		if (_seqTo != 0)
			_sequences[_frameNumber] = _seqTo + SEQ_TO_CODE + 128;
		_seqTo = 0;

		_seqCounter = _seqCounter2 = 0;
		_seqStack = 0;
		_frameNumber = -1;

		if (_images == nullptr || _images->size() == 0) {
			_type = NO_SHAPE;
		} else {
			_type = ACTIVE_BG_SHAPE;
			int idx = _sequences[0];
			if (idx >= _maxFrames)
				// Turn on: set up first frame
				idx = 0;

			_imageFrame = &(*_images)[idx];
		}
	}
}

/**
 * Check the state of the object
 */
void Object::checkObject(Object &o) {
	Scene &scene = *_vm->_scene;
	Sound &sound = *_vm->_sound;
	int checkFrame = _allow ? MAX_FRAME : 32000;
	bool codeFound;

	if (_seqTo) {
		byte *ptr = &_sequences[_frameNumber];
		if (*ptr == _seqTo) {
			// The sequence is completed
			*ptr = _seqTo + SEQ_TO_CODE + 128;	// Reset to normal
			_seqTo = 0;
		} else {
			// Continue doing sequence
			if (*ptr > _seqTo)
				*ptr--;
			else
				*ptr++;

			return;
		}
	}

	++_frameNumber;

	do {
		// Check for end of sequence
		codeFound = checkEndOfSequence();

		if (_sequences[_frameNumber] >= 128 && _frameNumber < checkFrame) {
			codeFound = true;
			int v = _sequences[_frameNumber];

			if (v >= 228) {
				// Goto code found
				v -= 228;
				_seqCounter2 = _seqCounter;
				_seqStack = _frameNumber + 1;
				setObjSequence(v, false);
			} else if (v >= SOUND_CODE && (v <= (SOUND_CODE + 29))) {
				codeFound = true;
				++_frameNumber;
				v -= SOUND_CODE;

				if (sound._soundOn && !_countCAnimFrames) {
					if (!scene._sounds[v - 1]._name.empty() && sound._digitized)
						sound.playLoadedSound(v - 1, 0);
				}
			} else if (v >= FLIP_CODE && v <= (FLIP_CODE + 2)) {
				// Flip code
				codeFound = true;
				++_frameNumber;
				v -= FLIP_CODE;

				// Alter the flipped status
				switch (v) {
				case 0:
					// Clear the flag
					_flags &= ~2;
					break;
				case 1:
					// Set the flag
					_flags |= 2;
					break;
				case 2:
					// Toggle the flag
					_flags ^= 2;
					break;
				default:
					break;
				}
			} else {
				v -= 128;

				// 68-99 is a squence code
				if (v > SEQ_TO_CODE) {
					byte *p = &_sequences[_frameNumber];
					v -= SEQ_TO_CODE;	// # from 1-32
					_seqTo = v;
					*p = *(p - 1);

					if (*p > 128)
						// If the high bit is set, convert to a real frame
						*p -= (byte)(SEQ_TO_CODE - 128);

					if (*p > _seqTo)
						*p -= 1;
					else
						*p += 1;

					// Will be incremented below to return back to original value
					--_frameNumber;
					v = 0;
				} else if (v == 10) {
					// Set delta for objects
					Common::Point pt(_sequences[_frameNumber + 1], _sequences[_frameNumber + 2]);
					if (pt.x > 128)
						pt.x = (pt.x - 128) *  -1;
					else
						pt.x--;

					if (pt.y > 128)
						pt.y = (pt.y - 128) * -1;
					else
						pt.y--;

					_delta = pt;
					_frameNumber += 2;
				} else if (v < 4) {
					for (int idx = 0; idx < 4; ++idx) {
						o.checkNameForCodes(_use[v]._names[idx], nullptr);
					}

					if (_use[v]._useFlag)
						_vm->setFlags(_use[v]._useFlag);
				}

				++_frameNumber;
			}
		}
	} while (codeFound);
}

/**
 * This will check to see if the object has reached the end of a sequence.
 * If it has, it switch to whichever next sequence should be started.
 * @returns		true if the end of a sequence was reached
 */
bool Object::checkEndOfSequence() {
	Screen &screen = *_vm->_screen;
	int checkFrame = _allow ? MAX_FRAME : 32000;
	bool result = false;

	if (_type == REMOVE || _type == INVALID)
		return false;

	if (_sequences[_frameNumber] == 0 || _frameNumber >= checkFrame) {
		result = true;

		if (_frameNumber >= (checkFrame - 1)) {
			_frameNumber = START_FRAME;
		}  else {
			// Determine next sequence to use
			int seq = _sequences[_frameNumber + 1];

			if (seq == 99) {
				--_frameNumber;
				screen._backBuffer1.transBlitFrom(_imageFrame->_frame, _position);
				screen._backBuffer2.transBlitFrom(_imageFrame->_frame, _position);
				_type = INVALID;
			} else {
				setObjSequence(seq, false);
			}
		}

		if (_allow && _frameNumber == 0) {
			// canimation just ended
			if (_type != NO_SHAPE && _type != REMOVE) {
				_type = REMOVE;

				if (!_countCAnimFrames) {
					// Save details before shape is removed
					_delta.x = _imageFrame->_frame.w;
					_delta.y = _imageFrame->_frame.h;
					_position = _imageFrame->_offset;

					// Free the images
					delete _images;
				}
			} else {
				_type = INVALID;
			}
		}
	}

	return result;
}

/**
 * Scans through the sequences array and finds the designated sequence.
 * It then sets the frame number of the start of that sequence
 */
void Object::setObjSequence(int seq, bool wait) {
	Scene &scene = *_vm->_scene;
	int checkFrame = _allow ? MAX_FRAME : 32000;

	if (seq >= 128) {
		// Loop the sequence until the count exceeded
		seq -= 128;

		++_seqCounter;
		if (_seqCounter >= seq) {
			// Go to next sequence
			if (_seqStack) {
				_frameNumber = _seqStack;
				_seqStack = 0;
				_seqCounter = _seqCounter2;
				_seqCounter2 = 0;
				if (_frameNumber >= checkFrame)
					_frameNumber = START_FRAME;

				return;
			}

			_frameNumber += 2;
			if (_frameNumber >= checkFrame)
				_frameNumber = 0;

			_seqCounter = 0;
			if (_sequences[_frameNumber] == 0)
				seq = _sequences[_frameNumber + 1];
			else
				return;
		} else {
			// Find beginning of sequence
			do {
				--_frameNumber;
			} while (_frameNumber > 0 && _sequences[_frameNumber] != 0);

			if (_frameNumber != 0)
				_frameNumber += 2;

			return;
		}
	} else {
		// Reset sequence counter
		_seqCounter = 0;
	}

	int idx = 0;
	int seqCc = 0;

	while (seqCc < seq && idx < checkFrame) {
		++idx;
		if (_sequences[idx] == 0) {
			++seqCc;
			idx += 2;
		}
	}

	if (idx >= checkFrame)
		idx = 0;
	_frameNumber = idx;

	if (wait) {
		seqCc = idx;
		while (_sequences[idx] != 0)
			++idx;

		idx = idx - seqCc + 2;
		for (; idx > 0; --idx)
			scene.doBgAnim();
	}
}

/**
* Checks for codes
* @param name		The name to check for codes
* @param messages	Provides a lookup list of messages that can be printed
* @returns		0 if no codes are found, 1 if codes were found
*/
int Object::checkNameForCodes(const Common::String &name, const char *const messages[]) {
	People &people = *_vm->_people;
	Scene &scene = *_vm->_scene;
	Screen &screen = *_vm->_screen;
	Talk &talk = *_vm->_talk;
	UserInterface &ui = *_vm->_ui;
	bool printed = false;
	char ch;
	const char *p;

	scene.toggleObject(name);

	if (name.hasPrefix("*")) {
		// A code was found
		printed = true;
		ch = toupper(name[1]);

		switch (ch) {
		case 'C':
			talk.talkTo(name.c_str() + 2);
			break;

		case 'T':
		case 'B':
		case 'F':
		case 'W':
			// Nothing: action was already done before canimation
			break;

		case 'G':
		case 'A': {
			// G: Have object go somewhere
			// A: Add onto existing co-ordinates
			Common::String sx(name.c_str() + 2, name.c_str() + 5);
			Common::String sy(name.c_str() + 6, name.c_str() + 9);
			
			if (ch == 'G')			
				_position = Common::Point(atoi(sx.c_str()), atoi(sy.c_str()));
			else
				_position += Common::Point(atoi(sx.c_str()), atoi(sy.c_str()));
			break;
		}

		default:
			if (ch >= '0' && ch <= '9') {
				scene._goToScene = atoi(name.c_str() + 1);

				if (scene._goToScene < 97 && _vm->_map[scene._goToScene].x) {
					_vm->_over.x = _vm->_map[scene._goToScene].x * 100 - 600;
					_vm->_over.y = _vm->_map[scene._goToScene].y * 100 + 900;
				}

				if ((p = strchr(name.c_str(), ',')) != nullptr) {
					++p;

					Common::String s(p, p + 3);
					scene._hsavedPos.x = atoi(s.c_str());

					s = Common::String(p + 3, p + 6);
					scene._hsavedPos.y = atoi(s.c_str());

					s = Common::String(p + 6, p + 9);
					scene._hsavedFs = atoi(s.c_str());
					if (scene._hsavedFs == 0)
						scene._hsavedFs = 10;
				} else if ((p = strchr(name.c_str(), '/')) != nullptr) {
					scene._hsavedPos = Common::Point(1, 0);
					scene._hsavedFs = 100 + atoi(p + 1);
				}
			} else {
				scene._goToScene = 100;
			}

			people[AL]._position = Common::Point(0, 0);
			break;
		}
	} else if (name.hasPrefix("!")) {
		// Message attached to canimation
		int messageNum = atoi(name.c_str() + 1);
		ui._infoFlag++;
		ui.clearInfo();
		screen.print(Common::Point(0, INFO_LINE + 1), INFO_FOREGROUND, messages[messageNum]);
		ui._menuCounter = 25;
	} else if (name.hasPrefix("@")) {
		// Message attached to canimation
		ui._infoFlag++;
		ui.clearInfo();
		screen.print(Common::Point(0, INFO_LINE + 1), INFO_FOREGROUND, name.c_str() + 1);
		printed = true;
		ui._menuCounter = 25;
	}

	return printed;
}

/**
 * Handle setting any flags associated with the object
 */
void Object::setFlagsAndToggles() {
	Scene &scene = *_vm->_scene;
	Talk &talk = *_vm->_talk;

	for (int useIdx = 0; useIdx < 4; ++useIdx) {
		if (_use[useIdx]._useFlag) {
			if (!_vm->readFlags(_use[useIdx]._useFlag))
				_vm->setFlags(_use[useIdx]._useFlag);
		}

		if (_use[useIdx]._cAnimSpeed) {
			if (_use[useIdx]._cAnimNum == 0)
				// 0 is really a 10
				scene.startCAnim(_use[useIdx]._cAnimNum - 1, _use[useIdx]._cAnimSpeed);
		}

		if (!talk._talkToAbort) {
			for (int idx = 0; idx < 4; ++idx)
				scene.toggleObject(_use[useIdx]._names[idx]);
		}
	}
}

/**
 * Adjusts the sprite's position and animation sequence, advancing by 1 frame.
 * If the end of the sequence is reached, the appropriate action is taken.
 */
void Object::adjustObject() {
	if (_type == REMOVE)
		return;

	_position += _delta;

	if (_position.y > LOWER_LIMIT)
		_position.y = LOWER_LIMIT;

	if (_type != NO_SHAPE) {
		int frame = _frameNumber;
		if (frame == -1)
			frame = 0;

		int imgNum = _sequences[frame];
		if (imgNum > _maxFrames)
			imgNum = 1;

		_imageFrame = &(*_images)[imgNum - 1];
	}
}

/**
 * Handles trying to pick up an object. If allowed, plays an y necessary animation for picking
 * up the item, and then adds it to the player's inventory
 */
int Object::pickUpObject(const char *const messages[]) {
	Inventory &inv = *_vm->_inventory;
	People &people = *_vm->_people;
	Scene &scene = *_vm->_scene;
	Screen &screen = *_vm->_screen;
	Talk &talk = *_vm->_talk;
	UserInterface &ui = *_vm->_ui;
	int pickup = _pickup & 0x7f;
	bool printed = false;
	bool takeFlag = true;
	int numObjects = 0;

	if (pickup == 99) {
		for (int idx = 0; idx < 4 && !talk._talkToAbort; ++idx) {
			if (checkNameForCodes(_use[0]._names[idx], nullptr)) {
				if (!talk._talkToAbort)
					printed = true;
			}
		}

		return 0;
	}

	if (!pickup || (pickup > 50 && pickup <= 80)) {
		int message = _pickup;
		if (message > 50)
			message -= 50;

		++ui._infoFlag;
		ui.clearInfo();
		screen.print(Common::Point(0, INFO_LINE + 1), INFO_LINE, messages[message]);
		ui._menuCounter = 30;
	} else {
		// Pick it up
		if ((_pickup & 0x80) == 0) {
			// Play an animation
			if (pickup > 80) {
				takeFlag = false;		// Don't pick it up
				scene.startCAnim(pickup - 81, 1);
				if (_pickupFlag)
					_vm->setFlags(_pickupFlag);
			} else {
				scene.startCAnim(pickup - 1, 1);
				if (!talk._talkToAbort) {
					// Erase the shape
					_type = _type == NO_SHAPE ? INVALID : REMOVE;
				}
			}

			if (talk._talkToAbort)
				return 0;
		} else {
			// Play generic pickup sequence
			// Original moved cursor position here
			people.goAllTheWay();
			ui._menuCounter = 25;
			ui._temp1 = 1;
		}

		for (int idx = 0; idx < 4 && !talk._talkToAbort; ++idx) {
			if (checkNameForCodes(_use[0]._names[idx], nullptr)) {
				if (!talk._talkToAbort)
					printed = true;
			}
		}
		if (talk._talkToAbort)
			return 0;

		// Add the item to the player's inventory
		if (takeFlag)
			numObjects = inv.putItemInInventory(*this);

		if (!printed) {
			ui._infoFlag++;
			ui.clearInfo();
			
			Common::String itemName = _description;
			itemName.setChar(tolower(itemName[0]), 0);
			screen.print(Common::Point(0, INFO_LINE + 1), INFO_FOREGROUND, "Picked up %s", itemName.c_str());
			ui._menuCounter = 25;
		}
	}

	return numObjects;
}

/**
 * Returns the current bounds for the sprite
 */
const Common::Rect Object::getNewBounds() const {
	Common::Point pt = _position;
	if (_imageFrame)
		pt += _imageFrame->_offset;

	return Common::Rect(pt.x, pt.y, pt.x + frameWidth(), pt.y + frameHeight());
}

/**
 * Returns the bounds for a sprite without a shape
 */
const Common::Rect Object::getNoShapeBounds() const {
	return Common::Rect(_position.x, _position.y,
		_position.x + _noShapeSize.x, _position.y + _noShapeSize.y);
}

/**
 * Returns the old bounsd for the sprite from the previous frame
 */
const Common::Rect Object::getOldBounds() const {
	return Common::Rect(_oldPosition.x, _oldPosition.y,
		_oldPosition.x + _oldSize.x, _oldPosition.y + _oldSize.y);
}

/*----------------------------------------------------------------*/

void CAnim::synchronize(Common::SeekableReadStream &s) {
	char buffer[12];
	s.read(buffer, 12);
	_name = Common::String(buffer);

	s.read(_sequences, 30);
	_position.x = s.readSint16LE();
	_position.y = s.readSint16LE();
	_size = s.readUint32LE();
	_type = (SpriteType)s.readUint16LE();
	_flags = s.readByte();
	_goto.x = s.readSint16LE();
	_goto.y = s.readSint16LE();
	_gotoDir = s.readSint16LE();
	_teleportPos.x = s.readSint16LE();
	_teleportPos.y = s.readSint16LE();
	_teleportDir = s.readSint16LE();
}

/*----------------------------------------------------------------*/

SceneImage::SceneImage() {
	_images = nullptr;
	_maxFrames = 0;
	_filesize = 0;
}

} // End of namespace Sherlock