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

#include "m4/burger/rooms/section5/section5.h"
#include "m4/burger/vars.h"

namespace M4 {
namespace Burger {
namespace Rooms {

Section5::Section5() : Rooms::Section() {
	add(501, &_room501);
	add(502, &_room502);
	add(503, &_room503);
	add(504, &_room504);
	add(505, &_room505);
	add(506, &_room506);
	add(507, &_room507);
	add(508, &_room508);
	add(509, &_room509);
	add(510, &_room510);
	add(511, &_room511);
	add(512, &_room512);
	add(513, &_room513);
}

void Section5::daemon() {
	// TODO: daemon method
	_G(kernel).continue_handling_trigger = true;
}

void Section5::parser() {
	_G(player).command_ready = false;
}

} // namespace Rooms
} // namespace Burger
} // namespace M4
