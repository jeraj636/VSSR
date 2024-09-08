#include "objekt_2D.h"
#include "tekstura.h"
Objekt_2D::Objekt_2D()
{
    m_tekstura_id = -1;
    m_pot_do_teksture = "";
}
Objekt_2D::Objekt_2D(mat::vec2 t_pozicija, mat::vec2 t_velikost, float t_rotacija, uint32_t t_barva, const std::string pot_do_teksture, bool t_aktiven)
{
    pozicija = t_pozicija;
    velikost = t_velikost;
    rotacija = t_rotacija;
    barva = t_barva;
    m_tekstura_id = nalozi_teksturo(pot_do_teksture);
    m_pot_do_teksture = pot_do_teksture;
    aktiven = t_aktiven;
}
Objekt_2D::Objekt_2D(const Objekt_2D &t)
{
    pozicija = t.pozicija;
    velikost = t.velikost;
    rotacija = t.rotacija;
    barva = t.barva;
    m_tekstura_id = nalozi_teksturo(t.m_pot_do_teksture);
    m_pot_do_teksture = t.m_pot_do_teksture;
    aktiven = t.aktiven;
}
Objekt_2D &Objekt_2D::operator=(const Objekt_2D &t)
{
    pozicija = t.pozicija;
    velikost = t.velikost;
    rotacija = t.rotacija;
    barva = t.barva;
    m_tekstura_id = nalozi_teksturo(t.m_pot_do_teksture);
    m_pot_do_teksture = t.m_pot_do_teksture;
    aktiven = t.aktiven;
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