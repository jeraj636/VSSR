#include <iostream>
#include "objekt_3D.h"
#include "risalnik.h"
#include "pisava.h"
#include "objekt_2D.h"
#include "zacetna_scena.h"
int main()
{
    Risalnik::odpri_okno("VSSR", 0x33333333);

    std::cout << "Pozdravljen sevet!\n";
    Zacetna_scena zs;
    zs.zacetek();
    while (!Risalnik::ali_je_okno_za_zapreti())
    {
        Risalnik::zacetek_okvir();
        zs.zanka();
        Risalnik::konec_okvir();
    }
}