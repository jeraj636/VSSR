/*
Opis: Deklaracija razreda Zacetna_scena
Avtor: Jakob Jeraj
Licenca: GNU GPL 3
*/

#ifndef ZACETNA_SCENA_H
#define ZACETNA_SCENA_H

#include "scena.h"
#include "pisava.h"
#include "besedilo.h"
#include "objekt_2D.h"
#include "objekt_3D.h"
#include "igra_scena.h"
#include "nastaviteve_scena.h"

class Zacetna_scena : public Scena
{
public:
    Zacetna_scena();
    void zacetek() override;
    void zanka() override;
    void konec() override;

    Igra_scena *p_igra_scena;
    Nastavitve_scena *p_nastavitve_scena;

private:
    Pisava m_pisava;

    Besedilo m_b_igraj;
    Besedilo m_b_nastavitve;
    Besedilo m_b_navodila;
    Besedilo m_b_zasluge;
    Besedilo m_b_izhod;

    Besedilo m_naslov;
    Besedilo m_podnaslov;
    Objekt_3D m_raketa;
    Objekt_2D m_zvezdno_nebo;

    Meteor m_meteor;

private:
    void posodobi_meni();
};

#endif