#pragma once

// ============================================================================
// INDUSTRIALMONITOR — CONTRAT PUBLIC, VERSION DE RÉFÉRENCE.
// ============================================================================
// Le contrat s'exprime volontairement avec peu de types. Il permet de discuter :
//  - de la sémantique des erreurs ;
//  - de la responsabilité de l'état (fonction pure versus objet) ;
//  - des invariants validables et des transitions d'hystérésis.
// Ce code ne conduit pas d'actionneur et ne constitue pas une certification.
// ============================================================================
namespace monitor {

enum class Severity {
    Normal,   // Température valide sous le seuil d'alerte d'entrée.
    Warning,  // Alerte nécessitant une attention, ou maintenue par hystérésis.
    Critical, // Température au seuil critique, ou état maintenu par hystérésis.
    Invalid   // Mesure / paramètres inutilisables : PAS synonyme de sécurité.
};

struct Reading {
    double temperature_c{}; // Température exprimée en degrés Celsius.
    int age_ms{};           // Âge de la mesure, toujours attendu positif / nul.
    bool sensor_ok{true};   // Diagnostic capteur (fourni par le producteur).
};

struct Limits {
    double warning_enter{60.0}; // Au-dessus ou égal : entrée Warning.
    double warning_exit{55.0};  // Au-dessous ou égal : sortie Warning.
    double critical_enter{80.0}; // Au-dessus ou égal : entrée Critical.
    double critical_exit{75.0};  // Au-dessous ou égal : sortie Critical.
    int max_age_ms{1000};       // Âge maximal autorisé, borne incluse.
};

// Évaluer une configuration avant sa mise en service.
// noexcept : les validations ne réalisent aucune allocation / ne lèvent pas.
[[nodiscard]] bool valid_limits(const Limits& limits) noexcept;

// Une classification instantanée n'utilise aucune mémoire antérieure.
// En cas de limites incohérentes, elle retourne Invalid (REQ-07).
[[nodiscard]] Severity classify(
    const Reading& reading,
    const Limits& limits
) noexcept;

// Machine à états : l'état précédent est indispensable pour l'hystérésis.
class Monitor {
public:
    // Le constructeur rejette les paramètres invalides : std::invalid_argument.
    // Il n'est pas noexcept, car l'erreur de configuration est une exception.
    explicit Monitor(Limits limits);

    // Prend une nouvelle mesure et retourne le nouvel état.
    // Attention : NE PAS appeler simultanément depuis plusieurs threads sans
    // synchronisation : state_ et recovery_count_ sont mutables.
    [[nodiscard]] Severity ingest(const Reading& reading) noexcept;

    // Accesseur sans effet de bord (attention aux accès concurrents éventuels).
    [[nodiscard]] Severity state() const noexcept { return state_; }

private:
    Limits limits_;                      // Configuration validée et figée.
    Severity state_{Severity::Normal};   // Valeur initiale pédagogique à revoir.
    unsigned recovery_count_{0};        // Maximum utile : 1 avant le 2e succès.
};

// Représentation textuelle pour les journaux / exemples.
[[nodiscard]] const char* to_string(Severity level) noexcept;

} // namespace monitor
