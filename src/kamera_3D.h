#ifndef KAMERA_3D_H
#define KAMERA_3D_H

#include "matematika.h"
class Risalnik;
class Kamera_3D
{
public:
    friend class Risalnik;
    Kamera_3D();
    void nastavi();
    void posodobi();
    mat::vec3 pozicija;
    mat::vec3 smer;
    mat::vec3 tarca;
    mat::vec3 spredaj;
    mat::vec3 gor;
    float yaw, pitch; //! Prevedi
    float vidno_polje;
    float hitrost_premikanja;
    float hitrost_miske;
    bool inveertirana;
    bool premikanje_kamere;

protected:
    mat::mat4 m_mat_pogled;

private:
    mat::vec3 m_kamera_gor;
};

#endif