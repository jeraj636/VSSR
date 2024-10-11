#include "streznik.h"
#include "dnevnik.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
        napaka("Ni podane st vrat!\n");
    Streznik::zazeni(std::atoi(argv[1]));
    while (Streznik::streznik_tece)
        ;
}