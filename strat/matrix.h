//
//  matrix.h
//  strat
//
//  Created by Anders Riggelsen on 1/27/13.
//  Copyright (c) 2013 strat. All rights reserved.
//

#ifndef strat_matrix_h
#define strat_matrix_h

typedef struct Vec2f {float x, y;} Vec2f;
struct Vec2f Vec2fMake(float x, float y);
float Vec2fDistanceBetween(const struct Vec2f a, const struct Vec2f b);

typedef struct Mat3f {float a,d,g,  b,e,h,  c,f,i;} Mat3f;
struct Mat3f Mat3fMakeIdentity();
struct Mat3f Mat3fMakeTranslate(float x, float y);
struct Mat3f Mat3fMakeScale(float x, float y);
struct Mat3f Mat3fMakeRotate(float angle);

struct Mat3f Mat3fMultiply(const struct Mat3f a, const struct Mat3f b);
struct Mat3f Mat3fTranslate(const struct Mat3f mat, float x, float y);
struct Mat3f Mat3fScale(const struct Mat3f mat, float sx, float sy);
struct Mat3f Mat3fRotate(const struct Mat3f mat, float angle);
struct Mat3f Mat3fTranspose(const struct Mat3f mat);
struct Mat3f Mat3fInvert(const struct Mat3f mat);
float Mat3fDeterminant(const struct Mat3f mat);
struct Vec2f Mat3fTransformVector(const struct Mat3f mat, const struct Vec2f point);
struct Vec2f Mat3fTransformPoint(const struct Mat3f mat, float x, float y);


#endif
