/*
Opis: Deklaracija razreda Kamen
Avtor: Jakob Jeraj
Licenca: GNU GPL 3
*/

#ifndef KAMEN_H
#define KAMEN_H

#include "objekt_3D.h"

/*
Kamen je dedovan odjekt iz Obejkt_3D. Dodano ima hitrost in smer.
*/
class Kamen : public Objekt_3D
{
public:
    mat::vec3 smer;
    float hitrost;
};

#endif