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

#include "engines/nancy/detection.h"

#include "common/system.h"
#include "common/savefile.h"
#include "common/textconsole.h"

#include "graphics/thumbnail.h"
#include "graphics/surface.h"

#include "engines/advancedDetector.h"

const char *const directoryGlobs[] = {
	"game",
	"datafiles",
	0
};

static const PlainGameDescriptor nancyGames[] = {
	// Games
	{"vampirediaries", "The Vampire Diaries"},
	{"nancy1", "Nancy Drew: Secrets Can Kill"},
	{"nancy2", "Nancy Drew: Stay Tuned for Danger"},
	{"nancy3", "Nancy Drew: Message in a Haunted Mansion"},
	{"nancy4", "Nancy Drew: Treasure in the Royal Tower"},
	{"nancy5", "Nancy Drew: The Final Scene"},
	{"nancy6", "Nancy Drew: Secret of the Scarlet Hand"},
	{0, 0}
};

static const Nancy::NancyGameDescription gameDescriptions[] = {

	{ // MD5 by fracturehill
		{
			"vampirediaries", 0,
			AD_ENTRY1s("vampire.exe", "c6207f4bb7418b8a067ad75ed9f57bdf", 114688),
			Common::EN_ANY,
			Common::kPlatformWindows,
			Nancy::NGF_8BITCOLOR,
			GUIO0()
		},
		Nancy::kGameTypeVampire
	},
	{ // MD5 by waltervn
		{
			"nancy1", 0,
			AD_ENTRY1s("ciftree.dat", "9f89e0b53717515ae0eb82d14ffe0e88", 4317962),
			Common::EN_ANY,
			Common::kPlatformWindows,
			ADGF_NO_FLAGS,
			GUIO0()
		},
		Nancy::kGameTypeNancy1
	},
	{ // MD5 by waltervn
		{
			"nancy2", 0,
			AD_ENTRY1s("ciftree.dat", "fa4293d728a1b31407961cd82e86a015", 7784516),
			Common::EN_ANY,
			Common::kPlatformWindows,
			ADGF_NO_FLAGS,
			GUIO0()
		},
		Nancy::kGameTypeNancy2
	},
	{ // MD5 by waltervn
		{
			"nancy3", 0,
			AD_ENTRY1s("ciftree.dat", "ee5f8832226567c3610556497c451b09", 16256355),
			Common::EN_ANY,
			Common::kPlatformWindows,
			ADGF_NO_FLAGS,
			GUIO0()
		},
		Nancy::kGameTypeNancy3
	},
	{ // MD5 by waltervn
		{
			"nancy3", 0,
			{
				{"data1.hdr", 0, "44906f3d2242f73f16feb8eb6a5161cb", 207327},
				{"data1.cab", 0, "e258cc871e5de5ae004d03c4e31431c7", 1555916},
				{"data2.cab", 0, "364dfd25677026da505f1fa6edd5571f", 137373135},
				AD_LISTEND
			},
			Common::EN_ANY,
			Common::kPlatformWindows,
			ADGF_NO_FLAGS,
			GUIO0()
		},
		Nancy::kGameTypeNancy3
	},
	{ // MD5 by waltervn
		{
			"nancy4", 0,
			AD_ENTRY1s("ciftree.dat", "e9d45f7db453b0d8f37d202fc979537c", 8742289),
			Common::EN_ANY,
			Common::kPlatformWindows,
			ADGF_NO_FLAGS,
			GUIO0()
		},
		Nancy::kGameTypeNancy3
	},
	{ // MD5 by waltervn
		{
			"nancy4", 0,
			{
				{"data1.hdr", 0, "fa4e7a1c411053557169a7731f287012", 263443},
				{"data1.cab", 0, "8f689f92fcca443d6a03faa5de7e2f1c", 1568756},
				{"data2.cab", 0, "5525aa428041f3f1421a6fb5d1b8dba1", 140518758},
				AD_LISTEND
			},
			Common::EN_ANY,
			Common::kPlatformWindows,
			ADGF_NO_FLAGS,
			GUIO0()
		},
		Nancy::kGameTypeNancy3
	},
	{ // MD5 by waltervn
		{
			"nancy5", 0,
			AD_ENTRY1s("ciftree.dat", "21fa81f322595c3100d8d58d100852d5", 8187692),
			Common::EN_ANY,
			Common::kPlatformWindows,
			ADGF_NO_FLAGS,
			GUIO0()
		},
		Nancy::kGameTypeNancy3
	},
	{ // MD5 by waltervn
		{
			"nancy5", 0,
			{
				{"data1.hdr", 0, "261105fba2a1226eedb090c2ce79fd35", 284091},
				{"data1.cab", 0, "7d27bb947ef7305831f1faaf1512a598", 1446301},
				{"data2.cab", 0, "00719c86cab733c1094b27079ce030f3", 145857935},
				AD_LISTEND
			},
			Common::EN_ANY,
			Common::kPlatformWindows,
			ADGF_NO_FLAGS,
			GUIO0()
		},
		Nancy::kGameTypeNancy3
	},
	{ // MD5 by clone2727
		{
			"nancy5", 0,
			{
				{"data1.hdr", 0, "258e27792fa7cc7a7125fd74d89f8487", 284091},
				{"data1.cab", 0, "70433b30b6114031d54d0c991ad44577", 1446301},
				{"data2.cab", 0, "66f47e4f5e6d431f815aa5250eb044bc", 145857937},
				AD_LISTEND
			},
			Common::EN_ANY,
			Common::kPlatformWindows,
			ADGF_NO_FLAGS,
			GUIO0()
		},
		Nancy::kGameTypeNancy3
	},
	{ // MD5 by Strangerke
		{
			"nancy6", 0,
			AD_ENTRY1s("ciftree.dat", "a97b848651fdcf38f5cad7092d98e4a1", 28888006),
			Common::EN_ANY,
			Common::kPlatformWindows,
			ADGF_NO_FLAGS,
			GUIO0()
		},
		Nancy::kGameTypeNancy3
	},
	{AD_TABLE_END_MARKER, Nancy::kGameTypeNone}
};

class NancyMetaEngineDetection : public AdvancedMetaEngineDetection {
public:
	NancyMetaEngineDetection() : AdvancedMetaEngineDetection(gameDescriptions, sizeof(Nancy::NancyGameDescription), nancyGames) {
		_maxScanDepth = 2;
		_directoryGlobs = directoryGlobs;
	}

	const char *getEngineId() const override {
		return "nancy";
	}

	const char *getName() const override {
		return "Nancy Drew";
	}

	const char *getOriginalCopyright() const override {
		return "Nancy Drew Engine copyright Her Interactive, 1995-2012";
	}

};

REGISTER_PLUGIN_STATIC(NANCY_DETECTION, PLUGIN_TYPE_ENGINE_DETECTION, NancyMetaEngineDetection);
