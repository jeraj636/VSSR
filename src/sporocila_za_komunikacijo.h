/*
Opis: Tukaj so makroti za vse kar je v zvezi s povezovo po omrežju
Avtor: Jakob Jeraj
Licenca: GNU GPL 3
*/

#ifndef SPOROCILA_ZA_KOMUNIKACIJO_H
#define SPOROCILA_ZA_KOMUNIKACIJO_H

#define T_ODJEMALEC 1
#define T_OPAZOVALEC 2

//* T pomeni tip
#define T_PROSNJA_ZA_POVEZAVO 1
#define T_ODOBRITEV_POVEZAVE 2
#define T_POZZ_STREZNIK 3
#define T_POZZ_ODJEMALEC 4
#define T_O_SE_SEM_TU 5
#define T_S_SE_SEM_TU 6
#define T_PODATKI_IGRALCA 7
#define T_PODATKI_O_IGRALCIH 8
#define T_ZAPUSCAM 9
#define T_USTRELIL 10
#define T_USTRELJEN 11
#define T_POSILJAM_KAMNE 12
#define T_POSILJAM_METEK 13

//* Časovniki (interrvali)
#define T_HITROST_POSILJANJA_PODATKOV 0.1
#define T_SE_SEM_TU_INTERVAL 2.0
#define T_CAS_ZA_POVEZOVANJE 0.5
#define T_CAS_USTRELJENEGA 10
#define T_CAS_POSILJANJA_KAMNOV 1
#endif