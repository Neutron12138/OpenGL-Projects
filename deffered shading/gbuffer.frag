#version 450 core

in vec3 v_pos;
in vec2 v_uv;

layout(location=0)out vec3 o_pos;
layout(location=1)out vec2 o_uv;

void main()
{
    o_pos = v_pos;
    o_uv = v_uv;
}
