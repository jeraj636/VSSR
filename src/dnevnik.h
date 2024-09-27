#ifndef DNEVNIK_H
#define DNEVNIK_h

#include <stdarg.h>

void napaka(const char *sporocilo, ...);
void opozorilo(const char *sporocilo, ...);
void sporocilo(const char *sporocilo, ...);
void izpis(const char *sporocilo, ...);

#endif