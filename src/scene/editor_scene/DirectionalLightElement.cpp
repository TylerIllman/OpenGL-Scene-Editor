#include "DirectionalLightElement.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtx/component_wise.hpp>

#include "rendering/imgui/ImGuiManager.h"
#include "scene/SceneContext.h"

std::unique_ptr<EditorScene::DirectionalLightElement> EditorScene::DirectionalLightElement::new_default(const SceneContext& scene_context, EditorScene::ElementRef parent) {
    auto light_element = std::make_unique<DirectionalLightElement>(
        parent,
        "New Directional Light",
        glm::vec3{10.0f, 10.0f, 0.0f}, // Correct direction vector
        DirectionalLight::create(glm::vec3{10.0f, 10.0f, 0.0f}, glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}), // This should correctly create a DirectionalLight

        EmissiveEntityRenderer::Entity::create(
            scene_context.model_loader.load_from_file<EmissiveEntityRenderer::VertexData>("cylinder.obj"),
            EmissiveEntityRenderer::InstanceData{
                glm::mat4{}, // Set via update_instance_data()
                EmissiveEntityRenderer::EmissiveEntityMaterial{
                    glm::vec4{1.0f}
                }
            },
            EmissiveEntityRenderer::RenderData{
                scene_context.texture_loader.default_white_texture()
            }
        )
    );

    light_element->update_instance_data();
    return light_element;
}

std::unique_ptr<EditorScene::DirectionalLightElement> EditorScene::DirectionalLightElement::from_json(const SceneContext& scene_context, EditorScene::ElementRef parent, const json& j) {
    auto light_element = new_default(scene_context, parent);

    // light_element->position = j["position"];
    light_element->light->colour = j["colour"];
    light_element->visible = j["visible"];
    light_element->visual_scale = j["visual_scale"];

    light_element->light->pitch = j["pitch"];
    light_element->light->pitch = j["yaw"];

    light_element->update_instance_data();
    return light_element;
}

json EditorScene::DirectionalLightElement::into_json() const {
    return {
        // {"position",     position},
        {"colour",       light->colour},
        {"visible",      visible},
        {"visual_scale", visual_scale},
        {"pitch", light->pitch},
        {"yaw", light->yaw},
    };
}

void EditorScene::DirectionalLightElement::add_imgui_edit_section(MasterRenderScene& render_scene, const SceneContext& scene_context) {
    ImGui::Text("Directional Light");
    SceneElement::add_imgui_edit_section(render_scene, scene_context);

    ImGui::Text("Local Transformation");
    bool transformUpdated = false;
    // transformUpdated |= ImGui::DragFloat3("Translation", &position[0], 0.01f);
    ImGui::DragDisableCursor(scene_context.window);
    ImGui::Spacing();

    ImGui::Spacing();

    ImGui::Text("Light Properties");
    transformUpdated |= ImGui::ColorEdit3("Colour", &light->colour[0]);
    ImGui::Spacing();
    ImGui::DragFloat("Intensity", &light->colour.a, 0.01f, 0.0f, FLT_MAX);
    ImGui::DragDisableCursor(scene_context.window);

    ImGui::Spacing();
    ImGui::Text("Visuals");

    transformUpdated |= ImGui::Checkbox("Show Visuals", &visible);
    transformUpdated |= ImGui::DragFloat("Visual Scale", &visual_scale, 0.01f, 0.0f, FLT_MAX);
    ImGui::DragDisableCursor(scene_context.window);


    // float pitch_degrees = glm::degrees(pitch);
    transformUpdated |= ImGui::SliderFloat("Pitch", &light->pitch, -90.0f, 90.0f);

    // float yaw_degrees = glm::degrees(yaw);
    transformUpdated |= ImGui::DragFloat("Yaw", &light->yaw);

    if (transformUpdated) {
        update_instance_data();
    }
}

void EditorScene::DirectionalLightElement::update_instance_data() {

    // Convert pitch and yaw from degrees to radians
    float pitch_rad = glm::radians(light->pitch);
    float yaw_rad = glm::radians(light->yaw);

    // Create rotation matrices for pitch and yaw
    glm::mat4 pitch_mat = glm::rotate(glm::mat4(1.0f), pitch_rad, glm::vec3(1, 0, 0));  // Rotate around X-axis
    glm::mat4 yaw_mat = glm::rotate(glm::mat4(1.0f), yaw_rad, glm::vec3(0, 1, 0));      // Rotate around Y-axis

    // Combine the rotations, applying yaw first, then pitch
    glm::mat4 combined_rotation = yaw_mat * pitch_mat;

    // Apply the combined rotation to the light's direction vector
    //glm::vec4 dir_vec4 = glm::vec4(light->direction, 1.0);  // Convert to vec4 for matrix multiplication
    glm::vec4 dir_vec4 = combined_rotation * glm::vec4{0.0f,7.0f,0.0f,1.0f};                // Apply combined rotation
    // dir_vec4 = combined_rotation * light->direction;                // Apply combined rotation

    // Update the light's direction with the new rotated direction
    light->direction = glm::vec3(dir_vec4);

    transform = combined_rotation;


    if (!EditorScene::is_null(parent)) {
        // Post multiply by transform so that local transformations are applied first
        transform = (*parent)->transform * transform;
    }

    // Set the position (translation) part of the transform matrix
    transform[3] = glm::vec4(light->direction, 1.0f);

     //light->position = glm::vec3(transform[3]); // Extract translation from matrix
    if (visible) {
         light_sphere->instance_data.model_matrix = transform * glm::scale(glm::vec3{0.1f * visual_scale});
    } else {
        // Throw off to infinity as a hacky way to make model invisible
         light_sphere->instance_data.model_matrix = glm::scale(glm::vec3{std::numeric_limits<float>::infinity()}) * glm::translate(glm::vec3{std::numeric_limits<float>::infinity()});
    }

    glm::vec3 normalised_colour = glm::vec3(light->colour) / glm::compMax(glm::vec3(light->colour));
     light_sphere->instance_data.material.emission_tint = glm::vec4(normalised_colour, light_sphere->instance_data.material.emission_tint.a);

}

const char* EditorScene::DirectionalLightElement::element_type_name() const {
    return ELEMENT_TYPE_NAME;
}
