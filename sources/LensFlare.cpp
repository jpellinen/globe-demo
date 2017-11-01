//
// LensFlare.cpp
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

#include "LensFlare.h"

#include "Shader.h"

using std::vector;

LensFlare::LensFlare(GLuint shader, GLuint texture, unsigned int numFrames) :
	m_shader(shader),
	m_texture(texture),
	m_numFrames(numFrames),
	m_fade(0.0f)
{
}

void LensFlare::addElement(float position, float size, float alpha, unsigned int frame)
{
	Element e;
	e.position = position;
	e.size = size;
	e.alpha = alpha;
	e.frame = frame;

	m_elements.push_back(e);
}

void LensFlare::addFade(float dt)
{
	m_fade = min(max(m_fade + dt, 0.0f), 1.0f);
}

void LensFlare::drawTexture(float x, float y, float width, float height, unsigned int frame, unsigned int numFrames)
{
	GLfloat verts[] =
	{
		x, y, x+width, y, x, y+height, x+width, y+height
	};

	GLfloat frameSize = 1.0f / (float)numFrames;
	GLfloat frameLeft = (float)frame * frameSize;
	GLfloat frameRight = frameLeft + frameSize;

	GLfloat texCoords[] =
	{
		frameLeft, 1, frameRight, 1, frameLeft, 0, frameRight, 0
	};

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, verts);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void LensFlare::paint()
{
	glUseProgram(m_shader);
	glUniformMatrix4fv(glGetUniformLocation(m_shader, "u_modelViewProjectionMatrix"), 1, GL_FALSE, m_projection);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	for (unsigned int i = 0; i < m_elements.size(); ++i)
	{
		const Element &e = m_elements[i];
		float ex = m_position.x - m_position.x * e.position - e.size * 0.5f;
		float ey = m_position.y - m_position.y * e.position - e.size * 0.5f;

		glUniform4f(glGetUniformLocation(m_shader, "u_color"), 1, 1, 1, e.alpha * m_fade);
		drawTexture(ex, ey, e.size, e.size, e.frame, m_numFrames);
	}

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

void LensFlare::setProjection(const Mat4 &m)
{
	m_projection = m;
}

void LensFlare::setPosition(const Vec3 &v)
{
	m_position = v;
}
