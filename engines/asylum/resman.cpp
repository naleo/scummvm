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
 * $URL$
 * $Id$
 *
 */

#include "asylum/resman.h"

namespace Asylum {

ResourceManager::ResourceManager(AsylumEngine *vm): _vm(vm) {
	_video = new Video(_vm->_mixer);
}

ResourceManager::~ResourceManager() {
	delete _video;
}

bool ResourceManager::loadVideo(uint8 fileNum) {
	return _video->playVideo(fileNum);
}

bool ResourceManager::loadGraphic(uint8 fileNum, uint32 offset, uint32 index) {
	GraphicResource *res = getGraphic(fileNum, offset, index);

	_vm->getScreen()->setFrontBuffer(0, 0, res->width, res->height, res->data);

	// TODO proper error check
	return true;
}

bool ResourceManager::loadPalette(uint8 fileNum, uint32 offset) {
	Bundle *bun = getBundle(fileNum);
	Bundle *ent = bun->getEntry(offset);

	if(!ent->initialized){
		ent = new Bundle(fileNum, ent->offset, ent->size);
		bun->setEntry(offset, ent);
	}

	uint8 palette[256 * 3];
	memcpy(palette, ent->getData() + 32, 256 * 3);

	_vm->getScreen()->setPalette(palette);

	// TODO proper error check
	return true;
}

bool ResourceManager::loadCursor(uint8 fileNum, uint32 offet, uint32 index) {
	GraphicResource *cur = getGraphic(fileNum, offet, index);
	_vm->_system->setMouseCursor(cur->data, cur->width, cur->height, 1, 1, 0);
	_vm->_system->showMouse(true);

	// TODO proper error check
	return true;
}

GraphicResource* ResourceManager::getGraphic(uint8 fileNum, uint32 offset, uint32 index) {
	Bundle *bun = getBundle(fileNum);
	Bundle *ent = bun->getEntry(offset);
	GraphicBundle *gra;

	if(!ent->initialized){
		gra = new GraphicBundle(fileNum, ent->offset, ent->size);
		bun->setEntry(offset, gra);
	}else{
		gra = (GraphicBundle*)bun->getEntry(offset);
	}

	return gra->getEntry(index);
}

/*
GraphicBundle* ResourceManager::getGraphic(uint8 fileNum, uint32 offset) {
	Bundle *bun = getBundle(fileNum);
	Bundle *ent = bun->getEntry(offset);

	if(!ent->initialized){
		GraphicBundle *gra = new GraphicBundle(fileNum, ent->offset, ent->size);
		bun->setEntry(offset, gra);
	}

	return (GraphicBundle*)bun->getEntry(offset);
}

*/
Bundle* ResourceManager::getBundle(uint8 fileNum) {
	// first check if the bundle exists in the cache
	Bundle* bun = NULL;

	for (uint32 i = 0; i < _bundleCache.size(); i++) {
		if (_bundleCache[i].id == fileNum ){
			*bun = _bundleCache[i];
		}
	}

	if(!bun) {
		bun = new Bundle(fileNum);
	}

	return bun;
}

} // end of namespace Asylum
