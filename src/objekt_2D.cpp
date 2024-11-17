/*
Opis: Funkcije za razred Objekt_2D
Avtor: Jakob Jeraj
Licenca: GNU GPL 3
*/

#include "objekt_2D.h"
#include "tekstura.h"
#include "risalnik.h"
#include "stdlib.h"
Objekt_2D::Objekt_2D()
{
    m_tekstura_id = -1;
    m_pot_do_teksture = "";
    poravnava = R_P_X_SREDINA | R_P_Y_SREDINA;
}

Objekt_2D::Objekt_2D(mat::vec2 t_pozicija, mat::vec2 t_velikost, float t_rotacija, uint32_t t_barva, const std::string pot_do_teksture, bool t_aktiven, uint8_t t_poravnava)
{
    pozicija = t_pozicija;
    velikost = t_velikost;
    rotacija = t_rotacija;
    barva = t_barva;
    m_tekstura_id = nalozi_teksturo(pot_do_teksture);
    m_pot_do_teksture = pot_do_teksture;
    aktiven = t_aktiven;
    poravnava = t_poravnava;
}

Objekt_2D::Objekt_2D(const Objekt_2D &t)
{
    pozicija = t.pozicija;
    velikost = t.velikost;
    rotacija = t.rotacija;
    barva = t.barva;
    m_tekstura_id = nalozi_teksturo(t.m_pot_do_teksture);
    m_pot_do_teksture = t.m_pot_do_teksture;
    poravnava = t.poravnava;
    aktiven = t.aktiven;
}

Objekt_2D &Objekt_2D::operator=(const Objekt_2D &t)
{
    pozicija = t.pozicija;
    velikost = t.velikost;
    rotacija = t.rotacija;
    barva = t.barva;
    if (m_pot_do_teksture.size() != 0)
        sprosti_teksturo(&m_tekstura_id);
    m_tekstura_id = nalozi_teksturo(t.m_pot_do_teksture);
    m_pot_do_teksture = t.m_pot_do_teksture;
    aktiven = t.aktiven;
    poravnava = t.poravnava;
    return *this;
}

Objekt_2D::~Objekt_2D()
{
    sprosti_teksturo(&m_tekstura_id);
}

void Objekt_2D::nastavi_teksturo(const std::string &pot_do_teksture)
{
    if (m_pot_do_teksture.size() != 0)
        sprosti_teksturo(&m_tekstura_id);
    m_pot_do_teksture = pot_do_teksture;
    m_tekstura_id = nalozi_teksturo(m_pot_do_teksture);
}

void Objekt_2D::nastavi(mat::vec2 t_pozicija, mat::vec2 t_velikost, float t_rotacija, uint32_t t_barva, const std::string pot_do_teksture, bool t_aktiven, uint8_t t_poravnava)
{
    pozicija = t_pozicija;
    velikost = t_velikost;
    rotacija = t_rotacija;
    barva = t_barva;
    if (m_pot_do_teksture.size() != 0)
        sprosti_teksturo(&m_tekstura_id);
    m_tekstura_id = nalozi_teksturo(pot_do_teksture);
    m_pot_do_teksture = pot_do_teksture;
    aktiven = t_aktiven;
    poravnava = t_poravnava;
}

bool Objekt_2D::ali_je_miska_gor()
{
    if (cakaj_do <= Cas::get_cas())
        aktiven = true;
    else
        aktiven = false;
    mat::vec2 poz = pozicija;
    //* Zamik glede na poravanvo
    //! Potrebno testirati
    if ((poravnava & R_P_LEVO) == R_P_LEVO)
        poz.x += velikost.x / 2;
    if ((poravnava & R_P_DESNO) == R_P_DESNO)
        poz.x -= velikost.x / 2;
    if ((poravnava & R_P_SPODAJ) == R_P_Y_SREDINA)
        poz.y -= velikost.x / 2;
    if ((poravnava & R_P_ZGORAJ) == R_P_ZGORAJ)
        poz.y += velikost.x / 2;

    // if ((poravnava & R_P_ZGORAJ) == R_P_Y_SREDINA)
    if (Risalnik::kazalec_miske.pozicija_kazalca_na_platnu.x > poz.x - velikost.x / 2 &&
        Risalnik::kazalec_miske.pozicija_kazalca_na_platnu.x < poz.x + velikost.x / 2 &&
        Risalnik::kazalec_miske.pozicija_kazalca_na_platnu.y > poz.y - velikost.y / 2 &&
        Risalnik::kazalec_miske.pozicija_kazalca_na_platnu.y < poz.y + velikost.y / 2)
        return true;
    return false;
}

Meteor::Meteor()
    : Objekt_2D(mat::vec2(11), mat::vec2(0.1), 0, 0xffffffff, "../sredstva/meteorcek.png", true, R_P_X_SREDINA | R_P_Y_SREDINA)
{
    hitrost = -2;
}

void Meteor::posodobi()
{
    if (pozicija.x > 5 || pozicija.x < -5 ||
        pozicija.y > 5 || pozicija.y < -5)
    {
        int stranica = (int)(rand() % 4);
        float poz = rand() / (float)RAND_MAX * 10;
        poz -= 5;
        switch (stranica)
        {
        case 0:
            pozicija = mat::vec2(poz, -4);
            break;
        case 1:
            pozicija = mat::vec2(4, poz);
            break;
        case 2:
            pozicija = mat::vec2(poz, 4);
            break;
        case 3:
            pozicija = mat::vec2(-4, poz);
            break;
        }
        smer = pozicija - mat::vec2(0);
        smer = smer.normaliziraj();
        nastavi_rotacijo();
    }
    // std::cout << smer << "\n";
    pozicija += smer * Cas::get_delta_cas() * hitrost;
}

#include <cmath>
void Meteor::nastavi_rotacijo()
{
    rotacija = radToDeg(atan2(smer.y, smer.x));
}