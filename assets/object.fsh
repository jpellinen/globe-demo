//
// object.fsh
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

uniform float u_rimAmount;
uniform float u_rimPower;
uniform sampler2D u_albedoTexture;
uniform sampler2D u_normalTexture;
uniform sampler2D u_specularTexture;

void main()
{
	vec4 albedo = texture2D(u_albedoTexture, v_texCoord);
	vec3 normal = normalize(texture2D(u_normalTexture, v_texCoord).xyz * 2.0 - 1.0);
	vec4 specularMap = texture2D(u_specularTexture, v_texCoord);

	// lighting terms.
	float diffuse = max(dot(normal, v_lightDir), 0.0);
	vec3 halfAngle = normalize(v_lightDir + v_eyeDir);
	float specular = max(dot(halfAngle, normal), 0.0);
	float rim = 1.0 - dot(v_eyeDir, normal);

	gl_FragColor = albedo * (diffuse + 0.15) + pow(specular, 32.0) * specularMap * 0.75 + pow(rim, 2.5) * 0.15;
}
