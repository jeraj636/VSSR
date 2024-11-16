/*
Opis: Funkcije razreda vec3
Avtor: Jakob Jeraj
Licenca: GNU GPL 3
*/

#include <cmath>
#include <iostream>

#include "matematika.h"

namespace mat
{

    vec3::vec3()
    {
        x = 0;
        y = 0;
        z = 0;
    }
    vec3::vec3(float t_x)
    {
        x = t_x;
        y = t_x;
        z = t_x;
    }
    vec3::vec3(float t_x, float t_y, float t_z)
    {
        x = t_x;
        y = t_y;
        z = t_z;
    }
    vec3::vec3(const vec2 &t_x, float t_z)
    {
        x = t_x.x;
        y = t_x.y;
        z = t_z;
    }

    vec3 vec3::operator+(const vec3 &t) const
    {
        vec3 tmp = *this;
        tmp.x += t.x;
        tmp.y += t.y;
        tmp.z += t.z;
        return tmp;
    }
    vec3 vec3::operator+(float t) const
    {
        vec3 tmp = *this;
        tmp.x += t;
        tmp.y += t;
        tmp.z += t;
        return tmp;
    }
    vec3 vec3::operator-(const vec3 &t) const
    {
        vec3 tmp = *this;
        tmp.x -= t.x;
        tmp.y -= t.y;
        tmp.z -= t.z;
        return tmp;
    }
    vec3 vec3::operator-(float t) const
    {
        vec3 tmp = *this;
        tmp.x -= t;
        tmp.y -= t;
        tmp.z -= t;
        return tmp;
    }
    vec3 vec3::operator*(float t)
    {
        vec3 tmp = *this;
        tmp.x *= t;
        tmp.y *= t;
        tmp.z *= t;
        return tmp;
    }
    vec3 vec3::operator*(const mat3 &t) const
    {
        mat::vec3 tmp;
        tmp.x = t.mat[0][0] * x + t.mat[0][1] * y + t.mat[0][2] * z;
        tmp.y = t.mat[1][0] * x + t.mat[1][1] * y + t.mat[1][2] * z;
        tmp.y = t.mat[2][0] * x + t.mat[2][1] * y + t.mat[2][2] * z;
        return tmp;
    }
    vec3 vec3::operator*(const mat4 &t) const
    {
        mat::vec3 tmp;
        tmp.x = t.mat[0][0] * x + t.mat[0][1] * y + t.mat[0][2] * z + t.mat[0][3];
        tmp.y = t.mat[1][0] * x + t.mat[1][1] * y + t.mat[1][2] * z + t.mat[1][3];
        tmp.z = t.mat[2][0] * x + t.mat[2][1] * y + t.mat[2][2] * z + t.mat[2][3];
        return tmp;
    }

    vec3 &vec3::operator+=(const vec3 &t)
    {
        x += t.x;
        y += t.y;
        z += t.z;
        return *this;
    }
    vec3 &vec3::operator+=(float t)
    {
        x += t;
        y += t;
        z += t;
        return *this;
    }
    vec3 &vec3::operator-=(const vec3 &t)
    {
        x -= t.x;
        y -= t.y;
        z -= t.z;
        return *this;
    }
    vec3 &vec3::operator-=(float t)
    {
        x -= t;
        y -= t;
        z -= t;
        return *this;
    }

    float vec3::dolzina()
    {
        return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
    }

    vec3 vec3::normaliziraj()
    {
        vec3 tmp = *this;
        float dol = tmp.dolzina();
        tmp.x /= dol;
        tmp.y /= dol;
        tmp.z /= dol;
        return tmp;
    }

    float skalarni_produkt(const vec3 &a, const vec3 &b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

}