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

#ifndef NANCY_ACTION_ACTIONMANAGER_H
#define NANCY_ACTION_ACTIONMANAGER_H

#include "engines/nancy/action/actionrecord.h"

#include "common/str.h"
#include "common/stream.h"
#include "common/array.h"
#include "common/func.h"

namespace Common {
class Serializer;
}

namespace Nancy {

class NancyEngine;
struct NancyInput;

namespace State {
class Scene;
}

namespace Action {

// The class that handles ActionRecords and their execution
class ActionManager {
	friend class Nancy::State::Scene;

public:
	ActionManager() {}
	virtual ~ActionManager() {}

	void handleInput(NancyInput &input);

	void processActionRecords();
	bool addNewActionRecord(Common::SeekableReadStream &inputData);
	Common::Array<ActionRecord *> &getActionRecords() { return _records; }
	ActionRecord *getActionRecord(uint id) { if (id < _records.size()) return _records[id]; else return nullptr;}
	void clearActionRecords();

	void onPause(bool pause);

	void synchronize(Common::Serializer &serializer);

protected:
	virtual ActionRecord *createActionRecord(uint16 type);

	Common::Array<ActionRecord *> _records;
};

} // End of namespace Action
} // End of namespace Nancy

#endif // NANCY_ACTION_ACTIONMANAGER_H
