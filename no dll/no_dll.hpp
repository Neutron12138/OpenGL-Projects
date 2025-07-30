#pragma once

#define GLFW_WRAPPER_INCLUDE_IMPL
#include <glfw_wrapper/glfw_wrapper.hpp>
#define GL_WRAPPER_INCLUDE_IMPL
#define GL_WRAPPER_INCLUDE_GLAD_IMPL
#include <gl_wrapper/gl_wrapper.hpp>

class NoDLL : public glfw_wrapper::MainLoop
{
protected:
    void _initialize() override
    {
        gl_wrapper::GLLoader();
        glViewport(0, 0, 1152, 648);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    }

    void _draw() override
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }
};
