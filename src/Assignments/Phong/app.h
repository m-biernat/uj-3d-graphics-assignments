//
// Created by pbialas on 05.08.2020.
//

#pragma once

#include <vector>

#include "Application/application.h"
#include "Application/utils.h"

#include "glad/gl.h"

#include "camera.h"
#include "camera_controler.h"

#include "Engine/Light.h"

class SimpleShapeApplication : public xe::Application
{
public:
    SimpleShapeApplication(int width, int height, std::string title, bool debug) : Application(width, height, title, debug) {}

    void init() override;

    void frame() override;

    void framebuffer_resize_callback(int w, int h) override; 

    void set_camera(Camera *camera) { camera_ = camera; }
    
    Camera *camera() { return camera_; }
    
    ~SimpleShapeApplication() {
        if (camera_) {
            delete camera_;
        }
    }

    void scroll_callback(double xoffset, double yoffset) override {
         Application::scroll_callback(xoffset, yoffset);   
         camera()->zoom(yoffset / 30.0f);
    }

    void set_controler(CameraControler *controler) { controler_ = controler; }

    void mouse_button_callback(int button, int action, int mods) {
        Application::mouse_button_callback(button, action, mods);

        if (controler_) {
            double x, y;
            glfwGetCursorPos(window_, &x, &y);

            if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
                controler_->LMB_pressed(x, y);

            if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
                controler_->LMB_released(x, y);
        }
    }

    void cursor_position_callback(double x, double y) {
        Application::cursor_position_callback(x, y);
        if (controler_) {
            controler_->mouse_moved(x, y);
        }
    }

    void add_light(const xe::PointLight &p_light) {
        p_lights_.push_back(p_light); 
    }

    void add_ambient(glm::vec3 ambient) {
        ambient_ = ambient;
    }

    void count_lights() {
        n_p_lights_ = p_lights_.size();
    }

private:
    GLuint vao_;

    GLuint u_transform_buffer_;

    Camera *camera_;

    CameraControler *controler_;

    glm::vec3 ambient_;
    std::vector<xe::PointLight> p_lights_;
    unsigned int n_p_lights_;

    GLuint u_light_buffer_;
};