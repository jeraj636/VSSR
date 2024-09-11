#ifndef OBJEKT_3D_H
#define OBJEKT_3D_H

#include <stdint.h>
#include "matematika.h"
#include "barva.h"

class Objekt_3D
{
public:
    mat::vec3 pozicija;
    mat::vec3 veliksot;
    mat::vec3 rotacija;
    Barva barva;
    bool aktiven;

public:
    Objekt_3D();
    Objekt_3D(const Objekt_3D &t);
    Objekt_3D(mat::vec3 t_pozicija, mat::vec3 t_velikost, mat::vec3 t_rotacija, uint32_t t_barva, bool t_aktiven, const std::string &t_pot_do_objekta);
    void nastavi(mat::vec3 t_pozicija, mat::vec3 t_velikost, mat::vec3 t_rotacija, uint32_t t_barva, bool t_aktiven, const std::string &t_pot_do_objekta);
    Objekt_3D &operator=(const Objekt_3D &t);
    void preberi_obj(const std::string &pot_do_objekta);
    ~Objekt_3D();
    friend class Risalnik;

protected:
    float *m_tocke;
    uint32_t m_velikost_tock;
    std::string m_pot_do_objekta;
    void preberi_obj();
};
#endif