#include "safety.hpp"
#include <cassert>
#include <limits>
int main(){
 assert(can_move({4.0, 20, true}, 1.0));
 assert(!can_move({1.0, 20, true}, 1.0));
 assert(!can_move({4.0, 20, false}, 1.0));
 // Target contract, intentionally failing until code is corrected:
 assert(!can_move({4.0, 9999, true}, 1.0));
 assert(!can_move({4.0, 20, true}, std::numeric_limits<double>::quiet_NaN()));
}
