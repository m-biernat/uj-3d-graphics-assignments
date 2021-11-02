#version 460

layout(location=0) in  vec4 a_vertex_position;
layout(location=1) in vec4 a_vertex_color;

out vec4 vfrag_color;

layout(std140, binding=1) uniform Transformations {
    vec2 scale;
    vec2 translation;
    mat2 rotation;
};

void main() {
    gl_Position.xy = rotation * (scale * a_vertex_position.xy) + translation;
    // gl_Position.xy = scale * (rotation * a_vertex_position.xy) + translation; <- dziala identycznie
    // gl_Position.xy = rotation * scale * a_vertex_position.xy + translation; <- nie dziala poprawnie
    // Operacje skalowania i rotacji muszą zostać przeprowadzone osobno (jedna z nich musi najpierw zadzialac na wierzcholkach)
    gl_Position.zw = a_vertex_position.zw;
    vfrag_color = a_vertex_color;
}
