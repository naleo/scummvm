/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "common/file.h"
#include "common/memstream.h"

#include "freescape/freescape.h"
#include "freescape/games/dark/dark.h"
#include "freescape/language/8bitDetokeniser.h"

namespace Freescape {

void DarkEngine::initCPC() {
	_viewArea = Common::Rect(40, 24, 279, 124);

}

extern byte kCPCPaletteTitleData[4][3];
extern byte kCPCPaletteBorderData[4][3];

extern Graphics::ManagedSurface *readCPCImage(Common::SeekableReadStream *file, bool mode0);

void DarkEngine::loadAssetsCPCFullGame() {
	Common::File file;

	file.open("DARK1.SCR");
	if (!file.isOpen())
		error("Failed to open DARK1.SCR");

	_title = readCPCImage(&file, false);
	_title->setPalette((byte*)&kCPCPaletteTitleData, 0, 4);

	file.close();
	file.open("DARK2.SCR");
	if (!file.isOpen())
		error("Failed to open DARK2.SCR");

	_border = readCPCImage(&file, true);
	_border->setPalette((byte*)&kCPCPaletteBorderData, 0, 4);

	file.close();
	file.open("DARKCODE.BIN");

	if (!file.isOpen())
		error("Failed to open DARKCODE.BIN");

	loadMessagesFixedSize(&file, 0x5d9, 14, 20);
	loadFonts(&file, 0x60f3);
	loadGlobalObjects(&file, 0x9a, 8);
	load8bitBinary(&file, 0x6255, 16);
}

void DarkEngine::drawCPCUI(Graphics::Surface *surface) {
	uint32 color = 7;
	uint8 r, g, b;

	_gfx->readFromPalette(color, r, g, b);
	uint32 front = _gfx->_texturePixelFormat.ARGBToColor(0xFF, r, g, b);

	color = _currentArea->_usualBackgroundColor;
	if (_gfx->_colorRemaps && _gfx->_colorRemaps->contains(color)) {
		color = (*_gfx->_colorRemaps)[color];
	}

	_gfx->readFromPalette(color, r, g, b);
	uint32 back = _gfx->_texturePixelFormat.ARGBToColor(0xFF, r, g, b);

	int score = _gameStateVars[k8bitVariableScore];
	int ecds = _gameStateVars[kVariableActiveECDs];
	drawStringInSurface(Common::String::format("%04d", int(2 * _position.x())), 191, 141, front, back, surface);
	drawStringInSurface(Common::String::format("%04d", int(2 * _position.z())), 191, 149, front, back, surface);
	drawStringInSurface(Common::String::format("%04d", int(2 * _position.y())), 191, 157, front, back, surface);

	drawStringInSurface(Common::String::format("%02d", int(_angleRotations[_angleRotationIndex])), 78, 165, front, back, surface);
	drawStringInSurface(Common::String::format("%3d", _playerSteps[_playerStepIndex]), 78, 173, front, back, surface);
	drawStringInSurface(Common::String::format("%07d", score), 94, 13, front, back, surface);
	drawStringInSurface(Common::String::format("%3d%%", ecds), 190, 13, front, back, surface);

	int seconds, minutes, hours;
	getTimeFromCountdown(seconds, minutes, hours);

	Common::String message;
	int deadline;
	getLatestMessages(message, deadline);
	if (deadline <= _countdown) {
		drawStringInSurface(message, 112, 173, back, front, surface);
		_temporaryMessages.push_back(message);
		_temporaryMessageDeadlines.push_back(deadline);
	} else
		drawStringInSurface(_currentArea->_name, 112, 173, front, back, surface);

	int energy = _gameStateVars[k8bitVariableEnergy]; // called fuel in this game
	int shield = _gameStateVars[k8bitVariableShield];

	if (shield >= 0) {
		Common::Rect shieldBar;
		shieldBar = Common::Rect(80, 140, 143 - (_maxShield - shield), 148);
		surface->fillRect(shieldBar, back);

		shieldBar = Common::Rect(80, 141, 143 - (_maxShield - shield), 147);
		surface->fillRect(shieldBar, front);
	}

	if (energy >= 0) {
		Common::Rect energyBar;
		energyBar = Common::Rect(80, 147, 143 - (_maxEnergy - energy), 155);
		surface->fillRect(energyBar, back);

		energyBar = Common::Rect(80, 148, 143 - (_maxEnergy - energy), 154);
		surface->fillRect(energyBar, front);
	}
	uint32 clockColor = _gfx->_texturePixelFormat.ARGBToColor(0xFF, 0xFF, 0x00, 0x00);
	drawBinaryClock(surface, 273, 128, clockColor, back);
	//drawIndicator(surface, 152, 140);
}

} // End of namespace Freescape