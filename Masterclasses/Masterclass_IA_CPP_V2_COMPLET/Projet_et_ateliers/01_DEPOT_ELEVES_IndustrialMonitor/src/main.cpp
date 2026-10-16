// Application de démonstration : ne commande aucun équipement physique.
// Elle illustre l'appel à l'API et la conversion du niveau en texte lisible.
#include "monitor/monitor.hpp"
#include <iostream>

int main() {
    // Les seuils par défaut proviennent de la spécification pédagogique.
    const monitor::Limits limits{};

    // Mesure nominale : 82,5 °C, reçue il y a 100 ms, capteur fonctionnel.
    const monitor::Reading reading{82.5, 100, true};

    // La classification instantanée ne doit pas conserver d'historique.
    const auto severity = monitor::classify(reading, limits);

    // Affichage uniquement ; aucune décision de commande robotique.
    std::cout << monitor::to_string(severity) << '\n';
}
