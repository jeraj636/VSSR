#include "risalnik.h"
#include <iostream>
#include "objekt_2D.h"
#include "objekt_3D.h"
#include <stdlib.h>
#include "shaderji.h"
#include "pisava.h"
void Risalnik::odpri_okno(const std::string &naslov_okna, Barva t_barva_okna)
{
    //* Inicializacija GLFW-ja
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //* Odpiranje GLFW okna
    m_glfw_okno = glfwCreateWindow(800, 600, naslov_okna.c_str(), NULL, NULL);
    if (m_glfw_okno == NULL)
    {
        std::cout << "Ni uspelo odpreti okna!\n";
        glfwTerminate();
        exit(1);
    }
    glfwMakeContextCurrent(m_glfw_okno);

    //* Inicializacija GLAD-a
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Ni uspelo inicializirati GLFW-ja!\n";
        glfwTerminate();
        exit(1);
    }
    glViewport(0, 0, 800, 600);

    //* nastavljanje tipk na na zacetno mesto
    for (int i = 0; i < 512; i++)
    {
        m_tipke[i] = false;
    }

    barva_okna = t_barva_okna;

    //*Inicializacija kazačca miske
    kazalec_miske.pozicija_kazalca = (mat::vec2){0, 0};
    kazalec_miske.pr_pozicija_kazalca = (mat::vec2){0, 0};
    kazalec_miske.prvi = true;

    //* Zakrivanje kazalca miske
    glfwSetInputMode(m_glfw_okno, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //* Callback
    glfwSetFramebufferSizeCallback(m_glfw_okno, posodobi_velikost_okna);
    glfwSetKeyCallback(m_glfw_okno, posodobi_tipke);
    glfwSetMouseButtonCallback(m_glfw_okno, posodobi_gumb_miske);
    //* Ustvarjanje bufferjev
    ustvari_bufferje_2D_p();
    ustvari_bufferje_2D();
    ustvari_bufferje_3D();

    //* Ustvarjanje shaderjev
    ustvari_shaderje_2D_p();
    ustvari_shaderje_2D();
    ustvari_shaderje_3D();

    //* Da se ne vidi skozi 3D elemente
    glEnable(GL_DEPTH_TEST);

    //* Da se lahko vidi skozi prozorne objekte
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
    //*Inicializacija kamer
    // kamera_2D.Nastavi(100, mat::vec2(0));
    kamera_3D.nastavi();

    /*
    ! Za ugotovitel maksimalne velikosti
    int maxBufferSize;
    glGetIntegerv(GL_MAX_ELEMENTS_VERTICES, &maxBufferSize);
    std::cout << "Max buffer size: " << maxBufferSize << std::endl;
    */
}

void Risalnik::posodobi_velikost_okna(GLFWwindow *okno, int dolzina, int visina)
{
    //* Nastavljanje velikosti okna
    glViewport(0, 0, dolzina, visina);

    kamera_3D.nastavi();
}
void Risalnik::posodobi_tipke(GLFWwindow *okno, int tipka, int koda_skeniranja, int akcija, int modi)
{

    if (tipka < 0 || tipka > 512) // Tipka ne obstaja!
        return;
    if (akcija == GLFW_PRESS) // Tipka pritisnjena!
    {
        m_tipke[tipka] = true;
    }
    if (akcija == GLFW_RELEASE) // Tipka spuščena!
    {
        m_tipke[tipka] = false;
    }
}

void Risalnik::zacetek_okvir()
{
    //* Čiščenje okna
    glClearColor(barva_okna.r, barva_okna.g, barva_okna.b, barva_okna.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //* Posodabljanje kamer
    kamera_2D.posodobi_kamero();
    kamera_3D.posodobi();

    //* Nastavljanje matrik:
    // Ortho 2D matrika
    m_ortho_mat_2D = mat::mat3(1);
    m_ortho_mat_2D = mat::ortho(m_ortho_mat_2D, 0, m_velikost_okna.x, 0, m_velikost_okna.y);

    // Proj 3D maatrika
    m_proj_mat_3D = mat::mat4(1);
    m_proj_mat_3D = mat::projekcija(m_proj_mat_3D, m_velikost_okna.y, m_velikost_okna.x, degToRad(kamera_3D.vidno_polje), 0.1, 1000);

    //* Miškin kazalec
    double kaz_poz_x, kaz_poz_y; //! ???????
    glfwGetCursorPos(m_glfw_okno, &kaz_poz_x, &kaz_poz_y);
    kazalec_miske.prebran_v_tem_okvirju = false;
    kazalec_miske.pr_pozicija_kazalca = kazalec_miske.pozicija_kazalca;
    kazalec_miske.pozicija_kazalca = mat::vec2(kaz_poz_x, kaz_poz_y);

    // *Cas zacetka okvirja
    Cas::m_zacetek_frame_cas = glfwGetTime();

    //* velikost okna
    bool sprememba_velikosti = false;
    int vel_x, vel_y;

    glfwGetWindowSize(m_glfw_okno, &vel_x, &vel_y);

    if (vel_x != m_velikost_okna.x)
    {
        sprememba_velikosti = true;
    }
    m_velikost_okna = mat::vec2(vel_x, vel_y);

    //! Le za testiranje al neki ¯\_( )_/¯
    if (sprememba_velikosti)
    {
        kazalec_miske.pr_pozicija_kazalca = kazalec_miske.pozicija_kazalca;
        kamera_3D.nastavi();
    }
}

/* Ptiček
!  ,,
! (°>
! //)
! ``
*/

void Risalnik::konec_okvir()
{
    glfwPollEvents();
    glfwSwapBuffers(m_glfw_okno);

    //* Izračun trajanja okvirja
    Cas::m_cas = glfwGetTime();
    Cas::m_delta_cas = Cas::m_cas - Cas::m_zacetek_frame_cas;
}
void Risalnik::ustvari_bufferje_2D()
{
    //* Nastavljanje osnovnih vrednosti
    float tocke[] = {
        0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 1.0f, 0.0f};
    uint32_t indeksi[] = {
        0, 1, 2,
        0, 2, 3};

    //* Ustvarjanje VAO-ja
    glGenVertexArrays(1, &m_VAO_2D);
    glBindVertexArray(m_VAO_2D);

    //* Ustvarjanje VBO-ja in EBO-ja
    // VBO
    glGenBuffers(1, &m_VBO_2D);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO_2D);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tocke), tocke, GL_STATIC_DRAW);
    // EBO
    glGenBuffers(1, &m_EBO_2D);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO_2D);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeksi), indeksi, GL_STATIC_DRAW);

    //* Nastavljanje attrib kazalcev
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Risalnik::ustvari_shaderje_2D()
{
    int uspeh;
    char informacije[512];

    //* Ustvarjanje vertex shaderja
    uint32_t vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_2D_s, NULL);

    glCompileShader(vertex_shader);

    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &uspeh);

    //* Preverjanje napak
    if (!uspeh)
    {
        glGetShaderInfoLog(vertex_shader, 512, NULL, informacije);
        std::cout << "Napaka: vertex_shader_2D.glsl" << informacije << "\n";
    }

    //* Ustvarjanje fragment shaderja
    uint32_t fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_2D_s, NULL);

    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &uspeh);

    //* Preverjanje napak
    if (!uspeh)
    {
        glGetShaderInfoLog(fragment_shader, 512, NULL, informacije);
        snprintf(informacije, sizeof(informacije), "NAPAKA %s", informacije);
        std::cout << "Napaka: fragment_shader_2D.glsl " << informacije << "\n";
        glfwTerminate();
        exit(1);
    }

    //* Ustvarjanje shader programa
    m_shader_program_2D = glCreateProgram();
    glAttachShader(m_shader_program_2D, vertex_shader);
    glAttachShader(m_shader_program_2D, fragment_shader);
    glLinkProgram(m_shader_program_2D);

    //* preverjanje napak
    glGetProgramiv(m_shader_program_2D, GL_LINK_STATUS, &uspeh);
    if (!uspeh)
    {
        glGetProgramInfoLog(m_shader_program_2D, 512, NULL, informacije);
        std::cout << "Napaka: " << informacije << "\n";
        glfwTerminate();
        exit(1);
    }

    //* Čiščenje odvečnih reči
    glDeleteShader(fragment_shader);
    glDeleteShader(vertex_shader);
}

void Risalnik::ustvari_bufferje_3D()
{
    //* Ustvarjanje VAO-ja
    glGenVertexArrays(1, &m_VAO_3D);
    glBindVertexArray(m_VAO_3D);

    //* Ustvarjanje VBO-ja
    glGenBuffers(1, &m_VBO_3D);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO_3D);
    glBufferData(GL_ARRAY_BUFFER, 100000, NULL, GL_DYNAMIC_DRAW);

    //* Nastavljanje attrib kazalcev
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void Risalnik::ustvari_shaderje_3D()
{
    int uspeh;
    char informacije[512];

    //* ustvarjanje verex shaderja
    uint32_t vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_3D_s, NULL);

    glCompileShader(vertex_shader);

    //* Preverjanje napak
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &uspeh);

    if (!uspeh)
    {
        glGetShaderInfoLog(vertex_shader, 512, NULL, informacije);
        std::cout << "Napaka: vertex_shader_3D.glsl " << informacije << '\n';
        glfwTerminate();
        exit(1);
    }

    uint32_t fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_3D_s, NULL);

    glCompileShader(fragment_shader);

    //* Preverjanje napak
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &uspeh);

    if (!uspeh)
    {
        glGetShaderInfoLog(fragment_shader, 512, NULL, informacije);
        snprintf(informacije, sizeof(informacije), "NAPAKA %s", informacije);
        std::cout << "Napaka: fragment_shader_3D.glsl " << informacije << '\n';
        glfwTerminate();
        exit(1);
    }

    //* Ustvarjanje shader programa
    m_shader_program_3D = glCreateProgram();
    glAttachShader(m_shader_program_3D, vertex_shader);
    glAttachShader(m_shader_program_3D, fragment_shader);
    glLinkProgram(m_shader_program_3D);

    glGetProgramiv(m_shader_program_3D, GL_LINK_STATUS, &uspeh);
    if (!uspeh)
    {
        glGetProgramInfoLog(m_shader_program_3D, 512, NULL, informacije);
        std::cout << "Napaka: " << informacije;
        glfwTerminate();
        exit(1);
    }

    glDeleteShader(fragment_shader);
    glDeleteShader(vertex_shader);
}

void Risalnik::ustvari_bufferje_2D_p()
{
    //* Ustvarjanje VAO-ja
    glGenVertexArrays(1, &m_VAO_2D_p);
    glBindVertexArray(m_VAO_2D_p);

    //* Ustvarjanje VBO-ja
    glGenBuffers(1, &m_VBO_2D_p);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO_2D_p);
    glBufferData(GL_ARRAY_BUFFER, 2000, NULL, GL_DYNAMIC_DRAW);

    //* Ustavarjanje EBO-ja
    glGenBuffers(1, &m_EBO_2D_p);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO_2D_p);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2000, NULL, GL_DYNAMIC_DRAW);

    //* Nastavljanje attrib kazalcev
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Risalnik::ustvari_shaderje_2D_p()
{
    int uspeh;
    char informacije[512];

    //* ustvarjanje verex shaderja
    uint32_t vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_2D_p_s, NULL);

    glCompileShader(vertex_shader);

    //* Preverjanje napak
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &uspeh);

    if (!uspeh)
    {
        glGetShaderInfoLog(vertex_shader, 512, NULL, informacije);
        std::cout << "Napaka: vertex_shader_2D_p.glsl" << informacije << '\n';
        glfwTerminate();
        exit(1);
    }

    uint32_t fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_2D_p_s, NULL);

    glCompileShader(fragment_shader);

    //* Preverjanje napak
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &uspeh);

    if (!uspeh)
    {
        glGetShaderInfoLog(fragment_shader, 512, NULL, informacije);
        snprintf(informacije, sizeof(informacije), "NAPAKA %s", informacije);
        std::cout << "Napaka: fragment_shader_2D_p.glsl " << informacije << '\n';
        glfwTerminate();
        exit(1);
    }

    //* Ustvarjanje shader programa
    m_shader_program_2D_p = glCreateProgram();
    glAttachShader(m_shader_program_2D_p, vertex_shader);
    glAttachShader(m_shader_program_2D_p, fragment_shader);
    glLinkProgram(m_shader_program_2D_p);

    glGetProgramiv(m_shader_program_2D_p, GL_LINK_STATUS, &uspeh);
    if (!uspeh)
    {
        glGetProgramInfoLog(m_shader_program_2D_p, 512, NULL, informacije);
        std::cout << "Napaka: " << informacije;
        glfwTerminate();
        exit(1);
    }

    glDeleteShader(fragment_shader);
    glDeleteShader(vertex_shader);
}

void Risalnik::narisi_2D_objekt(const Objekt_2D &obj)
{
    glUseProgram(m_shader_program_2D);
    glBindVertexArray(m_VAO_2D);

    //* Pošiljanje barve
    glUniform4f(glGetUniformLocation(m_shader_program_2D, "u_barva"), obj.barva.r, obj.barva.g, obj.barva.b, obj.barva.a);

    //* Nastavljanje teksture
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, obj.m_tekstura_id);
    glUniform1i(glGetUniformLocation(m_shader_program_2D, "u_tekstura"), 1);

    //* Pripravljanje transformacijske matrike
    mat::mat3 transformacija(1);
    transformacija = mat::pozicijska(transformacija, obj.pozicija);
    transformacija = mat::rotacijska(transformacija, degToRad(obj.rotacija));
    transformacija = mat::velikostna(transformacija, obj.velikost);

    //* Mnozenje z projekcijo in kamero
    transformacija = kamera_2D.m_mat_pogled * transformacija;
    transformacija = m_ortho_mat_2D * transformacija;

    //* Pošiljanje transformacijske matrike
    glUniformMatrix3fv(glGetUniformLocation(m_shader_program_2D, "u_transformacija"), 1, GL_TRUE, &transformacija.mat[0][0]);

    //* Risanje elementa
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glUseProgram(0);
    glBindVertexArray(0);
}
void Risalnik::narisi_3D_objekt(const Objekt_3D &obj)
{
    glUseProgram(m_shader_program_3D);
    glBindVertexArray(m_VAO_3D);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO_3D);

    //* Pošiljanje barve in pozicije luči
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * obj.m_velikost_tock * 9, obj.m_tocke);
    glUniform4f(glGetUniformLocation(m_shader_program_3D, "u_barva"), obj.barva.r, obj.barva.g, obj.barva.b, obj.barva.a);
    glUniform3f(glGetUniformLocation(m_shader_program_3D, "u_pozicija_luci"), pozicija_3D_luci.x, pozicija_3D_luci.y, pozicija_3D_luci.z);

    //* Priprava transformacijske matrike
    mat::mat4 transformacija(1);
    transformacija = mat::pozicijska(transformacija, obj.pozicija);
    transformacija = mat::velikostna(transformacija, obj.veliksot);
    transformacija = mat::rotacijska(transformacija, obj.rotacija);

    //* Množenje transformacije z kamero in projekcijo
    transformacija = kamera_3D.m_mat_pogled * transformacija;
    transformacija = m_proj_mat_3D * transformacija;

    //* Pošiljanje matrike
    glUniformMatrix4fv(glGetUniformLocation(m_shader_program_3D, "u_transformacija"), 1, GL_TRUE, &transformacija.mat[0][0]);

    //* Risanje objekta
    glDrawArrays(GL_TRIANGLES, 0, obj.m_velikost_tock);

    glUseProgram(0);
    glBindVertexArray(0);
}

bool Risalnik::ali_je_okno_za_zapreti()
{
    return glfwWindowShouldClose(m_glfw_okno);
}
bool Risalnik::dobi_tipko(int katera_tipka)
{
    if (katera_tipka >= 0 && katera_tipka < 512)
        return m_tipke[katera_tipka];
    else
        return false;
}

void Risalnik::narisi_besedilo(const Pisava &pisava, const Barva b_besedila, mat::vec2 pozicija, float velikost, const std::string niz, Poravnava poravnava_x)
{
    //* Rezervacija prostora v pomilniku
    float *tocke = new float[16 * niz.size()];
    uint32_t *indeksi = new uint32_t[6 * niz.size()];

    //* Izračum pravilne velikosti pisave
    velikost /= pisava.m_velikost;

    stbtt_aligned_quad quad;
    float x = 0, y = 0;
    //* Pridobivanje x in y velikosti
    /*
        y velikost dobimo na načim da najdemo y_max in y_min in izračunamo razliko
        x velikost pa s sestevanjem razlik med x1 in x0
    */
    for (int i = 0; i < niz.size(); i++)
    {
        stbtt_GetBakedQuad(pisava.m_char_data, 512, 512, niz[i], &x, &y, &quad, false);
        /*
        ! Morda to ni potrebno
        quad.x0 *= velikost;
        quad.x1 *= velikost;
        quad.y0 *= -velikost;
        quad.y1 *= -velikost;
        */
    }

    //* Zamik glede na poravnavo na osi x
    switch (poravnava_x)
    {
    case Poravnava::levo:
        break;
    case Poravnava::sredina:
        pozicija.x -= (x * velikost) / 2;
        break;
    case Poravnava::desno:
        pozicija.x -= x * velikost;
        break;

    default:
        std::cout << "Napaka! napačna poravnava besedila: " << niz << '\n';
        break;
    }

    //* Pisanje podatkov v tabele
    x = y = 0;
    for (int i = 0; i < niz.size(); i++)
    {
        stbtt_GetBakedQuad(pisava.m_char_data, 512, 512, niz[i], &x, &y, &quad, false);

        quad.x0 *= velikost;
        quad.x1 *= velikost;
        quad.y0 *= velikost;
        quad.y1 *= velikost;

        tocke[i * 16 + 0] = quad.x0;
        tocke[i * 16 + 1] = quad.y0;
        tocke[i * 16 + 2] = quad.s0;
        tocke[i * 16 + 3] = quad.t0;

        tocke[i * 16 + 4] = quad.x1;
        tocke[i * 16 + 5] = quad.y0;
        tocke[i * 16 + 6] = quad.s1;
        tocke[i * 16 + 7] = quad.t0;

        tocke[i * 16 + 8] = quad.x1;
        tocke[i * 16 + 9] = quad.y1;
        tocke[i * 16 + 10] = quad.s1;
        tocke[i * 16 + 11] = quad.t1;

        tocke[i * 16 + 12] = quad.x0;
        tocke[i * 16 + 13] = quad.y1;
        tocke[i * 16 + 14] = quad.s0;
        tocke[i * 16 + 15] = quad.t1;

        indeksi[i * 6 + 0] = 0 + i * 4;
        indeksi[i * 6 + 1] = 1 + i * 4;
        indeksi[i * 6 + 2] = 2 + i * 4;
        indeksi[i * 6 + 3] = 0 + i * 4;
        indeksi[i * 6 + 4] = 2 + i * 4;
        indeksi[i * 6 + 5] = 3 + i * 4;
    }

    glBindVertexArray(m_VAO_2D_p);
    glUseProgram(m_shader_program_2D_p);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO_2D_p);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO_2D_p);

    //* Pošiljanje točk in indeksov v GPU
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 16 * niz.size(), tocke);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(uint32_t) * 6 * niz.size(), indeksi);

    //* Aktiviranje teksture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, pisava.m_tekstura_ID);

    //* Nastavljanje barv in teksture
    glUniform4f(glGetUniformLocation(m_shader_program_2D_p, "u_b_obj"), b_besedila.r, b_besedila.g, b_besedila.b, b_besedila.a);
    glUniform1i(glGetUniformLocation(m_shader_program_2D_p, "u_tek_id"), 0);

    mat::mat3 transformacija(1);
    transformacija = mat::pozicijska(transformacija, pozicija);
    // std::cout << transformacija << "\n";
    transformacija = m_ortho_mat_2D * transformacija;
    glUniformMatrix3fv(glGetUniformLocation(m_shader_program_2D_p, "u_orto"), 1, GL_TRUE, &transformacija.mat[0][0]);

    glDrawElements(GL_TRIANGLES, 6 * niz.size(), GL_UNSIGNED_INT, 0);

    //* Nastavljanej na privzeto vrednost
    glBindVertexArray(0);
    glUseProgram(0);

    //* Sprostitev pomnilnika
    delete[] tocke;
    delete[] indeksi;
}

mat::vec2 Risalnik::dobi_velikost_okna()
{
    return m_velikost_okna;
}

void Risalnik::zapri_okno()
{
    glfwSetWindowShouldClose(m_glfw_okno, 1);
    glfwDestroyWindow(m_glfw_okno);
    glfwTerminate();
    exit(1);
}

void Risalnik::posodobi_gumb_miske(GLFWwindow *window, int gumb, int akcija, int mods)
{
    if (gumb == GLFW_MOUSE_BUTTON_LEFT && akcija == GLFW_PRESS)
        miskin_gumb.levi_gumb = true;
    else
        miskin_gumb.levi_gumb = false;

    if (gumb == GLFW_MOUSE_BUTTON_RIGHT && akcija == GLFW_PRESS)
        miskin_gumb.desni_gumb = true;
    else
        miskin_gumb.desni_gumb = false;
}
void Risalnik::aktivnost_kazalca_miske(bool aktivnost)
{
    if (aktivnost)
        glfwSetInputMode(m_glfw_okno, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    else
        glfwSetInputMode(m_glfw_okno, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
