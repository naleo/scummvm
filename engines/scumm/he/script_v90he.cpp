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

#ifdef ENABLE_HE

#include "scumm/actor.h"
#include "scumm/charset.h"
#include "scumm/he/animation_he.h"
#include "scumm/he/intern_he.h"
#include "scumm/he/logic_he.h"
#include "scumm/object.h"
#include "scumm/resource.h"
#include "scumm/scumm.h"
#include "scumm/sound.h"
#include "scumm/he/sprite_he.h"
#include "scumm/util.h"

namespace Scumm {

#define OPCODE(i, x)	_opcodes[i]._OPCODE(ScummEngine_v90he, x)

void ScummEngine_v90he::setupOpcodes() {
	ScummEngine_v80he::setupOpcodes();

	OPCODE(0x0a, o90_dup_n);

	/* 1C */
	OPCODE(0x1c, o90_wizImageOps);
	OPCODE(0x1d, o90_min);
	OPCODE(0x1e, o90_max);
	OPCODE(0x1f, o90_sin);
	/* 20 */
	OPCODE(0x20, o90_cos);
	OPCODE(0x21, o90_sqrt);
	OPCODE(0x22, o90_atan2);
	OPCODE(0x23, o90_getSegmentAngle);
	/* 24 */
	OPCODE(0x24, o90_getDistanceBetweenPoints);
	OPCODE(0x25, o90_getSpriteInfo);
	OPCODE(0x26, o90_setSpriteInfo);
	OPCODE(0x27, o90_getSpriteGroupInfo);
	/* 28 */
	OPCODE(0x28, o90_setSpriteGroupInfo);
	OPCODE(0x29, o90_getWizData);
	OPCODE(0x2a, o90_getActorData);
	OPCODE(0x2b, o90_priorityStartScript);
	/* 2C */
	OPCODE(0x2c, o90_priorityChainScript);
	OPCODE(0x2d, o90_videoOps);
	OPCODE(0x2e, o90_getVideoData);
	OPCODE(0x2f, o90_floodFill);
	/* 30 */
	OPCODE(0x30, o90_mod);
	OPCODE(0x31, o90_shl);
	OPCODE(0x32, o90_shr);
	OPCODE(0x33, o90_xor);
	/* 34 */
	OPCODE(0x34, o90_findAllObjectsWithClassOf);
	OPCODE(0x35, o90_getPolygonOverlap);
	OPCODE(0x36, o90_cond);
	OPCODE(0x37, o90_dim2dim2Array);
	/* 38 */
	OPCODE(0x38, o90_redim2dimArray);
	OPCODE(0x39, o90_getLinesIntersectionPoint);
	OPCODE(0x3a, o90_sortArray);

	OPCODE(0x44, o90_getObjectData);

	OPCODE(0x94, o90_getPaletteData);
	OPCODE(0x9e, o90_paletteOps);
	OPCODE(0xa5, o90_fontEnum);
	OPCODE(0xab, o90_getActorAnimProgress);
	OPCODE(0xc8, o90_kernelGetFunctions);
	OPCODE(0xc9, o90_kernelSetFunctions);
}

void ScummEngine_v90he::o90_dup_n() {
	int num;
	int args[16];

	push(fetchScriptWord());
	num = getStackList(args, ARRAYSIZE(args));

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < num; j++)
			push(args[j]);
	}
}

void ScummEngine_v90he::o90_wizImageOps() {
	int a, b;

	int subOp = fetchScriptByte();

	switch (subOp) {
	case SO_WIDTH: // 32, HE99+
		_wizImageCommand.actionFlags |= kWAFWidth;
		_wizImageCommand.width = pop();
		break;
	case SO_HEIGHT: // 33, HE99+
		_wizImageCommand.actionFlags |= kWAFHeight;
		_wizImageCommand.height = pop();
		break;
	case SO_GENERAL_CLIP_STATE: // 46
		// Dummy case
		pop();
		break;
	case SO_GENERAL_CLIP_RECT: // 47
		_wizImageCommand.box.bottom = pop();
		_wizImageCommand.box.right = pop();
		_wizImageCommand.box.top = pop();
		_wizImageCommand.box.left = pop();
		break;
	case SO_DRAW: // 48
		_wizImageCommand.actionMode = kWADraw;
		break;
	case SO_LOAD: // 49
		_wizImageCommand.actionFlags |= kWAFFilename;
		_wizImageCommand.actionMode = kWALoad;
		copyScriptString(_wizImageCommand.filename, sizeof(_wizImageCommand.filename));
		break;
	case SO_SAVE: // 50
		_wizImageCommand.actionFlags |= kWAFFilename;
		_wizImageCommand.actionMode = kWASave;
		copyScriptString(_wizImageCommand.filename, sizeof(_wizImageCommand.filename));
		_wizImageCommand.fileType = pop();
		break;
	case SO_CAPTURE: // 51
		_wizImageCommand.actionFlags |= kWAFRect | kWAFCompressionType;
		_wizImageCommand.actionMode = kWACapture;
		_wizImageCommand.box.bottom = pop();
		_wizImageCommand.box.right = pop();
		_wizImageCommand.box.top = pop();
		_wizImageCommand.box.left = pop();
		_wizImageCommand.compressionType = pop();
		adjustRect(_wizImageCommand.box);
		break;
	case SO_STATE: // 52
		_wizImageCommand.actionFlags |= kWAFState;
		_wizImageCommand.state = pop();
		break;
	case SO_ANGLE: // 53
		_wizImageCommand.actionFlags |= kWAFAngle;
		_wizImageCommand.angle = pop();
		break;
	case SO_SET_FLAGS: // 54
		_wizImageCommand.actionFlags |= kWAFFlags;
		_wizImageCommand.flags |= pop();
		break;
	case SO_NOW: // 56
		_wizImageCommand.flags = pop();
		_wizImageCommand.state = pop();
		_wizImageCommand.yPos = pop();
		_wizImageCommand.xPos = pop();
		_wizImageCommand.image = pop();

		_wiz->simpleDrawAWiz(
			_wizImageCommand.image,
			_wizImageCommand.state,
			_wizImageCommand.xPos,
			_wizImageCommand.yPos,
			_wizImageCommand.flags
		);

		break;
	case SO_INIT: // 57
		_wizImageCommand.image = pop();
		_wizImageCommand.actionMode = kWAUnknown;
		_wizImageCommand.actionFlags = 0;
		_wizImageCommand.remapCount = 0;
		_wizImageCommand.flags = 0;
		_wizImageCommand.propertyValue = 0;
		_wizImageCommand.propertyNumber = 0;
		_wizImageCommand.extendedRenderInfo.sprite = 0;
		_wizImageCommand.extendedRenderInfo.group = 0;
		break;
	case SO_AT_IMAGE: // 62, HE99+
		_wizImageCommand.actionFlags |= kWAFSourceImg;
		_wizImageCommand.sourceImage = pop();
		break;
	case SO_AT: // 65
	case SO_CURSOR_HOTSPOT: // 154
		_wizImageCommand.actionFlags |= kWAFSpot;
		_wizImageCommand.yPos = pop();
		_wizImageCommand.xPos = pop();
		break;
	case SO_COLOR: // 66
		// TODO: I haven't found SO_COLOR in any executable, recheck later...
	case SO_COLOR_LIST: // 249, HE98+
		b = pop();
		a = pop();
		_wizImageCommand.actionFlags |= kWAFRemapList;
		_wizImageCommand.actionMode = kWAModify;
		if (_wizImageCommand.remapCount == 0) {
			memset(_wizImageCommand.remapList, 0, sizeof(_wizImageCommand.remapList));
		}
		assert(_wizImageCommand.remapCount < ARRAYSIZE(_wizImageCommand.remapList));
		_wizImageCommand.remapList[_wizImageCommand.remapCount] = a;
		_wizImageCommand.remapTable[a] = b;
		_wizImageCommand.remapCount++;
		break;
	case SO_CLIPPED: // 67
		_wizImageCommand.actionFlags |= kWAFRect;
		_wizImageCommand.box.bottom = pop();
		_wizImageCommand.box.right = pop();
		_wizImageCommand.box.top = pop();
		_wizImageCommand.box.left = pop();
		adjustRect(_wizImageCommand.box);
		break;
	case SO_PALETTE: // 86, HE99+
		_wizImageCommand.actionFlags |= kWAFPalette;
		_wizImageCommand.palette = pop();
		break;
	case SO_SCALE: // 92
		_wizImageCommand.actionFlags |= kWAFScale;
		_wizImageCommand.scale = pop();
		break;
	case SO_SHADOW: // 98
		_wizImageCommand.actionFlags |= kWAFShadow;
		_wizImageCommand.shadow = pop();
		break;
	case SO_POLY_POLYGON: // 131, HE99+
		_wizImageCommand.actionFlags |= kWAFPolygon2 | kWAFCompressionType | kWAFPolygon;
		_wizImageCommand.actionMode = kWAPolyCapture;
		_wizImageCommand.polygon2 = pop();
		_wizImageCommand.polygon = pop();
		_wizImageCommand.compressionType = pop();
		break;
	case SO_RENDER_RECTANGLE: // 133, HE99+
		_wizImageCommand.actionFlags |= kWAFColor | kWAFRenderCoords;
		_wizImageCommand.actionMode = kWARenderRectangle;
		_wizImageCommand.colorValue = pop();
		_wizImageCommand.renderCoords.bottom = pop();
		_wizImageCommand.renderCoords.right = pop();
		_wizImageCommand.renderCoords.top = pop();
		_wizImageCommand.renderCoords.left = pop();
		adjustRect(_wizImageCommand.renderCoords);
		break;
	case SO_RENDER_LINE: // 134, HE99+
		_wizImageCommand.actionFlags |= kWAFColor | kWAFRenderCoords;
		_wizImageCommand.actionMode = kWARenderLine;
		_wizImageCommand.colorValue = pop();
		_wizImageCommand.renderCoords.bottom = pop();
		_wizImageCommand.renderCoords.right = pop();
		_wizImageCommand.renderCoords.top = pop();
		_wizImageCommand.renderCoords.left = pop();
		adjustRect(_wizImageCommand.renderCoords);
		break;
	case SO_RENDER_PIXEL: // 135, HE99+
		_wizImageCommand.actionFlags |= kWAFColor | kWAFRenderCoords;
		_wizImageCommand.actionMode = kWARenderPixel;
		_wizImageCommand.colorValue = pop();
		_wizImageCommand.renderCoords.top = _wizImageCommand.renderCoords.bottom = pop();
		_wizImageCommand.renderCoords.left = _wizImageCommand.renderCoords.right = pop();
		adjustRect(_wizImageCommand.renderCoords);
		break;
	case SO_RENDER_FLOOD_FILL: // 136, HE99+
		_wizImageCommand.actionFlags |= kWAFColor | kWAFRenderCoords;
		_wizImageCommand.actionMode = kWARenderFloodFill;
		_wizImageCommand.colorValue = pop();
		_wizImageCommand.renderCoords.top = _wizImageCommand.renderCoords.bottom = pop();
		_wizImageCommand.renderCoords.left = _wizImageCommand.renderCoords.right = pop();
		adjustRect(_wizImageCommand.renderCoords);
		break;
	case SO_RENDER_INTO_IMAGE: // 137, HE99+
		_wizImageCommand.actionFlags |= kWAFDestImage;
		_wizImageCommand.destImageNumber = pop();
		break;
	case SO_NEW_GENERAL_PROPERTY: // 139, HE99+
		_wizImageCommand.actionFlags |= kWAFProperty;
		_wizImageCommand.propertyValue = pop();
		_wizImageCommand.propertyNumber = pop();
		break;
	case SO_FONT_START: // 141, HE99+
		_wizImageCommand.actionMode = kWAFontStart;
		break;
	case SO_FONT_CREATE: // 142, HE99+
		_wizImageCommand.actionMode = kWAFontCreate;
		_wizImageCommand.fontProperties.bgColor = pop();
		_wizImageCommand.fontProperties.fgColor = pop();
		_wizImageCommand.fontProperties.size = pop();
		_wizImageCommand.fontProperties.style = pop();
		copyScriptString(_wizImageCommand.fontProperties.fontName, sizeof(_wizImageCommand.fontProperties.fontName));
		break;
	case SO_FONT_RENDER: // 143, HE99+
		_wizImageCommand.actionMode = kWAFontRender;
		_wizImageCommand.fontProperties.yPos = pop();
		_wizImageCommand.fontProperties.xPos = pop();
		copyScriptString(_wizImageCommand.fontProperties.string, sizeof(_wizImageCommand.fontProperties.string));
		break;
	case SO_RENDER_ELLIPSE: // 189, HE99+
		_wizImageCommand.actionMode = kWARenderEllipse;
		_wizImageCommand.ellipseProperties.color = pop();
		_wizImageCommand.ellipseProperties.lod = pop();
		_wizImageCommand.ellipseProperties.ky = pop();
		_wizImageCommand.ellipseProperties.kx = pop();
		_wizImageCommand.ellipseProperties.qy = pop();
		_wizImageCommand.ellipseProperties.qx = pop();
		_wizImageCommand.ellipseProperties.py = pop();
		_wizImageCommand.ellipseProperties.px = pop();
		break;
	case SO_FONT_END: // 196, HE99+
		_wizImageCommand.actionMode = kWAFontEnd;
		break;
	case SO_NEW: // 217, HE99+
		_wizImageCommand.actionMode = kWANew;
		break;
	case SO_SET_POLYGON: // 246
		_wizImageCommand.actionFlags |= kWAFFlags | kWAFSpot | kWAFPolygon;
		_wizImageCommand.flags |= kWRFIsPolygon;
		_wizImageCommand.polygon = _wizImageCommand.yPos = _wizImageCommand.xPos = pop();
		break;
	case SO_END: // 255
		if (_wizImageCommand.image)
			_wiz->processWizImageCmd(&_wizImageCommand);
		break;
	default:
		error("o90_wizImageOps: unhandled case %d", subOp);
	}
}

void ScummEngine_v90he::o90_min() {
	int a = pop();
	int b = pop();

	if (b < a) {
		push(b);
	} else {
		push(a);
	}
}

void ScummEngine_v90he::o90_max() {
	int a = pop();
	int b = pop();

	if (b > a) {
		push(b);
	} else {
		push(a);
	}
}

void ScummEngine_v90he::o90_sin() {
	// TODO: the math calculation albeit more precise
	// is not accurate enough with respect to what the
	// engine expects. Implement the correct one...
	double a = pop() * M_PI / 180.;
	push((int)(sin(a) * 100000));
}

void ScummEngine_v90he::o90_cos() {
	// TODO: the math calculation albeit more precise
	// is not accurate enough with respect to what the
	// engine expects. Implement the correct one...
	double a = pop() * M_PI / 180.;
	push((int)(cos(a) * 100000));
}

void ScummEngine_v90he::o90_sqrt() {
	// TODO: the math calculation albeit more precise
	// is not accurate enough with respect to what the
	// engine expects. Implement the correct one...
	int i = pop();
	if (i < 2) {
		push(i);
	} else {
		push((int)sqrt((double)(i + 1)));
	}
}

void ScummEngine_v90he::o90_atan2() {
	int y = pop();
	int x = pop();
	int a = (int)(atan2((double)y, (double)x) * 180. / M_PI);
	if (a < 0) {
		a += 360;
	}
	push(a);
}

void ScummEngine_v90he::o90_getSegmentAngle() {
	int y1 = pop();
	int x1 = pop();
	int dy = y1 - pop();
	int dx = x1 - pop();
	int a = (int)(atan2((double)dy, (double)dx) * 180. / M_PI);
	if (a < 0) {
		a += 360;
	}
	push(a);
}

void ScummEngine_v90he::o90_getDistanceBetweenPoints() {
	int x1, y1, z1, x2, y2, z2, dx, dy, dz, d;

	byte subOp = fetchScriptByte();

	switch (subOp) {
	case ScummEngine_v100he::SO_COORD_2D: // 23
	case SO_COORD_2D:                     // 28
		y2 = pop();
		x2 = pop();
		y1 = pop();
		x1 = pop();
		dx = x2 - x1;
		dy = y2 - y1;
		d = dx * dx + dy * dy;
		if (d < 2) {
			push(d);
		} else {
			push((int)sqrt((double)(d + 1)));
		}
		break;
	case ScummEngine_v100he::SO_COORD_3D: // 24
	case SO_COORD_3D:                     // 29
		z2 = pop();
		y2 = pop();
		x2 = pop();
		z1 = pop();
		y1 = pop();
		x1 = pop();
		dx = x2 - x1;
		dy = y2 - y1;
		dz = z2 - z1;
		d = dx * dx + dy * dy + dz * dz;
		if (d < 2) {
			push(d);
		} else {
			push((int)sqrt((double)(d + 1)));
		}
		break;
	default:
		error("o90_getDistanceBetweenPoints: Unknown case %d", subOp);
	}
}

void ScummEngine_v90he::o90_getSpriteInfo() {
	int args[16];
	int spriteId, flags, groupId, type, count;
	int32 x, y;

	byte subOp = fetchScriptByte();

	switch (subOp) {
	case SO_XPOS: // 30
		spriteId = pop();
		if (spriteId) {
			_sprite->getSpritePosition(spriteId, x, y);
			push(x);
		} else {
			push(0);
		}
		break;
	case SO_YPOS: // 31
		spriteId = pop();
		if (spriteId) {
			_sprite->getSpritePosition(spriteId, x, y);
			push(y);
		} else {
			push(0);
		}
		break;
	case SO_WIDTH: // 32
		spriteId = pop();
		if (spriteId) {
			_sprite->getSpriteImageDim(spriteId, x, y);
			push(x);
		} else {
			push(0);
		}
		break;
	case SO_HEIGHT: // 33
		spriteId = pop();
		if (spriteId) {
			_sprite->getSpriteImageDim(spriteId, x, y);
			push(y);
		} else {
			push(0);
		}
		break;
	case SO_STEP_DIST_X: // 34
		spriteId = pop();
		if (spriteId) {
			_sprite->getSpriteDist(spriteId, x, y);
			push(x);
		} else {
			push(0);
		}
		break;
	case SO_STEP_DIST_Y: // 35
		spriteId = pop();
		if (spriteId) {
			_sprite->getSpriteDist(spriteId, x, y);
			push(y);
		} else {
			push(0);
		}
		break;
	case SO_COUNT: // 36
		spriteId = pop();
		if (spriteId)
			push(_sprite->getSpriteImageStateCount(spriteId));
		else
			push(0);
		break;
	case SO_GROUP: // 37
		spriteId = pop();
		if (spriteId)
			push(_sprite->getSpriteGroup(spriteId));
		else
			push(0);
		break;
	case SO_DRAW_XPOS: // 38
		spriteId = pop();
		if (spriteId)
			push(_sprite->getSpriteDisplayX(spriteId));
		else
			push(0);
		break;
	case SO_DRAW_YPOS: // 39
		spriteId = pop();
		if (spriteId)
			push(_sprite->getSpriteDisplayY(spriteId));
		else
			push(0);
		break;
	case SO_PROPERTY: // 42
		flags = pop();
		spriteId = pop();
		if (spriteId) {
			switch (flags) {
			case SPRPROP_HFLIP: // 0
				push(_sprite->getSpriteFlagXFlipped(spriteId));
				break;
			case SPRPROP_VFLIP: // 1
				push(_sprite->getSpriteFlagYFlipped(spriteId));
				break;
			case SPRPROP_ACTIVE: // 2
				push(_sprite->getSpriteFlagActive(spriteId));
				break;
			case SPRPROP_BACKGROUND_RENDER: // 3
				push(_sprite->getSpriteFlagDoubleBuffered(spriteId));
				break;
			case SPRPROP_USE_IMAGE_REMAP_TABLE: // 4
				push(_sprite->getSpriteFlagRemapPalette(spriteId));
				break;
			default:
				push(0);
			}
		} else {
			push(0);
		}
		break;
	case SO_PRIORITY: // 43
		spriteId = pop();
		if (spriteId)
			push(_sprite->getSpritePriority(spriteId));
		else
			push(0);
		break;
	case SO_FIND: // 45
		if (_game.heversion == 99) {
			flags = getStackList(args, ARRAYSIZE(args));
			type = pop();
			groupId = pop();
			y = pop();
			x = pop();
			push(_sprite->findSpriteWithClassOf(x, y, groupId, type, flags, args));
		} else if (_game.heversion == 98) {
			type = pop();
			groupId = pop();
			y = pop();
			x = pop();
			push(_sprite->findSpriteWithClassOf(x, y, groupId, type, 0, 0));
		} else {
			groupId = pop();
			y = pop();
			x = pop();
			push(_sprite->findSpriteWithClassOf(x, y, groupId, 0, 0, 0));
		}
		break;
	case SO_STATE: // 52
		spriteId = pop();
		if (spriteId)
			push(_sprite->getSpriteImageState(spriteId));
		else
			push(0);
		break;
	case SO_AT_IMAGE: // 62
		spriteId = pop();
		if (spriteId)
			push(_sprite->getSpriteSourceImage(spriteId));
		else
			push(0);
		break;
	case SO_IMAGE: // 63
		spriteId = pop();
		if (spriteId)
			push(_sprite->getSpriteImage(spriteId));
		else
			push(0);
		break;
	case SO_ERASE: // 68
		spriteId = pop();
		if (spriteId)
			push(_sprite->getSpriteFlagEraseType(spriteId));
		else
			push(1);
		break;
	case SO_ANIMATION: // 82
		spriteId = pop();
		if (spriteId)
			push(_sprite->getSpriteFlagAutoAnim(spriteId));
		else
			push(0);
		break;
	case SO_PALETTE: // 86
		spriteId = pop();
		if (spriteId)
			push(_sprite->getSpritePalette(spriteId));
		else
			push(0);
		break;
	case SO_SCALE: // 92
		spriteId = pop();
		if (spriteId)
			push(_sprite->getSpriteScale(spriteId));
		else
			push(0);
		break;
	case SO_ANIMATION_SPEED: // 97
		spriteId = pop();
		if (spriteId)
			push(_sprite->getSpriteAnimSpeed(spriteId));
		else
			push(1);
		break;
	case SO_SHADOW: // 98
		spriteId = pop();
		if (spriteId)
			push(_sprite->getSpriteShadow(spriteId));
		else
			push(0);
		break;
	case SO_UPDATE: // 124
		spriteId = pop();
		if (spriteId)
			push(_sprite->getSpriteFlagUpdateType(spriteId));
		else
			push(0);
		break;
	case SO_CLASS: // 125
		count = getStackList(args, ARRAYSIZE(args));
		spriteId = pop();

		// TODO: Implement HE98-99-100 differences here
		if (spriteId) {
			push(_sprite->getSpriteClass(spriteId, count, args));
		} else {
			push(0);
		}
		break;
	case SO_NEW_GENERAL_PROPERTY: // 139
		flags = pop();
		spriteId = pop();
		if (spriteId)
			push(_sprite->getSpriteGeneralProperty(spriteId, flags));
		else
			push(0);
		break;
	case SO_MASK: // 140
		spriteId = pop();
		if (spriteId)
			push(_sprite->getSpriteMaskImage(spriteId));
		else
			push(0);
		break;
	case SO_ACTOR_VARIABLE: // 198
		pop();
		spriteId = pop();
		if (spriteId)
			push(_sprite->getSpriteUserValue(spriteId));
		else
			push(0);
		break;
	default:
		error("o90_getSpriteInfo: Unknown case %d", subOp);
	}
}

void ScummEngine_v90he::o90_setSpriteInfo() {
	int args[16];
	int spriteId;
	int32 tmp[2];
	int n;

	byte subOp = fetchScriptByte();

	switch (subOp) {
	case SO_STEP_DIST_X: // 34
		args[0] = pop();
		if (_curSpriteId > _curMaxSpriteId)
			break;
		spriteId = _curSpriteId;
		if (!spriteId)
			spriteId++;

		for (; spriteId <= _curMaxSpriteId; spriteId++) {
			_sprite->getSpriteDist(spriteId, tmp[0], tmp[1]);
			_sprite->setSpriteDist(spriteId, args[0], tmp[1]);
		}
		break;
	case SO_STEP_DIST_Y: // 35
		args[0] = pop();
		if (_curSpriteId > _curMaxSpriteId)
			break;
		spriteId = _curSpriteId;
		if (!spriteId)
			spriteId++;

		for (; spriteId <= _curMaxSpriteId; spriteId++) {
			_sprite->getSpriteDist(spriteId, tmp[0], tmp[1]);
			_sprite->setSpriteDist(spriteId, tmp[0], args[0]);
		}
		break;
	case SO_GROUP: // 37
		args[0] = pop();
		if (_curSpriteId > _curMaxSpriteId)
			break;
		spriteId = _curSpriteId;
		if (!spriteId)
			spriteId++;

		for (; spriteId <= _curMaxSpriteId; spriteId++)
			_sprite->setSpriteGroup(spriteId, args[0]);
		break;
	case SO_PROPERTY: // 42
		args[1] = pop();
		args[0] = pop();
		if (_curSpriteId > _curMaxSpriteId)
			break;
		spriteId = _curSpriteId;
		if (!spriteId)
			spriteId++;

		for (; spriteId <= _curMaxSpriteId; spriteId++)
			switch (args[1]) {
			case SPRPROP_HFLIP: // 0
				_sprite->setSpriteFlagXFlipped(spriteId, args[0]);
				break;
			case SPRPROP_VFLIP: // 1
				_sprite->setSpriteFlagYFlipped(spriteId, args[0]);
				break;
			case SPRPROP_ACTIVE: // 2
				_sprite->setSpriteFlagActive(spriteId, args[0]);
				break;
			case SPRPROP_BACKGROUND_RENDER: // 3
				_sprite->setSpriteFlagDoubleBuffered(spriteId, args[0]);
				break;
			case SPRPROP_USE_IMAGE_REMAP_TABLE: // 4
				_sprite->setSpriteFlagRemapPalette(spriteId, args[0]);
				break;
			default:
				break;
			}
		break;
	case SO_PRIORITY: // 43
		args[0] = pop();
		if (_curSpriteId > _curMaxSpriteId)
			break;
		spriteId = _curSpriteId;
		if (!spriteId)
			spriteId++;

		for (; spriteId <= _curMaxSpriteId; spriteId++)
			_sprite->setSpritePriority(spriteId, args[0]);
		break;
	case SO_MOVE: // 44
		args[1] = pop();
		args[0] = pop();
		if (_curSpriteId > _curMaxSpriteId)
			break;
		spriteId = _curSpriteId;
		if (!spriteId)
			spriteId++;

		for (; spriteId <= _curMaxSpriteId; spriteId++)
			_sprite->moveSprite(spriteId, args[0], args[1]);
		break;
	case SO_STATE: // 52
		args[0] = pop();
		if (_curSpriteId > _curMaxSpriteId)
			break;
		spriteId = _curSpriteId;
		if (!spriteId)
			spriteId++;

		for (; spriteId <= _curMaxSpriteId; spriteId++)
			_sprite->setSpriteImageState(spriteId, args[0]);
		break;
	case SO_ANGLE: // 53
		args[0] = pop();
		if (_curSpriteId > _curMaxSpriteId)
			break;
		spriteId = _curSpriteId;
		if (!spriteId)
			spriteId++;

		for (; spriteId <= _curMaxSpriteId; spriteId++)
			_sprite->setSpriteAngle(spriteId, args[0]);
		break;
	case SO_INIT: // 57
		if (_game.features & GF_HE_985 || _game.heversion >= 99) {
			_curMaxSpriteId = pop();
			_curSpriteId = pop();

			if (_curSpriteId > _curMaxSpriteId)
				SWAP(_curSpriteId, _curMaxSpriteId);
		} else {
			_curSpriteId = pop();
			_curMaxSpriteId = _curSpriteId; // to make all functions happy
		}
		break;
	case SO_AT_IMAGE: // 62, HE99+
		args[0] = pop();
		if (_curSpriteId > _curMaxSpriteId)
			break;
		spriteId = _curSpriteId;
		if (!spriteId)
			spriteId++;

		for (; spriteId <= _curMaxSpriteId; spriteId++)
			_sprite->setSpriteSourceImage(spriteId, args[0]);
		break;
	case SO_IMAGE: // 63
		args[0] = pop();
		if (_curSpriteId > _curMaxSpriteId)
			break;
		spriteId = _curSpriteId;
		if (!spriteId)
			spriteId++;

		for (; spriteId <= _curMaxSpriteId; spriteId++)
			_sprite->setSpriteImage(spriteId, args[0]);
		break;
	case SO_AT: // 65
		args[1] = pop();
		args[0] = pop();
		if (_curSpriteId > _curMaxSpriteId)
			break;
		spriteId = _curSpriteId;
		if (!spriteId)
			spriteId++;

		for (; spriteId <= _curMaxSpriteId; spriteId++)
			_sprite->setSpritePosition(spriteId, args[0], args[1]);
		break;
	case SO_ERASE: // 68
		args[0] = pop();
		if (_curSpriteId > _curMaxSpriteId)
			break;
		spriteId = _curSpriteId;
		if (!spriteId)
			spriteId++;

		for (; spriteId <= _curMaxSpriteId; spriteId++)
			_sprite->setSpriteFlagEraseType(spriteId, args[0]);
		break;
	case SO_STEP_DIST: // 77
		args[1] = pop();
		args[0] = pop();
		if (_curSpriteId > _curMaxSpriteId)
			break;
		spriteId = _curSpriteId;
		if (!spriteId)
			spriteId++;

		for (; spriteId <= _curMaxSpriteId; spriteId++)
			_sprite->setSpriteDist(spriteId, args[0], args[1]);
		break;
	case SO_ANIMATION: // 82
		args[0] = pop();
		if (_curSpriteId > _curMaxSpriteId)
			break;
		spriteId = _curSpriteId;
		if (!spriteId)
			spriteId++;

		for (; spriteId <= _curMaxSpriteId; spriteId++)
			_sprite->setSpriteFlagAutoAnim(spriteId, args[0]);
		break;
	case SO_PALETTE: // 86, HE98+
		args[0] = pop();
		if (_curSpriteId > _curMaxSpriteId)
			break;
		spriteId = _curSpriteId;
		if (!spriteId)
			spriteId++;

		for (; spriteId <= _curMaxSpriteId; spriteId++)
			_sprite->setSpritePalette(spriteId, args[0]);
		break;
	case SO_SCALE: // 92, HE99+
		args[0] = pop();
		if (_curSpriteId > _curMaxSpriteId)
			break;
		spriteId = _curSpriteId;
		if (!spriteId)
			spriteId++;

		for (; spriteId <= _curMaxSpriteId; spriteId++)
			_sprite->setSpriteScale(spriteId, args[0]);
		break;
	case SO_ANIMATION_SPEED: // 97, HE98+
		args[0] = pop();
		if (_curSpriteId > _curMaxSpriteId)
			break;
		spriteId = _curSpriteId;
		if (!spriteId)
			spriteId++;

		for (; spriteId <= _curMaxSpriteId; spriteId++)
			_sprite->setSpriteAnimSpeed(spriteId, args[0]);
		break;
	case SO_SHADOW: // 98
		args[0] = pop();
		if (_curSpriteId > _curMaxSpriteId)
			break;
		spriteId = _curSpriteId;
		if (!spriteId)
			spriteId++;

		for (; spriteId <= _curMaxSpriteId; spriteId++)
			_sprite->setSpriteShadow(spriteId, args[0]);
		break;
	case SO_UPDATE: // 124
		args[0] = pop();
		if (_curSpriteId > _curMaxSpriteId)
			break;
		spriteId = _curSpriteId;
		if (!spriteId)
			spriteId++;

		for (; spriteId <= _curMaxSpriteId; spriteId++)
			_sprite->setSpriteFlagUpdateType(spriteId, args[0]);
		break;
	case SO_CLASS: // 125
		n = getStackList(args, ARRAYSIZE(args));
		if (_curSpriteId != 0 && _curMaxSpriteId != 0 && n != 0) {
			int *p = &args[n - 1];
			do {
				int code = *p;
				if (code == 0) {
					for (int i = _curSpriteId; i <= _curMaxSpriteId; ++i) {
						_sprite->setSpriteResetClass(i);
					}
				} else if (code & 0x80) {
					for (int i = _curSpriteId; i <= _curMaxSpriteId; ++i) {
						_sprite->setSpriteSetClass(i, code & 0x7F, 1);
					}
				} else {
					for (int i = _curSpriteId; i <= _curMaxSpriteId; ++i) {
						_sprite->setSpriteSetClass(i, code & 0x7F, 0);
					}
				}
				--p;
			} while (--n);
		}
		break;
	case SO_NEW_GENERAL_PROPERTY: // 139, HE99+
		args[1] = pop();
		args[0] = pop();
		if (_curSpriteId > _curMaxSpriteId)
			break;
		spriteId = _curSpriteId;
		if (!spriteId)
			spriteId++;

		for (; spriteId <= _curMaxSpriteId; spriteId++)
			_sprite->setSpriteGeneralProperty(spriteId, args[0], args[1]);
		break;
	case SO_MASK: // 140, HE99+
		args[0] = pop();
		if (_curSpriteId > _curMaxSpriteId)
			break;
		spriteId = _curSpriteId;
		if (!spriteId)
			spriteId++;

		for (; spriteId <= _curMaxSpriteId; spriteId++)
			_sprite->setSpriteMaskImage(spriteId, args[0]);
		break;
	case SO_RESTART: // 158
		_sprite->resetTables(true);
		break;
	case SO_ACTOR_VARIABLE: // 198
		args[1] = pop();
		args[0] = pop();
		if (_curSpriteId > _curMaxSpriteId)
			break;
		spriteId = _curSpriteId;
		if (!spriteId)
			spriteId++;

		for (; spriteId <= _curMaxSpriteId; spriteId++)
			_sprite->setSpriteUserValue(spriteId, args[0], args[1]);
		break;
	case SO_NEW: // 217
		if (_curSpriteId > _curMaxSpriteId)
			break;
		spriteId = _curSpriteId;
		if (!spriteId)
			spriteId++;

		for (; spriteId <= _curMaxSpriteId; spriteId++)
			_sprite->resetSprite(spriteId);
		break;
	default:
		error("o90_setSpriteInfo: Unknown case %d", subOp);
	}
}

void ScummEngine_v90he::o90_getSpriteGroupInfo() {
	int32 tx, ty;
	int spriteGroupId, type;

	byte subOp = fetchScriptByte();

	switch (subOp) {
	case SO_ARRAY: // 8, HE99+
		spriteGroupId = pop();
		if (spriteGroupId)
			push(getGroupSpriteArray(spriteGroupId));
		else
			push(0);
		break;
	case SO_XPOS: // 30
		spriteGroupId = pop();
		if (spriteGroupId) {
			_sprite->getGroupPosition(spriteGroupId, tx, ty);
			push(tx);
		} else {
			push(0);
		}
		break;
	case SO_YPOS: // 31
		spriteGroupId = pop();
		if (spriteGroupId) {
			_sprite->getGroupPosition(spriteGroupId, tx, ty);
			push(ty);
		} else {
			push(0);
		}
		break;
	case SO_PROPERTY: // 42, HE99+
		type = pop();
		spriteGroupId = pop();
		if (spriteGroupId) {
			switch (type) {
			case SPRGRPPROP_XMUL: // 0
				push(_sprite->getGroupXMul(spriteGroupId));
				break;
			case SPRGRPPROP_XDIV: // 1
				push(_sprite->getGroupXDiv(spriteGroupId));
				break;
			case SPRGRPPROP_YMUL: // 2
				push(_sprite->getGroupYMul(spriteGroupId));
				break;
			case SPRGRPPROP_YDIV: // 3
				push(_sprite->getGroupYDiv(spriteGroupId));
				break;
			default:
				push(0);
			}
		} else {
			push(0);
		}
		break;
	case SO_PRIORITY: // 43
		spriteGroupId = pop();
		if (spriteGroupId)
			push(_sprite->getGroupPriority(spriteGroupId));
		else
			push(0);
		break;
	case SO_IMAGE: // 63, HE99+
		spriteGroupId = pop();
		if (spriteGroupId)
			push(_sprite->getGroupDstResNum(spriteGroupId));
		else
			push(0);
		break;
	case SO_NEW_GENERAL_PROPERTY: // 139, HE99+
		pop(); // propertyCode
		pop(); // groupNum

		// The original just checked if groupNum was within
		// bounds and just pushed 0 to the stack
		push(0);
		break;
	default:
		error("o90_getSpriteGroupInfo: Unknown case %d", subOp);
	}
}

void ScummEngine_v90he::o90_setSpriteGroupInfo() {
	int type, value1, value2, value3, value4;

	byte subOp = fetchScriptByte();

	switch (subOp) {
	case SO_GROUP: // 37
		type = pop();
		switch (type) {
		case SPRGRPOP_MOVE: // 1
			value2 = pop();
			value1 = pop();
			if (!_curSpriteGroupId)
				break;

			_sprite->moveGroupMembers(_curSpriteGroupId, value1, value2);
			break;
		case SPRGRPOP_ORDER: // 2
			value1 = pop();
			if (!_curSpriteGroupId)
				break;

			_sprite->setGroupMembersPriority(_curSpriteGroupId, value1);
			break;
		case SPRGRPOP_NEW_GROUP: // 3
			value1 = pop();
			if (!_curSpriteGroupId)
				break;

			_sprite->setGroupMembersGroup(_curSpriteGroupId, value1);
			break;
		case SPRGRPOP_UPDATE_TYPE: // 4
			value1 = pop();
			if (!_curSpriteGroupId)
				break;

			_sprite->setGroupMembersUpdateType(_curSpriteGroupId, value1);
			break;
		case SPRGRPOP_NEW: // 5
			if (!_curSpriteGroupId)
				break;

			_sprite->setGroupMembersResetSprite(_curSpriteGroupId);
			break;
		case SPRGRPOP_ANIMATION_SPEED: // 6
			value1 = pop();
			if (!_curSpriteGroupId)
				break;

			_sprite->setGroupMembersAnimationSpeed(_curSpriteGroupId, value1);
			break;
		case SPRGRPOP_ANIMATION_TYPE: // 7
			value1 = pop();
			if (!_curSpriteGroupId)
				break;

			_sprite->setGroupMembersAutoAnimFlag(_curSpriteGroupId, value1);
			break;
		case SPRGRPOP_SHADOW: // 8
			value1 = pop();
			if (!_curSpriteGroupId)
				break;

			_sprite->setGroupMembersShadow(_curSpriteGroupId, value1);
			break;
		default:
			error("o90_setSpriteGroupInfo subOp 0: Unknown case %d", subOp);
		}
		break;
	case SO_PROPERTY: // 42
		type = pop();
		value1 = pop();
		if (!_curSpriteGroupId)
			break;

		switch (type) {
		case SPRGRPPROP_XMUL: // 0
			_sprite->setGroupXMul(_curSpriteGroupId, value1);
			break;
		case SPRGRPPROP_XDIV: // 1
			_sprite->setGroupXDiv(_curSpriteGroupId, value1);
			break;
		case SPRGRPPROP_YMUL: // 2
			_sprite->setGroupYMul(_curSpriteGroupId, value1);
			break;
		case SPRGRPPROP_YDIV: // 3
			_sprite->setGroupYDiv(_curSpriteGroupId, value1);
			break;
		default:
			error("o90_setSpriteGroupInfo subOp 5: Unknown case %d", subOp);
		}
		break;
	case SO_PRIORITY: // 43
		value1 = pop();
		if (!_curSpriteGroupId)
			break;

		_sprite->setGroupPriority(_curSpriteGroupId, value1);
		break;
	case SO_MOVE: // 44
		value2 = pop();
		value1 = pop();
		if (!_curSpriteGroupId)
			break;

		_sprite->moveGroup(_curSpriteGroupId, value1, value2);
		break;
	case SO_INIT: // 57
		_curSpriteGroupId = pop();
		break;
	case SO_IMAGE: // 63
		value1 = pop();
		if (!_curSpriteGroupId)
			break;

		_sprite->setGroupImage(_curSpriteGroupId, value1);
		break;
	case SO_AT: // 65
		value2 = pop();
		value1 = pop();
		if (!_curSpriteGroupId)
			break;

		_sprite->setGroupPosition(_curSpriteGroupId, value1, value2);
		break;
	case SO_CLIPPED: // 67
		value4 = pop();
		value3 = pop();
		value2 = pop();
		value1 = pop();
		if (!_curSpriteGroupId)
			break;

		_sprite->setGroupBounds(_curSpriteGroupId, value1, value2, value3, value4);
		break;
	case SO_NEVER_ZCLIP: // 93
		if (!_curSpriteGroupId)
			break;

		_sprite->resetGroupBounds(_curSpriteGroupId);
		break;
	case SO_NEW: // 217
		if (!_curSpriteGroupId)
			break;

		_sprite->resetGroup(_curSpriteGroupId);
		break;
	default:
		error("o90_setSpriteGroupInfo: Unknown case %d", subOp);
	}
}

void ScummEngine_v90he::o90_getWizData() {
	byte filename[4096];
	int resId, state, type;
	int fontImageNum, fontProperty;
	int32 w, h;
	int32 x, y;

	byte subOp = fetchScriptByte();

	switch (subOp) {
	case SO_XPOS: // 30
		state = pop();
		resId = pop();
		_wiz->getWizImageSpot(resId, state, x, y);
		push(x);
		break;
	case SO_YPOS: // 31
		state = pop();
		resId = pop();
		_wiz->getWizImageSpot(resId, state, x, y);
		push(y);
		break;
	case SO_WIDTH: // 32
		state = pop();
		resId = pop();
		_wiz->getWizImageDim(resId, state, w, h);
		push(w);
		break;
	case SO_HEIGHT: // 33
		state = pop();
		resId = pop();
		_wiz->getWizImageDim(resId, state, w, h);
		push(h);
		break;
	case SO_COUNT: // 36
		resId = pop();
		push(_wiz->getWizImageStates(resId));
		break;
	case SO_FIND: // 45
		y = pop();
		x = pop();
		state = pop();
		resId = pop();
		push(_wiz->isWizPixelNonTransparent(resId, state, x, y, 0));
		break;
	case SO_COLOR: // 66
		y = pop();
		x = pop();
		state = pop();
		resId = pop();
		push(_wiz->getWizPixelColor(resId, state, x, y));
		break;
	case SO_HISTOGRAM: // 130
		h = pop();
		w = pop();
		y = pop();
		x = pop();
		state = pop();
		resId = pop();
		if (x == -1 && y == -1 && w == -1 && h == -1) {
			_wiz->getWizImageDim(resId, state, w, h);
			x = 0;
			y = 0;
		}
		push(computeWizHistogram(resId, state, x, y, w, h));
		break;
	case SO_NEW_GENERAL_PROPERTY: // 139
		// TODO: Recheck, this looks different at least in HE99...
		type = pop();
		state = pop();
		resId = pop();
		push(_wiz->getWizImageData(resId, state, type));
		break;
	case SO_FONT_START: // 141
		// TODO: Implement...

		fontProperty = pop();
		copyScriptString(filename, sizeof(filename));
		fontImageNum = pop();

		if (fontImageNum) {
			switch (fontProperty) {
			case 2: // PFONT_EXTENT_X
				//push(PFONT_GetStringWidth(iImage, szResultString));
				push(0);
				break;

			case 3: // PFONT_EXTENT_Y
				//push(PFONT_GetStringHeight(iImage, szResultString));
				push(0);
				break;
			default:
				// No default case in the original...
				break;
			}
		} else {
			push(0);
		}

		debug(0, "o90_getWizData() case 111 unhandled");
		break;
	default:
		error("o90_getWizData: Unknown case %d", subOp);
	}
}

void ScummEngine_v90he::o90_getActorData() {
	ActorHE *a;

	int subOp = pop();
	int val = pop();
	int act = pop();

	a = (ActorHE *)derefActor(act, "o90_getActorData");

	switch (subOp) {
	case 1:
		push(a->isUserConditionSet(val));
		break;
	case 2:
		assertRange(0, val, 15, "o90_getActorData: Limb");
		push(a->_cost.frame[val] * 4);
		break;
	case 3:
		push(a->getAnimSpeed());
		break;
	case 4:
		push(a->_shadowMode);
		break;
	case 5:
		push(a->_layer);
		break;
	case 6:
		push(a->_hePaletteNum);
		break;
	default:
		error("o90_getActorData: Unknown actor property %d", subOp);
	}
}

void ScummEngine_v90he::o90_priorityStartScript() {
	int args[25];
	int script, cycle;
	byte flags;

	getStackList(args, ARRAYSIZE(args));
	cycle = pop();
	script = pop();
	flags = fetchScriptByte();
	runScript(script, (flags == SO_BAK || flags == SO_BAKREC), (flags == SO_REC || flags == SO_BAKREC), args, cycle);
}

void ScummEngine_v90he::o90_priorityChainScript() {
	int args[25];
	int script, cycle;
	byte flags;

	getStackList(args, ARRAYSIZE(args));
	cycle = pop();
	script = pop();
	flags = fetchScriptByte();
	stopObjectCode();
	runScript(script, (flags == SO_BAK || flags == SO_BAKREC), (flags == SO_REC || flags == SO_BAKREC), args, cycle);
}

void ScummEngine_v90he::o90_videoOps() {
	// Uses Smacker video
	int status = fetchScriptByte();
	int subOp = status;

	switch (subOp) {
	case SO_LOAD: // 49
		copyScriptString(_videoParams.filename, sizeof(_videoParams.filename));
		_videoParams.status = status;
		break;
	case SO_SET_FLAGS: // 54
		_videoParams.flags |= pop();
		break;
	case SO_INIT: // 57
		memset(_videoParams.filename, 0, sizeof(_videoParams.filename));
		_videoParams.status = 0;
		_videoParams.flags = 0;
		_videoParams.number = pop();
		_videoParams.wizResNum = 0;
		break;
	case SO_IMAGE: // 63
		_videoParams.wizResNum = pop();
		if (_videoParams.wizResNum)
			_videoParams.flags |= MoviePlayer::vfImageSurface;
		break;
	case SO_CLOSE: // 165
		_videoParams.status = status;
		break;
	case SO_END: // 255
		if (_videoParams.status == SO_LOAD) {
			// Start video
			if (_videoParams.flags == 0)
				_videoParams.flags = MoviePlayer::vfDefault;

			if (_videoParams.flags & MoviePlayer::vfImageSurface) {
				VAR(VAR_OPERATION_FAILURE) = _moviePlay->load(convertFilePath(_videoParams.filename), _videoParams.flags, _videoParams.wizResNum);
			} else {
				VAR(VAR_OPERATION_FAILURE) = _moviePlay->load(convertFilePath(_videoParams.filename), _videoParams.flags);
			}
		} else if (_videoParams.status == SO_CLOSE) {
			// Stop video
			_moviePlay->close();
		}
		break;
	default:
		error("o90_videoOps: unhandled case %d", subOp);
	}
}

void ScummEngine_v90he::o90_getVideoData() {
	// Uses Smacker video
	byte subOp = fetchScriptByte();

	switch (subOp) {
	case SO_WIDTH: // 32
		pop();
		push(_moviePlay->getWidth());
		break;
	case SO_HEIGHT: // 33
		pop();
		push(_moviePlay->getHeight());
		break;
	case SO_COUNT: // 36
		pop();
		push(_moviePlay->getFrameCount());
		break;
	case SO_STATE: // 52
		pop();
		push(_moviePlay->getCurFrame());
		break;
	case SO_IMAGE: // 63
		pop();
		push(_moviePlay->getImageNum());
		break;
	case SO_NEW_GENERAL_PROPERTY: // 139
		debug(0, "o90_getVideoData: subOp 107 stub (%d, %d)", pop(), pop());
		push(0);
		break;
	default:
		error("o90_getVideoData: unhandled case %d", subOp);
	}
}

void ScummEngine_v90he::o90_floodFill() {
	byte subOp = fetchScriptByte();

	switch (subOp) {
	case SO_SET_FLAGS: // 54
		_floodFillCommand.flags |= pop();
		break;
	case SO_INIT: // 57
		_floodFillCommand.reset();
		_floodFillCommand.box.left = 0;
		_floodFillCommand.box.top = 0;
		_floodFillCommand.box.right = 639;
		_floodFillCommand.box.bottom = 479;
		break;
	case SO_AT: // 65
		_floodFillCommand.y = pop();
		_floodFillCommand.x = pop();
		break;
	case SO_COLOR: // 66
		_floodFillCommand.color = pop();
		break;
	case SO_CLIPPED: // 67
		_floodFillCommand.box.bottom = pop();
		_floodFillCommand.box.right = pop();
		_floodFillCommand.box.top = pop();
		_floodFillCommand.box.left = pop();
		break;
	case SO_END: // 255
		_wiz->pgFloodFillCmd(_floodFillCommand.x, _floodFillCommand.y, _floodFillCommand.color, &_floodFillCommand.box);
		break;
	default:
		error("o90_floodFill: Unknown case %d", subOp);
	}
}

void ScummEngine_v90he::o90_mod() {
	int a = pop();
	if (a == 0)
		error("modulus by zero");
	push(pop() % a);
}

void ScummEngine_v90he::o90_shl() {
	int a = pop();
	push(pop() << a);
}

void ScummEngine_v90he::o90_shr() {
	int a = pop();
	push(pop() >> a);
}

void ScummEngine_v90he::o90_xor() {
	int a = pop();
	push(pop() ^ a);
}

void ScummEngine_v90he::o90_findAllObjectsWithClassOf() {
	int args[16];
	int cond, num, cls, tmp;
	bool b;

	num = getStackList(args, ARRAYSIZE(args));
	int room = pop();
	int numObjs = 0;

	if (room != _currentRoom)
		error("o90_findAllObjectsWithClassOf: current room is not %d", room);

	writeVar(0, 0);
	defineArray(0, kDwordArray, 0, 0, 0, _numLocalObjects);
	for (int i = 1; i < _numLocalObjects; i++) {
		cond = 1;
		tmp = num;
		while (--tmp >= 0) {
			cls = args[tmp];
			b = getClass(_objs[i].obj_nr, cls);
			if ((cls & 0x80 && !b) || (!(cls & 0x80) && b))
				cond = 0;
		}

		if (cond) {
			numObjs++;
			writeArray(0, 0, numObjs, _objs[i].obj_nr);
		}
	}

	writeArray(0, 0, 0, numObjs);

	push(readVar(0));
}

void ScummEngine_v90he::o90_getPolygonOverlap() {
	int args1[32];
	int args2[32];

	int n1 = getStackList(args1, ARRAYSIZE(args1));
	int n2 = getStackList(args2, ARRAYSIZE(args2));

	int subOp = pop();

	switch (subOp) {
	case OVERLAP_POINT_TO_RECT: // 1
		{
			Common::Rect r(args1[0], args1[1], args1[2] + 1, args1[3] + 1);
			Common::Point p(args2[0], args2[1]);
			push(r.contains(p) ? 1 : 0);
		}
		break;
	case OVERLAP_POINT_TO_CIRCLE: // 2
		{
			int dx = args2[0] - args1[0];
			int dy = args2[1] - args1[1];
			int dist = dx * dx + dy * dy;
			if (dist >= 2) {
				dist = (int)sqrt((double)(dist + 1));
			}
			if (_game.heversion >= 98) {
				push((dist <= args1[2]) ? 1 : 0);
			} else {
				push((dist > args1[2]) ? 1 : 0);
			}
		}
		break;
	case OVERLAP_RECT_TO_RECT: // 3
		{
			Common::Rect r1(args1[0], args1[1], args1[2] + 1, args1[3] + 1);
			Common::Rect r2(args2[0], args2[1], args2[2] + 1, args2[3] + 1);
			push(r2.intersects(r1) ? 1 : 0);
		}
		break;
	case OVERLAP_CIRCLE_TO_CIRCLE: // 4
		{
			int dx = args2[0] - args1[0];
			int dy = args2[1] - args1[1];
			int dist = dx * dx + dy * dy;
			if (dist >= 2) {
				dist = (int)sqrt((double)(dist + 1));
			}
			push((dist < args1[2] && dist < args2[2]) ? 1 : 0);
		}
		break;
	case OVERLAP_POINT_N_SIDED_POLYGON: // 5
		{
			assert((n1 & 1) == 0);
			n1 /= 2;
			if (n1 == 0) {
				push(0);
			} else {
				WizPolygon wp;
				wp.reset();
				wp.numVerts = n1;
				assert(n1 < ARRAYSIZE(wp.vert));
				for (int i = 0; i < n1; ++i) {
					wp.vert[i].x = args1[i * 2 + 0];
					wp.vert[i].y = args1[i * 2 + 1];
				}
				push(_wiz->polygonContains(wp, args2[0], args2[1]) ? 1 : 0);
			}
		}
		break;
	// HE 98+
	case OVERLAP_SPRITE_TO_SPRITE: // 6
		{
			Common::Rect r1, r2;
			_sprite->getSpriteBounds(args2[0], false, r2);
			_sprite->getSpriteBounds(args1[0], false, r1);
			if (r2.isValidRect() == false) {
				push(0);
				break;
			}

			if (n2 == 3) {
				r2.left += args2[1];
				r2.right += args2[1];
				r2.top += args2[2];
				r2.bottom += args2[2];
			}
			if (n1 == 3) {
				r1.left += args1[1];
				r1.right += args1[1];
				r1.top += args1[2];
				r1.bottom += args1[2];
			}
			push(r2.intersects(r1) ? 1 : 0);
		}
		break;
	case OVERLAP_SPRITE_TO_RECT: // 7
		{
			Common::Rect r2;
			_sprite->getSpriteBounds(args2[0], false, r2);
			Common::Rect r1(args1[0], args1[1], args1[2] + 1, args1[3] + 1);
			if (r2.isValidRect() == false) {
				push(0);
				break;
			}

			if (n2 == 3) {
				r2.left += args2[1];
				r2.right += args2[1];
				r2.top += args2[2];
				r2.bottom += args2[2];
			}
			push(r2.intersects(r1) ? 1 : 0);
		}
		break;
	case OVERLAP_DRAW_POS_SPRITE_TO_SPRITE: // 8
	case OVERLAP_SPRITE_TO_SPRITE_PIXEL_PERFECT: // 10
	// TODO: Draw sprites to buffer and compare.
		{
			Common::Rect r1, r2;
			_sprite->getSpriteBounds(args2[0], true, r2);
			_sprite->getSpriteBounds(args1[0], true, r1);
			if (r2.isValidRect() == false) {
				push(0);
				break;
			}

			if (n2 == 3) {
				r2.left += args2[1];
				r2.right += args2[1];
				r2.top += args2[2];
				r2.bottom += args2[2];
			}
			if (n1 == 3) {
				r1.left += args1[1];
				r1.right += args1[1];
				r1.top += args1[2];
				r1.bottom += args1[2];
			}
			push(r2.intersects(r1) ? 1 : 0);
		}
		break;
	case OVERLAP_DRAW_POS_SPRITE_TO_RECT: // 9
		{
			Common::Rect r2;
			_sprite->getSpriteBounds(args2[0], true, r2);
			Common::Rect r1(args1[0], args1[1], args1[2] + 1, args1[3] + 1);
			if (r2.isValidRect() == false) {
				push(0);
				break;
			}

			if (n2 == 3) {
				r2.left += args2[1];
				r2.right += args2[1];
				r2.top += args2[2];
				r2.bottom += args2[2];
			}
			push(r2.intersects(r1) ? 1 : 0);
		}
		break;
	default:
		error("o90_getPolygonOverlap: default case %d", subOp);
	}
}

void ScummEngine_v90he::o90_cond() {
	int a = pop();
	int b = pop();
	int c = pop();

	if (!c)
		b = a;
	push(b);
}

void ScummEngine_v90he::o90_dim2dim2Array() {
	int data, dim1start, dim1end, dim2start, dim2end;

	byte subOp = fetchScriptByte();

	switch (subOp) {
	case SO_BIT: // 2
		data = kBitArray;
		break;
	case SO_NIBBLE: // 3
		data = kNibbleArray;
		break;
	case SO_BYTE: // 4
		data = kByteArray;
		break;
	case SO_INT: // 5
		data = kIntArray;
		break;
	case SO_DWORD: // 6
		data = kDwordArray;
		break;
	case SO_STRING: // 7
		data = kStringArray;
		break;
	default:
		error("o90_dim2dim2Array: default case %d", subOp);
	}

	if (pop() == 2) {
		dim1end = pop();
		dim1start = pop();
		dim2end = pop();
		dim2start = pop();
	} else {
		dim2end = pop();
		dim2start = pop();
		dim1end = pop();
		dim1start = pop();
	}

	defineArray(fetchScriptWord(), data, dim2start, dim2end, dim1start, dim1end);
}

void ScummEngine_v90he::o90_redim2dimArray() {
	int a, b, c, d;
	d = pop();
	c = pop();
	b = pop();
	a = pop();

	byte subOp = fetchScriptByte();

	switch (subOp) {
	case SO_BYTE: // 4
		redimArray(fetchScriptWord(), a, b, c, d, kByteArray);
		break;
	case SO_INT: // 5
		redimArray(fetchScriptWord(), a, b, c, d, kIntArray);
		break;
	case SO_DWORD: // 6
		redimArray(fetchScriptWord(), a, b, c, d, kDwordArray);
		break;
	default:
		error("o90_redim2dimArray: default type %d", subOp);
	}
}

void ScummEngine_v90he::o90_getLinesIntersectionPoint() {
	int var_ix = fetchScriptWord();
	int var_iy = fetchScriptWord();
	int line2_y2 = pop();
	int line2_x2 = pop();
	int line2_y1 = pop();
	int line2_x1 = pop();
	int line1_y2 = pop();
	int line1_x2 = pop();
	int line1_y1 = pop();
	int line1_x1 = pop();

	int result = 0;
	int ix = 0;
	int iy = 0;

	bool isLine1Point = (line1_x1 == line1_x2 && line1_y1 == line1_y2);
	bool isLine2Point = (line2_x1 == line2_x2 && line2_y1 == line2_y2);

	if (isLine1Point) {
		if (isLine2Point) {
			if (line1_x1 == line2_x1 && line1_y1 == line2_y2) {
				ix = line1_x1;
				iy = line2_x1;
				result = 1;
			}
		} else {
			// 1 point and 1 line
			int dx2 = line2_x2 - line2_x1;
			if (dx2 != 0) {
				int dy2 = line2_y2 - line2_y1;
				float y = (float)dy2 / dx2 * (line1_x1 - line2_x1) + line2_y1 + .5f;
				if (line1_y1 == (int)y) {
					ix = line1_x1;
					iy = line1_y1;
					result = 1;
				}
			} else {
				// vertical line
				if (line1_x1 == line2_x1) {
					if (line2_y1 > line2_y2) {
						if (line1_y1 >= line2_y2 && line1_y1 <= line2_y1) {
							ix = line1_x1;
							iy = line1_y1;
							result = 1;
						}
					} else {
						if (line1_y1 >= line2_y1 && line1_y1 <= line2_y2) {
							ix = line1_x1;
							iy = line1_y1;
							result = 1;
						}
					}
				}
			}
		}
	} else {
		if (isLine2Point) {
			// 1 point and 1 line
			int dx1 = line1_x2 - line1_x1;
			if (dx1 != 0) {
				int dy1 = line1_y2 - line1_y1;
				float y = (float)dy1 / dx1 * (line2_x1 - line1_x1) + line1_y1 + .5f;
				if (line2_y1 == (int)y) {
					ix = line2_x1;
					iy = line2_y1;
					result = 1;
				}
			} else {
				// vertical line
				if (line2_x1 == line1_x1) {
					if (line1_y1 > line1_y2) {
						if (line2_y1 >= line1_y2 && line2_y1 <= line1_y1) {
							ix = line2_x1;
							iy = line2_y1;
							result = 1;
						}
					} else {
						if (line2_y1 >= line1_y1 && line2_y1 <= line1_y2) {
							ix = line2_x2;
							iy = line2_y1;
							result = 1;
						}
					}
				}
			}
		} else {
			// 2 lines
			int dy1 = line1_y2 - line1_y1;
			int dx1 = line1_x2 - line1_x1;
			int dy2 = line2_y2 - line2_y1;
			int dx2 = line2_x2 - line2_x1;
			int det = dx1 * dy2 - dx2 * dy1;
			int cross_p1 = dx1 * (line1_y1 - line2_y1) - dy1 * (line1_x1 - line2_x1);
			int cross_p2 = dx2 * (line1_y1 - line2_y1) - dy2 * (line1_x1 - line2_x1);
			if (det == 0) {
				// parallel lines
				if (cross_p2 == 0) {
					ix = ABS(line2_x2 + line2_x1) / 2;
					iy = ABS(line2_y2 + line2_y1) / 2;
					result = 2;
				}
			} else {
				float rcp1 = (float)cross_p1 / det;
				float rcp2 = (float)cross_p2 / det;
				if (rcp1 >= 0 && rcp1 <= 1 && rcp2 >= 0 && rcp2 <= 1) {
					ix = (int)(dx1 * rcp2 + line1_x1 + .5f);
					iy = (int)(dy1 * rcp2 + line1_y1 + .5f);
					result = 1;
				}
			}
		}
	}

	writeVar(var_ix, ix);
	writeVar(var_iy, iy);
	push(result);
}

void ScummEngine_v90he::getArrayDim(int array, int *dim2start, int *dim2end, int *dim1start, int *dim1end) {
	ArrayHeader *ah = (ArrayHeader *)getResourceAddress(rtString, readVar(array));
	assert(ah);
	if (dim2start && *dim2start == -1) {
		*dim2start = FROM_LE_32(ah->dim2start);
	}
	if (dim2end && *dim2end == -1) {
		*dim2end = FROM_LE_32(ah->dim2end);
	}
	if (dim1start && *dim1start == -1) {
		*dim1start = FROM_LE_32(ah->dim1start);
	}
	if (dim1end && *dim1end == -1) {
		*dim1end = FROM_LE_32(ah->dim1end);
	}
}

static int sortArrayOffset;

static int compareByteArray(const void *a, const void *b) {
	int va = *((const uint8 *)a + sortArrayOffset);
	int vb = *((const uint8 *)b + sortArrayOffset);
	return va - vb;
}

static int compareByteArrayReverse(const void *a, const void *b) {
	int va = *((const uint8 *)a + sortArrayOffset);
	int vb = *((const uint8 *)b + sortArrayOffset);
	return vb - va;
}

static int compareIntArray(const void *a, const void *b) {
	int va = (int16)READ_LE_UINT16((const uint8 *)a + sortArrayOffset * 2);
	int vb = (int16)READ_LE_UINT16((const uint8 *)b + sortArrayOffset * 2);
	return va - vb;
}

static int compareIntArrayReverse(const void *a, const void *b) {
	int va = (int16)READ_LE_UINT16((const uint8 *)a + sortArrayOffset * 2);
	int vb = (int16)READ_LE_UINT16((const uint8 *)b + sortArrayOffset * 2);
	return vb - va;
}

static int compareDwordArray(const void *a, const void *b) {
	int va = (int32)READ_LE_UINT32((const uint8 *)a + sortArrayOffset * 4);
	int vb = (int32)READ_LE_UINT32((const uint8 *)b + sortArrayOffset * 4);
	return va - vb;
}

static int compareDwordArrayReverse(const void *a, const void *b) {
	int va = (int32)READ_LE_UINT32((const uint8 *)a + sortArrayOffset * 4);
	int vb = (int32)READ_LE_UINT32((const uint8 *)b + sortArrayOffset * 4);
	return vb - va;
}


/**
 * Sort a row range in a two-dimensional array by the value in a given column.
 *
 * We sort the data in the row range [dim2start..dim2end], according to the value
 * in column dim1start == dim1end.
 */
void ScummEngine_v90he::sortArray(int array, int dim2start, int dim2end, int dim1start, int dim1end, int sortOrder) {
	debug(9, "sortArray(%d, [%d,%d,%d,%d], %d)", array, dim2start, dim2end, dim1start, dim1end, sortOrder);

	assert(dim1start == dim1end);
	checkArrayLimits(array, dim2start, dim2end, dim1start, dim1end);
	ArrayHeader *ah = (ArrayHeader *)getResourceAddress(rtString, readVar(array));
	assert(ah);

	const int num = dim2end - dim2start + 1;	// number of rows to sort
	const int pitch = FROM_LE_32(ah->dim1end) - FROM_LE_32(ah->dim1start) + 1;	// length of a row = number of columns in it
	const int offset = pitch * (dim2start - FROM_LE_32(ah->dim2start));	// memory offset to the first row to be sorted
	sortArrayOffset = dim1start - FROM_LE_32(ah->dim1start);	// offset to the column by which we sort

	// Now we just have to invoke qsort on the appropriate row range. We
	// need to pass sortArrayOffset as an implicit parameter to the
	// comparison functions, which makes it necessary to use a global
	// (albeit local to this file) variable.
	// This could be avoided by using qsort_r or a self-written portable
	// analog (this function passes an additional, user determined
	// parameter to the comparison function).
	// Another idea would be to use Common::sort, but that only is
	// suitable if you sort objects of fixed size, which must be known
	// during compilation time; clearly this not the case here.
	switch (FROM_LE_32(ah->type)) {
	case kByteArray:
	case kStringArray:
		if (sortOrder <= 0) {
			qsort(ah->data + offset, num, pitch, compareByteArray);
		} else {
			qsort(ah->data + offset, num, pitch, compareByteArrayReverse);
		}
		break;
	case kIntArray:
		if (sortOrder <= 0) {
			qsort(ah->data + offset * 2, num, pitch * 2, compareIntArray);
		} else {
			qsort(ah->data + offset * 2, num, pitch * 2, compareIntArrayReverse);
		}
		break;
	case kDwordArray:
		if (sortOrder <= 0) {
			qsort(ah->data + offset * 4, num, pitch * 4, compareDwordArray);
		} else {
			qsort(ah->data + offset * 4, num, pitch * 4, compareDwordArrayReverse);
		}
		break;
	default:
		error("Invalid array type %d", FROM_LE_32(ah->type));
	}
}

void ScummEngine_v90he::o90_sortArray() {
	byte subOp = fetchScriptByte();

	switch (subOp) {
	case SO_SORT:
	case ScummEngine_v100he::SO_SORT: // HE100
		{
			int array = fetchScriptWord();
			int sortOrder = pop();
			int dim1end = pop();
			int dim1start = pop();
			int dim2end = pop();
			int dim2start = pop();
			getArrayDim(array, &dim2start, &dim2end, &dim1start, &dim1end);
			sortArray(array, dim2start, dim2end, dim1start, dim1end, sortOrder);
		}
		break;
	default:
		error("o90_sortArray: Unknown case %d", subOp);
	}
}

void ScummEngine_v90he::o90_getObjectData() {
	byte subOp = fetchScriptByte();

	switch (subOp) {
	case SO_WIDTH:
		if (_heObjectNum == -1)
			push(0);
		else
			push(_objs[_heObjectNum].width);
		break;
	case SO_HEIGHT:
		if (_heObjectNum == -1)
			push(0);
		else
			push(_objs[_heObjectNum].height);
		break;
	case SO_COUNT:
		if (_heObjectNum == -1)
			push(0);
		else
			push(getObjectImageCount(_heObject));
		break;
	case SO_DRAW_XPOS:
		if (_heObjectNum == -1)
			push(0);
		else
			push(_objs[_heObjectNum].x_pos);
		break;
	case SO_DRAW_YPOS:
		if (_heObjectNum == -1)
			push(0);
		else
			push(_objs[_heObjectNum].y_pos);
		break;
	case SO_STATE:
		push(getState(_heObject));
		break;
	case SO_INIT:
		_heObject = pop();
		_heObjectNum = getObjectIndex(_heObject);
		break;
	case SO_NEW_GENERAL_PROPERTY:
		// Dummy case, as in the original
		pop();
		push(0);
		break;
	default:
		error("o90_getObjectData: Unknown case %d", subOp);
	}
}

void ScummEngine_v90he::o90_getPaletteData() {
	int c, d, e;
	int r, g, b;
	int palSlot, color;

	byte subOp = fetchScriptByte();

	switch (subOp) {
	case SO_FIND:
		e = pop();
		d = pop();
		palSlot = pop();
		b = pop();
		g = pop();
		r = pop();
		push(getHEPaletteSimilarColor(palSlot, r, g, d, e));
		break;
	case SO_STATE:
		c = pop();
		b = pop();
		palSlot = pop();
		push(getHEPaletteColorComponent(palSlot, b, c));
		break;
	case SO_COLOR:
		color = pop();
		palSlot = pop();
		push(getHEPaletteColor(palSlot, color));
		break;
	case SO_CHANNEL:
		c = pop();
		b = pop();
		if (_game.features & GF_16BIT_COLOR)
			push(getHEPalette16BitColorComponent(b, c));
		else
			push(getHEPaletteColorComponent(1, b, c));
		break;
	case SO_NEW:
		b = pop();
		b = MAX(0, b);
		b = MIN(b, 255);
		g = pop();
		g = MAX(0, g);
		g = MIN(g, 255);
		r = pop();
		r = MAX(0, r);
		r = MIN(r, 255);

		if (_game.features & GF_16BIT_COLOR) {
			push(get16BitColor(r, g, b));
		} else {
			push(getHEPaletteSimilarColor(1, r, g, 10, 245));
		}
		break;
	default:
		error("o90_getPaletteData: Unknown case %d", subOp);
	}
}

void ScummEngine_v90he::o90_paletteOps() {
	int a, b, c, d, e;

	byte subOp = fetchScriptByte();

	switch (subOp) {
	case SO_INIT: // 57
		_hePaletteNum = pop();
		break;
	case SO_IMAGE: // 63
		b = pop();
		a = pop();
		if (_hePaletteNum != 0) {
			setHEPaletteFromImage(_hePaletteNum, a, b);
		}
		break;
	case SO_COLOR: // 66
		e = pop();
		d = pop();
		c = pop();
		b = pop();
		a = pop();
		if (_hePaletteNum != 0) {
			for (; a <= b; ++a) {
				setHEPaletteColor(_hePaletteNum, a, c, d, e);
			}
		}
		break;
	case SO_TO: // 70
		c = pop();
		b = pop();
		a = pop();
		if (_hePaletteNum != 0) {
			for (; a <= b; ++a) {
				copyHEPaletteColor(_hePaletteNum, a, c);
			}
		}
		break;
	case SO_COSTUME: // 76, HE99+
		a = pop();
		if (_hePaletteNum != 0) {
			setHEPaletteFromCostume(_hePaletteNum, a);
		}
		break;
	case SO_PALETTE: // 86
		a = pop();
		if (_hePaletteNum != 0) {
			copyHEPalette(_hePaletteNum, a);
		}
		break;
	case SO_ROOM_PALETTE: // 175
		b = pop();
		a = pop();
		if (_hePaletteNum != 0) {
			setHEPaletteFromRoom(_hePaletteNum, a, b);
		}
		break;
	case SO_NEW: // 217
		if (_hePaletteNum != 0) {
			restoreHEPalette(_hePaletteNum);
		}
		break;
	case SO_END: // 255
		_hePaletteNum = 0;
		break;
	default:
		error("o90_paletteOps: Unknown case %d", subOp);
	}
}

void ScummEngine_v90he::o90_fontEnum() {
	byte string[80];

	byte subOp = fetchScriptByte();

	switch (subOp) {
	case ScummEngine_v100he::SO_INIT: // HE100
	case SO_INIT:
		push(1);
		break;
	case ScummEngine_v100he::SO_PROPERTY:	// HE100
	case SO_PROPERTY:
		switch (pop()) {
		case 1: // FONT_ENUM_GET
			pop();
			writeVar(0, 0);
			defineArray(0, kStringArray, 0, 0, 0, 0);
			writeArray(0, 0, 0, 0);
			push(readVar(0));
			break;
		case 2: // FONT_ENUM_FIND
			copyScriptString(string, sizeof(string));
			push(-1);
			break;
		}

		break;
	default:
		error("o90_fontEnum: Unknown case %d", subOp);
	}

	debug(1, "o90_fontEnum stub (%d)", subOp);
}

void ScummEngine_v90he::o90_getActorAnimProgress() {
	Actor *a = derefActor(pop(), "o90_getActorAnimProgress");
	push(a->getAnimProgress());
}

void ScummEngine_v90he::o90_kernelGetFunctions() {
	int args[29];
	int num, tmp;
	Actor *a;

	num = getStackList(args, ARRAYSIZE(args));

	switch (args[0]) {
	case 1001:
		{
		double b = args[1] * M_PI / 180.;
		push((int)(sin(b) * 100000));
		}
		break;
	case 1002:
		{
		double b = args[1] * M_PI / 180.;
		push((int)(cos(b) * 100000));
		}
		break;
	case 1969:
		a = derefActor(args[1], "o90_kernelGetFunctions: 1969");
		tmp = a->_heCondMask;
		tmp &= 0x7FFF0000;
		push(tmp);
		break;
	case 2001:
		push(_logicHE->dispatch(args[1], num - 2, (int32 *)&args[2]));
		break;
	default:
		error("o90_kernelGetFunctions: default case %d", args[0]);
	}
}

void ScummEngine_v90he::o90_kernelSetFunctions() {
	int args[29];
	int num, tmp;
	ActorHE *a;

	num = getStackList(args, ARRAYSIZE(args));

	switch (args[0]) {
	case 20:
		a = (ActorHE *)derefActor(args[1], "o90_kernelSetFunctions: 20");
		queueAuxBlock(a); // TODO: Shouldn't this be HEQueueEraseAuxActor?
		break;
	case 21:
		_skipDrawObject = 1;
		break;
	case 22:
		_skipDrawObject = 0;
		break;
	case 23:
		clearCharsetMask();
		_fullRedraw = true;
		break;
	case 24:
		_skipProcessActors = 1;
		redrawAllActors();
		break;
	case 25:
		_skipProcessActors = 0;
		redrawAllActors();
		break;
	case 27:
		// Used in readdemo
		break;
	case 42:
		_wiz->_rectOverrideEnabled = true;
		_wiz->_rectOverride.left = args[1];
		_wiz->_rectOverride.top = args[2];
		_wiz->_rectOverride.right = args[3];
		_wiz->_rectOverride.bottom = args[4];
		adjustRect(_wiz->_rectOverride);
		break;
	case 43:
		_wiz->_rectOverrideEnabled = false;
		break;
	case 714:
		setResourceOffHeap(args[1], args[2], args[3]);
		break;
	case 1492:
		// Remote start script function
		break;
	case 1969:
		a = (ActorHE *)derefActor(args[1], "o90_kernelSetFunctions: 1969");
		tmp = a->_heCondMask;
		tmp ^= args[2];
		tmp &= 0x7FFF0000;
		a->_heCondMask ^= tmp;
		break;
	case 2001:
		_logicHE->dispatch(args[1], num - 2, (int32 *)&args[2]);
		break;
	case 201102: // Used in puttzoo iOS
	case 20111014: // Used in spyfox iOS
		break;
	default:
		error("o90_kernelSetFunctions: default case %d (param count %d)", args[0], num);
	}
}

} // End of namespace Scumm

#endif // ENABLE_HE
