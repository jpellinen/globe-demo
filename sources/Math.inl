//
// Math.inl
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

inline Vec3::Vec3()
{
}

inline Vec3::Vec3(float x, float y, float z) :
	x(x), y(y), z(z)
{
}

inline Vec3 Vec3::operator +(const Vec3 &o) const
{
	return Vec3(x+o.x, y+o.y, z+o.z);
}

inline Vec3 Vec3::operator -(const Vec3 &o) const
{
	return Vec3(x-o.x, y-o.y, z-o.z);
}

inline Vec3 Vec3::operator *(float k) const
{
	return Vec3(x*k, y*k, z*k);
}

inline Vec3::operator float *()
{
	return v;
}

inline Vec3::operator const float *() const
{
	return v;
}

inline void Vec3Normalize(Vec3 &dst, const Vec3 &src)
{
	dst = src * (1.0f / (sqrtf(src.x*src.x + src.y*src.y + src.z*src.z)));
}

inline float Vec3Dot(const Vec3 &src1, const Vec3 &src2)
{
	return src1.x*src2.x + src1.y*src2.y + src1.z*src2.z;
}

inline void Vec3Cross(Vec3 &dst, const Vec3 &src1, const Vec3 &src2)
{
	dst.x = src1.y*src2.z - src1.z*src2.y;
	dst.y = src1.z*src2.x - src1.x*src2.z;
	dst.z = src1.x*src2.y - src1.y*src2.x;
}

inline void Vec3Transform(Vec3 &dst, const Vec3 &src, const Mat4 &m)
{
	float tmp[4];
	
	for (int i = 0; i < 4; ++i)
	{
		tmp[i] = m[i]   * src.x +
				 m[4+i] * src.y +
				 m[8+i] * src.z +
				 m[12+i];
	}
	
	float invW = 1.0f / tmp[3];
	
	dst.x = tmp[0] * invW;
	dst.y = tmp[1] * invW;
	dst.z = tmp[2] * invW;
}

inline Mat4::Mat4()
{
}

inline Mat4 Mat4::operator*(const Mat4 &o) const
{
	Mat4 tmp;

	for (unsigned int i = 0; i < 4; ++i)
	{
		for (unsigned int j = 0; j < 4; ++j)
		{
			tmp.m[i][j] = m[i][0] * o.m[0][j] +
							m[i][1] * o.m[1][j] +
							m[i][2] * o.m[2][j] +
							m[i][3] * o.m[3][j];
		}
	}
	return tmp;
}

inline Mat4 &Mat4::operator*=(const Mat4 &o)
{
	return *this = *this * o;
}

inline Mat4::operator float *()
{
	return &m[0][0];
}

inline Mat4::operator const float *() const
{
	return &m[0][0];
}

inline void Mat4Identity(Mat4 &m)
{
	static const float IDENTITY[] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
	memcpy(m, IDENTITY, sizeof(IDENTITY));
}

inline void Mat4Translate(Mat4 &m, float x, float y, float z)
{
	Mat4Identity(m);
	m[12] = x;
	m[13] = y;
	m[14] = z;
}

inline void Mat4RotationX(Mat4 &m, float angle)
{
	float sin = sinf(angle);
	float cos = cosf(angle);

    m[0] = 1;  m[1] = 0;    m[2] = 0;    m[3] = 0;
    m[4] = 0;  m[5] = cos;  m[6] = sin;  m[7] = 0;
	m[8] = 0;  m[9] = -sin; m[10] = cos; m[11] = 0;
    m[12] = 0; m[13] = 0;   m[14] = 0;   m[15] = 1;
}

inline void Mat4RotationY(Mat4 &m, float angle)
{
	float sin = sinf(angle);
	float cos = cosf(angle);

    m[0] = cos; m[1] = 0;  m[2] = -sin; m[3] = 0;
    m[4] = 0;   m[5] = 1;  m[6] = 0;    m[7] = 0;
	m[8] = sin; m[9] = 0;  m[10] = cos; m[11] = 0;
    m[12] = 0;  m[13] = 0; m[14] = 0;   m[15] = 1;
}

inline void Mat4RotationZ(Mat4 &m, float angle)
{
	float sin = sinf(angle);
	float cos = cosf(angle);

    m[0] = cos;  m[1] = sin; m[2] = 0;  m[3] = 0;
    m[4] = -sin; m[5] = cos; m[6] = 0;  m[7] = 0;
	m[8] = 0;    m[9] = 0;   m[10] = 1; m[11] = 0;
    m[12] = 0;   m[13] = 0;  m[14] = 0; m[15] = 1;
}

inline void Mat4Scaling(Mat4 &m, float x, float y, float z)
{
    m[0] = x;  m[1] = 0; m[2] = 0; m[3] = 0;
    m[4] = 0;  m[5] = y; m[6] = 0; m[7] = 0;
	m[8] = 0;  m[9] = 0; m[10]= z; m[11]= 0;
    m[12]= 0;  m[13]= 0; m[14]= 0; m[15]= 1;
}

inline void Mat4LookAt(Mat4 &m, const Vec3 &eye, const Vec3 &center, const Vec3 &up)
{
	Vec3 f;
	Vec3Normalize(f, center - eye);
	Vec3 s;
	Vec3Cross(s, f, up);
	Vec3Normalize(s, s);
	Vec3 u;
	Vec3Cross(u, s, f);
	
	m[0] = s.x; m[1] = u.x;	m[2] =-f.x;	m[3] = 0;
	m[4] = s.y;	m[5] = u.y;	m[6] =-f.y;	m[7] = 0;
	m[8] = s.z;	m[9] = u.z;	m[10]=-f.z;	m[11]= 0;
	m[12]= 0;	m[13]= 0;	m[14]= 0;	m[15]= 1;

	Mat4 mt;
	Mat4Translate(mt,-eye.x,-eye.y,-eye.z);
	m = mt * m;
}

inline void Mat4Perspective(Mat4 &m, float fovy, float aspect, float zNear, float zFar)
{
	float rad = (fovy * 0.5f) * PI / 180.0f;
	float cotan = cosf(rad) / sinf(rad);
	float dz = 1.0f / (zNear - zFar);

	Mat4Identity(m);
	m[0] = cotan / aspect;
	m[5] = cotan;
	m[10] = (zFar + zNear) * dz;
	m[11] = -1;
	m[14] = (2 * zFar * zNear) * dz;
	m[15] = 0;
}

inline void Mat4Ortho(Mat4 &m, float left, float right, float bottom, float top, float zNear, float zFar)
{
	float dx = 1.0f / (right - left);
	float dy = 1.0f / (top - bottom);
	float dz = 1.0f / (zFar - zNear);

	Mat4Identity(m);
	m[0] = 2.0f * dx;
	m[5] = 2.0f * dy;
	m[10] = 2.0f * dz;
	m[12] = -(right + left) * dx;
	m[13] = -(top + bottom) * dy;
	m[14] = -(zFar + zNear) * dz;
}

inline void Mat4Inverse(Mat4 &dst, float *determinant, const Mat4 &src)
{
	dst[0]  =  src[5]*src[10]*src[15] - src[5]*src[11]*src[14] - src[9]*src[6]*src[15] + src[9]*src[7]*src[14] + src[13]*src[6]*src[11] - src[13]*src[7]*src[10];
    dst[4]  = -src[4]*src[10]*src[15] + src[4]*src[11]*src[14] + src[8]*src[6]*src[15] - src[8]*src[7]*src[14] - src[12]*src[6]*src[11] + src[12]*src[7]*src[10];
    dst[8]  =  src[4]*src[9]*src[15]  - src[4]*src[11]*src[13] - src[8]*src[5]*src[15] + src[8]*src[7]*src[13] + src[12]*src[5]*src[11] - src[12]*src[7]*src[9];
    dst[12] = -src[4]*src[9]*src[14]  + src[4]*src[10]*src[13] + src[8]*src[5]*src[14] - src[8]*src[6]*src[13] - src[12]*src[5]*src[10] + src[12]*src[6]*src[9];
    dst[1]  = -src[1]*src[10]*src[15] + src[1]*src[11]*src[14] + src[9]*src[2]*src[15] - src[9]*src[3]*src[14] - src[13]*src[2]*src[11] + src[13]*src[3]*src[10];
    dst[5]  =  src[0]*src[10]*src[15] - src[0]*src[11]*src[14] - src[8]*src[2]*src[15] + src[8]*src[3]*src[14] + src[12]*src[2]*src[11] - src[12]*src[3]*src[10];
    dst[9]  = -src[0]*src[9]*src[15]  + src[0]*src[11]*src[13] + src[8]*src[1]*src[15] - src[8]*src[3]*src[13] - src[12]*src[1]*src[11] + src[12]*src[3]*src[9];
    dst[13] =  src[0]*src[9]*src[14]  - src[0]*src[10]*src[13] - src[8]*src[1]*src[14] + src[8]*src[2]*src[13] + src[12]*src[1]*src[10] - src[12]*src[2]*src[9];
    dst[2]  =  src[1]*src[6]*src[15]  - src[1]*src[7]*src[14]  - src[5]*src[2]*src[15] + src[5]*src[3]*src[14] + src[13]*src[2]*src[7]  - src[13]*src[3]*src[6];
    dst[6]  = -src[0]*src[6]*src[15]  + src[0]*src[7]*src[14]  + src[4]*src[2]*src[15] - src[4]*src[3]*src[14] - src[12]*src[2]*src[7]  + src[12]*src[3]*src[6];
    dst[10] =  src[0]*src[5]*src[15]  - src[0]*src[7]*src[13]  - src[4]*src[1]*src[15] + src[4]*src[3]*src[13] + src[12]*src[1]*src[7]  - src[12]*src[3]*src[5];
    dst[14] = -src[0]*src[5]*src[14]  + src[0]*src[6]*src[13]  + src[4]*src[1]*src[14] - src[4]*src[2]*src[13] - src[12]*src[1]*src[6]  + src[12]*src[2]*src[5];
    dst[3]  = -src[1]*src[6]*src[11]  + src[1]*src[7]*src[10]  + src[5]*src[2]*src[11] - src[5]*src[3]*src[10] - src[9]*src[2]*src[7]   + src[9]*src[3]*src[6];
    dst[7]  =  src[0]*src[6]*src[11]  - src[0]*src[7]*src[10]  - src[4]*src[2]*src[11] + src[4]*src[3]*src[10] + src[8]*src[2]*src[7]   - src[8]*src[3]*src[6];
    dst[11] = -src[0]*src[5]*src[11]  + src[0]*src[7]*src[9]   + src[4]*src[1]*src[11] - src[4]*src[3]*src[9]  - src[8]*src[1]*src[7]   + src[8]*src[3]*src[5];
    dst[15] =  src[0]*src[5]*src[10]  - src[0]*src[6]*src[9]   - src[4]*src[1]*src[10] + src[4]*src[2]*src[9]  + src[8]*src[1]*src[6]   - src[8]*src[2]*src[5];
	
	float det = src[0]*dst[0] + src[1]*dst[4] + src[2]*dst[8] + src[3]*dst[12];

	if (det)
	{
		if (determinant) *determinant = det;
		return;
	}
	
	det = 1.0f / det;
	if (determinant)
		*determinant = det;
	
	for (unsigned int i = 0; i < 16; i++)
		dst[i] *= det;
}

inline RayHitInfo raySphereIntersection(const Vec3 &rayPos, const Vec3 &rayDir, const Vec3 &spherePos, float radius)
{
	Vec3 w = rayPos - spherePos;
	float a = Vec3Dot(rayDir, rayDir);
	float b = 2.0f * Vec3Dot(w, rayDir);
	float c = Vec3Dot(w, w) - radius * radius;
	float d = b*b - 4.0f*a*c;
  
	RayHitInfo hitInfo;
	hitInfo.hit = d >= 0.0f;
	hitInfo.dist = hitInfo.hit ? (-b - sqrtf(d)) / (2.0f * a) : -1.0f;
	return hitInfo;
}
