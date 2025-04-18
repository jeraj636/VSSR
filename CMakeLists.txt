cmake_minimum_required(VERSION 3.20)

project(VSSR)
set(CMAKE_CXX_STANDARD 20)
option(JE_WIN "Gradim za windows?" ON)

if(JE_WIN)
    set(OPEN_GL_LIB opengl32 ws2_32)
else()
    set(OPEN_GL_LIB GL)
endif()

add_subdirectory(lib/glfw)

add_executable(${PROJECT_NAME}
lib/glad/src/glad.c
src/navodila_scena.cpp
src/main.cpp
src/barva.cpp
src/mat3.cpp
src/mat4.cpp
src/vec2.cpp
src/vec3.cpp
src/matematika.cpp
src/obj.cpp
src/objekt_2D.cpp
src/objekt_3D.cpp
src/risalnik.cpp
src/tekstura.cpp
src/kamera_2D.cpp
src/kamera_3D.cpp
src/cas.cpp
src/pisava.cpp
src/zacetna_scena.cpp
src/besedilo.cpp
src/igra_scena.cpp
src/nastavitve_scena.cpp
src/streznik.cpp
src/odjemalec.cpp
src/dnevnik.cpp
src/nebesna_kocka.cpp
)

add_executable(streznik
src/streznik_main.cpp
src/dnevnik.cpp
src/vec3.cpp
src/vec2.cpp
src/mat3.cpp
src/mat4.cpp
src/matematika.cpp
src/streznik.cpp
src/objekt_3D.cpp
src/cas.cpp
src/barva.cpp
src/obj.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC
lib/glad/include/
lib/glad/
lib/glfw/include/
lib/stb/
lib/stb/fmt/include/
)

target_link_libraries(${PROJECT_NAME}
glfw
${OPEN_GL_LIB}
)

target_link_libraries(streznik
${OPEN_GL_LIB}
)