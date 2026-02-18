#pragma once

struct Color
{
    inline constexpr Color(const uint16_t value = SSD1306_WHITE) : value(value) {};

    static constexpr uint16_t white = SSD1306_WHITE;
    static constexpr uint16_t black = SSD1306_BLACK;
    static constexpr uint16_t inverse = SSD1306_INVERSE;

    uint16_t value = SSD1306_WHITE;

    inline constexpr operator uint16_t() const
    {
        return value;
    }
};
