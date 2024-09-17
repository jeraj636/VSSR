#include "besedilo.h"
#include "pisava.h"
#include <iostream>
Splosne_nastavitve_besedil::Splosne_nastavitve_besedil(Pisava t_pisava, uint32_t t_barva_besedil, uint8_t t_poravnava, float t_velikost)
    : pisava(t_pisava)
{
    barva_besedil = t_barva_besedil;
    poravnava = t_poravnava;
    velikost = t_velikost;
}

Besedilo::Besedilo(Pisava &t_pisava, uint32_t t_barva_besedila, mat::vec2 t_pozicija, float t_velikost, const std::string t_niz, uint8_t t_poravnava)
    : pisava(t_pisava)
{
    barva_besedila = t_barva_besedila;
    pozicija = t_pozicija;
    velikost = t_velikost;
    niz = t_niz;
    poravnava = t_poravnava;
}
Besedilo::Besedilo(Pisava &t_pisava)
    : pisava(t_pisava)
{
}

Besedilo::Besedilo(Splosne_nastavitve_besedil t_spl_nst_bes)
    : pisava(t_spl_nst_bes.pisava)
{
    barva_besedila = t_spl_nst_bes.barva_besedil;
    poravnava = t_spl_nst_bes.poravnava;
    velikost = t_spl_nst_bes.velikost;
}

void Besedilo::nastavi(Pisava &t_pisava, uint32_t t_barva_besedila, mat::vec2 t_pozicija, float t_velikost, const std::string t_niz, uint8_t t_poravnava)
{
    pisava = t_pisava;
    barva_besedila = t_barva_besedila;
    pozicija = t_pozicija;
    velikost = t_velikost;
    niz = t_niz;
    poravnava = t_poravnava;
}
bool Besedilo::ali_je_miska_gor()
{
    posodobi();

    //* ali je kazalec miske znotraj besedila
    if (Risalnik::kazalec_miske.pozicija_kazalca.x >= m_leva_pozicija.x &&
        Risalnik::kazalec_miske.pozicija_kazalca.x <= m_leva_pozicija.x + m_dejanska_velikost.x &&
        Risalnik::kazalec_miske.pozicija_kazalca.y >= m_leva_pozicija.y - m_dejanska_velikost.y &&
        Risalnik::kazalec_miske.pozicija_kazalca.y <= m_leva_pozicija.y)
        return true;
    else
        return false;
}
void Besedilo::narisi_me()
{
    Risalnik::narisi_besedilo(pisava, barva_besedila, pozicija, velikost, niz, poravnava);
}
void Besedilo::posodobi()
{
    float x = 0, y = 0;
    stbtt_aligned_quad quad;
    float mnozitelj_velikosti = velikost / pisava.m_velikost;
    float min_y = 1000, max_y = -1000;
    for (int i = 0; i < niz.size(); i++)
    {
        //* Funkcija stbtt_GetBakedQuad samodejno prišteva x in y
        stbtt_GetBakedQuad(pisava.m_char_data, 512, 512, niz[i], &x, &y, &quad, false);

        quad.y0 *= mnozitelj_velikosti; //* prirejanje velikosti za y (x ni potreben)
        quad.y1 *= mnozitelj_velikosti;

        //* iskanje najmanjše in največje vrednosti
        if (max_y < quad.y1)
            max_y = quad.y1;
        if (min_y > quad.y0)
            min_y = quad.y0;
    }

    //* Izračun zamika po osi x glede na poravnavo besedila
    m_leva_pozicija = pozicija;
    if (poravnava & R_P_X_SREDINA == R_P_X_SREDINA)
        pozicija.x -= (x * velikost) / 2;
    if (poravnava & R_P_DESNO == R_P_DESNO)
        pozicija.x -= x * velikost;

    m_dejanska_velikost = mat::vec2(x * mnozitelj_velikosti /* velikost po x je treba povečati ali pomanjšati*/,
                                    std::abs(min_y) + std::abs(max_y) /*Velikost na y se izračuna s seštevkom absolutnih vrednosti min_y in max_y*/);
}
