#pragma once

class BuzzlerModule
{
public:
  explicit constexpr BuzzlerModule(const uint8_t pin, const uint8_t channel = 0u) : pin(pin), channel(channel)
  {
  }

  // 8-bit resolution: 0..255
  inline void begin(const uint32_t freq_hz = 2000u, const uint8_t resolution_bits = 8u) const
  {
    ledcSetup(channel, freq_hz, resolution_bits);
    ledcAttachPin(pin, channel);
    ledcWrite(channel, 0);
  }

  inline void play_tone(const uint32_t duty = 128u) const
  {
    ledcWrite(channel, duty);
  }

  inline void stop_tone() const
  {
    ledcWrite(channel, 0u);
  }

private:
  uint8_t pin = 0u;
  uint8_t channel = 0u;
};