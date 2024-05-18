#ifndef DIRECTIONAL_LIGHT_ELEMENT_H
#define DIRECTIONAL_LIGHT_ELEMENT_H

#include "SceneElement.h"
#include "scene/SceneContext.h"

namespace EditorScene {
    class DirectionalLightElement : public SceneElement {
        float init_pitch = 0.0f;
        float init_yaw = 0.0f;

        float pitch;
        float yaw;

        const float YAW_SPEED = 0.01f;
        const float PITCH_SPEED = 0.01f;

        // QUESTION
        // NOTE: Should this calc be done in the shader
        const float YAW_PERIOD = 2.0f * (float) M_PI;
        const float PITCH_MIN = (float) -M_PI_2 + 0.01f;
        const float PITCH_MAX = (float) M_PI_2 - 0.01f;

    public:
        static constexpr const char* ELEMENT_TYPE_NAME = "Directional Light";


        glm::vec3 direction;
        // glm::vec3 position;
        bool visible = true;
        float visual_scale = 1.0f;
        std::shared_ptr<DirectionalLight> light;
        std::shared_ptr<EmissiveEntityRenderer::Entity> light_indicator;

        DirectionalLightElement(const ElementRef& parent, std::string name, glm::vec3 direction, std::shared_ptr<DirectionalLight> light, std::shared_ptr<EmissiveEntityRenderer::Entity> light_indicator) :
            SceneElement(parent, std::move(name)), direction(direction), light(std::move(light)), light_indicator(std::move(light_indicator)) {}

        static std::unique_ptr<DirectionalLightElement> new_default(const SceneContext& scene_context, ElementRef parent);
        static std::unique_ptr<DirectionalLightElement> from_json(const SceneContext& scene_context, ElementRef parent, const json& j);

        [[nodiscard]] json into_json() const override;

        void add_imgui_edit_section(MasterRenderScene& render_scene, const SceneContext& scene_context) override;

        void update_instance_data() override;

        void add_to_render_scene(MasterRenderScene& target_render_scene) override {
            target_render_scene.insert_entity(light_indicator);
            target_render_scene.insert_light(light);
        }

        void remove_from_render_scene(MasterRenderScene& target_render_scene) override {
            target_render_scene.remove_entity(light_indicator);
            target_render_scene.remove_light(light);
        }

        [[nodiscard]] const char* element_type_name() const override;
    };
}

#endif // DIRECTIONAL_LIGHT_ELEMENT_H
