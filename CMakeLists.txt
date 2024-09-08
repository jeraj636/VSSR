cmake_minimum_required(VERSION 3.20)

project(VSSR)

option(JE_WIN "Gradim za windows?" OFF)

if(JE_WIN)
    set(OPEN_GL_LIB opengl32)
else()
    set(OPEN_GL_LIB GL)
endif()


add_subdirectory(lib/glfw)
add_executable(${PROJECT_NAME}
lib/glad/src/glad.c
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
)

target_include_directories(${PROJECT_NAME} PUBLIC
lib/glad/include/
lib/glfw/include/
lib/stb/
)

target_link_libraries(${PROJECT_NAME}
glfw
${OPEN_GL_LIB}
)