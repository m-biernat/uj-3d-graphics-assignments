#version 410

layout(location=0) out vec4 vFragColor;

in vec4 vfrag_color;

void main() {
    vFragColor = vfrag_color;
}
