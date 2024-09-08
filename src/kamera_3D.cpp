#include "kamera_3D.h"
#include "risalnik.h"
#include <iostream>
#include <cmath>
#include "matematika.h"
Kamera_3D::Kamera_3D()
    : m_mat_pogled(1)
{
}

void Kamera_3D::nastavi()
{
    pozicija = mat::vec3(0, 0, 25);
    smer = mat::vec3(0, 0, 1);
    tarca = mat::vec3(0, 0, 0);
    spredaj = mat::vec3(0, 0, -1);
    gor = mat::vec3(0, 1, 0);
    yaw = -90;
    pitch = 0;
    vidno_polje = 60;
    hitrost_premikanja = 10;
    hitrost_miske = 1;
    m_kamera_gor = mat::vec3(0);
    m_mat_pogled = mat::mat4(0);
}

void Kamera_3D::posodobi()
{
    if (Risalnik::dobi_tipko('R'))
    {
        nastavi();
    }
    mat::vec2 premik_kazalca;
    premik_kazalca = Risalnik::kazalec_miske.pozicija_kazalca - Risalnik::kazalec_miske.pr_pozicija_kazalca;
    premik_kazalca.y *= -1;

    yaw += premik_kazalca.x * Cas::get_delta_cas() * hitrost_miske;
    pitch += premik_kazalca.y * Cas::get_delta_cas() * hitrost_miske;

    pitch = mat::obrezi_st(pitch, -89.0f, 89.0f);
    smer.x = cos(degToRad(yaw)) * cos(degToRad(pitch));
    smer.y = sin(degToRad(pitch));
    smer.z = sin(degToRad(yaw)) * cos(degToRad(pitch));
    spredaj = mat::normaliziraj(smer);

    mat::vec3 kamera_desno = mat::normaliziraj(mat::vektorski_produkt(gor, smer));
    mat::vec3 m_kamera_gor = mat::vektorski_produkt(smer, kamera_desno);

    //* Premikanje kamere glede na pritisnjeno tipko
    if (Risalnik::dobi_tipko('W'))
        pozicija += spredaj * hitrost_premikanja * Cas::get_delta_cas();
    if (Risalnik::dobi_tipko('S'))
        pozicija -= spredaj * hitrost_premikanja * Cas::get_delta_cas();
    if (Risalnik::dobi_tipko('A'))
        pozicija += mat::normaliziraj(mat::vektorski_produkt(spredaj, gor)) * hitrost_premikanja * Cas::get_delta_cas();
    if (Risalnik::dobi_tipko('D'))
        pozicija -= mat::normaliziraj(mat::vektorski_produkt(spredaj, gor)) * hitrost_premikanja * Cas::get_delta_cas();

    m_mat_pogled = mat::poglej(pozicija, pozicija + spredaj, m_kamera_gor);
}