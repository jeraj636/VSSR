#include <iostream>
#include "objekt_3D.h"
#include "risalnik.h"
#include "pisava.h"
#include "objekt_2D.h"
int main()
{
    Risalnik::odpri_okno("test", 0x33333333);

    std::cout << "Pozdravljen sevet!\n";
    Pisava pisava("../sredstva/pisava.ttf", 100);

    Objekt_3D tt(mat::vec3(0, 0, 50), mat::vec3(1), mat::vec3(0), 0xff00ffff, true, "../sredstva/test.obj");
    tt.preberi_obj("../sredstva/test.obj");

    Objekt_2D kokos((100, 100), 100, 180, 0xffffffff, "../sredstva/kokos.png", true);

    while (!Risalnik::ali_je_okno_za_zapreti())
    {
        Risalnik::zacetek_okvir();
        Risalnik::narisi_3D_objekt(tt);
        Risalnik::narisi_2D_objekt(kokos);
        Risalnik::konec_okvir();
    }
}