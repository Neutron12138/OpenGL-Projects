#include <iostream>
#include "font.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#undef STB_IMAGE_WRITE_IMPLEMENTATION

int main()
{
    Font font = load_font_from_file("simkai.ttf");
    std::cout << "load status: " << (font.get_font().data ? "success" : "failed") << std::endl;

    float scale = font.get_scale_for_pixel_height(1000.0f);
    std::cout << "scale: " << scale << std::endl;

    Glyph glyph = font.get_codepoint_bitmap({scale, scale}, L'好');
    std::cout << "render bitmap: " << (glyph.get_pixels() ? "success" : "failed") << std::endl;

    auto size = glyph.get_size();
    std::cout << "size: " << size.x << ", " << size.y << std::endl;
    auto offset = glyph.get_offset();
    std::cout << "offset: " << offset.x << ", " << offset.y << std::endl;

    glm::vec4 color = {0.498039f, 1.0f, 0.831373f, 1.0f};
    std::vector<base::Byte> pixels(size.x * size.y * 4);
    for (std::size_t i = 0; i < size.x * size.y; i++)
    {
        pixels[i * 4 + 0] = color.r * 255;
        pixels[i * 4 + 1] = color.g * 255;
        pixels[i * 4 + 2] = color.b * 255;
        pixels[i * 4 + 3] = color.a * glyph.get_pixels()[i];
    }

    int success = stbi_write_png("好.png", size.x, size.y, 4, pixels.data(), 0);
    std::cout << "save bitmap: " << (success ? "success" : "failed") << std::endl;

    return 0;
}