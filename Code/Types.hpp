#pragma once

#include <cstdint>


typedef unsigned int uint;
typedef float        f32;
typedef double       f64;

typedef int8_t       s8;
typedef uint8_t      u8;
typedef int16_t      s16;
typedef uint16_t     u16;
typedef int32_t      s32;
typedef uint32_t     u32;
typedef int64_t      s64;
typedef uint64_t     u64;


union float2
{
    f32 e[2];
    struct { f32 x; f32 y;};
    struct { f32 u; f32 v;};
};

union float3
{
    struct { float2 xy; f32 _z;};
    f32 e[3];
    struct { f32 r; f32 g; f32 b; };
    struct { f32 x; f32 y; f32 z; };
    struct { f32 pitch; f32 roll; f32 yaw;};
};

union float4
{
    float4() = default;
    float4(f32 _x, f32 _y, f32 _z, f32 _w)
        : x(_x), y(_y), z(_z), w(_w)
    {}

    float4 (float3 xyz, f32 w)
        : x(xyz.x), y(xyz.y), z(xyz.z), w(w)
    {}
    float4 (float2 xy, f32 _z, f32 _w)
        : x(xy.x), y(xy.y), z(_z), w(_w)
    {}
    f32 e[4];
    struct { f32 x; f32 y; f32 z; f32 w; };
    struct { f32 r; f32 g; f32 b; f32 a; };
    float3 xyz;
    float3 rgb;
    float2 xy;
};

union float16
{
    f32 e[16];
};

struct v4x4
{
    float4 x;
    float4 y;
    float4 z;
    float4 w;
};

union m4x4
{
    m4x4() = default;
    f32 e[16];
    f32 em[4][4];
    float4 c[4];
};