#include "zacetna_scena.h"
#include "risalnik.h"

Zacetna_scena::Zacetna_scena()
    : pisava("../sredstva/pisava.ttf", 100), test(pisava, 0xffffffff, (0), 100, "TEST!!", Poravnava::sredina)
{
}

void Zacetna_scena::zacetek()
{
    kokos = Objekt_2D(mat::vec2(200, 200), (200), 180, 0xffffffff, "../sredstva/kokos.png", true);
}

void Zacetna_scena::zanka()
{
    //! NUJNO
    //! Vedno posodablja pozicijo!!!!!!!!!!!!!
    kokos.pozicija = (Risalnik::dobi_velikost_okna() / 2);
    Risalnik::narisi_besedilo(pisava, 0xff0000ff, mat::vec2(Risalnik::dobi_velikost_okna().x / 2, 300), 100, "TEST!!", Poravnava::sredina);
    Risalnik::narisi_besedilo(pisava, 0xff0000ff, mat::vec2(Risalnik::dobi_velikost_okna().x / 2, 200), 100, "TEST!!", Poravnava::levo);
    Risalnik::narisi_besedilo(pisava, 0xff0000ff, mat::vec2(Risalnik::dobi_velikost_okna().x / 2, 400), 100, "TEST!!", Poravnava::desno);
    Risalnik::narisi_2D_objekt(kokos);
}

void Zacetna_scena::konec()
{
}