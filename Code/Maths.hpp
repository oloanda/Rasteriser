#pragma once

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <cstdio>
#include <cmath>

#include "Types.hpp"

// Constant and common maths functions

inline float3 GetSphericalCoords(f32 r, f32 theta, f32 phi)
{
    return {{r * sinf(theta) * cosf(phi),
            r * cosf(theta),
            r * sinf(theta) * sinf(phi)
        }};
}

namespace maths
{
    const f32 TAU = 6.283185307179586476925f;

    inline f32 cos(f32 x) { return cosf(x); }
    inline f32 sin(f32 x) { return sinf(x); }
    inline f32 tan(f32 x) { return tanf(x); }
}


namespace mat4
{
    inline m4x4 identity()
    {
        return {
            1.f, 0.f, 0.f, 0.f,
            0.f, 1.f, 0.f, 0.f,
            0.f, 0.f, 1.f, 0.f,
            0.f, 0.f, 0.f, 1.f,
        };
    }
}

inline m4x4 operator*(const m4x4& a, const m4x4& b)
{
    m4x4 result = {};

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                result.em[i][j] += a.em[i][k] * b.em[k][j];
            }
        }
    }
    return (result);
}

inline float3 operator*(const m4x4& a, float3 v)
{
    float3 result;
    const v4x4 *m = (const v4x4 *)&a;

    result.x = v.x * m->x.x + v.y * m->y.x + v.z * m->z.x + 1 * m->w.x;
    result.y = v.x * m->x.y + v.y * m->y.y + v.z * m->z.y + 1 * m->w.y;
    result.z = v.x * m->x.z + v.y * m->y.z + v.z * m->z.z + 1 * m->w.z;
    return (result);
}

inline float4 operator*(const m4x4& a, float4 v)
{
    float4 result;
    const v4x4 *m = (const v4x4 *)&a;

    result.x = v.x * m->x.x + v.y * m->y.x + v.z * m->z.x + v.w * m->w.x;
    result.y = v.x * m->x.y + v.y * m->y.y + v.z * m->z.y + v.w * m->w.y;
    result.z = v.x * m->x.z + v.y * m->y.z + v.z * m->z.z + v.w * m->w.z;
    result.w = v.x * m->x.w + v.y * m->y.w + v.z * m->z.w + v.w * m->w.w;
    return (result);
}

inline float4 operator+(const float4& v0, const float4& v1)
{
    float4 resVal;

    resVal.x = v0.x + v1.x;
    resVal.y = v0.y + v1.y;
    resVal.z = v0.z + v1.z;
    resVal.w = v0.w + v1.w;
    return resVal;

}

inline float4 operator*(const float4& v, f32 s)
{
    float4 resVal;
    resVal.x = v.x * s;
    resVal.y = v.y * s;
    resVal.z = v.z * s;
    resVal.w = v.w * s;
    return resVal;

}

inline float3 operator+(const float3& v0, const float3& v1)
{
    float3 resVal;

    resVal.x = v0.x + v1.x;
    resVal.y = v0.y + v1.y;
    resVal.z = v0.z + v1.z;
    return resVal;
}

inline float3 operator*(const float3& v, f32 s)
{
    float3 resVal;
    resVal.x = v.x * s;
    resVal.y = v.y * s;
    resVal.z = v.z * s;
    return resVal;

}

inline float3 operator/(float3 v, f32 a)
{
    float3 result = {};

    if (a < 0.001f)
        a = 1;
    result.x = v.x / a;
    result.y = v.y / a;
    result.z = v.z / a;
    return (result);
}

inline float2 operator-(const float2& a, const float2&  b)
{
    float2 resVal;
    resVal.x = a.x - b.x;
    resVal.y = a.y - b.y;
    return resVal;

}

inline float2 operator+(const float2& a,const float2& b)
{
    float2 resVal;
    resVal.x = a.x + b.x;
    resVal.y = a.y + b.y;
    return resVal;

}

inline float2 operator*(const f32& s, const float2& v)
{
    float2 resVal;
    resVal.x = v.x * s;
    resVal.y = v.y * s;
    return resVal;
}

inline float2 operator*(const float2& v, const f32& s)
{
    float2 resVal;
    resVal.x = v.x * s;
    resVal.y = v.y * s;
    return resVal;
}

class Vec3
{
    public:
    union
    {
        float3 v;
        struct {f32 x; f32 y; f32 z;};
        struct {f32 r; f32 g; f32 b;};
        f32 e[3];
        float2 xy;
    };

    f32 GetMagnitude() const 
    {
        return (sqrt(x*x + y*y + z*z));
    }

    static f32 Dot(const Vec3& a, const Vec3& b)
    {
        return ((f32)(a.x * b.x + a.y * b.y + a.z * b.z));
    }

    void Normalize()
    {
        f32 length = this->GetMagnitude();

        if (length == 0.0f)
            length = 1.0;
        x = x / length;
        y = y / length;
        z = z / length;
    }
    Vec3 operator+(const Vec3 a) const
    {
        Vec3 result = {};

        result.x = x + a.x;
        result.y = y + a.y;
        result.z = z + a.z;
        return (result);
    }
    Vec3 operator*(const f32 scale) const
    {
        Vec3 result;

        result.x = x * scale;
        result.y = y * scale;
        result.z = z * scale;
        return (result);
    }

    friend Vec3 operator*(f32 s, const Vec3& v)
    {
        return {v.x * s, v.y * s, v.z * s};
    }

    friend Vec3 operator-(const Vec3& a, const Vec3& b)
    {
        return {a.x - b.x, a.y - b.y, a.z - b.z};
    }

    void operator+=(const Vec3 a)
    {
        this->x = x + a.x;
        this->y = y + a.y;
        this->z = z + a.z;
    }
    void operator*=(const f32 scale)
    {
        this->x = x * scale;
        this->y = y * scale;
        this->z = z * scale;
    }
    void PrintVector3()
    {
        printf("x.%f y.%f z.%f\n", x, y, z);
    }
    //Reflect V about (normalized) N
    static Vec3 Reflect(const Vec3& v, const Vec3& n)
    {
        return v - 2.0f * Vec3::Dot(v,n) * n;
    }
};

class Vec4
{
    public:

    union
    {
        struct { f32 x; f32 y; f32 z; f32 w; };
        struct { f32 r; f32 g; f32 b; f32 a; };
        struct { float3 xyz; f32  _w; } ;
        struct { Vec3  v3; f32 __w; };
        f32 e[4];
        float2 xy;
        //float3 xyz;
        float4 v;
    };
    Vec4()
    { x = 0; y = 0; z = 0; w = 1; }
    Vec4(float4 vi) { v = vi; }
    Vec4(float a, float b, float c, float d)
    { x = a; y = b; z = c; w = d; }
    Vec4(float3 v, float a) { xyz = v; w = a; }

    void Homogenize()
    {
        x /= w;
        y /= w;
        z /= w;
        // We keep the w value for perspective interpolation later
        // We don't devide it by itself, because it becomes 1;
    }
    float GetMagnitude() const
    {float result; result = this->v3.GetMagnitude(); return (result);}

    void Normalize()    { this->v3.Normalize(); }
    void PrintVector4() { printf("x.%f y.%f z.%f w.%f\n", x, y, z, w); }
};

static float3 getNormalFromTriangle(float3 v0, float3 v1, float3 v2)
{
    float3 normal;

    float3 a = {{v1.x - v0.x, v1.y - v0.y, v1.z - v0.z}};
    float3 b = {{v2.x - v0.x, v2.y - v0.y, v2.z - v0.z}};

    normal.x = a.y * b.z - a.z * b.y;
    normal.y = a.z * b.x - a.x * b.z;
    normal.z = a.x * b.y - a.y * b.x;
    return (normal);
}

/*
 0  1  2  3
 4  5  6  7
 8  9  10 11
12 13 14 15
*/

class Mat4
{
    public:

    union
    {
        v4x4 vec;
        float4 c[4];
        f32 e[16];
        f32 em[4][4];
        m4x4  mat;
    };

    static Mat4 CreateIdentityMatrix()
    {
        Mat4 result = {};
        v4x4 *vec = &result.vec;

        vec->x.x = 1;
        vec->y.y = 1;
        vec->z.z = 1;
        vec->w.w = 1;
        return (result);
    };
    static Mat4 CreateTranslationMatrix(const Vec3& translation)
    {
        Mat4 result = CreateIdentityMatrix();;
        v4x4 *vec = &result.vec;

        vec->w.x = translation.x;
        vec->w.y = translation.y;
        vec->w.z = translation.z;
        return (result);
    }

    static Mat4 CreateScaleMatrix(const Vec3& scale)
    {
        Mat4 result = CreateIdentityMatrix();;

        v4x4 *vec = &result.vec;
        vec->x.x = scale.x;
        vec->y.y = scale.y;
        vec->z.z = scale.z;
        return(result);
    };

    static Mat4 CreateXRotationMatrix(f32 angle)
    {
        Mat4 result = CreateIdentityMatrix();
        v4x4 *vec = &result.vec;

        //degree° × π/180 = radians
        angle = angle * M_PI/180;

        vec->y.y =  cos(angle);
        vec->y.z =  sin(angle);
        vec->z.y =  -sin(angle);
        vec->z.z =  cos(angle);
        return (result);
    }

    static Mat4 CreateYRotationMatrix(f32 angle)
    {
        Mat4 result = CreateIdentityMatrix();

        v4x4 *vec = &result.vec;

        angle = angle * M_PI/180;

        vec->x.x =  cos(angle);
        vec->x.z =  -sin(angle);
        vec->z.x =  sin(angle);
        vec->z.z =  cos(angle);
        return (result);
    }

    static Mat4 CreateZRotationMatrix(f32 angle)
    {
        Mat4 result = CreateIdentityMatrix();
        v4x4 *vec = &result.vec;

        angle = angle * M_PI/180;

        vec->x.x =  cos(angle);
        vec->x.y =  sin(angle);
        vec->y.x =  -sin(angle);
        vec->y.y =  cos(angle);
        return (result);
    }

    static Mat4 CreateTransformMatrix(const Vec3& rotation,
                                      const Vec3& position,
                                      const Vec3& scale)
    {
        Mat4 r = {};
        Mat4 TranslationMatrix = {};
        Mat4 RotationYMatrix = {};
        Mat4 RotationXMatrix = {};
        Mat4 RotationZMatrix = {};
        Mat4 ScaleMatrix = {};

        TranslationMatrix = CreateTranslationMatrix(position);
        RotationYMatrix   = CreateYRotationMatrix  (rotation.y);
        RotationXMatrix   = CreateXRotationMatrix  (rotation.x);
        RotationZMatrix   = CreateZRotationMatrix  (rotation.z);
        ScaleMatrix       = CreateScaleMatrix      (scale);

        r.mat = (ScaleMatrix.mat * (RotationXMatrix.mat * (RotationZMatrix.mat * (RotationYMatrix.mat * TranslationMatrix.mat))));
        return (r);
    }

    static Mat4 CreateFrustrumMatrix(double left,
                                     double right,
                                     double bottom,
                                     double top,
                                     double near,
                                     double far)
    {
        Mat4 result = {};

        f32 a = (right + left) / (right - left);
        f32 b = (top + bottom) / (top - bottom);
        f32 c = -((far + near) / (far - near));
        f32 d = -((2 * far * near) / (far - near));

        result.em[0][0] = 2 * near / (right - left);

        result.em[1][1] = 2 * near / (top - bottom);

        result.em[2][0] = a;
        result.em[2][1] = b;
        result.em[2][2] = c;
        result.em[2][3] = -1;

        result.em[3][2] = d;

        return (result);
    }

    static Mat4 CreatePerspectiveMatrix(double fovy,
                                        double aspect,
                                        double zNear,
                                        double zFar)
    {
        Mat4 result = {};

        f32 yMax = zNear * tanf(fovy * M_PI / 360.0);
        f32 xMax = yMax * aspect;

        result = CreateFrustrumMatrix(-xMax, xMax, yMax, -yMax, zNear, zFar);
        return (result);
    }

    static Mat4 CreateOrthoMatrix(f32 left, f32 right, f32 bottom, f32 top,
                                  f32 nearVal, f32 farVal)
    {
        Mat4 result = {};

        result.em[0][0] = 2 / (right - left);
        result.em[1][1] = -2 / (top - bottom);
        result.em[2][2] = -2 / (farVal - nearVal);

        result.em[3][0] = -((right + left) / (right - left));
        result.em[3][1] = -((top + bottom) / (top - bottom));
        result.em[3][2] = -((farVal + nearVal) / (farVal - nearVal)); 
        result.em[3][3] = 1;

        //result.TransposeMatrix();
        return (result);
    }

    static Mat4 CreateInverseMatrix(Mat4 a)
    {
        Mat4 result = {};
        int i, j, k, p;
        f32 f, g, tol;

        f = 0.f;  /* Frobenius norm of a */
        for (i = 0; i < 4; ++i)
        {
            for (j = 0; j < 4; ++j)
            {
                g = a.em[i][j];
                f += g * g;
            }
        }
        f = sqrt(f);
        tol = f * 2.2204460492503131e-016;
        result = CreateIdentityMatrix();
        for (k = 0; k < 4; ++k)
        {  /* Main loop */
            f = fabs(a.em[k][k]);  /* Find pivot. */
            p = k;
            for (i = k+1; i < 4; ++i) {
                g = fabs(a.em[i][k]);
                if (g > f) {
                    f = g;
                    p = i;
                }
            }
            if (f < tol)
                return (result);  /* Matrix is singular. */
            if (p != k)
            {  /* Swap rows. */
                for (j = k; j < 4; ++j)
                {
                    f = a.em[k][j];
                    a.em[k][j] = a.em[p][j];
                    a.em[p][j] = f;
                }
                for (j = 0; j < 4; ++j)
                {
                    f = result.em[k][j];
                    result.em[k][j] = result.em[p][j];
                    result.em[p][j] = f;
                }
            }
            f = 1.f / a.em[k][k];  /* Scale row so pivot is 1. */
            for (j = k; j < 4; ++j)
                a.em[k][j] *= f;
            for (j = 0; j < 4; ++j)
                result.em[k][j] *= f;
            for (i = 0; i < 4; ++i)
            {  /* Subtract to get zeros. */
                if (i == k)
                    continue;
                f = a.em[i][k];
                for (j = k; j < 4; ++j)
                    a.em[i][j] -= a.em[k][j] * f;
                for (j = 0; j < 4; ++j)
                    result.em[i][j] -= result.em[k][j] * f;
            }
        }
        return (result);
    }

    void TransposeMatrix()
    {
        int i, j;
        Mat4 temp = {};

        for (i = 0; i < 4; i++)
        {
            for (j = 0; j < 4; j++)
                temp.em[j][i] = this->em[i][j];
        }

        for (int i = 0; i < 16; i++)
            this->e[i] = temp.e[i];
    };

    //printing in a COLUMN-ROW matrix as COLUMN-MAJOR matrix
    void PrintMatrix() const
    {
        printf("\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n",
               e[0],  e[1],  e[2],  e[3],
               e[4],  e[5],  e[6],  e[7],
               e[8],  e[9],  e[10], e[11],
               e[12], e[13], e[14], e[15]);
    }

    //printing in a COLUMN-ROW matrix as ROW-MAJOR matrix
    void PrintMatrixTransposed() const
    {
        printf("\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n",
               e[0], e[4], e[8],  e[12],
               e[1], e[5], e[9],  e[13],
               e[2], e[6], e[10], e[14],
               e[3], e[7], e[11], e[15]);
    }
};

inline float orient2d(const float2& a, const float2& b, const float2& c)
{
    return (b.x-a.x)*(c.y-a.y) - (b.y-a.y)*(c.x-a.x);
}

inline float3 getBarycentricCoordonates(float2 a, float2 b, float2 c, float2 p,
                                        f32 *areaABCp)
{
    float3 result = {};
    f32 areaABC = orient2d(a, b, c);

    result.x = orient2d(b, c, p) / areaABC;
    result.y = orient2d(c, a, p) / areaABC;
    result.z = orient2d(a, b, p) / areaABC;

    if (areaABCp != nullptr)
        *areaABCp = areaABC;
    return result;
}

// NOTE(V. Caraulan): If I was a C++ fantatic this would be a template

inline f32 barycentricInterpolation(float3 barycentricCoordinates, f32 *value)
{
    return(value[0] + barycentricCoordinates.y * (value[1] - value[0]) + 
           barycentricCoordinates.z * (value[2] - value[0]));
}

inline float2 barycentricInterpolation(float3 barycentricCoordinates,
                                       float2 *value)
{
    return(value[0] + (barycentricCoordinates.y * (value[1] - value[0]) + 
                       barycentricCoordinates.z * (value[2] - value[0])));
}

inline f32 Minf(f32 a, f32 b)
{
    if (a <= b)
        return (a);
    return (b);
}

inline f32 Maxf(f32 a, f32 b)
{
    if (a >= b)
        return (a);
    return (b);
}

inline float2 Minf(float2 a, float2 b)
{
    float2 result;

    result.x = Minf(a.x, b.x);
    result.y = Minf(a.y, b.y);
    return (result);
}

inline float2 Maxf(float2 a, float2 b)
{
    float2 result;

    result.x = Maxf(a.x, b.x);
    result.y = Maxf(a.y, b.y);
    return (result);
}

inline float3 Minf(float3 a, float3 b)
{
    float3 result;

    result.x = Minf(a.x, b.x);
    result.y = Minf(a.y, b.y);
    result.z = Minf(a.z, b.z);
    return (result);
}

inline float3 Maxf(float3 a, float3 b)
{
    float3 result;

    result.x = Maxf(a.x, b.x);
    result.y = Maxf(a.y, b.y);
    result.z = Maxf(a.z, b.z);
    return (result);
}


inline f32 Clamp(const f32 value, const f32 lower,const f32 upper)
{
    return Minf(upper, Maxf(value, lower));
}

inline f32 Lerp(float a,float b,float t)
{
    return (1 - t) * a + b * t;
}

inline f32 InverseLerp(const f32 xx,const  f32 yy,const  f32 value)
{
    return (value - xx)/(yy - xx);
}
