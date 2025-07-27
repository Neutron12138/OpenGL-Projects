#pragma once

#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glfw_wrapper/glfw_wrapper.hpp>
#include <gl_wrapper/gl_wrapper.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

class DeferredShading : public glfw_wrapper::MainLoop
{
public:
    static constexpr std::array<gl_wrapper::Vertex3DTex, 36> CONTAINER_VERTICES = {
        gl_wrapper::Vertex3DTex{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}},
        {{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},

        {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, 0.5f}, {1.0f, 1.0f}},
        {{0.5f, 0.5f, 0.5f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f}},
        {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}},

        {{-0.5f, 0.5f, 0.5f}, {1.0f, 0.0f}},
        {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},
        {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}},
        {{-0.5f, 0.5f, 0.5f}, {1.0f, 0.0f}},

        {{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}},
        {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},
        {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},
        {{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}},
        {{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f}},

        {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},
        {{0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}},
        {{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f}},
        {{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f}},
        {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}},

        {{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f}},
        {{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}},
        {{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f}},
        {{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f}},
        {{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f}},
    };

    static constexpr std::array<gl_wrapper::Vertex2DTex, 4> QUAD_VERTICES = {
        gl_wrapper::Vertex2DTex{{-1.0f, 1.0f}, {0.0f, 1.0f}},
        {{-1.0f, -1.0f}, {0.0f, 0.0f}},
        {{1.0f, 1.0f}, {1.0f, 1.0f}},
        {{1.0f, -1.0f}, {1.0f, 0.0f}},
    };

private:
    glm::mat4 projection_matrix;
    glm::mat4 view_matrix;
    gl_wrapper::Program container_program;
    gl_wrapper::Texture2D container_texture;
    glm::mat4 container_model_matrix;
    gl_wrapper::VertexArray container_vao;
    gl_wrapper::Texture2D vertex_pos_texture;
    gl_wrapper::Texture2D vertex_uv_texture;
    gl_wrapper::Framebuffer framebuffer;
    gl_wrapper::Renderbuffer renderbuffer;
    gl_wrapper::Program gbuffer_program;
    gl_wrapper::Program quad_program;
    gl_wrapper::VertexArray quad_vao;

private:
    void _initialize_gl()
    {
        gl_wrapper::GLLoader();
        gl_wrapper::DebugMessageCallback::get_instance();
        glViewport(0, 0, 1152, 648);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glDepthMask(GL_TRUE);
    }

    void _initialize_container()
    {
        container_program = gl_wrapper::load_program_from_file("container.vert", "container.frag");
        container_vao = gl_wrapper::create_vertex_array_from_vertices(CONTAINER_VERTICES);
        container_model_matrix = glm::mat4(1.0f);

        int width, height;
        stbi_uc *pixels = stbi_load("container.jpg", &width, &height, nullptr, 0);
        if (!pixels)
            throw BASE_MAKE_RUNTIME_ERROR("Failed to load image");

        container_texture = gl_wrapper::create_texture_2d_from_pixels(
            gl_wrapper::InternalFormat::RGB8, width, height, gl_wrapper::PixelFormat::RGB, pixels);
        stbi_image_free(pixels);
    }

    void _initialize_framebuffer()
    {
        vertex_pos_texture = gl_wrapper::create_texture_2d(gl_wrapper::InternalFormat::RGB32F, 1152, 648);
        vertex_uv_texture = gl_wrapper::create_texture_2d(gl_wrapper::InternalFormat::RG32F, 1152, 648);
        renderbuffer = gl_wrapper::create_renderbuffer(gl_wrapper::InternalFormat::Depth24Stencil8, 1152, 648);

        framebuffer = gl_wrapper::create_framebuffer(gl_wrapper::Framebuffer::FramebufferType::Default);
        framebuffer.attach_color_texture(vertex_pos_texture, 0);
        framebuffer.attach_color_texture(vertex_uv_texture, 1);
        framebuffer.attach_renderbuffer(gl_wrapper::Framebuffer::Attachment::DepthStencil, renderbuffer);
        auto status = framebuffer.check_status();
        if (status != gl_wrapper::Framebuffer::Status::Complete)
            throw BASE_MAKE_RUNTIME_ERROR("Framebuffer incomplete, status: ", static_cast<GLenum>(status));

        framebuffer.set_draw_buffers({
            gl_wrapper::Framebuffer::DrawBuffer::Color0,
            gl_wrapper::Framebuffer::DrawBuffer::Color1,
        });
    }

    void _initialize_quad()
    {
        quad_program = gl_wrapper::load_program_from_file("quad.vert", "quad.frag");
        quad_vao = gl_wrapper::create_vertex_array_from_vertices(QUAD_VERTICES);
    }

    void _draw_container()
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        container_vao.bind();
        glActiveTexture(GL_TEXTURE0);
        container_texture.bind();
        container_program.use();
        container_program.set_uniform(container_program.get_uniform_location("u_projection"), projection_matrix);
        container_program.set_uniform(container_program.get_uniform_location("u_view"), view_matrix);
        container_program.set_uniform(container_program.get_uniform_location("u_model"), container_model_matrix);
        container_vao.draw_arrays(gl_wrapper::VertexArray::DrawMode::Triangles, 36);
    }

    void _draw_gbuffer()
    {
        framebuffer.bind();
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        container_vao.bind();
        gbuffer_program.use();
        gbuffer_program.set_uniform(gbuffer_program.get_uniform_location("u_projection"), projection_matrix);
        gbuffer_program.set_uniform(gbuffer_program.get_uniform_location("u_view"), view_matrix);
        gbuffer_program.set_uniform(gbuffer_program.get_uniform_location("u_model"), container_model_matrix);
        container_vao.draw_arrays(gl_wrapper::VertexArray::DrawMode::Triangles, 36);
        framebuffer.unbind();
    }

    void _draw_quad()
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        quad_vao.bind();
        quad_program.use();
        glActiveTexture(GL_TEXTURE0);
        vertex_pos_texture.bind();
        glActiveTexture(GL_TEXTURE1);
        vertex_uv_texture.bind();
        glActiveTexture(GL_TEXTURE2);
        container_texture.bind();
        quad_program.set_uniform(quad_program.get_uniform_location("u_pos"), 0);
        quad_program.set_uniform(quad_program.get_uniform_location("u_uv"), 1);
        quad_program.set_uniform(quad_program.get_uniform_location("u_texture"), 2);
        quad_vao.draw_arrays(gl_wrapper::VertexArray::DrawMode::TriangleStrip, 4);
    }

protected:
    void _initialize() override
    {
        projection_matrix = glm::perspective(120.0f, 1152.0f / 648.0f, 0.1f, 100.0f);
        view_matrix = glm::lookAt(glm::vec3{1.0f, 2.0f, 3.0f}, glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.0f, 1.0f, 0.0f});

        _initialize_gl();
        _initialize_container();
        _initialize_framebuffer();
        _initialize_quad();
        gbuffer_program = gl_wrapper::load_program_from_file("gbuffer.vert", "gbuffer.frag");
    }

    void _draw() override
    {
        _draw_gbuffer();
        _draw_quad();
        //_draw_container();
    }

public:
    DeferredShading()
    {
        m_is_opengl_debug_context_enabled = true;
    }
};
