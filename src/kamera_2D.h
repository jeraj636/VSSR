/*
Opis: Deklaracija razreda Kamera_2D
Avtor: Jakob Jeraj
Licenca: GNU GPL 3
*/

#ifndef KAMERA_2D_H
#define KAMERA_2D_H

#include "matematika.h"

class Risalnik;

class Kamera_2D
{
public:
    friend class Risalnik;
    void nastavi(float t_hitrost, mat::vec2 t_pozicija);
    Kamera_2D();
    void posodobi_kamero();

public:
    mat::vec2 pozicija;
    float hitrost;

protected:
    mat::mat3 m_mat_pogled;
};
#endif