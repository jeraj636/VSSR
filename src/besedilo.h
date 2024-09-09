#ifndef BESEDILO_H
#define BESEDILO_H
#include "pisava.h"
#include "barva.h"
#include "risalnik.h"
class Besedilo
{
private:
    Pisava &m_pisava; //* Bodi previden
    float m_velikost;
    mat::vec2 m_pozicija;
    std::string niz;
    Poravnava m_poravnava_x;
    Poravnava m_poravnava_y;
    void posodobi();

public:
    Barva barva_besedila;
    Besedilo(Pisava &t_pisava, uint32_t t_barva_besedila, mat::vec2 t_pozicija, float t_velikost, const std::string t_niz, Poravnava t_poravnava_x);
    void nastavi(Pisava &t_pisava, uint32_t t_barva_besedila, mat::vec2 t_pozicija, float t_velikost, const std::string t_niz, Poravnava t_poravnava_x);
    bool ali_je_miska_gor();
    void narisi_me();
    void nastavi_pozicijo(mat::vec2 t_pozicija);

protected:
    mat::vec2 m_dejanska_velikost;
    mat::vec2 m_centrirana_pozicija;
};
#endif