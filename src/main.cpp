#include "include.h"
/*
  RAM:   [=         ]   6.8% (used 22144 bytes from 327680 bytes)
  Flash: [==        ]  24.0% (used 314225 bytes from 1310720 bytes)

  RAM:   [=         ]   6.7% (used 22096 bytes from 327680 bytes)
  Flash: [==        ]  24.0% (used 315013 bytes from 1310720 bytes)
*/

auto heap_size = 0u;

void sensor_task(void *);
void render_task(void *);
void button_task(void *);

//
//  Main
//
void setup()
{
  const auto start_time = esp_log_timestamp();
  Serial.begin(115200ul); // ESP32 default
  Serial.println("Init");

  if (!Wire.setPins(21, 22))
  {
    Serial.println("Wire set pins error");
    abort();
  }

  if (!Wire.begin(21, 22))
  {
    Serial.println("Wire begin error");
    abort();
  }

  if (!rtc_module.setup())
  {
    Serial.println("RTC not found");
    abort();
  }

  if (!rtc_module.is_set())
  {
    rtc_module.set_date(RTCDateTime()); // CT date
    Serial.println("RTC: update time");
  }

  button1.setup();
  buzzer_module.setup();
  dht_module.setup();

  display_module = new DisplayModule({128, 64}, -1);
  if (!display_module->begin())
  {
    Serial.println("SSD1306 allocation failed");
    abort();
  }
  heap_size = ESP.getHeapSize();

  display_module->clear();
  char buffer[64] = {};

  const auto time_load = esp_log_timestamp();
  sprintf(buffer, "Init time: %u ms\n\nHEAP: %u KB", time_load - start_time, heap_size / 1024u);
  display_module->draw_text({0, 0}, buffer, 1U);
  display_module->end();

  delay(2000u);

  xTaskCreate(sensor_task, "SENSOR_TASK", 2048u, nullptr, 1u, nullptr);
  xTaskCreate(render_task, "RENDER_TASK", 8192u, nullptr, 1u, nullptr);
  xTaskCreate(button_task, "BUTTON_TASK", 1024u, nullptr, 2u, nullptr);

  Serial.println("Init complete");
}

void loop()
{
  vTaskDelay(pdMS_TO_TICKS(1000));
}

//
//  Tasks
//

// Sensor
void sensor_task(void *)
{
  while (1)
  {
    const auto now_ms = esp_log_timestamp();
    dht_module.update();

    Serial.printf("dht update: %u ms\r\n", esp_log_timestamp() - now_ms);

    vTaskDelay(pdMS_TO_TICKS(5000u));
  }
}

// Render
void render_task(void *)
{
  while (1)
  {
    display_module->clear();

    const auto heap_pcent = (1.f - static_cast<float>(ESP.getFreeHeap()) / static_cast<float>(heap_size)) * 100.f;
    const auto tm = rtc_module.now();
    const auto &seconds = tm.ss;

    char str_temp[16] = {};
    char str_hum[16] = {};
    char str_heap[32] = {};
    char str_time[16] = {};
    char str_btn_press[32] = {};

    sprintf(str_temp, "%.1fC", dht_module.temperature.fast_get());
    sprintf(str_hum, "%.0f%%", dht_module.humidity.fast_get());
    sprintf(str_heap, "HEAP USED: %.0f%%", heap_pcent);
    sprintf(str_time, "%02d%s%02d", tm.hh, seconds % 2 == 0 ? ":" : " ", tm.mm);

    const auto sz_temp = display_module->get_text_size(str_temp, 1U);
    const auto sz_hum = display_module->get_text_size(str_hum, 1U);
    const auto sz_heap_sz_str = display_module->get_text_size(str_heap, 1U);
    const auto sz_time = display_module->get_text_size(str_time, 2U);

    display_module->draw_text(Vec2(display_module->size.x, 0) - Vec2(sz_temp.x, 0), str_temp);
    display_module->draw_text(Vec2(display_module->size.x, sz_temp.y + 2) - Vec2(sz_hum.x, 0), str_hum);
    display_module->draw_text(display_module->size - sz_heap_sz_str, str_heap, 1U, false);

    const auto pos_time = display_module->size / 2U - sz_time / 2U;
    display_module->draw_text(pos_time, str_time, 2U);

    const auto pos_seconds = pos_time + Vec2(0, sz_time.y + 2);
    display_module->draw_rect(pos_seconds, {static_cast<int16_t>((sz_time.x * seconds) / 60), 2});

    display_module->end();

    vTaskDelay(pdMS_TO_TICKS(60u));
  }
}

// Button
void button_task(void *)
{
  while (1)
  {
    bool pressed;

    button1.update();
    pressed = button1.is_pressed(1000u);

    if (pressed)
    {
      buzzer_module.play_tone();
    }
    else
    {
      buzzer_module.stop_tone();
    }

    vTaskDelay(pdMS_TO_TICKS(50u));
  }
}
