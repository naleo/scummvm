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

/*
* Based on the Reverse Engineering work of Christophe Fontanel,
* maintainer of the Dungeon Master Encyclopaedia (http://dmweb.free.fr/)
*/

#include "dm/dm.h"

#include "common/config-manager.h"
#include "common/error.h"
#include "common/fs.h"

#include "engines/advancedDetector.h"
namespace DM {
static const PlainGameDescriptor DMGames[] = {
	{"dm", "Dungeon Master"},
	{0, 0}
};

static const ADGameDescription gameDescriptions[] = {
	{
		"dm", "Amiga 2.0v English",
		{
			{"graphics.dat", 0, "c2205f6225bde728417de29394f97d55", 411960},
			{"Dungeon.dat", 0, "43a213da8eda413541dd12f90ce202f6", 25006},
		AD_LISTEND
		},
	    Common::EN_GRB, Common::kPlatformAmiga, ADGF_NO_FLAGS, GUIO1(GUIO_NONE)
	},

	AD_TABLE_END_MARKER
};

static ADGameDescription fallbackDesc = {
	"dm",
	"Unknown version",
	AD_ENTRY1(0, 0), // This should always be AD_ENTRY1(0, 0) in the fallback descriptor
	Common::UNK_LANG,
	Common::kPlatformDOS,
	ADGF_NO_FLAGS,
	GUIO1(GUIO_NONE)
};


static const ADExtraGuiOptionsMap optionsList[] = {
	AD_EXTRA_GUI_OPTIONS_TERMINATOR
};


class DMMetaEngine : public AdvancedMetaEngine {
public:
	DMMetaEngine() : AdvancedMetaEngine(DM::gameDescriptions, sizeof(ADGameDescription), DMGames, optionsList) {
		_singleId = "dm";
	}

	virtual const char *getName() const {
		return "Dungeon Master";
	}

	virtual const char *getOriginalCopyright() const {
		return "Dummy";
	}

	virtual const ADGameDescription *fallbackDetect(const FileMap &allFiles, const Common::FSList &fslist) const { return gameDescriptions; }

	virtual bool createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) const {
		if(desc)
			*engine = new DM::DMEngine(syst);
		return desc != nullptr;
	}
	virtual int getMaximumSaveSlot() const { return 99; }
	virtual SaveStateList listSaves(const char *target) const { return SaveStateList(); }
	SaveStateDescriptor querySaveMetaInfos(const char *target, int slot) const { return SaveStateDescriptor(); }
	virtual void removeSaveState(const char *target, int slot) const {}
};

}
#if PLUGIN_ENABLED_DYNAMIC(DM)
REGISTER_PLUGIN_DYNAMIC(DM, PLUGIN_TYPE_ENGINE, DM::DMMetaEngine);
#else
REGISTER_PLUGIN_STATIC(DM, PLUGIN_TYPE_ENGINE, DM::DMMetaEngine);
#endif
