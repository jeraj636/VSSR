/*
Opis: Deklaracija rezreda Scena
Avtor: Jakob Jeraj
Licenca: GNU GPL 3
*/

#ifndef SCENA_H
#define SCENA_H

class Scena
{
public:
    virtual void zacetek() = 0;
    virtual void zanka() = 0;
    virtual void konec() = 0;
};

#endif