//
// object.vsh
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

varying vec2 v_texCoord;
varying vec3 v_lightDir;
varying vec3 v_eyeDir;

uniform vec3 u_lightPos;
uniform vec3 u_eyePos;
uniform mat4 u_modelViewProjectionMatrix;
uniform mat4 u_modelMatrix;
uniform mat4 u_invModelMatrix;

void main()
{
	gl_Position = u_modelViewProjectionMatrix * gl_Vertex;
	v_texCoord = gl_MultiTexCoord0.xy;

	// calculate light and eye object space directions.
	vec3 pos = (u_modelMatrix * gl_Vertex).xyz;
	mat3 invRot = mat3(u_invModelMatrix[0].xyz, u_invModelMatrix[1].xyz, u_invModelMatrix[2].xyz);
	v_lightDir = normalize(invRot * (u_lightPos - pos));
	v_eyeDir = normalize(invRot * (u_eyePos - pos));
}
