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

#ifndef NANCY_UI_SCROLLBAR_H
#define NANCY_UI_SCROLLBAR_H

#include "engines/nancy/renderobject.h"

#include "common/rect.h"

namespace Nancy {

struct NancyInput;

namespace UI {

class Scrollbar : public RenderObject {
public:
	Scrollbar(RenderObject &redrawFrom) :
		RenderObject(redrawFrom),
		_isClicked(false),
		_currentPosition(0),
		_maxDist(0) {}
	virtual ~Scrollbar() = default;

	virtual void init() override;

	void handleInput(NancyInput &input);

	void resetPosition();
	float getPos() const { return _currentPosition; }

protected:
	virtual uint16 getZOrder() const override { return 9; }

	void calculatePosition();

	Common::Point _startPosition;
	uint _maxDist;

	float _currentPosition;

	bool _isClicked;
	Common::Point _mousePosOnClick;
};

} // End of namespace UI
} // End of namespace Nancy

#endif // NANCY_UI_SCROLLBAR_H
