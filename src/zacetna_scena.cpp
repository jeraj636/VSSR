#include "zacetna_scena.h"
#include "risalnik.h"

Zacetna_scena::Zacetna_scena()
    : pisava("../sredstva/pisava.ttf", 100), test(pisava, 0xffffffff, mat::vec2(0, 300), 25, "TEST!!", Poravnava::sredina)
{
}

void Zacetna_scena::zacetek()
{
    kokos = Objekt_2D(mat::vec2(200, 200), (200), 180, 0xffffffff, "../sredstva/kokos.png", true);
}

void Zacetna_scena::zanka()
{
    test.pozicija.x = Risalnik::dobi_velikost_okna().x / 2;
    test.narisi_me();
    kokos.pozicija = (Risalnik::dobi_velikost_okna() / 2);
    Risalnik::narisi_2D_objekt(kokos);
    if (test.ali_je_miska_gor())
        test.barva_besedila.nastavi(0x55, 'A');
    else
        test.barva_besedila.nastavi(0xff, 'A');
}

void Zacetna_scena::konec()
{
}