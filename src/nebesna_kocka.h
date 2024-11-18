#ifndef NEBESNA_KOCKA_H
#define NEBESNA_KOCKA_H

#include <string>
#include <stdint.h>
#include <vector>

class Nebesna_kocka
{
public:
    void nastavi(std::vector<std::string> ploskve);
    uint32_t id_teksture;
};

#endif