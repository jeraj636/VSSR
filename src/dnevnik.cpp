#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "dnevnik.h"
#include "define.h"

#ifdef WINDOWS
#include <windows.h>
#endif

void napaka(const char *sporocilo, ...) //* Sporocilo o napaki
{
#ifdef DEBUG
        if (STOPNJA_SPOROCANJA >= 1)
        {
#ifdef LINUX
                printf("\033[0;31m"); // rdeca
#endif
#ifdef WINDOWS
                HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED); // Zeleni tekst
#endif
                va_list parametri;
                va_start(parametri, sporocilo);
                vprintf(sporocilo, parametri);
                va_end(parametri);
                fflush(stdout);
#ifdef LINUX
                printf("\033[0m");
#endif
#ifdef WINDOWS
                SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY); // Resetiranje tekst
#endif
        }
#endif
}
void opozorilo(const char *sporocilo, ...)
{
#ifdef DEBUG
        if (STOPNJA_SPOROCANJA >= 2)
        {
#ifdef LINUX
                printf("\033[0;34m"); // modra
#endif
#ifdef WINDOWS
                HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE); // Zeleni tekst
#endif
                va_list parametri;
                va_start(parametri, sporocilo);
                vprintf(sporocilo, parametri);
                va_end(parametri);
                fflush(stdout);
#ifdef LINUX
                printf("\033[0m");
#endif
#ifdef WINDOWS
                SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY); // Resetiranje tekst
#endif
        }
#endif
}
void sporocilo(const char *sporocilo, ...)
{
#ifdef DEBUG
        if (STOPNJA_SPOROCANJA >= 3)
        {
#ifdef LINUX
                printf("\033[0;32m"); // zelena
#endif
#ifdef WINDOWS
                HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN); // Zeleni tekst
#endif
                va_list parametri;
                va_start(parametri, sporocilo);
                vprintf(sporocilo, parametri);
                va_end(parametri);
                fflush(stdout);
#ifdef LINUX
                printf("\033[0m");
#endif
#ifdef WINDOWS
                SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY); // Resetiranje tekst
#endif
        }
#endif
}
void izpis(const char *sporocilo, ...)
{
#ifdef DEBUG
        va_list parametri;
        va_start(parametri, sporocilo);
        vprintf(sporocilo, parametri);
        va_end(parametri);
        fflush(stdout);
}
#endif
}