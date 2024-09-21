#ifndef NASTAVITVE_SCENA_H
#define NASTAVITVE_SCENA_H

#include "scena.h"
#include "pisava.h"
#include "besedilo.h"
#include "objekt_2D.h"
#include "objekt_3D.h"
#include <thread>
class Nastavitve_scena : public Scena
{
public:
    Nastavitve_scena();
    void zacetek() override;
    void zanka() override;
    void konec() override;

    Scena *p_zacetna_scena;

private:
    Pisava m_pisava;
    Objekt_2D m_zvezdno_nebo;

    Objekt_2D m_polje_za_vpis_streznika;
    Objekt_2D m_polje_za_vpis_vrat;
    Objekt_2D m_polje_za_vpis_hitrosti_miske;

    Besedilo m_vrata_streznika;
    Besedilo m_streznik;
    Besedilo m_hitrost_miske;
    Besedilo m_gumb_za_na_meni;

private:
    void posodobi_gumbe();
    void posodobi_besedila();
};
#endif