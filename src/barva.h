#ifndef BARVA_H
#define BARVA_H
#include <cstdint>
#include <ostream>
class Barva
{
public:
    Barva();
    Barva(uint32_t hex_barva);
    void nastavi(short vrednost, char katera_barva);
    float r, g, b, a;

    friend std::ostream &operator<<(std::ostream &os, const Barva &t);
};
#endif