#pragma once

#include "glyph.hpp"

class Font : public base::Resource
{
private:
    stbtt_fontinfo m_font = {};
    std::vector<base::Byte> m_buffer = {};

public:
    Font() = default;
    Font(Font &&from)
        : m_font(std::exchange(from.m_font, {})),
          m_buffer(std::exchange(from.m_buffer, {})) {}
    ~Font() override { destroy(); }
    BASE_DELETE_COPY_FUNCTION(Font);

public:
    Font &operator=(Font &&from)
    {
        destroy();
        m_font = std::exchange(from.m_font, {});
        m_buffer = std::exchange(from.m_buffer, {});
        return *this;
    }

    base::Int64 get_resource_type() const override { return 1; }
    bool is_valid() const override { return m_font.data; }
    const stbtt_fontinfo &get_font() const { return m_font; }
    operator const stbtt_fontinfo *() const { return &m_font; }

public:
    void create(const std::vector<base::Byte> &buffer, base::Size offset = 0)
    {
        destroy();
        m_buffer = buffer;
        if (!stbtt_InitFont(&m_font, m_buffer.data(), offset))
            throw BASE_MAKE_CLASS_RUNTIME_ERROR("Failed to load font from buffer");
    }

    void destroy()
    {
        m_font = {};
        m_buffer.clear();
    }

public:
    float get_scale_for_pixel_height(float height) const { return stbtt_ScaleForPixelHeight(&m_font, height); }
    Glyph get_codepoint_bitmap(const glm::vec2 &scale, wchar_t codepoint) const
    {
        glm::ivec2 size, offset;
        base::Byte *pixels = stbtt_GetCodepointBitmap(
            &m_font, scale.x, scale.y, static_cast<int>(codepoint),
            &size.x, &size.y, &offset.x, &offset.y);
        if (!pixels)
            throw BASE_MAKE_CLASS_RUNTIME_ERROR(
                "Failed to render codepoint bitmap, codepoint: ", static_cast<base::Int32>(codepoint));

        Glyph glyph;
        glyph.create(pixels, size, offset);
        return glyph;
    }
};

Font load_font_from_file(const std::filesystem::path &filename)
{
    auto buffer = base::read_bytes_from_file(filename);
    Font font;
    font.create(buffer);
    return font;
}
