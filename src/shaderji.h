#ifndef SHADERJI_H
#define SHADERJI_H

// ! Polepšja kodo
const char *fragment_shader_2D_s = R"(


#version 330 core
out vec4 frag_color;

in vec2 tex_poz;

uniform vec4 u_barva;
uniform sampler2D u_tekstura;

void main()
{
    vec4 tekstura = texture(u_tekstura,tex_poz);
    frag_color = tekstura * u_barva;
}


)";

const char *fragment_shader_2D_p_s = R"(


#version 330 core
in vec2 tek_poz;

uniform vec4 u_b_obj;
uniform sampler2D u_tek_id;

out vec4 frag_color;

void main()
{
    vec4 tekstura = texture(u_tek_id,tek_poz);
    frag_color = tekstura.r * u_b_obj;
}


)";

const char *fragment_shader_3D_s = R"(


#version 330 core
out vec4 frag_color;

in vec3 barva;
in vec3 normale;
in vec4 trenutna_pozicija;

uniform vec4 u_barva;// se ne uporablja
uniform sampler2D u_tekstura;
in vec3 pozicija_luci;

void main()
{
float moc_ambienta = 2.3f;
vec3 barva_luci = vec3(1.0f,1.0f,1.0f);

vec3 normala = normalize(normale );
vec3 smer_luci = normalize(pozicija_luci - vec3(trenutna_pozicija.xyz));

float diffuse = max(dot(normala, vec3(smer_luci.xyz)) , 0.0f) + 0.1;

frag_color = vec4(barva * barva_luci * diffuse * moc_ambienta,1.0) * u_barva;
}


)";

const char *vertex_shader_2D_p_s = R"(
#version 330 core


layout (location = 0) in vec2 a_poz;
layout (location = 1) in vec2 i_tek_poz;

uniform mat3 u_orto;

out vec2 tek_poz;

void main()
{
    gl_Position = vec4(u_orto * vec3(a_poz,1.0),1.0);
    gl_Position.z =0;
    tek_poz = i_tek_poz;
}


)";

const char *vertex_shader_2D_s = R"(
#version 330 core

layout (location = 0) in vec2 a_pos;
layout (location = 1) in vec2 a_t_pos;

out vec2 tex_poz;

uniform mat3 u_transformacija;
uniform mat3 u_orto;

void main()
{
    tex_poz = a_t_pos;
    gl_Position = vec4(u_transformacija * vec3(a_pos,1.0),1.0);
    gl_Position.z =-0.9;
}

)";

const char *vertex_shader_3D_s = R"(
#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normale;
layout (location = 2) in vec3 a_barva;

out vec3 barva;
out vec3 normale;
out vec3 frag_pos;
out vec4 trenutna_pozicija;
out vec3 pozicija_luci;

uniform mat4 u_transformacija;
uniform mat4 u_projekcija;
uniform mat4 u_kamera;
uniform vec3 u_pozicija_luci;
void main()
{
    barva = a_barva;
    normale = mat3(transpose(inverse(u_transformacija))) * a_normale; //!To naj bi delal 
    //normale =  a_normale;
    trenutna_pozicija=u_transformacija * vec4(a_pos,1.0);
    pozicija_luci = u_pozicija_luci;
    gl_Position = u_projekcija*u_kamera* trenutna_pozicija;
    frag_pos = vec3(gl_Position.x,gl_Position.y,gl_Position.z);
}
)";

#endif