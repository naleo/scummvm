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

#ifndef PINK_CURSOR_MGR_H
#define PINK_CURSOR_MGR_H

#include "common/rect.h"

#include "graphics/wincursor.h"

#include "pink/objects/object.h"

namespace Pink {

class Actor;
class Action;
class GamePage;
class PinkEngine;

class CursorMgr : public Object {
public:
    CursorMgr(PinkEngine *game, GamePage *page);
    ~CursorMgr();

    void update();
    void setCursor(uint index, Common::Point point, const Common::String &itemName);
    void setCursor(Common::String &cursorName, Common::Point point);

private:
    void hideItem();
    void startAnimation(int index);

    Actor *_actor;
    GamePage *_page;
    PinkEngine *_game;

    uint _time;
    uint _firstFrameIndex;

    bool _isPlayingAnimation;
    bool _isSecondFrame;
};

} // End of namespace Pink

#endif
