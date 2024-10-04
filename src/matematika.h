#ifndef MATEMATIKA_H
#define MATEMATIKA_H

#define degToRad(angleInDegrees) ((angleInDegrees) * 3.14 / 180.0)
#define radToDeg(angleInRadians) ((angleInRadians) * 180.0 / 3.14)

#include <ostream>

namespace mat
{
    class mat3;
    class vec2
    {
    public:
        float x, y;
        vec2();
        vec2(float t_x);
        vec2(float t_x, float t_y);

        vec2 operator+(const vec2 &t) const;
        vec2 operator+(float t) const;
        vec2 operator-(const vec2 &t) const;
        vec2 operator-(float t) const;
        vec2 operator/(float t) const;

        vec2 &operator+=(const vec2 &t);
        vec2 &operator+=(float t);
        vec2 &operator-=(const vec2 &t);
        vec2 &operator-=(float t);
        friend std::ostream &operator<<(std::ostream &os, const vec2 &t);

        float dolzina();
        vec2 normaliziraj();

    private:
    };

    class vec3
    {
    public:
        float x, y, z;
        vec3();
        vec3(float t_x);
        vec3(float t_x, float t_y, float t_z);
        vec3(const vec2 &t_x, float t_z);

        vec3 operator+(const vec3 &t) const;
        vec3 operator+(float t) const;
        vec3 operator-(const vec3 &t) const;
        vec3 operator-(float t) const;
        vec3 operator*(float t);
        vec3 operator*(const mat3 &t) const;
        bool operator==(const vec3 &t);
        bool operator!=(const vec3 &t);
        vec3 &operator+=(const vec3 &t);
        vec3 &operator+=(float t);
        vec3 &operator-=(const vec3 &t);
        vec3 &operator-=(float t);
        friend std::ostream &operator<<(std::ostream &os, const vec3 &t);

        float dolzina();
        vec3 normaliziraj();

    private:
    };
    vec3 vektorski_produkt(const vec3 &a, const vec3 &b);

    float skalarni_produkt(const vec2 &a, const vec2 &b);
    float skalarni_produkt(const vec3 &a, const vec3 &b);
    class mat3
    {
    public:
        float mat[3][3];
        mat3();
        mat3(float t);

        mat3 operator-(const mat3 &t) const;
        mat3 operator-(float t) const;
        mat3 operator+(const mat3 &t) const;
        mat3 operator+(float t) const;

        mat3 &operator-=(const mat3 &t);
        mat3 &operator-=(float t);
        mat3 &operator+=(const mat3 &t);
        mat3 &operator+=(float t);

        mat3 operator*(const mat3 &t) const;
        mat3 operator*=(const mat3 &t);
        mat3 operator*(float t) const;
        mat3 operator*=(float t);
        friend std::ostream &operator<<(std::ostream &os, const mat3 &t);
    };

    class mat4
    {
    public:
        float mat[4][4];
        mat4();
        mat4(float t);

        mat4 operator-(const mat4 &t) const;
        mat4 operator-(float t) const;
        mat4 operator+(const mat4 &t) const;
        mat4 operator+(float t) const;

        mat4 &operator-=(const mat4 &t);
        mat4 &operator-=(float t);
        mat4 &operator+=(const mat4 &t);
        mat4 &operator+=(float t);

        mat4 operator*(const mat4 &t) const;
        mat4 operator*(float t) const;
        mat4 operator*=(const mat4 &t);
        mat4 operator*=(float t);
        friend std::ostream &operator<<(std::ostream &os, const mat4 &t);
    };
    mat4 rotacijska(const mat4 &t, vec3 rotacija);
    mat4 velikostna(const mat4 &t, vec3 velikost);
    mat4 pozicijska(const mat4 &t, vec3 pozicija);
    mat4 projekcija(const mat4 &t, float visina, float sirina, float vidni_kot, float z_blizu, float z_dalec);
    mat4 poglej(const vec3 &pozicija, const vec3 &tarca, const vec3 &gor);

    mat3 rotacijska(const mat3 &t, float rotacija);
    mat3 velikostna(const mat3 &t, vec2 velikost);
    mat3 pozicijska(const mat3 &t, vec2 pozicija);
    mat3 ortho(const mat3 &t, float levo, float desno, float gor, float dol);

    template <typename T>
    T normaliziraj(T a);

    template <class T>
    T obrezi_st(T a, T min, T max)
    {
        if (a > max)
            return max;
        else if (a < min)
            return min;
        else
            return a;
    }
}
#endif