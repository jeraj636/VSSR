#include "streznik_scena.h"
#include "risalnik.h"
#include "streznik.h"

Streznik_scena::Streznik_scena()
    : m_pisava("../sredstva/Cascadia.ttf", 55),
      m_gumb_za_vkop_izklop_streznika(m_pisava, 0xffffffff, mat::vec2(0), 55, "", R_P_X_SREDINA),
      m_gumb_za_na_meni(m_pisava, 0xffffffff, mat::vec2(0), 55, "Meni", R_P_X_SREDINA)
{
    m_ali_je_streznik_vklopljen = false;
}

void Streznik_scena::zacetek()
{
    //* Nastavljanje objektov
    m_zvezdno_nebo = Objekt_2D(mat::vec2(0), mat::vec2(0), 0, 0xffffffff, "../sredstva/nebo.png", true);
    m_polje_za_vpisovanje = Objekt_2D(mat::vec2(0), mat::vec2(200, 55), 0, 0xffffffff, "../sredstva/prazen.png", true);

    //* Nastavljanje miske
    Risalnik::kamera_3D.premikanje_kamere = false;
    Risalnik::nastavi_aktivnost_kazalca_miske(true);
    Risalnik::aktivna_scena_ptr = this;
}

void Streznik_scena::zanka()
{
    Risalnik::nastavi_testiranje_globine(false);

    m_zvezdno_nebo.velikost = Risalnik::dobi_velikost_okna();
    m_zvezdno_nebo.pozicija = Risalnik::dobi_velikost_okna() / 2;
    Risalnik::narisi_2D_objekt(m_zvezdno_nebo);

    Risalnik::narisi_besedilo(m_pisava, 0xffffffff, mat::vec2(Risalnik::dobi_velikost_okna().x / 2, 200), 55, "Streznik", R_P_X_SREDINA);

    m_polje_za_vpisovanje.pozicija = Risalnik::dobi_velikost_okna() / 2;
    m_polje_za_vpisovanje.pozicija.y -= 100;
    Risalnik::narisi_2D_objekt(m_polje_za_vpisovanje);

    if (m_polje_za_vpisovanje.ali_je_miska_gor())
    {
        m_polje_za_vpisovanje.barva.a = 0.5f;
        if (Risalnik::miskin_gumb.levi_gumb)
            Risalnik::buffer_za_vpis_podatkov = &m_st_porta;
    }
    else
        m_polje_za_vpisovanje.barva.a = 1.0f;

    Risalnik::narisi_besedilo(m_pisava, 0x000000ff, m_polje_za_vpisovanje.pozicija, 40, m_st_porta, R_P_X_SREDINA);

    m_gumb_za_vkop_izklop_streznika.pozicija = Risalnik::dobi_velikost_okna() / 2;
    m_gumb_za_vkop_izklop_streznika.pozicija.y += 100;

    if (m_ali_je_streznik_vklopljen)
    {
        m_gumb_za_vkop_izklop_streznika.niz = "Izklop";
    }
    else
        m_gumb_za_vkop_izklop_streznika.niz = "Vklop";

    if (m_gumb_za_vkop_izklop_streznika.ali_je_miska_gor())
    {
        m_gumb_za_vkop_izklop_streznika.barva_besedila.a = 0.5;
        if (Risalnik::miskin_gumb.levi_gumb && m_gumb_za_vkop_izklop_streznika.aktiven)
            if (!m_ali_je_streznik_vklopljen)
            {
                m_ali_je_streznik_vklopljen = Streznik::zazeni(atoi(m_st_porta.c_str()));
                m_gumb_za_vkop_izklop_streznika.cakaj_do = Cas::get_cas() + 1;
            }
            else
            {
                m_ali_je_streznik_vklopljen = false;
                Streznik::ugasni();
                m_gumb_za_vkop_izklop_streznika.cakaj_do = Cas::get_cas() + 1;
            }
    }
    else
        m_gumb_za_vkop_izklop_streznika.barva_besedila.a = 1;

    m_gumb_za_vkop_izklop_streznika.narisi_me();
    Risalnik::nastavi_testiranje_globine(true);
}

void Streznik_scena::konec()
{
}