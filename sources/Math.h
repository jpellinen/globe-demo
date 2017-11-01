//
// Math.h
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

#ifndef _MATH_H_
#define _MATH_H_

#include <math.h>
#include <string.h>

#define PI 3.14159f

#ifndef min
template<typename T> static T min(T a, T b) { return a < b ? a : b; }
#endif
#ifndef max
template<typename T> static T max(T a, T b) { return a > b ? a : b; }
#endif

struct Mat4;

struct Vec3
{
	Vec3();
	Vec3(float x, float y, float z);

	Vec3 operator +(const Vec3 &o) const;
	Vec3 operator -(const Vec3 &o) const;
	Vec3 operator *(float k) const;

	operator float *();
	operator const float *() const;

	union
	{
		struct { float x, y, z; };
		float v[3];
	};
};

void Vec3Normalize(Vec3 &dst, const Vec3 &src);
float Vec3Dot(const Vec3 &src1, const Vec3 &src2);
void Vec3Cross(Vec3 &dst, const Vec3 &src1, const Vec3 &src2);
void Vec3Transform(Vec3 &dst, const Vec3 &src, const Mat4 &m);

struct Mat4
{
	Mat4();

	Mat4 operator *(const Mat4 &o) const;
	Mat4 &operator *=(const Mat4 &o);

	operator float *();
	operator const float *() const;

	float m[4][4];
};

void Mat4Identity(Mat4 &m);
void Mat4Translate(Mat4 &m, float x, float y, float z);
void Mat4RotationX(Mat4 &m, float angle);
void Mat4RotationY(Mat4 &m, float angle);
void Mat4RotationZ(Mat4 &m, float angle);
void Mat4Scaling(Mat4 &m, float x, float y, float z);
void Mat4LookAt(Mat4 &m, const Vec3 &eye, const Vec3 &center, const Vec3 &up);
void Mat4Perspective(Mat4 &m, float fovy, float aspect, float zNear, float zFar);
void Mat4Ortho(Mat4 &m, float left, float right, float bottom, float top, float zNear, float zFar);
void Mat4Inverse(Mat4 &dst, float *determinant, const Mat4 &src);

struct RayHitInfo
{
	bool hit;
	float dist;
};

RayHitInfo raySphereIntersection(const Vec3 &rayPos, const Vec3 &rayDir, const Vec3 &spherePos, float radius);

#include "Math.inl"

#endif /* _MATH_H_ */