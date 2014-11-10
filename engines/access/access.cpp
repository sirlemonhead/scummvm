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

#include "common/scummsys.h"
#include "common/config-manager.h"
#include "common/debug-channels.h"
#include "common/events.h"
#include "engines/util.h"
#include "graphics/scaler.h"
#include "graphics/thumbnail.h"
#include "access/access.h"

namespace Access {

AccessEngine::AccessEngine(OSystem *syst, const AccessGameDescription *gameDesc) :
		_gameDescription(gameDesc), Engine(syst), _randomSource("Access"),
		_useItem(_flags[99]), _startup(_flags[170]), _manScaleOff(_flags[172]) {
	_animation = nullptr;
	_bubbleBox = nullptr;
	_char = nullptr;
	_debugger = nullptr;
	_events = nullptr;
	_files = nullptr;
	_inventory = nullptr;
	_player = nullptr;
	_room = nullptr;
	_screen = nullptr;
	_scripts = nullptr;
	_sound = nullptr;
	_video = nullptr;

	_destIn = nullptr;
	_current = nullptr;
	_pCount = 0;
	_mouseMode = 0;
	_currentMan = 0;
	_currentManOld = -1;
	_inactive._spritesPtr = nullptr;
	_inactive._flags = _inactive._frameNumber = _inactive._offsetY = 0;
	_inactive._position = Common::Point(0, 0);
	_music = nullptr;
	_title = nullptr;
	_converseMode = 0;
	_startAboutBox = 0;
	_startTravelBox = 0;
	_numAnimTimers = 0;
	_startup = 0;
	_currentCharFlag = false;
	_boxSelect = false;
	_scale = 0;
	_scaleH1 = _scaleH2 = 0;
	_scaleN1 = 0;
	_scaleT1 = 0;
	_scaleMaxY = 0;
	_scaleI = 0;
	_scaleFlag = false;
	_canSaveLoad = false;
	_eseg = nullptr;

	_conversation = 0;
	_currentMan = 0;
	_newTime = 0;
	_newDate = 0;
	Common::fill(&_objectsTable[0], &_objectsTable[100], (SpriteResource *)nullptr);
	Common::fill(&_establishTable[0], &_establishTable[100], false);
	Common::fill(&_flags[0], &_flags[256], 0);
	_establishFlag = false;
	_establishMode = 0;
	_establishGroup = 0;
	_establishCtrlTblOfs = 0;
	Common::fill(&_help1[0], &_help1[366], 0);
	Common::fill(&_help2[0], &_help2[366], 0);
	Common::fill(&_help1[0], &_help3[366], 0);
	_helpTbl[0] = _help1;
	_helpTbl[1] = _help2;
	_helpTbl[2] = _help3;
	_travel = 0;
	_ask = 0;
	_rScrollRow = 0;
	_rScrollCol = 0;
	_rScrollX = 0;
	_rScrollY = 0;
	_rOldRectCount = 0;
	_rNewRectCount = 0;
	_rKeyFlag = 0;
	_mapOffset = 0;
	_screenVirtX = 0;
	_lastTime = g_system->getMillis();
	_curTime = 0;
	_narateFile = 0;
	_txtPages = 0;
	_sndSubFile = 0;
	_loadSaveSlot = -1;
}

AccessEngine::~AccessEngine() {
	delete _animation;
	delete _bubbleBox;
	delete _char;
	delete _debugger;
	delete _events;
	delete _files;
	delete _inventory;
	delete _player;
	delete _room;
	delete _screen;
	delete _scripts;
	delete _sound;
	delete _video;

	freeCells();
	delete _inactive._spritesPtr;
	delete _music;
	delete _title;
	delete _eseg;
}

void AccessEngine::setVGA() {
	initGraphics(320, 200, false);
}

void AccessEngine::initialize() {
	// Set up debug channels
	DebugMan.addDebugChannel(kDebugPath, "Path", "Pathfinding debug level");
	DebugMan.addDebugChannel(kDebugScripts, "scripts", "Game scripts");
	DebugMan.addDebugChannel(kDebugGraphics, "graphics", "Graphics handling");

	if (isCD()) {
		const Common::FSNode gameDataDir(ConfMan.get("path"));
		// The CD version contains two versions of the game.
		// - The MCGA version, in the CDROM folder
		// - The VESA version, in the TDROM folder
		// We use the hires version.
		const Common::FSNode cdromDir = gameDataDir.getChild("tdrom");

		for (int idx = 0; idx < 15; ++idx) {
			Common::String folder = (idx == 0) ? "game" :
				Common::String::format("chap%.2d", idx);
			SearchMan.addSubDirectoryMatching(cdromDir, folder);
		}
	}

	// Create sub-objects of the engine
	ASurface::init();
	_animation = new AnimationManager(this);
	_bubbleBox = new BubbleBox(this);
	_char = new CharManager(this);
	_debugger = Debugger::init(this);
	_events = new EventsManager(this);
	_files = new FileManager(this);
	_inventory = new InventoryManager(this);
	_player = new Player(this);
	_screen = new Screen(this);
	_sound = new SoundManager(this, _mixer);
	_video = new VideoPlayer(this);

	_buffer1.create(g_system->getWidth() + TILE_WIDTH, g_system->getHeight());
	_buffer2.create(g_system->getWidth(), g_system->getHeight());

	// If requested, load a savegame instead of showing the intro
	if (ConfMan.hasKey("save_slot")) {
		int saveSlot = ConfMan.getInt("save_slot");
		if (saveSlot >= 0 && saveSlot <= 999)
			_loadSaveSlot = saveSlot;
	}
}

Common::Error AccessEngine::run() {
	setVGA();
	initialize();

	playGame();

	return Common::kNoError;
}

int AccessEngine::getRandomNumber(int maxNumber) {
	return _randomSource.getRandomNumber(maxNumber);
}

void AccessEngine::loadCells(Common::Array<CellIdent> &cells) {
	for (uint i = 0; i < cells.size(); ++i) {
		Resource *spriteData = _files->loadFile(cells[i]);
		_objectsTable[cells[i]._cell] = new SpriteResource(this, spriteData);
		delete spriteData;
	}
}

void AccessEngine::freeCells() {
	for (int i = 0; i < 100; ++i) {
		delete _objectsTable[i];
		_objectsTable[i] = nullptr;
	}
}

void AccessEngine::freeInactiveData() {
	delete _inactive._spritesPtr;
	_inactive._spritesPtr = nullptr;
}

void AccessEngine::speakText(ASurface *s, const Common::String &msg) {
	Common::String lines = msg;
	Common::String line;
	int curPage = 0;
	int soundsLeft = 0;

	for (;;) {
		soundsLeft = _countTbl[curPage];
		_events->zeroKeys();

		Common::String line;
		int width = 0;
		bool lastLine = _fonts._font2.getLine(lines, s->_maxChars * 6, line, width);

		// Set font colors
		_fonts._font2._fontColors[0] = 0;
		_fonts._font2._fontColors[1] = 28;
		_fonts._font2._fontColors[2] = 29;
		_fonts._font2._fontColors[3] = 30;

		_fonts._font2.drawString(s, line, s->_printOrg);
		s->_printOrg = Common::Point(s->_printStart.x, s->_printOrg.y + 9);

		if ((s->_printOrg.y > _printEnd) && (!lastLine)) {
			if (!_sound->_isVoice) {
				_events->waitKeyMouse();
			} else {
				for (;;) {
					_sound->freeSounds();
					Resource *sound = _sound->loadSound(_narateFile + 99, _sndSubFile);
					_sound->_soundTable.push_back(SoundEntry(sound, 1));
					_sound->playSound(0);
					_scripts->cmdFreeSound();

					_events->pollEvents();

					if (_events->_leftButton) {
						_events->debounceLeft();
						_sndSubFile += soundsLeft;
						break;
					} else if (_events->_keypresses.size() != 0) {
						_sndSubFile += soundsLeft;
						break;
					} else {
						++_sndSubFile;
						--soundsLeft;
						if (soundsLeft == 0)
							break;
					}
				}
			}

			s->copyBuffer(&_buffer2);
			s->_printOrg.y = s->_printStart.y;
			++curPage;
			soundsLeft = _countTbl[curPage];
		}

		if (lastLine)
			break;
	}

	if (soundsLeft == 0)
		return;

	if (!_sound->_isVoice) {
		_events->waitKeyMouse();
		return;
	}

	for (;;) {
		_sound->freeSounds();
		Resource *res = _sound->loadSound(_narateFile + 99, _sndSubFile);
		_sound->_soundTable.push_back(SoundEntry(res, 1));
		_sound->playSound(0);
		_scripts->cmdFreeSound();

		_events->pollEvents();

		if (_events->_leftButton) {
			_events->debounceLeft();
			_sndSubFile += soundsLeft;
			break;
		} else if (_events->_keypresses.size() != 0) {
			_sndSubFile += soundsLeft;
			break;
		} else {
			++_sndSubFile;
			--soundsLeft;
			if (soundsLeft == 0)
				break;
		}
	}
}

void AccessEngine::printText(ASurface *s, const Common::String &msg) {
	Common::String lines = msg;
	Common::String line;
	int width = 0;

	for (;;) {
		bool lastLine = _fonts._font2.getLine(lines, s->_maxChars * 6, line, width);

		// Set font colors
		_fonts._font2._fontColors[0] = 0;
		_fonts._font2._fontColors[1] = 28;
		_fonts._font2._fontColors[2] = 29;
		_fonts._font2._fontColors[3] = 30;
		_fonts._font2.drawString(s, line, s->_printOrg);

		s->_printOrg = Common::Point(s->_printStart.x, s->_printOrg.y + 9);

		if (s->_printOrg.y >_printEnd && !lastLine) {
			_events->waitKeyMouse();
			s->copyBuffer(&_buffer2);
			s->_printOrg.y = s->_printStart.y;
		}

		if (lastLine)
			break;
	}
	_events->waitKeyMouse();
}


void AccessEngine::plotList() {
	_player->calcPlayer();
	plotList1();
}

void AccessEngine::plotList1() {
	for (uint idx = 0; idx < _images.size(); ++idx) {
		ImageEntry &ie = _images[idx];

		_scaleFlag = (ie._flags & 8) != 0;
		Common::Point pt = ie._position - _screen->_bufferStart;
		SpriteResource *sprites = ie._spritesPtr;
		SpriteFrame *frame = sprites->getFrame(ie._frameNumber);

		Common::Rect bounds(pt.x, pt.y, pt.x + frame->w, pt.y + frame->h);
		if (!_scaleFlag) {
			bounds.setWidth(_screen->_scaleTable1[frame->w]);
			bounds.setHeight(_screen->_scaleTable1[frame->h]);
		}

		// Make a copy - some of the drawing methods I've adapted need the full
		// scaled dimensions on-screen, and handle clipping themselves
		Common::Rect destBounds = bounds;

		if (_buffer2.clip(bounds)) {
			ie._flags |= 1;
		} else {
			ie._flags &= ~1;
			if (_buffer2._leftSkip != 0 ||  _buffer2._rightSkip != 0
				|| _buffer2._topSkip != 0 || _buffer2._bottomSkip != 0)
				ie._flags |= 1;

			_newRects.push_back(bounds);

			if (!_scaleFlag) {
				_buffer2._rightSkip /= _scale;
				bounds.setWidth(bounds.width() / _scale);

				if (ie._flags & 2) {
					_buffer2.sPlotB(frame, destBounds);
				} else {
					_buffer2.sPlotF(frame, destBounds);
				}
			} else {
				if (ie._flags & 2) {
					_buffer2.plotB(frame, Common::Point(destBounds.left, destBounds.top));
				} else {
					_buffer2.plotF(frame, Common::Point(destBounds.left, destBounds.top));
				}
			}
		}
	}
}

void AccessEngine::copyBlocks() {
	// Copy the block list from the previous frame
	for (uint i = 0; i < _oldRects.size(); ++i) {
		_screen->copyBlock(&_buffer2, _oldRects[i]);
	}

	copyRects();
}

void AccessEngine::copyRects() {
	_oldRects.clear();
	for (uint i = 0; i < _newRects.size(); ++i) {
		_screen->copyBlock(&_buffer2, _newRects[i]);
		_oldRects.push_back(_newRects[i]);
	}
}

void AccessEngine::copyBF1BF2() {
	_buffer2.copyRectToSurface(_buffer1, 0, 0,
		Common::Rect(_buffer1._scrollX, _buffer1._scrollY,
		_buffer1._scrollX + _screen->_vWindowBytesWide,
		_buffer1._scrollY + _screen->_vWindowLinesTall));
}

void AccessEngine::copyBF2Vid() {
	const byte *srcP = (const byte *)_buffer2.getPixels();
	byte *destP = (byte *)_screen->getBasePtr(_screen->_windowXAdd, 
		_screen->_windowYAdd + _screen->_screenYOff);

	for (int yp = 0; yp < _screen->_vWindowLinesTall; ++yp) {
		Common::copy(srcP, srcP + _screen->_vWindowBytesWide, destP);
		srcP += _buffer2.pitch;
		destP += _screen->pitch;
	}
}

void AccessEngine::playVideo(int videoNum, const Common::Point &pt) {
	_video->setVideo(_screen, pt, FileIdent(96, videoNum), 10);

	while (!shouldQuit() && !_video->_videoEnd) {
		_video->playVideo();

		g_system->delayMillis(10);
		_events->pollEvents();
	}
}

void AccessEngine::doLoadSave() {
	error("TODO: doLoadSave");
}

void AccessEngine::freeChar() {
	_scripts->freeScriptData();
	_animation->clearTimers();
	_animation->freeAnimationData();
}

Common::Error AccessEngine::saveGameState(int slot, const Common::String &desc) {
	Common::OutSaveFile *out = g_system->getSavefileManager()->openForSaving(
		generateSaveName(slot));
	if (!out)
		return Common::kCreatingFileFailed;

	AccessSavegameHeader header;
	header._saveName = desc;
	writeSavegameHeader(out, header);

	Common::Serializer s(nullptr, out);
	synchronize(s);

	out->finalize();
	delete out;

	return Common::kNoError;
}

Common::Error AccessEngine::loadGameState(int slot) {
	Common::InSaveFile *saveFile = g_system->getSavefileManager()->openForLoading(
		generateSaveName(slot));
	if (!saveFile)
		return Common::kReadingFailed;

	Common::Serializer s(saveFile, nullptr);

	// Load the savaegame header
	AccessSavegameHeader header;
	if (!readSavegameHeader(saveFile, header))
		error("Invalid savegame");

	if (header._thumbnail) {
		header._thumbnail->free();
		delete header._thumbnail;
	}

	// Load most of the savegame data
	synchronize(s);
	delete saveFile;

	// Set extra post-load state
	_room->_function = 1;
	_timers._timersSavedFlag = false;

	return Common::kNoError;
}

Common::String AccessEngine::generateSaveName(int slot) {
	return Common::String::format("%s.%03d", _targetName.c_str(), slot);
}

bool AccessEngine::canLoadGameStateCurrently() {
	return _canSaveLoad;
}

bool AccessEngine::canSaveGameStateCurrently() {
	return _canSaveLoad;
}

void AccessEngine::synchronize(Common::Serializer &s) {
	s.syncAsUint16LE(_conversation);
	s.syncAsUint16LE(_currentMan);
	s.syncAsUint32LE(_newTime);
	s.syncAsUint32LE(_newDate);

	for (int i = 0; i < 256; ++i)
		s.syncAsUint16LE(_flags[i]);
	for (int i = 0; i < 366; ++i) {
		s.syncAsByte(_help1[i]);
		s.syncAsByte(_help2[i]);
		s.syncAsByte(_help3[i]);
	}

	s.syncAsUint16LE(_travel);
	s.syncAsUint16LE(_ask);
	s.syncAsUint16LE(_rScrollRow);
	s.syncAsUint16LE(_rScrollCol);
	s.syncAsSint16LE(_rScrollX);
	s.syncAsSint16LE(_rScrollY);
	s.syncAsUint16LE(_rOldRectCount);
	s.syncAsUint16LE(_rNewRectCount);
	s.syncAsUint16LE(_rKeyFlag);
	s.syncAsUint16LE(_mapOffset);
	s.syncAsUint16LE(_screenVirtX);

	for (int i = 0; i < 100; ++i)
		s.syncAsByte(_establishTable[i]);

	// Synchronize sub-objects
	_timers.synchronize(s);
	_inventory->synchronize(s);
	_player->synchronize(s);
}

const char *const SAVEGAME_STR = "ACCESS";
#define SAVEGAME_STR_SIZE 6

bool AccessEngine::readSavegameHeader(Common::InSaveFile *in, AccessSavegameHeader &header) {
	char saveIdentBuffer[SAVEGAME_STR_SIZE + 1];
	header._thumbnail = nullptr;

	// Validate the header Id
	in->read(saveIdentBuffer, SAVEGAME_STR_SIZE + 1);
	if (strncmp(saveIdentBuffer, SAVEGAME_STR, SAVEGAME_STR_SIZE))
		return false;

	header._version = in->readByte();
	if (header._version > ACCESS_SAVEGAME_VERSION)
		return false;

	// Read in the string
	header._saveName.clear();
	char ch;
	while ((ch = (char)in->readByte()) != '\0') header._saveName += ch;

	// Get the thumbnail
	header._thumbnail = Graphics::loadThumbnail(*in);
	if (!header._thumbnail)
		return false;

	// Read in save date/time
	header._year = in->readSint16LE();
	header._month = in->readSint16LE();
	header._day = in->readSint16LE();
	header._hour = in->readSint16LE();
	header._minute = in->readSint16LE();
	header._totalFrames = in->readUint32LE();

	return true;
}

void AccessEngine::writeSavegameHeader(Common::OutSaveFile *out, AccessSavegameHeader &header) {
	// Write out a savegame header
	out->write(SAVEGAME_STR, SAVEGAME_STR_SIZE + 1);

	out->writeByte(ACCESS_SAVEGAME_VERSION);

	// Write savegame name
	out->write(header._saveName.c_str(), header._saveName.size());
	out->writeByte('\0');

	// Write a thumbnail of the screen
	uint8 thumbPalette[PALETTE_SIZE];
	_screen->getPalette(thumbPalette);
	Graphics::Surface saveThumb;
	::createThumbnail(&saveThumb, (const byte *)_screen->getPixels(),
		_screen->w, _screen->h, thumbPalette);
	Graphics::saveThumbnail(*out, saveThumb);
	saveThumb.free();

	// Write out the save date/time
	TimeDate td;
	g_system->getTimeAndDate(td);
	out->writeSint16LE(td.tm_year + 1900);
	out->writeSint16LE(td.tm_mon + 1);
	out->writeSint16LE(td.tm_mday);
	out->writeSint16LE(td.tm_hour);
	out->writeSint16LE(td.tm_min);
	out->writeUint32LE(_events->getFrameCounter());
}

} // End of namespace Access