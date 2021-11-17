#version 460

layout(location=0) out vec4 vFragColor;

in vec4 vfrag_color;

layout(std140, binding=0) uniform Modifier {
    float strength;
    vec3  color; 
};

void main() {
    vFragColor = vfrag_color * vec4(strength * color, 1.0);
}
