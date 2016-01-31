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

#include "agi/agi.h"

namespace Agi {

bool AgiBase::getFlag(int16 flagNr) {
	uint8 *flagPtr = _game.flags;

	flagPtr += flagNr >> 3;
	return (*flagPtr & (1 << (flagNr & 0x07))) != 0;
}

void AgiBase::setFlag(int16 flagNr, bool newState) {
	uint8 *flagPtr = _game.flags;

	flagPtr += flagNr >> 3;
	if (newState)
		*flagPtr |= 1 << (flagNr & 0x07);	// set bit
	else
		*flagPtr &= ~(1 << (flagNr & 0x07));	// clear bit
}

void AgiBase::flipFlag(int16 flagNr) {
	uint8 *flagPtr = _game.flags;

	flagPtr += flagNr >> 3;
	*flagPtr ^= 1 << (flagNr & 0x07);	// flip bit
}

void AgiEngine::setVar(int16 varNr, byte newValue) {
	_game.vars[varNr] = newValue;

	if (varNr == VM_VAR_VOLUME) {
		_mixer->setVolumeForSoundType(Audio::Mixer::kSFXSoundType, newValue * 17);
		_mixer->setVolumeForSoundType(Audio::Mixer::kMusicSoundType, newValue * 17);
	}
}

byte AgiEngine::getVar(int16 varNr) {
	return _game.vars[varNr];
}

void AgiEngine::decrypt(uint8 *mem, int len) {
	const uint8 *key;
	int i;

	key = (getFeatures() & GF_AGDS) ? (const uint8 *)CRYPT_KEY_AGDS
	                   : (const uint8 *)CRYPT_KEY_SIERRA;

	for (i = 0; i < len; i++)
		*(mem + i) ^= *(key + (i % 11));
}

} // End of namespace Agi
