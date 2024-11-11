#include "objekt_3D.h"

Objekt_3D::Objekt_3D()
{
    m_tocke = nullptr;
    m_velikost_tock = 0;
    m_pot_do_objekta = "";
}

Objekt_3D::Objekt_3D(const Objekt_3D &t)
{
    pozicija = t.pozicija;
    veliksot = t.veliksot;
    rotacija = t.rotacija;
    barva = t.barva;
    aktiven = t.aktiven;
    m_pot_do_objekta = t.m_pot_do_objekta;
    m_tocke = nullptr;
    m_velikost_tock = 0;
    preberi_obj();
}

Objekt_3D::Objekt_3D(mat::vec3 t_pozicija, mat::vec3 t_velikost, mat::vec3 t_rotacija, uint32_t t_barva, bool t_aktiven, const std::string &t_pot_do_objekta)
{
    pozicija = t_pozicija;
    veliksot = t_velikost;
    rotacija = t_rotacija;
    barva = t_barva;
    aktiven = t_aktiven;
    m_pot_do_objekta = t_pot_do_objekta;
    m_tocke = nullptr;
    m_velikost_tock = 0;
    preberi_obj();
}

Objekt_3D &Objekt_3D::operator=(const Objekt_3D &t)
{
    pozicija = t.pozicija;
    veliksot = t.veliksot;
    rotacija = t.rotacija;
    barva = t.barva;
    aktiven = t.aktiven;

    if (m_pot_do_objekta.size() != 0 && m_tocke != nullptr) //* sprostitev pomnilnika
    {
        delete[] m_tocke;
        m_tocke = nullptr;
        m_velikost_tock = 0;
    }

    m_pot_do_objekta = t.m_pot_do_objekta;
    preberi_obj();
    return *this;
}

Objekt_3D::~Objekt_3D()
{
    if (m_pot_do_objekta.size() != 0 && m_tocke != nullptr) //* sprostitev pomnilnika
        delete[] m_tocke;
}

void Objekt_3D::nastavi(mat::vec3 t_pozicija, mat::vec3 t_velikost, mat::vec3 t_rotacija, uint32_t t_barva, bool t_aktiven, const std::string &t_pot_do_objekta)
{
    pozicija = t_pozicija; //! Potrebno testiranje
    veliksot = t_velikost;
    rotacija = t_rotacija;
    barva = t_barva;
    aktiven = t_aktiven;
    if (m_pot_do_objekta.size() != 0 && m_tocke != nullptr) //* sprostitev pomnilnika
    {
        delete[] m_tocke;
        m_tocke = nullptr;
        m_velikost_tock = 0;
    }

    m_pot_do_objekta = t_pot_do_objekta;
    preberi_obj();
}
#include <iostream>
#include <cmath>
#include "risalnik.h"
Objekt_3D Objekt_3D::hitbox(const Objekt_3D &objekt)
{
    Objekt_3D izh;
    izh.nastavi(objekt.pozicija, objekt.veliksot, objekt.rotacija, 0xffffffaa, true, "../sredstva/kocka.obj");
    //! Neka črna magija, ki se mi je posrečila
    /*
    Zadeva je sicer preprosta
    */

    /*
    poiščejo se minimumi in maksimumi danega objekta.
    */
    mat::vec3 min, max;
    for (int i = 0; i < objekt.m_velikost_tock; i++)
    {
        mat::vec3 tocka;
        tocka.x = objekt.m_tocke[i * 9 + 0];
        tocka.y = objekt.m_tocke[i * 9 + 1];
        tocka.z = objekt.m_tocke[i * 9 + 2];

        if (min.x > tocka.x)
            min.x = tocka.x;
        if (min.y > tocka.y)
            min.y = tocka.y;
        if (min.z > tocka.z)
            min.z = tocka.z;

        if (max.x < tocka.x)
            max.x = tocka.x;
        if (max.y < tocka.y)
            max.y = tocka.y;
        if (max.z < tocka.z)
            max.z = tocka.z;
    }

    /*
    minimumi in maksimumi se pripišejo kocki in tako neredijo hitbox
    */
    for (int i = 0; i < izh.m_velikost_tock; i++)
    {
        if (izh.m_tocke[i * 9 + 0] < 0)
            izh.m_tocke[i * 9 + 0] = min.x;
        else
            izh.m_tocke[i * 9 + 0] = max.x;

        if (izh.m_tocke[i * 9 + 1] < 0)
            izh.m_tocke[i * 9 + 1] = min.y;
        else
            izh.m_tocke[i * 9 + 1] = max.y;

        if (izh.m_tocke[i * 9 + 2] < 0)
            izh.m_tocke[i * 9 + 2] = min.z;
        else
            izh.m_tocke[i * 9 + 2] = max.z;
    }

    return izh;
}

mat::vec3 Objekt_3D::max_proc_tocka(const Objekt_3D &obj, mat::vec3 smer)
{
    mat::vec3 max_t(obj.m_tocke[0], obj.m_tocke[1], obj.m_tocke[2]);
    float max = mat::skalarni_produkt(max_t, smer);
    for (int i = 1; i < obj.m_velikost_tock; i++)
    {
        mat::vec3 tr_tocka(
            obj.m_tocke[i * 9 + 0],
            obj.m_tocke[i * 9 + 1],
            obj.m_tocke[i * 9 + 2]);
        float skalar = mat::skalarni_produkt(tr_tocka, smer);
        if (skalar > max)
        {
            max = skalar;
            max_t = tr_tocka;
        }
    }
    return max_t;
}

mat::vec3 Objekt_3D::max_proc_tocka_v_dveh(const Objekt_3D &a, const Objekt_3D &b, mat::vec3 smer)
{
    return Objekt_3D::max_proc_tocka(a, smer) - Objekt_3D::max_proc_tocka(b, mat::vec3(0) - smer);
}

#include <deque>

bool preveri_simpleks(std::deque<mat::vec3> &simpleks, mat::vec3 &smer)
{
    if (simpleks.size() == 2)
    {
        mat::vec3 A = simpleks[0], B = simpleks[1];
        mat::vec3 AB = B - A;
        mat::vec3 A0 = mat::vec3(0) - A;

        if (mat::skalarni_produkt(AB, A0) > 0)
        {
            smer = mat::vektorski_produkt(mat::vektorski_produkt(AB, A0), AB);
        }
        else
        {
            simpleks = {A};
            smer = A0;
        }
        return false;
    }
    if (simpleks.size() == 3)
    {
        mat::vec3 A = simpleks[0];
        mat::vec3 B = simpleks[1];
        mat::vec3 C = simpleks[2];

        mat::vec3 AB = B - A;
        mat::vec3 AC = C - A;
        mat::vec3 A0 = mat::vec3(0) - A;

        mat::vec3 normala_ABC = mat::vektorski_produkt(AB, AC);

        if (mat::skalarni_produkt(mat::vektorski_produkt(normala_ABC, AC), A0) > 0)
        {
            simpleks = {A, C};
            smer = mat::vektorski_produkt(mat::vektorski_produkt(AC, A0), AC);
        }
        else if (mat::skalarni_produkt(mat::vektorski_produkt(AB, normala_ABC), A0) > 0)
        {
            simpleks = {A, B};
            smer = mat::vektorski_produkt(mat::vektorski_produkt(AB, A0), AB);
        }
        else
        {
            smer = normala_ABC * (mat::skalarni_produkt(normala_ABC, A0) > 0 ? 1 : -1);
        }
        return false;
    }
    if (simpleks.size() == 4)
    {
        mat::vec3 A = simpleks[0];
        mat::vec3 B = simpleks[1];
        mat::vec3 C = simpleks[2];
        mat::vec3 D = simpleks[3];

        mat::vec3 A0 = mat::vec3(0) - A;

        mat::vec3 normala_ABC = mat::vektorski_produkt(B - A, C - A);
        mat::vec3 normala_ACD = mat::vektorski_produkt(C - A, D - A);
        mat::vec3 normala_ADB = mat::vektorski_produkt(D - A, B - A);

        if (mat::skalarni_produkt(normala_ABC, A0) > 0)
        {
            simpleks = {A, B, C};
            smer = normala_ABC;
            return false;
        }
        if (mat::skalarni_produkt(normala_ACD, A0) > 0)
        {
            simpleks = {A, C, D};
            smer = normala_ACD;
            return false;
        }
        if (mat::skalarni_produkt(normala_ADB, A0) > 0)
        {
            simpleks = {A, D, B};
            smer = normala_ADB;
            return false;
        }
        return true;
    }

    return false;
}

bool Objekt_3D::trk(const Objekt_3D &objekt_a, const Objekt_3D &objekt_b)
{
    Objekt_3D hitbox_a = hitbox(objekt_a);
    Objekt_3D hitbox_b = hitbox(objekt_b);

    mat::mat4 transformacija_a(1);
    transformacija_a = mat::pozicijska(transformacija_a, hitbox_a.pozicija);
    transformacija_a = mat::velikostna(transformacija_a, hitbox_a.veliksot);
    transformacija_a = mat::rotacijska(transformacija_a, hitbox_a.rotacija);

    for (int i = 0; i < hitbox_a.m_velikost_tock; i++)
    {
        mat::vec3 tmp(hitbox_a.m_tocke[i * 9 + 0], hitbox_a.m_tocke[i * 9 + 1], hitbox_a.m_tocke[i * 9 + 2]);
        tmp = tmp * transformacija_a;
        hitbox_a.m_tocke[i * 9 + 0] = tmp.x;
        hitbox_a.m_tocke[i * 9 + 1] = tmp.y;
        hitbox_a.m_tocke[i * 9 + 2] = tmp.z;
    }
    mat::mat4 transformacija_b(1);
    transformacija_b = mat::pozicijska(transformacija_b, hitbox_b.pozicija);
    transformacija_b = mat::velikostna(transformacija_b, hitbox_b.veliksot);
    transformacija_b = mat::rotacijska(transformacija_b, hitbox_b.rotacija);
    for (int i = 0; i < hitbox_b.m_velikost_tock; i++)
    {
        mat::vec3 tmp(hitbox_b.m_tocke[i * 9 + 0], hitbox_b.m_tocke[i * 9 + 1], hitbox_b.m_tocke[i * 9 + 2]);
        tmp = tmp * transformacija_b;
        hitbox_b.m_tocke[i * 9 + 0] = tmp.x;
        hitbox_b.m_tocke[i * 9 + 1] = tmp.y;
        hitbox_b.m_tocke[i * 9 + 2] = tmp.z;
    }

    mat::vec3 ta0(hitbox_a.m_tocke[0], hitbox_a.m_tocke[1], hitbox_a.m_tocke[2]), tb0(hitbox_b.m_tocke[0], hitbox_b.m_tocke[1], hitbox_b.m_tocke[2]);
    mat::vec3 smer = ta0 - tb0;
    std::deque<mat::vec3> simpleks;
    simpleks.push_back(max_proc_tocka_v_dveh(hitbox_a, hitbox_b, smer));
    smer = mat::vec3(0) - smer;

    // Logika za trk
    bool ali_je_trk = false;
    while (true)
    {
        mat::vec3 a = max_proc_tocka_v_dveh(hitbox_a, hitbox_b, smer);
        if (mat::skalarni_produkt(a, smer) < 0)
        {
            break;
        }
        simpleks.push_front(a);
        if (preveri_simpleks(simpleks, smer))
        {
            ali_je_trk = true;
            break;
        }
    }
    Objekt_3D izris_a = hitbox(objekt_a);
    Objekt_3D izris_b = hitbox(objekt_b);
    if (ali_je_trk)
    {
        izris_a.barva = 0xff000055;
        izris_b.barva = 0xff000055;
    }
    else
    {
        izris_a.barva = 0xffffff55;
        izris_b.barva = 0xffffffff;
    }

    // Risalnik::narisi_3D_objekt(izris_a);
    Risalnik::narisi_3D_objekt(izris_b);

    return ali_je_trk;
}

Izstrelek::Izstrelek()
{
    oblika.nastavi(mat::vec3(0), mat::vec3(0.2), 0, 0xffffffff, true, "../sredstva/izstrelek.obj");
}

void Izstrelek::posodobi()
{
    oblika.pozicija += smer * Izstrelek::hitrost * Cas::get_delta_cas() * -1;
}

bool Izstrelek::sem_neuporaben()
{
    float meja = 100;
    if (
        oblika.pozicija.x < -meja ||
        oblika.pozicija.x > meja ||
        oblika.pozicija.y < -meja ||
        oblika.pozicija.y > meja ||
        oblika.pozicija.z < meja ||
        oblika.pozicija.z > meja)
        return true;
    return false;
}