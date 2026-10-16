#include "monitor/monitor.hpp"

#include <cmath>       // std::nextafter, std::isfinite.
#include <cstdlib>     // EXIT_FAILURE.
#include <iostream>    // Diagnostic lisible.
#include <limits>      // Infini, NaN, bornes représentables.
#include <stdexcept>   // std::invalid_argument.

// ============================================================================
// TESTS DE RÉFÉRENCE : n'utilisent aucune bibliothèque externe.
// ============================================================================
// Un test échoué provoque un code de retour non nul, détecté par CTest.
// Le compteur indique exactement le nombre d'assertions réellement exécutées.
// Ce test est une preuve PARTIELLE, jamais une vérification exhaustive.
// ============================================================================
static int checks = 0;

static void check(bool condition, const char* label) {
    ++checks;
    if (!condition) {
        std::cerr << "ECHEC [" << checks << "]: " << label << '\n';
        std::exit(EXIT_FAILURE);
    }
}

int main() {
    using monitor::Severity;
    const monitor::Limits limits{};

    // ---- Partition nominale : REQ-01 à REQ-03.
    check(monitor::classify({20.0, 100, true}, limits) == Severity::Normal,
          "20 Normal");
    check(monitor::classify({65.0, 100, true}, limits) == Severity::Warning,
          "65 Warning");
    check(monitor::classify({85.0, 100, true}, limits) == Severity::Critical,
          "85 Critical");

    // ---- Frontières exactes : les comparateurs > et >= ne sont pas égaux !
    check(monitor::classify({60.0, 0, true}, limits) == Severity::Warning,
          "60.0 est Warning");
    check(monitor::classify({80.0, 0, true}, limits) == Severity::Critical,
          "80.0 est Critical");

    const double below60 = std::nextafter(60.0, -INFINITY);
    const double above80 = std::nextafter(80.0, INFINITY);
    const double below80 = std::nextafter(80.0, -INFINITY);
    check(monitor::classify({below60, 0, true}, limits) == Severity::Normal,
          "voisin inferieur de 60");
    check(monitor::classify({below80, 0, true}, limits) == Severity::Warning,
          "voisin inferieur de 80");
    check(monitor::classify({above80, 0, true}, limits) == Severity::Critical,
          "voisin superieur de 80");

    // ---- REQ-04 : valeurs non finies rejetées avant comparaisons.
    check(monitor::classify({NAN, 10, true}, limits) == Severity::Invalid,
          "NaN invalide");
    check(monitor::classify({INFINITY, 10, true}, limits) == Severity::Invalid,
          "infini positif invalide");
    check(monitor::classify({-INFINITY, 10, true}, limits) == Severity::Invalid,
          "infini negatif invalide");

    // ---- REQ-05/06 : intégrité et fraîcheur de la lecture.
    check(monitor::classify({85.0, 10, false}, limits) == Severity::Invalid,
          "capteur invalide");
    check(monitor::classify({65.0, -1, true}, limits) == Severity::Invalid,
          "age negatif");
    check(monitor::classify({65.0, 1000, true}, limits) == Severity::Warning,
          "age maximum inclus");
    check(monitor::classify({65.0, 1001, true}, limits) == Severity::Invalid,
          "age depasse");

    // ---- REQ-07 : cohérence, limites non finies et constructeur.
    auto bad = limits;
    bad.warning_enter = 85.0; // Devient supérieur à critical_exit.
    check(!monitor::valid_limits(bad), "limites mal ordonnees");
    check(monitor::classify({65.0, 0, true}, bad) == Severity::Invalid,
          "classify rejette limites invalides");

    bool rejected = false;
    try {
        monitor::Monitor m(bad);
        (void)m;
    } catch (const std::invalid_argument&) {
        rejected = true;
    }
    check(rejected, "constructeur rejette configuration");

    bad = limits;
    bad.critical_enter = NAN;
    check(!monitor::valid_limits(bad), "seuil NaN invalide");

    bad = limits;
    bad.max_age_ms = 0;
    check(!monitor::valid_limits(bad), "age maximum doit etre positif");

    // ---- REQ-08 : chemin d'ascension et descente en plusieurs étapes.
    monitor::Monitor machine(limits);
    check(machine.state() == Severity::Normal, "etat initial pedagogique");
    check(machine.ingest({60.0, 0, true}) == Severity::Warning,
          "entree Warning");
    check(machine.ingest({58.0, 0, true}) == Severity::Warning,
          "hysteresis conserve Warning");
    check(machine.ingest({55.0, 0, true}) == Severity::Normal,
          "sortie Warning a 55");
    check(machine.ingest({80.0, 0, true}) == Severity::Critical,
          "entree directe Critical");
    check(machine.ingest({20.0, 0, true}) == Severity::Warning,
          "Critical vers Warning en un pas");
    check(machine.ingest({20.0, 0, true}) == Severity::Normal,
          "Warning vers Normal en deuxieme pas");

    // ---- REQ-09 : séquences de récupération, y compris remise à zéro.
    check(machine.ingest({NAN, 0, true}) == Severity::Invalid,
          "premiere panne");
    check(machine.ingest({20.0, 0, true}) == Severity::Invalid,
          "premiere mesure valide reste Invalid");
    check(machine.ingest({85.0, 0, false}) == Severity::Invalid,
          "nouvelle panne remet compteur a zero");
    check(machine.ingest({20.0, 0, true}) == Severity::Invalid,
          "nouvelle premiere mesure valide");
    check(machine.ingest({65.0, 0, true}) == Severity::Warning,
          "deuxieme mesure reclassifie a 65");

    // ---- REQ-10 : même entrée = même résultat instantané.
    const auto first = monitor::classify({80.0, 50, true}, limits);
    const auto second = monitor::classify({80.0, 50, true}, limits);
    check(first == second, "classification deterministe (cas teste)");

    std::cout << checks << " assertions de reference executees avec succes\n";
}
