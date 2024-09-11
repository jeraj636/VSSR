#ifndef OBJEKT_2D_H
#define OBJEKT_2D_H
#include "matematika.h"
#include "barva.h"
#include <string>
class Objekt_2D
{
public:
    mat::vec2 pozicija;
    mat::vec2 velikost;
    float rotacija;
    Barva barva;
    bool aktiven;

public:
    Objekt_2D();
    Objekt_2D(const Objekt_2D &t);
    Objekt_2D(mat::vec2 t_pozicija, mat::vec2 t_velikost, float t_rotacija, uint32_t t_barva, const std::string pot_do_teksture, bool t_aktiven);
    void nastavi(mat::vec2 t_pozicija, mat::vec2 t_velikost, float t_rotacija, uint32_t t_barva, const std::string pot_do_teksture, bool t_aktiven);
    Objekt_2D &operator=(const Objekt_2D &t);
    void nastavi_teksturo(const std::string &pot_do_teksture);
    ~Objekt_2D();
    friend class Risalnik;

protected:
    uint32_t m_tekstura_id;
    std::string m_pot_do_teksture;
};
#endif