#include "monitor/monitor.hpp"

namespace monitor {

// ============================================================================
// VERSION INITIALE — volontairement non conforme à plusieurs exigences.
// ============================================================================
// Ce fichier représente un héritage logiciel réaliste : il compile et passe
// certains tests sans pour autant respecter le cahier des charges.
//
// CONSIGNE AUX ÉLÈVES :
// 1. Ne commencez pas par modifier la fonction.
// 2. Prouvez d'abord les écarts par des cas minimaux et reproductibles.
// 3. Documentez pour chaque défaut l'exigence violée et sa conséquence.
// 4. Implémentez seulement après validation du plan d'architecture.
// ============================================================================
Severity classify(const Reading& reading, const Limits& limits) noexcept {
    // Cette garde ne vérifie qu'une petite partie du contrat de lecture.
    if (!reading.sensor_ok) {
        return Severity::Invalid;
    }

    // Seuil métier fourni par la configuration.
    if (reading.temperature_c > limits.critical_enter) {
        return Severity::Critical;
    }
    if (reading.temperature_c > limits.warning_enter) {
        return Severity::Warning;
    }
    return Severity::Normal;
}

const char* to_string(Severity severity) noexcept {
    switch (severity) {
        case Severity::Normal:   return "Normal";
        case Severity::Warning:  return "Warning";
        case Severity::Critical: return "Critical";
        case Severity::Invalid:  return "Invalid";
    }
    // Défense minimale au cas où une valeur de l'énumération serait forcée.
    return "Unknown";
}

} // namespace monitor
