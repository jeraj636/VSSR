#include <iostream>
#include "objekt_3D.h"
#include "risalnik.h"
#include "pisava.h"
#include "objekt_2D.h"
#include "zacetna_scena.h"
#include "igra_scena.h"
#include "nastaviteve_scena.h"

#include "dnevnik.h"

int main()
{
    Risalnik::odpri_okno("VSSR", 0x111111ff);

    Igra_scena is;
    Nastavitve_scena ns;
    Zacetna_scena zs;

    zs.p_igra_scena = &is;
    zs.p_nastavitve_scena = &ns;

    ns.p_zacetna_scena = &zs;

    is.p_zacena_scena = &zs;
    is.p_nastavitve_scena = &ns;

    zs.zacetek();

    while (!Risalnik::ali_je_okno_za_zapreti())
    {
        Risalnik::zacetek_okvir();
        if (Risalnik::aktivna_scena_ptr)
            Risalnik::aktivna_scena_ptr->zanka();
        Risalnik::konec_okvir();
    }
}