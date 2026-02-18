#pragma once

class ButtonModule
{
public:
    explicit constexpr ButtonModule(const uint8_t pin) : pin(pin) {}

    void begin() const
    {
        pinMode(pin, INPUT_PULLUP);
    }

    inline void update()
    {
        const auto is_pressed = digitalRead(pin) == LOW;

        if (!is_pressed)
        {
            last_press_time = 0u;
            return;
        }

        if (last_press_time == 0u)
        {
            last_press_time = esp_log_timestamp();
        }
    }

    [[nodiscard]] inline bool is_pressed(const uint32_t press_time = 200u) const
    {
        if (last_press_time == 0u)
        {
            return false;
        }

        return (esp_log_timestamp() - last_press_time) >= press_time;
    }

    uint32_t last_press_time = 0u;

private:
    uint8_t pin = 0u;
};
