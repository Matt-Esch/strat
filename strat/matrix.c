//
//  matrix.c
//  strat
//
//  Created by Anders Riggelsen on 1/27/13.
//  Copyright (c) 2013 strat. All rights reserved.
//

#include "matrix.h"
#include <math.h>

struct vec2f vec2fMake(float x, float y)
{
	vec2f ret;
	ret.x = x;
	ret.y = y;
	return ret;
}

struct vec2i vec2iMake(int x, int y)
{
	vec2i ret;
	ret.x = x;
	ret.y = y;
	return ret;
}

float vec2fDistanceBetween(const struct vec2f a, const struct vec2f b)
{
	float dx = a.x - b.x;
	float dy = a.y - b.y;
	return sqrt(dx*dx+dy*dy);
}

struct mat3f mat3fMakeIdentity()
{
	mat3f ret = {0};
	ret.a = ret.e = ret.i = 1;
	return ret;
}

struct mat3f mat3fMakeTranslate(float x, float y)
{
	mat3f ret = {0};
	ret.a = ret.e = ret.i = 1;
	ret.c = x;
	ret.f = y;
	return ret;
}

struct mat3f mat3fMakeScale(float x, float y)
{
	mat3f ret = {0};
	ret.a = x;
	ret.e = y;
	ret.i = 1;
	return ret;
}

struct mat3f mat3fMakeRotate(float angle)
{
	float sina = sin(angle);
	float cosa = cos(angle);
	mat3f ret = {0};
	ret.a = cosa;
	ret.b = -sina;
	ret.d = sina;
	ret.e = cosa;
	ret.i = 1;
	return ret;
}

struct mat3f mat3fMultiply(const struct mat3f a, const struct mat3f b)
{
	mat3f r;
	r.a = a.a*b.a + a.b*b.d + a.c*b.g;
	r.b = a.a*b.b + a.b*b.e + a.c*b.h;
	r.c = a.a*b.c + a.b*b.f + a.c*b.i;
	r.d = a.d*b.a + a.e*b.d + a.f*b.g;
	r.e = a.d*b.b + a.e*b.e + a.f*b.h;
	r.f = a.d*b.c + a.e*b.f + a.f*b.i;
	r.g = a.g*b.a + a.h*b.d + a.i*b.g;
	r.h = a.g*b.b + a.h*b.e + a.i*b.h;
	r.i = a.g*b.c + a.h*b.f + a.i*b.i;
	return r;
}

struct mat3f mat3fTranslate(const struct mat3f mat, float x, float y)
{
	mat3f t = mat3fMakeTranslate(x,y);
	return mat3fMultiply(t, mat);
}

struct mat3f mat3fScale(const struct mat3f mat, float sx, float sy)
{
	mat3f t = mat3fMakeScale(sx,sy);
	return mat3fMultiply(t, mat);
}

struct mat3f mat3fRotate(const struct mat3f mat, float angle)
{
	mat3f t = mat3fMakeRotate(angle);
	return mat3fMultiply(t, mat);
}

struct mat3f mat3fTranspose(const struct mat3f m)
{
	mat3f r;
	r.a = m.a;
	r.b = m.d;
	r.c = m.g;
	r.d = m.b;
	r.e = m.e;
	r.f = m.h;
	r.g = m.c;
	r.h = m.f;
	r.i = m.i;
	return r;
}

float mat3fDeterminant(const struct mat3f mat)
{
	return	mat.a * mat.e * mat.i +
            mat.b * mat.f * mat.g +
            mat.c * mat.d * mat.h -
            mat.c * mat.e * mat.g -
            mat.b * mat.d * mat.i -
            mat.a * mat.f * mat.h;
}

struct mat3f mat3fInvert(const struct mat3f mat)
{
	float invD = 1.0f/mat3fDeterminant(mat);
	mat3f tr;
	tr.a = invD * (mat.e*mat.i - mat.f*mat.h);
	tr.b = invD * (mat.c*mat.h - mat.b*mat.i);
	tr.c = invD * (mat.b*mat.f - mat.c*mat.e);
	tr.d = invD * (mat.f*mat.g - mat.d*mat.i);
	tr.e = invD * (mat.a*mat.i - mat.c*mat.g);
	tr.f = invD * (mat.c*mat.d - mat.a*mat.f);
	tr.g = invD * (mat.d*mat.h - mat.e*mat.g);
	tr.h = invD * (mat.g*mat.b - mat.a*mat.h);
	tr.i = invD * (mat.a*mat.e - mat.b*mat.d);
	return tr;
}

struct vec2f mat3fTransformVector(const struct mat3f mat, const struct vec2f point)
{
	vec2f r;
	r.x = mat.a*point.x + mat.b*point.y + mat.c;
	r.y = mat.d*point.x + mat.e*point.y + mat.f;
	return r;
}

struct vec2f mat3fTransformPoint(const struct mat3f mat, float x, float y)
{
	vec2f r;
	r.x = mat.a*x + mat.b*y + mat.c;
	r.y = mat.d*x + mat.e*y + mat.f;
	return r;
}
