#include "zacetna_scena.h"
#include "risalnik.h"

Zacetna_scena::Zacetna_scena()
    : pisava("../sredstva/pisava.ttf", 100), test(pisava, 0xffffffff, (Risalnik::dobi_velikost_okna().x / 2, Risalnik::dobi_velikost_okna().y / 2), 50, "TEST!!", Poravnava::sredina, Poravnava::sredina)
{
}

void Zacetna_scena::zacetek()
{
}

void Zacetna_scena::zanka()
{
    // test.narisi_me();
    Risalnik::narisi_besedilo(pisava, 0xffffffff, mat::vec2(Risalnik::dobi_velikost_okna().x / 2.0f, 100), 200, "TEST!!", Poravnava::sredina, Poravnava::spodaj);
    // Risalnik::narisi_besedilo(pisava, 0xffffffff, mat::vec2(Risalnik::dobi_velikost_okna().x / 2.0f, 100), 50, "TEST!!", Poravnava::sredina, Poravnava::spodaj);
    // Risalnik::narisi_besedilo(pisava, 0xffffffff, mat::vec2(Risalnik::dobi_velikost_okna().x / 2.0f, 100), 25, "TEST!!", Poravnava::sredina, Poravnava::spodaj);
}

void Zacetna_scena::konec()
{
}