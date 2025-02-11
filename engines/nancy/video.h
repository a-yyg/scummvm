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

#ifndef NANCY_VIDEO_H
#define NANCY_VIDEO_H

#include "graphics/pixelformat.h"
#include "video/video_decoder.h"
#include "common/rational.h"

namespace Common {
class ReadStream;
class SeekableReadStream;
}

namespace Graphics {
struct Surface;
}

namespace Nancy {

class Decompressor;

class AVFDecoder : public Video::VideoDecoder {
public:
	virtual ~AVFDecoder();

	virtual bool loadStream(Common::SeekableReadStream *stream) override;
	const Graphics::Surface *decodeFrame(uint frameNr);
	void addFrameTime(const uint16 timeToAdd);

private:
	class AVFVideoTrack : public FixedRateVideoTrack {
	friend class AVFDecoder;
	public:
		AVFVideoTrack(Common::SeekableReadStream *stream, uint32 chunkFileFormat);
		virtual ~AVFVideoTrack();

		virtual uint16 getWidth() const override { return _width; }
		virtual uint16 getHeight() const override { return _height; }
		virtual Graphics::PixelFormat getPixelFormat() const override { return _pixelFormat; }
		virtual int getCurFrame() const override { return _curFrame; }
		virtual int getFrameCount() const override { return _frameCount; }
		virtual bool isSeekable() const override { return true; }
		virtual bool seek(const Audio::Timestamp &time) override;
		virtual bool setReverse(bool reverse) override;
		virtual bool isReversed() const override { return _reversed; }
		virtual bool endOfTrack() const override;
		virtual const Graphics::Surface *decodeNextFrame() override;
		const Graphics::Surface *decodeFrame(uint frameNr);

	protected:
		virtual Common::Rational getFrameRate() const override { return Common::Rational(1000, _frameTime); }

	private:
		struct ChunkInfo {
			Common::String name;
			uint16 index;
			uint32 offset;
			uint32 compressedSize;
			uint32 size;
			byte type;
		};

		bool decode(byte *outBuf, uint32 frameSize, Common::ReadStream &inBuf) const;

		Common::SeekableReadStream *_fileStream;
		Graphics::PixelFormat _pixelFormat;
		uint _width, _height, _depth, _frameSize;
		int _curFrame;
		uint _frameCount;
		uint32 _frameTime;
		Graphics::Surface *_surface;
		int _refFrame;
		Common::Array<ChunkInfo> _chunkInfo;
		Decompressor *_dec;
		bool _reversed;
	};
};

} // End of namespace Nancy

#endif // NANCY_VIDEO_H
