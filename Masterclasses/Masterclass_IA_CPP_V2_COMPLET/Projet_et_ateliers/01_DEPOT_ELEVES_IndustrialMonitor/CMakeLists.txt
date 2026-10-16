# ============================================================================
# PROJET PEDAGOGIQUE : IndustrialMonitor (C++20)
# ============================================================================
# CMake orchestre la construction : il ne remplace ni un compilateur ni des tests.
# - Le code métier est compilé dans une bibliothèque réutilisable « monitor ».
# - monitor_demo : exécutable de démonstration.
# - monitor_tests : vérifications locales lancées avec CTest.
# - Les dépendances sont volontairement limitées à la bibliothèque standard C++.
# ============================================================================
cmake_minimum_required(VERSION 3.20)
project(IndustrialMonitor VERSION 1.0.0 LANGUAGES CXX)

# Le standard C++20 est requis : aucune extension spécifique à GCC/Clang.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export utile pour clang-tidy et divers outils d'analyse statique.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# La bibliothèque expose les entêtes via include/.
add_library(monitor STATIC src/monitor.cpp)
target_include_directories(monitor PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

# Les avertissements du compilateur aident à relever les défauts ;
# ils ne constituent pas une preuve de conformité fonctionnelle.
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
  target_compile_options(monitor PRIVATE
    -Wall -Wextra -Wpedantic -Wconversion -Wshadow)
endif()

add_executable(monitor_demo src/main.cpp)
target_link_libraries(monitor_demo PRIVATE monitor)

# Activer CTest et y déclarer les tests, pour un lancement unique via « ctest ».
enable_testing()
add_executable(monitor_tests tests/test_monitor.cpp)
target_link_libraries(monitor_tests PRIVATE monitor)
add_test(NAME monitor_unit_tests COMMAND monitor_tests)
