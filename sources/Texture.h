//
// Texture.h
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

#ifndef _TEXTURE_H_
#define _TEXTURE_H_

//
// Simple and compact TGA loader 0.1
//
// * loads true color images(24 and 32-bits).
// * supports RLE-compression.
// * works on little-endian systems.
//
// 01/07/2011 Juha Pellinen
//
bool loadTGAFromMemory(const char *buf, unsigned int &width, unsigned int &height, unsigned int &comp, unsigned char *&pixels);
bool loadTGAFromFile(const char *filename, unsigned int &width, unsigned int &height, unsigned int &comp, unsigned char *&pixels);

#endif /* _TEXTURE_H_ */