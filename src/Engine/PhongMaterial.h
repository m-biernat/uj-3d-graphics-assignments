#pragma once

#include "Material.h"

#include <glad/gl.h>
#include <glm/glm.hpp>

#include "Application/utils.h"

namespace xe {

    class PhongMaterial : public Material {
    public:
        PhongMaterial(const glm::vec4 color) : color_(color) {
            texture_ = 0u;
            texture_unit_ = 0u;
        }

        void bind();

        static void init();

        static GLuint program() { return shader_; }

        GLuint get_texture() { return texture_; }
        void set_texture(GLuint texture) { texture_ = texture; }

        GLuint get_texture_unit() { return texture_unit_; }
        void set_texture_unit(GLuint texture_unit) { texture_unit_ = texture_unit; }

    private:
        static GLuint shader_;
        static GLuint color_uniform_buffer_;

        glm::vec4 color_;

        static GLint uniform_map_Kd_location_;
        GLuint texture_;
        GLuint texture_unit_;
    };

}