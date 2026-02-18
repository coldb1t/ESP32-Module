#pragma once

class ButtonModule
{
public:
    explicit ButtonModule(const uint8_t pin) : pin(pin) {}

    void setup()
    {
        pinMode(pin, INPUT_PULLUP);
        last_press_time.init();
    }

    inline void update()
    {
        const bool is_held = (digitalRead(pin) == LOW);

        if (!is_held)
        {
            was_pressed = false;
            last_press_time.set(0u);
            return;
        }

        if (last_press_time.fast_get() == 0u)
        {
            last_press_time.set(esp_log_timestamp());
        }
    }

    [[nodiscard]] inline bool is_pressed(const uint32_t press_time = 200u) const
    {
        const auto t0 = last_press_time.fast_get();
        if (t0 == 0u)
        {
            return false;
        }

        return (esp_log_timestamp() - t0) >= press_time;
    }

    [[nodiscard]] inline bool is_clicked()
    {
        const auto result = (last_press_time.fast_get() != 0u) && !was_pressed;
        if (result)
        {
            was_pressed = true;
        }
        return result;
    }

private:
    uint8_t pin = 0u;
    bool was_pressed = false;
    core::locked<uint32_t> last_press_time{0u};
};
