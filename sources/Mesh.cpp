//
// Mesh.cpp
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

#include "Mesh.h"

#include <fstream>
using std::ifstream;

Mesh::Mesh() :
	m_vertices(0),
	m_numVertices(0),
	m_indices(0),
	m_numIndices(0)
{
}

Mesh::~Mesh()
{
	delete[] m_vertices;
	delete[] m_indices;
}

bool loadMod(const char *filename, Mesh &mesh)
{
	ifstream file(filename, ifstream::binary);

	if (!file.is_open())
		return false;

	// validate.
	char header[4];
	file.read(header, sizeof(header));
	if (strncmp(header, "mod1", 4) != 0)
		return false;
	
	// read vertices.
	file.read((char *)&mesh.m_numVertices, sizeof(int));	
	mesh.m_vertices = new Mesh::Vertex[mesh.m_numVertices];
	file.read((char *)mesh.m_vertices, mesh.m_numVertices * sizeof(Mesh::Vertex));
	
	// read indices.
	file.read((char *)&mesh.m_numIndices, sizeof(int));
	mesh.m_indices = new short[mesh.m_numIndices];
	file.read((char *)mesh.m_indices, mesh.m_numIndices * sizeof(short));
	
	return true;
}
