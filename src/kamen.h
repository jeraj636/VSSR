#ifndef KAMEN_H
#define KAMEN_H

#include "objekt_3D.h"

class Kamen : public Objekt_3D
{
public:
    mat::vec3 smer;
    float hitrost;
};

#endif