#pragma once

#include "structs/dht.h"
#include "structs/display.h"
#include "structs/button.h"
#include "structs/buzzler.h"
#include "structs/rtc.h"

inline auto display_module = DisplayModule({128, 64}, -1);
inline auto dht_module = DHTModule(15u, DHT22);
inline auto buzzler_module = BuzzlerModule(25u, 0u);
inline auto button1 = ButtonModule(23u);
inline auto rtc_module = RTCModule();