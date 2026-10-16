#pragma once

// API stable de départ : les exigences sont dans docs/requirements.md.
// L'objectif du TP n'est pas de modifier l'interface sans nécessité, mais de
// démontrer ses écarts par des tests, puis de concevoir l'extension requise.
namespace monitor {

// Un niveau Invalid n'est pas un état sans danger : son traitement doit
// être défini par le système appelant (hors du périmètre de cet exercice).
enum class Severity { Normal, Warning, Critical, Invalid };

// Les durées sont données en millisecondes, non en microsecondes.
struct Reading {
    double temperature_c{};  // Valeur numérique fournie par le capteur.
    int age_ms{};            // Âge au moment de la réception / du traitement.
    bool sensor_ok{true};    // Faux si le diagnostic capteur indique une panne.
};

// Deux seuils d'entrée et deux seuils de sortie permettent l'hystérésis.
// Attention : la présence de ces champs n'implique pas que l'algorithme
// de départ utilise l'hystérésis ; ce point sera vérifié dans l'atelier.
struct Limits {
    double warning_enter{60.0};
    double warning_exit{55.0};
    double critical_enter{80.0};
    double critical_exit{75.0};
    int max_age_ms{1000};
};

// Contrat attendu : classifier UNE mesure indépendamment des précédentes.
Severity classify(const Reading& reading, const Limits& limits) noexcept;

// Conversion pour affichage humain ; sans localisation ni effets de bord.
const char* to_string(Severity severity) noexcept;

} // namespace monitor
