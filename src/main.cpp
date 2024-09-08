#include <iostream>
#include "objekt_3D.h"
#include "risalnik.h"
#include "pisava.h"
int main()
{
    Risalnik::odpri_okno("test", 0);

    std::cout << "Pozdravljen sevet!\n";
    Pisava pisava("../sredstva/pisava.ttf", 100);

    std::cout << "test1\n";
    Objekt_3D tt(mat::vec3(0, 0, 50), mat::vec3(1), mat::vec3(0), 0xff00ffff, true, "../sredstva/test.obj");
    // tt.preberi_obj("../sredstva/test.obj");

    while (!Risalnik::ali_je_okno_za_zapreti())
    {
        Risalnik::zacetek_okvir();
        Risalnik::narisi_3D_objekt(tt);
        Risalnik::narisi_besedilo(pisava, 0xffffffff, 0xff000000, mat::vec2(0, 0), 100, "J");
        Risalnik::konec_okvir();
    }
}