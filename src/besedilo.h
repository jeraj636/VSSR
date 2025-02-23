/*
Opis: Deklaracija razreda Besedilo
Avtor: Jakob Jeraj
Licenca: GNU GPL 3
*/

#ifndef BESEDILO_H
#define BESEDILO_H

#include "pisava.h"
#include "barva.h"
#include "risalnik.h"

/*
Razred besedilo hrani podatke o tekstu.
Vsebuje tudi funkcije za izris na zaslon in preverjanje ali je na njem kazelec miske.
*/
class Besedilo
{
public:
    Besedilo(Pisava &t_pisava, uint32_t t_barva_besedila, mat::vec2 t_pozicija, float t_velikost, const std::string t_niz, uint8_t t_poravnava);
    Besedilo(Pisava &t_pisava);
    void nastavi(Pisava &t_pisava, uint32_t t_barva_besedila, mat::vec2 t_pozicija, float t_velikost, const std::string t_niz, uint8_t t_poravnava);
    bool ali_je_miska_gor();
    void narisi_me();

public:
    Pisava &pisava; //! Bodi previden!
    float velikost;
    mat::vec2 pozicija;
    std::string niz;
    uint8_t poravnava;
    Barva barva_besedila;
    bool aktiven;
    double cakaj_do;

private:
    void posodobi();

private:
    mat::vec2 m_dejanska_velikost;
    mat::vec2 m_leva_pozicija;
};

#endif