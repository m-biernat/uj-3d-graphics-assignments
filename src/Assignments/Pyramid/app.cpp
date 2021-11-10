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

int indicesCount;

void SimpleShapeApplication::init()
{
    // A utility function that reads the shader sources, compiles them and creates the program object
    // As everything in OpenGL we reference program by an integer "handle".
    auto program = xe::utils::create_program(
        {{GL_VERTEX_SHADER, std::string(PROJECT_DIR) + "/shaders/base_vs.glsl"},
         {GL_FRAGMENT_SHADER, std::string(PROJECT_DIR) + "/shaders/base_fs.glsl"}});

    if (!program)
    {
        std::cerr << "Invalid program" << std::endl;
        exit(-1);
    }

    auto [w, h] = frame_buffer_size();

    // A vector containing the x,y,z vertex coordinates for the triangle.
    std::vector<GLfloat> vertices = {
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
    std::vector<GLushort> indices = {
        0, 2, 1, 2, 3, 1, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
    };

    indicesCount = indices.size();

    // Generating the buffer and loading the vertex data into it.
    GLuint v_buffer_handle;
    glGenBuffers(1, &v_buffer_handle);
    OGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle));
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint i_buffer_handle;
    glGenBuffers(1, &i_buffer_handle);
    OGL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i_buffer_handle));
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // This setups a Vertex Array Object (VAO) that  encapsulates
    // the state of all vertex buffers needed for rendering
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle);

    // This indicates that the data for attribute 0 should be read from a vertex buffer.
    glEnableVertexAttribArray(0);
    // and this specifies how the data is layout in the buffer.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(0));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(3 * sizeof(GLfloat)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, i_buffer_handle);

    glBindVertexArray(0);
    //end of vao "recording"

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Add uniform buffer
    GLuint u_buffer_handle[2];
    glGenBuffers(2, u_buffer_handle);

    glBindBuffer(GL_UNIFORM_BUFFER, u_buffer_handle[0]);
    glBufferData(GL_UNIFORM_BUFFER, 8 * sizeof(float), nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, u_buffer_handle[0]);

    float strength = 1.0;
    float color[3] = {1.0, 1.0, 1.0};

    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float), &strength);
    glBufferSubData(GL_UNIFORM_BUFFER, 4 * sizeof(float), 3 * sizeof(float), color);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    
    glBindBuffer(GL_UNIFORM_BUFFER, u_buffer_handle[1]);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, u_buffer_handle[1]);

    // Stare transfromacje zostały zastąpione następującymi wektorami:
    glm::vec3 translation(0.0, -0.25, 0.0);
    glm::vec3 rotation(glm::radians(90.0), glm::radians(0.0), glm::radians(0.0));
    glm::vec3 scale(0.5, 0.5, 0.0);

    glm::mat4 PVM(1.0);

    glm::mat4 Model(1.0);
    //Model = glm::translate(Model, translation);
    Model *= glm::toMat4(glm::quat(rotation));
    //Model = glm::scale(Model, scale);

    glm::mat4 View(1.0);
    glm::vec3 eye(0.0, 0.0, 5.0);      // Pozycja kamery
    glm::vec3 center(0.0, 0.0, 0.0);   // Kierunek patrzenia
    glm::vec3 up(0.0, 1.0, 0.0);       // Ustawienie osi y jako tej do gory
    View = glm::lookAt(eye, center, up);

    glm::mat4 Projection(1.0);
    Projection = glm::perspective(glm::radians(30.0f), (float)w/h, 0.01f, 1000.0f);

    PVM = Projection * View * Model;

    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &PVM);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // Setting the background color of the rendering window,
    // I suggest not to use white or black for better debuging.
    glClearColor(0.81f, 0.81f, 0.8f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // This setups an OpenGL vieport of the size of the whole rendering window.
    glViewport(0, 0, w, h);

    glUseProgram(program);
}

//This functions is called every frame and does the actual rendering.
void SimpleShapeApplication::frame()
{
    // Binding the VAO will setup all the required vertex buffers.
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_SHORT, reinterpret_cast<GLvoid *>(0));

    glBindVertexArray(0);
}
