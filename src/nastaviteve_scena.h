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

private:
    Pisava m_pisava;
    Besedilo m_gumb_za_vkop_izklop_streznika;
    Objekt_2D m_zvezdno_nebo;

private:
};
#endif