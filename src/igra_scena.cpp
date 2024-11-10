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
    Nasprotnik::raketa.nastavi(mat::vec3(0), mat::vec3(1), mat::vec3(1, 1, 1), 0xffffffff, true, "../sredstva/raketa.obj");
    m_zvezdno_nebo.nastavi(mat::vec2(0), mat::vec2(0), 0, 0xffffffff, "../sredstva/nebo.png", true, R_P_X_SREDINA | R_P_Y_SREDINA); // naredi sicer podobno (isto)
    m_srce.nastavi(mat::vec2(-0.8, 0.4), mat::vec2(0.05), 0, 0xff0000ff, "../sredstva/srce.png", true, R_P_X_SREDINA | R_P_Y_SREDINA);
    //* Nastavljanje okolja
    //* Prebere se zemljevid (mapa) igre
    std::ifstream i_dat("../sredstva/kamni/podatki_o_kamnih.txt");
    if (!i_dat.is_open())
    {
        std::cout << "Ni datoteke ../sredstva/kamni/podatki_o_kamnih.txt\n";
        exit(1);
    }

    for (int i = 0; i < 10; i++)
    {
        std::string s;
        std::getline(i_dat, s);
        mat::vec3 poz, rot;
        float vel;
        std::stringstream ss(s);
        ss >> poz.x >> poz.y >> poz.z >> vel >> rot.x >> rot.x >> rot.z;
        m_kamni1[i].nastavi(poz, mat::vec3(vel / 0.5), rot, 0xffffffff, true, "../sredstva/kamni/K1.obj");
    }

    m_velikost_merilca = mat::vec2(0.002, 0.02);
    for (int i = 0; i < 4; i++)
    {
        m_merilec[i].nastavi(mat::vec2(0, 0.0), m_velikost_merilca, i * 90, 0xffffffff, "../sredstva/merilec.png", true, R_P_ZGORAJ | R_P_LEVO);
    }
    m_merilec[0].pozicija = mat::vec2(0, m_razmik_merilca);
    m_merilec[1].pozicija = mat::vec2(m_razmik_merilca, 0);
    m_merilec[2].pozicija = mat::vec2(0, -m_razmik_merilca);
    m_merilec[3].pozicija = mat::vec2(-m_razmik_merilca, 0);

    i_dat.close();
}

void Igra_scena::zacetek()
{
    m_pavza = false;
    m_cas_naslednjega_posiljanja_podatkov = 0;
    m_cas_za_se_sem_tu = 0;
    m_streznik_nazadnje_se_sem_tu = Cas::get_cas() + T_SE_SEM_TU_INTERVAL * 2;
    m_sem_povezan = false;
    m_naslednje_streljanje = Cas::get_cas() + 1;

    //* Nastavljanje kamere in kazalca
    Risalnik::kamera_3D.premikanje_kamere = true;
    Risalnik::nastavi_aktivnost_kazalca_miske(false);
    Risalnik::aktivna_scena_ptr = this;
    Risalnik::kamera_3D.hitrost_miske = std::stoi(p_nastavitve_scena->m_hitrost_miske.niz);

    m_st_src = 3;
    m_vseh_st_src = 3;

    m_ali_sem_umrl = false;
    //* Povezava na streznik
    for (int i = 0; i < 10; i++)
    {
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
        if (m_odjmalec.zazeni(p_nastavitve_scena->m_streznik.niz, atoi(p_nastavitve_scena->m_vrata_odjemalca.niz.c_str()), tip) < 0)
        {
            opozorilo("igra_scena.cpp :: Poskus povezave %i od %i ni uspel!\n", i + 1, 10);
        }
        else
            break;
    }

    if (m_odjmalec.id != -1)
    {
        m_sem_povezan = true;
        std::thread nit(vzdrzuj_povezavo, this); //* Nit za branje iz vticnika
        nit.detach();
    }
    else
    {
        napaka("igra_scena.cpp :: Povezava ni uspela!\n");
        konec();
        p_zacena_scena->zacetek();
    }
}
#include <iostream>
void Igra_scena::zanka()
{
    //* Risanje in posodabljanje elementov
    Risalnik::nastavi_testiranje_globine(false);
    Risalnik::narisi_2D_objekt(m_zvezdno_nebo);

    Risalnik::nastavi_testiranje_globine(true);

    m_zvezdno_nebo.velikost = Risalnik::vel_platna;
    m_zvezdno_nebo.pozicija = mat::vec2(0);

    for (int i = 0; i < 10; i++) //* Risanje zemljevida
    {
        Risalnik::narisi_3D_objekt(m_kamni1[i]);

        Nasprotnik::raketa.pozicija = Risalnik::kamera_3D.pozicija;
        Nasprotnik::raketa.pozicija.x *= -1;
        Nasprotnik::raketa.pozicija.y *= -1;
        Nasprotnik::raketa.pozicija.z *= -1;

        if (Objekt_3D::trk(Nasprotnik::raketa, m_kamni1[i]))
        {
            Risalnik::kamera_3D.premakni_nazaj();
            std::cout << "trk\n";
        }
    }

    if (!m_pavza)
    {
        //* Risanje merilca
        if (Risalnik::miskin_gumb.desni_gumb || Risalnik::miskin_gumb.levi_gumb)
        {
            m_razmik_merilca = MERIM_RAZNIK_MERILCA;
            Risalnik::kamera_3D.vidno_polje = OD_BLIZU;
        }
        else
        {
            m_razmik_merilca = STOJIM_RAZMIK_MERILCA;
            Risalnik::kamera_3D.vidno_polje = OD_DALEC;
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
        if (Risalnik::miskin_gumb.levi_gumb && m_naslednje_streljanje < Cas::get_cas())
        {
            m_naslednje_streljanje = Cas::get_cas() + 1;

            m_izstrelki.push_back(Izstrelek());
            m_izstrelki.back().oblika.pozicija = Risalnik::kamera_3D.pozicija * -1;
            m_izstrelki.back().oblika.pozicija += Risalnik::kamera_3D.smer * -1 * 2;

            m_izstrelki.back().smer = Risalnik::kamera_3D.smer;
        }
    }

    //* Risanje izstrelkov
    for (int i = 0; i < m_izstrelki.size(); i++)
    {

        bool uporaben = true;
        for (int j = 0; j < 10; j++)
            if (Objekt_3D::trk(m_kamni1[j], m_izstrelki[i].oblika))
            {
                uporaben = false;
            }

        //* Ali je zadeta kakšnja ladja
        for (int k = 0; k < nasprotniki.size(); k++)
        {
            Nasprotnik::raketa.pozicija = nasprotniki[k].pozicija;
            Nasprotnik::raketa.rotacija = nasprotniki[k].rotacija;

            if (uporaben && Objekt_3D::trk(Nasprotnik::raketa, m_izstrelki[i].oblika))
            {
                char buff[10];
                buff[0] = T_USTRELIL;
                memcpy(&buff[1], (char *)&nasprotniki[k].id, sizeof(nasprotniki[k].id));
                m_odjmalec.poslji(buff, 5);
                uporaben = false;
            }
        }
        //* Brisanje neuporabnih izstrelkov
        if (m_izstrelki[i].sem_neuporaben() || !uporaben)
        {
            std::swap(m_izstrelki[i], m_izstrelki.back());
            m_izstrelki.pop_back();
        }
        std::cout << m_izstrelki.size() << "\n";
        m_izstrelki[i].posodobi();
        Risalnik::narisi_3D_objekt(m_izstrelki[i].oblika);
    }

    //* Risanje nasprotnikov
    for (int i = 0; i < nasprotniki.size(); i++)
    {
        //! Interpolacija
        // pozicija
        mat::vec3 smer = nasprotniki[i].tr_pozicija - nasprotniki[i].pr_pozicija;
        if (smer.dolzina() == 0)
            Nasprotnik::raketa.pozicija = nasprotniki[i].tr_pozicija;
        else
        {
            nasprotniki[i].pr_pozicija += smer * Risalnik::kamera_3D.hitrost_premikanja * Cas::get_delta_cas();
            Nasprotnik::raketa.pozicija = nasprotniki[i].pr_pozicija;
        }

        // rotacija
        smer = nasprotniki[i].tr_rotacija - nasprotniki[i].pr_pozicija;
        if (smer.dolzina() == 0)
            Nasprotnik::raketa.rotacija = nasprotniki[i].tr_rotacija;
        else
        {
            nasprotniki[i].pr_rotacija += smer * Risalnik::kamera_3D.hitrost_miske * Cas::get_delta_cas();
            Nasprotnik::raketa.rotacija = nasprotniki[i].pr_rotacija;
        }

        Nasprotnik::raketa.rotacija = nasprotniki[i].tr_rotacija;
        Risalnik::narisi_3D_objekt(Nasprotnik::raketa);
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
            m_st_src = 3;
            if (!m_pavza)
            {

                Risalnik::kamera_3D.premikanje_kamere = true;
                Risalnik::nastavi_aktivnost_kazalca_miske(false);
            }
        }
        Risalnik::narisi_besedilo(m_pisava, 0xffffffff, mat::vec2(0, -.1), 0.09, "Umrl si!", R_P_X_SREDINA | R_P_Y_SREDINA);
    }

    //* Preverjanje stanj gumbov
    if (Risalnik::dobi_tipko(256 /*Tipka ESC*/))
    {
        m_pavza = true;
        Risalnik::kamera_3D.premikanje_kamere = false;
        Risalnik::nastavi_aktivnost_kazalca_miske(true);
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
        else
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
        m_odjmalec.poslji(buff, poz);
    }
    // Strežnik že 4-krat ni poslal še sem tu
    if (m_streznik_nazadnje_se_sem_tu + T_SE_SEM_TU_INTERVAL * 4 < Cas::get_cas())
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
    char buff[256] = {0};
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
            }
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

                memcpy((char *)&id, buff + poz, sizeof(id));
                poz += sizeof(id);
                memcpy((char *)&pozicija, buff + poz, sizeof(pozicija));
                poz += sizeof(pozicija);
                memcpy((char *)&rotacija, buff + poz, sizeof(rotacija));
                poz += sizeof(rotacija);

                bool ali_je_v_tabeli = false;
                for (int j = 0; j < is->nasprotniki.size(); j++)
                {
                    if (id == is->nasprotniki[j].id) // Posodabljanje nasprotnikov
                    {
                        is->nasprotniki[j].pr_pozicija = is->nasprotniki[j].tr_pozicija;
                        is->nasprotniki[j].pr_rotacija = is->nasprotniki[j].tr_rotacija;
                        is->nasprotniki[j].tr_pozicija = pozicija;
                        is->nasprotniki[j].tr_rotacija = rotacija;
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