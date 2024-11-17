/*
Opis: Funkcije razreda vec2
Avtor: Jakob Jeraj
Licenca: GNU GPL 3
*/

#include <cmath>
#include <iostream>

#include "matematika.h"

using namespace mat;
vec2::vec2()
{
    x = 0;
    y = 0;
}

vec2::vec2(float t_x)
{
    x = t_x;
    y = t_x;
}

vec2::vec2(float t_x, float t_y)
{
    x = t_x;
    y = t_y;
}

vec2 vec2::operator+(const vec2 &t) const
{
    vec2 tmp = *this;
    tmp.x += t.x;
    tmp.y += t.y;
    return tmp;
}

vec2 vec2::operator+(float t) const
{
    vec2 tmp = *this;
    tmp.x += t;
    tmp.y += t;
    return tmp;
}

vec2 vec2::operator-(const vec2 &t) const
{
    vec2 tmp = *this;
    tmp.x -= t.x;
    tmp.y -= t.y;
    return tmp;
}

vec2 vec2::operator-(float t) const
{
    vec2 tmp = *this;
    tmp.x -= t;
    tmp.y -= t;
    return tmp;
}
vec2 vec2::operator*(float t) const
{
    vec2 tmp = *this;
    tmp.x *= t;
    tmp.y *= t;
    return tmp;
}
vec2 vec2::operator/(float t) const
{
    vec2 tmp = *this;
    tmp.x /= t;
    tmp.y /= t;
    return tmp;
}
vec2 &vec2::operator+=(const vec2 &t)
{
    x += t.x;
    y += t.y;
    return *this;
}
vec2 &vec2::operator+=(float t)
{
    x += t;
    y += t;
    return *this;
}
vec2 &vec2::operator-=(const vec2 &t)
{
    x -= t.x;
    y -= t.y;
    return *this;
}
vec2 &vec2::operator-=(float t)
{
    x -= t;
    y -= t;
    return *this;
}

float vec2::dolzina()
{
    return sqrt(pow(x, 2) + pow(y, 2));
}

vec2 vec2::normaliziraj()
{
    vec2 tmp = *this;
    float dol = tmp.dolzina();
    tmp.x /= dol;
    tmp.y /= dol;
    return tmp;
}

float skalarni_produkt(const vec2 &a, const vec2 &b)
{
    return a.x * b.x + a.x * b.x;
}
