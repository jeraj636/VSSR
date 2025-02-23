/*
Opis: Deklaracija razreda Cas
Avtor: Jakob Jeraj
Licenca: GNU GPL 3
*/
#ifndef CAS_H
#define CAS_H
class Cas
{
public:
    friend class Risalnik;
    static double get_cas();
    static double get_delta_cas();
    static void nastavi_zamik(double zamik);

protected:
    static inline double m_zamik, m_cas, m_delta_cas, m_zacetek_frame_cas;
};
#endif