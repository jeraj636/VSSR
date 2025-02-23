/*
Opis: Deklaracija funkcij izpis sporočil
Avtor: Jakob Jeraj
Licenca: GNU GPL 3
*/

#ifndef DNEVNIK_H
#define DNEVNIK_h

#include <stdarg.h>
/*
... - To pomeni da je lahko več argumentov z različnimi podatkovnimi tipi
To omogoča knjižnica stdarg.h
*/
void napaka(const char *sporocilo, ...);
void opozorilo(const char *sporocilo, ...);
void sporocilo(const char *sporocilo, ...);
void izpis(const char *sporocilo, ...);

#endif