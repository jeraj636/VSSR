#include <cmath>

#include "matematika.h"

template <typename T>
T mat::normaliziraj(T a)
{
    return a;
}

template <>
mat::vec2 mat::normaliziraj(mat::vec2 a)
{
    a = a.normaliziraj();
    return a;
}

template <>
mat::vec3 mat::normaliziraj(mat::vec3 a)
{
    a = a.normaliziraj();
    return a;
}

mat::vec3 mat::vektorski_produkt(const mat::vec3 &a, const mat::vec3 &b)
{
    mat::vec3 tmp;
    tmp.x = a.y * b.z - a.z * b.y;
    tmp.y = a.z * b.x - a.x * b.z;
    tmp.z = a.x * b.y - a.y * b.x;
    return tmp;
}
namespace mat
{

    std::ostream &operator<<(std::ostream &os, const vec3 &t)
    {
        os << t.x << " " << t.y << " " << t.z;
        return os;
    }

    std::ostream &operator<<(std::ostream &os, const vec2 &t)
    {
        os << t.x << " " << t.y;
        return os;
    }

    std::ostream &operator<<(std::ostream &os, const mat3 &t)
    {
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
                os << t.mat[i][j] << " ";
            os << "\n";
        }
        return os;
    }

    std::ostream &operator<<(std::ostream &os, const mat4 &t)
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
                os << t.mat[i][j] << " ";
            os << "\n";
        }
        return os;
    }
};