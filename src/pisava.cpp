#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include "dnevnik.h"
#include "pisava.h"
#include "risalnik.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_true_type.h>

Pisava::Pisava(const std::string &pot, uint32_t velikost)
{
    //* Nastavljanje in nalaganje nove pisave
    m_pot_do_pisave = pot;
    m_velikost = velikost;
    nalozi_pisavo();
}
Pisava &Pisava::operator=(const Pisava &t)
{
    //* Če je že nalozena kaksna pisava
    if (m_pot_do_pisave.size() != 0)
    {
        unici();
    }
    //* Nastavljanje in nalaganje nove pisave
    m_pot_do_pisave = t.m_pot_do_pisave;
    m_velikost = t.m_velikost;
    nalozi_pisavo();
    return *this;
}
Pisava::~Pisava()
{
    unici();
}
void Pisava::nalozi_pisavo()
{
    //* Odpiranje datoteke s pisavo
    std::ifstream i_dat(m_pot_do_pisave, std::ios::binary);

    if (!i_dat.is_open())
    {
        napaka("pisava.cpp :: Ni datoteke: %s\n", m_pot_do_pisave.c_str());
        exit(0);
    }
    //* Ugotavljanje velikosti datoteke
    i_dat.seekg(0, std::ios::end);
    int velikost_datoteke = i_dat.tellg();
    i_dat.seekg(0, std::ios::beg);

    //* Branje datoteke
    // unsigned char *ttf_podatki = new unsigned char[velikost_datoteke];
    unsigned char *ttf_podatki = nullptr;
    ttf_podatki = new unsigned char[velikost_datoteke];

    i_dat.read((char *)ttf_podatki, velikost_datoteke);
    if (i_dat.fail())
    {
        napaka("pisava.cpp :: Napaka pri branju datoteke: %s\n", m_pot_do_pisave.c_str());
        exit(0);
    }

    //* Branje pisave v bitmap
    unsigned char *bitmap = nullptr;
    bitmap = new unsigned char[512 * 512];

    int rezultat = stbtt_BakeFontBitmap(ttf_podatki, 0, m_velikost, bitmap, 512, 512, '\0', 128, m_char_data);

    //* Generiranje teksture
    glGenTextures(1, &m_tekstura_ID);
    glBindTexture(GL_TEXTURE_2D, m_tekstura_ID);
    // Nastavljanje atributov teksture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 512, 512, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap);

    //* Sprostitev pomnilnika
    delete[] ttf_podatki;
    delete[] bitmap;
}
void Pisava::nastavi(const std::string &pot, uint32_t velikost)
{
    //* Če je že naložena kakšna pisava
    if (m_pot_do_pisave.size() != 0)
    {
        unici();
    }
    m_pot_do_pisave = pot;
    m_velikost = velikost;
    nalozi_pisavo();
}
void Pisava::unici()
{
    //* Sprostitev pomnilnika
    m_pot_do_pisave = "";
    glDeleteTextures(1, &m_tekstura_ID);
    m_tekstura_ID = 0;
}