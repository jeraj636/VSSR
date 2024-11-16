/*
Opis: Deklaracija razreda Objekt_3D in Izstrelek
Avtor: Jakob Jeraj
Licenca: GNU GPL 3
*/

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
    static Objekt_3D hitbox(const Objekt_3D &objekt);
    static bool trk(const Objekt_3D &objekt_a, const Objekt_3D &objekt_b);

protected:
    float *m_tocke;
    uint32_t m_velikost_tock;
    std::string m_pot_do_objekta;
    void preberi_obj();

private:
    static mat::vec3 max_proc_tocka(const Objekt_3D &obj, mat::vec3 smer);
    static mat::vec3 max_proc_tocka_v_dveh(const Objekt_3D &a, const Objekt_3D &b, mat::vec3 smer);
};

class Izstrelek
{
public:
    Izstrelek();
    void posodobi();
    bool sem_neuporaben();
    mat::vec3 smer;
    Objekt_3D oblika;
    static inline float hitrost = 100;
};
#endif