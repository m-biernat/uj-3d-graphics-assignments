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

xe::Mesh *pyramid_mesh;

void SimpleShapeApplication::init()
{
    xe::ColorMaterial::init();

    glm::vec4 color_r(1.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 color_y(1.0f, 1.0f, 0.0f, 1.0f);
    glm::vec4 color_g(0.0f, 1.0f, 0.0f, 1.0f);
    glm::vec4 color_b(0.0f, 0.0f, 1.0f, 1.0f);
    glm::vec4 color_m(1.0f, 0.0f, 1.0f, 1.0f);

    xe::ColorMaterial *red_mat = new xe::ColorMaterial(color_r);
    xe::ColorMaterial *yellow_mat = new xe::ColorMaterial(color_y);
    xe::ColorMaterial *green_mat = new xe::ColorMaterial(color_g);
    xe::ColorMaterial *blue_mat = new xe::ColorMaterial(color_b);
    xe::ColorMaterial *magenta_mat = new xe::ColorMaterial(color_m);


    int w, h;
    std::tie(w, h) = frame_buffer_size();
    
    set_camera(new Camera);
    set_controler(new CameraControler(camera()));

    camera()->perspective(glm::pi<float>() / 4.0f, (float)w / h, 0.1f, 100.0f);

    glm::vec3 eye(0.0, 0.0, 5.0);      // Pozycja kamery
    glm::vec3 center(0.0, 0.0, 0.0);   // Kierunek patrzenia
    glm::vec3 up(0.0, 1.0, 0.0);       // Ustawienie osi y jako tej do gory
    camera()->look_at(eye, center, up);


    // A vector containing the x,y,z vertex coordinates for the triangle.
    std::vector<GLfloat> pyramid_vertices = {
        -0.5, -0.5, 0.5, 1.0, 0.0, 0.0,     // p0 (0)
        0.5, -0.5, 0.5, 1.0, 0.0, 0.0,      // p1 (1)
        -0.5, -0.5, -0.5, 1.0, 0.0, 0.0,    // p3 (2)
        0.5, -0.5, -0.5, 1.0, 0.0, 0.0,     // p2 (3)


         0.5, -0.5, -0.5, 1.0, 1.0, 0.0,    // p2 (4)
        -0.5, -0.5, -0.5, 1.0, 1.0, 0.0,    // p3 (5)
        0.0, 0.5, 0.0, 1.0, 1.0, 0.0,       // p4 (6)

        0.5, -0.5, 0.5, 0.0, 1.0, 0.0,      // p1 (7)
        0.5, -0.5, -0.5, 0.0, 1.0, 0.0,     // p2 (8)
        0.0, 0.5, 0.0, 0.0, 1.0, 0.0,       // p4 (9)

        -0.5, -0.5, 0.5, 0.0, 0.0, 1.0,     // p0 (10)
        0.5, -0.5, 0.5, 0.0, 0.0, 1.0,      // p1 (11)
        0.0, 0.5, 0.0, 0.0, 0.0, 1.0,       // p4 (12)

        -0.5, -0.5, -0.5, 1.0, 0.0, 1.0,    // p3 (13)
        -0.5, -0.5, 0.5, 1.0, 0.0, 1.0,     // p0 (14)
        0.0, 0.5, 0.0, 1.0, 0.0, 1.0        // p4 (15)
    };

    // Podstawa musi zostac rysowana zgodnie ze wskazowkami zegara (zeby byla widoczna z zewnątrz)
    std::vector<GLushort> pyramid_indices = {
        0, 2, 1, 2, 3, 1, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
    };

    pyramid_mesh = new xe::Mesh();

    pyramid_mesh->allocate_vertex_buffer(pyramid_vertices.size() * sizeof(GLfloat), GL_STATIC_DRAW);
    pyramid_mesh->allocate_index_buffer(pyramid_indices.size() * sizeof(GLfloat), GL_STATIC_DRAW);

    pyramid_mesh->load_vertices(0, pyramid_vertices.size() * sizeof(GLfloat), pyramid_vertices.data());
    pyramid_mesh->load_indices(0, pyramid_indices.size() * sizeof(GLfloat), pyramid_indices.data());
    
    pyramid_mesh->vertex_attrib_pointer(0, 3, GL_FLOAT, 6 * sizeof(GLfloat), 0);
    pyramid_mesh->vertex_attrib_pointer(1, 3, GL_FLOAT, 6 * sizeof(GLfloat), 3 * sizeof(GLfloat));

    pyramid_mesh->add_submesh(0, 6, red_mat);
    pyramid_mesh->add_submesh(6, 9, yellow_mat);
    pyramid_mesh->add_submesh(9, 12, green_mat);
    pyramid_mesh->add_submesh(12, 15, blue_mat);
    pyramid_mesh->add_submesh(15, 18, magenta_mat);


    glGenBuffers(1, &u_pvm_buffer_);
    glBindBuffer(GL_UNIFORM_BUFFER, u_pvm_buffer_);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, u_pvm_buffer_);

    // Setting the background color of the rendering window,
    // I suggest not to use white or black for better debuging.
    glClearColor(0.81f, 0.81f, 0.8f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // This setups an OpenGL vieport of the size of the whole rendering window.
    glViewport(0, 0, w, h);
}

//This functions is called every frame and does the actual rendering.
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
