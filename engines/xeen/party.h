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

#ifndef XEEN_PARTY_H
#define XEEN_PARTY_H

#include "common/scummsys.h"
#include "common/array.h"
#include "common/rect.h"
#include "common/serializer.h"
#include "xeen/items.h"

namespace Xeen {

enum Direction { 
	DIR_NORTH = 0, DIR_EAST = 1, DIR_SOUTH = 2, DIR_WEST = 3, DIR_ALL = 4
};

enum Difficulty { ADVENTURER = 0, WARRIOR = 1 };

enum Sex { MALE = 0, FEMALE = 1, YES_PLEASE = 2 };

enum Race { HUMAN = 0, ELF = 1, DWARF = 2, GNOME = 3, HALF_ORC = 4 };

enum CharacterClass {
	CLASS_KNIGHT = 0, CLASS_PALADIN = 1, CLASS_ARCHER = 2, CLASS_CLERIC = 3,
	CLASS_SORCERER = 4, CLASS_ROBBER = 5, CLASS_NINJA = 6, CLASS_BARBARIAN = 7, 
	CLASS_DRUID = 8, CLASS_RANGER = 9, 
	CLASS_12 = 12, CLASS_15 = 15, CLASS_16 = 16
};

enum Skill { THIEVERY = 0, ARMS_MASTER = 1, ASTROLOGER = 2, BODYBUILDER = 3,
	CARTOGRAPHER = 4, CRUSADER = 5, DIRECTION_SENSE = 6, LINGUIST = 7, 
	MERCHANT = 8, MOUNTAINEER = 9, NAVIGATOR = 10, PATHFINDER = 11,
	PRAYER_MASTER = 12, PRESTIDIGITATION = 13, SWIMMING = 14, TRACKING = 15,
	SPOT_DOORS = 16, DANGER_SENSE = 17
};

enum Condition { CURSED = 0, HEART_BROKEN = 1, WEAK = 2, POISONED = 3,
	DISEASED = 4, INSANE = 5, IN_LOVE = 6, DRUNK = 7, SLEEP = 8, 
	DEPRESSED = 9, CONFUSED = 10, PARALYZED = 11, UNCONSCIOUS = 12,
	DEAD = 13, STONED = 14, ERADICATED = 15,
	NO_CONDITION = 16
};

#define ITEMS_COUNT 36
#define TOTAL_CHARACTERS 30
#define XEEN_TOTAL_CHARACTERS 24
#define MAX_ACTIVE_PARTY 6

class XeenEngine;

class AttributePair {
public:	
	int _permanent;
	int _temporary;
public:
	AttributePair();
	void synchronize(Common::Serializer &s);
};

class PlayerStruct {
public:
	Common::String _name;
	Sex _sex;
	Race _race;
	int _xeenSide;
	CharacterClass _class;
	AttributePair _might;
	AttributePair _intellect;
	AttributePair _personality;
	AttributePair _endurance;
	AttributePair _speed;
	AttributePair _accuracy;
	AttributePair _luck;
	int _ACTemp;
	AttributePair _level;
	int _dbDay;
	int _tempAge;
	int _skills[18];
	bool _awards[512];
	bool _spells[312];
	int _lloydMap;
	Common::Point _lloydPosition;
	bool _hasSpells;
	int _currentSpell;
	int _quickOption;
	XeenItem _weapons[9];
	XeenItem _armor[9];
	XeenItem _accessories[9];
	XeenItem _misc[9];
	int _lloydSide;
	AttributePair _fireResistence;
	AttributePair _coldResistence;
	AttributePair _electricityResistence;
	AttributePair _poisonResistence;
	AttributePair _energyResistence;
	AttributePair _magicResistence;
	int _conditions[16];
	int _townUnknown;
	int _unknown2;
	int _currentHp;
	int _currentSp;
	int _ybDay;
	uint32 _experience;
	int _currentAdventuringSpell;
	int _currentCombatSpell;
public:
	PlayerStruct();
	void synchronize(Common::Serializer &s);

	Condition worstCondition() const;

	int getAge(int partyYear, bool ignoreTemp);

	int getMaxHp();
};

class Roster: public Common::Array<PlayerStruct> {
public:
	Roster() {}

	void synchronize(Common::Serializer &s);
};

class Party {
public:
	// Dynamic data that's saved
	int _partyCount;
	int _realPartyCount;
	int _partyMembers[8];
	Direction _mazeDirection;
	Common::Point _mazePosition;
	int _mazeId;
	int _priorMazeId;
	bool _levitateActive;
	bool _automapOn;
	bool _wizardEyeActive;
	bool _clairvoyanceActive;
	bool _walkOnWaterActive;
	bool _blessedActive;
	bool _powerShieldActive;
	bool _holyBonusActive;
	bool _heroismActive;
	Difficulty _difficulty;
	XeenItem _blacksmithWeapons[ITEMS_COUNT];
	XeenItem _blacksmithArmor[ITEMS_COUNT];
	XeenItem _blacksmithAccessories[ITEMS_COUNT];
	XeenItem _blacksmithMisc[ITEMS_COUNT];
	bool _cloudsEnd;
	bool _darkSideEnd;
	bool _worldEnd;
	int hour_maybe;
	int _day;
	int _year;
	int _minutes;
	int _food;
	int _lightCount;
	int _torchCount;
	int _fireResistence;
	int _electricityResistence;
	int _coldResistence;
	int _poisonResistence;
	int _deathCount;
	int _winCount;
	int _lossCount;
	int _gold;
	int _gems;
	int _bankGold;
	int _bankGems;
	int _totalTime;
	bool _rested;
	bool _gameFlags[512];
	bool _autoNotes[128];
	bool _quests[64];
	int _questItems[85];
	XeenItem _blacksmithWeapons2[ITEMS_COUNT];
	XeenItem _blacksmithArmor2[ITEMS_COUNT];
	XeenItem _blacksmithAccessories2[ITEMS_COUNT];
	XeenItem _blacksmithMisc2[ITEMS_COUNT];
	bool _characterFlags[30][24];
public:
	// Other party related runtime data
	Common::Array<PlayerStruct> _activeParty;
	int _combatPartyCount;
public:
	Party();

	void synchronize(Common::Serializer &s);

	bool checkSkill(Skill skillId);

	bool isInParty(int charId);

	void copyPartyToRoster(Roster &r);
};

} // End of namespace Xeen

#endif	/* XEEN_PARTY_H */
