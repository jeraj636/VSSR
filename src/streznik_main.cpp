/*
Opis: Glavna datoteka programa streznik.bin/exe
Avtor: Jakob Jeraj
Licenca: GNU GPL 3
*/

#include "streznik.h"
#include "dnevnik.h"
#include <stdio.h>
int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        printf("Uporaba ./streznik/streznik.exe <vrata> <st_igralcev> <st_odjemalcev>\n");
        return 1;
    }
    Streznik::zazeni(std::atoi(argv[1]), std::atoi(argv[2]), std::atoi(argv[3]));
    while (Streznik::streznik_tece)
        ;
}