#version 460

layout(location=0) out vec4 vFragColor;

#if __VERSION__ > 410
layout(std140, binding=0) uniform Modifiers {
#else
layout(std140) uniform Color {
    #endif
    vec4 Kd;
    bool use_map_Kd;
};

in vec2 vertex_texcoords;
in vec3 vertex_normals_in_vs;
in vec4 vertex_coords_in_vs;

uniform sampler2D map_Kd;


const int MAX_POINT_LIGHTS=24;

struct PointLight {
    vec3 position_in_ws;
    vec3 position_in_vs;
    vec3 color;
    float intensity;
    float radius;
};  

layout(std140, binding=2) uniform Lights {
    vec3 ambient;
    uint n_p_lights;
    PointLight p_light[MAX_POINT_LIGHTS];
};


void main() {

    vec4 diffuse = vec4(1.0f);  // Wektor koloru powierzchni

    if (use_map_Kd)
        diffuse = Kd * texture(map_Kd, vertex_texcoords);
    else
        diffuse = Kd;

    vec3 normal = normalize(vertex_normals_in_vs);
    vec3 pos_in_vs = vec3(vertex_coords_in_vs);

    vec3 diffuse_light = vec3(0.0f);    // Wektor koloru wynikający z oświetlenia

    for (int i = 0; i < n_p_lights; i++) {
        vec3 lightDir = normalize(p_light[i].position_in_ws - pos_in_vs);
        float diff = max(dot(normal, lightDir), 0.0f);
        diffuse_light += diff * p_light[i].color * diffuse.rgb;
    }

    vec4 result = vec4(diffuse_light + diffuse.rgb * ambient, diffuse.a);

    vFragColor = normalize(result);
}
