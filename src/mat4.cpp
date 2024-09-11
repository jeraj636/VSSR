#include <cmath>
#include "matematika.h"
#include <iostream> //!le te zest
namespace mat
{

    mat4::mat4()
    {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                mat[i][j] = 0;
    }

    mat4::mat4(float t)
    {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                mat[i][j] = 0;
        for (int i = 0; i < 4; i++)
            mat[i][i] = t;
    }

    mat4 mat4::operator-(const mat4 &t) const
    {
        mat4 tmp;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                tmp.mat[i][j] = mat[i][j] - t.mat[i][j];
        return tmp;
    }

    mat4 mat4::operator-(float t) const
    {
        mat4 tmp;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                tmp.mat[i][j] = mat[i][j] - t;
        return tmp;
    }

    mat4 mat4::operator+(const mat4 &t) const
    {
        mat4 tmp;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                tmp.mat[i][j] = mat[i][j] + t.mat[i][j];
        return tmp;
    }

    mat4 mat4::operator+(float t) const
    {
        mat4 tmp;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                tmp.mat[i][j] = mat[i][j] + t;
        return tmp;
    }

    mat4 &mat4::operator-=(const mat4 &t)
    {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                mat[i][j] -= t.mat[i][j];
        return *this;
    }

    mat4 &mat4::operator-=(float t)
    {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                mat[i][j] -= t;
        return *this;
    }

    mat4 &mat4::operator+=(const mat4 &t)
    {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                mat[i][j] += t.mat[i][j];
        return *this;
    }

    mat4 &mat4::operator+=(float t)
    {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                mat[i][j] += t;
        return *this;
    }

    mat4 mat4::operator*(const mat4 &t) const
    {
        mat4 tmp;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                for (int k = 0; k < 4; k++)
                    tmp.mat[i][j] += mat[i][k] * t.mat[k][j];
        return tmp;
    }

    mat4 mat4::operator*(float t) const
    {
        mat4 tmp = *this;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                tmp += t;
        return tmp;
    }

    mat4 mat4::operator*=(const mat4 &t)
    {
        mat4 tmp;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                for (int k = 0; k < 4; k++)
                    tmp.mat[i][j] += mat[i][k] * t.mat[k][j];
        *this = tmp;
        return *this;
    }

    mat4 mat4::operator*=(float t)
    {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                mat[i][j] *= t;
        return *this;
    }

    mat4 rotacijska(const mat4 &t, vec3 rotacija)
    {
        mat4 x(1), y(1), z(1);
        if (rotacija.z != 0)
        {
            z.mat[0][0] = cos(rotacija.z);
            z.mat[0][1] = -sin(rotacija.z);
            z.mat[1][1] = cos(rotacija.z);
            z.mat[1][0] = sin(rotacija.z);
        }
        if (rotacija.y != 0)
        {

            y.mat[0][0] = cos(rotacija.y);
            y.mat[0][2] = sin(rotacija.y);
            y.mat[2][2] = cos(rotacija.y);
            y.mat[2][0] = -sin(rotacija.y);
        }
        if (rotacija.x != 0)
        {

            x.mat[1][1] = cos(rotacija.x);
            x.mat[1][2] = -sin(rotacija.x);
            x.mat[2][1] = sin(rotacija.x);
            x.mat[2][2] = cos(rotacija.x);
        }

        mat4 tmp = y * x;
        tmp = z * tmp;
        return t * tmp;
    }

    mat4 velikostna(const mat4 &t, vec3 velikost)
    {
        mat4 tmp(1);
        tmp.mat[0][0] = velikost.x;
        tmp.mat[1][1] = velikost.y;
        tmp.mat[2][2] = velikost.z;
        tmp.mat[3][3] = 1;
        return t * tmp;
    }

    mat4 pozicijska(const mat4 &t, vec3 pozicija)
    {
        mat4 tmp(1);
        tmp.mat[0][3] = pozicija.x;
        tmp.mat[1][3] = pozicija.y;
        tmp.mat[2][3] = pozicija.z;
        tmp.mat[3][3] = 1;
        return t * tmp;
    }

    mat4 projekcija(const mat4 &t, float visina, float sirina, float vidni_kot, float z_blizu, float z_dalec)
    {
        mat4 tmp(0);
        float a = visina / sirina;
        vidni_kot = vidni_kot / 2;
        tmp.mat[0][0] = (1 / tan(vidni_kot)) * a;
        tmp.mat[1][1] = (1 / tan(vidni_kot)); // /a // /a na koncu //! Ne sme se deliti z a zazlog neznan
        tmp.mat[2][2] = z_dalec / (z_dalec - z_blizu);
        tmp.mat[3][2] = 1;
        tmp.mat[2][3] = (-z_dalec * z_blizu) / (z_dalec - z_blizu);

        return t * tmp;
    }
    mat4 poglej(const vec3 &pozicija, const vec3 &tarca, const vec3 &gor)
    {
        mat4 poglej(1), pozicija_mat(1);

        vec3 desno, kamera_gor, smer;

        smer = pozicija - tarca;
        smer = smer.normaliziraj();

        desno = vektorski_produkt(gor, smer);
        desno.normaliziraj();

        kamera_gor = vektorski_produkt(smer, desno);
        kamera_gor.normaliziraj();

        poglej.mat[0][0] = desno.x;
        poglej.mat[0][1] = desno.y;
        poglej.mat[0][2] = desno.z;

        poglej.mat[1][0] = kamera_gor.x;
        poglej.mat[1][1] = kamera_gor.y;
        poglej.mat[1][2] = kamera_gor.z;

        poglej.mat[2][0] = smer.x;
        poglej.mat[2][1] = smer.y;
        poglej.mat[2][2] = smer.z;

        vec3 poz = pozicija;
        pozicija_mat = pozicijska(pozicija_mat, poz);
        return poglej * pozicija_mat;
    }

};