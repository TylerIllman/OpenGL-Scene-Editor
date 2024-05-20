#version 410 core
#include "../common/lights.glsl"

// Per vertex data
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texture_coordinate;

out VertexOut {
    vec2 texture_coordinate;
    vec3 ws_position;
    vec3 ws_normal;
} vertex_out;

// Per instance data
uniform mat4 model_matrix;
uniform mat3 normal_matrix;


// Light Data
#if NUM_PL > 0
layout (std140) uniform PointLightArray {
    PointLightData point_lights[NUM_PL];
};
#endif

// Global data
uniform vec3 ws_view_position;
uniform mat4 projection_view_matrix;

void main() {
    // Transform vertices
    vertex_out.ws_position = (model_matrix * vec4(vertex_position, 1.0f)).xyz;
    vertex_out.ws_normal = normalize(normal_matrix * normal);
    vertex_out.texture_coordinate = texture_coordinate;


    gl_Position = projection_view_matrix * vec4(vertex_out.ws_position, 1.0f);
}
