#ifndef ZACETNA_SCENA_H
#define ZACETNA_SCENA_H
#include "scena.h"
#include "pisava.h"
#include "besedilo.h"
class Zacetna_scena : public Scena
{
public:
    Zacetna_scena();
    void zacetek() override;
    void zanka() override;
    void konec() override;

private:
    Pisava pisava;
    Besedilo test;
};
#endif