#ifndef BESEDILO_H
#define BESEDILO_H

#include "pisava.h"
#include "barva.h"
#include "risalnik.h"

class Splosne_nastavitve_besedil
{
public:
    Pisava pisava;
    Barva barva_besedil;
    uint8_t poravnava;
    float velikost;
    Splosne_nastavitve_besedil(Pisava t_pisava, uint32_t t_barva_besedil, uint8_t t_poravnava, float t_velikost);
};

class Besedilo
{

public:
    Pisava &pisava; //* Bodi previden
    float velikost;
    mat::vec2 pozicija;
    std::string niz;
    uint8_t poravnava;
    Barva barva_besedila;
    Besedilo(Pisava &t_pisava, uint32_t t_barva_besedila, mat::vec2 t_pozicija, float t_velikost, const std::string t_niz, uint8_t t_poravnava);
    Besedilo(Pisava &t_pisava);
    Besedilo(Splosne_nastavitve_besedil t_spl_nst_bes);
    void nastavi(Pisava &t_pisava, uint32_t t_barva_besedila, mat::vec2 t_pozicija, float t_velikost, const std::string t_niz, uint8_t t_poravnava);
    bool ali_je_miska_gor();
    void narisi_me();
    bool aktiven;
    double cakaj_do;

private:
    void posodobi();

    mat::vec2 m_dejanska_velikost;
    mat::vec2 m_leva_pozicija;
};

#endif