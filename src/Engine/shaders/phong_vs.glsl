#version 460

layout(location=0) in vec4 a_vertex_position;
layout(location=1) in vec2 a_vertex_texcoords;
layout(location=5) in vec3 a_vertex_normals;

#if __VERSION__ > 410
layout(std140, binding=1) uniform Transformations {
#else
    layout(std140) uniform Transformations {
    #endif
    mat4 PVM;
    mat4 VM;
    mat3 N;
    vec4 view;
};

out vec2 vertex_texcoords;
out vec4 vertex_coords_in_vs;
out vec3 vertex_normals_in_vs;
out vec4 view_in_vs;

void main() {

    gl_Position = PVM * a_vertex_position;
    vertex_texcoords = a_vertex_texcoords;
    vertex_coords_in_vs = VM * a_vertex_position;
    vertex_normals_in_vs = normalize(N * a_vertex_normals);
    view_in_vs = view;

}
