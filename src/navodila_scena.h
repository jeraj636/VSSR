/*
Opis: Deklaracija razreda Zacetna_scena
Avtor: Jakob Jeraj
Licenca: GNU GPL 3
*/

#ifndef NAVODILA_SCENA_H
#define NAVODILA_SCENA_H

#include "scena.h"
#include "pisava.h"
#include "besedilo.h"
#include "objekt_2D.h"
#include "objekt_3D.h"
#include "igra_scena.h"

class Navodila_scena : public Scena
{
public:
    Navodila_scena();
    void zacetek() override;
    void zanka() override;
    void konec() override;

    Scena *p_zacetna_scena;

private:
    Pisava m_pisava;
    Besedilo m_naslov;
    Besedilo m_b_meni;
    Objekt_2D m_zvezdno_nebo;
    Meteor m_meteor;

private:
    void posodobi_meni();
};

#endif