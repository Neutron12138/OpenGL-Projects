#pragma once

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>
#undef STB_TRUETYPE_IMPLEMENTATION
#include <glm/glm.hpp>
#include <base/misc/resource.hpp>
#include <base/misc/read_from_file.hpp>

class Glyph : public base::Resource
{
private:
    std::unique_ptr<base::Byte> m_pixels = {};
    glm::ivec2 m_size = {};
    glm::ivec2 m_offset = {};

public:
    Glyph() = default;
    Glyph(Glyph &&from)
        : m_pixels(std::move(from.m_pixels)),
          m_size(std::exchange(from.m_size, {})),
          m_offset(std::exchange(from.m_offset, {})) {}
    ~Glyph() override { destroy(); }
    BASE_DELETE_COPY_FUNCTION(Glyph);

public:
    Glyph &operator=(Glyph &&from)
    {
        destroy();
        m_pixels = std::move(from.m_pixels);
        m_size = std::exchange(from.m_size, {});
        m_offset = std::exchange(from.m_offset, {});
        return *this;
    }

    base::Int64 get_resource_type() const override { return 0; }
    bool is_valid() const override { return m_pixels.get(); }
    base::Byte *get_pixels() const { return m_pixels.get(); }
    const glm::ivec2 &get_size() const { return m_size; }
    const glm::ivec2 &get_offset() const { return m_offset; }

public:
    void create(base::Byte *pixels, const glm::ivec2 &size, const glm::ivec2 &offset)
    {
        destroy();
        m_pixels.reset(pixels);
        m_size = size;
        m_offset = offset;
    }

    void destroy()
    {
        stbtt_FreeBitmap(m_pixels.release(), nullptr);
        m_size = {};
        m_offset = {};
    }
};
