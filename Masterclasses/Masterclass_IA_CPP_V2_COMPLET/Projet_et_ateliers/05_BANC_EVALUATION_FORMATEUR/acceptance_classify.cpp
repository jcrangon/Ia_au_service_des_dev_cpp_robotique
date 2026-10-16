// ============================================================================
// BANC INDÉPENDANT DE CLASSIFICATION — réservé au formateur.
// ============================================================================
// Ce fichier est compilé EN DEHORS du projet soumis par les agents pour éviter
// qu'une IA façonne sa propre « preuve ». Il fonctionne avec l'API de départ.
// À NE PAS montrer aux modèles avant la première implémentation A/B.
// ============================================================================
#include "monitor/monitor.hpp"
#include <cmath>
#include <iostream>
#include <limits>

int main() {
    using monitor::Severity;
    const monitor::Limits l{};
    int pass = 0, fail = 0;
    auto expect = [&](bool ok, const char* name) {
        if (ok) { ++pass; std::cout << "PASS " << name << '\n'; }
        else { ++fail; std::cerr << "FAIL " << name << '\n'; }
    };

    // Une suite minimale couvre les écarts faciles à oublier.
    expect(monitor::classify({20.0, 0, true}, l) == Severity::Normal,
           "REQ01 nomimal");
    expect(monitor::classify({60.0, 0, true}, l) == Severity::Warning,
           "REQ02 equality");
    expect(monitor::classify({80.0, 0, true}, l) == Severity::Critical,
           "REQ03 equality");
    expect(monitor::classify({std::nextafter(80.0, -INFINITY), 0, true}, l)
           == Severity::Warning, "REQ03 below");
    expect(monitor::classify({NAN, 0, true}, l) == Severity::Invalid,
           "REQ04 NaN");
    expect(monitor::classify({INFINITY, 0, true}, l) == Severity::Invalid,
           "REQ04 infinity");
    expect(monitor::classify({-INFINITY, 0, true}, l) == Severity::Invalid,
           "REQ04 -infinity");
    expect(monitor::classify({85.0, 0, false}, l) == Severity::Invalid,
           "REQ05 bad sensor");
    expect(monitor::classify({65.0, -1, true}, l) == Severity::Invalid,
           "REQ06 negative age");
    expect(monitor::classify({65.0, 1000, true}, l) == Severity::Warning,
           "REQ06 inclusive max age");
    expect(monitor::classify({65.0, 1001, true}, l) == Severity::Invalid,
           "REQ06 stale");
    auto wrong = l;
    wrong.warning_enter = 85.0;
    expect(monitor::classify({65.0, 0, true}, wrong) == Severity::Invalid,
           "REQ07 invalid configuration");

    std::cout << "TOTAL pass=" << pass << " fail=" << fail << '\n';
    return fail == 0 ? 0 : 1; // La valeur de retour est une preuve observable.
}
