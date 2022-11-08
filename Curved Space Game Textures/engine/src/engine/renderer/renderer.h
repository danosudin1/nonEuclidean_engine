#pragma once

#include "render_command.h"
#include "shader.h"
#include "model.h"
#include "camera.h"
#include "engine/entities/game_object.h"

namespace engine
{
    class renderer
    {
    public:
        static void init();
        static void resize(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

        static void begin_scene(const camera& camera, const ref<shader>& shader);
        static void end_scene();

        static void submit(
            const ref<shader>& shader, 
            const ref<vertex_array>& vertex_array, 
            const glm::mat4& transform = glm::mat4(1.f),
			const bool& lines = false,
			const int& patches = 0);

        static void submit(
            const ref<shader>& shader, 
            const ref<mesh>& mesh,
            const glm::mat4& transform = glm::mat4(1.f),
			const bool& lines = false,
			const bool& patches = false);

		static void submit(
            const ref<shader>& shader, 
            const ref<game_object>& object,
			const glm::mat4& transform = glm::mat4(1.f),
			const bool& lines = true,
			const bool& patches = false);

		static void submit_non_curved_tess(
			const ref<shader>& shader,
			const ref<game_object>& object,
			const int tessellation);

		static void submit_curved_lines(
			const ref<shader>& shader,
			const ref<game_object>& object,
			const int tessellation,
			const float K,
			const glm::mat4& transform = glm::mat4(1.f));

		static void submit_curved_texture(
			const ref<shader>& shader,
			const ref<game_object>& object,
			const int tessellation,
			const float K,
			const glm::mat4& transform = glm::mat4(1.f));

        static e_api_type api() { return renderer_api::api(); }
        static renderer& get() { return *s_instance; }
        static const scope<shader_library>& shaders_library() { return get().m_shader_library; }
    private:
        struct scene_data
        {
            glm::mat4 view_projection_matrix; 
            ref<shader> shader; 
        };

        static scene_data* s_scene_data;
        static renderer*   s_instance;
        scope<shader_library> m_shader_library;
    };
}
