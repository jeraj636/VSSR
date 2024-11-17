/*
Opis: Deklaracija razreda Nastavitve_scena
Avtor: Jakob Jeraj
Licenca: GNU GPL 3
*/

#ifndef NASTAVITVE_SCENA_H
#define NASTAVITVE_SCENA_H

#include <thread>

#include "scena.h"
#include "pisava.h"
#include "besedilo.h"
#include "objekt_2D.h"
#include "objekt_3D.h"

class Nastavitve_scena : public Scena
{
public:
    Nastavitve_scena();
    void zacetek() override;
    void zanka() override;
    void konec() override;

public:
    Scena *p_zacetna_scena;

    friend class Igra_scena;
    friend class Zacetna_scena;

private:
    Pisava m_pisava;
    Objekt_2D m_zvezdno_nebo;

    Objekt_2D m_polje_za_vpis_streznika;
    Objekt_2D m_polje_za_vpis_vrat_odjemalca;
    Objekt_2D m_polje_za_vpis_hitrosti_miske;
    Objekt_2D m_polje_za_vpis_vrat_streznika;
    Objekt_2D m_polje_za_vpis_st_odjemalcev;
    Objekt_2D m_polje_za_vpis_st_opazovalcev;
    Meteor m_meteor;

    Besedilo m_gumb_za_na_meni;
    Besedilo m_gumb_za_vklop_izkop_streznika;
    Besedilo m_gumb_za_vklop_izklop_opazovalca;

protected:
    bool ali_streznik_tece;
    bool ali_sem_opazovalec;

    Besedilo m_vrata_odjemalca;
    Besedilo m_streznik;
    Besedilo m_hitrost_miske;
    Besedilo m_vrata_streznika;
    Besedilo m_st_odjemalcev;
    Besedilo m_st_opazovalcev;

private:
    void posodobi_gumbe();
    void posodobi_besedila();
};
#endif