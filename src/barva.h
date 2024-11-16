/*
Opis: Deklaracija razreda barva
Avtor: Jakob Jeraj
Licenca: GNU GPL 3
*/

#ifndef BARVA_H
#define BARVA_H

#include <cstdint>
#include <ostream>

/*
Razred sharanjuje podatke o barvi na način RGBA v spremenjivkah r, g, b in a.
Zgroraj omenjene spremenjivke so javne in imajo vrednost med 0 in 1.
Razred dobi podatke o barvi v HEX kodiranju.
*/
class Barva
{
public:
    Barva();
    Barva(uint32_t hex_barva);
    void nastavi(short vrednost /*0x00 - 0xFF alternativa tudi 0 - 255*/, char katera_barva);
    friend std::ostream &operator<<(std::ostream &os, const Barva &t);

public:
    float r, g, b, a;
};

#endif