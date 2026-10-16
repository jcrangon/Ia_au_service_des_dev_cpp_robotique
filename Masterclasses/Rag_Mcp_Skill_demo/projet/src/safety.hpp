#pragma once
#include <cstdint>
struct Sensor { double distance_m; std::int64_t age_ms; bool healthy; };
bool can_move(const Sensor& s, double speed_mps);
