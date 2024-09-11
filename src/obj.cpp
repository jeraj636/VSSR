#include "objekt_3D.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstring>
#include "matematika.h"
void Objekt_3D::preberi_obj(const std::string &pot_do_objekta)
{
    if (m_pot_do_objekta.size() != 0) //* sprostitev pomnilnika
    {
        delete[] m_tocke;
        m_velikost_tock = 0;
        m_tocke = nullptr;
    }
    m_pot_do_objekta = pot_do_objekta;
    preberi_obj();
}

int ponovitve_znaka(const std::string &s, char znak)
{
    int st_ponovitev = 0;
    for (int i = 0; i < s.size(); i++)
        if (znak == s[i])
            ++st_ponovitev;
    return st_ponovitev;
}
void zamenjaj_znake(std ::string &s, char znak1, char znak2)
{
    for (int i = 0; i < s.size(); i++)
        if (s[i] == znak1)
            s[i] = znak2;
}
std::string zamenjaj_koncnico(const std::string &datoteka, const std::string &koncnica)
{
    size_t pozicija_pike = datoteka.find_last_of('.');

    if (pozicija_pike == std::string::npos)
    {
        return datoteka;
        std::cout << "Napaka! Datoteka:" << datoteka << " nima veljavne koncnice! \n ";
    }

    return datoteka.substr(0, pozicija_pike + 1) + koncnica;
}
void Objekt_3D::preberi_obj()
{
    //* Odpiranje datoteke
    std::ifstream i_dat(m_pot_do_objekta);

    if (!i_dat.is_open())
        std::cout << "Napaka pri odpiranju datoteke: " << m_pot_do_objekta << '\n';

    std::vector<mat::vec3> v_tab;  //*podatki o tockah
    std::vector<mat::vec3> vn_tab; //* podatki o normalah

    std::string s;
    while (std::getline(i_dat, s))
    {

        if (s[0] == 'v' && s[1] == ' ')
        {
            std::stringstream stream(s);
            std::string tmp;
            float x, y, z;
            stream >> tmp >> x >> y >> z;
            v_tab.push_back(mat::vec3(x, y, z));
        }

        if (s[0] == 'v' && s[1] == 'n')
        {
            std::stringstream stream(s);
            std::string tmp;
            float x, y, z;
            stream >> tmp >> x >> y >> z;
            vn_tab.push_back(mat::vec3(x, y, z));
        }

        if (s[0] == 'f' && s[1] == ' ')
        {
            int stevilo_podatkov = ponovitve_znaka(s, ' ');
            m_velikost_tock += ((stevilo_podatkov - 2) * 3);
        }
    }

    i_dat.clear(); //* Premik na zacetek datoteke
    i_dat.seekg(0, std::ios::beg);

    m_tocke = new float[m_velikost_tock * 9];
    int poz_v_tockah = 0;

    Barva trenutna_barva = 0xffffffff;

    while (std::getline(i_dat, s))
    {
        if (s[0] == 'f' && s[1] == ' ')
        {
            int stevilo_podatkov = ponovitve_znaka(s, ' ');
            std::string podatek;
            std::stringstream stream(s);
            stream >> podatek; //* Odvečen f na zacetku niza

            int prva_tocka, prva_normala, kr_neki; //* Branje prvih podatkov
            stream >> podatek;
            zamenjaj_znake(podatek, '/', ' ');
            std::stringstream tok_podatkov(podatek);
            tok_podatkov >> prva_tocka >> kr_neki >> prva_normala;

            int prejsnja_tocka, prejsnja_normala; //*Branje prejsnjega podatka
            stream >> podatek;
            zamenjaj_znake(podatek, '/', ' ');
            tok_podatkov.clear();
            tok_podatkov.str(podatek);
            tok_podatkov >> prejsnja_tocka >> kr_neki >> prejsnja_normala;
            for (int j = 2; j < stevilo_podatkov; j++)
            {
                stream >> podatek;
                zamenjaj_znake(podatek, '/', ' ');
                tok_podatkov.clear();
                tok_podatkov.str(podatek);

                int tocka, normala;
                tok_podatkov >> tocka >> kr_neki >> normala;
                //* Pisanje podatkov v tabelo
                m_tocke[poz_v_tockah] = v_tab[tocka - 1].x;
                m_tocke[poz_v_tockah + 1] = v_tab[tocka - 1].y;
                m_tocke[poz_v_tockah + 2] = v_tab[tocka - 1].z;

                m_tocke[poz_v_tockah + 3] = vn_tab[normala - 1].x;
                m_tocke[poz_v_tockah + 4] = vn_tab[normala - 1].y;
                m_tocke[poz_v_tockah + 5] = vn_tab[normala - 1].z;

                m_tocke[poz_v_tockah + 6] = trenutna_barva.r;
                m_tocke[poz_v_tockah + 7] = trenutna_barva.g;
                m_tocke[poz_v_tockah + 8] = trenutna_barva.b;

                poz_v_tockah += 9;

                m_tocke[poz_v_tockah] = v_tab[prejsnja_tocka - 1].x;
                m_tocke[poz_v_tockah + 1] = v_tab[prejsnja_tocka - 1].y;
                m_tocke[poz_v_tockah + 2] = v_tab[prejsnja_tocka - 1].z;

                m_tocke[poz_v_tockah + 3] = vn_tab[prejsnja_normala - 1].x;
                m_tocke[poz_v_tockah + 4] = vn_tab[prejsnja_normala - 1].y;
                m_tocke[poz_v_tockah + 5] = vn_tab[prejsnja_normala - 1].z;

                m_tocke[poz_v_tockah + 6] = trenutna_barva.r;
                m_tocke[poz_v_tockah + 7] = trenutna_barva.g;
                m_tocke[poz_v_tockah + 8] = trenutna_barva.b;

                poz_v_tockah += 9;

                m_tocke[poz_v_tockah] = v_tab[prva_tocka - 1].x;
                m_tocke[poz_v_tockah + 1] = v_tab[prva_tocka - 1].y;
                m_tocke[poz_v_tockah + 2] = v_tab[prva_tocka - 1].z;

                m_tocke[poz_v_tockah + 3] = vn_tab[prva_normala - 1].x;
                m_tocke[poz_v_tockah + 4] = vn_tab[prva_normala - 1].y;
                m_tocke[poz_v_tockah + 5] = vn_tab[prva_normala - 1].z;

                m_tocke[poz_v_tockah + 6] = trenutna_barva.r;
                m_tocke[poz_v_tockah + 7] = trenutna_barva.g;
                m_tocke[poz_v_tockah + 8] = trenutna_barva.b;

                prejsnja_normala = normala;
                prejsnja_tocka = tocka;
                poz_v_tockah += 9;
            }
        }
        if (std::strncmp("usemtl", s.c_str(), std::strlen("usemtl")) == 0)
        {
            std::stringstream ss(s); //* dobivanje imena podatka
            std::string ime_materiala;
            std::string tmp;
            ss >> tmp >> ime_materiala;

            //* Odpiranje datoteke z opisom materialov
            std::ifstream i_mtl_dat(zamenjaj_koncnico(m_pot_do_objekta, "mtl"));
            std::string trenuten_material;
            while (std::getline(i_mtl_dat, s))
            {
                if (std::strncmp("newmtl", s.c_str(), std::strlen("newmtl")) == 0)
                {
                    ss = std::stringstream(s);
                    ss >> tmp >> trenuten_material; //! tu sem ostal
                }
                if (std::strncmp("Kd", s.c_str(), std::strlen("Kd")) == 0)
                {
                    if (trenuten_material == ime_materiala)
                    {
                        ss = std::stringstream(s);
                        ss >> tmp >> trenutna_barva.r >> trenutna_barva.g >> trenutna_barva.b;
                    }
                }
            }
        }
    }

    i_dat.close();
}