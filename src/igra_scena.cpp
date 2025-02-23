/*
Opis: Funkcije razreda Scena
Avtor: Jakob Jeraj
Licenca: GNU GPL 3
*/

#include <iostream>
#include <fstream>
#include <sstream>

#include "igra_scena.h"
#include "risalnik.h"
#include "dnevnik.h"

Igra_scena::Igra_scena()
    : m_pisava("../sredstva/Cascadia.ttf", 80),
      m_gumb_za_na_meni(m_pisava, 0xffffffff, mat::vec2(0, 0.09), 0.06, "Meni", R_P_X_SREDINA | R_P_Y_SREDINA),
      m_gumb_za_nadaljevanje(m_pisava, 0xffffffff, mat::vec2(0, 0), 0.06, "Nadaljuj", R_P_X_SREDINA | R_P_Y_SREDINA)
{
    m_nebesna_kocka.nastavi({"../sredstva/neki.png",
                             "../sredstva/neki.png",
                             "../sredstva/neki.png",
                             "../sredstva/neki.png",
                             "../sredstva/neki.png",
                             "../sredstva/neki.png"},
                            mat::vec3(0), mat::vec3(0), mat::vec3(300));
    //* Nastavljanje osnovnih objektov
    Nasprotnik::raketa.nastavi(mat::vec3(0), mat::vec3(1), mat::vec3(1, 1, 1), 0xffffffff, true, "../sredstva/raketa.obj");
    m_zvezdno_nebo.nastavi(mat::vec2(0), mat::vec2(0), 0, 0xffffffff, "../sredstva/nebo.png", true, R_P_X_SREDINA | R_P_Y_SREDINA); // naredi sicer podobno (isto)

    m_vc_za_streljati.zadaj.nastavi(mat::vec2(0.6, 0.4), mat::vec2(0.3, 0.05), 0, 0x555555ff, "../sredstva/prazen.png", true, R_P_LEVO | R_P_Y_SREDINA);
    m_vc_za_streljati.spredaj.nastavi(mat::vec2(0.6, 0.4), mat::vec2(0.3, 0.05), 0, 0xffffffff, "../sredstva/prazen.png", true, R_P_LEVO | R_P_Y_SREDINA);

    m_vc_za_teleportirati.zadaj.nastavi(mat::vec2(0.6, 0.34), mat::vec2(0.3, 0.05), 0, 0x555555ff, "../sredstva/prazen.png", true, R_P_LEVO | R_P_Y_SREDINA);
    m_vc_za_teleportirati.spredaj.nastavi(mat::vec2(0.6, 0.34), mat::vec2(0.3, 0.05), 0, 0xffffffff, "../sredstva/prazen.png", true, R_P_LEVO | R_P_Y_SREDINA);

    m_vc_za_oziveti.zadaj.nastavi(mat::vec2(-0.4, 0.34), mat::vec2(0.8, 0.05), 0, 0x555555ff, "../sredstva/prazen.png", true, R_P_LEVO | R_P_Y_SREDINA);
    m_vc_za_oziveti.spredaj.nastavi(mat::vec2(-0.4, 0.34), mat::vec2(0.8, 0.05), 0, 0xffffffff, "../sredstva/prazen.png", true, R_P_LEVO | R_P_Y_SREDINA);

    m_srce.nastavi(mat::vec2(-0.8, 0.4), mat::vec2(0.05), 0, 0xff0000ff, "../sredstva/srce.png", true, R_P_X_SREDINA | R_P_Y_SREDINA);

    m_modri_kamen.nastavi(mat::vec3(0), mat::vec3(3), mat::vec3(0), 0xffffffff, true, "../sredstva/modra_zadeva.obj");

    //* Nastavljanje okolja
    //* Prebere se zemljevid (mapa) igre
    std::ifstream i_dat("../sredstva/kamni/podatki_o_kamnih.txt");
    if (!i_dat.is_open())
    {
        napaka("Ni datoteke ../sredstva/kamni/podatki_o_kamnih.txt\n");
        exit(1);
    }

    //* Branje kamnov iz datoteke
    for (int i = 0; i < 10; i++)
    {
        std::string s;
        std::getline(i_dat, s);
        mat::vec3 poz, rot;
        float vel;
        std::stringstream ss(s);
        ss >> poz.x >> poz.y >> poz.z >> vel >> rot.x >> rot.x >> rot.z;
        m_kamni1[i].nastavi(poz, mat::vec3(vel / 0.5), rot, 0xffffffff, true, "../sredstva/kamni/K1.obj");
        m_kamni1[i].hitrost = 0;
    }
    i_dat.close();

    /*
    !Ptiček
    *  ,
    * ()>
    * //
    * ''
    */

    //* Nastavljanje pocivalisc
    //* Zadeva deluje zelo kmečko:
    // Dva pocivalica sta na isti lokaciji samo, da je eden manjši.
    // S tem se ustvari učinek pristajanja
    for (int i = 0; i < 3; i++)
    {
        m_pocivalisce[i].nastavi(mat::vec3(0), mat::vec3(15), mat::vec3(0), 0xffffffff, true, "../sredstva/pocivalisce.obj");
    }
    for (int i = 3; i < 6; i++) //* Vem da je grdo, življenje ni potica
    {
        m_pocivalisce[i].nastavi(mat::vec3(0), mat::vec3(11), mat::vec3(0), 0xffffffff, true, "../sredstva/pocivalisce.obj");
    }
    m_pocivalisce[0].pozicija = mat::vec3(80, 90, 90);
    m_pocivalisce[1].pozicija = mat::vec3(0, -50, 0);
    m_pocivalisce[2].pozicija = mat::vec3(-40, 60, -90);
    m_pocivalisce[3].pozicija = mat::vec3(80, 90, 90);
    m_pocivalisce[4].pozicija = mat::vec3(0, -50, 0);
    m_pocivalisce[5].pozicija = mat::vec3(-40, 60, -90);

    //* Nastavljanje merilca
    // Merilec je sestavljen iz štirih črtic tako da tvorijo križec, ki ga je mogoče raztegniti ali skrčiti s pomočjo spremenjivke "m_razmik_merilca"
    m_velikost_merilca = mat::vec2(0.002, 0.02);
    for (int i = 0; i < 4; i++)
    {
        m_merilec[i].nastavi(mat::vec2(0, 0.0), m_velikost_merilca, i * 90, 0xffffffff, "../sredstva/merilec.png", true, R_P_ZGORAJ | R_P_LEVO);
    }
    m_merilec[0].pozicija = mat::vec2(0, m_razmik_merilca);
    m_merilec[1].pozicija = mat::vec2(m_razmik_merilca, 0);
    m_merilec[2].pozicija = mat::vec2(0, -m_razmik_merilca);
    m_merilec[3].pozicija = mat::vec2(-m_razmik_merilca, 0);
}

void Igra_scena::zacetek()
{
    //* Stanja
    m_pavza = false;
    m_ali_sem_umrl = false;
    m_sem_povezan = false;

    //* Števci
    m_st_uspesnih_zadetkov = 0;
    m_st_src = 3;
    m_vseh_st_src = 3;

    //* Casovniki
    m_cas_naslednjega_posiljanja_podatkov = 0;
    m_cas_za_se_sem_tu = 0;
    m_streznik_nazadnje_se_sem_tu = Cas::get_cas() + T_SE_SEM_TU_INTERVAL * 2;
    m_naslednje_streljanje = Cas::get_cas() + 1;
    m_naslednja_teleportacija = Cas::get_cas() + 2;
    m_naslednje_odvzetje_zivljenja_ob_trku = Cas::get_cas();
    m_naslednje_zdravljenje = Cas::get_cas();

    //* Nastavljanje kamere in kazalca
    Risalnik::kamera_3D.premikanje_kamere = true;
    Risalnik::nastavi_aktivnost_kazalca_miske(false);
    Risalnik::aktivna_scena_ptr = this;

    //* Nastavljanje hitrosti miske
    try
    {
        m_hitrost_miske = std::stoi(p_nastavitve_scena->m_hitrost_miske.niz);
    }
    catch (const std::exception &e)
    {
        m_hitrost_miske = 50;
    }
    Risalnik::kamera_3D.hitrost_miske = m_hitrost_miske;

    //* Nastavljanje teleportacije
    Risalnik::kamera_3D.nastavi();
    Risalnik::kamera_3D.pozicija = mat::vec3(0, 0, 30);
    m_teleportacija.pozicija = Risalnik::kamera_3D.pozicija;
    m_teleportacija.jaw = Risalnik::kamera_3D.yaw;
    m_teleportacija.pitch = Risalnik::kamera_3D.pitch;

    //* Povezava na streznik
    // Nastavljnje porta iz nastavitev
    int port;
    try
    {
        port = atoi(p_nastavitve_scena->m_vrata_odjemalca.niz.c_str());
    }
    catch (const std::exception &e)
    {
        port = -1;
    }

    //* Ker igrica deluje na podlagi UDP je tukaj deset poiskusov povezave
    for (int i = 0; i < 10; i++)
    {
        // Nastavljanje zacetnega sporočila
        int tip;
        if (p_nastavitve_scena->ali_sem_opazovalec)
        {
            tip = T_OPAZOVALEC;
            m_opazujem = true;
        }
        else
        {
            tip = T_ODJEMALEC;
            m_opazujem = false;
        }

        // Dejanski poizkus povezave na strežnik
        if (m_odjmalec.zazeni(p_nastavitve_scena->m_streznik.niz, port, tip) < 0) // Povezava neuspela
        {
            opozorilo("igra_scena.cpp :: Poskus povezave %i od %i ni uspel!\n", i + 1, 10);
        }
        else
            break; // uspešna povezava
    }

    if (m_odjmalec.id != -1) // če je "m_odjemalec_id" enak -1 pomeni, da je strežnik povezavo zavrnil
    {
        m_sem_povezan = true;
        std::thread nit(vzdrzuj_povezavo, this); //* Nit za branje iz vticnika
        nit.detach();
    }
    else // Vrnitev na zacetni meni
    {
        napaka("igra_scena.cpp :: Povezava ni uspela!\n");
        konec();
        p_zacena_scena->zacetek();
    }
}

// Glavna logika igre
void Igra_scena::zanka()
{
    //* Risanje in posodabljanje elementov

    // poseben meni če je pavza ali igralec je umrl
    if (m_pavza || m_ali_sem_umrl)
    {
        m_meteor.posodobi();
        Risalnik::nastavi_testiranje_globine(false);

        m_zvezdno_nebo.velikost = Risalnik::vel_platna;
        m_zvezdno_nebo.pozicija = mat::vec2(0); // Ni potrebno

        Risalnik::narisi_2D_objekt(m_zvezdno_nebo); //! Morda je potrebno spremeniti
        Risalnik::narisi_2D_objekt(m_meteor);
        Risalnik::nastavi_testiranje_globine(true);
    }
    else
    {
        // Drugace se izrisiuje nebesna kocka
        Risalnik::narisi_nebesno_kocko(m_nebesna_kocka);
    }

    //* Risanje zemljevida
    // Nastavljanje lastne pozicije rakete, da se zazna kolizijo z elementi zemljevida
    Nasprotnik::raketa.pozicija = Risalnik::kamera_3D.pozicija;
    Nasprotnik::raketa.pozicija.x *= -1;
    Nasprotnik::raketa.pozicija.y *= -1;
    Nasprotnik::raketa.pozicija.z *= -1;

    // Modri kamen
    Risalnik::narisi_3D_objekt(m_modri_kamen);
    if (!p_nastavitve_scena->ali_sem_opazovalec && Objekt_3D::trk(Nasprotnik::raketa, m_modri_kamen))
    {

        mat::vec3 smer_premika = mat::normaliziraj(m_modri_kamen.pozicija - Nasprotnik::raketa.pozicija);
        Risalnik::kamera_3D.pozicija += smer_premika * Cas::get_delta_cas() * 100;
    }
    // Rjav premikajoč kamen se lahko zaleti v modro kuglo
    for (int i = 10; i < 10; i++)
    {
        if (Objekt_3D::trk(m_kamni1[i], m_modri_kamen))
        {
            // Izračun novih smeri
            m_kamni1[i].smer = m_kamni1[i].smer * -1;
            m_kamni1[i].smer = m_kamni1[i].smer.normaliziraj();

            // Nekoliko hitrejši odboj
            m_kamni1[i].pozicija += m_kamni1[i].smer * m_kamni1[i].hitrost * Cas::get_delta_cas();
            m_kamni1[i].pozicija += m_kamni1[i].smer * m_kamni1[i].hitrost * Cas::get_delta_cas();
        }
    }

    // RIsanje rjavih kamnov
    for (int i = 0; i < 10; i++)
    {
        m_kamni1[i].pozicija += m_kamni1[i].smer * m_kamni1[i].hitrost * Cas::get_delta_cas();
        Risalnik::narisi_3D_objekt(m_kamni1[i]);

        //* Preverjanje trkov

        if (!p_nastavitve_scena->ali_sem_opazovalec && Objekt_3D::trk(Nasprotnik::raketa, m_kamni1[i]))
        {
            mat::vec3 smer_premika = mat::normaliziraj(m_kamni1[i].pozicija - Nasprotnik::raketa.pozicija);
            Risalnik::kamera_3D.pozicija += smer_premika * Cas::get_delta_cas() * 100;
            if (Cas::get_cas() >= m_naslednje_odvzetje_zivljenja_ob_trku)
            {
                m_naslednje_odvzetje_zivljenja_ob_trku = Cas::get_cas() + 2;
                m_st_src--;
            }
        }
        // Rjav premikajoč kamen se lahko zaleti v drug premikajpč rjav kamen
        for (int j = 10; j < 10; j++)
        {
            if (i != j && Objekt_3D::trk(m_kamni1[i], m_kamni1[j]))
            {
                // Izračun novih smeri
                m_kamni1[i].smer = m_kamni1[i].pozicija - m_kamni1[j].pozicija;
                m_kamni1[j].smer = m_kamni1[j].pozicija - m_kamni1[i].pozicija;
                m_kamni1[i].smer = m_kamni1[i].smer.normaliziraj();
                m_kamni1[j].smer = m_kamni1[j].smer.normaliziraj();

                // Nekoliko hitrejši odboj
                m_kamni1[i].pozicija += m_kamni1[i].smer * m_kamni1[i].hitrost * Cas::get_delta_cas();
                m_kamni1[i].pozicija += m_kamni1[i].smer * m_kamni1[i].hitrost * Cas::get_delta_cas();
                m_kamni1[j].pozicija += m_kamni1[j].smer * m_kamni1[j].hitrost * Cas::get_delta_cas();
                m_kamni1[j].pozicija += m_kamni1[j].smer * m_kamni1[j].hitrost * Cas::get_delta_cas();
            }
        }
    }

    // Risanje pocivalisc
    bool ali_se_zdravim = false;
    for (int i = 0; i < 3; i++)
    {
        Risalnik::narisi_3D_objekt(m_pocivalisce[i]);
        if (!p_nastavitve_scena->ali_sem_opazovalec && Objekt_3D::trk(Nasprotnik::raketa, m_pocivalisce[i]))
        {
            ali_se_zdravim = true;
            if (m_st_src < m_vseh_st_src && Risalnik::dobi_tipko('H') && Cas::get_cas() >= m_naslednje_zdravljenje) //* Najbolj kmecka funkcionalnost zdravljenja!
            {
                m_st_src++;
                m_naslednje_zdravljenje = Cas::get_cas() + 2;
            }
        }

        for (int i = 10; i < 10; i++)
        {
            if (Objekt_3D::trk(m_kamni1[i], m_pocivalisce[i]))
            {
                // Izračun novih smeri
                m_kamni1[i].smer = m_pocivalisce[i].pozicija - m_kamni1[i].pozicija;
                m_kamni1[i].smer = m_kamni1[i].smer.normaliziraj();

                // Nekoliko hitrejši odboj
                m_kamni1[i].pozicija += m_kamni1[i].smer * m_kamni1[i].hitrost * Cas::get_delta_cas();
                m_kamni1[i].pozicija += m_kamni1[i].smer * m_kamni1[i].hitrost * Cas::get_delta_cas();
            }
        }
    }
    // Da igralec ne more noter v pocivalisce
    for (int i = 3; i < 6; i++)
    {
        if (!p_nastavitve_scena->ali_sem_opazovalec && Objekt_3D::trk(Nasprotnik::raketa, m_pocivalisce[i]))
        {
            mat::vec3 smer_premika = mat::normaliziraj(m_pocivalisce[i].pozicija - Nasprotnik::raketa.pozicija);
            Risalnik::kamera_3D.pozicija += smer_premika * Cas::get_delta_cas() * 100;
        }
    }
    if (!m_pavza && !m_ali_sem_umrl && !p_nastavitve_scena->ali_sem_opazovalec)
    {
        //* Risanje merilca
        if (Risalnik::miskin_gumb.desni_gumb)
        {
            m_razmik_merilca = MERIM_RAZMIK_MERILCA;
            Risalnik::kamera_3D.vidno_polje = OD_BLIZU;
            Risalnik::kamera_3D.hitrost_miske = m_hitrost_miske;
        }
        else
        {
            m_razmik_merilca = STOJIM_RAZMIK_MERILCA;
            Risalnik::kamera_3D.vidno_polje = OD_DALEC;
            //! Ce se mi bo ljubilo lahko dodam spodnjo vrstico v nižje pogoje
            Risalnik::kamera_3D.hitrost_miske = m_hitrost_miske;
        }

        if (Risalnik::dobi_tipko('W') ||
            Risalnik::dobi_tipko('A') ||
            Risalnik::dobi_tipko('S') ||
            Risalnik::dobi_tipko('D'))
        {
            m_razmik_merilca = POCASEN_RAZMIK_MERILCA;
            Risalnik::kamera_3D.vidno_polje = OD_DALEC;

            if (Risalnik::dobi_tipko(340) /*SHIFT*/)
            {
                m_razmik_merilca = HITER_RAZMIK_MERILCA;
            }
        }

        m_merilec[0].pozicija = mat::vec2(0, m_razmik_merilca);
        m_merilec[1].pozicija = mat::vec2(m_razmik_merilca, 0);
        m_merilec[2].pozicija = mat::vec2(0, -m_razmik_merilca);
        m_merilec[3].pozicija = mat::vec2(-m_razmik_merilca, 0);

        for (int i = 0; i < 4; i++)
        {
            Risalnik::narisi_2D_objekt(m_merilec[i]);
        }

        //* Risanje src
        mat::vec2 poz_prvega_srca = m_srce.pozicija;
        for (int i = 0; i < m_vseh_st_src; i++)
        {
            if (i > m_st_src - 1)
                m_srce.barva = 0x555555ff;

            Risalnik::narisi_2D_objekt(m_srce);
            m_srce.pozicija.x += 0.05;
        }
        m_srce.barva = 0xff0000ff;
        m_srce.pozicija = poz_prvega_srca;

        //* Streljanje
        if (Risalnik::miskin_gumb.levi_gumb && m_naslednje_streljanje < Cas::get_cas() && !ali_se_zdravim)
        {
            m_naslednje_streljanje = Cas::get_cas() + 0.5;

            m_izstrelki.push_back(Izstrelek());
            m_izstrelki.back().oblika.pozicija = Risalnik::kamera_3D.pozicija * -1;

            m_izstrelki.back().smer = Risalnik::kamera_3D.smer;
            m_vc_za_streljati.spredaj.velikost.x = 0;

            //* Posiljanje strezniku da sem ustrelil
            // Malo grda koda ampak je kul :)
            char buff[128];
            buff[0] = T_POSILJAM_METEK;
            memcpy(&buff[1], (char *)&m_odjmalec.id, sizeof(m_odjmalec.id));
            memcpy(&buff[5], (char *)&m_izstrelki.back().oblika.pozicija, sizeof(mat::vec3));
            memcpy(&buff[5 + sizeof(mat::vec3)], (char *)&m_izstrelki.back().smer, sizeof(mat::vec3));
            m_odjmalec.poslji(buff, 6 + 2 * sizeof(mat::vec3));

            // Reciol
            // Ce je treba
            Risalnik::kamera_3D.pitch -= 3;
        }

        //* Risanje in izracun vidnih stevcev
        // za streljanje
        m_vc_za_streljati.spredaj.velikost.x += m_vc_za_streljati.zadaj.velikost.x * Cas::get_delta_cas() * 2;
        m_vc_za_streljati.spredaj.velikost.x = mat::obrezi_st(m_vc_za_streljati.spredaj.velikost.x, 0.0f, m_vc_za_streljati.zadaj.velikost.x);
        Risalnik::narisi_2D_objekt(m_vc_za_streljati.spredaj);
        Risalnik::narisi_2D_objekt(m_vc_za_streljati.zadaj);
        // za teleportacijo
        m_vc_za_teleportirati.spredaj.velikost.x += m_vc_za_teleportirati.zadaj.velikost.x * Cas::get_delta_cas() / 1.7 /*Korekcija če slučajno zalega*/;
        m_vc_za_teleportirati.spredaj.velikost.x = mat::obrezi_st(m_vc_za_teleportirati.spredaj.velikost.x, 0.0f, m_vc_za_teleportirati.zadaj.velikost.x);
        Risalnik::narisi_2D_objekt(m_vc_za_teleportirati.spredaj);
        Risalnik::narisi_2D_objekt(m_vc_za_teleportirati.zadaj);

        Risalnik::narisi_besedilo(m_pisava, 0xffffffff, mat::vec2(-.9f, -.9f), 0.1, std::to_string(m_st_uspesnih_zadetkov), R_P_X_SREDINA | R_P_ZGORAJ);
    }

    //* Izris tujih izstrelkov
    for (int i = 0; i < m_tuji_izstrelki.size(); i++)
    {
        // Brisnje izstelov
        bool uporaben = true;
        uporaben = (Objekt_3D::trk(m_modri_kamen, m_tuji_izstrelki[i].oblika)) ? false : true;
        for (int j = 0; j < 10; j++)
            if (Objekt_3D::trk(m_kamni1[j], m_tuji_izstrelki[i].oblika))
            {
                uporaben = false;
            }
        int st_trkov_z_naspotniki = 0;
        for (int k = 0; k < nasprotniki.size(); k++)
        {
            Nasprotnik::raketa.pozicija = nasprotniki[k].tr_pozicija;
            Nasprotnik::raketa.rotacija = nasprotniki[k].tr_rotacija;
            Nasprotnik::raketa.veliksot = 1;
            if (Objekt_3D::trk(m_tuji_izstrelki[i].oblika, Nasprotnik::raketa))
            {
                st_trkov_z_naspotniki++;
            }
        }
        if (st_trkov_z_naspotniki > 1) //* Ker se lahko zaleti v tistega, ki ga je izstrelil
            uporaben = false;

        // Brisanje iz tabele
        if (m_tuji_izstrelki[i].sem_neuporaben() || !uporaben)
        {
            std::swap(m_tuji_izstrelki[i], m_tuji_izstrelki.back());
            m_tuji_izstrelki.pop_back();
            continue;
        }
        // Risanje izstrelkov
        m_tuji_izstrelki[i].posodobi();
        Risalnik::narisi_3D_objekt(m_tuji_izstrelki[i].oblika);
    }

    //* Risanje svojih izstrelkov
    for (int i = 0; i < m_izstrelki.size(); i++)
    {
        // Brisnje izstrelkov
        bool uporaben = true;
        uporaben = (Objekt_3D::trk(m_modri_kamen, m_izstrelki[i].oblika)) ? false : true;
        for (int j = 0; j < 10; j++)
            if (Objekt_3D::trk(m_kamni1[j], m_izstrelki[i].oblika))
            {
                uporaben = false;
            }

        // Ali je zadeta kakšnja ladja
        for (int k = 0; k < nasprotniki.size(); k++)
        {
            Nasprotnik::raketa.pozicija = nasprotniki[k].tr_pozicija;
            Nasprotnik::raketa.rotacija = nasprotniki[k].tr_rotacija;
            Nasprotnik::raketa.veliksot = 1;
            if (uporaben && Objekt_3D::trk(m_izstrelki[i].oblika, Nasprotnik::raketa))
            {
                // Posiljanje streznuku da sem zadel nakoga
                char buff[10];
                buff[0] = T_USTRELIL;
                memcpy(&buff[1], (char *)&nasprotniki[k].id, sizeof(nasprotniki[k].id));
                m_odjmalec.poslji(buff, 5);
                // Zdaj je metek neuporaben
                uporaben = false;
                m_st_uspesnih_zadetkov++;
            }
        }
        // Brisanje neuporabnih izstrelkov iz tabeles
        if (m_izstrelki[i].sem_neuporaben() || !uporaben)
        {
            std::swap(m_izstrelki[i], m_izstrelki.back());
            m_izstrelki.pop_back();
            continue;
        }
        // Izrsis izstrelkov
        m_izstrelki[i].posodobi();
        Risalnik::narisi_3D_objekt(m_izstrelki[i].oblika);
    }

    //* Risanje nasprotnikov
    for (int i = 0; i < nasprotniki.size(); i++)
    {
        //! Interpolacija
        // pozicija
        mat::vec3 smer = mat::vec3(0) - (nasprotniki[i].pr_pozicija - nasprotniki[i].tr_pozicija);
        smer = smer.normaliziraj();
        std::cout << nasprotniki[i].hitrost << "   " << Risalnik::kamera_3D.hitrost_premikanja << "\n";
        nasprotniki[i].pozicija += smer * Cas::get_delta_cas() * nasprotniki[i].hitrost;

        // rotacija
        smer = nasprotniki[i].tr_rotacija - nasprotniki[i].pr_rotacija;
        // smer = smer.normaliziraj();
        if (smer.dolzina() == 0)
            Nasprotnik::raketa.rotacija = nasprotniki[i].tr_rotacija;
        else
        {
            nasprotniki[i].pr_rotacija += smer * Risalnik::kamera_3D.hitrost_miske * Cas::get_delta_cas();
            Nasprotnik::raketa.rotacija = nasprotniki[i].pr_rotacija;
        }
        Nasprotnik::raketa.pozicija = nasprotniki[i].pozicija;
        // Nasprotnik::raketa.rotacija = nasprotniki[i].tr_rotacija;
        Risalnik::narisi_3D_objekt(Nasprotnik::raketa);
    }
    if (m_st_src == 0)
    {
        m_st_src = 3;
        m_ali_sem_umrl = true;
        m_cas_do_ozivetja = 10 + Cas::get_cas();
        m_vc_za_oziveti.spredaj.velikost.x = 0;
    }
    //* Ce je igralec mrtev
    if (m_ali_sem_umrl)
    {
        Risalnik::kamera_3D.premikanje_kamere = false;
        Risalnik::nastavi_aktivnost_kazalca_miske(true);
        Risalnik::kamera_3D.pozicija = mat::vec3(1000);
        if (m_cas_do_ozivetja <= Cas::get_cas())
        {
            m_ali_sem_umrl = false;

            Risalnik::kamera_3D.nastavi();
            Risalnik::kamera_3D.pozicija = mat::vec3(0, 0, 30);
            m_st_src = 3;
            if (!m_pavza)
            {

                Risalnik::kamera_3D.premikanje_kamere = true;
                Risalnik::nastavi_aktivnost_kazalca_miske(false);
            }
        }
        Risalnik::narisi_besedilo(m_pisava, 0xffffffff, mat::vec2(0, -.1), 0.09, "Umrl si!", R_P_X_SREDINA | R_P_Y_SREDINA);

        m_vc_za_oziveti.spredaj.velikost.x += m_vc_za_oziveti.zadaj.velikost.x * Cas::get_delta_cas() / 10;
        m_vc_za_oziveti.spredaj.velikost.x = mat::obrezi_st(m_vc_za_oziveti.spredaj.velikost.x, 0.0f, m_vc_za_oziveti.zadaj.velikost.x);
        Risalnik::narisi_2D_objekt(m_vc_za_oziveti.spredaj);
        Risalnik::narisi_2D_objekt(m_vc_za_oziveti.zadaj);
    }

    //* Preverjanje stanj gumbov
    if (Risalnik::dobi_tipko(256 /*Tipka ESC*/))
    {
        m_pavza = true;
        Risalnik::kamera_3D.premikanje_kamere = false;
        Risalnik::nastavi_aktivnost_kazalca_miske(true);
    }
    if (Risalnik::dobi_tipko('R') && (m_naslednja_teleportacija <= Cas::get_cas() || p_nastavitve_scena->ali_sem_opazovalec))
    {
        Risalnik::kamera_3D.pozicija = m_teleportacija.pozicija;
        Risalnik::kamera_3D.yaw = m_teleportacija.jaw;
        Risalnik::kamera_3D.pitch = m_teleportacija.pitch;
        m_naslednja_teleportacija += 2;
        m_vc_za_teleportirati.spredaj.velikost.x = 0;
    }
    if (Risalnik::dobi_tipko('T') && !p_nastavitve_scena->ali_sem_opazovalec)
    {
        m_teleportacija.pozicija = Risalnik::kamera_3D.pozicija;
        m_teleportacija.jaw = Risalnik::kamera_3D.yaw;
        m_teleportacija.pitch = Risalnik::kamera_3D.pitch;
    }
    //* Premor
    if (m_pavza && !m_ali_sem_umrl)
    {
        Risalnik::narisi_besedilo(m_pisava, 0xffffffff, mat::vec2(0, -.1), 0.09, "Premor", R_P_X_SREDINA | R_P_Y_SREDINA);

        if (m_gumb_za_na_meni.ali_je_miska_gor())
        {
            m_gumb_za_na_meni.barva_besedila.a = 0.5;
            if (Risalnik::miskin_gumb.levi_gumb)
            {
                konec();
                p_zacena_scena->zacetek();
            }
        }

        m_gumb_za_na_meni.barva_besedila.a = 1;

        if (m_gumb_za_nadaljevanje.ali_je_miska_gor())
        {
            m_gumb_za_nadaljevanje.barva_besedila.a = 0.5;
            if (Risalnik::miskin_gumb.levi_gumb)
            {
                Risalnik::kamera_3D.premikanje_kamere = true;
                Risalnik::nastavi_aktivnost_kazalca_miske(false);
                m_pavza = false;
            }
        }
        else
            m_gumb_za_nadaljevanje.barva_besedila.a = 1;

        m_gumb_za_nadaljevanje.narisi_me();
        m_gumb_za_na_meni.narisi_me();
    }

    //* Komunikacija s streznikom
    // Posiljanje se sem tu
    if (m_cas_za_se_sem_tu <= Cas::get_cas())
    {
        m_cas_za_se_sem_tu = Cas::get_cas() + T_SE_SEM_TU_INTERVAL;
        char buff[10];
        buff[0] = T_O_SE_SEM_TU;
        memcpy(buff + 1, (char *)&m_odjmalec.id, 4);
        if (m_opazujem)
            buff[5] = T_OPAZOVALEC;
        else
            buff[5] = T_ODJEMALEC;
        m_odjmalec.poslji(buff, 6);
    }

    // Posiljanje svojih podatkov
    if (m_cas_naslednjega_posiljanja_podatkov <= Cas::get_cas() && !m_opazujem)
    {
        m_cas_naslednjega_posiljanja_podatkov += T_HITROST_POSILJANJA_PODATKOV;
        char buff[256];
        buff[0] = T_PODATKI_IGRALCA;
        int poz = 1;
        memcpy(buff + poz, (char *)&m_odjmalec.id, sizeof(m_odjmalec.id));
        poz += sizeof(m_odjmalec.id);
        memcpy(buff + poz, (char *)&Risalnik::kamera_3D.pozicija, sizeof(Risalnik::kamera_3D.pozicija));
        poz += sizeof(Risalnik::kamera_3D.pozicija);
        memcpy(buff + poz, (char *)&Risalnik::kamera_3D.rotacija, sizeof(Risalnik::kamera_3D.rotacija));
        poz += sizeof(Risalnik::kamera_3D.rotacija);
        memcpy(buff + poz, (char *)&Risalnik::kamera_3D.hitrost_premikanja, sizeof(Risalnik::kamera_3D.hitrost_premikanja));
        poz += sizeof(Risalnik::kamera_3D.hitrost_premikanja);

        m_odjmalec.poslji(buff, poz);
    }
    // Strežnik že 4-krat ni poslal še sem tu
    if (m_streznik_nazadnje_se_sem_tu + T_SE_SEM_TU_INTERVAL * 10 < Cas::get_cas())
    {
        napaka("igra_scena.cpp :: Streznik se ne odziva!\n");
        konec();
        p_zacena_scena->zacetek();
    }

    if (!m_sem_povezan && !m_pavza /*Ko je pavza == true se kliče konec preko gumba ne pa če se povezava prekine*/) //* Če se povezava s strežnikom prekine se vrne na glavni meni
    {
        konec();
        p_zacena_scena->zacetek();
        sporocilo("igra_scena.cpp :: Konec!\n");
    }
}
void Igra_scena::vzdrzuj_povezavo(Igra_scena *is)
{
    char buff[512] = {0};
    while (is->m_sem_povezan)
    {
        int n = is->m_odjmalec.prejmi(buff);

        if (n == -1)
            continue;

        if (buff[0] == T_S_SE_SEM_TU)
        {
            is->m_streznik_nazadnje_se_sem_tu = Cas::get_cas();
        }
        if (buff[0] == T_USTRELJEN)
        {
            is->m_st_src--;
            if (is->m_st_src == 0)
            {
                is->m_ali_sem_umrl = true;
                is->m_cas_do_ozivetja = 10 + Cas::get_cas();
                is->m_vc_za_oziveti.spredaj.velikost.x = 0;
            }
        }
        if (buff[0] == T_POSILJAM_KAMNE)
        {
            sporocilo("S posiljam kamne!\n");
            int kateri_kamen = 0;
            int poz = 1;
            memcpy((char *)&kateri_kamen, &buff[poz], sizeof(kateri_kamen));
            poz += sizeof(kateri_kamen);
            memcpy((char *)&is->m_kamni1[kateri_kamen].pozicija, &buff[poz], sizeof(mat::vec3));
            poz += sizeof(mat::vec3);
            memcpy((char *)&is->m_kamni1[kateri_kamen].smer, &buff[poz], sizeof(mat::vec3));
            poz += sizeof(mat::vec3);
            memcpy((char *)&is->m_kamni1[kateri_kamen].rotacija, &buff[poz], sizeof(mat::vec3));
            poz += sizeof(mat::vec3);
            memcpy((char *)&is->m_kamni1[kateri_kamen].hitrost, &buff[poz], sizeof(float));
            poz += sizeof(float);
        }
        if (buff[0] == T_POSILJAM_METEK)
        {
            sporocilo("S posiljam podatke o metku\n");
            is->m_tuji_izstrelki.push_back(Izstrelek());
            memcpy((char *)&is->m_tuji_izstrelki.back().oblika.pozicija, &buff[5], sizeof(mat::vec3));
            memcpy((char *)&is->m_tuji_izstrelki.back().smer, &buff[5 + sizeof(mat::vec3)], sizeof(mat::vec3));
        }
        /*
        Ko strežnik pošlje podatke o igralcih so 4 možnisti:
        1. Igralcev je enako število kot v moji tabeli
            le posodobitev pozicij in rotacije
        2. Igralcev je več kot v moji tabeli
            dodajanje novih igralcev v tabel in posodabljanje
        3. Igralcev je manj kot v moji tabeli
            brisanje igralcev iz tabele in posodabljanje ostalih
        4. 2. in 3. točka skupaj
            Potrebno bo dodajati in izbrisovati nasprotnike
        */
        if (buff[0] == T_PODATKI_O_IGRALCIH)
        {
            sporocilo("S Podatki igralcev\n");

            for (int i = 0; i < is->nasprotniki.size(); i++) // nastavljnje da noben od naspotnikov še ni bil prebran v tej zanki
                is->nasprotniki[i].prebran = false;

            int vel; // služi da se ve koliko je poslanih nasprotnikov

            int poz = 1; // Pozicija v bufferju

            memcpy((char *)&vel, buff + poz, sizeof(vel)); // branje števila nasprotnikov
            poz += sizeof(vel);

            for (int i = 0; i < vel; i++)
            {
                // Vsak podatek o nasprotniku je sestavljen iz njegovega ID, pozicije in rotacije
                int id;
                mat::vec3 pozicija;
                mat::vec3 rotacija;
                float hitrost;
                memcpy((char *)&id, buff + poz, sizeof(id));
                poz += sizeof(id);
                memcpy((char *)&pozicija, buff + poz, sizeof(pozicija));
                poz += sizeof(pozicija);
                memcpy((char *)&rotacija, buff + poz, sizeof(rotacija));
                poz += sizeof(rotacija);
                memcpy((char *)&hitrost, buff + poz, sizeof(hitrost));
                poz += sizeof(hitrost);
                bool ali_je_v_tabeli = false;
                for (int j = 0; j < is->nasprotniki.size(); j++)
                {
                    if (id == is->nasprotniki[j].id) // Posodabljanje nasprotnikov
                    {
                        is->nasprotniki[j].pr_pozicija = is->nasprotniki[j].tr_pozicija;
                        is->nasprotniki[j].pozicija = is->nasprotniki[j].tr_pozicija;
                        is->nasprotniki[j].tr_pozicija = pozicija;

                        is->nasprotniki[j].pr_rotacija = is->nasprotniki[j].tr_rotacija;
                        is->nasprotniki[j].tr_rotacija = rotacija;
                        is->nasprotniki[j].hitrost = hitrost;
                        is->nasprotniki[j].prebran = true;
                        ali_je_v_tabeli = true;
                    }
                }

                if (!ali_je_v_tabeli) // Ustvarjanje novega nasprotnika v primeru če tega ni v tabeli (nov nasprotnik)
                {
                    Nasprotnik nov_nasprotnik;
                    is->nasprotniki.push_back(nov_nasprotnik);
                    is->nasprotniki.back().id = id;
                    is->nasprotniki.back().tr_pozicija = pozicija;
                    is->nasprotniki.back().pr_pozicija = pozicija;
                    is->nasprotniki.back().tr_rotacija = rotacija;
                    is->nasprotniki.back().pr_rotacija = rotacija;
                    is->nasprotniki.back().prebran = true;
                    is->nasprotniki.back().hitrost = hitrost;
                }
            }

            for (int i = 0; i < is->nasprotniki.size(); i++) // brisanje neprebranih nasprotnikov teh namreč ni več v igri
            {
                if (!is->nasprotniki[i].prebran)
                {
                    std::swap(is->nasprotniki[i], is->nasprotniki.back());
                    is->nasprotniki.pop_back();
                }
            }
        }
    }
}

void Igra_scena::konec()
{
    m_sem_povezan = false;
    while (nasprotniki.size() != 0)
        nasprotniki.pop_back();
    char buff[10];
    buff[0] = T_ZAPUSCAM;
    memcpy(buff + 1, (char *)&m_odjmalec.id, sizeof(m_odjmalec.id));
    m_odjmalec.poslji(buff, 5);

    m_odjmalec.ustavi(); //* Konec povezave
}