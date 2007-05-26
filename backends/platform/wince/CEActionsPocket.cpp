/* ScummVM - Scumm Interpreter
 * Copyright (C) 2001-2007 The ScummVM project
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
 * $URL$
 * $Id$
 *
 */


#include "common/stdafx.h"
#include "CEActionsPocket.h"
#include "EventsBuffer.h"
#include "gui/message.h"
#include "scumm/scumm.h"
#include "common/config-manager.h"
#include "gui/KeysDialog.h"

#ifdef _WIN32_WCE
#define		KEY_ALL_SKIP	3457
#endif

const String pocketActionNames[] = {
	"Pause",
	"Save",
	"Quit",
	"Skip",
	"Hide Toolbar",
	"Show Keyboard",
	"Sound on/off",
	"Right click",
	"Show/Hide Cursor",
	"Free look",
	"Zoom up",
	"Zoom down",
	"FT Cheat",
	"Bind Keys",
	"Cursor Up",
	"Cursor Down",
	"Cursor Left",
	"Cursor Right",
	"Left Click",
};

void CEActionsPocket::init() {
	_instance = new CEActionsPocket(ConfMan.get("gameid"));
}


String CEActionsPocket::actionName(GUI::ActionType action) {
	return pocketActionNames[action];
}

int CEActionsPocket::size() {
	return POCKET_ACTION_LAST;
}

String CEActionsPocket::domain() {
	return ConfMan.kApplicationDomain;
}

int CEActionsPocket::version() {
	return POCKET_ACTION_VERSION;
}

CEActionsPocket::CEActionsPocket(const Common::String &gameid) :
GUI::Actions()
{
	int i;

	_right_click_needed = false;
	_hide_toolbar_needed = false;
	_zoom_needed = false;

	for (i=0; i<POCKET_ACTION_LAST; i++) {
		_action_mapping[i] = 0;
		_action_enabled[i] = false;
	}

	// apply some default settings for emulated mouse
	_action_enabled[POCKET_ACTION_LEFTCLICK] = true;
	_action_enabled[POCKET_ACTION_UP] = true;
	_action_enabled[POCKET_ACTION_DOWN] = true;
	_action_enabled[POCKET_ACTION_LEFT] = true;
	_action_enabled[POCKET_ACTION_RIGHT] = true;
	_action_mapping[POCKET_ACTION_LEFTCLICK] = SDLK_RETURN;
	_action_mapping[POCKET_ACTION_UP] = SDLK_UP;
	_action_mapping[POCKET_ACTION_DOWN] = SDLK_DOWN;
	_action_mapping[POCKET_ACTION_LEFT] = SDLK_LEFT;
	_action_mapping[POCKET_ACTION_RIGHT] = SDLK_RIGHT;
}

void CEActionsPocket::initInstanceMain(OSystem *mainSystem) {
	// Nothing generic to do for Pocket PC
	_CESystem = static_cast<OSystem_WINCE3*>(mainSystem);
	GUI_Actions::initInstanceMain(mainSystem);
}

void CEActionsPocket::initInstanceGame() {
	String gameid(ConfMan.get("gameid"));
	bool is_simon = (strncmp(gameid.c_str(), "simon", 5) == 0);
	bool is_sword1 = (gameid == "sword1");
	bool is_sword2 = (strcmp(gameid.c_str(), "sword2") == 0);
	bool is_queen = (gameid == "queen");
	bool is_sky = (gameid == "sky");
	bool is_comi = (strncmp(gameid.c_str(), "comi", 4) == 0);
	bool is_gob = (strncmp(gameid.c_str(), "gob", 3) == 0);
	bool is_saga = (gameid == "saga");
	bool is_kyra = (gameid == "kyra1");
	bool is_samnmax = (gameid == "samnmax");
	bool is_cine = (gameid == "cine");
	bool is_touche = (gameid == "touche");
	bool is_agi = (gameid == "agi");

	GUI_Actions::initInstanceGame();

	// See if a right click mapping could be needed
	if (is_sword1 || is_sword2 || is_sky || is_queen || is_comi || is_gob || is_samnmax || is_cine || is_touche)
		_right_click_needed = true;

	// See if a "hide toolbar" mapping could be needed
	if (is_sword1 || is_sword2 || is_comi)
		_hide_toolbar_needed = true;

	// Initialize keys for different actions
	// Pause
	_key_action[POCKET_ACTION_PAUSE].setAscii(VK_SPACE);
	_action_enabled[POCKET_ACTION_PAUSE] = true;
	// Save
	if (is_simon || is_sword2 || is_gob || is_kyra || is_touche)
		_action_enabled[POCKET_ACTION_SAVE] = false;
	else if (is_queen) {
		_action_enabled[POCKET_ACTION_SAVE] = true;
		_key_action[POCKET_ACTION_SAVE].setAscii(286); // F1 key for FOTAQ
	} else if (is_sky) {
		_action_enabled[POCKET_ACTION_SAVE] = true;
		_key_action[POCKET_ACTION_SAVE].setAscii(63);
	} else if (is_cine) {
		_action_enabled[POCKET_ACTION_SAVE] = true;
		_key_action[POCKET_ACTION_SAVE].setAscii(291); // F10
	} else if (is_agi) {
		_action_enabled[POCKET_ACTION_SAVE] = true;
		_key_action[POCKET_ACTION_SAVE].setAscii(SDLK_ESCAPE);
	} else {
		_action_enabled[POCKET_ACTION_SAVE] = true;
		_key_action[POCKET_ACTION_SAVE].setAscii(319); // F5 key
	}
	// Quit
	_action_enabled[POCKET_ACTION_QUIT] = true;
	// Skip
	if (!is_cine)
		_action_enabled[POCKET_ACTION_SKIP] = true;
	if (is_simon || is_sky || is_sword2 || is_queen || is_sword1 || is_gob || is_saga || is_kyra || is_touche)
		_key_action[POCKET_ACTION_SKIP].setAscii(VK_ESCAPE);
	else
		_key_action[POCKET_ACTION_SKIP].setAscii(KEY_ALL_SKIP);
	// Hide
	_action_enabled[POCKET_ACTION_HIDE] = true;
	// Keyboard
	_action_enabled[POCKET_ACTION_KEYBOARD] = true;
	// Sound
	_action_enabled[POCKET_ACTION_SOUND] = true;
	// RightClick
	_action_enabled[POCKET_ACTION_RIGHTCLICK] = true;
	// Cursor
	_action_enabled[POCKET_ACTION_CURSOR] = true;
	// Freelook
	_action_enabled[POCKET_ACTION_FREELOOK] = true;
	// Zoom
	if (is_sword1 || is_sword2 || is_comi || is_touche) {
		_zoom_needed = true;
		_action_enabled[POCKET_ACTION_ZOOM_UP] = true;
		_action_enabled[POCKET_ACTION_ZOOM_DOWN] = true;
	}
	// FT Cheat
	_action_enabled[POCKET_ACTION_FT_CHEAT] = true;
	_key_action[POCKET_ACTION_FT_CHEAT].setAscii(86); // shift-V
	// Key bind method
	_action_enabled[POCKET_ACTION_BINDKEYS] = true;
}


CEActionsPocket::~CEActionsPocket() {
}

bool CEActionsPocket::perform(GUI::ActionType action, bool pushed) {
	static bool keydialogrunning = false;

	if (!pushed) {
		switch(action) {
		case POCKET_ACTION_RIGHTCLICK:
			_CESystem->add_right_click(false);
			return true;
		case POCKET_ACTION_LEFTCLICK:
			_CESystem->add_left_click(false);
			return true;
		case POCKET_ACTION_PAUSE:
		case POCKET_ACTION_SAVE:
		case POCKET_ACTION_SKIP:
		case POCKET_ACTION_FT_CHEAT:
			EventsBuffer::simulateKey(&_key_action[action], false);
			return true;

		}
		return false;
	}

	switch (action) {
		case POCKET_ACTION_PAUSE:
		case POCKET_ACTION_SAVE:
		case POCKET_ACTION_SKIP:
		case POCKET_ACTION_FT_CHEAT:
			EventsBuffer::simulateKey(&_key_action[action], true);
			return true;
		case POCKET_ACTION_KEYBOARD:
			_CESystem->swap_panel();
			return true;
		case POCKET_ACTION_HIDE:
			_CESystem->swap_panel_visibility();
			return true;
		case POCKET_ACTION_SOUND:
			_CESystem->swap_sound_master();
			return true;
		case POCKET_ACTION_RIGHTCLICK:
			_CESystem->add_right_click(true);
			return true;
		case POCKET_ACTION_CURSOR:
			_CESystem->swap_mouse_visibility();
			return true;
		case POCKET_ACTION_FREELOOK:
			_CESystem->swap_freeLook();
			return true;
		case POCKET_ACTION_ZOOM_UP:
			_CESystem->swap_zoom_up();
			return true;
		case POCKET_ACTION_ZOOM_DOWN:
			_CESystem->swap_zoom_down();
			return true;
		case POCKET_ACTION_LEFTCLICK:
			_CESystem->add_left_click(true);
			return true;
		case POCKET_ACTION_UP:
			_CESystem->move_cursor_up();
			return true;
		case POCKET_ACTION_DOWN:
			_CESystem->move_cursor_down();
			return true;
		case POCKET_ACTION_LEFT:
			_CESystem->move_cursor_left();
			return true;
		case POCKET_ACTION_RIGHT:
			_CESystem->move_cursor_right();
			return true; 
		case POCKET_ACTION_QUIT:
			{
				GUI::MessageDialog alert("   Are you sure you want to quit ?   ", "Yes", "No");
				if (alert.runModal() == GUI::kMessageOK)
					_mainSystem->quit();
				return true;
			}
		case POCKET_ACTION_BINDKEYS:
			if (!keydialogrunning) {
				keydialogrunning = true;
				GUI::KeysDialog *keysDialog = new GUI::KeysDialog();
				keysDialog->runModal();
				delete keysDialog;
				keydialogrunning = false;
			}
			return true;
	}
	return false;
}

bool CEActionsPocket::needsRightClickMapping() {
	if (!_right_click_needed)
		return false;
	else
		return (_action_mapping[POCKET_ACTION_RIGHTCLICK] == 0);
}

bool CEActionsPocket::needsHideToolbarMapping() {
	if (!_hide_toolbar_needed)
		return false;
	else
		return (_action_mapping[POCKET_ACTION_HIDE] == 0);
}


bool CEActionsPocket::needsZoomMapping() {
	if (!_zoom_needed)
		return false;
	else
		return (_action_mapping[POCKET_ACTION_ZOOM_UP] == 0 || _action_mapping[POCKET_ACTION_ZOOM_DOWN] == 0);
}
