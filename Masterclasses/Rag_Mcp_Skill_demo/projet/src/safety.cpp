#include "safety.hpp"
#include <cmath>
// DELIBERATE TRAINING DEFECT: stale readings not rejected.
bool can_move(const Sensor& s, double speed_mps) {
    return s.healthy && s.distance_m > speed_mps * 2.0;
}
