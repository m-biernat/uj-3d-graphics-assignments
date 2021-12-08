//
// Created by pbialas on 25.09.2020.
//

#include "app.h"

#include <iostream>
#include <vector>
#include <tuple>

#include "Application/utils.h"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Engine/Mesh.h"
#include "Engine/Material.h"

#include "spdlog/spdlog.h"

#define STB_IMAGE_IMPLEMENTATION  1
#include "3rdParty/stb/stb_image.h"

xe::Mesh *pyramid_mesh;

void SimpleShapeApplication::init()
{
    int w, h;
    std::tie(w, h) = frame_buffer_size();
    
    set_camera(new Camera);
    set_controler(new CameraControler(camera()));

    camera()->perspective(glm::pi<float>() / 4.0f, (float)w / h, 0.1f, 100.0f);

    glm::vec3 eye(0.0, 0.0, 5.0);      // Pozycja kamery
    glm::vec3 center(0.0, 0.0, 0.0);   // Kierunek patrzenia
    glm::vec3 up(0.0, 1.0, 0.0);       // Ustawienie osi y jako tej do gory
    camera()->look_at(eye, center, up);


    std::vector<GLfloat> pyramid_vertices = {
        -0.5, -0.5, 0.5,    0.1910, 0.5000,     // p0 (0)
        0.5, -0.5, 0.5,     0.5000, 0.8090,     // p1 (1)
        -0.5, -0.5, -0.5,   0.5000, 0.1910,     // p3 (2)
        0.5, -0.5, -0.5,    0.8090, 0.5000,     // p2 (3)

        0.5, -0.5, -0.5,    0.8090, 0.5000,     // p2 (4)
        -0.5, -0.5, -0.5,   0.5000, 0.1910,     // p3 (5)
        0.0, 0.5, 0.0,      1.0000, 0.0000,     // p4 (6)

        0.5, -0.5, 0.5,     0.8090, 0.5000,     // p1 (7)
        0.5, -0.5, -0.5,    0.5000, 0.8090,     // p2 (8)
        0.0, 0.5, 0.0,      1.0000, 1.0000,     // p4 (9)

        -0.5, -0.5, 0.5,    0.5000, 0.8090,     // p0 (10)
        0.5, -0.5, 0.5,     0.1910, 0.5000,     // p1 (11)
        0.0, 0.5, 0.0,      0.0000, 1.0000,     // p4 (12)

        -0.5, -0.5, -0.5,   0.5000, 0.1910,     // p3 (13)
        -0.5, -0.5, 0.5,    0.1910, 0.5000,     // p0 (14)
        0.0, 0.5, 0.0,      0.0000, 0.0000      // p4 (15)
    };

    std::vector<GLushort> pyramid_indices = {
        0, 2, 1, 2, 3, 1, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
    };

    pyramid_mesh = new xe::Mesh();

    pyramid_mesh->allocate_vertex_buffer(pyramid_vertices.size() * sizeof(GLfloat), GL_STATIC_DRAW);
    pyramid_mesh->allocate_index_buffer(pyramid_indices.size() * sizeof(GLfloat), GL_STATIC_DRAW);

    pyramid_mesh->load_vertices(0, pyramid_vertices.size() * sizeof(GLfloat), pyramid_vertices.data());
    pyramid_mesh->load_indices(0, pyramid_indices.size() * sizeof(GLfloat), pyramid_indices.data());
    
    pyramid_mesh->vertex_attrib_pointer(0, 3, GL_FLOAT, 5 * sizeof(GLfloat), 0);
    pyramid_mesh->vertex_attrib_pointer(1, 2, GL_FLOAT, 5 * sizeof(GLfloat), 3 * sizeof(GLfloat));


    xe::ColorMaterial::init();
    xe::ColorMaterial *texture_mat = new xe::ColorMaterial(glm::vec4(1.0f));

    pyramid_mesh->add_submesh(0, 18, texture_mat);


    stbi_set_flip_vertically_on_load(true);
    GLint width, height, channels;
    auto texture_file = std::string(ROOT_DIR) + "/Models/multicolor.png";
    auto img = stbi_load(texture_file.c_str(), &width, &height, &channels, 0);
    if (!img) {
        spdlog::warn("Could not read image from file `{}'", texture_file);
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, 0);

    texture_mat->set_texture(texture);
    texture_mat->set_texture_unit(0);


    glGenBuffers(1, &u_pvm_buffer_);
    glBindBuffer(GL_UNIFORM_BUFFER, u_pvm_buffer_);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, u_pvm_buffer_);


    glClearColor(0.81f, 0.81f, 0.8f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glViewport(0, 0, w, h);
}

void SimpleShapeApplication::frame()
{
    auto PVM = camera()->projection() * camera()->view();
    glBindBuffer(GL_UNIFORM_BUFFER, u_pvm_buffer_);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &PVM[0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    
    pyramid_mesh->draw();
}

void SimpleShapeApplication::framebuffer_resize_callback(int w, int h) {
    Application::framebuffer_resize_callback(w, h);
    glViewport(0, 0, w, h); 
    camera()->set_aspect((float) w / h);
}
