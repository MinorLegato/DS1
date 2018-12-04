#ifndef __ATS_H__
#define __ATS_H__

#include <math.h>
#include <stdint.h>

#define STRUCT(NAME) typedef struct NAME NAME; struct NAME

#define FOR_MATRIX(X, Y) for (int32_t y = 0; y < Y; y++) for (int32_t x = 0; x < X; x++)

#define randi(min, max) (rand() % ((max) - (min)) + (min))
#define randf(min, max) ((rand() / (float)RAND_MAX) * fabs((float)(max) - (float)(min)) + (float)(min))

#define Array(T, N) struct { int32_t size; T data[N]; }

#define array_add(arr, e)     do { (arr)->data[(arr)->size++] = (e); } while (0);
#define array_rem(arr, i)     do { (arr)->data[(i)] = (arr)->data[--(arr)->size]; } while (0);
#define array_clear(arr)      do { (arr)->size = 0; } while (0);
#define array_set(arr, i, e)  do { (arr)->data[i] = e; } while (0);

#define array_get(arr, i) ((arr)->data[(i)])
#define array_size(arr)   ((arr)->size)

#define PI (3.14159265359f)

#define TO_RAD(deg) ((deg) * ((PI) / 180.0f))
#define TO_DEG(rad) ((rad) * (180.0f / (PI)))

// from Quake 3 Arena
static inline float rsqrt(float number) {
	uint32_t i;
	float x2, y;
	const float threehalfs = 1.5f;

	x2 = number * 0.5F;
	y  = number;
    i = (union { float f; uint32_t l; }) { y }.l;  // evil floating point bit level hacking
	i  = 0x5f3759df - (i >> 1);             // what the fuck? 
    y = (union { uint32_t l; float f; }) { i }.f;
	y  = y * (threehalfs - (x2 * y * y));   // 1st iteration

	return y;
}

// modified version of stb_perlin.h from nothings stb library
static const uint8_t stb__perlin_randtab[512] = {
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

static inline float stb__perlin_lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

// different grad function from Perlin's, but easy to modify to match reference
static inline float stb__perlin_grad(int32_t hash, float x, float y, float z) {
    static const float basis[12][4] = {
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
    static const uint8_t indices[64] = {
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

static inline float cfloor(float n) {
    return n >= 0? (int32_t)(n) : (int32_t)(n - 1.0f);
}

static inline float perlinNoise(float x, float y, float z) {
    static const int32_t x_wrap = 0;
    static const int32_t y_wrap = 0;
    static const int32_t z_wrap = 0;
    
    const uint32_t x_mask = (x_wrap-1) & 255;
    const uint32_t y_mask = (y_wrap-1) & 255;
    const uint32_t z_mask = (z_wrap-1) & 255;
    
    const int32_t px = (int32_t)cfloor(x);
    const int32_t py = (int32_t)cfloor(y);
    const int32_t pz = (int32_t)cfloor(z);
    
    x -= px;
    y -= py;
    z -= pz;
    
    int32_t x0 = px & x_mask, x1 = (px+1) & x_mask;
    int32_t y0 = py & y_mask, y1 = (py+1) & y_mask;
    int32_t z0 = pz & z_mask, z1 = (pz+1) & z_mask;
    
#define stb__perlin_ease(a)   (((a*6-15)*a + 10) * a * a * a)
    
    const float u = stb__perlin_ease(x);
    const float v = stb__perlin_ease(y);
    const float w = stb__perlin_ease(z);
    
    const int32_t r0 = stb__perlin_randtab[x0];
    const int32_t r1 = stb__perlin_randtab[x1];
    
    const int32_t r00 = stb__perlin_randtab[r0+y0];
    const int32_t r01 = stb__perlin_randtab[r0+y1];
    const int32_t r10 = stb__perlin_randtab[r1+y0];
    const int32_t r11 = stb__perlin_randtab[r1+y1];
    
    const float n000 = stb__perlin_grad(stb__perlin_randtab[r00+z0], x  , y  , z   );
    const float n001 = stb__perlin_grad(stb__perlin_randtab[r00+z1], x  , y  , z-1 );
    const float n010 = stb__perlin_grad(stb__perlin_randtab[r01+z0], x  , y-1, z   );
    const float n011 = stb__perlin_grad(stb__perlin_randtab[r01+z1], x  , y-1, z-1 );
    const float n100 = stb__perlin_grad(stb__perlin_randtab[r10+z0], x-1, y  , z   );
    const float n101 = stb__perlin_grad(stb__perlin_randtab[r10+z1], x-1, y  , z-1 );
    const float n110 = stb__perlin_grad(stb__perlin_randtab[r11+z0], x-1, y-1, z   );
    const float n111 = stb__perlin_grad(stb__perlin_randtab[r11+z1], x-1, y-1, z-1 );
    
    const float n00 = stb__perlin_lerp(n000,n001,w);
    const float n01 = stb__perlin_lerp(n010,n011,w);
    const float n10 = stb__perlin_lerp(n100,n101,w);
    const float n11 = stb__perlin_lerp(n110,n111,w);
    
    const float n0 = stb__perlin_lerp(n00,n01,v);
    const float n1 = stb__perlin_lerp(n10,n11,v);
    
    return stb__perlin_lerp(n0,n1,u);
}

#endif

