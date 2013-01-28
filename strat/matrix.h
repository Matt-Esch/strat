//
//  matrix.h
//  strat
//
//  Created by Anders Riggelsen on 1/27/13.
//  Copyright (c) 2013 strat. All rights reserved.
//

#ifndef strat_matrix_h
#define strat_matrix_h

typedef struct vec2f {float x, y;} vec2f;
struct vec2f vec2fMake(float x, float y);

typedef struct vec2i {int x, y;} vec2i;
struct vec2i vec2iMake(int x, int y);

float vec2fDistanceBetween(const struct vec2f a, const struct vec2f b);

typedef struct mat3f {float a,d,g,  b,e,h,  c,f,i;} mat3f;
struct mat3f mat3fMakeIdentity();
struct mat3f mat3fMakeTranslate(float x, float y);
struct mat3f mat3fMakeScale(float x, float y);
struct mat3f mat3fMakeRotate(float angle);

struct mat3f mat3fMultiply(const struct mat3f a, const struct mat3f b);
struct mat3f mat3fTranslate(const struct mat3f mat, float x, float y);
struct mat3f mat3fScale(const struct mat3f mat, float sx, float sy);
struct mat3f mat3fRotate(const struct mat3f mat, float angle);
struct mat3f mat3fTranspose(const struct mat3f mat);
struct mat3f mat3fInvert(const struct mat3f mat);
float mat3fDeterminant(const struct mat3f mat);
struct vec2f mat3fTransformVector(const struct mat3f mat, const struct vec2f point);
struct vec2f mat3fTransformPoint(const struct mat3f mat, float x, float y);


#endif
