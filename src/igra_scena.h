#ifndef IGRA_SCENA_H
#define IGRA_SCENA_H
#include "scena.h"
#include "pisava.h"
#include "besedilo.h"
#include "objekt_2D.h"
#include "objekt_3D.h"

class Igra_scena : public Scena
{
public:
    Igra_scena();
    void zacetek() override;
    void zanka() override;
    void konec() override;

    Scena *p_zacena_scena;

private:
    Pisava m_pisava;
    Objekt_2D m_zvezdno_nebo;
    Objekt_3D m_kamni1[10];

    bool m_pavza;

    Besedilo m_gumb_za_na_meni;
    Besedilo m_gumb_za_nadaljevanje;

private:
};

#endif