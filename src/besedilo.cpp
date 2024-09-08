#include "besedilo.h"
#include "pisava.h"
#include <iostream>
Besedilo::Besedilo(Pisava &t_pisava, uint32_t t_barva_besedila, mat::vec2 t_pozicija, float t_velikost, const std::string t_niz, Poravnava t_poravnava_x, Poravnava t_poravnava_y)
    : m_pisava(t_pisava)
{
    barva_besedila = t_barva_besedila;
    m_pozicija = t_pozicija;
    m_velikost = t_velikost;
    niz = t_niz;
    m_poravnava_x = t_poravnava_x;
    m_poravnava_y = t_poravnava_y;
}
void Besedilo::nastavi(Pisava &t_pisava, uint32_t t_barva_besedila, mat::vec2 t_pozicija, float t_velikost, const std::string t_niz, Poravnava t_poravnava_x, Poravnava t_poravnava_y)
{
    m_pisava = t_pisava;
    barva_besedila = t_barva_besedila;
    m_pozicija = t_pozicija;
    m_velikost = t_velikost;
    niz = t_niz;
    m_poravnava_x = t_poravnava_x;
    m_poravnava_y = t_poravnava_y;
}
bool Besedilo::ali_je_miska_gor()
{
    return 1;
}
void Besedilo::narisi_me()
{
    Risalnik::narisi_besedilo(m_pisava, barva_besedila, m_pozicija, m_velikost, niz, m_poravnava_x, m_poravnava_y);
}
void Besedilo::posodobi()
{
    //* Pridobivanje x in y velikosti
    /*
        y velikost dobimo na načim da najdemo y_max in y_min in izračunamo razliko
        x velikost pa s sestevanjem razlik med x1 in x0
    */
    float vel = m_velikost / m_pisava.m_velikost;
    float x = 0, y = 0;
    float min_y = 1000, max_y = -1000;
    stbtt_aligned_quad quad;
    for (int i = 0; i < niz.size(); i++)
    {
        stbtt_GetBakedQuad(m_pisava.m_char_data, 512, 512, niz[i], &x, &y, &quad, false);
        x += quad.x1 - quad.x0;
        if (quad.y1 > max_y)
            max_y = quad.y1;
        if (quad.y0 < min_y)
            min_y = quad.y0;
    }
    m_dejanska_velikost = {x, std::abs(min_y) + std::abs(max_y)};
    switch (m_poravnava_x)
    {
    case Poravnava::levo:
        m_centrirana_pozicija.x = m_pozicija.x + x / 2;
        break;
    case Poravnava::desno:
        m_centrirana_pozicija.x = m_pozicija.x - x / 2;
        break;
    }
    switch (m_poravnava_y)
    {
    case Poravnava::zograj:
        m_pozicija.y = y / 2;
        break;
    case Poravnava::spodaj:
        m_pozicija.y = -y / 2;
        break;
    }
}
