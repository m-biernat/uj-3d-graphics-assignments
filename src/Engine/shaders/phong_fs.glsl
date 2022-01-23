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
in vec4 view_in_vs;

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
    vec3 ambient_light;
    uint n_p_lights;
    PointLight p_light[MAX_POINT_LIGHTS];
};

float Ns = 500.0f;
vec3 Ks = vec3(1.0f);

void main() {
    vec4 color = vec4(1.0f);

    if (use_map_Kd)
        color = Kd * texture(map_Kd, vertex_texcoords);
    else
        color = Kd;

    vec3 ambient = color.rgb * ambient_light;

    vec3 normal = normalize(vertex_normals_in_vs);
    if(!gl_FrontFacing)
        normal = -normal;
    
    vec3 pos_in_vs = vec3(vertex_coords_in_vs);
    vec3 view_dir = normalize(view_in_vs.xyz - pos_in_vs);

    vec3 diffuse = vec3(0.0f);
    vec3 specular = vec3(0.0f);

    for (int i = 0; i < n_p_lights; i++) {
        vec3 light_dir = normalize(p_light[i].position_in_vs - pos_in_vs);
        
        float diff = max(dot(normal, light_dir), 0.0f);
        diffuse += diff * p_light[i].color * p_light[i].intensity * color.rgb;

        vec3 half_dir = normalize(light_dir + view_dir);
        float spec = pow(max(dot(normal, half_dir), 0.0f), Ns);
        specular += spec * Ks;
    }

    vec4 result = vec4(ambient + diffuse + specular, color.a);

    vFragColor = normalize(result);
}
