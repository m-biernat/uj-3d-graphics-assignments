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

    // A vector containing the x,y,z vertex coordinates for the triangle.
    std::vector<GLfloat> vertices = {
        -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,    // 0
        0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,     // 1
        0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,     // 2
        -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,    // 3 (0)
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,   // 4
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,    // 5
        0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f      // 6 (1)
    };

    std::vector<GLushort> indices = {
        0, 1, 2, 3, 4, 5, 3, 5, 6
    };

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

    float strength = 0.5;
    float color[3] = {1.0, 1.0, 1.0};

    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float), &strength);
    glBufferSubData(GL_UNIFORM_BUFFER, 4 * sizeof(float), 3 * sizeof(float), color);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    
    glBindBuffer(GL_UNIFORM_BUFFER, u_buffer_handle[1]);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, u_buffer_handle[1]);

    // Stare transfromacje zostały zastąpione następującymi wektorami:
    glm::vec3 translation(0.0, -0.25, 0.0);
    glm::vec3 rotation(glm::radians(0.0), glm::radians(0.0), glm::radians(30.0));
    glm::vec3 scale(0.5, 0.5, 0.0);

    glm::mat4 PVM(1.0);

    glm::mat4 Model(1.0);
    Model = glm::translate(Model, translation);
    Model *= glm::toMat4(glm::quat(rotation));  // Zmieniam wektor katow Euelera na Quaternion i bezposrednio wymnazam macierz
    //Model = glm::rotate(Model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	//Model = glm::rotate(Model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    //Model = glm::rotate(Model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    Model = glm::scale(Model, scale);
    // Po tych transformacjach model wygląda tak jak w poprzednim zadaniu.

    glm::mat4 View(1.0);
    glm::vec3 eye(0.0, 0.0, 5.0);      // Pozycja kamery
    glm::vec3 center(0.0, 0.0, 0.0);   // Kierunek patrzenia
    glm::vec3 up(0.0, 1.0, 0.0);       // Ustawienie osi y jako tej do gory
    View = glm::lookAt(eye, center, up);

    glm::mat4 Projection(1.0);
    Projection = glm::perspective(glm::radians(30.0), 1.0, 0.01, 1000.0);

    PVM = Projection * View * Model;

    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &PVM);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // Setting the background color of the rendering window,
    // I suggest not to use white or black for better debuging.
    glClearColor(0.81f, 0.81f, 0.8f, 1.0f);

    // This setups an OpenGL vieport of the size of the whole rendering window.
    auto [w, h] = frame_buffer_size();
    glViewport(0, 0, w, h);

    glUseProgram(program);
}

//This functions is called every frame and does the actual rendering.
void SimpleShapeApplication::frame()
{
    // Binding the VAO will setup all the required vertex buffers.
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_SHORT, reinterpret_cast<GLvoid *>(0));
    //glDrawArrays(GL_TRIANGLES, 0, 9);
    glBindVertexArray(0);
}
