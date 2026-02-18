#pragma once

class DHTModule
{
public:
    explicit DHTModule(uint8_t pin, uint8_t type)
        : pin(pin), type(type), dht(pin, type)
    {
    }

    void begin()
    {
        dht.begin();
    }

    inline void update()
    {
        const auto t = dht.readTemperature();
        const auto h = dht.readHumidity();

        temperature = isnan(t) ? temperature : t;
        humidity = isnan(h) ? humidity : h;
    }

    float temperature = 0.f;
    float humidity = 0.f;

private:
    uint8_t pin = 0u;
    uint8_t type = 0u;
    DHT dht;
};