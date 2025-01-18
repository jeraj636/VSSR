/*
Opis: Glavna datoteka projekta VSSR
Avtor: Jakob Jeraj
Licenca: GNU GPL 3
*/

#include <iostream>

#include "objekt_3D.h"
#include "risalnik.h"
#include "pisava.h"
#include "objekt_2D.h"
#include "zacetna_scena.h"
#include "igra_scena.h"
#include "nastaviteve_scena.h"
#include "navodila_scena.h"
#include "dnevnik.h"

int main()
{
    Risalnik::odpri_okno("VSSR", 0x111111ff);

    //* Nastavljanje scen
    Igra_scena is;
    Nastavitve_scena ns;
    Navodila_scena nas;
    Zacetna_scena zs;

    zs.p_igra_scena = &is;
    zs.p_nastavitve_scena = &ns;
    zs.p_navodila_scena = &nas;

    ns.p_zacetna_scena = &zs;

    is.p_zacena_scena = &zs;
    is.p_nastavitve_scena = &ns;

    nas.p_zacetna_scena = &zs;

    zs.zacetek();

    while (!Risalnik::ali_je_okno_za_zapreti())
    {
        Risalnik::zacetek_okvir();
        if (Risalnik::aktivna_scena_ptr)
            Risalnik::aktivna_scena_ptr->zanka();
        Risalnik::konec_okvir();
    }
}