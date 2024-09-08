#include "risalnik.h"

double Cas::get_cas()
{
    return m_cas;
}
double Cas::get_delta_cas()
{
    return m_delta_cas;
}
void Cas::nastavi_zamik(double zamik)
{
    m_zamik = zamik;
}