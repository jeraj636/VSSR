#ifndef KAMERA_2D_H
#define KAMERA_2D_H
#include "matematika.h"
class Risalnik;

class Kamera_2D
{
public:
    mat::vec2 pozicija;
    Kamera_2D();
    void nastavi(float t_hitrost, mat::vec2 t_pozicija);
    friend class Risalnik;
    void posodobi_kamero();
    float hitrost;

protected:
    mat::mat3 m_mat_pogled;
};
#endif