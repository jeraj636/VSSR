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
    m_zvezdno_nebo = Objekt_2D(mat::vec2(0), mat::vec2(0), 0, 0xffffffff, "../sredstva/nebo.png", true, R_P_X_SREDINA | R_P_Y_SREDINA);
    m_polje_za_vpisovanje = Objekt_2D(mat::vec2(0), mat::vec2(200, 55), 0, 0xffffffff, "../sredstva/prazen.png", true, R_P_X_SREDINA | R_P_Y_SREDINA);

    //* Nastavljanje miske
    Risalnik::kamera_3D.premikanje_kamere = false;
    Risalnik::nastavi_aktivnost_kazalca_miske(true);
    Risalnik::aktivna_scena_ptr = this;
}

void Streznik_scena::zanka()
{
    Risalnik::nastavi_testiranje_globine(false);

    // Risanje odzadja
    m_zvezdno_nebo.velikost = Risalnik::dobi_velikost_okna();
    m_zvezdno_nebo.pozicija = Risalnik::dobi_velikost_okna() / 2;
    Risalnik::narisi_2D_objekt(m_zvezdno_nebo);

    // Naslov
    Risalnik::narisi_besedilo(m_pisava, 0xffffffff, mat::vec2(Risalnik::dobi_velikost_okna().x / 2, 200), 55, "Streznik", R_P_X_SREDINA);

    // Kvadrat za vpis
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
        m_polje_za_vpisovanje.barva.a = 0.8f;

    // Vpisano besedilo
    Risalnik::narisi_besedilo(m_pisava, 0x000000ff, mat::vec2(m_polje_za_vpisovanje.pozicija.x - m_polje_za_vpisovanje.velikost.x / 2, m_polje_za_vpisovanje.pozicija.y), 40, m_st_porta, R_P_LEVO | R_P_Y_SREDINA);

    m_gumb_za_vkop_izklop_streznika.pozicija = Risalnik::dobi_velikost_okna() / 2;
    m_gumb_za_vkop_izklop_streznika.pozicija.y += 100;

    // gumb za streznik (nastavljanje besedila)
    if (m_ali_je_streznik_vklopljen)
    {
        m_gumb_za_vkop_izklop_streznika.niz = "Izklop";
    }
    else
        m_gumb_za_vkop_izklop_streznika.niz = "Vklop";

    // gumb za streznik
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

    m_gumb_za_na_meni.pozicija = Risalnik::dobi_velikost_okna() / 2;
    m_gumb_za_na_meni.pozicija.y += 300;

    if (m_gumb_za_na_meni.ali_je_miska_gor())
    {
        m_gumb_za_na_meni.barva_besedila.a = 0.5f;
        if (Risalnik::miskin_gumb.levi_gumb)
        {
            konec();
            Risalnik::aktivna_scena_ptr = p_zacena_scena;
        }
    }
    else
        m_gumb_za_na_meni.barva_besedila.a = 1.0f;
    m_gumb_za_na_meni.narisi_me();
    Risalnik::nastavi_testiranje_globine(true);
}

void Streznik_scena::konec()
{
}