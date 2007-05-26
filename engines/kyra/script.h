/* ScummVM - Scumm Interpreter
 * Copyright (C) 2004-2006 The ScummVM project
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

#ifndef KYRA_SCRIPT_H
#define KYRA_SCRIPT_H

#include "kyra/kyra.h"

namespace Kyra {

struct ScriptState;

struct Opcode {
	virtual ~Opcode() {}

	virtual operator bool() const = 0;

	virtual int operator()(ScriptState*) const = 0;
};

template<class T>
struct OpcodeImpl : public Opcode {
	T *vm;
	typedef int (T::*Callback)(ScriptState*);
	Callback callback;

	OpcodeImpl(T *v, Callback c) : Opcode(), vm(v), callback(c) {} 

	operator bool() const { return callback != 0; }

	int operator()(ScriptState *state) const {
		return (vm->*callback)(state);
	}
};

struct ScriptData {
	byte *text;
	uint16 *data;
	uint16 *ordr;
	uint16 dataSize;

	const Common::Array<const Opcode*> *opcodes;
};

struct ScriptState {
	uint16 *ip;
	const ScriptData *dataPtr;
	int16 retValue;
	uint16 bp;
	uint16 sp;
	int16 variables[30];
	int16 stack[61];
};

class ScriptHelper {
public:
	ScriptHelper(KyraEngine *vm);
	
	bool loadScript(const char *filename, ScriptData *data, const Common::Array<const Opcode*> *opcodes);
	void unloadScript(ScriptData *data);
	
	void initScript(ScriptState *scriptState, const ScriptData *data);
	bool startScript(ScriptState *script, int function);
	
	bool validScript(ScriptState *script);
	
	bool runScript(ScriptState *script);
protected:
	uint32 getFORMBlockSize(const byte *&data) const;
	uint32 getIFFBlockSize(const byte *start, const byte *&data, uint32 maxSize, const uint32 chunk) const;
	bool loadIFFBlock(const byte *start, const byte *&data, uint32 maxSize, const uint32 chunk, void *loadTo, uint32 ptrSize) const;
	
	KyraEngine *_vm;
	int16 _parameter;
	bool _continue;

	typedef void (ScriptHelper::*CommandProc)(ScriptState*);
	struct CommandEntry {
		CommandProc proc;
		const char* desc;
	};
	
	const CommandEntry *_commands;
private:
	void c1_jmpTo(ScriptState*);
	void c1_setRetValue(ScriptState*);
	void c1_pushRetOrPos(ScriptState*);
	void c1_push(ScriptState*);
	//void c1_push(); same as 03
	void c1_pushVar(ScriptState*);
	void c1_pushBPNeg(ScriptState*);
	void c1_pushBPAdd(ScriptState*);
	void c1_popRetOrPos(ScriptState*);
	void c1_popVar(ScriptState*);
	void c1_popBPNeg(ScriptState*);
	void c1_popBPAdd(ScriptState*);
	void c1_addSP(ScriptState*);
	void c1_subSP(ScriptState*);
	void c1_execOpcode(ScriptState*);
	void c1_ifNotJmp(ScriptState*);
	void c1_negate(ScriptState*);
	void c1_eval(ScriptState*);
	void c1_setRetAndJmp(ScriptState*);
};
} // end of namespace Kyra

#endif

