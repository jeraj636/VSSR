/*
Opis: Deklaracija razredov Igra_scena, Nasprotnik, Teleportacija in Casovnik
Avtor: Jakob Jeraj
Licenca: GNU GPL 3
*/

#ifndef IGRA_SCENA_H
#define IGRA_SCENA_H

#include <vector>

#include "scena.h"
#include "pisava.h"
#include "besedilo.h"
#include "objekt_2D.h"
#include "objekt_3D.h"
#include "odjemalec.h"
#include "nastaviteve_scena.h"
#include "kamen.h"
#include "nebesna_kocka.h"

#define HITER_RAZMIK_MERILCA 0.1
#define POCASEN_RAZMIK_MERILCA 0.05
#define STOJIM_RAZMIK_MERILCA 0.03
#define MERIM_RAZMIK_MERILCA 0.013

class Nasprotnik
{
public:
    static inline Objekt_3D raketa;
    mat::vec3 pozicija, rotacija;
    mat::vec3 pr_pozicija, pr_rotacija;
    mat::vec3 tr_pozicija, tr_rotacija;
    int id;
    bool prebran;
};

class Teleportacija
{
public:
    mat::vec3 pozicija;
    float jaw, pitch;
};

class Vidni_casovnik
{
public:
    Objekt_2D spredaj;
    Objekt_2D zadaj;
};

class Igra_scena : public Scena
{
public:
    Igra_scena();
    void zacetek() override;
    void zanka() override;
    void konec() override;

    Scena *p_zacena_scena;
    Nastavitve_scena *p_nastavitve_scena;

private:
    static void vzdrzuj_povezavo(Igra_scena *is);

private:
    Pisava m_pisava;
    Objekt_2D m_zvezdno_nebo;
    Nebesna_kocka m_nebesna_kocka;
    Kamen m_kamni1[10];
    Objekt_3D m_modri_kamen;
    Meteor m_meteor;

    //* Merilec
    Objekt_2D m_merilec[4];
    float m_razmik_merilca = 0.015;
    mat::vec2 m_velikost_merilca;

    float m_hitrost_miske;

    //* Stanja
    bool m_ali_sem_umrl;
    bool m_pavza;
    bool m_sem_povezan;

    //* Casovniki
    double m_cas_naslednjega_posiljanja_podatkov;
    double m_cas_za_se_sem_tu;
    double m_streznik_nazadnje_se_sem_tu;
    double m_naslednje_streljanje;
    double m_naslednja_teleportacija;
    double m_cas_do_ozivetja;

    Odjemalec m_odjmalec;

    Besedilo m_gumb_za_na_meni;
    Besedilo m_gumb_za_nadaljevanje;

    std::vector<Nasprotnik> nasprotniki;

    bool m_opazujem;

    std::vector<Izstrelek> m_izstrelki;
    std::vector<Izstrelek> m_tuji_izstrelki;

    int m_vseh_st_src;
    int m_st_src;
    Objekt_2D m_srce;

    Teleportacija m_teleportacija;
    Vidni_casovnik m_vc_za_streljati, m_vc_za_teleportirati, m_vc_za_oziveti;

private:
};

#endif