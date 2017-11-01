//
// Ext.cpp
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

#include "Ext.h"

PFNGLGENBUFFERSARBPROC				glGenBuffers = 0;
PFNGLBINDBUFFERARBPROC				glBindBuffer = 0;
PFNGLBUFFERDATAARBPROC				glBufferData = 0;
PFNGLDELETEBUFFERSARBPROC			glDeleteBuffers = 0;
PFNGLDRAWBUFFERSARBPROC				glDrawBuffers = 0;
PFNGLMULTITEXCOORD2FARBPROC			glMultiTexCoord2f = 0;
PFNGLMULTITEXCOORD3FARBPROC			glMultiTexCoord3f = 0;
PFNGLACTIVETEXTUREARBPROC			glActiveTexture = 0;
PFNGLCLIENTACTIVETEXTUREARBPROC		glClientActiveTexture = 0;

PFNGLGENFRAMEBUFFERSEXTPROC			glGenFramebuffers = 0;
PFNGLDELETEFRAMEBUFFERSEXTPROC		glDeleteFramebuffers = 0;
PFNGLBINDFRAMEBUFFEREXTPROC			glBindFramebuffer = 0;
PFNGLFRAMEBUFFERTEXTURE2DEXTPROC	glFramebufferTexture2D = 0;
PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC	glCheckFramebufferStatus = 0;
PFNGLGENRENDERBUFFERSEXTPROC		glGenRenderbuffers;
PFNGLDELETERENDERBUFFERSEXTPROC		glDeleteRenderbuffers;
PFNGLBINDRENDERBUFFEREXTPROC		glBindRenderbuffer = 0;
PFNGLRENDERBUFFERSTORAGEEXTPROC		glRenderbufferStorage = 0;
PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC	glFramebufferRenderbuffer = 0;

PFNGLMAPBUFFERARBPROC				glMapBuffer = 0;
PFNGLUNMAPBUFFERARBPROC				glUnmapBuffer = 0;

PFNGLUSEPROGRAMOBJECTARBPROC		glUseProgram = 0;
PFNGLCREATEPROGRAMOBJECTARBPROC		glCreateProgram = 0;
PFNGLDELETEOBJECTARBPROC			glDeleteProgram = 0;
PFNGLLINKPROGRAMARBPROC				glLinkProgram = 0;
PFNGLGETPROGRAMIVARBPROC			glGetProgramiv = 0;
PFNGLGETINFOLOGARBPROC				glGetProgramInfoLog = 0;
PFNGLATTACHOBJECTARBPROC			glAttachShader = 0;
PFNGLCREATESHADEROBJECTARBPROC		glCreateShader = 0;
PFNGLDELETEOBJECTARBPROC			glDeleteShader = 0;
PFNGLSHADERSOURCEARBPROC			glShaderSource = 0;
PFNGLCOMPILESHADERARBPROC			glCompileShader = 0;
PFNGLGETOBJECTPARAMETERIVARBPROC	glGetShaderiv = 0;
PFNGLGETINFOLOGARBPROC				glGetShaderInfoLog = 0;
PFNGLGETACTIVEATTRIBARBPROC			glGetActiveAttrib = 0;
PFNGLBINDATTRIBLOCATIONARBPROC		glBindAttribLocation = 0;
PFNGLGETATTRIBLOCATIONARBPROC		glGetAttribLocation = 0;
PFNGLGETACTIVEUNIFORMARBPROC		glGetActiveUniform = 0;
PFNGLGETUNIFORMLOCATIONARBPROC		glGetUniformLocation = 0;
PFNGLUNIFORM1IARBPROC				glUniform1i = 0;
PFNGLUNIFORM1FARBPROC				glUniform1f = 0;
PFNGLUNIFORM2FARBPROC				glUniform2f = 0;
PFNGLUNIFORM3FARBPROC				glUniform3f = 0;
PFNGLUNIFORM3FVARBPROC				glUniform3fv = 0;
PFNGLUNIFORM4FARBPROC				glUniform4f = 0;
PFNGLUNIFORM4FVARBPROC				glUniform4fv = 0;
PFNGLUNIFORMMATRIX4FVARBPROC		glUniformMatrix4fv = 0;

void initExt() throw(...)
{
	glGenBuffers				= (PFNGLGENBUFFERSARBPROC)wglGetProcAddress("glGenBuffersARB");
	glBindBuffer				= (PFNGLBINDBUFFERARBPROC)wglGetProcAddress("glBindBufferARB");
	glBufferData				= (PFNGLBUFFERDATAARBPROC)wglGetProcAddress("glBufferDataARB");
	glDeleteBuffers				= (PFNGLDELETEBUFFERSARBPROC)wglGetProcAddress("glDeleteBuffersARB");
	glDrawBuffers				= (PFNGLDRAWBUFFERSARBPROC)wglGetProcAddress("glDrawBuffers");

	if(!glGenBuffers || !glBindBuffer || !glBufferData || !glDeleteBuffers || !glDrawBuffers)
	{
		throw ExtException(EXT_ERR_VBO);
	}

	glGenFramebuffers			= (PFNGLGENFRAMEBUFFERSEXTPROC)wglGetProcAddress("glGenFramebuffersEXT");
	glDeleteFramebuffers		= (PFNGLDELETEFRAMEBUFFERSEXTPROC)wglGetProcAddress("glDeleteFramebuffersEXT");
	glBindFramebuffer			= (PFNGLBINDFRAMEBUFFEREXTPROC)wglGetProcAddress("glBindFramebufferEXT");
	glFramebufferTexture2D		= (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC)wglGetProcAddress("glFramebufferTexture2DEXT");
	glCheckFramebufferStatus	= (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC)wglGetProcAddress("glCheckFramebufferStatusEXT");
	glGenRenderbuffers			= (PFNGLGENRENDERBUFFERSEXTPROC)wglGetProcAddress("glGenRenderbuffersEXT");
	glBindRenderbuffer			= (PFNGLBINDRENDERBUFFEREXTPROC)wglGetProcAddress("glBindRenderbufferEXT");
	glDeleteRenderbuffers		= (PFNGLDELETERENDERBUFFERSEXTPROC)wglGetProcAddress("glDeleteRenderbuffersEXT");
	glRenderbufferStorage		= (PFNGLRENDERBUFFERSTORAGEEXTPROC)wglGetProcAddress("glRenderbufferStorageEXT");
	glFramebufferRenderbuffer	= (PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC)wglGetProcAddress("glFramebufferRenderbufferEXT");

	if (!glGenFramebuffers || !glDeleteFramebuffers || !glBindFramebuffer || !glFramebufferTexture2D ||	!glCheckFramebufferStatus ||
		!glGenRenderbuffers || !glDeleteRenderbuffers || !glBindRenderbuffer || !glRenderbufferStorage || !glFramebufferRenderbuffer)
	{
		throw ExtException(EXT_ERR_FBO);
	}

	glMapBuffer					= (PFNGLMAPBUFFERARBPROC)wglGetProcAddress("glMapBufferARB");
	glUnmapBuffer				= (PFNGLUNMAPBUFFERARBPROC)wglGetProcAddress("glUnmapBufferARB");

	if (!glMapBuffer || !glUnmapBuffer)
	{
		throw ExtException(EXT_ERR_PBO);
	}

	glMultiTexCoord2f			= (PFNGLMULTITEXCOORD2FARBPROC)wglGetProcAddress("glMultiTexCoord2fARB");
	glMultiTexCoord3f			= (PFNGLMULTITEXCOORD3FARBPROC)wglGetProcAddress("glMultiTexCoord3fARB");
	glActiveTexture				= (PFNGLACTIVETEXTUREARBPROC)wglGetProcAddress("glActiveTextureARB");
	glClientActiveTexture		= (PFNGLCLIENTACTIVETEXTUREARBPROC)wglGetProcAddress("glClientActiveTextureARB");

	if(!glMultiTexCoord2f || !glMultiTexCoord3f || !glActiveTexture || !glClientActiveTexture)
	{
		throw ExtException(EXT_ERR_MULTITEX);
	}

	glUseProgram				= (PFNGLUSEPROGRAMOBJECTARBPROC)wglGetProcAddress("glUseProgram");
	glCreateProgram				= (PFNGLCREATEPROGRAMOBJECTARBPROC)wglGetProcAddress("glCreateProgram");
	glDeleteProgram				= (PFNGLDELETEOBJECTARBPROC)wglGetProcAddress("glDeleteProgram");
	glLinkProgram				= (PFNGLLINKPROGRAMARBPROC)wglGetProcAddress("glLinkProgram");
	glGetProgramiv				= (PFNGLGETPROGRAMIVARBPROC)wglGetProcAddress("glGetProgramiv");
	glGetProgramInfoLog			= (PFNGLGETINFOLOGARBPROC)wglGetProcAddress("glGetProgramInfoLog");
	glAttachShader				= (PFNGLATTACHOBJECTARBPROC)wglGetProcAddress("glAttachShader");
	glCreateShader				= (PFNGLCREATESHADEROBJECTARBPROC)wglGetProcAddress("glCreateShader");
	glDeleteShader				= (PFNGLDELETEOBJECTARBPROC)wglGetProcAddress("glDeleteShader");
	glShaderSource				= (PFNGLSHADERSOURCEARBPROC)wglGetProcAddress("glShaderSource");
	glCompileShader				= (PFNGLCOMPILESHADERARBPROC)wglGetProcAddress("glCompileShader");
	glGetShaderiv				= (PFNGLGETOBJECTPARAMETERIVARBPROC)wglGetProcAddress("glGetShaderiv");
	glGetShaderInfoLog			= (PFNGLGETINFOLOGARBPROC)wglGetProcAddress("glGetShaderInfoLog");
	glGetActiveAttrib			= (PFNGLGETACTIVEATTRIBARBPROC)wglGetProcAddress("glGetActiveAttrib");
	glBindAttribLocation		= (PFNGLBINDATTRIBLOCATIONARBPROC)wglGetProcAddress("glBindAttribLocation");
	glGetAttribLocation			= (PFNGLGETATTRIBLOCATIONARBPROC)wglGetProcAddress("glGetAttribLocation");
	glGetActiveUniform			= (PFNGLGETACTIVEUNIFORMARBPROC)wglGetProcAddress("glGetActiveUniform");
	glGetUniformLocation		= (PFNGLGETUNIFORMLOCATIONARBPROC)wglGetProcAddress("glGetUniformLocation");
	glUniform1i					= (PFNGLUNIFORM1IARBPROC)wglGetProcAddress("glUniform1i");
	glUniform1f					= (PFNGLUNIFORM1FARBPROC)wglGetProcAddress("glUniform1f");
	glUniform2f					= (PFNGLUNIFORM2FARBPROC)wglGetProcAddress("glUniform2f");
	glUniform3f					= (PFNGLUNIFORM3FARBPROC)wglGetProcAddress("glUniform3f");
	glUniform3fv				= (PFNGLUNIFORM3FVARBPROC)wglGetProcAddress("glUniform3fv");
	glUniform4f					= (PFNGLUNIFORM4FARBPROC)wglGetProcAddress("glUniform4f");
	glUniform4fv				= (PFNGLUNIFORM4FVARBPROC)wglGetProcAddress("glUniform4fv");
	glUniformMatrix4fv			= (PFNGLUNIFORMMATRIX4FVARBPROC)wglGetProcAddress("glUniformMatrix4fv");

	if (!glUseProgram || !glCreateProgram || !glDeleteProgram || !glLinkProgram || !glGetProgramiv || !glGetProgramInfoLog ||
		!glAttachShader || !glCreateShader || !glDeleteShader || !glShaderSource ||
		!glCompileShader || !glGetShaderiv || !glGetShaderInfoLog ||
		!glGetActiveAttrib || !glBindAttribLocation || !glGetAttribLocation || !glGetActiveUniform || !glGetUniformLocation ||
		!glUniform1i || !glUniform1f || !glUniform2f || !glUniform3f || !glUniform3fv ||
		!glUniform4f || !glUniform4fv || !glUniformMatrix4fv)
	{
		throw ExtException(EXT_ERR_SHADER);
	}
}

