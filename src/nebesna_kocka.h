/*
Opis: Deklaracija razreda Nebesna_kocka
Avtor: Jakob Jeraj
Licenca: GNU GPL 3
*/

#ifndef NEBESNA_KOCKA_H
#define NEBESNA_KOCKA_H

#include <string>
#include <stdint.h>
#include <vector>

#include "matematika.h"
class Nebesna_kocka
{
public:
    void nastavi(std::vector<std::string> ploskve, mat::vec3 poz, mat::vec3 rot, mat::vec3 vel);
    uint32_t id_teksture;
    mat::vec3 pozicija, rotacija, velikost;

private:
    bool ali_sem_ze_nastavil;
};

#endif