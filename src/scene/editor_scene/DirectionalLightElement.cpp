#include "DirectionalLightElement.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtx/component_wise.hpp>

#include "rendering/imgui/ImGuiManager.h"
#include "scene/SceneContext.h"

std::unique_ptr<EditorScene::DirectionalLightElement> EditorScene::DirectionalLightElement::new_default(const SceneContext& scene_context, EditorScene::ElementRef parent) {
    auto light_element = std::make_unique<DirectionalLightElement>(
        parent,
        "New Directional Light",
        glm::vec3{1.0f, 1.0f, 0.0f}, // Correct direction vector
        DirectionalLight::create(glm::vec3{0.5f, 0.3f, 0.0f}, glm::vec4{1.0f, 1.0f, 1.0f, 1.0f}), // This should correctly create a DirectionalLight

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
    // Added for attenuation
    // light_element->light->light_attenuation = j["light_attenuation"];

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
        {"pitch", pitch},
        {"yaw", yaw},
        // Added for parameterisation of attenuation
        // {"light_attenuation", light->light_attenuation}
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

    // Added for light attenuation
    // ImGui::Spacing();
    // transformUpdated |= ImGui::DragFloat3("Light Attenuation", &light->light_attenuation[0], 0.01f, 0.0f, FLT_MAX);


    if (transformUpdated) {
        update_instance_data();
    }
}

void EditorScene::DirectionalLightElement::update_instance_data() {
  //trans based on dir moves in world
  // CHANGE POSITION
    // transform = glm::translate(direction);

    if (!EditorScene::is_null(parent)) {
        // Post multiply by transform so that local transformations are applied first
        transform = (*parent)->transform * transform;
    }

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
