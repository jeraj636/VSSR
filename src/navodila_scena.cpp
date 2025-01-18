/*
Opis: Funkcije razreda Zacetna_scena
Avtor: Jakob Jeraj
Licenca: GNU GPL 3
*/

#include "navodila_scena.h"
#include "risalnik.h"
#include "streznik.h"

Navodila_scena::Navodila_scena()
    : m_pisava("../sredstva/Cascadia.ttf", 80),
      m_b_meni(m_pisava, 0xffffffff, mat::vec2(0, .4), 0.05, "Meni", R_P_X_SREDINA),
      m_naslov(m_pisava, 0xffffffff, mat::vec2(0, 0), 0.13, "Navodila", R_P_X_SREDINA)
{
    m_zvezdno_nebo = Objekt_2D(mat::vec2(0), mat::vec2(0), 0, 0xffffffff, "../sredstva/nebo.png", true, R_P_X_SREDINA | R_P_Y_SREDINA);
}

void Navodila_scena::zacetek()
{
    Risalnik::kamera_3D.premikanje_kamere = false;
    Risalnik::nastavi_aktivnost_kazalca_miske(true);
    Risalnik::aktivna_scena_ptr = this;

    Risalnik::kamera_3D.nastavi();
}

void Navodila_scena::zanka()
{
    m_meteor.posodobi();
    Risalnik::nastavi_testiranje_globine(false);
    Risalnik::narisi_2D_objekt(m_zvezdno_nebo);
    Risalnik::narisi_2D_objekt(m_meteor);
    Risalnik::nastavi_testiranje_globine(true);

    Risalnik::narisi_besedilo(m_pisava, 0xffffffff, mat::vec2(-0.6, -0.3), 0.05, "W, A, S, D - premnikanje", R_P_LEVO);
    Risalnik::narisi_besedilo(m_pisava, 0xffffffff, mat::vec2(-0.6, -0.2), 0.05, "SHIFT - pospesek", R_P_LEVO);
    Risalnik::narisi_besedilo(m_pisava, 0xffffffff, mat::vec2(-0.6, -0.1), 0.05, "LEVI KLIK - streljanje", R_P_LEVO);
    Risalnik::narisi_besedilo(m_pisava, 0xffffffff, mat::vec2(-0.6, -0.0), 0.05, "DESNI KLIK - merjenje", R_P_LEVO);
    Risalnik::narisi_besedilo(m_pisava, 0xffffffff, mat::vec2(-0.6, 0.1), 0.05, "R - teleportacija", R_P_LEVO);
    Risalnik::narisi_besedilo(m_pisava, 0xffffffff, mat::vec2(-0.6, 0.2), 0.05, "T - mesto teleportacije", R_P_LEVO);
    Risalnik::narisi_besedilo(m_pisava, 0xffffffff, mat::vec2(-0.6, 0.3), 0.05, "ESC - premor", R_P_LEVO);

    if (m_b_meni.ali_je_miska_gor())
    {
        m_b_meni.barva_besedila.a = 0.5;
        if (Risalnik::miskin_gumb.levi_gumb)
        {
            konec();
            p_zacetna_scena->zacetek();
        }
    }
    else
        m_b_meni.barva_besedila.a = 1;

    m_naslov.pozicija = 0;
    m_naslov.pozicija.y -= .4f;
    m_naslov.narisi_me();
    m_b_meni.narisi_me();
    m_zvezdno_nebo.velikost = Risalnik::vel_platna;
    m_zvezdno_nebo.pozicija = 0;
}

void Navodila_scena::konec()
{
}
