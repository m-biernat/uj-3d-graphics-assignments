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
#include "Engine/mesh_loader.h"

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


    pyramid_mesh = xe::load_mesh_from_obj(std::string(ROOT_DIR) + "/Models/blue_marble.obj",
                                          std::string(ROOT_DIR) + "/Models");

    xe::ColorMaterial::init();
    xe::ColorMaterial *texture_mat = new xe::ColorMaterial(glm::vec4(1.0f));

    pyramid_mesh->add_submesh(0, 18, texture_mat);


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
