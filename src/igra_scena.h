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

class Nasprotnik
{
public:
    static inline Objekt_3D raketa;
    mat::vec3 pozicija;
    int id;
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
    Pisava m_pisava;
    Objekt_2D m_zvezdno_nebo;
    Objekt_3D m_kamni1[10];
    static void vzdrzuj_povezavo(Igra_scena *is);
    bool m_pavza;
    bool m_sem_povezan;

    Odjemalec m_odjmalec;

    Besedilo m_gumb_za_na_meni;
    Besedilo m_gumb_za_nadaljevanje;
    std::vector<Nasprotnik> nasprotniki;

private:
};

#endif