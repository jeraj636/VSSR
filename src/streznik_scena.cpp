#include "streznik_scena.h"
#include "risalnik.h"

Streznik_scena::Streznik_scena()
    : m_pisava("../sredstva/Cascadia.ttf", 55),
      m_gumb_za_vkop_izklop_streznika(m_pisava, 0xffffffff, mat::vec2(0), 55, "", R_P_X_SREDINA)
{
}

void Streznik_scena::zacetek()
{
    // morda malo nenavadno zdaj sem ze pozabil kaj je nenavadno
    m_zvezdno_nebo = Objekt_2D(mat::vec2(0), mat::vec2(0), 0, 0xffffffff, "../sredstva/nebo.png", true);
    Risalnik::kamera_3D.premikanje_kamere = true;
    Risalnik::aktivnost_kazalca_miske(false);
    Risalnik::aktivna_scena_ptr = this;
}

void Streznik_scena::zanka()
{
    Risalnik::nastavi_testiranje_globine(false);
    Risalnik::narisi_2D_objekt(m_zvezdno_nebo);
    Risalnik::nastavi_testiranje_globine(true);

    m_zvezdno_nebo.velikost = Risalnik::dobi_velikost_okna();
    m_zvezdno_nebo.pozicija = Risalnik::dobi_velikost_okna() / 2;

    Risalnik::narisi_besedilo(m_pisava, 0xffffffff, mat::vec2(Risalnik::dobi_velikost_okna().x / 2, 200), 55, "Streznik", R_P_X_SREDINA);
    //    Risalnik::narisi_3D_objekt(m_raketa);
}

void Streznik_scena::konec()
{
}