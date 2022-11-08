#include "pch.h"
#include "renderer.h"
#include "platform/opengl/gl_shader.h"
#include "engine/entities/shape.h"

engine::renderer::scene_data* engine::renderer::s_scene_data = new scene_data;
engine::renderer* engine::renderer::s_instance = new renderer();

void engine::renderer::init()
{
    s_instance->m_shader_library = std::make_unique<shader_library>();
    renderer_api::init();

    renderer::shaders_library()->load("assets/shaders/animated_mesh.glsl");
	renderer::shaders_library()->load("assets/shaders/mesh_lighting.glsl");
	renderer::shaders_library()->load("assets/shaders/mesh_material.glsl");
	renderer::shaders_library()->load("assets/shaders/text_2D.glsl");

	ref<shader> text_shader = engine::gl_shader::create("text", "assets/shaders/text_2D_vert.glsl", "", "", "", "assets/shaders/text_2D_frag.glsl");
	renderer::shaders_library()->add(text_shader);

	ref<shader> curv_shader_hyperbolic = engine::gl_shader::create("curvature_shader_hyperbolic", "assets/shaders/2dshader_texture/hyperbolic/2DShader_vert.glsl", "",
		"assets/shaders/2dshader_texture/hyperbolic/2DShader_tesc.glsl", "assets/shaders/2dshader_texture/hyperbolic/2DShader_tese.glsl", "assets/shaders/2dshader_texture/hyperbolic/2DShader_frag.glsl");

	ref<shader> curv_shader_spherical = engine::gl_shader::create("curvature_shader_spherical", "assets/shaders/2dshader_texture/spherical/2DShader_vert.glsl", "",
		"assets/shaders/2dshader_texture/spherical/2DShader_tesc.glsl", "assets/shaders/2dshader_texture/spherical/2DShader_tese.glsl", "assets/shaders/2dshader_texture/spherical/2DShader_frag.glsl");

	ref<shader> curv_shader_hyperbolic_lines = engine::gl_shader::create("curvature_shader_hyperbolic_lines", "assets/shaders/2dshader/hyperbolic/2DShader_vert.glsl", "",
		"assets/shaders/2dshader/hyperbolic/2DShader_tesc.glsl", "assets/shaders/2dshader/hyperbolic/2DShader_tese.glsl", "assets/shaders/2dshader/hyperbolic/2DShader_frag.glsl");

	ref<shader> curv_shader_spherical_lines = engine::gl_shader::create("curvature_shader_spherical_lines", "assets/shaders/2dshader/spherical/2DShader_vert.glsl", "",
		"assets/shaders/2dshader/spherical/2DShader_tesc.glsl", "assets/shaders/2dshader/spherical/2DShader_tese.glsl", "assets/shaders/2dshader/spherical/2DShader_frag.glsl");

	ref<shader> curv_equation_shader_hyperbolic = engine::gl_shader::create("curvature_equation_shader_hyperbolic", "assets/shaders/2dshader_equation/hyperbolic/2DShader_vert.glsl", "assets/shaders/2dshader_equation/hyperbolic/2DShader_geom.glsl",
		"assets/shaders/2dshader_equation/hyperbolic/2DShader_tesc.glsl", "assets/shaders/2dshader_equation/hyperbolic/2DShader_tese.glsl", "assets/shaders/2dshader_equation/hyperbolic/2DShader_frag.glsl");

	ref<shader> curv_equation_shader_spherical = engine::gl_shader::create("curvature_equation_shader_spherical", "assets/shaders/2dshader_equation/spherical/2DShader_vert.glsl", "assets/shaders/2dshader_equation/spherical/2DShader_geom.glsl",
		"assets/shaders/2dshader_equation/spherical/2DShader_tesc.glsl", "assets/shaders/2dshader_equation/spherical/2DShader_tese.glsl", "assets/shaders/2dshader_equation/spherical/2DShader_frag.glsl");

	ref<shader> curv_equation_shader_spherical_great_circle_nav = engine::gl_shader::create("curvature_equation_shader_spherical_great_circle", "assets/shaders/2dshader_equation/spherical_great_circle/2DShader_vert.glsl", "assets/shaders/2dshader_equation/spherical_great_circle/2DShader_geom.glsl",
		"assets/shaders/2dshader_equation/spherical_great_circle/2DShader_tesc.glsl", "assets/shaders/2dshader_equation/spherical_great_circle/2DShader_tese.glsl", "assets/shaders/2dshader_equation/spherical_great_circle/2DShader_frag.glsl");

	ref<shader> non_curv_shader = engine::gl_shader::create("non_curvature_shader", "assets/shaders/2dshader_simplified/2DShader_vert.glsl", "", "", "", "assets/shaders/2dshader_simplified/2DShader_frag.glsl");

	ref<shader> non_curv_tess_shader = engine::gl_shader::create("non_curved_tesselation_shader", "assets/shaders/2dshader_non_curved_tesselation/2DShader_vert.glsl", "",
		"assets/shaders/2dshader_non_curved_tesselation/2DShader_tesc.glsl", "assets/shaders/2dshader_non_curved_tesselation/2DShader_tese.glsl", "assets/shaders/2dshader_non_curved_tesselation/2DShader_frag.glsl");


	renderer::shaders_library()->add(curv_shader_hyperbolic);
	renderer::shaders_library()->add(curv_shader_spherical);
	renderer::shaders_library()->add(curv_shader_hyperbolic_lines);
	renderer::shaders_library()->add(curv_shader_spherical_lines);
	renderer::shaders_library()->add(curv_equation_shader_hyperbolic);
	renderer::shaders_library()->add(curv_equation_shader_spherical);
	renderer::shaders_library()->add(curv_equation_shader_spherical_great_circle_nav);
	renderer::shaders_library()->add(non_curv_shader);
	renderer::shaders_library()->add(non_curv_tess_shader);
}

void engine::renderer::resize(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
    render_command::resize_viewport(x,y,width, height);
}

void engine::renderer::begin_scene(const camera& camera, const ref<shader>& shader)
{
    s_scene_data->view_projection_matrix = camera.view_projection_matrix();
    s_scene_data->shader = shader;
    shader->bind();
    std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("u_view_projection", s_scene_data->view_projection_matrix);
}

void engine::renderer::end_scene()
{
    s_scene_data->shader->unbind(); 
}

void engine::renderer::submit(
    const ref<shader>& shader, 
    const ref<vertex_array>& vertex_array, 
    const glm::mat4& transform /*= glm::mat4(1.f)*/,
	const bool& lines /* = false*/,
	const int& patches /* = 0*/)
{
    std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("u_transform", transform);
	vertex_array->bind();
	
	if(lines)
		render_command::submit_lines(vertex_array);
	else
	{
		if(patches == 2)
			render_command::submit_patches_two(vertex_array);
		else if (patches == 3)
			render_command::submit_patches(vertex_array);
		else
			render_command::submit(vertex_array);
	}
		
}

void engine::renderer::submit(
    const ref<shader>& shader, 
	const ref<game_object>& object,
	const glm::mat4& transform /*= glm::mat4(1.f)*/,
	const bool& lines /* = false*/,
	const bool& patches /* = false*/)
{
	glm::mat4 object_transform = transform;
	if(transform == glm::mat4(1.0f))
		object->transform(object_transform);

	if (object->shape() != NULL) {
		if (object->texture() != NULL && !lines)
		{
			object->bind_texture();
			submit(shader, object->shape()->mesh()->va(), object_transform);
		}
		else
		{
			std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("colouring_on", true);
			std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("in_colour", object->colour());
			submit(shader, object->shape()->mesh()->va(), object_transform, lines, 3);
			std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("colouring_on", false);
		}
	}
}

void engine::renderer::submit_non_curved_tess(
	const ref<shader>& shader,
	const ref<game_object>& object,
	const int tessellation)
{
	glm::mat4 object_transform(1.0f);
	object->transform(object_transform);

	if (object->shape() != NULL) {
		if (object->texture() != NULL)
			object->bind_texture();

		float strips = 30.f;
		float tesselation = 30.f;
		//std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("num_strips", strips);
		std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("num_segments", tessellation);
		std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("in_object_position", glm::vec3(object->position().r, object->position().theta, 0.0f));
		std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("in_object_rotation", object->rotation());

		object_transform = glm::translate(object_transform, glm::vec3(0.f, 0.f, -0.05f));

		submit(shader, object->shape()->mesh()->va(), object_transform, false, 2);
	}
}

void engine::renderer::submit_curved_lines(
	const ref<shader>& shader,
	const ref<game_object>& object,
	const int tessellation,
	const float K,
	const glm::mat4& transform /*= glm::mat4(1.f)*/)
{
	glm::mat4 object_transform = transform;
	if (transform == glm::mat4(1.0f) && K <= 0.01f && K >= -0.01f)
		object->transform(object_transform);

	if (object->shape() != NULL) {

		if (object->texture() != NULL)
			object->bind_texture();

		std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("in_colour", object->colour());

		int strips = 1;
		int tesselation = 30;
		std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("num_strips", strips);
		std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("num_segments", tesselation);

		int curv;
		float k;

		if (K > 0.01) {
			curv = 1;
			k = 160.0f / sqrt(K);
		}
		else if (K < -0.01) {
			curv = -1;
			k = 160.0f / sqrt(-1.0f * K);
		}
		else {
			curv = 0;
			k = 0.0f;
		}

		std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("in_k", k);
		std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("in_object_position", glm::vec3(object->position().r, object->position().theta, 0.0f));
		std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("in_object_rotation", object->rotation());

		object_transform = glm::translate(object_transform, glm::vec3(0.f, 0.f, -0.05f));

		submit(shader, object->shape()->mesh()->va(), object_transform, false, 2);
	}
}

void engine::renderer::submit_curved_texture(
	const ref<shader>& shader,
	const ref<game_object>& object,
	const int tessellation,
	const float K,
	const glm::mat4& transform /*= glm::mat4(1.f)*/)
{
	glm::mat4 object_transform = transform;
	if (transform == glm::mat4(1.0f) && K <= 0.01f && K >= -0.01f)
		object->transform(object_transform);

	if (object->shape() != NULL) {

		if (object->texture() != NULL)
			object->bind_texture();

		std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("in_colour", object->colour());

		int strips = 30;
		int tesselation = 30;
		std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("num_strips", strips);
		std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("num_segments", tesselation);

		int curv;
		float k;

		if (K > 0.01) {
			curv = 1;
			k = 160.0f / sqrt(K);
		}
		else if (K < -0.01) {
			curv = -1;
			k = 160.0f / sqrt(-1.0f * K);
		}
		else {
			curv = 0;
			k = 0.0f;
		}

		std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("in_k", k);
		std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("in_object_position", glm::vec3(object->position().r, object->position().theta, 0.0f));
		std::dynamic_pointer_cast<engine::gl_shader>(shader)->set_uniform("in_object_rotation", object->rotation());

		object_transform = glm::translate(object_transform, glm::vec3(0.f, 0.f, -0.05f));

		submit(shader, object->shape()->mesh()->va(), object_transform, false, 2);
	}
}
