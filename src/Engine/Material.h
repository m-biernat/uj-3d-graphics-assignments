//
// Created by Piotr Białas on 02/11/2021.
//

#pragma once

#include <glad/gl.h>
#include <glm/glm.hpp>

#include "Application/utils.h"

namespace xe {

    class Material {
    public:
        static GLuint program() { return shader_; }

        virtual void bind() = 0;

        static void init_materials();

    protected:
        static GLuint shader_;
    };

    class ColorMaterial : public Material {
    public:
        ColorMaterial(const glm::vec4 color) : color_(color) {}

        void bind();

        static void init();

    private:

        static GLuint color_uniform_buffer_;
        glm::vec4 color_;
    };


}


