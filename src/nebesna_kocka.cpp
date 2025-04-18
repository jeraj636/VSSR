/*
Opis: Funkcije razreda Nebesna_kocka
Avtor: Jakob Jeraj
Licenca: GNU GPL 3
*/

#include "nebesna_kocka.h"

#include <stb_image.h>
#include <iostream>

#include "risalnik.h"
#include "dnevnik.h"

void Nebesna_kocka::nastavi(std::vector<std::string> ploskve, mat::vec3 poz, mat::vec3 rot, mat::vec3 vel)
{
    //* Nastavljanje vrednosti
    pozicija = poz;
    velikost = vel;
    rotacija = rot;

    //* Če se funkcija ne kliče prvič
    if (ali_sem_ze_nastavil)
    {
        // glDeleteTextures(1, &id_teksture);
    }
    //* Ustvarjanje teksture
    glGenTextures(1, &id_teksture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id_teksture);
    int sirina, visina, kanali;

    for (int i = 0; i < ploskve.size(); i++)
    {
        //* Branje slike
        unsigned char *podatki = stbi_load(ploskve[i].c_str(), &sirina, &visina, &kanali, 0);
        if (podatki)
        {
            //* Izdelovanje tekstur
            if (kanali == 4)
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, sirina, visina, 0, GL_RGBA, GL_UNSIGNED_BYTE, podatki);
            if (kanali == 3)
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, sirina, visina, 0, GL_RGB, GL_UNSIGNED_BYTE, podatki);
            stbi_image_free(podatki);
        }
        else
        {
            napaka("nebesna_kocka.cpp :: Ni mogoce dobiti datoteke %s", ploskve[i].c_str()); //* Datotekek ni mogoče prebrati
            stbi_image_free(podatki);
        }
    }
    //* Nastavljanje podatkov o teksturi
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    ali_sem_ze_nastavil = true;
}