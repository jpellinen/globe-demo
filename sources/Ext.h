//
// Ext.h
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

#ifndef _EXT_H_
#define _EXT_H_

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#include "glext.h"

extern PFNGLGENBUFFERSARBPROC				glGenBuffers;
extern PFNGLBINDBUFFERARBPROC				glBindBuffer;
extern PFNGLBUFFERDATAARBPROC				glBufferData;
extern PFNGLDELETEBUFFERSARBPROC			glDeleteBuffers;
extern PFNGLDRAWBUFFERSARBPROC				glDrawBuffers;
extern PFNGLMULTITEXCOORD2FARBPROC			glMultiTexCoord2f;
extern PFNGLMULTITEXCOORD3FARBPROC			glMultiTexCoord3f;
extern PFNGLACTIVETEXTUREARBPROC			glActiveTexture;
extern PFNGLCLIENTACTIVETEXTUREARBPROC		glClientActiveTexture;

extern PFNGLGENFRAMEBUFFERSEXTPROC			glGenFramebuffers;
extern PFNGLDELETEFRAMEBUFFERSEXTPROC		glDeleteFramebuffers;
extern PFNGLBINDFRAMEBUFFEREXTPROC			glBindFramebuffer;
extern PFNGLFRAMEBUFFERTEXTURE2DEXTPROC		glFramebufferTexture2D;
extern PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC	glCheckFramebufferStatus;
extern PFNGLGENRENDERBUFFERSEXTPROC			glGenRenderbuffers;
extern PFNGLDELETERENDERBUFFERSEXTPROC		glDeleteRenderbuffers;
extern PFNGLBINDRENDERBUFFEREXTPROC			glBindRenderbuffer;
extern PFNGLRENDERBUFFERSTORAGEEXTPROC		glRenderbufferStorage;
extern PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC	glFramebufferRenderbuffer;

extern PFNGLMAPBUFFERARBPROC				glMapBuffer;
extern PFNGLUNMAPBUFFERARBPROC				glUnmapBuffer;

extern PFNGLUSEPROGRAMOBJECTARBPROC			glUseProgram;
extern PFNGLCREATEPROGRAMOBJECTARBPROC		glCreateProgram;
extern PFNGLDELETEOBJECTARBPROC				glDeleteProgram;
extern PFNGLLINKPROGRAMARBPROC				glLinkProgram;
extern PFNGLGETPROGRAMIVARBPROC				glGetProgramiv;
extern PFNGLGETINFOLOGARBPROC				glGetProgramInfoLog;
extern PFNGLATTACHOBJECTARBPROC				glAttachShader;
extern PFNGLCREATESHADEROBJECTARBPROC		glCreateShader;
extern PFNGLDELETEOBJECTARBPROC				glDeleteShader;
extern PFNGLSHADERSOURCEARBPROC				glShaderSource;
extern PFNGLCOMPILESHADERARBPROC			glCompileShader;
extern PFNGLGETOBJECTPARAMETERIVARBPROC		glGetShaderiv;
extern PFNGLGETINFOLOGARBPROC				glGetShaderInfoLog;
extern PFNGLGETACTIVEATTRIBARBPROC			glGetActiveAttrib;
extern PFNGLBINDATTRIBLOCATIONARBPROC		glBindAttribLocation;
extern PFNGLGETATTRIBLOCATIONARBPROC		glGetAttribLocation;
extern PFNGLGETACTIVEUNIFORMARBPROC			glGetActiveUniform;
extern PFNGLGETUNIFORMLOCATIONARBPROC		glGetUniformLocation;
extern PFNGLUNIFORM1IARBPROC				glUniform1i;
extern PFNGLUNIFORM1FARBPROC				glUniform1f;
extern PFNGLUNIFORM2FARBPROC				glUniform2f;
extern PFNGLUNIFORM3FARBPROC				glUniform3f;
extern PFNGLUNIFORM3FVARBPROC				glUniform3fv;
extern PFNGLUNIFORM4FARBPROC				glUniform4f;
extern PFNGLUNIFORM4FVARBPROC				glUniform4fv;
extern PFNGLUNIFORMMATRIX4FVARBPROC			glUniformMatrix4fv;

enum ExtErrType
{
	EXT_ERR_VBO,
	EXT_ERR_FBO,
	EXT_ERR_PBO,
	EXT_ERR_MULTITEX,
	EXT_ERR_SHADER
};

class ExtException
{
public:
	ExtException(ExtErrType t) :
		type(t)
	{
	}

	const char *getDesc() const
	{
		switch (type)
		{
		case EXT_ERR_VBO: return "Error initializing vertex buffer objects."; break;
		case EXT_ERR_FBO: return "Error initializing frame buffer objects."; break;
		case EXT_ERR_PBO: return "Error initializing pixel buffer objects."; break;
		case EXT_ERR_MULTITEX: return "Error initializing multi texturing."; break;
		case EXT_ERR_SHADER: return "Error initializing shaders."; break;
		default:
			return "Unknown error.";
		};
	}

public:
	ExtErrType type;
};

extern void initExt() throw(...);

#endif /* _EXT_H_ */