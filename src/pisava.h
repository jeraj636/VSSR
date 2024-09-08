#ifndef PISAVA_H
#define PISAVA_H

#include <cstdint>
#include "matematika.h"
#include <string>
#include <stb_true_type.h>

class Pisava
{
public:
    Pisava(const std::string &pot, uint32_t velikost);
    Pisava() = default;
    Pisava &operator=(const Pisava &t);
    ~Pisava();
    void nastavi(const std::string &pot, uint32_t velikost);
    friend class Risalnik;
    friend class Besedilo;

protected:
    uint32_t m_tekstura_ID;
    stbtt_bakedchar m_char_data[128];
    std::string m_pot_do_pisave;
    uint32_t m_velikost;
    void nalozi_pisavo();
    void unici();
};
#endif