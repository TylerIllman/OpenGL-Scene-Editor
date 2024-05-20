#ifndef NUM_PL
#define NUM_PL 0
#endif

#ifndef NUM_DL
#define NUM_DL 0
#endif


// Material Properties
struct Material {
    vec3 diffuse_tint;
    vec3 specular_tint;
    vec3 ambient_tint;
    float shininess;
};

// Light Data
struct LightCalculatioData {
    vec3 ws_frag_position;
    vec3 ws_view_dir;
    vec3 ws_normal;
};

struct PointLightData {
    vec3 position;
    vec3 colour;
    // added for light attenuation
    vec3 light_attenuation;
};

// ADDED for directional lights
struct DirectionalLightData {
    vec3 direction;
    vec3 color;
};


// Calculations
const float ambient_factor = 0.002f;


// Point Lights
void point_light_calculation(PointLightData point_light, LightCalculatioData calculation_data, float shininess, inout vec3 total_diffuse, inout vec3 total_specular, inout vec3 total_ambient) {
    vec3 ws_light_offset = point_light.position - calculation_data.ws_frag_position;

    // Calculate distance between fragment and light
    float distance = length(ws_light_offset);



    // Attenuation calculation
    float attenuation = 1.0 / (point_light.light_attenuation.x + point_light.light_attenuation.y * distance + point_light.light_attenuation.z * distance * distance);

    // Ambient
    vec3 ambient_component = attenuation * ambient_factor * point_light.colour;

    // Diffuse
    vec3 ws_light_dir = normalize(ws_light_offset);
    float diffuse_factor = max(dot(ws_light_dir, calculation_data.ws_normal), 0.0f);
    vec3 diffuse_component = attenuation * diffuse_factor * point_light.colour;

    // Specular
    vec3 ws_halfway_dir = normalize(ws_light_dir + calculation_data.ws_view_dir);
    float specular_factor = pow(max(dot(calculation_data.ws_normal, ws_halfway_dir), 0.0f), shininess);
    vec3 specular_component = attenuation * specular_factor * point_light.colour;

    total_diffuse += diffuse_component;
    total_specular += specular_component;
    total_ambient += ambient_component;
}


void directional_light_calculation(DirectionalLightData directional_light, LightCalculatioData calc_data, float shininess, inout vec3 total_diffuse, inout vec3 total_specular, inout vec3 total_ambient) {

    vec3 light_dir = normalize(directional_light.direction);

    // Multiply color by 0.1 to decrease intensity
    vec3 light_color = directional_light.color * 0.1;


    float diff = max(dot(calc_data.ws_normal, light_dir), 0.0); // Calculate the diffuse strength based on angle between light and normal

    // Calculate diffuse lighting
    vec3 diffuse = diff * light_color; // Lambert's cosine law for diffuse lighting

    // Calculate specular lighting using the Blinn-Phong model
    vec3 view_dir = normalize(calc_data.ws_view_dir); // Direction from the surface point to the viewer/camera
    vec3 halfway_dir = normalize(light_dir + view_dir); // Halfway direction between light direction and view direction
    float spec = pow(max(dot(calc_data.ws_normal, halfway_dir), 0.0), shininess); // Specular strength based on shininess
    vec3 specular = spec * light_color; // Specular reflection component

    // Ambient lighting
    vec3 ambient = 0.005 * light_color; // Simple ambient lighting

    // Accumulate the computed lighting components to the total lighting
    total_diffuse += diffuse;
    total_specular += specular;
    total_ambient += ambient;

}



// Total Calculation
struct LightingResult {
    vec3 total_diffuse;
    vec3 total_specular;
    vec3 total_ambient;
};

LightingResult total_light_calculation(LightCalculatioData light_calculation_data, Material material
        #if NUM_PL > 0
        ,PointLightData point_lights[NUM_PL]
        #endif
        #if NUM_DL > 0
        ,DirectionalLightData directional_lights[NUM_DL]
        #endif
    ) {

    vec3 total_diffuse = vec3(0.0f);
    vec3 total_specular = vec3(0.0f);
    vec3 total_ambient = vec3(0.0f);

    #if NUM_PL > 0
    for (int i = 0; i < NUM_PL; i++) {
        point_light_calculation(point_lights[i], light_calculation_data, material.shininess, total_diffuse, total_specular, total_ambient);
    }
    #endif


    #if NUM_DL > 0
    for (int i = 0; i < NUM_DL; i++) {
        directional_light_calculation(directional_lights[i], light_calculation_data, material.shininess, total_diffuse, total_specular, total_ambient);
    }
    #endif

    #if NUM_DL > 0
      total_ambient /= float(NUM_DL);
    #endif

    #if NUM_PL > 0 && NUM_DL > 0
      total_ambient /= float(NUM_PL + NUM_DL);
    #elif NUM_DL > 0 
      total_ambient /= float(NUM_DL);
    #elif NUM_PL > 0
      total_ambient /= float(NUM_PL);
    #endif


    total_diffuse *= material.diffuse_tint;
    total_specular *= material.specular_tint;
    total_ambient *= material.ambient_tint;

    return LightingResult(total_diffuse, total_specular, total_ambient);
}

vec3 resolve_textured_light_calculation(LightingResult result, sampler2D diffuse_texture, sampler2D specular_map, vec2 texture_coordinate) {
    vec3 texture_colour = texture(diffuse_texture, texture_coordinate).rgb;
    vec3 specular_map_sample = texture(specular_map, texture_coordinate).rgb;

    vec3 textured_diffuse = result.total_diffuse * texture_colour;
    vec3 sampled_specular = result.total_specular * specular_map_sample;
    vec3 textured_ambient = result.total_ambient * texture_colour;

    // Mix the diffuse and ambient so that there is no ambient in bright scenes
    return max(textured_diffuse, textured_ambient) + sampled_specular;
}
