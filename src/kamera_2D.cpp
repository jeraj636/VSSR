#include "kamera_2D.h"
#include "risalnik.h"

Kamera_2D::Kamera_2D()
    : m_mat_pogled(1)
{
    pozicija.x = 0;
    pozicija.y = 0;
}

void Kamera_2D::posodobi_kamero()
{
    //* Premikanje kamere glede na pritisnjeno tipko
    if (Risalnik::dobi_tipko('D'))
    {
        pozicija.x -= hitrost * Cas::get_delta_cas();
    }
    if (Risalnik::dobi_tipko('A'))
    {
        pozicija.x += hitrost * Cas::get_delta_cas();
    }
    if (Risalnik::dobi_tipko('W'))
    {
        pozicija.y -= hitrost * Cas::get_delta_cas();
    }
    if (Risalnik::dobi_tipko('S'))
    {
        pozicija.y += hitrost * Cas::get_delta_cas();
    }

    //* Ustvarjanje matrike za kamero
    m_mat_pogled = mat::mat3(1);
    // m_mat_pogled = mat::pozicijska(m_mat_pogled, pozicija);
}