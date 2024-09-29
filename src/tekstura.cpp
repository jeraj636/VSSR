#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "risalnik.h"
#include "dnevnik.h"
#include "tekstura.h"

uint32_t nalozi_teksturo(const std::string &pot_do_teksture)
{

    //* Branje podatkov iz slike
    int sirina, visina, kanali;
    unsigned char *podatki = stbi_load(pot_do_teksture.c_str(), &sirina, &visina, &kanali, 0);

    if (podatki == NULL)
    {
        napaka("tekstura.cpp :: Napaka pri nalaganju slike: %s", pot_do_teksture.c_str());
        glfwTerminate();
        exit(1);
    }

    //* Ustvarjanje teksture
    uint32_t tekstura;
    glGenTextures(1, &tekstura);
    glBindTexture(GL_TEXTURE_2D, tekstura);

    //* Nastavljanje parametrov teksture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR);

    //*generiranje teksture iz prebrane slike
    switch (kanali)
    {
    case 1:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, sirina, visina, 0, GL_RGBA, GL_UNSIGNED_BYTE, podatki);
        break;
    case 3:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sirina, visina, 0, GL_RGBA, GL_UNSIGNED_BYTE, podatki);
        break;
    case 4:
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sirina, visina, 0, GL_RGBA, GL_UNSIGNED_BYTE, podatki);
        break;
    default:
        break;
    };

    stbi_image_free(podatki);
    return tekstura;
}

void sprosti_teksturo(uint32_t *tekstura)
{
    glDeleteTextures(1, tekstura);
}
