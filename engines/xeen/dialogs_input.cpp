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

#include "xeen/dialogs_input.h"
#include "xeen/scripts.h"
#include "xeen/xeen.h"

namespace Xeen {

int Input::show(XeenEngine *vm, Window *window, Common::String &line, 
		uint maxLen, int maxWidth, bool isNumeric) {
	Input *dlg = new Input(vm, window);
	int result = dlg->getString(line, maxLen, maxWidth, isNumeric);
	delete dlg;

	return result;
}

/**
 * Allows the user to enter a string
 */
int Input::getString(Common::String &line, uint maxLen, int maxWidth, bool isNumeric) {
	_vm->_noDirectionSense = true;
	Common::String msg = Common::String::format("\x03""l\t000\x04%03d\x03""c", maxWidth);
	_window->writeString(msg);
	_window->update();

	while (!_vm->shouldQuit()) {
		Common::KeyCode keyCode = doCursor(msg);

		if ((keyCode == Common::KEYCODE_BACKSPACE || keyCode == Common::KEYCODE_DELETE)
			&& line.size() > 0)
			line.deleteLastChar();
		else if (line.size() < maxLen && (line.size() > 0 || keyCode != Common::KEYCODE_SPACE)
				&& ((isNumeric && keyCode >= Common::KEYCODE_0 && keyCode < Common::KEYCODE_9) ||
				   (!isNumeric && keyCode >= Common::KEYCODE_SPACE && keyCode < Common::KEYCODE_DELETE))) {
			line += (char)keyCode;
		} else if (keyCode == Common::KEYCODE_RETURN || keyCode == Common::KEYCODE_KP_ENTER) {
			break;
		} else if (keyCode == Common::KEYCODE_ESCAPE) {
			line = "";
			break;
		}
	}

	_vm->_noDirectionSense = false;
	return line.size();
}

/**
 * Draws the cursor and waits until the user presses a key
 */
Common::KeyCode Input::doCursor(const Common::String &msg) {
	EventsManager &events = *_vm->_events;
	Interface &intf = *_vm->_interface;
	Screen &screen = *_vm->_screen;

	bool oldUpDoorText = intf._upDoorText;
	byte oldTillMove = intf._tillMove;
	intf._upDoorText = false;
	intf._tillMove = 0;

	bool flag = !_vm->_startupWindowActive && !screen._windows[25]._enabled
		&& _vm->_mode != MODE_FF && _vm->_mode != MODE_17;

	Common::KeyCode ch = Common::KEYCODE_INVALID;
	while (!_vm->shouldQuit()) {
		events.updateGameCounter();

		if (flag)
			intf.draw3d(false);
		_window->writeString(msg);
		_window->update();

		if (flag)
			screen._windows[3].update();

		events.wait(1, true);
		if (events.isKeyPending()) {
			Common::KeyState keyState;
			events.getKey(keyState);
			ch = keyState.keycode;
			break;
		}
	}

	_window->writeString("");
	_window->update();

	intf._tillMove = oldTillMove;
	intf._upDoorText = oldUpDoorText;

	return ch;
}

/*------------------------------------------------------------------------*/

StringInput::StringInput(XeenEngine *vm): Input(vm, &vm->_screen->_windows[6]) {
}

int StringInput::show(XeenEngine *vm, bool type, const Common::String &msg1,
		const Common::String &msg2, int opcode) {
	StringInput *dlg = new StringInput(vm);
	int result = dlg->execute(type, msg1, msg2, opcode);
	delete dlg;

	return result;
}

int StringInput::execute(bool type, const Common::String &expected, 
		const Common::String &title, int opcode) {
	Interface &intf = *_vm->_interface;
	Screen &screen = *_vm->_screen;
	Scripts &scripts = *_vm->_scripts;
	Window &w = screen._windows[6];
	SoundManager &sound = *_vm->_sound;
	int result = 0;

	w.open();
	w.writeString(Common::String::format("\r\x03""c%s\v024\t000", title.c_str()));
	w.update();

	Common::String line;
	if (getString(line, 30, 200, false)) {
		if (type) {
			if (line == intf._interfaceText) {
				result = true;
			} else if (line == expected) {
				result = (opcode == 55) ? -1 : 1;
			}
		} else {
			// Load in the mirror list
			File f(Common::String::format("%smirr.txt",
				_vm->_files->_isDarkCc ? "dark" : "xeen"));
			MirrorEntry me;
			scripts._mirror.clear();
			while (me.synchronize(f))
				scripts._mirror.push_back(me);

			for (uint idx = 0; idx < scripts._mirror.size(); ++idx) {
				if (line == scripts._mirror[idx]._name) {
					result = idx;
					sound.playFX(_vm->_files->_isDarkCc ? 35 : 61);
					break;
				}
			}
		}
	}

	w.close();
	return result;
}

/*------------------------------------------------------------------------*/

NumericInput::NumericInput(XeenEngine *vm, int window) : Input(vm, &vm->_screen->_windows[window]) {
}

int NumericInput::show(XeenEngine *vm, int window, int maxLength, int maxWidth) {
	NumericInput *dlg = new NumericInput(vm, window);
	int result = dlg->execute(maxLength, maxWidth);
	delete dlg;

	return result;
}

int NumericInput::execute(int maxLength, int maxWidth) {
	Common::String line;

	if (getString(line, maxLength, maxWidth, true))
		return atoi(line.c_str());
	else
		return 0;
}


} // End of namespace Xeen