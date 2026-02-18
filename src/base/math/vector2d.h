#pragma once

class Vec2
{
public:
    // Constructors
    constexpr Vec2() = default;
    constexpr Vec2(const int16_t x, const int16_t y) : x(x), y(y) {}

    // Methods
    [[nodiscard]] inline bool is_valid() const
    {
        return x != -1 || y != -1;
    }

    // Operators
    [[nodiscard]] inline Vec2 operator+(const Vec2 &rhs) const
    {
        return Vec2(this->x + rhs.x, this->y + rhs.y);
    }

    [[nodiscard]] inline Vec2 operator-(const Vec2 &rhs) const
    {
        return Vec2(this->x - rhs.x, this->y - rhs.y);
    }

    [[nodiscard]] inline Vec2 operator/(const int16_t& rhs) const
    {
        return Vec2(this->x / rhs, this->y / rhs);
    }

    int16_t x = -1;
    int16_t y = -1;
};
