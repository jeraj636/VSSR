#ifndef RISALNIK
#define RISALNIK

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

#include "matematika.h"
#include "barva.h"
#include "kamera_2D.h"
#include "kamera_3D.h"

class Pisava;
class Risalnik;
class Objekt_2D;
class Objekt_3D;
class Scena;

#define R_P_ZGORAJ 1
#define R_P_SPODAJ 2
#define R_P_Y_SREDINA 4
#define R_P_X_SREDINA 8
#define R_P_LEVO 16
#define R_P_DESNO 32

class Kazalec
{
public:
    mat::vec2 pozicija_kazalca, pr_pozicija_kazalca;
    friend class Risalnik;

protected:
    bool prvi, prebran_v_tem_okvirju;
};
class Miskin_gumb
{
public:
    bool levi_gumb;
    bool desni_gumb;
};
class Cas
{
public:
    friend class Risalnik;
    static double get_cas();
    static double get_delta_cas();
    static void nastavi_zamik(double zamik);

protected:
    static inline double m_zamik, m_cas, m_delta_cas, m_zacetek_frame_cas;
};
class Risalnik
{
public:
    static inline Barva barva_okna;
    static inline Kazalec kazalec_miske;
    static inline Miskin_gumb miskin_gumb;
    static inline mat::vec3 pozicija_3D_luci;
    static inline Kamera_2D kamera_2D;
    static inline Kamera_3D kamera_3D;
    static mat::vec2 dobi_velikost_okna();
    static inline Scena *aktivna_scena_ptr;
    static inline std::string *buffer_za_vpis_podatkov;

private:
    static inline GLFWwindow *m_glfw_okno;
    static inline uint32_t m_VBO_2D, m_EBO_2D, m_VAO_2D;
    static inline uint32_t m_VBO_2D_p, m_EBO_2D_p, m_VAO_2D_p;
    static inline uint32_t m_VBO_3D, m_EBO_3D, m_VAO_3D;
    static inline uint32_t m_shader_program_2D;
    static inline uint32_t m_shader_program_2D_p;
    static inline uint32_t m_shader_program_3D;
    static inline mat::mat3 m_ortho_mat_2D;
    static inline mat::mat4 m_proj_mat_3D;
    static inline mat::vec2 m_velikost_okna;
    static inline bool m_tipke[512];
    static inline mat::vec2 m_resolucija;
    static inline bool m_razsiljivo_okno = true;

public:
    static void odpri_okno(const std::string &naslov_okna, Barva t_barva_okna);
    static void zacetek_okvir();
    static void konec_okvir();
    static void narisi_2D_objekt(const Objekt_2D &obj);
    static void narisi_3D_objekt(const Objekt_3D &obj);
    static void narisi_besedilo(const Pisava &pisava, const Barva b_besedila, mat::vec2 pozicija, float velikost, const std::string niz, uint8_t zastavice);
    static bool ali_je_okno_za_zapreti();
    static bool dobi_tipko(int katera_tipka);
    static void zapri_okno();
    static void nastavi_aktivnost_kazalca_miske(bool aktivnost);
    static void nastavi_testiranje_globine(bool aktivno);

private:
    static void posodobi_velikost_okna(GLFWwindow *okno, int dolzina, int visina);
    static void posodobi_tipke(GLFWwindow *okno, int tipka, int koda_skeniranja, int akcija, int modi);
    static void posodobi_gumb_miske(GLFWwindow *window, int gumb, int akcija, int mods);
    static void posodobi_besedilo_vhod(GLFWwindow *okno, uint32_t unicode);
    static void ustvari_bufferje_2D();
    static void ustvari_bufferje_3D();
    static void ustvari_bufferje_2D_p();

    static void ustvari_shaderje_2D();
    static void ustvari_shaderje_3D();
    static void ustvari_shaderje_2D_p();
};
#endif