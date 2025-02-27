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

#include "engines/nancy/action/actionrecord.h"
#include "engines/nancy/action/actionmanager.h"

#include "engines/nancy/state/scene.h"

#include "engines/nancy/nancy.h"
#include "engines/nancy/nancy.h"
#include "engines/nancy/graphics.h"
#include "engines/nancy/sound.h"
#include "engines/nancy/input.h"
#include "engines/nancy/resource.h"
#include "engines/nancy/util.h"

#include "engines/nancy/action/responses.cpp"

#include "common/str.h"

namespace Nancy {
namespace Action {

void SceneChange::readData(Common::SeekableReadStream &stream) {
	_sceneChange.readData(stream);
}

void SceneChange::execute() {
	NancySceneState.changeScene(_sceneChange);
	_isDone = true;
}

void HotMultiframeSceneChange::readData(Common::SeekableReadStream &stream) {
	SceneChange::readData(stream);
	uint16 numHotspots = stream.readUint16LE();

	_hotspots.reserve(numHotspots);
	for (uint i = 0; i < numHotspots; ++i) {
		_hotspots.push_back(HotspotDescription());
		HotspotDescription &newDesc = _hotspots[i];
		newDesc.readData(stream);
	}
}

void HotMultiframeSceneChange::execute() {
	switch (_state) {
	case kBegin:
		// turn main rendering on
		_state = kRun;
		// fall through
	case kRun:
		_hasHotspot = false;
		for (uint i = 0; i < _hotspots.size(); ++i) {
			if (_hotspots[i].frameID == NancySceneState.getSceneInfo().frameID) {
				_hasHotspot = true;
				_hotspot = _hotspots[i].coords;
			}
		}
		break;
	case kActionTrigger:
		SceneChange::execute();
		break;
	}
}

void Hot1FrSceneChange::readData(Common::SeekableReadStream &stream) {
	SceneChange::readData(stream);
	_hotspotDesc.readData(stream);
}

void Hot1FrSceneChange::execute() {
	switch (_state) {
	case kBegin:
		_hotspot = _hotspotDesc.coords;
		_state = kRun;
		// fall through
	case kRun:
		if (_hotspotDesc.frameID == NancySceneState.getSceneInfo().frameID) {
			_hasHotspot = true;
		} else {
			_hasHotspot = false;
		}
		break;
	case kActionTrigger:
		SceneChange::execute();
		break;
	}
}

void HotMultiframeMultisceneChange::readData(Common::SeekableReadStream &stream) {
	stream.seek(0x14, SEEK_CUR);
	uint size = stream.readUint16LE() * 0x12;
	stream.skip(size);
}

void StartFrameNextScene::readData(Common::SeekableReadStream &stream) {
	stream.skip(4);
}

void StartStopPlayerScrolling::readData(Common::SeekableReadStream &stream) {
	stream.skip(1);
}

void MapCall::readData(Common::SeekableReadStream &stream) {
	stream.skip(1);
}

void MapCall::execute() {
	_execType = kRepeating;
	NancySceneState.requestStateChange(NancyEngine::kMap);
	finishExecution();
}

void MapCallHot1Fr::readData(Common::SeekableReadStream &stream) {
	stream.skip(0x12);
}

void MapCallHot1Fr::execute() {
	switch (_state) {
	case kBegin:
		_hotspot = _hotspotDesc.coords;
		_state = kRun;
		// fall through
	case kRun:
		if (_hotspotDesc.frameID == NancySceneState.getSceneInfo().frameID) {
			_hasHotspot = true;
		}
		break;
	case kActionTrigger:
		MapCall::execute();
		break;
	}
}

void MapCallHotMultiframe::readData(Common::SeekableReadStream &stream) {
	uint16 numDescs = stream.readUint16LE();
	_hotspots.reserve(numDescs);
	for (uint i = 0; i < numDescs; ++i) {
		_hotspots.push_back(HotspotDescription());
		_hotspots[i].readData(stream);
	}
}

void MapCallHotMultiframe::execute() {
	switch (_state) {
	case kBegin:
		_state = kRun;
		// fall through
	case kRun:
		_hasHotspot = false;
		for (uint i = 0; i < _hotspots.size(); ++i) {
			if (_hotspots[i].frameID == NancySceneState.getSceneInfo().frameID) {
				_hasHotspot = true;
				_hotspot = _hotspots[i].coords;
			}
		}
		break;
	case kActionTrigger:
		MapCall::execute();
		break;
	}
}

void MapLocationAccess::readData(Common::SeekableReadStream &stream) {
	stream.skip(4);
}

void MapSound::readData(Common::SeekableReadStream &stream) {
	stream.skip(0x10);
}

void MapAviOverride::readData(Common::SeekableReadStream &stream) {
	stream.skip(2);
}

void MapAviOverrideOff::readData(Common::SeekableReadStream &stream) {
	stream.skip(1);
}

void TextBoxWrite::readData(Common::SeekableReadStream &stream) {
	uint16 size = stream.readUint16LE();
	stream.skip(size);

	if (size > 10000) {
		error("Action Record atTextboxWrite has too many text box chars: %d", size);
	}
}

void TextBoxClear::readData(Common::SeekableReadStream &stream) {
	stream.skip(1);
}

void BumpPlayerClock::readData(Common::SeekableReadStream &stream) {
	stream.skip(5);
}

void SaveContinueGame::readData(Common::SeekableReadStream &stream) {
	stream.skip(1);
}

void TurnOffMainRendering::readData(Common::SeekableReadStream &stream) {
	stream.skip(1);
}

void TurnOnMainRendering::readData(Common::SeekableReadStream &stream) {
	stream.skip(1);
}

void ResetAndStartTimer::readData(Common::SeekableReadStream &stream) {
	stream.skip(1);
}

void ResetAndStartTimer::execute() {
	NancySceneState.resetAndStartTimer();
	_isDone = true;
}

void StopTimer::readData(Common::SeekableReadStream &stream) {
	stream.skip(1);
}

void StopTimer::execute() {
	NancySceneState.stopTimer();
	_isDone = true;
}

void EventFlags::readData(Common::SeekableReadStream &stream) {
	_flags.readData(stream);
}

void EventFlags::execute() {
	_flags.execute();
	_isDone = true;
}

void EventFlagsMultiHS::readData(Common::SeekableReadStream &stream) {
	EventFlags::readData(stream);
	uint16 numHotspots = stream.readUint16LE();

	_hotspots.reserve(numHotspots);
	for (uint16 i = 0; i < numHotspots; ++i) {
		_hotspots.push_back(HotspotDescription());
		HotspotDescription &newDesc = _hotspots[i];
		newDesc.readData(stream);
	}
}

void EventFlagsMultiHS::execute() {
	switch (_state) {
	case kBegin:
		// turn main rendering on
		_state = kRun;
		// fall through
	case kRun:
		_hasHotspot = false;

		for (uint i = 0; i < _hotspots.size(); ++i) {
			if (_hotspots[i].frameID == NancySceneState.getSceneInfo().frameID) {
				_hasHotspot = true;
				_hotspot = _hotspots[i].coords;
			}
		}

		break;
	case kActionTrigger:
		_hasHotspot = false;
		EventFlags::execute();
		finishExecution();
		break;
	}
}

void LoseGame::readData(Common::SeekableReadStream &stream) {
	stream.skip(1);
}

void LoseGame::execute() {
	g_nancy->_sound->stopAndUnloadSpecificSounds();
	g_nancy->setState(NancyEngine::kMainMenu);
	NancySceneState.resetStateToInit();
	_isDone = true;
}

void PushScene::readData(Common::SeekableReadStream &stream) {
	stream.skip(1);
}

void PopScene::readData(Common::SeekableReadStream &stream) {
	stream.skip(1);
}

void WinGame::readData(Common::SeekableReadStream &stream) {
	stream.skip(1);
}

void WinGame::execute() {
	g_nancy->_sound->stopAndUnloadSpecificSounds();
	g_nancy->setState(NancyEngine::kCredits, NancyEngine::kMainMenu);

	// TODO replace with destroy()?
	NancySceneState.resetStateToInit();
	_isDone = true;
}

void AddInventoryNoHS::readData(Common::SeekableReadStream &stream) {
	_itemID = stream.readUint16LE();
}

void AddInventoryNoHS::execute() {
	if (NancySceneState.hasItem(_itemID) == kFalse) {
		NancySceneState.addItemToInventory(_itemID);
	}

	_isDone = true;
}

void RemoveInventoryNoHS::readData(Common::SeekableReadStream &stream) {
	stream.skip(2);
}

void DifficultyLevel::readData(Common::SeekableReadStream &stream) {
	_difficulty = stream.readUint16LE();
	_flag.label = stream.readSint16LE();
	_flag.flag = (NancyFlag)stream.readUint16LE();
}

void DifficultyLevel::execute() {
	NancySceneState.setDifficulty(_difficulty);
	NancySceneState.setEventFlag(_flag);
	_isDone = true;
}

void ShowInventoryItem::init() {
	g_nancy->_resource->loadImage(_imageName, _fullSurface);

	_drawSurface.create(_fullSurface, _bitmaps[0].src);

	RenderObject::init();
}

void ShowInventoryItem::readData(Common::SeekableReadStream &stream) {
	_objectID = stream.readUint16LE();
	readFilename(stream, _imageName);

	uint16 numFrames = stream.readUint16LE();

	_bitmaps.reserve(numFrames);
	for (uint i = 0; i < numFrames; ++i) {
		_bitmaps.push_back(BitmapDescription());
		_bitmaps[i].readData(stream);
	}
}

void ShowInventoryItem::execute() {
	switch (_state) {
	case kBegin:
		init();
		registerGraphics();
		_state = kRun;
		// fall through
	case kRun: {
		int newFrame = -1;

		for (uint i = 0; i < _bitmaps.size(); ++i) {
			if (_bitmaps[i].frameID == NancySceneState.getSceneInfo().frameID) {
				newFrame = i;
				break;
			}
		}

		if (newFrame != _drawnFrameID) {
			_drawnFrameID = newFrame;

			if (newFrame != -1) {
				_hasHotspot = true;
				_hotspot = _bitmaps[newFrame].dest;
				_drawSurface.create(_fullSurface, _bitmaps[newFrame].src);
				_screenPosition = _bitmaps[newFrame].dest;
				setVisible(true);
			} else {
				_hasHotspot = false;
				setVisible(false);
			}
		}

		break;
	}
	case kActionTrigger:
		g_nancy->_sound->playSound(24); // Hardcoded by original engine
		NancySceneState.addItemToInventory(_objectID);
		setVisible(false);
		_hasHotspot = false;
		finishExecution();
		break;
	}
}

void ShowInventoryItem::onPause(bool pause) {
	if (pause) {
		registerGraphics();
	}
}

void PlayDigiSoundAndDie::readData(Common::SeekableReadStream &stream) {
	_sound.read(stream, SoundDescription::kDIGI);
	_sceneChange.readData(stream);
	_flagOnTrigger.label = stream.readSint16LE();
	_flagOnTrigger.flag = (NancyFlag)stream.readByte();
	stream.skip(2);
}

void PlayDigiSoundAndDie::execute() {
	switch (_state) {
	case kBegin:
		g_nancy->_sound->loadSound(_sound);
		g_nancy->_sound->playSound(_sound);
		_state = kRun;
		break;
	case kRun:
		if (!g_nancy->_sound->isSoundPlaying(_sound)) {
			_state = kActionTrigger;
		}

		break;
	case kActionTrigger:
		if (_sceneChange.sceneID != 9999) {
			NancySceneState.changeScene(_sceneChange);
		}

		NancySceneState.setEventFlag(_flagOnTrigger);
		g_nancy->_sound->stopSound(_sound);

		finishExecution();
		break;
	}
}

void PlaySoundPanFrameAnchorAndDie::readData(Common::SeekableReadStream &stream) {
	stream.skip(0x20);
}

void PlaySoundMultiHS::readData(Common::SeekableReadStream &stream) {
	_sound.read(stream, SoundDescription::kNormal);
	_sceneChange.readData(stream);
	_flag.label = stream.readSint16LE();
	_flag.flag = (NancyFlag)stream.readByte();
	stream.skip(2);
	uint16 numHotspots = stream.readUint16LE();

	_hotspots.reserve(numHotspots);
	for (uint i = 0; i < numHotspots; ++i) {
		_hotspots.push_back(HotspotDescription());
		_hotspots.back().frameID = stream.readUint16LE();
		readRect(stream, _hotspots.back().coords);
	}
}

void PlaySoundMultiHS::execute() {
	switch (_state) {
	case kBegin:
		_state = kRun;
		// fall through
	case kRun: {
		_hasHotspot = false;
		uint currentFrame = NancySceneState.getSceneInfo().frameID;

		for (uint i = 0; i < _hotspots.size(); ++i) {
			if (_hotspots[i].frameID == currentFrame) {
				_hotspot = _hotspots[i].coords;
				_hasHotspot = true;
				break;
			}
		}

		break;
	}
	case kActionTrigger:
		g_nancy->_sound->loadSound(_sound);
		g_nancy->_sound->playSound(_sound);
		NancySceneState.changeScene(_sceneChange);
		NancySceneState.setEventFlag(_flag);
		finishExecution();
		break;
	}
}

void HintSystem::readData(Common::SeekableReadStream &stream) {
	_characterID = stream.readByte();
	_genericSound.read(stream, SoundDescription::kNormal);
}

void HintSystem::execute() {
	switch (_state) {
	case kBegin:
		if (NancySceneState.getHintsRemaining() > 0) {
			selectHint();
		} else {
			getHint(0, NancySceneState.getDifficulty());
		}

		NancySceneState.getTextbox().clear();
		NancySceneState.getTextbox().addTextLine(_text);

		g_nancy->_sound->loadSound(_genericSound);
		g_nancy->_sound->playSound(_genericSound);
		_state = kRun;
		break;
	case kRun:
		if (!g_nancy->_sound->isSoundPlaying(_genericSound)) {
			g_nancy->_sound->stopSound(_genericSound);
			_state = kActionTrigger;
		} else {
			break;
		}

		// fall through
	case kActionTrigger:
		NancySceneState.useHint(_hintID, _hintWeight);
		NancySceneState.getTextbox().clear();

		NancySceneState.changeScene(_sceneChange);

		_isDone = true;
		break;
	}
}

void HintSystem::selectHint() {
	for (const auto &hint : nancy1Hints) {
		if (hint.characterID != _characterID) {
			continue;
		}

		bool satisfied = true;

		for (const auto &flag : hint.flagConditions) {
			if (flag.label == -1) {
				break;
			}

			if (!NancySceneState.getEventFlag(flag.label, flag.flag)) {
				satisfied = false;
				break;
			}
		}

		for (const auto &inv : hint.inventoryCondition) {
			if (inv.label == -1) {
				break;
			}

			if (NancySceneState.hasItem(inv.label) != inv.flag) {
				satisfied = false;
				break;
			}
		}

		if (satisfied) {
			getHint(hint.hintID, NancySceneState.getDifficulty());
			break;
		}
	}
}

void HintSystem::getHint(uint hint, uint difficulty) {
	uint fileOffset;
	if (_characterID < 3) {
		fileOffset = nancy1HintOffsets[_characterID];
	}

	fileOffset += 0x288 * hint;

	Common::File file;
	file.open("game.exe");
	file.seek(fileOffset);

	_hintID = file.readSint16LE();
	_hintWeight = file.readSint16LE();

	file.seek(difficulty * 10, SEEK_CUR);

	readFilename(file, _genericSound.name);

	file.seek(-(difficulty * 10) - 10, SEEK_CUR);
	file.seek(30 + difficulty * 200, SEEK_CUR);

	char textBuf[200];
	file.read(textBuf, 200);
	textBuf[199] = '\0';
	_text = textBuf;

	file.seek(-(difficulty * 200) - 200, SEEK_CUR);
	file.seek(600, SEEK_CUR);

	_sceneChange.readData(file);
}

} // End of namespace Action
} // End of namespace Nancy
