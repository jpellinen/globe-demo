//
// Main.cpp
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

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#if __APPLE__
#include <sys/time.h>
#endif
#include <GLUT/glut.h>

#include "Math.h"
#include "Texture.h"
#include "Shader.h"
#include "Mesh.h"
#include "LensFlare.h"

// screen.
float screenWidth;
float screenHeight;
float screenAspect;

// mouse.
int mouseX = 0;
int mouseY = 0;
bool mouseLeft = false;
bool mouseRight = false;

// debug labels.
char resolutionLabel[64];
char frameRateLabel[64];

// shaders.
enum
{
	SHADER_COLOR,
	SHADER_TEXTURE,
	SHADER_TEXTURE_STRIP,
	SHADER_OBJECT,
	NUM_SHADERS
};

GLuint shaders[NUM_SHADERS];

// textures.
enum
{
	TEXTURE_GLOBE_ALBEDO,
	TEXTURE_GLOBE_NORMAL,
	TEXTURE_GLOBE_SPECULAR,
	TEXTURE_FLARE,
	TEXTURE_SKYBOX,
	NUM_TEXTURES
};

GLuint textures[NUM_TEXTURES];

// objects.
struct Object
{
	GLuint vbo;
	GLuint ibo;
	short numIndices;
};

enum
{
	OBJECT_GLOBE,
	OBJECT_SKYBOX,
	OBJECT_PLANE,
	NUM_OBJECTS
};

Object objects[NUM_OBJECTS];

// camera.
Vec3 cameraPosition;
Vec3 cameraTarget;
Mat4 cameraProjection;
Mat4 cameraView;
float cameraYaw = PI * 0.5f;
float cameraPitch = PI * 0.5f;
float cameraZoom = 4.0f;

// lens flare.
LensFlare *flare;

Vec3 lightPos(5, 0, 0);
Vec3 globePos(0, 0, 0);
float globeRot = 0.0f;

bool loadTexture(const char *filename, GLuint &tex)
{
	unsigned int width, height, comp;
	unsigned char *pixels;
	
	if (loadTGAFromFile(filename, width, height, comp, pixels))
	{
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
	
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height, comp == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, pixels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		delete[] pixels;

		return true;
	}
	return false;
}

bool loadMesh(const char *filename, Object &o)
{
	// load mesh.
	Mesh mesh;
	if (!loadMod(filename, mesh))
		return false;

	// create vertex buffer object.
	glGenBuffers(1, &o.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, o.vbo);
	glBufferData(GL_ARRAY_BUFFER, mesh.m_numVertices*sizeof(Mesh::Vertex), mesh.m_vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	// create index buffer object.
	glGenBuffers(1, &o.ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, o.ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.m_numIndices*sizeof(short), mesh.m_indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	o.numIndices = mesh.m_numIndices;

	return true;
}

bool init()
{
	// load shaders.
	if (!loadShader(shaders[SHADER_COLOR], "assets/color.vsh", "assets/color.fsh") ||
		!loadShader(shaders[SHADER_TEXTURE], "assets/texture.vsh", "assets/texture.fsh") ||
		!loadShader(shaders[SHADER_TEXTURE_STRIP], "assets/texture_strip.vsh", "assets/texture_strip.fsh") ||
		!loadShader(shaders[SHADER_OBJECT], "assets/object.vsh", "assets/object.fsh"))
	{
		return false;
	}

	// init shaders.
	glUseProgram(shaders[SHADER_TEXTURE]);
	glUniform1i(glGetUniformLocation(shaders[SHADER_TEXTURE], "u_texture"), 0);
	glUseProgram(shaders[SHADER_TEXTURE_STRIP]);
	glUniform1i(glGetUniformLocation(shaders[SHADER_TEXTURE_STRIP], "u_texture"), 0);
	glUseProgram(shaders[SHADER_OBJECT]);
	glUniform1i(glGetUniformLocation(shaders[SHADER_OBJECT], "u_albedoTexture"), 0);
	glUniform1i(glGetUniformLocation(shaders[SHADER_OBJECT], "u_normalTexture"), 1);
	glUniform1i(glGetUniformLocation(shaders[SHADER_OBJECT], "u_specularTexture"), 2);
	glUseProgram(0);

	// load textures.
	if (!loadTexture("assets/earth_albedo.tga", textures[TEXTURE_GLOBE_ALBEDO]) ||
		!loadTexture("assets/earth_normal.tga", textures[TEXTURE_GLOBE_NORMAL]) ||
		!loadTexture("assets/earth_specular.tga", textures[TEXTURE_GLOBE_SPECULAR]) ||
		!loadTexture("assets/flares.tga", textures[TEXTURE_FLARE]) ||
		!loadTexture("assets/skybox.tga", textures[TEXTURE_SKYBOX]))
	{
		return false;
	}

	// load meshes.
	if (!loadMesh("assets/earth.mod", objects[OBJECT_GLOBE]) ||
		!loadMesh("assets/skybox.mod", objects[OBJECT_SKYBOX]) ||
		!loadMesh("assets/plane.mod", objects[OBJECT_PLANE]))
	{
		return false;
	}

	// init lens flare.
	flare = new LensFlare(shaders[SHADER_TEXTURE], textures[TEXTURE_FLARE], 4);
	flare->addElement(0.0f, 1.04f, 0.75f, 2);
	flare->addElement(0.4f, 0.13f, 0.1f, 1);
	flare->addElement(0.75f, 0.26f, 0.1f, 3);
	flare->addElement(1.15f, 0.26f, 0.1f, 0);
	flare->addElement(1.5f, 0.52f, 0.1f, 3);
	flare->addElement(2.0f, 0.78f, 0.1f, 1);
	flare->addElement(2.5f, 0.13f, 0.1f, 1);

	// init OpenGL states.
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	return true;
}

void deInit()
{
	// clean up.
	delete flare;

	for (unsigned int i = 0; i < NUM_SHADERS; ++i)
		glDeleteProgram(shaders[i]);

	glDeleteTextures(NUM_TEXTURES, textures);

	for (unsigned int i = 0; i < NUM_OBJECTS; ++i)
	{
		Object &o = objects[i];
		glDeleteBuffers(1, &o.vbo);
		glDeleteBuffers(1, &o.ibo);
	}
}

void update(float dt)
{
	// rotate globe.
	globeRot += dt * 0.00025f;

	// update camera.
	static int oldX = 0;
	static int oldY = 0;

	if (mouseLeft)
	{
		cameraYaw += (float)(mouseX - oldX) * 0.005f;
		cameraPitch = min(max(cameraPitch + (float)(oldY - mouseY) * 0.005f, 0.0001f), PI);
	}
	if (mouseRight)
	{
		cameraZoom = min(max(cameraZoom + (float)(mouseY - oldY) * 0.01f, 2.0f), 16.0f);
	}
	oldX = mouseX;
	oldY = mouseY;

	cameraPosition.x = cameraZoom * sinf(cameraPitch) * cosf(cameraYaw);
	cameraPosition.y = cameraZoom * cosf(cameraPitch);
	cameraPosition.z = cameraZoom * sinf(cameraPitch) * sinf(cameraYaw);

	Mat4LookAt(cameraView, cameraPosition, cameraTarget, Vec3(0, 1, 0));

	// update lens flare.
	Vec3 flarePos;
	Vec3Transform(flarePos, lightPos, cameraView * cameraProjection);
	flare->setPosition(Vec3(flarePos.x * screenAspect, flarePos.y, 0.0f));

	RayHitInfo hitInfo = raySphereIntersection(cameraPosition, lightPos - cameraPosition, globePos, 0.5f);
	bool visible = (!hitInfo.hit || hitInfo.dist > 1.0f) && fabsf(flarePos.x) < 1.0f && fabsf(flarePos.y) < 1.0f && flarePos.z < 1.0f;
	flare->addFade((visible?1:-1) * dt * 0.005f);
}

#if __APPLE__
long GetTickCount()
{
	timeval t;
	gettimeofday(&t, NULL);
	return ((long)t.tv_sec)*1000 + ((long)t.tv_usec)/1000;
}
#endif

void renderObject(const Object &o)
{
	glBindBuffer(GL_ARRAY_BUFFER, o.vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, o.ibo);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(Mesh::Vertex), 0);
	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, sizeof(Mesh::Vertex), (GLvoid *)(3*sizeof(float)));
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Mesh::Vertex), (GLvoid *)(6*sizeof(float)));
	
	glDrawElements(GL_TRIANGLES, o.numIndices, GL_UNSIGNED_SHORT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void renderSkybox()
{
	glUseProgram(shaders[SHADER_TEXTURE]);

	Mat4 model;
	Mat4Translate(model, cameraPosition.x, cameraPosition.y, cameraPosition.z);
	glUniformMatrix4fv(glGetUniformLocation(shaders[SHADER_TEXTURE], "u_modelViewProjectionMatrix"), 1, GL_FALSE, model * cameraView * cameraProjection);
	glUniform4f(glGetUniformLocation(shaders[SHADER_TEXTURE], "u_color"), 1, 1, 1, 1);

	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_SKYBOX]);

	renderObject(objects[OBJECT_SKYBOX]);

	glEnable(GL_DEPTH_TEST);
}

void renderGlow()
{
	glUseProgram(shaders[SHADER_TEXTURE_STRIP]);

	Mat4 mt;
	Mat4Translate(mt, globePos.x - cameraPosition.x, globePos.y - cameraPosition.y, globePos.z - cameraPosition.z);
	Mat4 mr;
	Mat4Inverse(mr, 0, cameraView);
	Mat4 ms;
	Mat4Scaling(ms, 1.3f, 1.3f, 1.3f);
	Mat4 model = ms * mr * mt;
	glUniformMatrix4fv(glGetUniformLocation(shaders[SHADER_TEXTURE_STRIP], "u_modelViewProjectionMatrix"), 1, GL_FALSE, model * cameraView * cameraProjection);
	glUniform4f(glGetUniformLocation(shaders[SHADER_TEXTURE_STRIP], "u_texCoordManipulator"), 0, 0, 0.25f, 1);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_FLARE]);

	renderObject(objects[OBJECT_PLANE]);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

void renderGlobe()
{
	glUseProgram(shaders[SHADER_OBJECT]);

	Mat4 mt;
	Mat4Translate(mt, globePos.x, globePos.y, globePos.z);
	Mat4 mr;
	Mat4RotationY(mr, globeRot);
	Mat4 model = mr * mt;
	glUniformMatrix4fv(glGetUniformLocation(shaders[SHADER_OBJECT], "u_modelViewProjectionMatrix"), 1, GL_FALSE, model * cameraView * cameraProjection);
	glUniformMatrix4fv(glGetUniformLocation(shaders[SHADER_OBJECT], "u_modelMatrix"), 1, GL_FALSE, model);
	Mat4 invModel;
	Mat4Inverse(invModel, 0, model);
	glUniformMatrix4fv(glGetUniformLocation(shaders[SHADER_OBJECT], "u_invModelMatrix"), 1, GL_FALSE, invModel);
	glUniform3fv(glGetUniformLocation(shaders[SHADER_OBJECT], "u_eyePos"), 1, cameraPosition);
	glUniform3fv(glGetUniformLocation(shaders[SHADER_OBJECT], "u_lightPos"), 1, lightPos);

	glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_GLOBE_ALBEDO]);
	glActiveTexture(GL_TEXTURE1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_GLOBE_NORMAL]);
	glActiveTexture(GL_TEXTURE2);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_GLOBE_SPECULAR]);

	renderObject(objects[OBJECT_GLOBE]);

	glActiveTexture(GL_TEXTURE1);
	glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE2);
	glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
}

void renderLight()
{
	glUseProgram(shaders[SHADER_COLOR]);

	Mat4 mt;
	Mat4Translate(mt, lightPos.x, lightPos.y, lightPos.z);
	Mat4 ms;
	Mat4Scaling(ms, 0.2f, 0.2f, 0.2f);
	Mat4 model = ms * mt;
	glUniformMatrix4fv(glGetUniformLocation(shaders[SHADER_COLOR], "u_modelViewProjectionMatrix"), 1, GL_FALSE, model * cameraView * cameraProjection);
	glUniform4f(glGetUniformLocation(shaders[SHADER_COLOR], "u_color"), 1, 1, 1, 1);

	glDisable(GL_TEXTURE_2D);

	renderObject(objects[OBJECT_GLOBE]);

	glEnable(GL_TEXTURE_2D);
}

float getTimeElapsed()
{
#if _WIN32
	static __int64 freq = -1;
	if (freq == -1)
		QueryPerformanceFrequency((LARGE_INTEGER*)&freq);

	__int64 cur;
	QueryPerformanceCounter((LARGE_INTEGER*)&cur);

	static __int64 last;
	float time = (float)(cur - last) / (float)freq * 1000.0f;
	last = cur;
#elif __APPLE__
	timeval t;
	gettimeofday(&t, NULL);
	long cur = ((long)t.tv_sec)*1000*1000 + (long)t.tv_usec;
	
	static long last;
	float time = (float)(cur - last) * 0.001f;
	last = cur;
#else
#error getTimeElapsed() not implemented!
#endif
	return time;
}

void printText(const char *text, float x, float y)
{
	glRasterPos2f(x/screenWidth, y/screenHeight);

	while (*text)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *(text++));
}

void paintDebugInfo()
{
	// frame rate.
	static unsigned int numFrames = 0;
	static float timeElapsed = 0.0f;
	static float frameRate = 0.0f;

	++numFrames;
	timeElapsed += getTimeElapsed();
	if (timeElapsed >= 500.0f)
	{
		frameRate = 500.0f / numFrames;
		sprintf(frameRateLabel, "ms: %f", frameRate);
		timeElapsed = 0.0f;
		numFrames = 0;
	}

	// debug texts.
	glUseProgram(0);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, 1, 0, 1,-1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	//printText(resolutionLabel, 20, 35);
	printText(frameRateLabel, 20, 20);

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
}

void display()
{
	// update.
	long curTick = GetTickCount();
	static long lastTick = curTick;
	update((float)(curTick - lastTick));
	lastTick = curTick;
	
	// render scene.
	glClear(GL_DEPTH_BUFFER_BIT);

	renderSkybox();
	renderGlow();
	renderGlobe();
	renderLight();
	flare->paint();

	paintDebugInfo();

    glutSwapBuffers();
}

void reshape(int width, int height)
{
	screenWidth = (float)width;
	screenHeight = (float)height;
	screenAspect = screenWidth / screenHeight;
	sprintf(resolutionLabel, "%.0f x %.0f", screenWidth, screenHeight);

	Mat4Perspective(cameraProjection, 60.0f, screenAspect, 0.1f, 100.0f);

	Mat4 proj;
	Mat4Ortho(proj,-screenAspect, screenAspect,-1, 1,-1, 1);
	flare->setProjection(proj);

    glViewport(0, 0, width, height);
}

void idle()
{
    glutPostRedisplay();
}

void keyPressed(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		deInit();
		exit(0);
	};
}

void mouseFunc(int button, int state, int x, int y)
{
	bool down = state == GLUT_DOWN ? true : false;

	if (button == GLUT_LEFT_BUTTON)
		mouseLeft = down;
	else if (button == GLUT_RIGHT_BUTTON)
		mouseRight = down;
}

void mouseMotionFunc(int x, int y)
{
	mouseX = x;
	mouseY = y;
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("GlobeDemo by Juha Pellinen 2012");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
	glutKeyboardFunc(keyPressed);
	glutMouseFunc(mouseFunc);
	glutMotionFunc(mouseMotionFunc);
	glutPassiveMotionFunc(mouseMotionFunc);
#ifdef _WIN32
	// init extensions.
	try
	{
		initExt();
	}
	catch (ExtException &e)
	{
		MessageBoxA(0, e.getDesc(), 0, 0);
		return EXIT_FAILURE;
	}
#endif
	// init program.
	if (!init())
	{
		return EXIT_FAILURE;
	}    
    glutMainLoop();
	
    return EXIT_SUCCESS;
}