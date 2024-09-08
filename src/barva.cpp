#include "barva.h"
#include <iostream>
Barva::Barva()
{
    r = 0;
    g = 0;
    b = 0;
    a = 0;
}
Barva::Barva(uint32_t hex_barva)
{
    a = (hex_barva & 0xff) / 255.0f;
    b = ((hex_barva >> 8) & 0xff) / 255.0f;
    g = ((hex_barva >> 16) & 0xff) / 255.0f;
    r = ((hex_barva >> 24) & 0xff) / 255.0f;
}
void Barva::nastavi(short vrednost, char katera_barva)
{
    switch (katera_barva)
    {
    case 'r':
    case 'R':
        r = ((float)vrednost) / 255.0f;
        break;

    case 'g':
    case 'G':
        g = ((float)vrednost) / 255.0f;
        break;

    case 'b':
    case 'B':
        b = ((float)vrednost) / 255.0f;
        break;

    case 'a':
    case 'A':
        a = ((float)vrednost) / 255.0f;
        break;

    default:
        std::cout << "Napaka pri nastavljanju barve: " << katera_barva << " ni veljaven znak!\n";
        break;
    }
}
std::ostream &operator<<(std::ostream &os, const Barva &t)
{
    os << t.r << " " << t.g << " " << t.b << " " << t.a;
    return os;
}