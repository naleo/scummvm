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
 */

/*
 * This file is based on WME Lite.
 * http://dead-code.org/redir.php?target=wmelite
 * Copyright (c) 2011 Jan Nedoma
 */

#include "engines/wintermute/tinyxml/tinyxml.h"
#include "engines/wintermute/base/base_game.h"
#include "engines/wintermute/base/base_registry.h"
#include "engines/wintermute/utils/path_util.h"
#include "engines/wintermute/utils/string_util.h"
#include "engines/wintermute/utils/utils.h"
#include "engines/wintermute/wintermute.h"
#include "common/savefile.h"
#include "common/config-manager.h"
#include "common/file.h"

namespace WinterMute {

//////////////////////////////////////////////////////////////////////////
BaseRegistry::BaseRegistry(BaseGame *inGame) : BaseClass(inGame) {
	_iniName = NULL;

	setIniName("./wme.ini");
	loadValues(true);
}


//////////////////////////////////////////////////////////////////////////
BaseRegistry::~BaseRegistry() {
	saveValues();
	delete[] _iniName;
	_iniName = NULL;
}



//////////////////////////////////////////////////////////////////////////
AnsiString BaseRegistry::readString(const AnsiString &subKey, const AnsiString &key, const AnsiString &init) {
	AnsiString ret = "";

	bool found = false;
	ret = getValue(_localValues, subKey, key, found);
	if (!found) {
		ret = getValue(_values, subKey, key, found);
	}
	if (!found) {
		ret = init;
	}

	return ret;
}


//////////////////////////////////////////////////////////////////////////
bool BaseRegistry::writeString(const AnsiString &subKey, const AnsiString &key, const AnsiString &value) {
	_values[subKey][key] = value;
	return true;
}


//////////////////////////////////////////////////////////////////////////
int BaseRegistry::readInt(const AnsiString &subKey, const AnsiString &key, int init) {
	if (subKey == "Audio") {
		if (key == "MasterVolume") {
			if (ConfMan.hasKey("master_volume")) {
				return ConfMan.getInt("master_volume");
			} else {
				return init;
			}
		} else if (key == "SFXVolume") {
			if (ConfMan.hasKey("sfx_volume")) {
				error("This key shouldn't be read by the scripts");
			} else {
				return init;
			}
		} else if (key == "SpeechVolume") {
			if (ConfMan.hasKey("speech_volume")) {
				error("This key shouldn't be read by the scripts");
			} else {
				return init;
			}
		} else if (key == "MusicVolume") {
			if (ConfMan.hasKey("music_volume")) {
				error("This key shouldn't be read by the scripts");
			} else {
				return init;
			}
		}
	}
	AnsiString val = readString(subKey, key, "");
	if (val.empty()) {
		return init;
	} else {
		return atoi(val.c_str());
	}
}


//////////////////////////////////////////////////////////////////////////
bool BaseRegistry::writeInt(const AnsiString &subKey, const AnsiString &key, int value) {
	if (subKey == "Audio") {
		if (key == "MasterVolume") {
			ConfMan.setInt("master_volume", value);
			return true;
		} else if (key == "SFXVolume") {
			error("This key shouldn't be read by the scripts");
			return true;
		} else if (key == "SpeechVolume") {
			error("This key shouldn't be read by the scripts");
			return true;
		} else if (key == "MusicVolume") {
			error("This key shouldn't be read by the scripts");
			return true;
		}
	}
	writeString(subKey, key, StringUtil::toString(value));
	return true;
}


//////////////////////////////////////////////////////////////////////////
bool BaseRegistry::readBool(const AnsiString &subKey, const AnsiString &key, bool init) {
	return (readInt(subKey, key, (int)init) != 0);
}


//////////////////////////////////////////////////////////////////////////
bool BaseRegistry::writeBool(const AnsiString &subKey, const AnsiString &key, bool value) {
	return writeInt(subKey, key, (int)value);
}


//////////////////////////////////////////////////////////////////////////
void BaseRegistry::setIniName(const char *name) {
	delete[] _iniName;
	_iniName = NULL;

	if (strchr(name, '\\') == NULL && strchr(name, '/') == NULL) {
		_iniName = new char [strlen(name) + 3];
		sprintf(_iniName, "./%s", name);
	} else {
		_iniName = new char [strlen(name) + 1];
		strcpy(_iniName, name);
	}
}


//////////////////////////////////////////////////////////////////////////
char *BaseRegistry::getIniName() {
	return _iniName;
}

//////////////////////////////////////////////////////////////////////////
void BaseRegistry::loadValues(bool local) {
	Common::String filename = Common::String(_gameRef->getGameId()) + "-settings.xml";
	loadXml(filename, _values);
}

//////////////////////////////////////////////////////////////////////////
void BaseRegistry::saveValues() {
	Common::String filename = Common::String(_gameRef->getGameId()) + "-settings.xml";
	saveXml(filename, _values);
}

//////////////////////////////////////////////////////////////////////////
void BaseRegistry::setBasePath(const char *basePath) {
	_basePath = PathUtil::getFileNameWithoutExtension(basePath);

	loadValues(false);
}

//////////////////////////////////////////////////////////////////////////
AnsiString BaseRegistry::getValue(PathValueMap &values, const AnsiString path, const AnsiString &key, bool &found) {
	found = false;
	PathValueMap::iterator it = values.find(path);
	if (it == values.end()) {
		return "";
	}

	KeyValuePair pairs = (*it)._value;
	KeyValuePair::iterator keyIt = pairs.find(key);
	if (keyIt == pairs.end()) {
		return "";
	} else {
		found = true;
		return (*keyIt)._value;
	}
}

//////////////////////////////////////////////////////////////////////////
void BaseRegistry::loadXml(const AnsiString fileName, PathValueMap &values) {
	Common::SeekableReadStream *stream = g_wintermute->getSaveFileMan()->openForLoading(fileName);
	if (!stream) {
		return;
	}
	char *data = new char[stream->size()];
	stream->read(data, stream->size());
	TiXmlDocument doc;
	doc.Parse(data);
	delete[] data;

	TiXmlElement *rootElem = doc.RootElement();
	if (!rootElem || Common::String(rootElem->Value()) != "Settings") { // TODO: Avoid this strcmp-use. (Hack for now, since we might drop TinyXML all together)
		return;
	}

	for (TiXmlElement *pathElem = rootElem->FirstChildElement(); pathElem != NULL; pathElem = pathElem->NextSiblingElement()) {
		for (TiXmlElement *keyElem = pathElem->FirstChildElement(); keyElem != NULL; keyElem = keyElem->NextSiblingElement()) {
			values[Common::String(pathElem->Value())][Common::String(keyElem->Value())] = keyElem->GetText();
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void BaseRegistry::saveXml(const AnsiString fileName, PathValueMap &values) {
	BaseUtils::createPath(fileName.c_str());

	TiXmlDocument doc;
	doc.LinkEndChild(new TiXmlDeclaration("1.0", "utf-8", ""));

	TiXmlElement *root = new TiXmlElement("Settings");
	doc.LinkEndChild(root);

	PathValueMap::iterator pathIt;
	for (pathIt = _values.begin(); pathIt != _values.end(); ++pathIt) {
		TiXmlElement *pathElem = new TiXmlElement((*pathIt)._key.c_str());
		root->LinkEndChild(pathElem);


		KeyValuePair pairs = (*pathIt)._value;
		KeyValuePair::iterator keyIt;
		for (keyIt = pairs.begin(); keyIt != pairs.end(); ++keyIt) {
			TiXmlElement *keyElem = new TiXmlElement((*keyIt)._key.c_str());
			pathElem->LinkEndChild(keyElem);

			keyElem->LinkEndChild(new TiXmlText((*keyIt)._value.c_str()));
		}
	}


	TiXmlPrinter printer;
	doc.Accept(&printer);

	Common::WriteStream *stream = g_wintermute->getSaveFileMan()->openForSaving(fileName);

	if (!stream) {
		return;
	} else {
		stream->write(printer.CStr(), printer.Size());
		stream->finalize();
		delete stream;
	}
}

} // end of namespace WinterMute
