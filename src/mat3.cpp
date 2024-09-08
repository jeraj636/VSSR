#include <cmath>
#include "matematika.h"

namespace mat
{

    mat3::mat3()
    {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                mat[i][j] = 0;
    }

    mat3::mat3(float t)
    {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                mat[i][j] = 0;
        for (int i = 0; i < 3; i++)
            mat[i][i] = t;
    }

    mat3 mat3::operator-(const mat3 &t) const
    {
        mat3 tmp;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                tmp.mat[i][j] = mat[i][j] - t.mat[i][j];
        return tmp;
    }

    mat3 mat3::operator-(float t) const
    {
        mat3 tmp;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                tmp.mat[i][j] = mat[i][j] - t;
        return tmp;
    }

    mat3 mat3::operator+(const mat3 &t) const
    {
        mat3 tmp;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                tmp.mat[i][j] = mat[i][j] + t.mat[i][j];
        return tmp;
    }

    mat3 mat3::operator+(float t) const
    {
        mat3 tmp;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                tmp.mat[i][j] = mat[i][j] + t;
        return tmp;
    }

    mat3 &mat3::operator-=(const mat3 &t)
    {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                mat[i][j] -= t.mat[i][j];
        return *this;
    }

    mat3 &mat3::operator-=(float t)
    {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                mat[i][j] -= t;
        return *this;
    }

    mat3 &mat3::operator+=(const mat3 &t)
    {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                mat[i][j] += t.mat[i][j];
        return *this;
    }

    mat3 &mat3::operator+=(float t)
    {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                mat[i][j] += t;
        return *this;
    }

    mat3 mat3::operator*(const mat3 &t) const
    {
        mat3 tmp;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                for (int k = 0; k < 3; k++)
                    tmp.mat[i][j] += mat[i][k] * t.mat[k][j];
        return tmp;
    }

    mat3 mat3::operator*(float t) const
    {
        mat3 tmp = *this;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                tmp += t;
        return tmp;
    }

    mat3 mat3::operator*=(const mat3 &t)
    {
        mat3 tmp;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                for (int k = 0; k < 3; k++)
                    tmp.mat[i][j] += mat[i][k] * t.mat[k][j];
        *this = tmp;
        return *this;
    }

    mat3 mat3::operator*=(float t)
    {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                mat[i][j] *= t;
        return *this;
    }

    mat3 rotacijska(const mat3 &t, float rotacija)
    {
        mat3 tmp(1);
        tmp.mat[0][0] = cos(rotacija);
        tmp.mat[0][1] = -sin(rotacija);
        tmp.mat[1][0] = sin(rotacija);
        tmp.mat[1][1] = cos(rotacija);
        return t * tmp;
    }
    mat3 velikostna(const mat3 &t, vec2 velikost)
    {
        mat3 tmp(1);
        tmp.mat[0][0] = velikost.x;
        tmp.mat[1][1] = velikost.y;
        return t * tmp;
    }
    mat3 pozicijska(const mat3 &t, vec2 pozicija)
    {
        mat3 tmp(1);
        tmp.mat[0][2] = pozicija.x;
        tmp.mat[1][2] = pozicija.y;
        return t * tmp;
    }
    mat3 ortho(const mat3 &t, float levo, float desno, float gor, float dol)
    {
        mat3 tmp(1);
        tmp.mat[0][0] = 2.0f / (desno - levo);
        tmp.mat[1][1] = 2.0f / (gor - dol);
        tmp.mat[0][2] = -((desno + levo) / (desno - levo));
        tmp.mat[1][2] = -((gor + dol) / (gor - dol));
        return t * tmp;
    }

};