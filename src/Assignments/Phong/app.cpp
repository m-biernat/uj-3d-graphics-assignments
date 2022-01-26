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
#include "Engine/ColorMaterial.h"
#include "Engine/PhongMaterial.h"
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


    xe::ColorMaterial::init();
    xe::PhongMaterial::init();


    pyramid_mesh = xe::load_mesh_from_obj(std::string(ROOT_DIR) + "/Models/square.obj",
                                          std::string(ROOT_DIR) + "/Models");


    add_ambient(glm::vec3(1.0f, 1.0f, 1.0f));
    add_light(xe::PointLight(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 5.0f));
    count_lights();


    glGenBuffers(1, &u_transform_buffer_);
    glBindBuffer(GL_UNIFORM_BUFFER, u_transform_buffer_);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + sizeof(glm::mat4x3) + sizeof(glm::vec4), nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, u_transform_buffer_);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glGenBuffers(1, &u_light_buffer_);
    glBindBuffer(GL_UNIFORM_BUFFER, u_light_buffer_);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec3) 
                                    + sizeof(unsigned int) 
                                    + sizeof(xe::PointLight) * p_lights_.size(), nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 2, u_light_buffer_);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);


    glClearColor(0.81f, 0.81f, 0.8f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glViewport(0, 0, w, h);
}

void SimpleShapeApplication::frame()
{
    auto PV = camera()->projection() * camera()->view();
    
    
    glBindBuffer(GL_UNIFORM_BUFFER, u_light_buffer_);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(ambient_), &ambient_);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(ambient_), sizeof(unsigned int), &n_p_lights_);
    for (int i = 0; i < p_lights_.size(); i++)
        p_lights_[i].position_in_vs = camera()->view() * glm::vec4(p_lights_[i].position_in_ws, 1.0f);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(ambient_) + sizeof(unsigned int), 
                                       sizeof(xe::PointLight) * p_lights_.size(), p_lights_.data());
    glBindBuffer(GL_UNIFORM_BUFFER, 0);


    auto M = glm::mat4(1.0f);

    auto PVM = PV * M;
    auto VM = camera()->view() * M;
    
    auto R = glm::mat3(VM);
    auto N = glm::mat3(glm::cross(R[1], R[2]), glm::cross(R[2], R[0]), glm::cross(R[0], R[1]));

    auto view = camera()->view() * glm::vec4(camera()->position(), 1.0f);

    glBindBuffer(GL_UNIFORM_BUFFER, u_transform_buffer_);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &PVM[0]);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &VM[0]);
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::vec3), &N[0]);
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + sizeof(glm::vec4), sizeof(glm::vec3), &N[1]);
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + 2 * sizeof(glm::vec4), sizeof(glm::vec3), &N[2]);
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + 3 * sizeof(glm::vec4), sizeof(glm::vec4), &view);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    pyramid_mesh->draw();
}

void SimpleShapeApplication::framebuffer_resize_callback(int w, int h) {
    Application::framebuffer_resize_callback(w, h);
    glViewport(0, 0, w, h); 
    camera()->set_aspect((float) w / h);
}
