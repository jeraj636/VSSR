#include "objekt_3D.h"

Objekt_3D::Objekt_3D()
{
    m_tocke = nullptr;
    m_velikost_tock = 0;
    m_pot_do_objekta = "";
}
Objekt_3D::Objekt_3D(const Objekt_3D &t)
{
    pozicija = t.pozicija;
    veliksot = t.veliksot;
    rotacija = t.rotacija;
    barva = t.barva;
    aktiven = t.aktiven;
    m_pot_do_objekta = t.m_pot_do_objekta;
    m_tocke = nullptr;
    m_velikost_tock = 0;
    preberi_obj();
}
Objekt_3D::Objekt_3D(mat::vec3 t_pozicija, mat::vec3 t_velikost, mat::vec3 t_rotacija, uint32_t t_barva, bool t_aktiven, const std::string &t_pot_do_objekta)
{
    pozicija = t_pozicija;
    veliksot = t_velikost;
    rotacija = t_rotacija;
    barva = t_barva;
    aktiven = t_aktiven;
    m_pot_do_objekta = t_pot_do_objekta;
    m_tocke = nullptr;
    m_velikost_tock = 0;
    preberi_obj();
}
Objekt_3D &Objekt_3D::operator=(const Objekt_3D &t)
{
    pozicija = t.pozicija;
    veliksot = t.veliksot;
    rotacija = t.rotacija;
    barva = t.barva;
    aktiven = t.aktiven;

    if (m_pot_do_objekta.size() != 0 && m_tocke != nullptr) //* sprostitev pomnilnika
    {
        delete[] m_tocke;
        m_tocke = nullptr;
        m_velikost_tock = 0;
    }

    m_pot_do_objekta = t.m_pot_do_objekta;
    preberi_obj();
    return *this;
}

Objekt_3D::~Objekt_3D()
{
    if (m_pot_do_objekta.size() != 0 && m_tocke != nullptr) //* sprostitev pomnilnika
        delete[] m_tocke;
}