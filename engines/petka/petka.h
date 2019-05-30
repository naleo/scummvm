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

#ifndef PETKA_PETKA_H
#define PETKA_PETKA_H

#include "engines/engine.h"

#include "gui/debugger.h"

struct ADGameDescription;

namespace Petka {

class Console;
class FileMgr;
class QManager;

enum {
	kPetkaDebugGeneral = 1 << 0,
};

class PetkaEngine : public Engine {
public:
	PetkaEngine(OSystem *syst, const ADGameDescription *desc);
	~PetkaEngine();

	virtual Common::Error run();
	Common::SeekableReadStream *openFile(const Common::String &name, bool addCurrentPath);

private:
	void loadStores();

private:
	Console *_console;
	Common::ScopedPtr<FileMgr> _fileMgr;
	Common::ScopedPtr<QManager> _resMgr;
	const ADGameDescription *_desc;

	Common::String _currentPath;
	Common::String _speechPath;

	Common::String _chapterStoreName;

	uint8 _part;
	uint8 _chapter;
};

class Console : public GUI::Debugger {
public:
	Console(PetkaEngine *vm) {}
	virtual ~Console() {}
};

} // End of namespace Petka

#endif
