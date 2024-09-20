#ifndef STREZNIK_SCENA_H
#define STREZNIK_SCENA_H

#include "scena.h"
#include "pisava.h"
#include "besedilo.h"
#include "objekt_2D.h"
#include "objekt_3D.h"
#include <thread>
class Streznik_scena : public Scena
{
public:
    Streznik_scena();
    void zacetek() override;
    void zanka() override;
    void konec() override;
    Scena *p_zacena_scena;

private:
    Pisava m_pisava;
    Besedilo m_gumb_za_vkop_izklop_streznika;
    Besedilo m_gumb_za_na_meni;
    Objekt_2D m_zvezdno_nebo;
    std::string m_st_porta;
    Objekt_2D m_polje_za_vpisovanje;
    bool m_ali_je_streznik_vklopljen;

private:
};
#endif