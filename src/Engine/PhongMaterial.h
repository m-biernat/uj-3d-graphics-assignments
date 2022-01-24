#pragma once

#include "Material.h"

#include <glad/gl.h>
#include <glm/glm.hpp>

#include "Application/utils.h"

namespace xe {

    class PhongMaterial : public Material {
    public:
        PhongMaterial() {
            map_Kd = 0u;
            map_Kd_unit = 0u;
        }

        void bind();

        static void init();

        static GLuint program() { return shader_; }

        glm::vec4 Kd;
        
        GLuint map_Kd;
        GLuint map_Kd_unit;
        
        glm::vec4 Ka;
        glm::vec4 Ks;

        float Ns;

    private:
        static GLuint shader_;
        static GLuint material_uniform_buffer_;

        static GLint uniform_map_Kd_location_;
    };

}