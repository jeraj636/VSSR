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

private:
    Pisava m_pisava;
    Objekt_2D m_zvezdno_nebo;

private:
};
#endif