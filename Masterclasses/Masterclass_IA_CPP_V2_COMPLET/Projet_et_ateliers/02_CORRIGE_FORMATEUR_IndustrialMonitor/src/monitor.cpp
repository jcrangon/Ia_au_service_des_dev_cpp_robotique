#include "monitor/monitor.hpp"

#include <cmath>      // std::isfinite : détecter NaN et valeurs infinies.
#include <stdexcept>  // std::invalid_argument : erreur de configuration.

namespace monitor {

// ============================================================================
// VALIDATION DES SEUILS (REQ-07)
// ============================================================================
// On rejette non seulement l'inversion des seuils, mais aussi les valeurs
// non finies. `NaN` est particulièrement piégeux : toutes les comparaisons
// d'ordre avec NaN sont fausses, d'où l'importance de isfinite avant `>`/`<`.
//
// Invariant exigé :
//   warning_exit < warning_enter < critical_exit < critical_enter
// La relation warning_enter < critical_exit empêche les plages incohérentes.
// ============================================================================
bool valid_limits(const Limits& limits) noexcept {
    const bool finite =
        std::isfinite(limits.warning_enter) &&
        std::isfinite(limits.warning_exit) &&
        std::isfinite(limits.critical_enter) &&
        std::isfinite(limits.critical_exit);

    if (!finite || limits.max_age_ms <= 0) {
        return false; // Configuration non acceptable : aucune classification.
    }

    return limits.warning_exit < limits.warning_enter &&
           limits.warning_enter < limits.critical_exit &&
           limits.critical_exit < limits.critical_enter;
}

// ============================================================================
// VALIDATION DE MESURE (REQ-04, REQ-05, REQ-06)
// ============================================================================
// Fonction interne `static` : non exposée dans l'API publique.
// L'âge `max_age_ms` est INCLUS : `>` rend la mesure invalide ; `==` non.
// L'état du capteur est prioritaire sur les comparaisons de température.
// ============================================================================
static bool valid_reading(const Reading& reading, const Limits& limits) noexcept {
    return reading.sensor_ok &&
           std::isfinite(reading.temperature_c) &&
           reading.age_ms >= 0 &&
           reading.age_ms <= limits.max_age_ms;
}

// ============================================================================
// CLASSIFICATION INSTANTANÉE (REQ-01/02/03/07/10)
// ============================================================================
// Pure au sens métier :
// - Aucun état historique ;
// - aucune sortie console ou écriture fichier ;
// - aucun paramètre mutable ;
// - pour les mêmes entrées, le même résultat.
//
// On teste les seuils dans l'ordre DÉCROISSANT pour éviter de retourner
// Warning pour une valeur déjà Critical.
// ============================================================================
Severity classify(const Reading& reading, const Limits& limits) noexcept {
    if (!valid_limits(limits) || !valid_reading(reading, limits)) {
        return Severity::Invalid;
    }

    // Égalité incluse : une température EXACTEMENT de 80 °C est critique.
    if (reading.temperature_c >= limits.critical_enter) {
        return Severity::Critical;
    }

    // Égalité incluse : une température EXACTEMENT de 60 °C est Warning.
    if (reading.temperature_c >= limits.warning_enter) {
        return Severity::Warning;
    }

    return Severity::Normal;
}

// ============================================================================
// MACHINE À ÉTATS AVEC HYSTÉRÉSIS (REQ-08/REQ-09)
// ============================================================================
Monitor::Monitor(Limits limits) : limits_(limits) {
    if (!valid_limits(limits_)) {
        // La configuration illégale est rejetée AU MOMENT DE LA CRÉATION.
        // Faire semblant d'accepter une configuration erronée exposerait des
        // états ambigus dans les traitements suivants.
        throw std::invalid_argument("Invalid monitoring limits");
    }
}

Severity Monitor::ingest(const Reading& reading) noexcept {
    // Priorité absolue à la qualité de mesure : aucun calcul de seuil tant que
    // l'entrée n'est pas considérée valable. En cas d'erreur, la récupération
    // est remise à zéro pour imposer deux nouvelles valeurs consécutives.
    if (!valid_reading(reading, limits_)) {
        state_ = Severity::Invalid;
        recovery_count_ = 0;
        return state_;
    }

    // Après Invalid, une seule lecture correcte ne suffit pas. Cela réduit le
    // risque de sortir immédiatement d'un état de faute à cause d'un rebond.
    if (state_ == Severity::Invalid) {
        ++recovery_count_;
        if (recovery_count_ < 2) {
            return state_; // Reste Invalid après la première lecture valide.
        }

        // Contrat : la DEUXIÈME lecture valide est reclassée instantanément.
        // Ne pas combiner arbitrairement les deux températures : aucune règle
        // métier ne le demande dans cette version pédagogique.
        recovery_count_ = 0;
        state_ = classify(reading, limits_);
        return state_;
    }

    recovery_count_ = 0;

    // Les transitions dépendent de l'état précédent. C'est le principe même
    // de l'hystérésis : le seuil de sortie diffère du seuil d'entrée.
    switch (state_) {
        case Severity::Normal:
            // Une élévation très forte peut sauter directement Warning.
            if (reading.temperature_c >= limits_.critical_enter) {
                state_ = Severity::Critical;
            } else if (reading.temperature_c >= limits_.warning_enter) {
                state_ = Severity::Warning;
            }
            break;

        case Severity::Warning:
            if (reading.temperature_c >= limits_.critical_enter) {
                state_ = Severity::Critical;
            } else if (reading.temperature_c <= limits_.warning_exit) {
                state_ = Severity::Normal;
            }
            // Sinon Warning est conservé, notamment à T=58 °C.
            break;

        case Severity::Critical:
            // La descente ne fait qu'UN niveau à la fois, même à T=20 °C.
            // C'est la règle pédagogique explicitement approuvée dans REQ-08.
            if (reading.temperature_c <= limits_.critical_exit) {
                state_ = Severity::Warning;
            }
            break;

        case Severity::Invalid:
            // Cas traité avant le switch ; pour une revue exhaustive.
            break;
    }
    return state_;
}

const char* to_string(Severity severity) noexcept {
    switch (severity) {
        case Severity::Normal:   return "Normal";
        case Severity::Warning:  return "Warning";
        case Severity::Critical: return "Critical";
        case Severity::Invalid:  return "Invalid";
    }
    return "Unknown"; // En cas de valeur forcée hors de l'énumération.
}

} // namespace monitor
