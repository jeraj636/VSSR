/*
Opis: Deklaracija razreda Tekstura
Avtor: Jakob Jeraj
Licenca: GNU GPL 3
*/

#ifndef TEKSTURA_H
#define TEKSTURA_H

#include <string>
#include <stdint.h>

uint32_t nalozi_teksturo(const std::string &pot_do_teksture);

void sprosti_teksturo(uint32_t *tekstura);

#endif