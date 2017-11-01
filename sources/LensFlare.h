//
// LensFlare.h
//
// Copyright (c) 2012 Juha Pellinen
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

#pragma once

#include "Math.h"

#include <vector>
#include <GLUT/glut.h>

class LensFlare
{
public:
	LensFlare(GLuint shader, GLuint texture, unsigned int numFrames);

	void addElement(float position, float size, float alpha, unsigned int frame);
	void addFade(float dt);
	void paint();

	void setProjection(const Mat4 &m);
	void setPosition(const Vec3 &v);

private:
	void drawTexture(float x, float y, float width, float height, unsigned int frame, unsigned int numFrames);

private:
	GLuint m_shader;
	Mat4 m_projection;

	GLuint m_texture;
	unsigned int m_numFrames;

	struct Element
	{
		float position;
		float size;
		float alpha;
		unsigned int frame;
	};

	std::vector<Element> m_elements;

	Vec3 m_position;
	float m_fade;
};
