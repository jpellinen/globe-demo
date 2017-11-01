//
// Texture.cpp
//
// Copyright (c) 2011 Juha Pellinen
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#include "Texture.h"

#include <fstream>
using std::ifstream;

bool loadTGAFromMemory(const char *buf, unsigned int &width, unsigned int &height, unsigned int &comp, unsigned char *&pixels)
{	
	// header.
	struct TGAHeader
	{
		unsigned char IDLength;
		unsigned char colorMapType;
		unsigned char imageType;
		unsigned char colorMapSpecs[5];
		
		// image specification.
		unsigned short originX;
		unsigned short originY;
		unsigned short width;
		unsigned short height;
		unsigned char pixelDepth;
		unsigned char imageDesc;
	};	
	const TGAHeader *header = (TGAHeader *)buf;
	
	// supported types.
	static const unsigned int TRUE_COLOR = 2;
	static const unsigned int TRUE_COLOR_RLE = 10;
	if (header->imageType != TRUE_COLOR && header->imageType != TRUE_COLOR_RLE)
		return false;
	// supported pixel depths.
	if (header->pixelDepth != 24 && header->pixelDepth != 32)
		return false;
	// no color map.
	if (memcmp(header->colorMapSpecs, "\0\0\0\0\0", 5) != 0)
		return false;
	
	// read image data.
	width = header->width;
	height = header->height;
	comp = header->pixelDepth / 8;
	bool alpha = comp == 4;
	// ensure that origo is at the top.
	bool flip = !(header->imageDesc & 32);

	pixels = new unsigned char[width*height*comp];	
	const unsigned char *imageData = (unsigned char *)&buf[sizeof(TGAHeader) + header->IDLength];
		
	bool rle = false;
	unsigned int repeat = 1;
	unsigned char r, g, b, a;

	for (unsigned int i = 0; i < width*height; ++i)
	{
		// read RLE-compression field.
		bool newColor = false;
		if (header->imageType == TRUE_COLOR_RLE && --repeat == 0)
		{
			unsigned char repetition = *(imageData++);
			rle = (repetition & 128) != 0;
			repeat = (repetition & 127) + 1;
			newColor = true;
		}
		// read color components.
		if (!rle || newColor)
		{
			b = *(imageData++);
			g = *(imageData++);
			r = *(imageData++);
			a = alpha ? *(imageData++) : 0;
		}
		// store pixel.
		unsigned int x = i % width;
		unsigned int y = i / width;
		unsigned int index = (x + (flip ? height-y-1 : y) * width)*comp;
		pixels[index]   = r;
		pixels[index+1] = g;
		pixels[index+2] = b;
		if (alpha)
			pixels[index+3] = a;
	}
	return true;
}

bool loadTGAFromFile(const char *filename, unsigned int &width, unsigned int &height, unsigned int &comp, unsigned char *&pixels)
{
	ifstream file(filename, ifstream::binary);
	
	if (!file.is_open())
		return false;
	
	// file length.
	file.seekg (0, ifstream::end);
	int length = (int)file.tellg();
	file.seekg (0, ifstream::beg);
	
	// load image.
	char *buf = new char[length];
	file.read(buf, length);
	bool result = loadTGAFromMemory(buf, width, height, comp, pixels);
	delete[] buf;
	
	return result;
}
