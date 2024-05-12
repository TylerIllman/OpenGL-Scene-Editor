#version 410 core
#include "../common/lights.glsl"

in VertexOut {
    LightingResult lighting_result;
    vec2 texture_coordinate;
} frag_in;

layout(location = 0) out vec4 out_colour;

// Global Data
uniform float inverse_gamma;

// ADDED BELOW LINE
// test uniform scale
uniform vec2 texture_scale;

uniform sampler2D diffuse_texture;
uniform sampler2D specular_map_texture;

void main() {

    // test hard coded scale
    // vec2 texture_scale = vec2(10.0, 10.0);

    // Scaled texture coords
    vec2 scaled_coords = frag_in.texture_coordinate * texture_scale;

    // Resolve the per vertex lighting with per fragment texture sampling.
    vec3 resolved_lighting = resolve_textured_light_calculation(frag_in.lighting_result, diffuse_texture, specular_map_texture, scaled_coords);

    out_colour = vec4(resolved_lighting, 1.0f);
    out_colour.rgb = pow(out_colour.rgb, vec3(inverse_gamma));
}
