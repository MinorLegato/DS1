#ifndef __ATS_H__
#define __ATS_H__

#include <math.h>
#include <stdint.h>

#define STRUCT(NAME) typedef struct NAME NAME; struct NAME

#define FOR_MATRIX(X, Y) \
    for (i32 y = 0; y < Y; y++) for (i32 x = 0; x < X; x++)

typedef int32_t b32;

typedef float r32;
typedef double r64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

#define randi(min, max) (rand() % ((max) - (min)) + (min))
#define randf(min, max) ((rand() / (r32)RAND_MAX) * fabs((r32)(max) - (r32)(min)) + (r32)(min))

#define Array(T, N) struct { i32 size; T data[N]; }

#define pushBack(arr, e)    do { (arr)->data[(arr)->size++] = (e); } while (0);
#define eraseElem(arr, i)   do { (arr)->data[(i)] = (arr)->data[--(arr)->size]; } while (0);
#define clearArray(arr)     do { (arr)->size = 0; } while (0);
#define setElem(arr, i, e)  do { (arr)->data[i] = e; } while (0);

#define getElem(arr, i)     ((arr)->data[(i)])
#define getSize(arr)        ((arr)->size)

#define forEach(INDEX, ARRAY) for (i32 INDEX = 0; INDEX < getSize(ARRAY); INDEX++)

#define PI (3.14159265359f)

#define toRad(deg) ((deg) * ((PI) / 180.0f))
#define toDeg(rad) ((rad) * (180.0f / (PI)))

// from Quake 3 Arena
static inline r32 rsqrt(r32 number) {
	u32 i;
	r32 x2, y;
	const r32 threehalfs = 1.5f;

	x2 = number * 0.5F;
	y  = number;
    i = (union { r32 f; u32 l; }) { y }.l; // evil floating point bit level hacking
	i  = 0x5f3759df - (i >> 1);            // what the fuck? 
    y = (union { u32 l; r32 f; }) { i }.f;
	y  = y * (threehalfs - (x2 * y * y));  // 1st iteration

	return y;
}

STRUCT(v2) {
    r32 x;
    r32 y;
};

static inline v2 V2(r32 x, r32 y)
{
    v2 v = { x, y }; return v;
}

static inline v2 v2Add(v2 a, v2 b) {
    a.x += b.x;
    a.y += b.y;
    return a;
}

static inline v2 v2Sub(v2 a, v2 b) {
    a.x -= b.x;
    a.y -= b.y;
    return a;
}

static inline v2 v2Mul(v2 a, r32 s) {
    a.x *= s;
    a.y *= s;
    return a;
}

static inline v2 v2Div(v2 a, r32 s) {
    a.x /= s;
    a.y /= s;
    return a;
}

static inline r32 len2(v2 a) {
    return a.x * a.x + a.y * a.y;
}

static inline r32 len(v2 a) {
    return sqrtf(len2(a));
}

static inline r32 revLen(v2 a) {
    return rsqrt(len2(a));
}

static inline v2 norm(v2 a) {
    r32 l = len(a);
    return V2(a.x / l, a.y / l);
}

static inline r32 dot(v2 a, v2 b) {
    return a.x * b.x + a.y * b.y;
}

static inline r32 det(v2 a, v2 b) {
    return a.x * b.y - b.x * a.y;
}

static inline r32 dist(v2 a, v2 b) {
    v2 c = { a.x - b.x, a.y - b.y };
    return len(c);
}

static inline r32 dist2(v2 a, v2 b) {
    v2 c = { a.x - b.x, a.y - b.y };
    return len2(c);
}

static inline v2 v2Dir(v2 from, v2 to) {
    v2 dir = { to.x - from.x, to.y - from.y };
    r32 rlen = revLen(dir);
    dir.x *= rlen;
    dir.y *= rlen;
    return dir;
}

static inline v2 v2Round(v2 a) {
    return V2(roundf(a.x), roundf(a.y));
}

static inline v2 rotateRad(v2 v, r32 rad) {
    return V2(
        v.x * cosf(rad) - v.y * sinf(rad),
        v.x * sinf(rad) + v.y * cosf(rad));
}

static inline v2 rotateDeg(v2 v, r32 deg) {
    r32 rad = toRad(deg);
    return V2(
        v.x * cosf(rad) - v.y * sinf(rad),
        v.x * sinf(rad) + v.y * cosf(rad));
}

STRUCT(AABB) {
    v2 c;
    v2 r;
};

static inline AABB makeAABB(r32 x, r32 y, r32 w, r32 h) {
    w = 0.5f * w;
    h = 0.5f * h;
    x += w;
    y += h;

    AABB ret = { { x, y }, { w, h } };
    return ret;
}

static inline b32 intersects(AABB a, AABB b) {
    if (fabs(a.c.x - b.c.x) > (a.r.x + b.r.x)) return 0;
    if (fabs(a.c.y - b.c.y) > (a.r.y + b.r.y)) return 0;
    return 1;
}

static inline b32 contains(AABB a, v2 p) {
    if ((p.x < a.c.x - a.r.x) || (p.x > a.c.x + a.r.x)) return 0;
    if ((p.y < a.c.y - a.r.y) || (p.y > a.c.y + a.r.y)) return 0;
    return 1;
}

// modified version of stb_perlin.h from nothings stb library
static const u8 stb__perlin_randtab[512] = {
    23, 125, 161, 52, 103, 117, 70, 37, 247, 101, 203, 169, 124, 126, 44, 123, 
    152, 238, 145, 45, 171, 114, 253, 10, 192, 136, 4, 157, 249, 30, 35, 72, 
    175, 63, 77, 90, 181, 16, 96, 111, 133, 104, 75, 162, 93, 56, 66, 240, 
    8, 50, 84, 229, 49, 210, 173, 239, 141, 1, 87, 18, 2, 198, 143, 57, 
    225, 160, 58, 217, 168, 206, 245, 204, 199, 6, 73, 60, 20, 230, 211, 233, 
    94, 200, 88, 9, 74, 155, 33, 15, 219, 130, 226, 202, 83, 236, 42, 172, 
    165, 218, 55, 222, 46, 107, 98, 154, 109, 67, 196, 178, 127, 158, 13, 243, 
    65, 79, 166, 248, 25, 224, 115, 80, 68, 51, 184, 128, 232, 208, 151, 122, 
    26, 212, 105, 43, 179, 213, 235, 148, 146, 89, 14, 195, 28, 78, 112, 76, 
    250, 47, 24, 251, 140, 108, 186, 190, 228, 170, 183, 139, 39, 188, 244, 246, 
    132, 48, 119, 144, 180, 138, 134, 193, 82, 182, 120, 121, 86, 220, 209, 3, 
    91, 241, 149, 85, 205, 150, 113, 216, 31, 100, 41, 164, 177, 214, 153, 231, 
    38, 71, 185, 174, 97, 201, 29, 95, 7, 92, 54, 254, 191, 118, 34, 221, 
    131, 11, 163, 99, 234, 81, 227, 147, 156, 176, 17, 142, 69, 12, 110, 62, 
    27, 255, 0, 194, 59, 116, 242, 252, 19, 21, 187, 53, 207, 129, 64, 135, 
    61, 40, 167, 237, 102, 223, 106, 159, 197, 189, 215, 137, 36, 32, 22, 5,  
    
    // and a second copy so we don't need an extra mask or static initializer
    23, 125, 161, 52, 103, 117, 70, 37, 247, 101, 203, 169, 124, 126, 44, 123, 
    152, 238, 145, 45, 171, 114, 253, 10, 192, 136, 4, 157, 249, 30, 35, 72, 
    175, 63, 77, 90, 181, 16, 96, 111, 133, 104, 75, 162, 93, 56, 66, 240, 
    8, 50, 84, 229, 49, 210, 173, 239, 141, 1, 87, 18, 2, 198, 143, 57, 
    225, 160, 58, 217, 168, 206, 245, 204, 199, 6, 73, 60, 20, 230, 211, 233, 
    94, 200, 88, 9, 74, 155, 33, 15, 219, 130, 226, 202, 83, 236, 42, 172, 
    165, 218, 55, 222, 46, 107, 98, 154, 109, 67, 196, 178, 127, 158, 13, 243, 
    65, 79, 166, 248, 25, 224, 115, 80, 68, 51, 184, 128, 232, 208, 151, 122, 
    26, 212, 105, 43, 179, 213, 235, 148, 146, 89, 14, 195, 28, 78, 112, 76, 
    250, 47, 24, 251, 140, 108, 186, 190, 228, 170, 183, 139, 39, 188, 244, 246, 
    132, 48, 119, 144, 180, 138, 134, 193, 82, 182, 120, 121, 86, 220, 209, 3, 
    91, 241, 149, 85, 205, 150, 113, 216, 31, 100, 41, 164, 177, 214, 153, 231, 
    38, 71, 185, 174, 97, 201, 29, 95, 7, 92, 54, 254, 191, 118, 34, 221, 
    131, 11, 163, 99, 234, 81, 227, 147, 156, 176, 17, 142, 69, 12, 110, 62, 
    27, 255, 0, 194, 59, 116, 242, 252, 19, 21, 187, 53, 207, 129, 64, 135, 
    61, 40, 167, 237, 102, 223, 106, 159, 197, 189, 215, 137, 36, 32, 22, 5,  
};

static inline r32 stb__perlin_lerp(r32 a, r32 b, r32 t) {
    return a + (b - a) * t;
}

// different grad function from Perlin's, but easy to modify to match reference
static inline r32 stb__perlin_grad(i32 hash, r32 x, r32 y, r32 z) {
    static const r32 basis[12][4] = {
        {  1, 1, 0 },
        { -1, 1, 0 },
        {  1,-1, 0 },
        { -1,-1, 0 },
        {  1, 0, 1 },
        { -1, 0, 1 },
        {  1, 0,-1 },
        { -1, 0,-1 },
        {  0, 1, 1 },
        {  0,-1, 1 },
        {  0, 1,-1 },
        {  0,-1,-1 },
    };
    
    // perlin's gradient has 12 cases so some get used 1/16th of the time
    // and some 2/16ths. We reduce bias by changing those fractions
    // to 5/16ths and 6/16ths, and the same 4 cases get the extra weight.
    static const u8 indices[64] = {
        0,1,2,3,4,5,6,7,8,9,10,11,
        0,9,1,11,
        0,1,2,3,4,5,6,7,8,9,10,11,
        0,1,2,3,4,5,6,7,8,9,10,11,
        0,1,2,3,4,5,6,7,8,9,10,11,
        0,1,2,3,4,5,6,7,8,9,10,11,
    };
    
    // if you use reference permutation table, change 63 below to 15 to match reference
    
    //float *grad = basis[indices[hash & 63]];
    
    //return grad[0] * x + grad[1] * y + grad[2] * z;
    return basis[indices[hash & 63]][0] * x +
        basis[indices[hash & 63]][1] * y +
        basis[indices[hash & 63]][2] * z;
}

static inline r32 cfloor(r32 n) {
    return n >= 0? (i32)(n) : (i32)(n - 1.0f);
}

static inline r32 perlinNoise(r32 x, r32 y, r32 z) {
    static const i32 x_wrap = 0;
    static const i32 y_wrap = 0;
    static const i32 z_wrap = 0;
    
    const u32 x_mask = (x_wrap-1) & 255;
    const u32 y_mask = (y_wrap-1) & 255;
    const u32 z_mask = (z_wrap-1) & 255;
    
    const i32 px = (i32)cfloor(x);
    const i32 py = (i32)cfloor(y);
    const i32 pz = (i32)cfloor(z);
    
    x -= px;
    y -= py;
    z -= pz;
    
    i32 x0 = px & x_mask, x1 = (px+1) & x_mask;
    i32 y0 = py & y_mask, y1 = (py+1) & y_mask;
    i32 z0 = pz & z_mask, z1 = (pz+1) & z_mask;
    
#define stb__perlin_ease(a)   (((a*6-15)*a + 10) * a * a * a)
    
    const r32 u = stb__perlin_ease(x);
    const r32 v = stb__perlin_ease(y);
    const r32 w = stb__perlin_ease(z);
    
    const i32 r0 = stb__perlin_randtab[x0];
    const i32 r1 = stb__perlin_randtab[x1];
    
    const i32 r00 = stb__perlin_randtab[r0+y0];
    const i32 r01 = stb__perlin_randtab[r0+y1];
    const i32 r10 = stb__perlin_randtab[r1+y0];
    const i32 r11 = stb__perlin_randtab[r1+y1];
    
    const r32 n000 = stb__perlin_grad(stb__perlin_randtab[r00+z0], x  , y  , z   );
    const r32 n001 = stb__perlin_grad(stb__perlin_randtab[r00+z1], x  , y  , z-1 );
    const r32 n010 = stb__perlin_grad(stb__perlin_randtab[r01+z0], x  , y-1, z   );
    const r32 n011 = stb__perlin_grad(stb__perlin_randtab[r01+z1], x  , y-1, z-1 );
    const r32 n100 = stb__perlin_grad(stb__perlin_randtab[r10+z0], x-1, y  , z   );
    const r32 n101 = stb__perlin_grad(stb__perlin_randtab[r10+z1], x-1, y  , z-1 );
    const r32 n110 = stb__perlin_grad(stb__perlin_randtab[r11+z0], x-1, y-1, z   );
    const r32 n111 = stb__perlin_grad(stb__perlin_randtab[r11+z1], x-1, y-1, z-1 );
    
    const r32 n00 = stb__perlin_lerp(n000,n001,w);
    const r32 n01 = stb__perlin_lerp(n010,n011,w);
    const r32 n10 = stb__perlin_lerp(n100,n101,w);
    const r32 n11 = stb__perlin_lerp(n110,n111,w);
    
    const r32 n0 = stb__perlin_lerp(n00,n01,v);
    const r32 n1 = stb__perlin_lerp(n10,n11,v);
    
    return stb__perlin_lerp(n0,n1,u);
}

#endif

