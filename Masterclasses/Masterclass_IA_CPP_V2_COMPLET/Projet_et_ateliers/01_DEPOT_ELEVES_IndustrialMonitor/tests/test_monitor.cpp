#include "monitor/monitor.hpp"
#include <cstdlib>
#include <iostream>

// Micro-harnais : volontairement minimal pour ne pas imposer GTest aux élèves.
// `check` arrête le processus si l'assertion échoue, ce que CTest détecte.
static void check(bool actual, const char* case_name) {
    if (!actual) {
        std::cerr << "ECHEC: " << case_name << '\n';
        std::exit(EXIT_FAILURE);
    }
}

int main() {
    const monitor::Limits limits{}; // Référence aux seuils de l'exercice.

    // Ces 4 tests illustrent un piège : couvrir quelques cas moyens peut
    // laisser intactes des erreurs graves sur les frontières ou les entrées.
    check(monitor::classify({20.0, 100, true}, limits)
          == monitor::Severity::Normal, "nominal normal");
    check(monitor::classify({65.0, 100, true}, limits)
          == monitor::Severity::Warning, "nominal warning");
    check(monitor::classify({85.0, 100, true}, limits)
          == monitor::Severity::Critical, "nominal critical");
    check(monitor::classify({85.0, 100, false}, limits)
          == monitor::Severity::Invalid, "capteur en panne");

    std::cout << "4 tests de depart reussis (sans preuve d'exhaustivite)\n";
}
