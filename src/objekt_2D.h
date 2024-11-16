/*
Opis: Deklaracija razreda Objekt_2D
Avtor: Jakob Jeraj
Licenca: GNU GPL 3
*/

#ifndef OBJEKT_2D_H
#define OBJEKT_2D_H

#include <string>

#include "matematika.h"
#include "barva.h"

class Objekt_2D
{
public:
    mat::vec2 pozicija;
    mat::vec2 velikost;
    float rotacija;
    Barva barva;
    bool aktiven;
    double cakaj_do;
    uint8_t poravnava;

public:
    Objekt_2D();
    Objekt_2D(const Objekt_2D &t);
    Objekt_2D(mat::vec2 t_pozicija, mat::vec2 t_velikost, float t_rotacija, uint32_t t_barva, const std::string pot_do_teksture, bool t_aktiven, uint8_t t_poravnava);
    void nastavi(mat::vec2 t_pozicija, mat::vec2 t_velikost, float t_rotacija, uint32_t t_barva, const std::string pot_do_teksture, bool t_aktiven, uint8_t t_poravnava);
    Objekt_2D &operator=(const Objekt_2D &t);
    void nastavi_teksturo(const std::string &pot_do_teksture);
    ~Objekt_2D();
    friend class Risalnik;
    bool ali_je_miska_gor();

protected:
    uint32_t m_tekstura_id;
    std::string m_pot_do_teksture;
};
#endif